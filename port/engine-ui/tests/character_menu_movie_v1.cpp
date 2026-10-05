// Real authored character SWF and real AS receivers. Native reload components,
// startup settings, localization and GPU remain explicit fixture providers.
#define main historical_movie_fixture_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "swf_movie.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_menu_movie_v1.hpp"
#include "../character_menu_as_bridge_v1.hpp"
#include "../character_menu_reload_action_v1.hpp"
#include "gameswf/gameswf_function.h"
#include <stdexcept>
#include <cstring>
namespace {
void require(bool yes,const std::string& error){if(!yes)throw std::runtime_error(error);}
struct Host:Test {
 std::shared_ptr<int> owner=std::make_shared<int>(1);
 std::unique_ptr<CharacterMenuReloadActionV1> reload;
 std::unique_ptr<CharacterMenuAsBridgeV1> bridge;
 unsigned reloads=0,phases=0,starts=0,query_boundaries=0,rollover_boundaries=0;bool reject=false,reject_query=false;
 static int service(void* p,const MenuReloadRequest32V1* r,MenuReloadResponse16V1* out){
  auto& h=*static_cast<Host*>(p);++h.phases;
  if(r->service==reload_saved_level_v1)out->value=0;
  if(r->service==reload_menu_fx_v1)out->identity=0xabcd00000002ULL;
  return 0;
 }
 Host(){
  CharacterMenuReloadActionGraphV1 graph;graph.owner=owner;graph.reload={this,service};
  graph.player=[](auto index,bool remote,auto& actor,auto&){require(index==0&&!remote,"Original startup player selection");actor=0xabcd00000001ULL;return true;};
  graph.player_index=[](double value,auto& index,auto&){index=std::int32_t(value);return true;};
  reload=std::make_unique<CharacterMenuReloadActionV1>(std::move(graph));
  bridge=std::make_unique<CharacterMenuAsBridgeV1>(owner,[this](const char* name,auto& call,auto& error){++reloads;return reload->dispatch(name,call,error);});
 }
 static bool start(void* p,const SwfAsLease& lease,std::string& error){
  auto& h=*static_cast<Host*>(p);++h.starts;
  gameswf::as_value version;
  require(!lease.root&&lease.player->get_global()->get_member("$version",&version)&&version.is_string()&&!std::strcmp(version.to_string(),"gameSWF"),"Source builtin version unavailable before shared load");
  if(h.reject){error="Required original graph startup rejected";return false;}
  return true;
 }
 static bool native(void* p,const char* name,const gameswf::fn_call& fn,std::string& error){
  auto& h=*static_cast<Host*>(p);
  if(!std::strcmp(name,"NativeLoadSettings"))return true;
  if(!std::strcmp(name,"NativeIsMultiplayerEnabled")){fn.result->set_bool(false);return true;}
  if(!std::strcmp(name,"NativeSkillsGetSkillPointsLeft")||!std::strcmp(name,"NativeGetSkillDetails")||!std::strcmp(name,"NativeSkillGetEquipedSkillsIDs")){
   ++h.query_boundaries;
   // Explicit gameplay boundary fixture. A fresh player query is declared
   // null on these startup queries, so the source leaves result/receivers alone.
   if(h.reject_query){error="Genuine startup skill query provider required";return false;}
   return true;
  }
  if(!std::strcmp(name,"NativeChangeRolloverInputBehavior")){
   // Explicit render-array/input boundary fixture. The full source entry
   // implementation is a separate candidate; this proves routing, not flags.
   ++h.rollover_boundaries;return true;
  }
  return h.bridge->dispatch(name,fn,error);
 }
 SwfServices connected(){auto s=services();s.native_owner=owner;s.native_action=native;s.graph_start=start;
  s.native_actions={"NativeLoadSettings","NativeIsMultiplayerEnabled","NativeReloadSkills"};return s;}
 static bool inspect(void*,SwfAsGraph& graph,std::string& error){
  SwfAsValue root,global,version,value,result,menu;bool found=false,callable=false;
  require(graph.root_value(root,error)&&graph.global_value(global,error),error);
  require(graph.get_member(global,"$version",version,found,error)&&found&&version.kind()==SwfAsValue::Kind::text,"$version must be the source plain string");
  std::string text;require(graph.to_text(version,text,error)&&text=="gameSWF",error);
  require(graph.invoke(root,global,"getVersion",{},result,callable,error)&&callable&&graph.to_text(result,text,error)&&text=="LINUX","getVersion source platform value differs from source $version");
  require(graph.get_member(root,"WarningType",value,found,error)&&found&&graph.to_text(value,text,error)&&text.empty(),"Authored root startup warning reset not executed");
  bool flag=true;
  require(graph.get_member(root,"AddedStatsThisTurn",value,found,error)&&found&&graph.to_boolean(value,flag,error)&&!flag,"Authored stats reset not executed");
  require(graph.find_target(root,"_root.menu_CharacterMenu",menu,error)&&graph.get_member(menu,"useSkillPoint",value,found,error)&&found&&graph.to_boolean(value,flag,error)&&!flag,"Authored skill-point reset not executed");
  return true;
 }
};
}
int main(int argc,char** argv){try{
 require(argc==2,"Original menu directory required");Host host;host.base=argv[1];CharacterMenuMovieV1 movie;std::string error;
 require(movie.load(host.connected(),error),error);
 require(host.reloads==1&&host.phases==9&&host.starts==1,"Actual character startup must execute the whole NativeReloadSkills caller once");
 require(movie.screens().size()==20,"Complete authored root menu catalog");
 require(movie.action_script(nullptr,Host::inspect,error),error);
 SwfAsValue result;bool callable=true;
 require(movie.invoke("menu_CharacterMenu","MissingSourceMethod",{},result,callable,error)&&!callable,"Source absent method result not preserved");
 auto* prior=movie.movie();const auto prior_identity=movie.screens()[0].receiver.identity();
 host.reject=true;require(!movie.load(host.connected(),error)&&error=="Required original graph startup rejected"&&movie.movie()==prior&&movie.screens()[0].receiver.identity()==prior_identity,"Failed reload must retain previous actual graph");host.reject=false;
 host.reject_query=true;
 require(!movie.load(host.connected(),error)&&error=="Genuine startup skill query provider required"&&movie.movie()==prior&&movie.screens()[0].receiver.identity()==prior_identity,"Authored native startup misses must reject publication instead of becoming undefined functions");
 host.reject_query=false;
 require(movie.advance(0,error)&&movie.display(0,0,854,480,error),error);
 auto missing=host.connected();missing.native_actions.clear();require(!movie.load(missing,error)&&movie.movie()==prior,"Missing required startup callback accepted");
 std::cout<<"{\"validation\":\"PASS\",\"authored_root_menu_receivers\":"<<movie.screens().size()<<",\"actual_authored_startup_reload_calls\":"<<host.reloads<<",\"whole_reload_component_fixture_phases\":"<<host.phases<<",\"startup_skill_query_fixture_boundaries\":"<<host.query_boundaries<<",\"rollover_fixture_boundaries\":"<<host.rollover_boundaries<<",\"source_platform_builtin_values\":2,\"authored_startup_fields_checked\":3,\"failed_reload_preserves_graph\":true,\"required_native_dispatch_failure_rejects_publication\":true,\"draw_calls\":"<<host.draw_counts[SwfDraw::triangle_strip]<<",\"core_error_diagnostics\":"<<host.errors<<",\"limits\":{\"gameplay_startup_localization_GPU_are_fixtures\":true,\"actual_character_SWF_startup_executed\":true,\"MenuBase_lifecycle_and_navigation_connected\":false,\"live_Android\":false}}\n";
 for(const auto& message:movie.movie()->diagnostics())std::cerr<<message<<'\n';
 return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
