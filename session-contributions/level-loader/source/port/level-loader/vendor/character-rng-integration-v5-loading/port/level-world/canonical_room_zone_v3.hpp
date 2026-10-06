#pragma once
#include "game_object_initialization_owner_v1.hpp"
#include "game_object_set_position_v2.hpp"
#include "game_object_relative_box_v3.hpp"
namespace dh2::world {
struct RoomZoneServicesV3 {
 GameObjectInitializationServicesV1 game_object;
 GameObjectSetPositionServicesV2 position;
};
// Source factory11 -> RoomZoneC1 -> Zone(false,true) -> SAME GameObjectC2.
// DeclareProperties396554 is bx lr: generated instances have no property
// default substitution. They enter source Spawn's InitPost/condition path.
class CanonicalRoomZoneV3 {
 CanonicalGameObjectBaseOwnerV1 base_;
 RoomZoneServicesV3 services_;GameObjectInitializationOwnerV1 initialization_;
 std::array<float,3> dimensions_{};
 std::uint8_t physical380_{0},trigger381_{1},local388_{1},local389_{1},local390_{0};
 std::uintptr_t node384_{},module38c_{};
 std::vector<std::uintptr_t> occupants_; // Original empty394/398 list.
public:
 CanonicalRoomZoneV3(std::shared_ptr<void>,actor::RuntimeState&,RoomZoneServicesV3);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 CanonicalPropertyActorV1 properties()noexcept{return base_.properties();}
 bool init_post(std::string&);
 // Actual RoomZone vtable964bd0 slot38 -> GameObject::IsUpdatable38aac0:
 // whole source body mov r0,#1; bx lr (not inferred from static84).
 bool is_updatable()const noexcept{return true;}
 bool init_bounds(const std::array<float,6>&,std::uintptr_t same_module,std::string&);
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 const std::array<float,3>& dimensions()const noexcept{return dimensions_;}
 std::uintptr_t module()const noexcept{return module38c_;}
 std::uint8_t* source_byte(std::uint32_t o)noexcept{
  switch(o){case 0x380:return &physical380_;case 0x381:return &trigger381_;case 0x388:return &local388_;case 0x389:return &local389_;case 0x390:return &local390_;default:return base_.byte(o);}
 }
};
}
