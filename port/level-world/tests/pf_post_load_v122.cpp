#include "../module_pf_room_v3.hpp"
#include <cassert>
#include <iostream>
using namespace dh2;
namespace {
struct Fixture {
 std::shared_ptr<floors::World> world=std::make_shared<floors::World>();
 world::ModulePFRoomsV3 rooms{world,{}, {}};
 world::PFWorldFlushFieldsV1 fields;std::string error;
 Fixture(){auto native=rooms.native_storage_v106();assert(native->lend(*world,native->identity(),[](std::string&){return true;},fields,error));}
};
}
int main(){
 {
  Fixture f;assert(*f.fields.initialized4==0&&!f.world->sewn);
  assert(f.rooms.post_load_source_v122(f.error)&&*f.fields.initialized4==0&&!f.world->sewn);
  assert(f.world->records.empty()&&f.world->search_offsets.empty());
  // A state0 no-op must not suppress a later genuine initialization/PostLoad.
  assert(f.rooms.native_storage_v106()->initialize_for_load(f.error)&&*f.fields.initialized4==1);
  assert(f.rooms.post_load_source_v122(f.error)&&*f.fields.initialized4==2&&f.world->sewn);
  assert(f.rooms.rooms().empty()&&f.world->records.empty());
  assert(f.world->collision_world.room_count==0&&f.world->collision_world.floor_count==0);
  assert(f.world->search_offsets.size()==1&&f.world->search_offsets[0]==0);
  const auto* offsets=f.world->search_offsets.data();
  assert(f.rooms.post_load_source_v122(f.error)&&*f.fields.initialized4==2&&f.world->search_offsets.data()==offsets);
 }
 {
  Fixture f;*f.fields.initialized4=2;
  // Original state2 returns before inspecting even unavailable/malformed room storage.
  auto record=std::make_unique<floors::Record>();auto* same=record.get();f.world->records.push_back(std::move(record));
  assert(f.rooms.post_load_source_v122(f.error)&&*f.fields.initialized4==2&&!f.world->sewn&&f.world->records[0].get()==same);
 }
 {
  Fixture f;assert(f.rooms.native_storage_v106()->initialize_for_load(f.error));
  auto record=std::make_unique<floors::Record>();auto* same=record.get();f.world->records.push_back(std::move(record));
  const auto outer=*f.fields.outer44,inner=*f.fields.inner48;
  assert(!f.rooms.post_load_source_v122(f.error)&&*f.fields.initialized4==2&&!f.world->sewn);
  const auto first=f.error;assert(!first.empty()&&f.world->records[0].get()==same&&*f.fields.outer44==outer&&*f.fields.inner48==inner);
  // Repairing storage cannot turn a failed reached prefix into successful replay.
  f.world->records.clear();assert(!f.rooms.post_load_source_v122(f.error)&&f.error==first&&*f.fields.initialized4==2&&!f.world->sewn);
 }
 {
  Fixture f;auto foreign=std::make_shared<floors::World>();bool work=true;
  assert(!f.rooms.native_storage_v106()->begin_post_load_v122(*foreign,work,f.error)&&!work&&*f.fields.initialized4==0);
 }
 std::cout<<"PASS SAME PF PostLoad state0/1/2, empty world, state2 prefix and sticky failure\n";
}
