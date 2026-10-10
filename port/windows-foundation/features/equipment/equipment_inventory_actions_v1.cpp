#include "equipment_inventory_actions_v1.hpp"
#include <algorithm>

namespace dh::foundation::equipment_menu {
namespace {
const InventoryItem* find_unequipped(const CharacterState& character, const std::string& id, std::string& error) {
    const auto found = std::find_if(character.inventory.begin(), character.inventory.end(),
        [&](const InventoryItem& item) { return item.instance_id == id; });
    if (id.empty() || found == character.inventory.end() || found->quantity == 0) {
        error = "Inventory item is not owned";
        return nullptr;
    }
    for (const auto& binding : character.equipment)
        if (binding.item_instance_id == id) {
            error = "Equipped items cannot be dropped or transmuted (unequip first)";
            return nullptr;
        }
    return &*found;
}
}  // namespace

bool drop_inventory_item(CharacterState& character, const std::string& instance_id,
                         const DropItemToWorldFn& drop_to_world, std::string& error) {
    error.clear();
    if (!find_unequipped(character, instance_id, error)) return false;
    if (!drop_to_world) {
        error = "World item drop owner is unavailable in this build; the item stays in the bag";
        return false;
    }
    return drop_to_world(character, instance_id, error);
}

bool transmute_inventory_item(CharacterState& character, const std::string& instance_id, std::int32_t amount,
                              std::string& error) {
    error.clear();
    const auto* owned = find_unequipped(character, instance_id, error);
    if (!owned) return false;
    if (amount < 1) {
        error = "Transmute amount must be at least 1 (source clamp)";
        return false;
    }
    const auto index = std::size_t(owned - character.inventory.data());
    if (character.gold > UINT64_MAX - std::uint64_t(amount)) {
        error = "Gold overflow";
        return false;
    }
    character.gold += std::uint64_t(amount);
    if (character.inventory[index].quantity <= 1) character.inventory.erase(character.inventory.begin() + std::ptrdiff_t(index));
    else --character.inventory[index].quantity;
    return true;
}

}  // namespace dh::foundation::equipment_menu
