// Reuse exact cache readers; this test exercises a different startup order.
#define main historical_session_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_script_session.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_idle_events.hpp"
#include "../character_script_init_vitals.hpp"
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
 std::vector<std::unique_ptr<Retained>> actors;unsigned initializations=0,frames=0;IdleEventCounts total{};
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
  const int deferred_status=deferred.load_and_init(1,init_services);
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
 actors.clear();modules.reset();debug_owner.reset();
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_delayed_monsters\":"<<initializations<<",\"source_Idle_before_AIS\":"<<initializations<<",\"retained_FSM_and_CPU_owners\":"<<observations<<",\"split_phase_pairs\":"<<frames<<",\"source_elapsed_incremented\":false,\"required_skills_are_fixtures\":true,\"source_vitals_and_debug_delivered\":true,\"combat_state_unchanged\":true,\"full_InitPost_or_Update\":false,\"mismatches\":0}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
