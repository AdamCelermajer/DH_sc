#include "player_info_character_removal_tail_v1.hpp"

#include "netstruct_members_v1.hpp"

namespace dh2::player_info_character_removal_tail_v1 {
namespace {
namespace p = player_info_record_v1;
namespace n = netstruct_members_v1;

Status scalar(p::Record& record, std::uint32_t offset, std::int32_t value,
              std::int32_t residue) {
    return p::set_character_scalar(record, offset, value, residue) == n::Status::complete
        ? Status::complete : Status::member_unavailable;
}

Status record_name(p::Record& record) {
    return p::set_character_name(record, {}) == n::Status::complete
        ? Status::complete : Status::member_unavailable;
}

Status clear_character_bytes(p::Record& record) {
    auto* member = record.at(0x3b0);
    if (!member || !member->constructed || member->kind != n::Kind::byte_array ||
        member->bytes.size != 36)
        return Status::member_unavailable;
    return n::set_buffer(*member, {nullptr, 0}) == n::Status::complete
        ? Status::complete : Status::member_unavailable;
}

Status set_ingame_false(p::Record& record) {
    auto* member = record.at(0x4c8);
    if (!member || !member->constructed || member->kind != n::Kind::boolean ||
        member->header.size_bits != 1)
        return Status::member_unavailable;
    return n::set_bool(*member, 0) == n::Status::complete
        ? Status::complete : Status::member_unavailable;
}
}

Status remove_character_tail(
    p::Record& record, std::uintptr_t character_identity,
    std::uint8_t manager_byte_6c9, bool is_active,
    const p::ResetResidues& residues, Result* output) {
    Result local{};
    auto& result = output ? *output : local;
    result = {};
    if (!character_identity || manager_byte_6c9 > 1)
        return result.status = Status::invalid_argument;
    if (!record.constructed || record.busy)
        return result.status = Status::invalid_record;
    if (record.character_660 != character_identity)
        return result.status = Status::wrong_character;

    // PlayerManager::RemoveCharacter writes manager+0x6c9 into PlayerInfo+0x660
    // only on the zero-byte branch. Its _ManageCharacters state-0 caller
    // reaches this with zero, clearing the association after Character::Delete.
    if (!manager_byte_6c9) {
        record.character_660 = 0;
        result.character_link_cleared = true;
        ++result.source_member_writes;
    }

    // The source stores zero directly to PlayerInfo+0x684 and sends zero through
    // the existing NetStruct member at +0x4a0 before testing IsActive.
    record.field_684 = 0;
    ++result.source_member_writes;
    auto* field_4a0 = record.at(0x4a0);
    if (!field_4a0 || !field_4a0->constructed || field_4a0->kind != n::Kind::integer ||
        n::set_int(*field_4a0, 0) != n::Status::complete)
        return result.status = Status::member_unavailable;
    ++result.source_member_writes;

    if (!is_active) {
        result.status = Status::complete;
        return result.status;
    }

    // Exact active-PlayerInfo reset order from RemoveCharacter: class, name,
    // level, the 36-byte character buffer, XP, gold, then SetIngame(false).
    if (scalar(record, p::character_class, -1, residues.setters[2]) != Status::complete)
        return result.status = Status::member_unavailable;
    ++result.source_member_writes;
    if (record_name(record) != Status::complete)
        return result.status = Status::member_unavailable;
    ++result.source_member_writes;
    if (scalar(record, p::character_level, -1, residues.setters[0]) != Status::complete)
        return result.status = Status::member_unavailable;
    ++result.source_member_writes;
    if (clear_character_bytes(record) != Status::complete)
        return result.status = Status::member_unavailable;
    ++result.source_member_writes;
    if (scalar(record, p::character_xp, 0, residues.setters[4]) != Status::complete)
        return result.status = Status::member_unavailable;
    ++result.source_member_writes;
    if (scalar(record, p::character_gold, 0, residues.setters[5]) != Status::complete)
        return result.status = Status::member_unavailable;
    ++result.source_member_writes;
    if (set_ingame_false(record) != Status::complete)
        return result.status = Status::member_unavailable;
    ++result.source_member_writes;
    result.active_character_fields_reset = true;
    result.status = Status::complete;
    return result.status;
}

} // namespace dh2::player_info_character_removal_tail_v1
