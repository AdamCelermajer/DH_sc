#pragma once
#include "character_world_npc_object_v1.hpp"
#include "character_design_services.hpp"
#include <list>
namespace dh2::character {
struct WorldNpcRoomObjectV1 {
 std::uintptr_t identity{};
 WorldNpcObjectFieldsV1* fields{};
 CharacterWorldNpcObjectV1* lifecycle{};
 const float* position{}; // same Character+160; not a copied placement
};
struct WorldNpcRoomServicesV1 {
 void* context{};
 // Lookup actual retained RoomZone by original room-owner projection. No
 // arbitrary room selection or spatial fallback.
 class CharacterWorldNpcRoomV1* (*room)(void*,std::uintptr_t){};
};
// The source RoomZone+394 list owns list nodes, not Characters. World registry,
// AI update deque and PF obstacle registry remain their distinct source owners.
class CharacterWorldNpcRoomV1 {
 std::uintptr_t identity_;
 const float* bounds_; // same room absolute AABB [xmin,ymin,zmin,xmax,ymax,zmax]
 DebugSwitches* debug_;
 const DebugFileServices24* files_;
 WorldNpcRoomServicesV1 services_;
 std::list<std::uintptr_t> objects_;
 std::string error_;
public:
 CharacterWorldNpcRoomV1(std::uintptr_t,const float*,DebugSwitches*,const DebugFileServices24*,WorldNpcRoomServicesV1);
 CharacterWorldNpcRoomV1(const CharacterWorldNpcRoomV1&)=delete;
 CharacterWorldNpcRoomV1& operator=(const CharacterWorldNpcRoomV1&)=delete;
 void add(std::uintptr_t);    // whole RoomZone::AddObject 39672c, deduplicates
 void remove(std::uintptr_t);// whole RoomZone::RemoveObject 3968cc, first only;
                             // does not clear object's +2f4/+2ef
 // 1 accepted,0 null/nonzonable/outside, -1 missing producer/failure. Exact
 // Debug -> old room removal -> ownership fields -> ZoneEntered -> append.
 int add_initial(const WorldNpcRoomObjectV1*);
 const std::list<std::uintptr_t>& objects()const noexcept{return objects_;}
 const std::string& error()const noexcept{return error_;}
};
}
