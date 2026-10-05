#pragma once
#include "../character_menu_as_bridge_v1.hpp"
#include "../swf_movie.hpp"
#include "gameswf/gameswf_function.h"
#include "gameswf/gameswf_as_classes/as_array.h"
#include <fstream>
#include <iterator>
#include <algorithm>

// Actual AS transport over a retained original movie. GPU uploads, settings,
// startup localization and the enclosing offline World remain labelled fixtures.
// The menu queries/actions, property sheets, equipment and scripts are real.
struct ConnectedMenuASHostV1 {
 struct Provider {
  ConnectedMenuASHostV1* host;
  dh2::ui::CharacterMenuQueriesOwnerV1* queries;
  std::string directory;
  std::unique_ptr<dh2::ui::CharacterMenuAsBridgeV1> bridge;
  Provider(ConnectedMenuASHostV1& h,dh2::ui::CharacterMenuQueriesOwnerV1& q,
           std::shared_ptr<void> game,const std::string& menus):host(&h),queries(&q),directory(menus){
   bridge=std::make_unique<dh2::ui::CharacterMenuAsBridgeV1>(std::move(game),[this](const char* name,dh2::ui::CharacterMenuCallV1& call,std::string& error){
    const auto real_member=call.member;
    call.member=[this,real_member](auto identity,const char* member,const auto& value,auto& failure){
     if(!real_member(identity,member,value,failure))return false;
     if(identity==host->object)host->members.emplace_back(member,value);
     return true;
    };
    return queries->dispatch(name,call,error);
   });
  }
  static bool read(void* p,const char* uri,std::vector<std::uint8_t>& bytes,std::string& error){
   std::ifstream input(static_cast<Provider*>(p)->directory+"/"+uri,std::ios::binary);
   if(!input){error="Original menu resource unavailable";return false;}
   bytes.assign(std::istreambuf_iterator<char>(input),{});return true;
  }
  static bool texture(void*,const char*,int width,int height,dh2::ui::SwfTexture& out,std::string&){
   out.identity=1;out.width=width?width:1024;out.height=height?height:1024;return true;
  }
  static bool image(void*,int width,int height,unsigned,const std::uint8_t*,int,dh2::ui::SwfTexture& out,std::string&){
   out.identity=2;out.width=width;out.height=height;return true;
  }
  static bool draw(void*,const dh2::ui::SwfDraw&,std::string&){return true;}
  static bool stencil(void*,const float*,std::uint8_t,bool& result,std::string&){result=false;return true;}
  static bool localization(void*,const char* name,const std::vector<dh2::ui::SwfValue>& args,dh2::ui::SwfValue& result,std::string& error){
   if(std::string(name)!="NativeGetStringFromSymbol"){error="Unowned legacy menu callback";return false;}
   result.kind=dh2::ui::SwfValue::text;result.string=args.empty()?"":args[0].string;return true;
  }
  static bool native(void* p,const char* name,const gameswf::fn_call& fn,std::string& error){
   auto& provider=*static_cast<Provider*>(p);
   if(std::string(name)=="NativeLoadSettings")return true; // explicit startup fixture
   if(std::string(name)=="NativeIsMultiplayerEnabled"){fn.result->set_bool(false);return true;}
   return provider.bridge->dispatch(name,fn,error);
  }
 };
 std::uintptr_t object{},array{};
 std::vector<std::pair<std::string,dh2::ui::CharacterMenuValueV1>> members;
 std::vector<dh2::ui::CharacterMenuValueV1> elements;
 unsigned callbacks=0;
 // Release movie/graph handles while borrowed callback state is still alive.
 // Provider owns no graph handle, so the facade and pins cannot form a cycle.
 std::shared_ptr<Provider> provider;
 dh2::ui::SwfAsValue root_value,object_value,array_value;
 dh2::ui::SwfMovie movie;
 ConnectedMenuASHostV1(dh2::ui::CharacterMenuQueriesOwnerV1& queries,
                      std::shared_ptr<void> game,const std::string& directory){
  provider=std::make_shared<Provider>(*this,queries,std::move(game),directory);
  dh2::ui::SwfServices services;services.context=provider.get();services.read=Provider::read;
  services.texture=Provider::texture;services.image=Provider::image;services.draw=Provider::draw;
  services.stencil=Provider::stencil;services.native_call=Provider::localization;
  services.native_owner=provider;services.native_action=Provider::native;
  services.native_actions={"NativeLoadSettings","NativeIsMultiplayerEnabled","NativeGetPlayerStats",
   "NativeInvGetItemDetails","NativeInvGetEquipedItem","NativeInvGetHasOffHandWeapon",
   "NativeInvGetHasTwoHandedWeapon","NativeGetSkillDetails","NativeSkillsTrainSkill",
   "NativeEquipSkill","NativeSkillGetEquipedSkillsIDs","NativeStatsAssignPoint",
   "NativeInvUnequipItem","NativeInvEquipItem","NativeSwapEquipment"};
  std::string error;
  check(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",services,error),error);
  check(movie.action_script(this,[](void* p,dh2::ui::SwfAsGraph& graph,std::string& failure){
   auto& host=*static_cast<ConnectedMenuASHostV1*>(p);gameswf::as_object* root=nullptr;
   if(!graph.root_value(host.root_value,failure)||!graph.borrow_object(host.root_value,root,failure))return false;
   gameswf::gc_ptr<gameswf::as_object> receiver=new gameswf::as_object(root->get_player());
   gameswf::gc_ptr<gameswf::as_array> rows=new gameswf::as_array(root->get_player());
   rows->push(gameswf::as_value(99));
   if(!graph.retain_object(receiver.get_ptr(),host.object_value,failure)||!graph.retain_object(rows.get_ptr(),host.array_value,failure))return false;
   host.object=host.object_value.identity();host.array=host.array_value.identity();return true;
  },error),error);
 }
 dh2::ui::CharacterMenuCallV1 call(std::initializer_list<dh2::ui::CharacterMenuValueV1> args){
  dh2::ui::CharacterMenuCallV1 result;result.arguments=args;return result;
 }
 static bool observe(dh2::ui::SwfAsGraph& graph,const dh2::ui::SwfAsValue& value,
                     dh2::ui::CharacterMenuValueV1& out,std::string& error){
  out={};using Kind=dh2::ui::SwfAsValue::Kind;
  switch(value.kind()){
   case Kind::undefined:return true;
   case Kind::null_value:out.kind=5;return true;
   case Kind::boolean:out.kind=1;return graph.to_boolean(value,out.boolean,error);
   case Kind::number:out.kind=2;return graph.to_number(value,out.number,error);
   case Kind::text:out.kind=4;return graph.to_text(value,out.text,error);
   case Kind::object:out.kind=5;out.object=value.identity();return true;
   case Kind::property:error="Unexpected uncoerced property in positive menu test";return false;
  }
  return false;
 }
 bool dispatch(const char* name,dh2::ui::CharacterMenuCallV1& call,std::string& error){
  return dispatch(*provider->queries,name,call,error);
 }
 bool dispatch(dh2::ui::CharacterMenuQueriesOwnerV1& queries,const char* name,
               dh2::ui::CharacterMenuCallV1& call,std::string& error){
  struct Batch {ConnectedMenuASHostV1& host;const char* name;dh2::ui::CharacterMenuCallV1& call;} batch{*this,name,call};
  auto* previous=provider->queries;provider->queries=&queries;
  const bool status=movie.action_script(&batch,[](void* p,dh2::ui::SwfAsGraph& graph,std::string& failure){
   auto& batch=*static_cast<Batch*>(p);auto& host=batch.host;
   std::vector<dh2::ui::SwfAsValue> arguments;
   for(const auto& value:batch.call.arguments){
    switch(value.kind){
     case 0:arguments.emplace_back();break;
     case 1:arguments.push_back(dh2::ui::SwfAsValue::boolean(value.boolean));break;
     case 2:arguments.push_back(dh2::ui::SwfAsValue::number(value.number));break;
     case 4:arguments.push_back(dh2::ui::SwfAsValue::text(value.text));break;
     case 5:
      if(!value.object)arguments.push_back(dh2::ui::SwfAsValue::null());
      else if(value.object==host.object)arguments.push_back(host.object_value);
      else if(value.object==host.array)arguments.push_back(host.array_value);
      else {failure="Test argument object not retained by actual graph";return false;}
      break;
     default:failure="Unsupported test AS argument";return false;
    }
   }
   dh2::ui::SwfAsValue result;bool callable=false;
   if(!graph.invoke(host.root_value,host.root_value,batch.name,arguments,result,callable,failure)||!callable)return false;
   if(!observe(graph,result,batch.call.result,failure))return false;
   gameswf::as_object* array=nullptr;if(!graph.borrow_object(host.array_value,array,failure))return false;
   host.elements.clear();
   auto* rows=static_cast<gameswf::as_array*>(array);
   for(const auto& item:rows->m_array){
    dh2::ui::CharacterMenuValueV1 out;
    if(item.is_bool()){out.kind=1;out.boolean=item.to_bool();}
    else if(item.is_string()){out.kind=4;out.text=item.to_string();}
    else if(item.is_object()){out.kind=5;out.object=reinterpret_cast<std::uintptr_t>(item.to_object());}
    else if(!item.is_undefined()){out.kind=2;out.number=item.to_number();}
    host.elements.push_back(std::move(out));
   }
   ++host.callbacks;return true;
  },error);
  provider->queries=previous;return status;
 }
 const dh2::ui::CharacterMenuValueV1& member(const char* name){
  auto at=std::find_if(members.rbegin(),members.rend(),[&](auto& value){return value.first==name;});
  check(at!=members.rend(),name);
  struct Batch{ConnectedMenuASHostV1& host;const char* name;dh2::ui::CharacterMenuValueV1& out;} batch{*this,name,at->second};
  std::string error;
  check(movie.action_script(&batch,[](void* p,dh2::ui::SwfAsGraph& graph,std::string& failure){
   auto& batch=*static_cast<Batch*>(p);dh2::ui::SwfAsValue actual;bool found=false;
   return graph.get_member(batch.host.object_value,batch.name,actual,found,failure)&&found&&observe(graph,actual,batch.out,failure);
  },error),error);return at->second;
 }
};
