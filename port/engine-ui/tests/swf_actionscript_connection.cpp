// Real original shared/HUD resources and retained facade. Native game services
// and texture upload below are explicit host fixtures, not a settings/potion
// implementation or a claim of complete ActionScript fork equivalence.
#define main historical_movie_fixture_main
#include "swf_movie.cpp"
#undef main
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_text.h"
#include "../renderfx_text_connection.hpp"
#include <stdexcept>
#include <limits>
namespace {
#define require(ok,why) do {if(!(ok))throw std::runtime_error(std::string(why)+" at line "+std::to_string(__LINE__));} while(false)
unsigned watch_calls=0;
unsigned property_gets=0,property_sets=0;
void getter(const gameswf::fn_call& fn){++property_gets;fn.result->set_double(11);}
void setter(const gameswf::fn_call& fn){require(fn.nargs==1&&fn.arg(0).to_number()==9,"Property setter argument changed");++property_sets;}
void watcher(const gameswf::fn_call& fn){
 require(fn.nargs==4&&std::string(fn.arg(0).to_string())=="BridgeWatched","Watcher source argument order lost");
 ++watch_calls;fn.result->set_double(fn.arg(2).to_number()+10);
}
struct Connected:Test {
 unsigned startup_settings{},startup_multiplayer{},typed{},potions{},guards{},classes{},methods{},member_ops{},typed_arguments{},startup_hooks{};
 SwfAsGraph* current{};bool release_on_call=false,reject_settings=false,reject_hook=false;
 static bool graph_start(void* c,const SwfAsLease& lease,std::string& error){
  auto& t=*static_cast<Connected*>(c);
  require(lease.owner&&lease.player&&!lease.root,"Startup hook did not borrow the exact unattached graph");
  ++t.startup_hooks;
  if(t.reject_hook){error="Required graph construction observer rejected startup";return false;}
  return true;
 }
 static bool native_as(void* c,const char* name,const gameswf::fn_call& fn,std::string& error){
  auto& t=*static_cast<Connected*>(c);const std::string n=name;
  if(n=="NativeLoadSettings"){
   ++t.startup_settings;
   if(t.reject_settings){error="Owned settings backend deliberately unavailable";return false;}
   return true; // Controlled wrapper delivery only, explicitly not a file load.
  }
  if(n=="NativeIsMultiplayerEnabled"){
   ++t.startup_multiplayer;fn.result->set_bool(false);return true; // controlled capability fixture
  }
  if(n=="NativeUsePotion"){
   require(fn.nargs==1&&fn.arg(0).to_number()==0,"Authored potion handler argument changed");++t.potions;return true;
  }
  if(n=="NativeBridgeReject"){error="Required connected callback rejected delivery";return false;}
  if(n=="NativeBridgeInspect"){
   require(fn.nargs==7,"Typed callback argument count lost");
   require(fn.arg(0).is_object()&&fn.arg(0).to_object(),"Native callback flattened passed object");
   require(fn.arg(1).is_bool()&&fn.arg(1).to_bool(),"Boolean tag/order lost");
   require(fn.arg(2).is_string()&&std::string(fn.arg(2).to_string())=="2.75","String tag/order lost");
   require(fn.arg(3).is_number()&&fn.arg(3).to_number()==-1.25,"Number tag/order lost");
   require(fn.arg(4).is_undefined()&&fn.arg(5).is_null()&&std::isnan(fn.arg(6).to_number()),"Undefined/null/NaN tags lost");
   fn.arg(0).to_object()->set_member("BridgeResult",gameswf::as_value(42));
   fn.result->set_as_object(fn.arg(0).to_object());++t.typed;t.typed_arguments+=7;
   std::string reentry;require(!t.movie->advance(0,reentry)&&reentry=="SWF core busy","Native callback escaped core reentry guard");++t.guards;
   if(t.release_on_call)t.current->release();
   return true;
  }
  error="Unexpected host callback: "+n;return false;
 }
};
struct Batch {
 Connected* t;SwfAsValue* escaped;SwfAsValue foreign;bool foreign_test=false,release_test=false;SwfAsValue* property_escape{};
 static bool apply(void* c,SwfAsGraph& as,std::string& error){auto& b=*static_cast<Batch*>(c);auto& t=*b.t;t.current=&as;
  SwfAsValue root,global,menu,hudinfo,value,result;bool found=false,accepted=false,callable=false;
  require(as.root_value(root,error)&&as.global_value(global,error),error.c_str());
  if(b.foreign_test){
   require(!as.set_member(b.foreign,"BridgeResult",SwfAsValue::number(99),accepted,error)&&error=="AS object belongs to a different retained movie","Foreign graph handle accepted");++t.guards;
   gameswf::as_object* raw=nullptr;require(!as.borrow_object(b.foreign,raw,error),"Foreign graph raw borrow accepted");++t.guards;error.clear();return true;
  }
  require(as.find_target(root,"_root.menu_HUD_0",menu,error),error.c_str());
  require(as.get_member(menu,"hudInfos",hudinfo,found,error)&&found&&hudinfo.kind()==SwfAsValue::Kind::object,"Original HUD information object missing");
  if(b.release_test){
   t.release_on_call=true;
   require(as.invoke(root,root,"NativeBridgeInspect",{hudinfo,SwfAsValue::boolean(true),SwfAsValue::text("2.75"),SwfAsValue::number(-1.25),{},SwfAsValue::null(),SwfAsValue::number(std::numeric_limits<double>::quiet_NaN())},result,callable,error)&&callable,error.c_str());
   require(result.identity()==hudinfo.identity(),"Callback release invalidated native result object");
   require(!as.get_member(result,"BridgeResult",value,found,error),"Released graph still allowed operations");++t.guards;error.clear();return true;
  }
  const char* packages[]={"com.gameloft.components.ui","com.gameloft.dungeonhunter.menus.HUD","com.gameloft.input","as.hud"};
  for(const char* path:packages){require(as.find_target(global,path,value,error)&&value.identity(),"Authored class package absent from actual global namespace");++t.classes;}
  const char* methods[]={"onLoad","onPush","onShow","applyElementsEventsHandlers","setSkillsButtons","DisplayRightHud","onHide","onPop","AlliesBarDisplay","EnemyBarDisplay"};
  for(const char* name:methods){
   require(as.get_member(menu,name,value,found,error)&&found&&value.identity(),"Authored menu method/prototype unavailable");
   gameswf::as_object* raw=nullptr;require(as.borrow_object(value,raw,error)&&raw->is(gameswf::as_function::m_class_id),"Method is not the actual AS function");++t.methods;
  }
  gameswf::as_object* raw=nullptr;require(as.borrow_object(hudinfo,raw,error)&&raw,"Actual HUD object borrow failed");
  require(as.retain_object(raw,value,error)&&value.identity()==hudinfo.identity(),"Source object identity lost on retention");
  gameswf::as_value readonly(7);readonly.set_flags(gameswf::as_value::READ_ONLY);raw->m_members.set("BridgeReadonly",readonly);
  require(as.set_member(hudinfo,"BridgeReadonly",SwfAsValue::number(88),accepted,error),error.c_str());
  require(as.get_member(hudinfo,"BridgeReadonly",value,found,error)&&found,error.c_str());double number=0;
  require(as.to_number(value,number,error)&&number==7,"Read-only member mutated by bridge");t.member_ops+=2;
  gameswf::as_value w(watcher);require(raw->watch("BridgeWatched",w.to_function(),gameswf::as_value()),"Watcher install failed");
  const auto prior_watch=watch_calls;
  require(as.set_member(hudinfo,"BridgeWatched",SwfAsValue::number(5),accepted,error)&&as.get_member(hudinfo,"BridgeWatched",value,found,error)&&as.to_number(value,number,error)&&number==15&&watch_calls==prior_watch+1,"Actual setter/watch result lost");t.member_ops+=2;
  require(raw->unwatch("BridgeWatched"),"Watcher removal failed");
  raw->builtin_member("BridgeProperty",gameswf::as_value(gameswf::as_value(getter),gameswf::as_value(setter)));
  require(as.get_member(hudinfo,"BridgeProperty",value,found,error)&&found&&value.kind()==SwfAsValue::Kind::property,"Property tag lost");
  const auto before_get=property_gets;require(value.identity()==0&&property_gets==before_get,"Property identity executed getter outside an operation");
  if(b.property_escape)*b.property_escape=value;
  require(as.to_number(value,number,error)&&number==11&&property_gets==before_get+1,"Actual property getter not called");
  require(as.set_member(hudinfo,"BridgeProperty",SwfAsValue::number(9),accepted,error)&&property_sets==watch_calls,"Actual property setter not called");t.member_ops+=2;
  gameswf::as_object* root_object=nullptr;require(as.borrow_object(root,root_object,error),error.c_str());
  require(raw->get_member("BridgeProperty",&root_object->get_environment()->m_global_register[0]),"Bound property register fixture missing");
  require(as.get_member(hudinfo,"BridgeAbsent",value,found,error)&&!found&&value.kind()==SwfAsValue::Kind::undefined,"Missing member invented a value");++t.member_ops;
  require(as.to_number(SwfAsValue::text("2.75"),number,error)&&number==2.75,"Actual AS conversion bypassed");
  require(as.invoke(root,root,"NativeBridgeInspect",{hudinfo,SwfAsValue::boolean(true),SwfAsValue::text("2.75"),SwfAsValue::number(-1.25),{},SwfAsValue::null(),SwfAsValue::number(std::numeric_limits<double>::quiet_NaN())},result,callable,error)&&callable,error.c_str());
  require(result.identity()==hudinfo.identity(),"Native result copied instead of retaining same AS object");
  require(as.get_member(result,"BridgeResult",value,found,error)&&as.to_number(value,number,error)&&number==42,"Native mutation did not reach retained AS object");++t.member_ops;
  require(as.invoke(root,root,"MissingBridgeMethod",{},result,callable,error)&&!callable&&result.kind()==SwfAsValue::Kind::undefined,"Absent AS method treated as successful game callback");++t.guards;
  *b.escaped=hudinfo;return true;
 }
 static bool text(void*,SwfAsGraph& as,std::string& e){
  SwfAsValue root,field,menu,value;bool found=false;std::string bound;
  require(as.root_value(root,e)&&as.find_target(root,"_root.menu_HUD_0",menu,e)&&as.find_target(menu,"HUDelements.HealthBars.btn_potion.cnt.value",field,e),e.c_str());
  gameswf::as_object* raw=nullptr;require(as.borrow_object(field,raw,e)&&raw&&raw->is(gameswf::edit_text_character::m_class_id),"Authored potion text receiver missing");
  auto* f=static_cast<gameswf::edit_text_character*>(raw);const auto old_html=f->m_def->m_html;const auto old_maximum=f->m_def->m_max_length;const tu_string old_variable=f->m_def->m_var_name;
  f->m_def->m_html=true;f->m_def->m_max_length=3;f->m_def->m_var_name="BridgeBoundText";
  require(renderfx_set_plain_text(as,field,"<b>57</b>",false,e),e.c_str());
  require(f->m_text==tu_string("<b>")&&f->m_def->m_html,"Plain text was parsed or shared definition mutated");
  SwfAsValue parent;require(as.retain_object(f->get_parent(),parent,e)&&as.get_member(parent,"BridgeBoundText",value,found,e)&&found&&as.to_text(value,bound,e)&&bound=="<b>57</b>","Bound variable did not receive source untruncated text");
  const tu_string before=f->m_text;
  require(!renderfx_set_plain_text(as,field,"88",true,e)&&f->m_text==before,"Missing HTML backend mutated text as if supported");
  require(renderfx_set_plain_text(as,menu,nullptr,true,e),"Non-text original type gate not honored");
  f->m_def->m_html=old_html;f->m_def->m_max_length=old_maximum;f->m_def->m_var_name=old_variable;
  return renderfx_set_plain_text(as,field,"7",false,e);
 }
 static bool reject(void* c,SwfAsGraph& as,std::string& e){auto& t=*static_cast<Connected*>(c);SwfAsValue root,out;bool callable=false;
  require(as.root_value(root,e)&&as.invoke(root,root,"NativeBridgeReject",{},out,callable,e)&&callable,"Failure callback never reached");++t.guards;return true;
 }
 static bool potion(void* c,SwfAsGraph& as,std::string& e){auto& t=*static_cast<Connected*>(c);SwfAsValue root,menu,button,fn,out;bool callable=false,found=false;
  require(as.root_value(root,e)&&as.find_target(root,"_root.menu_HUD_0",menu,e)&&as.find_target(menu,"HUDelements.HealthBars.btn_potion",button,e),e.c_str());
  require(as.get_member(button,"onRelease",fn,found,e)&&found,"Original potion handler not installed");
  require(as.invoke(button,button,"onRelease",{},out,callable,e)&&callable,e.c_str());require(t.potions==1,"Authored potion handler did not call native service");return true;
 }
 static bool released_dispatch(void* c,SwfAsGraph& as,std::string& e){auto& t=*static_cast<Connected*>(c);
  SwfAsValue root,global,method;bool found=false;gameswf::as_object* raw_root=nullptr;gameswf::as_object* raw_method=nullptr;
  require(as.root_value(root,e)&&as.global_value(global,e)&&as.get_member(global,"NativeBridgeReject",method,found,e)&&found&&as.borrow_object(root,raw_root,e)&&as.borrow_object(method,raw_method,e),e.c_str());
  gameswf::gc_ptr<gameswf::as_function> function=static_cast<gameswf::as_function*>(raw_method);
  auto* environment=raw_root->get_environment();root={};global={};method={};
  as.release();gameswf::as_value result;
  (*function)(gameswf::fn_call(&result,raw_root,environment,0,environment->get_top_index()));++t.guards;return true;
 }
};
}
int main(int argc,char** argv){try{
 if(argc!=2)return 2;SwfAsValue escaped,escaped_property;std::weak_ptr<Connected> provider_lifetime;
 unsigned settings=0,multiplayer=0,classes=0,methods=0,guards=0,typed=0,members=0,potions=0,arguments=0,hooks=0;
 {
  auto t=std::make_shared<Connected>();t->base=argv[1];provider_lifetime=t;
  auto services=t->services();services.native_owner=t;services.native_action=Connected::native_as;
  services.graph_start=Connected::graph_start;
  services.native_actions={"NativeLoadSettings","NativeIsMultiplayerEnabled","NativeUsePotion","NativeBridgeInspect","NativeBridgeReject"};
  SwfMovie movie;t->movie=&movie;std::string error;
  require(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",services,error)&&movie.advance(0,error),error.c_str());
  require(t->startup_hooks==1&&t->startup_settings==1&&t->startup_multiplayer==1,"Original shared startup did not reach registered callbacks exactly once");
  Batch b{t.get(),&escaped};b.property_escape=&escaped_property;require(movie.action_script(&b,Batch::apply,error),error.c_str());
  auto* graph=t->current;SwfAsValue outside;bool found=false;
  require(!graph->get_member(escaped,"BridgeResult",outside,found,error)&&error=="Required retained AS graph Scope unavailable","Core operation ran outside movie Scope");++t->guards;
  require(movie.action_script(t.get(),Batch::potion,error),error.c_str());
  require(movie.action_script(t.get(),Batch::text,error),error.c_str());
  require(!movie.action_script(t.get(),Batch::reject,error)&&error=="Required connected callback rejected delivery","Callback failure discarded by facade");
  auto bad=services;bad.native_owner.reset();require(!movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",bad,error),"Unowned callback provider accepted");++t->guards;
  bad=services;bad.native_actions.push_back("NativeBridgeInspect");require(!movie.load({},"dqhud_droid.swf",bad,error),"Duplicate native registration accepted");++t->guards;
  t->reject_settings=true;require(!movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",services,error)&&error=="Owned settings backend deliberately unavailable","Missing settings backend silently treated as successful startup");++t->guards;t->reject_settings=false;
  const auto startup=t->startup_settings;const auto export_reads=t->exports.size();t->reject_hook=true;
  require(!movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",services,error)&&error=="Required graph construction observer rejected startup"&&t->startup_settings==startup&&t->exports.size()==export_reads,"Failed observer still loaded authored resources");++t->guards;t->reject_hook=false;
  // Failed replacement leaves the original graph and held object intact.
  require(movie.action_script(&b,Batch::apply,error),error.c_str());
  SwfMovie other;auto unbound=t->services();require(other.load({"dqshared_droid.swf"},"dqhud_droid.swf",unbound,error)&&other.advance(0,error),error.c_str());
  Batch foreign{t.get(),&escaped,escaped,true};require(other.action_script(&foreign,Batch::apply,error),error.c_str());
  const auto reads=property_gets;foreign.foreign=escaped_property;
  require(other.action_script(&foreign,Batch::apply,error)&&property_gets==reads,"Foreign property validation executed original graph getter");
  SwfMovie detached;require(detached.load({"dqshared_droid.swf"},"dqhud_droid.swf",services,error),error.c_str());
  require(!detached.action_script(t.get(),Batch::released_dispatch,error)&&error=="Required native AS graph released: NativeBridgeReject","Expired callback graph silently reported successful dispatch");
  Batch release{t.get(),&escaped,{},false,true};require(movie.action_script(&release,Batch::apply,error),error.c_str());
  settings=t->startup_settings;multiplayer=t->startup_multiplayer;classes=t->classes;methods=t->methods;guards=t->guards;typed=t->typed;members=t->member_ops;potions=t->potions;arguments=t->typed_arguments;hooks=t->startup_hooks;
 }
 require(!provider_lifetime.expired()&&escaped.identity(),"Retained value did not keep exact graph/provider alive after movie destruction");
 const auto reads=property_gets;require(escaped_property.identity()==0&&property_gets==reads,"Property getter executed outside core Scope");
 escaped_property={};escaped={};require(provider_lifetime.expired(),"Retained graph/provider leaked after final value release");
 require(property_gets==reads,"Quiescent graph teardown executed property getter");
 require(hooks==4,"Expected pre-construction hooks/rejected startup were not delivered");
 std::cout<<"{\"validation\":\"PASS\",\"startup_settings\":"<<settings<<",\"startup_multiplayer\":"<<multiplayer<<",\"global_packages\":"<<classes<<",\"original_menu_methods\":"<<methods<<",\"member_operations\":"<<members<<",\"typed_callbacks\":"<<typed<<",\"typed_arguments\":"<<arguments<<",\"authored_potion_callbacks\":"<<potions<<",\"guards\":"<<guards<<",\"provider_lifetime_checks\":2,\"limits\":{\"native_game_services_are_host_fixtures\":true,\"texture_uploads_are_host_fixtures\":true,\"full_actions_fork_parity\":false}}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
