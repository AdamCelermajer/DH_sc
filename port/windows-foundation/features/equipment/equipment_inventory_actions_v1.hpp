#pragma once
#include "../../character_state.hpp"
#include <cstdint>
#include <functional>
#include <string>

// Details-panel Drop and Transmute owners (P14 EQUIP).
//
// Original: menu_InventorySheetDetails btn_Drop -> menu_confirm2 (GAMEPLAYMENUS_DROP_QUESTION) -> NativeInvDropItem(index)
// (IDA 0x43cfb4: ItemInventory::TransferItemTo(inv, index, temp, 1) then ItemObject::DropInventory) and
// btn_GAMEPLAYMENUS_TRANSMUTE2 -> menu_confirm2 (GAMEPLAYMENUS_TRANSMUTE_QUESTION) -> NativeInvTransmuteItem(index)
// (IDA 0x43f23c -> Character::INV_TransmuteItem(index, false) 0x3a4a3c, then Character::INV_UpdateSkin).
// Both buttons are hidden/disabled by displaySelectedItemInfos for an equipped selection (authored-actions.txt
// 0001cbbe-0001cc44), so neither ever acts on a worn item.
namespace dh::foundation::equipment_menu {

// Publishes ONE unit of `instance_id` as a world item at the player and removes it from the CharacterState, atomically.
// Implemented by the root over the DROPS stream's world-item system (loot::drop_item_to_world). Returns false with
// `error` set when unavailable; the CharacterState must then be unchanged.
using DropItemToWorldFn = std::function<bool(CharacterState&, const std::string& instance_id, std::string& error)>;

// Drop: refuses an equipped or unknown item, then defers to `drop_to_world`. A missing owner is a logged failure
// (the item stays in the bag), never a silent delete.
bool drop_inventory_item(CharacterState& character, const std::string& instance_id,
                         const DropItemToWorldFn& drop_to_world, std::string& error);

// Character::INV_TransmuteItem(item, false): one unit leaves the bag (RemoveItem for quantity <= 1, otherwise
// AddQty(-1)) and `amount` gold is added (ItemInventory::AddGold). `amount` is the source ValueBox amount
// (RuntimeEquipmentTextProviderV1::transmute_value, min 1). Equipped items are refused. Atomic.
// Not reproduced (documented in the EQUIP report): CharProperties 213 counter and the "gear_transmute" trophy at 300.
bool transmute_inventory_item(CharacterState& character, const std::string& instance_id, std::int32_t amount,
                              std::string& error);

}  // namespace dh::foundation::equipment_menu
