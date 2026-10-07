#pragma once
#include <room_zone_runtime_v104.hpp>
#include <canonical_object_manager_v1.hpp>
namespace dh2::world {class CanonicalRoomZoneV3;}
namespace model_renderer {
struct CampaignGeneratedRoomNativeV92 {
 std::shared_ptr<void> owner; // independent authority, never containing World
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_zonable;
 std::function<bool(const dh2::world::RoomObjectBorrowV104&,bool,std::string&)> zone_transition;
 std::function<bool(std::uintptr_t,bool,std::string&)> occupant_transition;
 std::function<bool(const std::list<std::uintptr_t>&,bool,std::string&)> room_objects;
 std::function<bool(const char*,std::string&)> trace;
 std::function<bool(const dh2::world::CanonicalObjectBorrowV1&,bool,std::string&)> test_enable_condition;
 std::function<bool(std::string&)> null_handle_assertion;
 std::function<bool(dh2::world::CanonicalRoomZoneV3&,std::string&)> destroy_source;
};
}
