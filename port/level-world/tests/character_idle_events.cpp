// Reuse the frozen genuine GameDesign/monster Init fixture's input readers only.
#define main previous_session_audit_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#include "character_script_session.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_idle_events.hpp"
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
struct EndObservation {
 Retained* retained;unsigned calls=0,nested=0;
 static int observe(void* opaque,const dh2_script_callback_scope* scope,const dh2_script_value*,std::uint32_t,char*,std::size_t){
  auto& self=*static_cast<EndObservation*>(opaque);auto& actor=*self.retained;
  ++self.calls;check(actor.states.state().current==3);
  const auto before=actor.events->counts();
  // Synchronous native callback can reenter the source event dispatcher only
  // under this genuinely protected same-private-VM scope.
  if(!self.nested){++self.nested;check(actor.events->raise(0x23,0,scope));check(actor.events->counts().FSM_events==before.FSM_events+3);check(actor.events->counts().animation_selections==before.animation_selections+1);}
  return 0;
 }
};
std::vector<float> graph_pose(const scene::Scene& scene){std::vector<float> result;for(const auto& node:scene.graph)result.insert(result.end(),node.world.begin(),node.world.end());return result;}
std::vector<std::uint8_t> asset(const std::filesystem::path& p){return file(p.string().c_str());}
struct ExistingDebugFile {
 unsigned opened=0,closed=0;
 static int open(void* context,const char* name,std::uintptr_t* out){auto& self=*static_cast<ExistingDebugFile*>(context);check(!std::strcmp(name,"DebugSwitches.savegame"));++self.opened;*out=17;return 0;}
 static int close(void* context,std::uintptr_t handle){auto& self=*static_cast<ExistingDebugFile*>(context);check(handle==17);++self.closed;return 0;}
};
}
int main(int argc,char** argv){try{
 check(argc==7); // GDO1,commons,monster,DACT,342-asset-root,animation-table-directory
 Inputs raw(argv[1]);const auto common=file(argv[2]),monster=file(argv[3]),dact=file(argv[4]);const auto authored=placements(dact);check(authored.size()==11);
 const std::filesystem::path assets=argv[5],table_root=argv[6];std::string error;
 auto design=std::make_unique<CharacterGameDesign>();check(design->initialize(raw.input,error));auto borrow=design->borrow();
 Host host_projection;host_projection.rows.resize(borrow.levels()->levels.size());for(unsigned i=0;i<host_projection.rows.size();++i)std::memcpy(&host_projection.rows[i],borrow.levels()->levels[i].scalar.words+12,24);
 const auto crypt=std::find(borrow.levels()->level_names.begin(),borrow.levels()->level_names.end(),"GOTHICUS_CRYPT_01");check(crypt!=borrow.levels()->level_names.end());host_projection.level={std::int32_t(crypt-borrow.levels()->level_names.begin()),0};
 data::PropertyState player;data::reset_properties(*borrow.rules(),player,&borrow.characters()->rows[263]);check(data::recalc_properties_with_class(*borrow.classes(),*borrow.rules(),player,error));auto player_view=data::property_view(*borrow.rules(),player);check(!dh2_character_host_context_sync_level(&host_projection.player,&player_view));
 HostContextBindings16 host{{&host_projection,&Host::invoke}};MissingSave missing;
 DebugFileServices24 files{&missing,&MissingSave::open,&MissingSave::close};auto debug_owner=std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)>(dh2_character_debug_create(),&dh2_character_debug_destroy);auto* debug=debug_owner.get();check(debug);DebugLevelBinding16 debug_binding{debug,&files};LevelServices16 level{&debug_binding,&dh2_character_debug_level_service};
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
  check(actor->session->start()==0&&actor->session->error().empty());auto view=active(*actor->session);check(view.kind==script_external&&view.loaded_files==2);
  check(get(view.vm,"saved_X").number==source.position[0]&&get(view.vm,"saved_Y").number==source.position[1]);
  load(view.vm,"before_level_state=GetState()");check(get(view.vm,"before_level_state").number==-1);
  check(actor->events->publish_external());check(actor->events->initialize_idle());++initializations;
  check(actor->states.state().current==3&&actor->states.state().flags==0x2380&&actor->states.native_fsm().state==&actor->states.state());
  load(view.vm,"after_level_state=GetState()");check(get(view.vm,"after_level_state").number==3);
  check(actor->events->counts().state_changed==1&&actor->events->counts().animation_helpers>=2&&actor->events->counts().animation_selections==1);
  actors.push_back(std::move(actor));
 }
 check(missing.opened==1);banks.clear();borrow={};design.reset();
 // Shared resource owners and actual session design owners remain pinned;
 // GL context disposal requires no state/pose/clock reconstruction.
 const auto untouched_pose=graph_pose(actors.back()->animation->scene());const auto untouched_clock=actors.back()->animation->playback().root_timestamp;
 for(unsigned i=0;i<220;++i){auto& event=*actors.front()->events;if(!event.scene_phase(1000+i*71)||!event.animator_phase())throw std::runtime_error("First actor tick "+std::to_string(i)+": "+event.error());checks+=2;++frames;}
 check(graph_pose(actors.back()->animation->scene())==untouched_pose&&actors.back()->animation->playback().root_timestamp==untouched_clock);
 for(std::size_t index=1;index<actors.size();++index)for(unsigned i=0;i<220;++i){auto& event=*actors[index]->events;if(!event.scene_phase(1000+i*71)||!event.animator_phase())throw std::runtime_error("Actor "+std::to_string(index)+" tick "+std::to_string(i)+": "+event.error());checks+=2;++frames;}
 unsigned actual_common_end=0,debug_queries=0;
 for(const auto& actor:actors){check(actor->events->error().empty());const auto& count=actor->events->counts();check(count.external_end_calls>0&&count.end_relays==count.external_end_calls&&count.animation_helpers>2&&count.FSM_events>=count.animation_helpers);actual_common_end+=count.external_end_calls;
  total.character_events+=count.character_events;total.animation_events+=count.animation_events;total.AI_services+=count.AI_services;total.FSM_events+=count.FSM_events;total.state_getters+=count.state_getters;total.state_changed+=count.state_changed;total.end_relays+=count.end_relays;total.external_end_calls+=count.external_end_calls;total.animation_helpers+=count.animation_helpers;
  check(actor->states.state().current==3&&actor->states.state().flags==0x2380&&actor->states.state().elapsed_ms==0);
  check(actor->debug->retained_strings()==0&&actor->debug->queries()==actor->debug->destructions()&&actor->debug->queries()>0);debug_queries+=actor->debug->queries();
  for(float value:graph_pose(actor->animation->scene()))check(std::isfinite(value));
 }
 auto& first=*actors.front();auto firstvm=active(*first.session).vm;const auto top=dh2_script_vm_stack_size(firstvm);
 EndObservation observation{&first};check(!dh2_script_vm_bind_source_scoped(firstvm,"ObserveNativeEnd",EndObservation::observe,&observation));
 // New diagnostic global AFTER genuine authored commons callbacks: observes
 // exact end-before-FSM order and recursively dispatches through same VM scope.
 load(firstvm,"function OnEndOfAnim() assert(GetState()==3);ObserveNativeEnd() end");
 auto previous=first.events->counts();check(first.events->raise(0x23));check(observation.calls==2&&observation.nested==1);
 check(first.events->counts().external_end_calls==previous.external_end_calls+2&&first.events->counts().FSM_events==previous.FSM_events+6&&first.events->counts().animation_selections==previous.animation_selections+2&&dh2_script_vm_stack_size(firstvm)==top);
 //22 shares actual +98 relay and forwards after callback even under gates.
 first.ai.global_blocked=1;first.receiver.locked=1;previous=first.events->counts();check(first.events->raise(0x22));check(first.events->counts().external_end_calls==previous.external_end_calls+1&&first.events->counts().FSM_events==previous.FSM_events+4&&first.events->counts().animation_helpers==previous.animation_helpers&&first.events->counts().state_changed==previous.state_changed);
 //24 honors controller/global gate: skips helper but still calls real FSM.
 previous=first.events->counts();check(first.events->raise(0x24));check(first.events->counts().animation_helpers==previous.animation_helpers&&first.events->counts().FSM_events==previous.FSM_events+1);
 first.receiver.forced=1;previous=first.events->counts();check(first.events->raise(0x26));check(first.events->counts().animation_helpers==previous.animation_helpers+1);
 first.ai.global_blocked=0;first.receiver.locked=0;first.receiver.forced=0;
 // Focus's diagnostic prefix runs even when the source suppression byte
 // prevents flags/animation changes. This is an explicit byte-projection case.
 auto& suppressed=*actors.back();const auto before_diagnostics=suppressed.debug->queries();const auto before_selection=suppressed.events->counts().animation_selections;
 suppressed.states.state().idle_suppressed=1;suppressed.states.state().flags=0x5a5a;
 StateOwnerRequest48 focus_request{};focus_request.operation=state_owner_focus;focus_request.source_function=0x3c3020;focus_request.state=3;focus_request.character=suppressed.states.native_fsm().character;StateOwnerResponse8 ignored{};
 const auto& suppress_service=suppressed.events->state_services();check(!suppress_service.invoke(suppress_service.context,&suppressed.states.machine(),&focus_request,&ignored));
 check(suppressed.debug->queries()==before_diagnostics+1&&suppressed.debug->retained_strings()==0&&suppressed.states.state().flags==0x5a5a&&suppressed.events->counts().animation_selections==before_selection);
 suppressed.states.state().idle_suppressed=0;suppressed.states.state().flags=0x2380;
 // Source getter's current value and retained previous signed word enter the
 // genuine empty External StateChanged method; null active remains source gate.
 first.ai.active=0;previous=first.events->counts();check(first.events->raise(0x1d,UINT32_C(0x80000000)));check(first.events->counts().state_changed==previous.state_changed+1);first.ai.active=active(*first.session).identity;
 // Unknown nonempty selected AIS virtual is explicit failure, not a no-op.
 std::array<std::uintptr_t,51> unknown{};std::copy_n(character_idle_external_keys(),51,unknown.begin());unknown[0x20/4]=0x123456;
 first.ai.ais_virtuals=unknown.data();const auto position=std::array<float,3>{first.session->position()[0],first.session->position()[1],first.session->position()[2]};check(!first.events->raise(0x1d,0)&&!first.events->error().empty());++guards;
 check(first.states.state().current==3&&first.session->position()[0]==position[0]);check(!first.events->scene_phase(25000));++guards;
 first.ai.ais_virtuals=character_idle_external_keys();
 unsigned failure_checks=2;
 // Required pin/unknown animation-trigger/property-event/input paths never
 // become successful predicates. All failures retain their delivered prefix.
 auto& pin=*actors[1];pin.states.machine().physical=0x121212;check(!pin.events->raise(0x30)&&pin.states.state().current==3);++failure_checks;
 auto& trigger_actor=*actors[2];const animation::TriggeredEvent trigger{-17,"unresolved-authored-event"};const auto trigger_before=trigger_actor.events->counts();
 check(!trigger_actor.events->raise(0x28,reinterpret_cast<std::uintptr_t>(&trigger))&&trigger_actor.events->counts().FSM_events==trigger_before.FSM_events);++failure_checks;
 check(!actors[3]->events->raise(0x36));++failure_checks;
 CharacterScriptSessionInput unrelated;unrelated.identity=17;check(!actors[4]->events->bind_input(unrelated)&&!unrelated.state_machine);++failure_checks;
 {
  ExistingDebugFile existing;DebugFileServices24 existing_files{&existing,ExistingDebugFile::open,ExistingDebugFile::close};
  auto existing_owner=std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)>(dh2_character_debug_create(),dh2_character_debug_destroy);check(bool(existing_owner));
  StateOwnerDebugDiagnostics diagnostics(*existing_owner,existing_files);
  StateOwnerRequest48 begin{};begin.operation=state_owner_profile_begin;begin.source_function=0x337888;
  StateOwnerRequest48 end{};end.operation=state_owner_profile_end;end.source_function=0x318254;
  check(diagnostics.invoke(end)==-1&&diagnostics.retained_strings()==0);++failure_checks;
  check(diagnostics.invoke(begin)==-3&&existing.opened==1&&existing.closed==1&&diagnostics.loads()==1&&diagnostics.queries()==0&&diagnostics.retained_strings()==0);++failure_checks;
 }
 actors.clear();debug_owner.reset();
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_monster_Init\":"<<initializations<<",\"owned_Idle_Focus\":"<<initializations<<",\"banks\":4,\"split_phase_pairs\":"<<frames<<",\"generated_animation_events\":"<<total.animation_events<<",\"full_AI_router_services\":"<<total.AI_services<<",\"actual_commons_End_callbacks\":"<<actual_common_end<<",\"genuine_DebugSwitches_queries\":"<<debug_queries<<",\"scoped_recursive_end_callbacks\":"<<observation.calls<<",\"failure_checks\":"<<failure_checks<<",\"same_FSM_before_Init\":true,\"per_instance_pose_clock_isolation\":true,\"elapsed_dt_incremented\":false,\"full_original_frame\":false,\"physics_navigation\":false,\"controller_gate_producers\":false,\"sanitizer_findings\":0}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
