#include "character_group_respawn.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::character_group {
namespace {

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a <= b ? b - a < left_size : a - b < right_size;
}

bool valid_group_header(const GroupState* group, const Result* result) {
    return group != nullptr && result != nullptr &&
        group->reserved[0] == 0 && group->reserved[1] == 0 &&
        group->reserved[2] == 0 &&
        !overlaps(group, sizeof(*group), result, sizeof(*result));
}

bool valid_member_range(const GroupState* group, const Services* services,
                        const Result* result) {
    if ((group->member_count != 0 && group->members == nullptr) ||
        group->member_count > SIZE_MAX / sizeof(MemberRef)) {
        return false;
    }
    if (group->member_count == 0) return true;

    const auto size = static_cast<std::size_t>(group->member_count) * sizeof(MemberRef);
    if (overlaps(group->members, size, group, sizeof(*group)) ||
        overlaps(group->members, size, result, sizeof(*result)) ||
        (services != nullptr &&
         overlaps(group->members, size, services, sizeof(*services)))) {
        return false;
    }
    for (std::uint32_t i = 0; i < group->member_count; ++i) {
        if (group->members[i].character_identity == 0) return false;
    }
    return true;
}

}  // namespace

Status can_respawn(std::int32_t character_state_id, GroupState* group,
                   const Services* services, Result* result) {
    if (result == nullptr) return Status::invalid_argument;

    Result completed{0, 0, 0, 0};

    // The original switch reads Character state first and returns false
    // without dereferencing GroupInfo for every state other than 0 or 3.
    if (character_state_id != 0 && character_state_id != 3) {
        *result = completed;
        return Status::complete;
    }
    if (!valid_group_header(group, result)) return Status::invalid_argument;

    // Limbus actors use only the GroupInfo +0x28 gate.
    if (character_state_id == 0) {
        if (group->limbus_respawn_gate > 1) return Status::invalid_argument;
        completed.can_respawn = group->limbus_respawn_gate ^ 1u;
        *result = completed;
        return Status::complete;
    }

    completed.group_status_after = group->respawn_status;
    completed.group_status_observed = 1;

    if (group->respawn_status == 2) {
        completed.can_respawn = 1;
        completed.group_status_after = group->respawn_status;
        *result = completed;
        return Status::complete;
    }
    if (group->respawn_status != 1) {
        completed.group_status_after = group->respawn_status;
        *result = completed;
        return Status::complete;
    }

    if (!valid_member_range(group, services, result)) {
        return Status::invalid_argument;
    }

    if (group->member_count != 0 &&
        (services == nullptr || services->is_in_limbus == nullptr)) {
        return Status::invalid_argument;
    }

    bool all_in_limbus = true;
    for (std::uint32_t i = 0; i < group->member_count; ++i) {
        std::uint32_t in_limbus = 0;
        if (services->is_in_limbus(services->context,
                                   group->members[i].character_identity,
                                   &in_limbus) != 0 || in_limbus > 1) {
            return Status::member_query_failed;
        }
        ++completed.members_queried;
        if (in_limbus == 0) all_in_limbus = false;
    }

    if (all_in_limbus) {
        group->respawn_status = 2;
        completed.can_respawn = 1;
    }
    completed.group_status_after = group->respawn_status;
    *result = completed;
    return Status::complete;
}

}  // namespace dh2::character_group
