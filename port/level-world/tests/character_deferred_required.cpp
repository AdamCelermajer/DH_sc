#define main historical_session_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_session.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_deferred_script.hpp"
namespace {
struct RequiredInit{
 unsigned mode,calls=0;
 static int invoke(void* opaque,CharacterScriptSession& session,const ScriptLifecycleRequest32& request){
  auto& self=*static_cast<RequiredInit*>(opaque);++self.calls;
  if(request.service!=script_update_skills)return 0;
  check(!session.missing_bindings().empty());const auto& missing=session.missing_bindings().front();
  check(missing.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_")==std::string::npos);
  const std::string callback="_G['"+missing+"']()";
  const std::string code=self.mode==0?
   "function OnInitPost() post_prefix=1;"+callback+" end;function OnInitFinal() final_prefix=1 end":self.mode==1?
   "function OnInitPost() post_prefix=1;pcall(function() "+callback+" end);post_after=1 end;function OnInitFinal() final_prefix=1 end":
   "function OnInitPost() post_prefix=1 end;function OnInitFinal() final_prefix=1;"+callback+" end";
  load(active(session).vm,code.c_str());return 0;
 }
};
}
int main(int argc,char** argv){try{
 check(argc==5);Inputs raw(argv[1]);const auto common=file(argv[2]),monster=file(argv[3]);const auto actors=placements(file(argv[4]));check(actors.size()==11);
 CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error));auto data=design.borrow();
 Host h;h.rows.resize(data.levels()->levels.size());for(unsigned i=0;i<h.rows.size();++i)std::memcpy(&h.rows[i],data.levels()->levels[i].scalar.words+12,24);
 const auto at=std::find(data.levels()->level_names.begin(),data.levels()->level_names.end(),"GOTHICUS_CRYPT_01");check(at!=data.levels()->level_names.end());h.level={int(at-data.levels()->level_names.begin()),0};
 dh2::data::PropertyState player;dh2::data::reset_properties(*data.rules(),player,&data.characters()->rows[263]);check(dh2::data::recalc_properties_with_class(*data.classes(),*data.rules(),player,error));auto player_view=dh2::data::property_view(*data.rules(),player);check(!dh2_character_host_context_sync_level(&h.player,&player_view));HostContextBindings16 host{{&h,Host::invoke}};
 MissingSave missing;DebugFileServices24 files{&missing,MissingSave::open,MissingSave::close};auto debug=std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)>(dh2_character_debug_create(),&dh2_character_debug_destroy);check(bool(debug));DebugLevelBinding16 binding{debug.get(),&files};LevelServices16 level{&binding,dh2_character_debug_level_service};
 for(unsigned mode=0;mode<3;++mode){const auto& actor=actors[mode];const auto row=std::find(data.characters()->names.begin(),data.characters()->names.end(),actor.row);check(row!=data.characters()->names.end());
  CharacterScriptSessionInput in;in.identity=UINT64_C(0xacdef00123450000)+mode+1;in.name=actor.name;in.position=actor.position;in.source_is_character=1;in.properties=std::make_shared<dh2::data::PropertyState>();in.combat=std::make_shared<dh2::data::CombatActorState>();in.common={common.data(),common.size()};in.external={monster.data(),monster.size()};in.host=&host;in.level=&level;
  dh2::data::reset_properties(*data.rules(),*in.properties,&data.characters()->rows[row-data.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*data.classes(),*data.rules(),*in.properties,error));auto session=CharacterScriptSession::create(design.borrow(),in,error);check(session&&error.empty());
  CharacterDeferredScript deferred(*session);RequiredInit provider{mode};CharacterInitServices16 services{&provider,RequiredInit::invoke};check(deferred.load_and_init(1,services)==-2&&deferred.failed());
  check(deferred.last_virtual_status()==DH2_SCRIPT_REQUIRED_FAILURE_STATUS&&!deferred.error().empty());check(provider.calls==3);
  const auto view=active(*session);check(get(view.vm,"post_prefix").number==1&&dh2_script_vm_required_failure_epoch(view.vm)==1);
  if(mode<2)check(get(view.vm,"final_prefix").type==DH2_SCRIPT_NIL);else check(get(view.vm,"final_prefix").number==1);
  if(mode==1)check(get(view.vm,"post_after").number==1);
  check(deferred.load_and_init(1,services)==0&&provider.calls==3&&dh2_script_vm_stack_size(view.vm)==5);
 }
 std::cout<<"{\"validation\":\"PASS\",\"required_Post_and_Final_failures\":3,\"Lua_caught_required_failure_rejected\":true,\"native_prefix_preserved\":true,\"no_implicit_retry\":true,\"checks\":"<<checks<<",\"mismatches\":0}\n";
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
