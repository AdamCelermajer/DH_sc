#include "equipment_inventory_actions_v1.hpp"
#include <algorithm>
#include <cstddef>

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

// One unit leaves the row: RemoveItem when this is the last unit, otherwise AddQty(-1).
void take_one_unit(CharacterState& character, std::size_t index) {
    if (character.inventory[index].quantity <= 1) character.inventory.erase(character.inventory.begin() + std::ptrdiff_t(index));
    else --character.inventory[index].quantity;
}
}  // namespace

bool drop_item_to_world(const InventoryItem&, std::string& error) {
    // DROPS stream owns the world-item store; not merged into this branch yet.
    error = "no world store bound";
    return false;
}

bool drop_inventory_item(CharacterState& character, const std::string& instance_id, std::string& error, WorldDropFn publish) {
    error.clear();
    const auto* owned = find_unequipped(character, instance_id, error);
    if (!owned) return false;
    InventoryItem one_unit = *owned;
    one_unit.quantity = 1;
    const auto index = std::size_t(owned - character.inventory.data());
    if (!publish || !publish(one_unit, error)) {
        if (error.empty()) error = "world drop failed";
        return false;
    }
    take_one_unit(character, index);
    return true;
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
    if (character.gold > UINT64_MAX - std::uint64_t(amount)) {
        error = "Gold overflow";
        return false;
    }
    const auto index = std::size_t(owned - character.inventory.data());
    character.gold += std::uint64_t(amount);
    take_one_unit(character, index);
    return true;
}

}  // namespace dh::foundation::equipment_menu
