#include "character_world_target_pose_v2.hpp"
#include "character_target_search_v5.hpp"
#include "character_path_commands.hpp"
#include <array>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
using namespace dh2;
struct Fixture {
 target_search::Object48 player{};
 std::array<target_search::Object48,4> enemies{};
 std::array<target_search::Entry16,5> entries{};
 target_search::Room16 room{},end{};target_search::Registry8 registry{};
 Fixture(){
  player.identity=0x100000001ull;player.visible=1;
  const float points[4][3]{{0,-100,0},{100,0,0},{0,100,0},{-100,0,0}};
  for(unsigned i=0;i<4;++i){enemies[i].identity=0x200000001ull+i;enemies[i].visible=1;std::copy_n(points[i],3,enemies[i].position);}
  room={&end,&entries[4]};end={&room,nullptr};registry.rooms=&end;
  for(unsigned i=0;i<4;++i)entries[i]={i==3?&entries[4]:&entries[i+1],&enemies[i]};
  entries[4]={&entries[0],nullptr};
 }
 static int query(void* raw,const target_search::Request24* q,target_search::Response16* out){
  auto& f=*static_cast<Fixture*>(raw);*out={};
  if(q->service==target_search::resolve_character){
   if(q->subject==f.player.identity)out->word=reinterpret_cast<std::uintptr_t>(&f.player);
   for(auto& e:f.enemies)if(q->subject==e.identity)out->word=reinterpret_cast<std::uintptr_t>(&e);
   return 0;
  }
  if(q->service==target_search::is_character||q->service==target_search::is_interactive||q->service==target_search::is_enemy){out->word=1;return 0;}
  if(q->service==target_search::is_dead||q->service==target_search::is_zonable)return 0;
  if(q->service==target_search::is_player){out->word=q->subject==f.player.identity;return 0;}
  if(q->service==target_search::melee_radius||q->service==target_search::interaction_radius){out->number=5;return 0;}
  return -1;
 }
};
int main(){
 Fixture f;target_search::Services16 services{&f,Fixture::query};
 std::array<target_search::Target24,8> heap{};target_search::List40 list{};
 assert(target_search::dh2_target_list_init(&list,heap.data(),heap.size(),&f.player,2,&services)==0);
 actor::RotationState rotation{};unsigned checks=0;
 for(unsigned direction=0;direction<4;++direction){
  rotation.rotation[2]=float(direction)*1.5707963705062866211f;
  rotation.heading_angle=rotation.rotation[2]+1.5707963705062866211f;
  f.player.has_target_position=0;f.player.character_word1314=42;
  const float actual_position[3]{};
  assert(character::skills::character_world_target_pose_v2(f.player,f.player.identity,actual_position,&rotation)==0);
  assert(f.player.rotation==rotation.rotation[2]&&f.player.rotation!=rotation.heading_angle&&f.player.character_word1314==42);
  // Normal attack uses the source live-center overload; skill Lua Search uses
  // the source snapshot-origin successor. Both consume this same owner Euler.
  assert(target_search::dh2_target_search(&list,&f.registry,200,.3f,&services)==0);
  assert(list.count==1&&list.heap[0].identity==f.enemies[direction].identity);
  assert(target_search::dh2_target_search_snapshot_v5(&list,&f.registry,200,.3f,actual_position,&services,nullptr)==0);
  assert(list.count==1&&list.heap[0].identity==f.enemies[direction].identity);checks+=4;
 }
 // Source LookAt writes desired heading; actual Euler changes during original
 // GameObject rotation update. A projection must not use the desired field.
 rotation={};character::LookAtState16 aim{};
 assert(dh2_character_look_at_point(&aim,f.enemies[1].position)==0);
 rotation.heading_angle=aim.heading_angle;
 assert(character::skills::character_world_target_pose_v2(f.player,f.player.identity,aim.position,&rotation)==0);
 assert(f.player.rotation==0);
 actor::RotationPolicy policy{1,250,1,1};std::uint32_t sync=0;
 assert(dh2_actor_update_rotation(&rotation,&policy,&sync)==0&&sync==1);
 assert(character::skills::character_world_target_pose_v2(f.player,f.player.identity,aim.position,&rotation)==0);
 assert(f.player.rotation==rotation.heading_angle);
 assert(target_search::dh2_target_search(&list,&f.registry,200,.3f,&services)==0);
 assert(list.count==1&&list.heap[0].identity==f.enemies[1].identity);checks+=6;
 auto before=f.player;
 assert(character::skills::character_world_target_pose_v2(f.player,f.player.identity,aim.position,nullptr)<0);
 assert(!std::memcmp(&before,&f.player,sizeof(before)));++checks;
 // Actual source payload offsets and signed gate, shared by both Search
 // entrypoints. Hidden candidate must not win merely because projections0.
 std::array<std::int32_t,224> props{};props[198]=-7;props[199]=-3;
 assert(character::skills::character_world_target_cached_fields_v2(f.player,props.data(),props.size())==0);
 assert(f.player.character_word1310==-7&&f.player.character_word1314==-3);checks+=2;
 rotation.rotation[2]=0;
 assert(character::skills::character_world_target_pose_v2(f.player,f.player.identity,aim.position,&rotation)==0);
 f.enemies[0].character_word1310=-2;
 assert(target_search::dh2_target_search(&list,&f.registry,200,.3f,&services)==0&&list.count==0);
 assert(target_search::dh2_target_search_snapshot_v5(&list,&f.registry,200,.3f,aim.position,&services,nullptr)==0&&list.count==0);checks+=2;
 props[199]=-2;assert(character::skills::character_world_target_cached_fields_v2(f.player,props.data(),props.size())==0);
 assert(target_search::dh2_target_search(&list,&f.registry,200,.3f,&services)==0&&list.count==1);++checks;
 before=f.player;
 assert(character::skills::character_world_target_cached_fields_v2(f.player,props.data(),199)<0);
 assert(!std::memcmp(&before,&f.player,sizeof(before)));++checks;
 std::cout<<"shared same-actor Euler projection: "<<checks<<" checks PASS; normal attack/skill Search/facing frame\n";
}
