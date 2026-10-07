// Reuse exact cache readers; this test exercises a different startup order.
#define main historical_session_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include "character_script_session.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_idle_events.hpp"
#include "../character_script_init_vitals.hpp"
#include "../character_update_queued.hpp"
#include <filesystem>
#include <cmath>
namespace {
using namespace dh2;
struct Retained {
 CharacterStateOwner states;
 std::unique_ptr<CharacterAnimationInstance> animation;
 std::unique_ptr<CharacterScriptSession> session;
 Facts facts{};
 AIEventOwner48 receiver{};
 AIEventState64 ai{};
 AnimationAIState96 animation_ai{};
 std::unique_ptr<StateOwnerDebugDiagnostics> debug;
 std::unique_ptr<CharacterIdleEvents> events;
 Retained(std::uintptr_t identity,std::unique_ptr<CharacterAnimationInstance> instance):states(identity),animation(std::move(instance)){}
 ~Retained(){session.reset();events.reset();animation.reset();}
};
std::vector<float> graph_pose(const scene::Scene& scene){std::vector<float> result;for(const auto& node:scene.graph)result.insert(result.end(),node.world.begin(),node.world.end());return result;}
std::vector<std::uint8_t> asset(const std::filesystem::path& path){return file(path.string().c_str());}
struct DelayedInitFixture {
 ScriptSessionInit32* binding=nullptr; unsigned vitals=0,configured=0,updated=0;
 static int invoke(void* opaque,CharacterScriptSession& session,const ScriptLifecycleRequest32& request){
  auto& self=*static_cast<DelayedInitFixture*>(opaque);
  check(session.owner().lifecycle().active);
  if(request.service==script_refresh_vitals){
   check(request.subject==session.timers().owner && self.binding);
   auto expected=*session.properties();auto expected_view=session.property_view();
   expected_view.base=expected.base.data();expected_view.saved=expected.saved.data();
   expected_view.gear=expected.gear.data();expected_view.resolved=expected.resolved.data();
   data::VitalsChange hp{},mp{};check(!dh2_vitals_initialize(&expected_view,&hp,&mp));
   auto combat=*session.combat_state();
   check(!dh2_character_script_session_init_service(self.binding,session,request));
   check(!std::memcmp(&expected,session.properties().get(),sizeof(expected)));
   check(!std::memcmp(&combat,session.combat_state().get(),sizeof(combat)));++self.vitals;return 0;
  }
  check(!request.subject);
  if(request.service==script_configure_skills){++self.configured;return 0;}
  if(request.service==script_update_skills){++self.updated;return 0;}
  return -1;
 }
};
// Exact complete prefix composition. App/network/CanUpdate gate producers and
// final Kill/unload effects below are declared fixtures, not accepted live bodies.
struct SharedQueueFixture {
 CharacterDeferredQueue* queue=dh2_character_deferred_queue_create();
 std::array<DeferredQueueOwner16,11> projections{};
 std::vector<std::unique_ptr<Retained>>* retained=nullptr;
 unsigned kills=0,unloads=0;
 DeferredQueueServices24 services{this,invoke,3,0};
 ~SharedQueueFixture(){if(queue)check(!dh2_character_deferred_queue_destroy(queue));}
 unsigned count(){std::array<DeferredQueueRow16,32> rows{};unsigned n=0;check(!dh2_character_deferred_queue_snapshot(queue,rows.data(),rows.size(),&n));return n;}
 static int invoke(void* p,CharacterDeferredQueue* q,const DeferredQueueRequest32* request){
  auto& self=*static_cast<SharedQueueFixture*>(p);check(q==self.queue&&request&&!request->reserved&&request->final==1&&self.retained);
  auto owner=std::find_if(self.retained->begin(),self.retained->end(),[&](const auto& actor){return actor->receiver.owner==request->owner;});
  check(owner!=self.retained->end());
  if(request->operation==deferred_queue_kill){check(request->controller==(*owner)->receiver.controller);++self.kills;}
  else if(request->operation==deferred_queue_unload_ai){check(!request->controller);++self.unloads;}
  else return 1;
  return 0; // Explicit final effect fixtures; no live Kill/unload claim.
 }
};
struct QueuedStartupFixture {
 Retained* actor=nullptr;CharacterDeferredScript* deferred=nullptr;
 const CharacterInitServices16* init=nullptr;const data::AiTables* ai=nullptr;
 DebugSwitches* debug=nullptr;const DebugFileServices24* files=nullptr;
 unsigned level=0,clock=0,state_queries=0,loads=0,clocks=0,name_lifetimes=0;
 std::string name;const char* borrowed=nullptr;
 static int can(void*,CanUpdateOwner40*,const CanUpdateRequest24* r,CanUpdateResponse16* out){
  if(!r||!out||(r->operation!=can_update_online&&r->operation!=can_update_dead))return 1;
  *out={0,0,0};return 0; // Explicit offline/source dead-byte gate fixture.
 }
 static int invoke(void* p,UpdateStartupOwner64* owner,const UpdateStartupRequest40* r,UpdateStartupResponse16* out){
  auto& self=*static_cast<QueuedStartupFixture*>(p);check(r&&out&&!r->reserved&&owner&&r->owner==self.actor->receiver.owner);*out={};
  switch(r->operation){
   case update_debug_load:return dh2_character_debug_load(self.debug,self.files)==1?0:1;
   case update_debug_construct:check(r->name&&!self.borrowed);self.name=r->name;self.borrowed=r->name;++self.name_lifetimes;return 0;
   case update_debug_query:check(r->name==self.borrowed&&self.name==r->name);return dh2_character_debug_get(&out->word,self.debug,self.name.c_str(),self.files)==1?0:1;
   case update_debug_destroy:check(r->name==self.borrowed&&self.name==r->name);self.borrowed=nullptr;self.name.clear();return 0;
   case update_get_state:{std::int32_t id=0;check(dh2_character_native_fsm_get_integer(&id,&self.actor->states.native_fsm(),0)==1);std::memcpy(&out->word,&id,4);++self.state_queries;return 0;}
   case update_get_current_level:out->word=self.level;return 0; // Actual cache-selected row, manager identity fixture.
   case update_load_and_init:{check(r->argument==1);int result=self.deferred->load_and_init(1,*self.init);if(result<0)return 1;out->word=result;owner->active_ai=self.actor->session->owner().lifecycle().active;owner->resolved_hp=self.actor->session->properties()->resolved[36];++self.loads;return 0;}
   case update_is_monster:case update_is_miniboss:case update_is_boss:{
    const auto* row=data::ai_props(*self.ai,self.actor->session->properties()->resolved[1]);check(row);
    out->word=r->operation==update_is_monster?row->type==4:(row->flags>>(r->operation==update_is_miniboss?1:2))&1u;return 0;
   }
   case update_online:out->word=0;return 0; // Offline network fixture.
   case update_real_time:out->word=self.clock;++self.clocks;return 0; // Explicit raw manager clock word.
   default:return 1; // Nonempty unknown branches do not become success.
  }
 }
};

}
int main(int argc,char** argv){try{
 check(argc==7); // GDO1,commons,monster,DACT,asset-root,animation-table-directory
 Inputs raw(argv[1]);const auto common=file(argv[2]),monster=file(argv[3]),dact=file(argv[4]);const auto authored=placements(dact);check(authored.size()==11);
 const std::filesystem::path assets=argv[5],table_root=argv[6];std::string error;
 auto design=std::make_unique<CharacterGameDesign>();check(design->initialize(raw.input,error));auto borrow=design->borrow();
 Host host_projection;host_projection.rows.resize(borrow.levels()->levels.size());for(unsigned i=0;i<host_projection.rows.size();++i)std::memcpy(&host_projection.rows[i],borrow.levels()->levels[i].scalar.words+12,24);
 const auto crypt=std::find(borrow.levels()->level_names.begin(),borrow.levels()->level_names.end(),"GOTHICUS_CRYPT_01");check(crypt!=borrow.levels()->level_names.end());host_projection.level={std::int32_t(crypt-borrow.levels()->level_names.begin()),0};
 data::PropertyState player;data::reset_properties(*borrow.rules(),player,&borrow.characters()->rows[263]);check(data::recalc_properties_with_class(*borrow.classes(),*borrow.rules(),player,error));auto player_view=data::property_view(*borrow.rules(),player);check(!dh2_character_host_context_sync_level(&host_projection.player,&player_view));
 HostContextBindings16 host{{&host_projection,&Host::invoke}};MissingSave missing;
 DebugFileServices24 files{&missing,&MissingSave::open,&MissingSave::close};auto debug_owner=std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)>(dh2_character_debug_create(),&dh2_character_debug_destroy);auto* debug=debug_owner.get();check(debug);DebugLevelBinding16 debug_binding{debug,&files};LevelServices16 level{&debug_binding,&dh2_character_debug_level_service};
 auto modules=std::unique_ptr<fx::DebugModules,decltype(&dh2_fx_debug_modules_destroy)>(dh2_fx_debug_modules_create(debug,&files),&dh2_fx_debug_modules_destroy);check(bool(modules));
 fx::PreloadServices16 vitals_debug{modules.get(),&dh2_fx_debug_preload_service};
 const auto dn=asset(table_root/"animations_dictionary_pyarraynames.bin"),dv=asset(table_root/"animations_dictionary_pyarray.bin");data::Dictionary dictionary;check(data::load_dictionary({dn.data(),dn.size()},{dv.data(),dv.size()},dictionary,error));
 const auto ar=asset(table_root/"animations_pyarray.bin"),an=asset(table_root/"animations_pyarraynames.bin"),af=asset(table_root/"animations_pystructnames.bin");data::AnimationTables tables;check(data::load_animation_tables({ar.data(),ar.size()},{an.data(),an.size()},{af.data(),af.size()},dictionary,tables,error));
 data::AnimationRandom shared_random;
 const auto idle_field=std::find(tables.state_names.begin(),tables.state_names.end(),"Idle");check(idle_field!=tables.state_names.end());
 const char* bank_names[]={"skeleton","slime","slime-red","ghost"};const char* models[]={"skeleton","slime_green_v2","slime_green_v2","ghost"};
 std::vector<std::shared_ptr<const CharacterAnimationResources>> banks;
 for(unsigned i=0;i<4;++i){
  const auto metadata=asset(assets/"data"/(std::string("monster-")+bank_names[i]+"-animation-bank.bin"));const auto model=asset(assets/"actors"/(std::string(models[i])+".bdae"));
  resources::BresView bres{};scene::Scene factory;check(dh2_bres_open(&bres,model.data(),model.size())==resources::BresError::ok);check(scene::load(bres,factory,error));
  std::shared_ptr<const CharacterAnimationResources> resource;
  auto reader=[&](const data::AnimationBankResource& r,std::vector<std::uint8_t>& out,std::string&){out=asset(assets/r.asset);return true;};
  check(CharacterAnimationResources::load({metadata.data(),metadata.size()},factory,reader,resource,error));banks.push_back(std::move(resource));
 }
 std::vector<std::unique_ptr<Retained>> actors;unsigned initializations=0,frames=0;
 SharedQueueFixture shared_queue;check(shared_queue.queue);shared_queue.retained=&actors;unsigned application_updates=0,prefixes=0,query_calls=0,clock_calls=0;
 for(unsigned index=0;index<authored.size();++index){
  const auto& source=authored[index];const auto row=std::find(borrow.characters()->names.begin(),borrow.characters()->names.end(),source.row);check(row!=borrow.characters()->names.end());
  CharacterScriptSessionInput input;input.identity=UINT64_C(0x9876012300000000)+index+1;input.name=source.name;input.position=source.position;input.source_is_character=1;
  input.properties=std::make_shared<data::PropertyState>();input.combat=std::make_shared<data::CombatActorState>();input.common={common.data(),common.size()};input.external={monster.data(),monster.size()};input.host=&host;input.level=&level;
  data::reset_properties(*borrow.rules(),*input.properties,&borrow.characters()->rows[row-borrow.characters()->names.begin()]);check(data::recalc_properties_with_class(*borrow.classes(),*borrow.rules(),*input.properties,error));
  const auto resource=std::find_if(banks.begin(),banks.end(),[&](const auto& b){return b->metadata().animation_table==std::uint32_t(input.properties->resolved[2]);});check(resource!=banks.end());
  auto instance=CharacterAnimationInstance::create(*resource,error);check(bool(instance));
  auto actor=std::make_unique<Retained>(input.identity,std::move(instance));
  const auto& idle=tables.characters.at((*resource)->metadata().animation_table).fields[idle_field-tables.state_names.begin()];check(idle.size()==1);actor->facts.idle=idle[0];
  // Actual Character/NativeFSM/property identities; caller controller and AI
  // identities plus source-gate bytes are explicit projected producer fixtures.
  actor->receiver={input.identity,UINT64_C(0xb100000000000000)+index+1,reinterpret_cast<std::uintptr_t>(&actor->states.native_fsm()),reinterpret_cast<std::uintptr_t>(input.properties.get()),0,0,0,0};
  actor->ai={UINT64_C(0xb200000000000000)+index+1,&actor->receiver,character_idle_ai_keys(),0,nullptr,0,0,0,0,0,0};
  actor->animation_ai.owner=input.identity;actor->animation_ai.controller=actor->receiver.controller;
  actor->debug=std::make_unique<StateOwnerDebugDiagnostics>(*debug,files);IdleEventProviders providers{};providers.diagnostics=actor->debug.get();
  actor->events=std::make_unique<CharacterIdleEvents>(actor->states,*actor->animation,tables,shared_random,actor->facts,actor->ai,actor->animation_ai,1,providers);
  check(actor->events->error().empty()&&actor->events->bind_input(input));check(input.state_machine==&actor->states.native_fsm());
  actor->session=CharacterScriptSession::create(design->borrow(),input,error);check(actor->session&&error.empty());check(actor->events->attach(*actor->session));

  // Explicit preceding InitPost/body/zone and source gate projections remain
  // fixtures. This tests the actual null-AIS Idle transition on the same owner.
  check(!actor->session->owner().lifecycle().active);
  check(actor->session->owner().lifecycle().load_step==0&&actor->session->timers().count==0);
  check(!actor->ai.active&&!actor->ai.ais_virtuals&&!actor->states.native_fsm().current_present);
  check(actor->states.initialize_level(3,actor->events->state_services())==1);
  check(actor->events->error().empty()&&actor->states.state().current==3&&actor->states.state().flags==0x2380);
  check(actor->events->counts().state_changed==1&&actor->events->counts().animation_selections==1);
  check(!actor->ai.active&&!actor->session->owner().lifecycle().active&&actor->session->timers().count==0);
  // Sample one real CPU phase pair while the AIS remains uninitialized.
  check(actor->events->scene_phase(1000)&&actor->events->animator_phase());++frames;
  check(!actor->session->owner().lifecycle().active&&actor->session->owner().lifecycle().load_step==0);
  auto* cpu=actor->animation.get();auto* fsm=&actor->states.native_fsm();
  const auto phase=actor->animation->playback().root_timestamp;
  const auto selections=actor->events->counts().animation_selections;
  CharacterDeferredScript deferred(*actor->session);DelayedInitFixture init;
  const CharacterInitServices16 init_services{&init,&DelayedInitFixture::invoke};
  ScriptSessionInit32 vitals_binding{input.identity,&vitals_debug,&init_services,0};init.binding=&vitals_binding;
  QueuedStartupFixture startup;startup.actor=actor.get();startup.deferred=&deferred;startup.init=&init_services;startup.ai=borrow.ai();startup.debug=debug;startup.files=&files;startup.level=host_projection.level.row_index;startup.clock=0x80000000u+index;
  CanUpdateOwner40 eligible{input.identity,nullptr,input.identity,0,0,0,0,0,0};CanUpdateServices24 can_services{&startup,&QueuedStartupFixture::can,63,0};
  UpdateStartupOwner64 owner{&eligible,&can_services,0,actor->receiver.controller,&application_updates,nullptr,actor->session->properties()->resolved[36],1,{},0};
  shared_queue.projections[index]={input.identity,actor->receiver.controller};UpdateQueuedBinding24 queue_binding{shared_queue.queue,&shared_queue.projections[index],&shared_queue.services};
  UpdateStartupServices24 prefix_services{&startup,&QueuedStartupFixture::invoke,(1u<<19)-1,0};unsigned stage=99;
  check(!dh2_character_update_queued(&owner,&prefix_services,&queue_binding,&stage)&&stage==update_controller_ready);
  check(owner.active_ai==actor->session->owner().lifecycle().active&&owner.active_ai&&startup.loads==1&&!startup.borrowed&&startup.state_queries==3);
  // A later prefix observes the same active publication and same NativeFSM,
  // skips reload/clock/registration, and increments Application stats once.
  check(!dh2_character_update_queued(&owner,&prefix_services,&queue_binding,&stage)&&startup.loads==1&&!startup.borrowed&&startup.state_queries==6);
  prefixes+=2;query_calls+=startup.state_queries;clock_calls+=startup.clocks;
  const int deferred_status=deferred.failed()?-2:1;
  if(deferred_status!=1||deferred.failed()||!deferred.error().empty())throw std::runtime_error("Deferred "+source.name+" status "+std::to_string(deferred_status)+" virtual "+std::to_string(deferred.last_virtual_status())+": "+deferred.error());
  ++checks;
  check(init.vitals==1&&init.configured==1&&init.updated==1);
  auto view=active(*actor->session);check(view.kind==script_external&&view.loaded_files==2);
  check(get(view.vm,"saved_X").number==source.position[0]&&get(view.vm,"saved_Y").number==source.position[1]);
  load(view.vm,"after_delayed_start=GetState()");check(get(view.vm,"after_delayed_start").number==3);
  check(actor->events->publish_external()&&actor->ai.active==view.identity);
  check(actor->animation.get()==cpu&&&actor->states.native_fsm()==fsm&&actor->animation->playback().root_timestamp==phase);
  check(actor->events->counts().animation_selections==selections&&actor->states.state().elapsed_ms==0);
  check(deferred.load_and_init(1,init_services)==0&&init.vitals==1&&init.configured==1&&init.updated==1);
  check(actor->events->scene_phase(1071)&&actor->events->animator_phase());++frames;
  ++initializations;actors.push_back(std::move(actor));
 }
 check(missing.opened==1);banks.clear();borrow={};design.reset();
 unsigned observations=0;
 for(auto& actor:actors){
  check(actor->events->error().empty()&&actor->states.state().current==3&&actor->states.state().elapsed_ms==0);
  check(actor->debug->retained_strings()==0&&actor->debug->queries()==actor->debug->destructions());
  for(float value:graph_pose(actor->animation->scene()))check(std::isfinite(value));
  load(active(*actor->session).vm,"retained_delayed_state=GetState()");
  check(get(active(*actor->session).vm,"retained_delayed_state").number==3);++observations;
 }
 check(application_updates==22&&prefixes==22&&query_calls==66);
 const auto queue_count=shared_queue.count();check(shared_queue.kills==shared_queue.unloads);
 check(!dh2_character_deferred_queue_destroy(shared_queue.queue));shared_queue.queue=nullptr;
 actors.clear();modules.reset();debug_owner.reset();
 std::cout<<"{\"validation\":\"PASS\",\"queued_prefix_calls\":"<<prefixes<<",\"application_stats_increments\":"<<application_updates<<",\"same_native_FSM_state_queries\":"<<query_calls<<",\"raw_clock_fixture_calls\":"<<clock_calls<<",\"retained_queue_entries\":"<<queue_count<<",\"explicit_Kill_fixture_calls\":"<<shared_queue.kills<<",\"explicit_AIUnload_fixture_calls\":"<<shared_queue.unloads<<",\"checks\":"<<checks<<",\"actual_delayed_monsters\":"<<initializations<<",\"source_Idle_before_AIS\":"<<initializations<<",\"retained_FSM_and_CPU_owners\":"<<observations<<",\"split_phase_pairs\":"<<frames<<",\"source_elapsed_incremented\":false,\"required_skills_are_fixtures\":true,\"source_vitals_and_debug_delivered\":true,\"combat_state_unchanged\":true,\"full_InitPost_or_Update\":false,\"CanUpdate_and_Application_producers_are_fixtures\":true,\"Kill_and_full_AIUnload_are_fixtures\":true,\"mismatches\":0}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
