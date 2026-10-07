#pragma once
#include "character_menu_inventory_mutation_v1.hpp"
#include <functional>
namespace dh2::ui {
std::int32_t character_menu_transmute_value_v1(std::int32_t value,std::int32_t raw_bonus,std::int32_t multiplier)noexcept;
struct CharacterMenuItemActionsGraphV1 {
 std::shared_ptr<void> owner;
 data::FreshInventoryOwnedV4* inventory{};
 data::PropertyView* properties{};
 data::OwnedInventoryServicesV4 inventory_services;
 std::function<bool(std::int32_t&,std::string&)> transmute_multiplier;
 std::function<bool(bool&,std::string&)> is_local_player,is_player,online;
 std::function<bool(const char*,std::string&)> achievement;
 // Genuine source Character::Skin only, not full property recalculation.
 std::function<bool(std::string&)> skin;
 // Source constructs an empty ItemInventory, not a player-inventory clone.
 std::function<bool(std::unique_ptr<data::FreshInventoryOwnedV4>&,data::OwnedInventoryServicesV4&,std::string&)> drop_container;
 // Online packet producer must consume actual item/power IDs and character.
 std::function<bool(std::uintptr_t,const data::ItemInstanceV1&,std::string&)> drop_packet;
 // Original DropInventory receives temp, Character, Character, null.
 std::function<bool(data::FreshInventoryOwnedV4&,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::string&)> drop_world;
};
class CharacterMenuItemActionsV1 {
 CharacterMenuItemActionsGraphV1 graph_;bool running_{};
 bool ready(std::string&)const;
public:
 explicit CharacterMenuItemActionsV1(CharacterMenuItemActionsGraphV1 graph):graph_(std::move(graph)){}
 const CharacterMenuItemActionsGraphV1& bindings()const noexcept{return graph_;}
 bool transmute(std::uint32_t,bool preview,std::int32_t& value,std::string&);
 bool drop(std::uint32_t,std::string&);
};
}
