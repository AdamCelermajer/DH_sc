#include "../character_world_runtime_v1.hpp"
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
int main(){
 data::AiTables ai;ai.rows.resize(9);ai.rows[0].type=1;ai.rows[1].type=4;ai.rows[0].melee_radius=ai.rows[1].melee_radius=20;ai.factions={{{0,1},{1,-1}},{{0,-1},{1,1}}};
 CharacterWorldRuntimeV1 world(ai,16);Actor player(0x100000001ull,0,0),monster(0x200000002ull,1,1);monster.search.position[0]=10;
 assert(!world.add(player.registration(1)));assert(!world.add(monster.registration(2)));assert(world.add(monster.registration(2))==-1);
 assert(!world.refresh());std::uintptr_t relation{};assert(!world.relationship(player.search.identity,monster.search.identity,true,&relation)&&relation==1);assert(!world.relationship(player.search.identity,monster.search.identity,false,&relation)&&relation==0);
 auto native=world.native_world(player.search.identity,nullptr);target_search::Target24 heap[8]{};target_search::List40 list{};
 assert(!target_search::dh2_target_list_init(&list,heap,8,native.character,1,&native.search));assert(!target_search::dh2_target_search(&list,native.registry,100,3.141593f,&native.search));assert(list.count==1&&heap[0].identity==monster.search.identity);
 player.machine.flags=0x1234;assert(!world.refresh()&&player.target.flags520==0x1234);player.machine.flags=0x2000;
 assert(!world.targets().look_at(player.search.identity,monster.search.identity));assert(player.angle==player.controller_angle);
 world.begin_frame(9);target_providers::Handle16 cached{};assert(!world.get_handle(monster.search.identity,&cached));assert(monster.handle.frame==9);assert(!world.resolve(&cached,&relation)&&relation==monster.search.identity);
 monster.life.dead=1;player.current=monster.search.identity;assert(!world.notify_death(monster.search.identity));assert(!player.current&&player.died_calls==1);
 assert(!target_search::dh2_target_search(&list,native.registry,100,3.141593f,&native.search)&&list.count==0);assert(monster.target.dead1449==1);
 assert(!world.remove(monster.search.identity));assert(!world.resolve(&cached,&relation)&&!relation);assert(!monster.handle.key);assert(world.relationship(player.search.identity,monster.search.identity,true,&relation)!=0);
 auto missing=monster.registration(3);missing.machine=nullptr;assert(!world.add(missing));
 assert(!world.relationship(player.search.identity,monster.search.identity,true,&relation)&&relation==1);
 target_search::Request24 interactive_request{target_search::is_interactive,0,monster.search.identity,player.search.identity};target_search::Response16 response{};
 assert(!native.search.invoke(native.search.context,&interactive_request,&response)&&response.word==0); // dead prefix never reads flags
 monster.life.dead=0;assert(native.search.invoke(native.search.context,&interactive_request,&response)!=0); // live +520 requires actual FSM
 assert(world.notify_death(player.search.identity)==-1);world.clear();assert(world.handles().count==0);
 std::cout<<"{\"validation\":\"PASS\",\"whole_relationship_registration_handle_death_search\":true,\"same_state_flags_and_life\":true,\"source_death_callback\":true,\"above_4gib_identities\":true}\n";
}
