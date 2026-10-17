#pragma once
#include "fresh_inventory_owned_v4.hpp"

namespace dh2::data {

// Mirrors the authored inventory category selection over the existing Item
// row and Character property sheet. EquipmentSlots::Count (9) is valuables,
// not a request to show every inventory item.
bool inventory_slot_matches_v1(const ItemRecord164&, const PropertySheet&,
                               std::uint32_t slot) noexcept;

}
