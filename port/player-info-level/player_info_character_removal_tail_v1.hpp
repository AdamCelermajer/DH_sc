#pragma once

#include "player_info_record_v1.hpp"

#include <cstdint>

namespace dh2::player_info_character_removal_tail_v1 {

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    wrong_character,
    invalid_record,
    member_unavailable,
};

struct Result {
    Status status{Status::complete};
    std::uint32_t source_member_writes{};
    bool character_link_cleared{};
    bool active_character_fields_reset{};
};

// PlayerInfo tail of PlayerManager::RemoveCharacter after the source Character
// side effects and current-Level/camera branch have completed. The caller must
// pass the canonical PlayerInfo record returned by GetPlayerByCharacter for
// this exact Character. `manager_byte_6c9` is the reached manager byte; source
// stores it into +0x660 only when it is zero. `is_active` is the actual
// PlayerInfo::IsActive result; PlayerManager::Update's supported offline path
// supplies true, while online callers must query the source branch.
Status remove_character_tail(
    player_info_record_v1::Record&,
    std::uintptr_t character_identity,
    std::uint8_t manager_byte_6c9,
    bool is_active,
    const player_info_record_v1::ResetResidues&,
    Result*);

} // namespace dh2::player_info_character_removal_tail_v1
