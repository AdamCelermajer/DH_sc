#pragma once
#include "game_object_initialization_owner_v1.hpp"
#include "game_object_set_position_v2.hpp"
#include "game_object_relative_box_v3.hpp"
#include "room_zone_runtime_v104.hpp"
namespace dh2::world {
struct RoomDestroyServicesV106 {
 std::shared_ptr<void> owner;
 std::function<bool(std::uintptr_t,std::string&)> node384_drop;
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::string&)> game_object_d2;
};
struct RoomZoneServicesV3 {
 GameObjectInitializationServicesV1 game_object;
 GameObjectSetPositionServicesV2 position;
};
// Source factory11 -> RoomZoneC1 -> Zone(false,true) -> SAME GameObjectC2.
// DeclareProperties396554 delegates to Zone397df8; InitPost396548 delegates
// to Zone39771c. Generated instances use the same inherited source lifecycle.
class CanonicalRoomZoneV3 {
 CanonicalGameObjectBaseOwnerV1 base_;
 RoomZoneServicesV3 services_;GameObjectInitializationOwnerV1 initialization_;
 std::array<float,3> dimensions_{};
 std::uint8_t physical380_{0},trigger381_{1},local388_{1},local389_{1},local390_{0};
 std::uintptr_t node384_{},module38c_{};
 std::list<std::uintptr_t> occupants_; // Original C1-empty394/398 list.
 RoomZoneRuntimeServicesV104 runtime_v104_;
 bool destroy_attempted_v106_{},destroy_complete_v106_{};std::string destroy_failure_v106_;
public:
 CanonicalRoomZoneV3(std::shared_ptr<void>,actor::RuntimeState&,RoomZoneServicesV3);
 CanonicalGameObjectBaseOwnerV1& base()noexcept{return base_;}
 CanonicalObjectBorrowV1 canonical(std::shared_ptr<void> lease){return base_.canonical(std::move(lease));}
 CanonicalPropertyActorV1 properties()noexcept{return base_.properties();}
 bool init_post(std::string&);
 bool source_destroy_v106(const RoomDestroyServicesV106&,std::string&);
 // Actual RoomZone vtable964bd0 slot38 -> GameObject::IsUpdatable38aac0:
 // whole source body mov r0,#1; bx lr (not inferred from static84).
 bool is_updatable()const noexcept{return true;}
 bool init_bounds(const std::array<float,6>&,std::uintptr_t same_module,std::string&);
 bool init_final(bool& eligible,std::string& e){return initialization_.init_final(eligible,e);}
 const std::array<float,3>& dimensions()const noexcept{return dimensions_;}
 bool bind_runtime_v104(RoomZoneRuntimeServicesV104,std::string&);
 bool source_add_object_v104(std::uintptr_t,std::string&);
 void source_remove_object_v104(std::uintptr_t)noexcept;
 bool source_add_initial_object_v104(std::uintptr_t,bool&,std::string&);
 bool source_init_object_list_v104(CanonicalObjectManagerV1&,
  const std::function<bool(const CanonicalObjectBorrowV1&,std::uintptr_t&,std::string&)>&,std::string&);
 bool source_activate_v104(bool,std::string&);
 bool source_update_v104(std::string&);
 bool source_has_inside_v104(const std::array<float,3>&)const noexcept;
 bool source_has_been_visited_v104(bool&,std::string&);
 bool source_set_visited_v104(bool,std::string&);
 bool source_set_visited_byte_v104(std::uint8_t,std::string&);
 const std::list<std::uintptr_t>& source_occupants394_v104()const noexcept{return occupants_;}
 std::uintptr_t module()const noexcept{return module38c_;}
 std::uint8_t* source_byte(std::uint32_t o)noexcept{
  switch(o){case 0x380:return &physical380_;case 0x381:return &trigger381_;case 0x388:return &local388_;case 0x389:return &local389_;case 0x390:return &local390_;default:return base_.byte(o);}
 }
};
}
