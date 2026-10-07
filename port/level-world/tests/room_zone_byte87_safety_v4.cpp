#include "../canonical_room_zone_v3.hpp"
#include <cassert>
#include <iostream>
int main(){
 using namespace dh2;std::string error;auto lease=std::make_shared<int>(1);
 for(unsigned i=0;i<9;++i){
  actor::RuntimeState runtime{};world::CanonicalRoomZoneV3 zone(lease,runtime,{});
  auto borrow=zone.canonical(lease);std::uint8_t across=255;
  assert(borrow.read_across_rooms87(borrow.context,across,error)&&across==0);
  assert(*borrow.type_f4==11&&*borrow.room64==-1);
  assert(zone.base().lifecycle().static84==1&&zone.is_updatable());
  assert(zone.base().store_byte(0x87,1,error));
  assert(borrow.read_across_rooms87(borrow.context,across,error)&&across==1);
 }
 std::cout<<"RoomZone modern byte87 safety PASS 9 fresh receivers; no full Module lifecycle claim\n";
}
