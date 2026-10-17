#include "character_level_member.hpp"

namespace dh2::character_level_member {
namespace {

void set_changed(IntMember* member, std::uint64_t* serial) {
    const std::uint32_t stamp = member->source_stamp;
    member->changed = 1;
    member->field_14 = stamp;
    member->field_10 = stamp;
    const std::uint64_t old_serial = *serial;
    member->revision = old_serial;
    *serial = old_serial + std::uint64_t{1};
}

}  // namespace

Status mark_changed(IntMember* member, std::uint64_t* serial) {
    if (!member || !serial) return Status::invalid_argument;
    set_changed(member, serial);
    return Status::complete;
}

Status set_value(IntMember* member, std::uint64_t* serial,
                 std::int32_t value, Result* result) {
    if (!member || !serial || !result) return Status::invalid_argument;
    *result = {};
    if (member->value != value) {
        member->value = value;
        set_changed(member, serial);
        result->member_marked = 1;
        result->member_value_changed = 1;
    }
    result->member_value_after = member->value;
    result->serial_after = *serial;
    return Status::complete;
}

Status set_character_level(State* state, std::int32_t value,
                           std::int32_t stack_residue, Result* result) {
    if (!state || !state->character_level_member ||
        !state->global_change_serial || !result)
        return Status::invalid_argument;

    *result = {};
    // PlayerInfo::SetCharacterLevel constructs a 0x24-byte temporary with
    // source_stamp=0, value=the pre-existing stack word and changed=0.
    IntMember temporary{};
    temporary.size_bits = 16;
    temporary.field_10 = UINT32_MAX;
    temporary.field_14 = UINT32_MAX;
    temporary.value = stack_residue;
    if (value != temporary.value) {
        temporary.value = value;
        set_changed(&temporary, state->global_change_serial);
        result->temporary_marked = 1;
    }

    Result member_result{};
    const Status status = set_value(state->character_level_member,
                                    state->global_change_serial,
                                    temporary.value, &member_result);
    if (status != Status::complete) return status;
    result->member_marked = member_result.member_marked;
    result->member_value_changed = member_result.member_value_changed;
    result->member_value_after = member_result.member_value_after;
    result->serial_after = member_result.serial_after;
    return Status::complete;
}

}  // namespace dh2::character_level_member
