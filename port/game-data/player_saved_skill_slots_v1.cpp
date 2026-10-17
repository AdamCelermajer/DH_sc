#include "player_saved_skill_slots_v1.hpp"

namespace dh2::player_saved_skill_slots_v1 {

std::int32_t source_current_skill_set(
    const data::FreshInventoryOwnedV4&, std::int32_t) noexcept {
    // ItemInventory::GetCurrentSkillSet(int), source body 0x3fc6a0:
    // `mov r0,#0; bx lr`.
    return 0;
}

std::int32_t source_current_equipment_set(
    const data::FreshInventoryOwnedV4& inventory,
    std::int32_t selector) noexcept {
    // ItemInventory::GetCurrentEquipSet(int), source body 0x3fc6a8.
    // Negative selectors and 1..2 read the selected signed-byte projection;
    // selector 0 and selectors above 2 return zero.
    if (selector >= 0 && (selector == 0 || selector > 2)) return 0;
    return inventory.current_equipment();
}

BoundSlotsV1::BoundSlotsV1(data::PlayerSavegameV1& save,
                           data::FreshInventoryOwnedV4& inventory) noexcept
    : save_(&save), inventory_(&inventory) {}

bool BoundSlotsV1::owner_matches() const noexcept {
    return save_ && inventory_ && save_->character() != 0 &&
           inventory_->character() != 0 &&
           save_->character() == inventory_->character();
}

IntegerResult BoundSlotsV1::has_skill_slots() const noexcept {
    if (!owner_matches()) return {Status::owner_mismatch, -1};
    if (source_current_skill_set(*inventory_, -1) != 0)
        return {Status::unsupported_source_set, -1};
    return {Status::ok, save_->has_skill_slots() ? 1 : 0};
}

IntegerResult BoundSlotsV1::skill_in_slot(std::int32_t slot) const noexcept {
    if (!owner_matches()) return {Status::owner_mismatch, -1};
    if (source_current_skill_set(*inventory_, -1) != 0)
        return {Status::unsupported_source_set, -1};
    return {Status::ok, save_->skill_in_slot(slot)};
}

IntegerResult BoundSlotsV1::skill_slot(std::uint32_t row) const noexcept {
    if (!owner_matches()) return {Status::owner_mismatch, -1};
    if (source_current_skill_set(*inventory_, -1) != 0)
        return {Status::unsupported_source_set, -1};
    return {Status::ok, save_->skill_slot(row)};
}

Status BoundSlotsV1::set_skill_in_slot(
    std::int32_t slot, std::uint32_t row,
    const data::SavedSkillUpdateServicesV1& services,
    std::string& error) {
    if (!owner_matches()) {
        error = "saved slots and inventory belong to different Characters";
        return Status::owner_mismatch;
    }
    if (source_current_skill_set(*inventory_, -1) != 0) {
        error = "unsupported source skill-set selector result";
        return Status::unsupported_source_set;
    }
    return save_->set_skill_in_slot(slot, row, services, error)
               ? Status::ok
               : Status::source_rejected;
}

}  // namespace dh2::player_saved_skill_slots_v1
