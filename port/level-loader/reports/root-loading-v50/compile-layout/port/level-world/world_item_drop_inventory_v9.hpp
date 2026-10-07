#pragma once
#include "character_loot_item_source_v9.hpp"
namespace dh2::character {
class WorldLootItemRuntimeV1;struct CharacterMenuMutationServicesV4;
struct WorldItemDropInventoryServicesV9 {
 void* context{};
 bool(*is_character)(void*,std::uintptr_t,bool&,std::string&){};
 // Actual PlayerManager.GetByCharacter(false)->friendly678, signed16 store.
 bool(*friendly_index)(void*,std::uintptr_t,std::int32_t&,std::string&){};
};
// Installs only the real world endpoint; caller owns SAME145 runtime for this
// world and keeps it alive through synchronous authored menu dispatch.
void bind_menu_drop_world_v9(CharacterMenuMutationServicesV4&,std::shared_ptr<void>,
 WorldLootItemRuntimeV1&,WorldItemDropInventoryServicesV9);
}
