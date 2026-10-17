#include "character_group_limbus_blur.hpp"

#include <cstddef>
#include <limits>

namespace dh2::character_group_limbus_blur {
namespace {

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a <= b ? b - a < left_size : a - b < right_size;
}

bool valid_header(const dh2::character_group::GroupState* group,
                  const dh2::character_group::Services* services,
                  const Result* result) {
    return group != nullptr &&
           !overlaps(group, sizeof(*group), result, sizeof(*result)) &&
           (services == nullptr ||
            !overlaps(group, sizeof(*group), services, sizeof(*services))) &&
           group->reserved[0] == 0 && group->reserved[1] == 0 &&
           group->reserved[2] == 0;
}

bool valid_range(const dh2::character_group::GroupState* group,
                 std::uint32_t captured_count,
                 const dh2::character_group::Services* services,
                 const Result* result) {
    if (captured_count == 0) return true;
    if (group->members == nullptr ||
        captured_count > SIZE_MAX / sizeof(dh2::character_group::MemberRef)) {
        return false;
    }
    const auto bytes = static_cast<std::size_t>(captured_count) *
                       sizeof(dh2::character_group::MemberRef);
    const auto address = reinterpret_cast<std::uintptr_t>(group->members);
    if (address > std::numeric_limits<std::uintptr_t>::max() - bytes) return false;
    return !overlaps(group->members, bytes, group, sizeof(*group)) &&
           !overlaps(group->members, bytes, result, sizeof(*result)) &&
           (services == nullptr ||
            !overlaps(group->members, bytes, services, sizeof(*services)));
}

}  // namespace

Status after_revive(std::uintptr_t owner_identity, std::int32_t group_role,
                    dh2::character_group::GroupState* group,
                    const dh2::character_group::Services* services,
                    Result* result) {
    if (owner_identity == 0 || result == nullptr ||
        (services != nullptr &&
         overlaps(services, sizeof(*services), result, sizeof(*result)))) {
        return Status::invalid_argument;
    }
    Result completed{};
    if (group_role != 3) {
        *result = completed;
        return Status::complete;
    }
    if (!valid_header(group, services, result)) return Status::invalid_argument;

    const std::uint32_t count = group->member_count;
    completed.member_count_snapshot = count;
    const dh2::character_group::Services callbacks =
        services == nullptr ? dh2::character_group::Services{} : *services;

    for (std::uint32_t i = 0; i < count; ++i) {
        // The ARM source reloads GroupInfo vector begin for each index, while
        // retaining its initial length. Validate only the current live range.
        if (!valid_range(group, count, services, result))
            return Status::invalid_argument;
        const auto identity = group->members[i].character_identity;
        if (identity == 0) return Status::invalid_argument;
        if (identity == owner_identity) {
            ++completed.owner_entries_skipped;
            continue;
        }
        if (callbacks.is_in_limbus == nullptr) return Status::invalid_argument;
        std::uint32_t in_limbus = 0;
        if (callbacks.is_in_limbus(callbacks.context, identity, &in_limbus) != 0 ||
            in_limbus > 1) {
            return Status::member_query_failed;
        }
        ++completed.members_queried;
        completed.any_other_in_limbus |= in_limbus;
    }

    // Guard a pointer changed by the final query before committing output.
    if (!valid_range(group, count, services, result)) return Status::invalid_argument;
    completed.group_status_after = completed.any_other_in_limbus ? 2 : 0;
    group->respawn_status = completed.group_status_after;
    completed.group_status_written = 1;
    *result = completed;
    return Status::complete;
}

}  // namespace dh2::character_group_limbus_blur
