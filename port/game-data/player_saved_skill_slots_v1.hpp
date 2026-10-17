#pragma once

#include "fresh_inventory_owned_v4.hpp"
#include "player_savegame_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::player_saved_skill_slots_v1 {

enum class Status : std::uint32_t {
    ok,
    owner_mismatch,
    unsupported_source_set,
    source_rejected,
};

struct IntegerResult {
    Status status{Status::owner_mismatch};
    std::int32_t value{-1};
};

// Exact ItemInventory selector projections used by the saved-slot route.
// The source skill-set getter is a constant-zero body and does not consult the
// selected equipment set. Equipment-set selection is a separate source getter.
std::int32_t source_current_skill_set(
    const data::FreshInventoryOwnedV4&, std::int32_t selector) noexcept;
std::int32_t source_current_equipment_set(
    const data::FreshInventoryOwnedV4&, std::int32_t selector) noexcept;

// Borrows the existing saved owner and the same Character's existing V4
// inventory. Every operation revalidates both full-width identities because
// PlayerSavegameV1::set_character remains mutable. The source's current skill
// set selector resolves to map zero even while the inventory equipment set is
// swapped; no saved map or inventory is owned here.
class BoundSlotsV1 {
public:
    BoundSlotsV1(data::PlayerSavegameV1&,
                 data::FreshInventoryOwnedV4&) noexcept;
    BoundSlotsV1(const BoundSlotsV1&) = delete;
    BoundSlotsV1& operator=(const BoundSlotsV1&) = delete;
    BoundSlotsV1(BoundSlotsV1&&) = delete;
    BoundSlotsV1& operator=(BoundSlotsV1&&) = delete;

    bool owner_matches() const noexcept;
    IntegerResult has_skill_slots() const noexcept;
    IntegerResult skill_in_slot(std::int32_t slot) const noexcept;
    IntegerResult skill_slot(std::uint32_t row) const noexcept;
    Status set_skill_in_slot(std::int32_t slot, std::uint32_t row,
                             const data::SavedSkillUpdateServicesV1&,
                             std::string& error);

private:
    data::PlayerSavegameV1* save_{};
    data::FreshInventoryOwnedV4* inventory_{};
};

}  // namespace dh2::player_saved_skill_slots_v1
