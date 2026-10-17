#pragma once

#include <cstdint>

namespace dh2::character_group {

// Logical view of the source CharAI::GroupInfo values consumed by
// CanRespawn. This is not an ARM32 object overlay and does not own members.
struct MemberRef {
    std::uintptr_t character_identity;
};

struct GroupState {
    std::int32_t respawn_status;       // source GroupInfo +0x24
    std::uint8_t limbus_respawn_gate;  // source GroupInfo +0x28
    std::uint8_t reserved[3];
    const MemberRef* members;          // source ordered vector at +0x18
    std::uint32_t member_count;
};

struct Services {
    void* context;
    // Implements each member's CharStateMachine::SM_IsInLimbus() query.
    // Return 0 on success and place the normalized bool in result.
    std::int32_t (*is_in_limbus)(void*, std::uintptr_t character_identity,
                                 std::uint32_t* result);
};

struct Result {
    std::uint32_t can_respawn;
    std::uint32_t members_queried;
    std::int32_t group_status_after; // meaningful only when observed is 1
    std::uint32_t group_status_observed; // unsupported states/Limbus do not read +0x24
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    member_query_failed = 2,
};

// Reconstructs CharAI::GroupInfo::CanRespawn(Character*) for the caller's
// state ID. Queries retain source member-vector order and duplicates. The
// sole source mutation is GroupInfo status 1 -> 2 when every member reports
// Limbus; all other paths preserve the incoming status.
Status can_respawn(std::int32_t character_state_id, GroupState* group,
                   const Services* services, Result* result);

}  // namespace dh2::character_group
