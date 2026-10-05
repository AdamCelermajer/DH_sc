#define main existing_script_session_fixture_main
#include "character_script_session.cpp"
#undef main
#include "../character_world_npc_initialization_v1.hpp"
namespace {
namespace sk=dh2::character::skills;
struct Npc {
 std::uintptr_t id{},node{},target_id{};std::uint8_t enabled=0;
 dh2::target_search::Object48 search{};sk::SkillTargetCharacterV6 character{};
 dh2::scene::Scene scene{};dh2::target_providers::Handle16 handle{};
 std::shared_ptr<dh2::data::PropertyState> properties;
 std::shared_ptr<dh2::data::CombatActorState> life;
 std::unique_ptr<CharacterScriptSession> session;
 std::unique_ptr<CharacterWorldNpcStateOwnerV1> owner;
 std::unique_ptr<CharacterWorldNpcControllerV1> controller;
 AIEventOwner48 ai_owner{};AIEventState64 ai{};
 std::unique_ptr<CharacterWorldNpcStateChangedV1> notifications;
 Facts facts{};StateOwnerBehaviorPredicate8 predicates{};Services bodies{};
 unsigned animation_deliveries=0,event_deliveries=0;
 static void body(void* p,State* s,const Request* r){auto& n=*static_cast<Npc*>(p);check(r->service==set_animation);s->current_animation=r->argument[0];++n.animation_deliveries;}
 static int remaining(void* p,StateOwnerMachine40*,const StateOwnerRequest48* r,StateOwnerResponse8*){auto& n=*static_cast<Npc*>(p);if(r->operation!=state_owner_character_event)return 1;check(r->event==0x1d);++n.event_deliveries;return n.notifications->notify(*r);}
 static int refresh(void* p,sk::WorldTargetActorBorrowV1* out){auto& n=*static_cast<Npc*>(p);*out={n.id,&n.search,&n.character,&n.scene,n.life.get(),&n.node,&n.enabled,nullptr,n.search.position,nullptr,nullptr,nullptr,nullptr};return 0;}
};
}
int main(int argc,char** argv){try{
 check(argc==2);const std::string root=argv[1];std::string error;
 Inputs raw((root+"/port/level-world/reference/character-game-design/real-cache-inputs.bin").c_str());CharacterGameDesign design;check(design.initialize(raw.input,error));auto d=design.borrow();
 auto dact=file((root+"/port/android-native/app/src/main/assets/worlds/crypt01.dact").c_str());auto authored=placements(dact);
 auto binary=file((root+"/port/level-world/reference/actor-initialization/crypt01-actor-initialization.bin").c_str());ActorInitializationDigest digest{};std::memcpy(digest.data(),binary.data()+64,32);
 std::vector<ActorInitializationKey> keys;for(std::size_t at=16;at<dact.size();at+=256){std::uint32_t kind,room;std::memcpy(&kind,dact.data()+at,4);std::memcpy(&room,dact.data()+at+4,4);if(kind==1)keys.push_back({room,reinterpret_cast<const char*>(dact.data()+at+8),reinterpret_cast<const char*>(dact.data()+at+72)});}
 ActorInitialization initialization;check(load_actor_initialization(binary.data(),binary.size(),digest,keys,initialization,error));check(keys.size()==11&&authored.size()==11);
 sk::CharacterWorldRuntimeV1 world(*d.ai());CharacterWorldNpcInitializationV1 loader(world,initialization);std::vector<std::unique_ptr<Npc>> actors;
 MissingSave missing;DebugFileServices24 files{&missing,MissingSave::open,MissingSave::close};std::unique_ptr<DebugSwitches,decltype(&dh2_character_debug_destroy)> debug(dh2_character_debug_create(),dh2_character_debug_destroy);check(debug!=nullptr);StateOwnerDebugDiagnostics diagnostics(*debug,files);
 unsigned interactive=0;std::vector<std::int32_t> types;for(const auto& row:d.ai()->rows)types.push_back(row.type);dh2::target_providers::Types16 type_view{types.data(),std::uint32_t(types.size()),0};
 for(unsigned index=0;index<keys.size();++index){
  auto npc=std::make_unique<Npc>();auto& n=*npc;n.id=0x200000001ull+index;n.properties=std::make_shared<dh2::data::PropertyState>();n.life=std::make_shared<dh2::data::CombatActorState>();
  auto found=std::find(d.characters()->names.begin(),d.characters()->names.end(),keys[index].character);check(found!=d.characters()->names.end());dh2::data::reset_properties(*d.rules(),*n.properties,&d.characters()->rows[found-d.characters()->names.begin()]);check(dh2::data::recalc_properties_with_class(*d.classes(),*d.rules(),*n.properties,error));
  n.facts.idle=215;n.bodies={&n,Npc::body};WorldNpcStateServicesV1 services{};services.facts=&n.facts;services.bodies=&n.bodies;services.predicates=&n.predicates;services.remaining_methods={&n,Npc::remaining};services.diagnostics=&diagnostics;services.diagnostics_required=1;n.owner=std::make_unique<CharacterWorldNpcStateOwnerV1>(n.id,services);
  CharacterScriptSessionInput input{};input.identity=n.id;input.name=keys[index].name;input.properties=n.properties;input.combat=n.life;input.source_is_character=1;input.state_machine=&n.owner->native_fsm();n.session=CharacterScriptSession::create(design.borrow(),input,error);check(n.session!=nullptr);
  n.controller=std::make_unique<CharacterWorldNpcControllerV1>(n.id);auto* controller=n.controller->command_state(0);check(controller&&controller->owner==n.id&&!controller->locked&&!controller->forced);n.ai_owner={n.id,n.controller->identity(),reinterpret_cast<std::uintptr_t>(&n.owner->native_fsm()),reinterpret_cast<std::uintptr_t>(&n.session->property_view()),0,0,0,0};n.ai={reinterpret_cast<std::uintptr_t>(&n.ai),&n.ai_owner,character_idle_ai_keys(),0,character_idle_external_keys(),0,0,0,0,0,0};n.notifications=std::make_unique<CharacterWorldNpcStateChangedV1>(*n.owner,n.ai);
  n.search.identity=n.id;n.search.visible=1;n.character={n.id,n.properties->resolved.data(),keys[index].name.c_str(),0,0,0,1,1};check(world.add({n.id,int(index+1),&n,Npc::refresh,&n.owner->state(),&n.handle,&n.target_id})==0);
  const auto before=n.properties->resolved;auto queries=world.targets().query_services();std::uintptr_t flags=99;dh2::target_providers::Request24 request{dh2::target_providers::live_state_flags,0,n.id,0};check(!queries.invoke(queries.context,&request,&flags)&&flags==0);
  std::int32_t can_target=99;check(!sk::dh2_character_skill_target_query_live_flags_v6(&can_target,dh2::target_providers::is_interactive,&n.character,nullptr,&type_view,&queries)&&can_target==0);
  WorldNpcInitializationBorrowV1 in{n.id,keys[index].room,keys[index].name.c_str(),n.owner.get(),n.session.get()};check(loader.initialize(in)==1);check(n.owner->state().current==3&&n.owner->state().flags==0x2380);check(n.animation_deliveries==1&&n.event_deliveries==1);check(n.properties->resolved==before&&n.session->combat_state()==n.life);check(!queries.invoke(queries.context,&request,&flags)&&flags==0x2380);check(!sk::dh2_character_skill_target_query_live_flags_v6(&can_target,dh2::target_providers::is_interactive,&n.character,nullptr,&type_view,&queries)&&can_target==1);++interactive;
  auto bad=in;bad.name="absent";check(loader.initialize(bad)==-1&&n.animation_deliveries==1);actors.push_back(std::move(npc));
 }
 check(interactive==11);check(diagnostics.loads()==11&&diagnostics.queries()==11&&diagnostics.destructions()==11&&diagnostics.retained_strings()==0);check(missing.opened==1);
 // Actual source External endpoint key is inherited Default's empty body;
 // this explicit AIS identity fixture has no Lua notification to invoke.
 auto& first=*actors[0];first.ai.active=0x8123456789abcdefull;check(first.notifications->raise(std::uintptr_t(-1))==1);first.ai.global_blocked=1;check(first.notifications->raise(std::uintptr_t(-1))==1);first.ai.global_blocked=0;
 std::array<std::uintptr_t,51> unknown{};unknown[8]=0xdead;first.ai.ais_virtuals=unknown.data();check(first.notifications->raise(std::uintptr_t(-1))==-2&&!first.notifications->error().empty());first.ai.ais_virtuals=character_idle_external_keys();
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_Crypt_presets\":11,\"same_World_ScriptSession_FSM_properties_life\":true,\"original_Level_state_entry\":true,\"ordered_animation_fixture\":true,\"source_state_change_dispatch_and_External_empty_endpoint\":true,\"actual_Debug_missing_file_fixture\":true,\"full_Character_InitPost\":false,\"full_NPC_AI\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
