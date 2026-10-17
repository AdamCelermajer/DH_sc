#pragma once

#include "character_group_respawn.hpp"

#include <cstdint>

namespace dh2::character_group_limbus_blur {

struct Result {
    std::uint32_t member_count_snapshot;
    std::uint32_t members_queried;
    std::uint32_t owner_entries_skipped;
    std::uint32_t any_other_in_limbus;
    std::int32_t group_status_after;
    std::uint32_t group_status_written;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument = 1,
    member_query_failed = 2,
};

// Source role-3 branch of CSLimbus::OnBlur, after its Revive call. Other roles
// return without reading GroupInfo or invoking member services. Role 3 captures
// member count once, reloads the member pointer for each index, skips the owner,
// and queries every other entry in source order, retaining duplicates. Any
// other Limbus member writes status 2; none writes 0, including an empty group.
//
// Reuses the existing GroupState and SM_IsInLimbus service projections. The
// caller keeps the group, service context, actors and the entire captured-count
// member range live until return. A callback may change the member pointer only
// to another range valid for that captured count; changes to member_count do
// not alter this invocation's source loop. No actor or group ownership is added.
// Errors preserve the outer result and make no kernel-owned status write;
// callback effects and exceptions are not rolled back.
Status after_revive(std::uintptr_t owner_identity, std::int32_t group_role,
                    dh2::character_group::GroupState* group,
                    const dh2::character_group::Services* services,
                    Result* result);

}  // namespace dh2::character_group_limbus_blur
