#pragma once
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
namespace dh2::world {
struct GameObjectZoningBorrowV108 {
 std::shared_ptr<void> owner;std::uintptr_t identity{};
 std::uint8_t *zoning2ee{},*inside2f0{};
 std::uintptr_t *room2f4{},*visual2d8{};
};
struct GameObjectZoningServicesV108 {
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> room_remove,room_add;
 std::function<bool(std::uintptr_t,std::uint8_t&,std::string&)> room_active389;
 std::function<bool(std::string&)> add_no_room,remove_no_room;
 std::function<bool(bool&,std::string&)> is_zonable;
 std::function<bool(std::uint8_t,std::string&)> set_updating;
 std::function<bool(std::uintptr_t,std::string&)> sync_visibility;
 std::function<bool(bool,std::string&)> qualified_zone_event;
};
bool gameobject_disable_zoning_v108(GameObjectZoningBorrowV108&,const GameObjectZoningServicesV108&,std::string&);
bool gameobject_enable_zoning_v108(GameObjectZoningBorrowV108&,const GameObjectZoningServicesV108&,std::string&);
}
