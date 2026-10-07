#define main historical_geometry_main
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
#include "character_world_attack_geometry_v1_host.cpp"
#pragma GCC diagnostic pop
#undef main
#include "../character_path_commands.hpp"
#include <cstring>
struct LiveFacing : Actor {
 float desired{},controller_angle{};bool blocked{};
 LiveFacing(std::uintptr_t id,int faction,int type):Actor(id,faction,type){}
 static int live(void* raw,WorldTargetActorBorrowV1* b){auto& a=*static_cast<LiveFacing*>(raw);Actor::refresh(&a,b);b->heading_angle=&a.desired;b->controller_heading_angle=&a.controller_angle;return 0;}
 WorldActorRegistrationV1 live_registration(int key){auto r=registration(key);r.context=this;r.refresh=live;return r;}
};
int main(){
 data::AiTables ai;ai.rows.resize(9);ai.rows[0].type=1;ai.rows[1].type=4;ai.factions={{{0,1},{1,-1}},{{0,-1},{1,1}}};
 CharacterWorldRuntimeV1 world(ai,16);LiveFacing player(0x500000005ull,0,0),target(0x600000006ull,1,1);
 assert(!world.add(player.live_registration(1))&&!world.add(target.live_registration(2)));
 Context context{player,target};DebugFileServices24 files{nullptr,Context::open,Context::close};auto* switches=dh2_character_debug_create();
 CharacterWorldAttackGeometryV1 geometry(world,ai,*switches,files,{&context,Context::inventory,Context::object_kind,Context::interaction});
 std::string error;int value=77;unsigned checks=0;
 assert(geometry.close_range_v38(player.search.identity,0,0,value,error)&&value==0);++checks;
 player.words[32]=4;player.words[30]=10*256;player.words[31]=20*256;context.missing=true;
 target.search.position[0]=9;assert(geometry.close_range_v38(player.search.identity,target.search.identity,0,value,error)&&value==1&&context.inventory_calls==0);++checks;
 target.search.position[0]=10;assert(geometry.close_range_v38(player.search.identity,target.search.identity,0,value,error)&&value==0);++checks;
 target.node=1;target.enabled=1;target.cache[0]=2;
 assert(geometry.close_range_v38(player.search.identity,target.search.identity,0,value,error)&&value==1);++checks;
 auto services=world.targets().control_services_v38();
 for(unsigned heading=0;heading<4;++heading){
  const float points[4][3]{{0,-10,0},{10,0,0},{0,10,0},{-10,0,0}};
  std::memcpy(target.cache,points[heading],12);
  CharacterControlResponse16 out{};CharacterControlRequest32 q{};q.service=control_target_position;q.subject=target.search.identity;
  assert(services.invoke(services.context,&q,&out)==1&&!std::memcmp(out.position,target.cache,12));++checks;
  q.service=control_look_at_point;q.subject=player.search.identity;std::memcpy(q.position,out.position,12);
  LookAtState16 expected{{0,0,0},player.desired};assert(!dh2_character_look_at_point(&expected,q.position));
  assert(services.invoke(services.context,&q,&out)==1&&player.desired==expected.heading_angle&&player.controller_angle==expected.heading_angle);++checks;
 }
 // No cache fallback when node+enabled selects missing cache: remove receiver
 // instead of constructing an alternate position/heading owner.
 world.remove(target.search.identity);CharacterControlRequest32 q{};q.service=control_target_position;q.subject=target.search.identity;CharacterControlResponse16 out{};
 assert(services.invoke(services.context,&q,&out)<0);++checks;
 dh2_character_debug_destroy(switches);
 std::cout<<"target facing V38 SAME registered cache/heading, moving cardinal targets, removed handle, close-range inventory shortcut PASS "<<checks<<" checks; full camera/FX rendering not exercised\n";
}
