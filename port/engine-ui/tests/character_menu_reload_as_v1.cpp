// Actual original shared/HUD movie transport; reload components/EABI/GPU and
// startup providers remain explicit fixtures. No authored charmenu-flow claim.
#define main historical_movie_fixture_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "swf_movie.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_menu_as_bridge_v1.hpp"
#include "../character_menu_reload_action_v1.hpp"
#include "gameswf/gameswf_function.h"
#include <cstring>
#include <cmath>
#include <stdexcept>
namespace {
void require(bool yes,const std::string& why){if(!yes)throw std::runtime_error(why);}
unsigned getters=0;
void getter(const gameswf::fn_call& fn){++getters;fn.result->set_double(7.75);}
void setter(const gameswf::fn_call& fn){fn.result->set_undefined();}
struct ReloadHost:Test {
 std::shared_ptr<int> game=std::make_shared<int>(1);
 std::unique_ptr<CharacterMenuReloadActionV1> reload;
 std::unique_ptr<CharacterMenuAsBridgeV1> bridge;
 bool present=true;int fail=-1;std::int32_t selected=-1;
 unsigned selections=0,indices=0,services=0,callbacks=0;
 static int service(void* p,const MenuReloadRequest32V1* request,MenuReloadResponse16V1* response){
  auto& host=*static_cast<ReloadHost*>(p);++host.services;
  require(request->subject==(request->service==reload_spec_prompt_v1?0xfabc00000009ULL:0xfabc00000001ULL),"Same retained Character/Menu identity");
  if(int(request->service)==host.fail)return -1;
  if(request->service==reload_saved_level_v1)response->value=0;
  if(request->service==reload_menu_fx_v1)response->identity=0xfabc00000009ULL;
  if(request->service==reload_spec_prompt_v1)require(!request->argument&&!std::strcmp(request->path,"_root.menu_CharacterMenu")&&!std::strcmp(request->callback,"IsSpecTime"),"Original AS specialization boundary");
  return 0;
 }
 ReloadHost(){
  CharacterMenuReloadActionGraphV1 graph;graph.owner=game;graph.reload={this,service};
  graph.player=[this](auto index,bool remote,auto& actor,auto&){require(!remote,"Original remote flag");++selections;selected=index;actor=present?0xfabc00000001ULL:0;return true;};
  graph.player_index=[this](double value,auto& index,auto& error){
   if(!std::isfinite(value)||value<INT32_MIN||value>INT32_MAX){error="EABI fixture only proves finite representable values";return false;}
   index=std::int32_t(value);++indices;return true;
  };
  reload=std::make_unique<CharacterMenuReloadActionV1>(std::move(graph));
  bridge=std::make_unique<CharacterMenuAsBridgeV1>(game,[this](const char* name,auto& call,auto& error){++callbacks;return reload->dispatch(name,call,error);});
 }
 static bool native(void* p,const char* name,const gameswf::fn_call& fn,std::string& error){
  auto& host=*static_cast<ReloadHost*>(p);
  if(!std::strcmp(name,"NativeLoadSettings"))return true;
  if(!std::strcmp(name,"NativeIsMultiplayerEnabled")){fn.result->set_bool(false);return true;}
  return host.bridge->dispatch(name,fn,error);
 }
 static bool batch(void* p,SwfAsGraph& graph,std::string& error){
  auto& host=*static_cast<ReloadHost*>(p);SwfAsValue root,result;bool callable=false;
  require(graph.root_value(root,error),error);
  for(const auto& args:std::vector<std::vector<SwfAsValue>>{
   {},{SwfAsValue::number(0)},{SwfAsValue::text("2.75")},
   {SwfAsValue::boolean(true)},{SwfAsValue::null()}}){
   auto before=host.services;
   require(graph.invoke(root,root,"NativeReloadSkills",args,result,callable,error)&&callable,error);
   require(result.kind()==SwfAsValue::Kind::undefined&&host.services==before+9,"Original untouched result and complete reload delivery");
  }
  gameswf::as_object* object=nullptr;require(graph.borrow_object(root,object,error),error);
  object->builtin_member("ReloadBound",gameswf::as_value(gameswf::as_value(getter),gameswf::as_value(setter)));
  auto* env=object->get_environment();auto size=env->get_stack_size();
  auto invoke=[&](int count,gameswf::as_value& out){return host.bridge->dispatch("NativeReloadSkills",gameswf::fn_call(&out,object,env,count,env->get_top_index()),error);};
  env->push(gameswf::as_value());require(object->get_member("ReloadBound",&env->bottom(env->get_top_index())),"Retained bound argument");
  gameswf::as_value out(77);getters=0;
  require(invoke(1,out)&&getters==1&&host.selected==7&&out.to_number()==77,error);
  env->set_stack_size(size);env->push(gameswf::as_value(4));env->push(gameswf::as_value());
  require(object->get_member("ReloadBound",&env->bottom(env->get_top_index())),"Ignored original argument");
  getters=0;require(invoke(2,out)&&getters==0&&host.selected==0&&out.to_number()==77,"Multi-argument path coerced ignored property/result");
  env->set_stack_size(size);getters=0;
  require(object->get_member("ReloadBound",&out),"Bound result");
  require(invoke(0,out)&&getters==0&&out.is_property(),"Unchanged original bound result was coerced");
  out.set_double(77);host.present=false;auto before=host.services;
  require(invoke(0,out)&&host.services==before&&out.to_number()==77,"Null player performed reload");host.present=true;
  host.fail=reload_saved_skills_v1;before=host.services;
  require(!invoke(0,out)&&host.services==before+2&&out.to_number()==77&&error.find("phase 2")!=std::string::npos,"Required prefix/result lost");host.fail=-1;
  env->push(gameswf::as_value());before=host.selections;
  require(!invoke(1,out)&&host.selections==before&&out.to_number()==77,"Unproven EABI conversion accepted");
  env->set_stack_size(size);error.clear();return true;
 }
};
}
int main(int argc,char** argv){try{
 require(argc==2,"original menu directory");std::weak_ptr<int> retained;
 unsigned callbacks=0,selections=0,indices=0,services=0;
 {
  auto host=std::make_shared<ReloadHost>();host->base=argv[1];retained=host->game;
  auto providers=host->Test::services();providers.native_owner=host;providers.native_action=ReloadHost::native;
  providers.native_actions={"NativeLoadSettings","NativeIsMultiplayerEnabled","NativeReloadSkills"};
  SwfMovie movie;host->movie=&movie;std::string error;
  require(movie.load({"dqshared_droid.swf"},"dqhud_droid.swf",providers,error),error);
  require(movie.action_script(host.get(),ReloadHost::batch,error),error);
  callbacks=host->callbacks;selections=host->selections;indices=host->indices;services=host->services;
  host->game.reset();require(!retained.expired(),"Graph owner released during facade lifetime");
 }
 require(retained.expired(),"Retained graph cycle");
 std::cout<<"{\"validation\":\"PASS\",\"actual_AS_reload_callbacks\":"<<callbacks<<",\"fresh_player_selections\":"<<selections<<",\"finite_EABI_fixture_conversions\":"<<indices<<",\"ordered_reload_services\":"<<services<<",\"bound_property_and_result_checks\":3,\"required_or_null_guards\":3,\"whole_reload_coordinator_composed\":true,\"component_and_platform_services_are_fixtures\":true,\"authored_character_menu_flow\":false}\n";return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
