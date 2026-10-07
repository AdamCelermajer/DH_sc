#pragma once
#include "character_loot_item_manager_v8.hpp"
#include "loot_inventory_source_v9.hpp"
namespace dh2::character {
struct LootItemSourceRequestV9 {LootItemRequestV8 source;LootInventorySourceV9* inventory{};};
struct LootItemSourceServicesV9 {
 void* context{};
 bool(*invoke)(void*,const LootItemSourceRequestV9&,std::string&){};
};
}
