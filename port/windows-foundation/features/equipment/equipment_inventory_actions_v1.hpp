#pragma once
#include "../../character_state.hpp"
#include <cstdint>
#include <string>

// Details-panel Drop and Transmute owners (P14 EQUIP).
//
// Original: menu_InventorySheetDetails btn_Drop -> menu_confirm2 (GAMEPLAYMENUS_DROP_QUESTION) -> NativeInvDropItem(index)
// (IDA 0x43cfb4: ItemInventory::TransferItemTo(inv, index, temp, 1) then ItemObject::DropInventory: ONE unit becomes a
// world item at the player) and btn_GAMEPLAYMENUS_TRANSMUTE2 -> menu_confirm2 (GAMEPLAYMENUS_TRANSMUTE_QUESTION) ->
// NativeInvTransmuteItem(index) (IDA 0x43f23c -> Character::INV_TransmuteItem(index, false) 0x3a4a3c, then
// Character::INV_UpdateSkin). Both buttons are hidden/disabled for an equipped selection (authored-actions.txt
// 0001cbbe-0001cc44), so neither ever acts on a worn item.
namespace dh::foundation::equipment_menu {

// Publishes ONE unit (`one_unit.quantity == 1`) as a world item at the player. This is the narrow seam for the DROPS
// stream (loot world-item system). Until that stream is bound in this build it returns false with error
// "no world store bound", so the bag is never changed by a drop.
bool drop_item_to_world(const InventoryItem& one_unit, std::string& error);

using WorldDropFn = bool (*)(const InventoryItem& one_unit, std::string& error);

// Drop: refuses an equipped or unknown item, publishes one unit through `publish`, and removes that unit from the bag
// only after the publish succeeded (atomic). A failure is returned in `error` and logged by the caller, never a silent delete.
bool drop_inventory_item(CharacterState& character, const std::string& instance_id, std::string& error,
                         WorldDropFn publish = &drop_item_to_world);

// Character::INV_TransmuteItem(item, false): one unit leaves the bag (RemoveItem for quantity <= 1, otherwise
// AddQty(-1)) and `amount` gold is added (ItemInventory::AddGold). `amount` is the source ValueBox amount
// (RuntimeEquipmentTextProviderV1::transmute_value, min 1). Equipped items are refused. Atomic.
// Not reproduced (documented in the EQUIP report): CharProperties 213 counter and the "gear_transmute" trophy at 300,
// and the GAMEPLAYMENUS_TRANSMUTE_QUESTION confirmation popup (menu_confirm2 in dqcharmenu_droid.swf; not ported).
bool transmute_inventory_item(CharacterState& character, const std::string& instance_id, std::int32_t amount,
                              std::string& error);

}  // namespace dh::foundation::equipment_menu
