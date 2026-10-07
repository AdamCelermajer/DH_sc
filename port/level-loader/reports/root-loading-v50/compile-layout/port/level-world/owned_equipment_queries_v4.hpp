#pragma once
#include "player_equipment_queries_v1.hpp"
#include "../game-data/fresh_inventory_owned_v4.hpp"
namespace dh2::player {
// Read the same selected equipment and property storage for any Character.
// No item allocation, equipment mutation, RNG draw, or player-only Gear owner.
bool owned_equipment_queries_v4(EquipmentQueries12V1&,
 const data::FreshInventoryOwnedV4&,std::uintptr_t,const std::int32_t*,std::string&);
}
