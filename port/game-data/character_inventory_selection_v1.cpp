#include "character_inventory_selection_v1.hpp"

namespace dh2::data {

bool inventory_slot_matches_v1(const ItemRecord164& row,
                               const PropertySheet& properties,
                               std::uint32_t slot) noexcept {
    const auto item_slot = row.words[26];
    // Adapted from Adam Celermajer's character_menu_inventory_order_v1 and
    // character_menu_queries_owner_v1 at 11fa5242. His authored query owner
    // and original SWF treat EquipmentSlots::Count as valuables; ItemTable
    // uses -1 for those entries.
    if (slot == 9) return item_slot == -1;
    if (slot >= 9 || item_slot == -1) return false;

    auto target = item_slot;
    const auto item_type = row.words[22];
    // Dual-wield capability moves ordinary one-hand weapons into both hand
    // lists. Item types 4 and 5 retain their authored main-hand-only category.
    if (item_type != 4 && item_type != 5 && target == 1 && properties[202])
        target = -3;
    if (target >= 0 && target < 9) return std::uint32_t(target) == slot;
    if (target == -3) return slot == 1 || slot == 2;
    if (target == -2) return slot == 5 || slot == 6;
    // Two-handed items are listed under the right-hand category only.
    return target == -4 && slot == 1;
}

}
