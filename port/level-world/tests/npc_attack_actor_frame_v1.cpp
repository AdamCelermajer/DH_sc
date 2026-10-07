#include "../npc_attack_command_owner_v1.hpp"
#include "../npc_actor_frame_v1.hpp"
#include "../world.hpp"
#include "../canonical_point3d_globals_v1.hpp"
#include <cassert>
#include <fstream>
#include <iterator>
#include <iostream>
using namespace dh2;using namespace dh2::character;using namespace dh2::character::skills;
static std::vector<std::uint8_t> read(const char* path){std::ifstream f(path,std::ios::binary);return {std::istreambuf_iterator<char>(f),{}};}
// Explicit external table/geometry/body-method fixtures around actual retained
// World, target, controller, native FSM, attack kernel and cache floor owners.
struct Actor {
 target_search::Object48 search{};SkillTargetCharacterV6 character{};
 std::int32_t words[224]{};data::CombatActorState life{};State fallback{};
 scene::Scene scene{};target_providers::Handle16 handle{};std::uintptr_t current{},node{};
 std::uint8_t enabled{};float cache[3]{},angle{},heading_angle{};State* machine{};
 static int refresh(void* p,WorldTargetActorBorrowV1* out){auto& a=*static_cast<Actor*>(p);*out={a.search.identity,&a.search,&a.character,&a.scene,&a.life,&a.node,&a.enabled,a.cache,a.search.position,&a.angle,&a.heading_angle,nullptr,nullptr};return 0;}
 static int command(void* p,ControllerCommandState32* out){auto& a=*static_cast<Actor*>(p);*out={1,a.search.identity,0,0,0,0};return 0;}
 static int died(void*,std::uintptr_t){return -1;}
 WorldActorRegistrationV1 registration(int key){return {search.identity,key,this,refresh,machine,&handle,&current,command,nullptr,nullptr,died};}
 Actor(std::uintptr_t id,int faction,int ai){search.identity=id;search.visible=1;words[0]=faction;words[1]=ai;character={id,words,"named-native-fixture",0,0,0,1,1};fallback.flags=0x2000;machine=&fallback;}
};
struct Fixture {
 Facts facts{};std::vector<Request> requests;bool online{},reject_diagnostic{};unsigned online_calls{};
 std::array<float,3> target{},look{{0,-1,0}};
 static void body(void* p,State* s,const Request* q){auto& t=*static_cast<Fixture*>(p);t.requests.push_back(*q);if(q->service==set_animation)s->current_animation=q->argument[0];}
 static int remaining(void*,StateOwnerMachine40*,const StateOwnerRequest48* q,StateOwnerResponse8*){
  return q->operation==state_owner_character_event||q->operation==state_owner_pin||q->operation==state_owner_profile_begin||q->operation==state_owner_profile_end?0:-1;
 }
 static int target_service(void*,TargetState48*,const TargetRequest24* q,std::uint32_t* out){
  switch(q->service){case target_debug_load:case target_debug_query:*out=0;return 0;
   case target_owner_ai_id:*out=1;return 0;case target_virtual_dead:*out=0;return 0;case target_in_sight:*out=1;return 0;default:return -1;}
 }
 static bool is_online(void* p,bool& out,std::string&){auto& t=*static_cast<Fixture*>(p);++t.online_calls;out=t.online;return true;}
 static bool radius(void*,std::uintptr_t,float& out,std::string&){out=20;return true;}
 static bool query(void*,std::uintptr_t,std::uintptr_t,WorldAIAttackQueryV1 q,std::int32_t& out,std::string&){out=q==WorldAIAttackQueryV1::CharacterCanRangeAttack?0:1;return true;}
 static int backend(void* p,const AttackRequest32* q,AttackResponse16* out,const dh2_script_callback_scope* scope,std::string&){
  auto& t=*static_cast<Fixture*>(p);assert(!scope);
  if(q->service==attack_diagnostic){if(t.reject_diagnostic)return -1;out->word=0;return 0;}
  if(q->service==attack_frontal_angle){out->word=90;return 0;}return -1;
 }
 static int remote(void*,bool& out){out=false;return 0;}
 static int position(void* p,std::uintptr_t,const float*& out){out=static_cast<Fixture*>(p)->target.data();return 0;}
 static int look_vector(void* p,const float*& out){out=static_cast<Fixture*>(p)->look.data();return 0;}
 static int path_policy(void*,std::uint32_t* out){*out=1;return 0;}
 static bool heading_remote(void*,bool& out,std::string&){out=false;return true;}
 static bool physics(void*,bool& out,std::string&){out=false;return true;}
 static bool raise(void*,std::uint32_t,std::string&){return false;}
 static bool no_target(void*,const float*& out,std::string&){out=nullptr;return true;}
};
int main(int argc,char** argv){
 if(argc!=3)return 2;
 auto bres=read(argv[1]),dwld=read(argv[2]);resources::BresView view{};std::string error;world::Level level;
 assert(dh2_bres_open(&view,bres.data(),bres.size())==resources::BresError::ok);
 if(!world::load(view,dwld.data(),dwld.size(),level,error)){std::cerr<<error<<"\n";return 3;}
 data::AiTables ai;ai.rows.resize(9);ai.rows[0].type=1;ai.rows[1].type=4;
 ai.rows[0].melee_radius=ai.rows[1].melee_radius=20;ai.factions={{{0,1},{1,-1}},{{0,-1},{1,1}}};
 CharacterWorldRuntimeV1 world(ai,16);Actor npc(0x100000006ull,1,1),enemy(0x100000001ull,0,0);
 Fixture f;f.facts.idle=2;f.facts.attack_static=7;f.facts.attack_moving=8;
 Services bodies{&f,Fixture::body};StateOwnerBehaviorPredicate8 predicates{};WorldNpcStateServicesV1 services{};
 services.facts=&f.facts;services.bodies=&bodies;services.predicates=&predicates;services.remaining_methods={&f,Fixture::remaining};
 CharacterWorldNpcStateOwnerV1 machine(npc.search.identity,services);npc.machine=&machine.state();
 assert(machine.initialize_level("Idle")==1);assert(machine.state().flags==0x2380);
 assert(!world.add(npc.registration(6))&&!world.add(enemy.registration(1)));
 TargetOwner16 target_owner{npc.search.identity,0,0,0};TargetState48 target{};target.owner=&target_owner;
 TargetBindings48 targets{&target,{&f,Fixture::target_service},nullptr,{}};
 ControllerCommandState32 controller{3,npc.search.identity,0,0,0,0};AnimationAIState96 animation_ai{};
 animation_ai.owner=npc.search.identity;animation_ai.owner_byte14a8=-1;
 NpcAttackCommandOwnerV1 attack({&world,&targets,&machine,&controller,&animation_ai,nullptr,nullptr},
  {&f,Fixture::is_online,Fixture::backend,{&f,Fixture::query},Fixture::radius},16);
 assert(attack.command(enemy.search.identity,nullptr)==0);
 assert(machine.state().current==5&&machine.state().flags==0x2341&&machine.state().current_animation==7);
 assert(target.target==enemy.search.identity&&animation_ai.target==target.target&&!targets.scope);
 assert(!f.requests.empty()); // Source Focus/Blur bodies ran through real FSM.
 controller.locked=1;const auto calls=f.online_calls;assert(!attack.command(0,nullptr)&&f.online_calls==calls);controller.locked=0;
 f.online=true;assert(attack.command(0,nullptr)<0&&attack.error().find("network byte")!=std::string::npos);f.online=false;
 assert(machine.event(0x22,0)==1&&machine.state().current==3); // Explicit fixture event, not manufactured animator delivery.
 f.reject_diagnostic=true;assert(attack.command(enemy.search.identity,nullptr)<0);assert(machine.state().current==3);f.reject_diagnostic=false;
 // Actual cache floor and same NPC Runtime/PF/World/position frame composition.
 actor::RuntimeState runtime{};const auto& tri=level.native_floor->records[0]->triangles[0];
 for(unsigned i=0;i<3;++i){npc.search.position[i]=(tri.points[0][i]+tri.points[1][i]+tri.points[2][i])/3.f;f.target[i]=(tri.points[0][i]+tri.points[1][i]*2+tri.points[2][i])/4.f;}
 CharacterHeadingOwnerV1 heading(controller,machine.state(),target,targets.services,runtime.controller,runtime.path,npc.search.position,runtime.subobjects.destination,nullptr,{&f,nullptr,Fixture::heading_remote,Fixture::physics,Fixture::raise});
 std::uint8_t static84=0;std::uint32_t limit26c=0;NpcScriptCommandsV1 commands({npc.search.identity,&controller,&runtime,&target,npc.search.position,world::canonical_vec3_k_v1().data(),&static84,&limit26c,&heading,level.native_floor.get()},
  {&f,Fixture::remote,Fixture::position,Fixture::look_vector,nullptr,nullptr,Fixture::path_policy});
 navigation::ObstacleEntry entries[16]{};std::uint32_t buckets[16]{};navigation::ObstacleRegistry registry{entries,0,16,buckets,0,16};
 navigation::ProducerFields fields{navigation::ProducerClass::character,0,0,0,{-1,-1},{1,1}};
 assert(commands.initialize_pf(registry,fields));
 auto& bindings=commands.bindings();dh2_script_value object{};object.type=DH2_SCRIPT_IDENTITY;object.identity=enemy.search.identity;std::uint32_t returned{};
 assert(dh2_character_script_command(bindings.state,script_head_to,&object,1,&bindings.services,nullptr,0,&returned)==1&&runtime.path.count);
 std::vector<navigation::PathSegment> segments(level.native_floor->graph.node_count+1);
 navigation::AvoidanceActor actors[16]{};std::uint32_t floor_scratch[16]{};
 navigation::ControllerWorkspace workspace{segments.data(),std::uint32_t(segments.size()),0,actors,16,0,floor_scratch,16,0};
 navigation::MotionPolicy motion{};assert(!dh2_nav_motion_policy_defaults(&motion));std::uintptr_t auxiliary=0;
 // No visual/body is a declared fixture branch. No positive root-motion claim.
 assert(machine.event(0xc351,0)==1&&machine.state().current==4);actor::RuntimeResult result{};
 NpcActorFrameBorrowV1 frame{npc.search.identity,&runtime,&machine.state(),npc.words,npc.search.position,&commands,&world,nullptr,nullptr,nullptr,&registry,&motion,&workspace,nullptr,&auxiliary,0,0};
 assert(!npc_actor_frame_v1(result,frame,{&f,nullptr,nullptr,Fixture::no_target,nullptr},16,error));
 assert(result.phase==actor::completed&&runtime.object.user==npc.search.identity);
 assert(commands.suspend_navigation());assert(npc_actor_frame_v1(result,frame,{&f,nullptr,nullptr,Fixture::no_target,nullptr},16,error));
 std::cout<<"PASS NPC Cmd_Attack→same target/native FSM c354 focus; controller/network/failure/scoped restore; actual-cache same-runtime actor frame and suspended-floor boundary. External AI/geometry/body fixtures explicit.\n";
}
