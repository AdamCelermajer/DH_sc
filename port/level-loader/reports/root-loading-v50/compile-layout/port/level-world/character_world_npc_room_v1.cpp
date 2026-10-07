#include "character_world_npc_room_v1.hpp"
#include <algorithm>
namespace dh2::character {
CharacterWorldNpcRoomV1::CharacterWorldNpcRoomV1(std::uintptr_t id,const float* bounds,
 DebugSwitches* debug,const DebugFileServices24* files,WorldNpcRoomServicesV1 services)
 :identity_(id),bounds_(bounds),debug_(debug),files_(files),services_(services){}
void CharacterWorldNpcRoomV1::add(std::uintptr_t id){if(std::find(objects_.begin(),objects_.end(),id)==objects_.end())objects_.push_back(id);}
void CharacterWorldNpcRoomV1::remove(std::uintptr_t id){auto i=std::find(objects_.begin(),objects_.end(),id);if(i!=objects_.end())objects_.erase(i);}
int CharacterWorldNpcRoomV1::add_initial(const WorldNpcRoomObjectV1* object){
 error_.clear();if(!object||!object->identity)return 0;
 if(!object->fields){error_="required source room object fields";return -1;}
 if(object->fields->non_zonable2ed)return 0;
 if(!object->position||!bounds_){error_="required same source Character position/RoomZone absolute AABB";return -1;}
 if(!(bounds_[0]<=object->position[0]&&object->position[0]<=bounds_[3]&&bounds_[1]<=object->position[1]&&object->position[1]<=bounds_[4]))return 0;
 std::uint32_t ignored{};
 if(!debug_||!files_||dh2_character_debug_load(debug_,files_)!=1||dh2_character_debug_get(&ignored,debug_,"isTracingRoomZoneInit",files_)!=1){
  error_="required source RoomZone initial Debug prefix";return -1;
 }
 auto& fields=*object->fields;
 if(fields.assigned2ef&&fields.room2f4){
  auto* old=services_.room?services_.room(services_.context,fields.room2f4):nullptr;
  if(!old){error_="required actual previous RoomZone owner";return -1;}old->remove(object->identity);
 }
 fields.room2f4=identity_;fields.assigned2ef=1;
 if(!object->lifecycle||!object->lifecycle->zone_entered()){
  error_=object->lifecycle?object->lifecycle->error():"required same source GameObject ZoneEntered";return -1;
 }
 objects_.push_back(object->identity);return 1;
}
}
