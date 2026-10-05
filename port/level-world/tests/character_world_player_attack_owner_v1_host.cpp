#include "../character_world_player_attack_owner_v1.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;using namespace dh2::character;using namespace dh2::character::skills;
struct Actor {
 target_search::Object48 search{};SkillTargetCharacterV6 target{};
 std::int32_t words[224]{};data::CombatActorState life{};State machine{};
 scene::Scene scene{};target_providers::Handle16 handle{};std::uintptr_t current{},node{};
 std::uint8_t enabled{};float cache[3]{},angle{},controller_angle{};unsigned died_calls{};
 static int refresh(void* p,WorldTargetActorBorrowV1* out){auto& a=*static_cast<Actor*>(p);*out={a.search.identity,&a.search,&a.target,&a.scene,&a.life,&a.node,&a.enabled,a.cache,a.search.position,&a.angle,&a.controller_angle,nullptr,nullptr};return 0;}
 static int command(void* p,ControllerCommandState32* out){auto& a=*static_cast<Actor*>(p);*out={1,a.search.identity,0,0,0,0};return 0;}
 static int died(void* p,std::uintptr_t id){auto& a=*static_cast<Actor*>(p);assert(a.current==id);++a.died_calls;a.current=0;return 0;}
 WorldActorRegistrationV1 registration(int key){return {search.identity,key,this,refresh,&machine,&handle,&current,command,nullptr,nullptr,died};}
 Actor(std::uintptr_t id,int faction,int type){search.identity=id;search.visible=1;words[0]=faction;words[1]=type;target={id,words,"fixture",0,0,0,1,1};machine.flags=0x2000;}
};

struct Backend {
 State* machine{};unsigned transitions{};int reject{-1};
 static bool radius(void*,std::uintptr_t,float& value,std::string&){value=20;return true;} // Named geometry fixture.
 static bool query(void* p,std::uintptr_t,std::uintptr_t,WorldAIAttackQueryV1 q,std::int32_t& out,std::string&){
  auto& b=*static_cast<Backend*>(p);
  if(q==WorldAIAttackQueryV1::IsInMeleeRange&&b.reject==attack_current_in_melee)return false;
  out=q==WorldAIAttackQueryV1::CharacterCanRangeAttack?0:1;return true;
 }
 static int invoke(void* p,const AttackRequest32* q,AttackResponse16* out){
  auto& b=*static_cast<Backend*>(p);if(static_cast<int>(q->service)==b.reject)return -1;
  switch(q->service){
  case attack_owner_ranged:case attack_diagnostic:case attack_network_mode:out->word=0;return 0;
  case attack_frontal_angle:out->word=90;return 0;
  case attack_can_attack_current:case attack_current_in_melee:out->word=1;return 0;
  case attack_set_attack_state:++b.transitions;b.machine->current=5;b.machine->flags=0x2341;return 0;
  default:return -1;
  }
 }
 static int target(void*,TargetState48*,const TargetRequest24* q,std::uint32_t* out){
  switch(q->service){case target_debug_load:case target_debug_query:*out=0;return 0;
   case target_owner_ai_id:*out=0;return 0;case target_virtual_dead:*out=0;return 0;
   case target_in_sight:*out=1;return 0;default:return -1;}
 }
};
int main(){
 data::AiTables ai;ai.rows.resize(9);ai.rows[0].type=1;ai.rows[1].type=4;
 ai.rows[0].melee_radius=ai.rows[1].melee_radius=20;
 ai.factions={{{0,1},{1,-1}},{{0,-1},{1,1}}};
 CharacterWorldRuntimeV1 world(ai,16);Actor player(0x100000001ull,0,0),far(0x200000002ull,1,1),near(0x300000003ull,1,1);
 far.search.position[1]=-10;near.search.position[1]=-5;
 assert(!world.add(player.registration(1)));assert(!world.add(far.registration(2)));assert(!world.add(near.registration(3)));
 player.machine.current=3;TargetOwner16 combo{player.search.identity,0,0,0};
 TargetState48 target{0x700000007ull,&combo,0,0,0,0,0,0,0,0};
 AttackState64 fields{};fields.owner=player.search.identity;fields.object_of_interest_type=-1;
 ControllerAttackState32 controller{player.search.identity,player.search.identity,0,0,0,0};
 Backend b{&player.machine};CharacterWorldPlayerAttackOwnerV1 owner(world,fields,target,controller,player.machine,{nullptr,Backend::target},{&b,Backend::invoke,{&b,Backend::query},Backend::radius},16);
 assert(!owner.command());assert(target.target==far.search.identity);assert(b.transitions==1);
 target.target=0;player.machine.current=3;player.machine.heading_active=1;
 assert(!owner.command());assert(target.target==near.search.identity);assert(b.transitions==2);
 // Original continued attack keeps current target rather than starting new FSM.
 assert(!owner.command());assert(target.target==near.search.identity&&b.transitions==2&&fields.continued==1);
 assert(fields.owner_flags528==player.machine.attack_gate&&player.machine.flags==0x2341);
 // Character+520 behavior flags bit1 cannot stand in for +528 attack gate.
 // The genuine gate bit rejects before IsAttacking/continued processing.
 player.machine.attack_gate=1;fields.continued=0;
 assert(!owner.command());assert(fields.continued==0&&b.transitions==2);
 player.machine.attack_gate=0;
 controller.locked=1;assert(!owner.command(far.search.identity));assert(target.target==near.search.identity&&b.transitions==2);
 controller.locked=0;player.machine.current=3;b.reject=attack_current_in_melee;
 assert(owner.command(far.search.identity)==2);assert(target.target==far.search.identity&&b.transitions==2);
 assert(!owner.error().empty()); // Actual SetTarget prefix retained; no accepted attack.
 b.reject=-1;target.target=0;player.machine.heading_active=0;far.life.dead=near.life.dead=1;
 fields.object_of_interest=near.search.identity;fields.object_of_interest_type=8;
 assert(!owner.command());assert(target.target==near.search.identity&&target.last_target==near.search.identity&&fields.seeking==1);
 std::cout<<"{\"validation\":\"PASS\",\"source_search_set_target_backup_controller\":true,\"failure_prefix_no_throw\":true,\"fixture_backend_not_production\":true}\n";
}
