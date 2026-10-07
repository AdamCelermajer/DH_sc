#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <string>
namespace dh2::world {
struct RoomObjectBorrowV104 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 const float* position160{};
 std::uintptr_t* room2f4{};
 std::uint8_t* initial_room2ef{};
 std::function<bool(bool&,std::string&)> is_zonable_c4;
};
// Native leaves borrow the existing camera, object, module and manager owners.
// The room keeps its original occupant list; this receipt is not a registry.
struct RoomZoneRuntimeServicesV104 {
 std::shared_ptr<void> owner;
 std::function<bool(const char*,std::string&)> trace;
 std::function<bool(std::uintptr_t,RoomObjectBorrowV104&,std::string&)> object;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> remove_from_room;
 std::function<bool(const RoomObjectBorrowV104&,bool,std::string&)> zone_transition;
 std::function<bool(std::uintptr_t,bool,std::string&)> occupant_transition;
 std::function<bool(const std::list<std::uintptr_t>&,bool,std::string&)> room_objects;
 std::function<bool(std::array<std::array<float,4>,6>&,std::string&)> camera_planes;
 std::function<bool(std::uintptr_t,std::uint8_t*&,std::shared_ptr<void>&,std::string&)> module_visited;
 std::function<bool(std::array<float,3>&,bool&,std::string&)> local_player_position;
 std::function<bool(std::string&)> count_visible_room;
};
}
