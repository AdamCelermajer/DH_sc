// Transport test uses real original SWF/AS objects. Gameplay, localization,
// settings and GPU callbacks here are labelled fixtures, not live menu proof.
#define main historical_movie_fixture_main
#pragma GCC diagnostic push
// The included historical main relies on main's implicit return 0. Its renamed
// body is never called; reuse only its labelled resource/provider fixtures.
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "swf_movie.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_menu_as_bridge_v1.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <stdexcept>
#include <cstring>
namespace {
void require(bool ok,const std::string& message,int line=__builtin_LINE()){
 if(!ok)throw std::runtime_error(message+" at "+std::to_string(line));
}
unsigned gets=0,watches=0,nested=0;
void property_getter(const gameswf::fn_call& fn){++gets;fn.result->set_double(11);}
void property_setter(const gameswf::fn_call& fn){fn.result->set_undefined();}
CharacterMenuAsBridgeV1* active_bridge=nullptr;
void watcher(const gameswf::fn_call& fn){
 ++watches;std::string error;
 require(active_bridge&&active_bridge->dispatch("NestedWatcher",fn,error),error);
}
struct Connected:Test {
 unsigned calls{},writes{},appends{},created{},conversions{},guards{};
 std::shared_ptr<int> game=std::make_shared<int>(42);
 std::unique_ptr<CharacterMenuAsBridgeV1> bridge;
 Connected(){bridge=std::make_unique<CharacterMenuAsBridgeV1>(game,[this](const char* name,CharacterMenuCallV1& call,std::string& error){return query(name,call,error);});}
 bool query(const char* name,CharacterMenuCallV1& call,std::string& error){
  ++calls;const std::string callback=name;
  if(callback=="NestedWatcher"){
   require(call.arguments.size()==4&&call.arguments[0].kind==4&&call.arguments[0].text=="BridgeWatched","Watcher original arguments lost");
   double n;require(call.number(call.arguments[2],n,error),error);
   call.result.kind=2;call.result.number=n+10;++nested;return true;
  }
  if(callback=="NativeMenuProperty"){
   require(call.arguments.size()==1&&call.arguments[0].kind==6&&gets==0,"Bound property copied/coerced before source operation");
   double n;require(call.number(call.arguments[0],n,error)&&n==11&&gets==1,"Reached original property getter not delivered once");
   ++conversions;call.result.kind=2;call.result.number=n;return true;
  }
  if(callback=="NativeMenuPreserveProperty"){
   require(call.arguments.empty()&&call.result.kind==6&&gets==0,"Preexisting result getter executed during projection");return true;
  }
  if(callback=="NativeMenuBadHandle"){
   call.result.kind=5;call.result.object=0x1234;return true;
  }
  if(callback=="NativeMenuWrongArray")return call.array(call.arguments[0].object,error);
  if(callback=="NativeMenuCreatedPrefixFailure"){
   std::uintptr_t identity;require(call.create_object(identity,error),error);++created;
   CharacterMenuValueV1 member;member.kind=4;member.text="Retained prefix";
   require(call.member(identity,"ItemName",member,error),error);++writes;
   CharacterMenuValueV1 object;object.kind=5;object.object=identity;
   require(call.append(call.arguments[0].object,object,error),error);++appends;
   error="Deliberate row-provider failure";return false;
  }
  if(callback=="NativeMenuPrefixFailure"){
   CharacterMenuValueV1 n;n.kind=2;n.number=17;
   require(call.member(call.arguments[0].object,"FailurePrefix",n,error),error);++writes;
   call.result=n;error="Deliberate required game-provider failure";return false;
  }
  require(callback=="NativeMenuTransport"&&call.arguments.size()==7,"Wrong transport callback");
  require(call.arguments[0].kind==5&&call.arguments[1].kind==1&&call.arguments[1].boolean&&call.arguments[2].kind==4&&call.arguments[3].kind==2&&std::isnan(call.arguments[3].number)&&call.arguments[4].kind==5&&call.arguments[4].object==0&&call.arguments[5].kind==0&&call.arguments[6].kind==5,"Actual AS kinds/order/null/NaN lost");
  CharacterMenuValueV1 value;value.kind=2;value.number=5;
  require(call.member(call.arguments[0].object,"BridgeWatched",value,error),error);++writes;
  value.number=88;require(call.member(call.arguments[0].object,"BridgeReadonly",value,error),error);++writes;
  require(call.array(call.arguments[6].object,error),error);
  for(unsigned i=0;i<3;++i){value.number=i;require(call.append(call.arguments[6].object,value,error),error);++appends;}
  for(unsigned i=0;i<3;++i){
   std::uintptr_t identity;require(call.create_object(identity,error),error);++created;
   value.kind=4;value.text="Original item row "+std::to_string(i);
   require(call.member(identity,"ItemName",value,error),error);++writes;
   value.kind=2;value.number=i;
   require(call.member(identity,"ItemIndex",value,error),error);++writes;
   value.kind=5;value.object=identity;
   require(call.append(call.arguments[6].object,value,error),error);++appends;
  }
  double number;bool flag;
  require(call.number(call.arguments[2],number,error)&&number==2.75,"Original string-number conversion lost");++conversions;
  require(call.number(call.arguments[4],number,error)&&number==0,"Original null conversion lost");++conversions;
  require(call.number(call.arguments[5],number,error)&&std::isnan(number),"Original undefined conversion lost");++conversions;
  require(call.boolean(call.arguments[0],flag,error)&&flag,"Original object boolean conversion lost");++conversions;
  call.result=call.arguments[0];return true;
 }
 static bool native(void* p,const char* name,const gameswf::fn_call& fn,std::string& error){
  auto& self=*static_cast<Connected*>(p);
  if(!std::strcmp(name,"NativeLoadSettings"))return true; // explicit startup fixture
  if(!std::strcmp(name,"NativeIsMultiplayerEnabled")){fn.result->set_bool(false);return true;}
  return self.bridge->dispatch(name,fn,error);
 }
};
struct Batch {
 Connected& test;unsigned mode=0;
 static bool apply(void* p,SwfAsGraph& graph,std::string& error){
  auto& b=*static_cast<Batch*>(p);auto& t=b.test;SwfAsValue root,global,info,result,value;bool found=false,callable=false;
  require(graph.root_value(root,error)&&graph.global_value(global,error)&&graph.find_target(root,"_root.menu_HUD_0",info,error),error);
  gameswf::as_object* receiver=nullptr;gameswf::as_object* root_object=nullptr;
  require(graph.borrow_object(info,receiver,error)&&receiver&&graph.borrow_object(root,root_object,error),error);
  if(b.mode==1)return graph.invoke(root,root,"NativeMenuPrefixFailure",{info},result,callable,error);
  if(b.mode==2){
   double n;require(graph.get_member(info,"FailurePrefix",value,found,error)&&found&&graph.to_number(value,n,error)&&n==17,"Native failure discarded completed AS writes");
   return true;
  }
  if(b.mode==3){
   require(graph.invoke(root,root,"NativeMenuBadHandle",{},result,callable,error)&&callable,error);return true;
  }
  if(b.mode==4){require(graph.invoke(root,root,"NativeMenuWrongArray",{info},result,callable,error)&&callable,error);return true;}
  if(b.mode==5){
   require(graph.get_member(info,"BridgeRows",value,found,error)&&found,error);
   return graph.invoke(root,root,"NativeMenuCreatedPrefixFailure",{value},result,callable,error);
  }
  if(b.mode==6){
   require(graph.get_member(info,"BridgeRows",value,found,error)&&found,error);
   gameswf::as_object* retained=nullptr;require(graph.borrow_object(value,retained,error)&&retained->is(gameswf::as_array::m_class_id),error);
   auto* rows=static_cast<gameswf::as_array*>(retained);require(rows->size()==8,"Provider failure dropped completed row");
   gameswf::as_value field;require(rows->m_array[7].to_object()->get_member("ItemName",&field)&&std::string(field.to_string())=="Retained prefix","Created row disappeared after native frame release");
   return true;
  }
  gameswf::gc_ptr<gameswf::as_array> array=new gameswf::as_array(receiver->get_player());array->push(gameswf::as_value(99));
  SwfAsValue array_value;require(graph.retain_object(array.get_ptr(),array_value,error),error);
  gameswf::as_value readonly(7);readonly.set_flags(gameswf::as_value::READ_ONLY);receiver->m_members.set("BridgeReadonly",readonly);
  gameswf::as_value watch(watcher);require(receiver->watch("BridgeWatched",watch.to_function(),gameswf::as_value()),"Actual AS watcher installation failed");
  active_bridge=t.bridge.get();
  require(graph.invoke(root,root,"NativeMenuTransport",{info,SwfAsValue::boolean(true),SwfAsValue::text("2.75"),SwfAsValue::number(std::nan("")),SwfAsValue::null(),{},array_value},result,callable,error)&&callable,error);
  require(result.identity()==info.identity()&&watches==1&&nested==1,"Original receiver/result/watch identity lost");
  double n;require(graph.get_member(info,"BridgeWatched",value,found,error)&&graph.to_number(value,n,error)&&n==15,"Nested watcher effects lost");
  require(graph.get_member(info,"BridgeReadonly",value,found,error)&&graph.to_number(value,n,error)&&n==7,"Read-only source setter was replaced");
  require(array->size()==7&&array->m_array[0].to_number()==99&&array->m_array[3].to_number()==2,"Array append cleared/replaced original entries");
  for(unsigned i=0;i<3;++i){
   gameswf::as_value field;auto* row=array->m_array[4+i].to_object();
   require(row&&row->get_player()==receiver->get_player()&&row->get_member("ItemIndex",&field)&&field.to_number()==i,"Created same-player item object not retained by actual array");
  }
  bool accepted=false;require(graph.set_member(info,"BridgeRows",array_value,accepted,error)&&accepted,error);
  require(receiver->unwatch("BridgeWatched"),"Watcher removal failed");active_bridge=nullptr;
  receiver->builtin_member("BridgeProperty",gameswf::as_value(gameswf::as_value(property_getter),gameswf::as_value(property_setter)));
  // Direct borrowed argument slots avoid as_value's property-copy getter.
  // This is still the real retained graph and environment in its core Scope.
  auto* env=root_object->get_environment();const auto size=env->get_stack_size();env->push(gameswf::as_value());
  require(receiver->get_member("BridgeProperty",&env->bottom(env->get_top_index())),"Source bound property unavailable");
  require(gets==0,"Obtaining property argument executed getter");
  gameswf::as_value property_result;
  require(t.bridge->dispatch("NativeMenuProperty",gameswf::fn_call(&property_result,root_object,env,1,env->get_top_index()),error)&&property_result.to_number()==11,error);
  env->set_stack_size(size);gets=0;
  require(receiver->get_member("BridgeProperty",&property_result),"Property result unavailable");
  require(t.bridge->dispatch("NativeMenuPreserveProperty",gameswf::fn_call(&property_result,root_object,env,0,env->get_top_index()),error)&&gets==0&&property_result.is_property(),"Unchanged result coerced/clobbered");
  auto* foreign=new gameswf::player;gameswf::gc_ptr<gameswf::player> foreign_pin=foreign;
  env->push(gameswf::as_value(foreign->get_global()));gameswf::as_value rejected;
  require(!t.bridge->dispatch("NativeMenuWrongArray",gameswf::fn_call(&rejected,root_object,env,1,env->get_top_index()),error)&&error.find("another player")!=std::string::npos,"Foreign object was treated as a local receiver");++t.guards;
  env->set_stack_size(size);
  require(!t.bridge->dispatch("NativeMenuTransport",gameswf::fn_call(nullptr,root_object,env,0,env->get_top_index()),error),"Null result accepted");++t.guards;
  error.clear();return true;
 }
};
}
int main(int argc,char** argv){try{
 require(argc==2,"Usage: character-menu-as-bridge original-menu-directory");
 std::weak_ptr<int> game;
 unsigned calls=0,writes=0,appends=0,created=0,conversions=0,guards=0;
 {
  auto test=std::make_shared<Connected>();test->base=argv[1];game=test->game;
  auto services=test->services();services.native_owner=test;services.native_action=Connected::native;
  services.native_actions={"NativeLoadSettings","NativeIsMultiplayerEnabled","NativeMenuTransport","NativeMenuPrefixFailure","NativeMenuBadHandle","NativeMenuWrongArray","NativeMenuCreatedPrefixFailure"};
  SwfMovie movie;test->movie=&movie;std::string error;
  require(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",services,error),error);
  Batch batch{*test};require(movie.action_script(&batch,Batch::apply,error),error);
  batch.mode=1;require(!movie.action_script(&batch,Batch::apply,error)&&error=="Deliberate required game-provider failure","Facade hid required native failure");++test->guards;
  batch.mode=2;require(movie.action_script(&batch,Batch::apply,error),error);
  batch.mode=3;require(!movie.action_script(&batch,Batch::apply,error)&&error.find("not borrowed")!=std::string::npos,"Unborrowed result handle accepted");++test->guards;
  batch.mode=4;require(!movie.action_script(&batch,Batch::apply,error)&&error.find("actual AS array")!=std::string::npos,"Plain object accepted as Array");++test->guards;
  batch.mode=5;require(!movie.action_script(&batch,Batch::apply,error)&&error=="Deliberate row-provider failure","Facade hid required item row failure");++test->guards;
  batch.mode=6;require(movie.action_script(&batch,Batch::apply,error),error);
  test->game.reset();require(!game.expired(),"Bridge dropped retained gameplay authority");
  calls=test->calls;writes=test->writes;appends=test->appends;created=test->created;conversions=test->conversions;guards=test->guards;
 }
 require(game.expired(),"AS bridge/provider formed a retained owner cycle");
 std::cout<<"{\"validation\":\"PASS\",\"typed_callbacks\":"<<calls<<",\"real_member_writes\":"<<writes<<",\"retained_array_appends\":"<<appends<<",\"retained_created_objects\":"<<created<<",\"source_conversions\":"<<conversions<<",\"nested_watchers\":"<<nested<<",\"guards\":"<<guards<<",\"property_getter_checks\":2,\"source_receiver_identity\":true,\"limits\":{\"gameplay_callbacks_fixture\":true,\"localization_fixture\":true,\"GPU_fixture\":true,\"whole_character_menu_live\":false}}\n";
 return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
