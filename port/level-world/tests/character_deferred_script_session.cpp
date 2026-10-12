// Use immutable real-cache input decoding and explicit Host/missing-file fixtures.
#define main historical_session_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_session.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_deferred_script.hpp"
namespace {
struct InitProvider {
 CharacterDeferredScript* adapter=nullptr;std::vector<unsigned> calls;
 int fail_at=-1;bool post_error=false;unsigned reentries=0;
 static int nested(void* p,const dh2_script_value*,unsigned,dh2_script_value*,unsigned,unsigned* count,char*,std::size_t){auto& t=*static_cast<InitProvider*>(p);CharacterInitServices16 services{&t,invoke};check(t.adapter->load_and_init(1,services)==0);++t.reentries;*count=0;return 0;}
 static int invoke(void* p,CharacterScriptSession& session,const ScriptLifecycleRequest32& r){
  auto& t=*static_cast<InitProvider*>(p);check(session.owner().lifecycle().active);t.calls.push_back(r.service);
  if(r.service==script_refresh_vitals){check(r.subject==session.timers().owner);CharacterInitServices16 nested{&t,invoke};check(t.adapter->load_and_init(1,nested)==0);++t.reentries;}
  if(int(r.service)==t.fail_at)return -1;
  if(r.service==script_update_skills){auto v=active(session);check(!dh2_script_vm_bind_source_values(v.vm,"NestedStart",nested,&t));load(v.vm,t.post_error?
   "function OnInitPost() NestedStart();post_count=(post_count or 0)+1;error('post-source-diagnostic') end;function OnInitFinal() final_count=(final_count or 0)+1 end":
   "function ReplacementPost() NestedStart();post_count=(post_count or 0)+1;return setmetatable({},{__index=function(_,k) if k=='_this' then projected=(projected or 0)+1 end end}) end;AddToVFTable('OnInitPost','ReplacementPost');function OnInitFinal() final_count=(final_count or 0)+1 end");}
  return 0;
 }
};
}
int main(int argc,char** argv){try{
 check(argc==5);Inputs raw(argv[1]);auto common=file(argv[2]),monster=file(argv[3]);auto actors=placements(file(argv[4]));check(actors.size()==11);CharacterGameDesign design;std::string error;check(design.initialize(raw.input,error));auto d=design.borrow();
 Host h;h.rows.resize(d.levels()->levels.size());for(unsigned i=0;i<h.rows.size();++i)std::memcpy(&h.rows[i],d.levels()->levels[i].scalar.words+12,24);auto at=std::find(d.levels()->level_names.begin(),d.levels()->level_names.end(),"GOTHICUS_CRYPT_01");check(at!=d.levels()->level_names.end());h.level={int(at-d.levels()->level_names.begin()),0};auto player=std::make_shared<dh2::data::PropertyState>();dh2::data::reset_properties(*d.rules(),*player,&d.characters()->rows[263]);check(dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*player,error));auto pv=dh2::data::property_view(*d.rules(),*player);check(!dh2_character_host_context_sync_level(&h.player,&pv));HostContextBindings16 host{{&h,Host::invoke}};
 MissingSave missing;DebugFileServices24 files{&missing,MissingSave::open,MissingSave::close};auto* debug=dh2_character_debug_create();check(debug);DebugLevelBinding16 binding{debug,&files};LevelServices16 level{&binding,dh2_character_debug_level_service};unsigned initialized=0,post=0,final=0,reentered=0,prefixes=0;
 const std::string malformed="syntax ???";
 for(unsigned i=0;i<actors.size()+4;++i){const auto& actor=actors[i%actors.size()];auto row=std::find(d.characters()->names.begin(),d.characters()->names.end(),actor.row);check(row!=d.characters()->names.end());CharacterScriptSessionInput in;in.identity=0xabcdef0123000001ull+i;in.name=actor.name;in.properties=std::make_shared<dh2::data::PropertyState>();in.combat=std::make_shared<dh2::data::CombatActorState>();in.position=actor.position;in.source_is_character=1;in.common={common.data(),common.size()};in.external=i==14?dh2::data::Bytes{reinterpret_cast<const std::uint8_t*>(malformed.data()),malformed.size()}:dh2::data::Bytes{monster.data(),monster.size()};in.host=&host;in.level=&level;dh2::data::reset_properties(*d.rules(),*in.properties,&d.characters()->rows[row-d.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*in.properties,error));auto session=CharacterScriptSession::create(design.borrow(),in,error);check(session&&error.empty());
  // Preparation retains pending metadata only: there is no active AIS or timer.
  check(!session->owner().lifecycle().active&&session->owner().lifecycle().load_step==0&&session->owner().lifecycle().delayed==1&&session->timers().count==0);
  CharacterDeferredScript deferred(*session);InitProvider provider;provider.adapter=&deferred;if(i==11)provider.fail_at=script_configure_skills;if(i==12)provider.post_error=true;CharacterInitServices16 services{&provider,InitProvider::invoke};unsigned final_requested=i==13?0:1;
  const int result=deferred.load_and_init(final_requested,services);check(session->owner().lifecycle().active&&session->owner().lifecycle().load_step==7&&session->timers().count==2);auto v=active(*session);if(i!=14)check(get(v.vm,"saved_X").number==actor.position[0]&&get(v.vm,"saved_Y").number==actor.position[1]);else check(session->owner().last_source_load_status()>0&&!deferred.error().empty());check(dh2_script_vm_stack_size(v.vm)==5);
  if(i==11){check(result==-2&&deferred.failed()&&provider.calls.size()==2);check(get(v.vm,"post_count").type==DH2_SCRIPT_NIL);++prefixes;}
  else {check(result==1&&!deferred.failed()&&provider.calls==std::vector<unsigned>({12,13,14}));check(get(v.vm,"post_count").number==1);++post;if(final_requested){check(get(v.vm,"final_count").number==1);++final;}else check(get(v.vm,"final_count").type==DH2_SCRIPT_NIL);if(i==12)check(!deferred.error().empty());else check(get(v.vm,"projected").number==1);if(i<11)++initialized;}
  auto before=provider.calls;check(deferred.load_and_init(1,services)==0&&provider.calls==before);check(deferred.load_and_init(2,services)==-1);reentered+=provider.reentries;
 }
 dh2_character_debug_destroy(debug);std::cout<<"{\"validation\":\"PASS\",\"actual_Crypt_monster_load_then_init\":"<<initialized<<",\"active_Post_calls\":"<<post<<",\"active_Final_calls\":"<<final<<",\"nested_active_guard_calls\":"<<reentered<<",\"required_failure_prefixes\":"<<prefixes<<",\"source_Lua_Post_error_delivered\":1,\"positive_source_Lua_load_error_delivered\":1,\"final_false_cases\":1,\"DebugSwitches_missing_file_fixture_deliveries\":"<<missing.opened<<",\"host_fixture_projection_reads\":"<<h.reads<<",\"checks\":"<<checks<<",\"VM_checks\":"<<vm_checks<<",\"mismatches\":0}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
