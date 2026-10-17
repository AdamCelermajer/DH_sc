#include "character_respawn_outer.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace dh2::character_respawn {
namespace {

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(left);
    const auto b = reinterpret_cast<std::uintptr_t>(right);
    return a <= b ? b - a < left_size : a - b < right_size;
}

bool valid_facts(const Facts* facts) {
    return facts != nullptr && facts->reserved[0] == 0 &&
           facts->reserved[1] == 0 && facts->reserved[2] == 0;
}

bool valid_shared(const Facts* facts, const Services* services,
                  const void* result, std::size_t result_size) {
    return valid_facts(facts) && result != nullptr &&
           !overlaps(facts, sizeof(*facts), result, result_size) &&
           (services == nullptr ||
            (!overlaps(facts, sizeof(*facts), services, sizeof(*services)) &&
             !overlaps(result, result_size, services, sizeof(*services))));
}

bool valid_group_boundary(const Facts* facts, const Services* services,
                          const dh2::character_group::GroupState* group,
                          const PredicateResult* result) {
    // The leaf receives a local Result, so its own result-alias checks cannot
    // protect the caller's outer output or the larger outer Services object.
    if (overlaps(group, sizeof(*group), result, sizeof(*result)) ||
        overlaps(group, sizeof(*group), facts, sizeof(*facts)) ||
        overlaps(group, sizeof(*group), services, sizeof(*services))) {
        return false;
    }

    // Preserve the leaf's no-read paths: unsupported states do not read the
    // group at all, and only Idle/status 1 consumes its member range.
    if (facts->character_state_id != 3) return true;
    if (group->reserved[0] != 0 || group->reserved[1] != 0 ||
        group->reserved[2] != 0) return false;
    if (group->respawn_status != 1) return true;
    if (group->member_count == 0) return true;
    if (group->members == nullptr ||
        group->member_count > SIZE_MAX / sizeof(dh2::character_group::MemberRef)) {
        return false;
    }
    const auto bytes = static_cast<std::size_t>(group->member_count) *
                       sizeof(dh2::character_group::MemberRef);
    const auto address = reinterpret_cast<std::uintptr_t>(group->members);
    if (address > std::numeric_limits<std::uintptr_t>::max() - bytes) return false;
    return !overlaps(group->members, bytes, result, sizeof(*result)) &&
           !overlaps(group->members, bytes, facts, sizeof(*facts)) &&
           !overlaps(group->members, bytes, services, sizeof(*services));
}

std::int32_t asr8(std::int32_t value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    std::uint32_t shifted = bits >> 8;
    if ((bits & 0x80000000u) != 0) shifted |= 0xff000000u;
    std::int32_t result = 0;
    std::memcpy(&result, &shifted, sizeof(result));
    return result;
}

std::int32_t mul1000_arm(std::int32_t value) {
    const auto product = static_cast<std::uint32_t>(value) * 1000u;
    std::int32_t result = 0;
    std::memcpy(&result, &product, sizeof(result));
    return result;
}

Status read_property(const Services* services, std::int32_t* value) {
    if (services == nullptr || services->read_raw_property == nullptr ||
        value == nullptr) {
        return Status::invalid_argument;
    }
    std::int32_t raw = 0;
    if (services->read_raw_property(services->property_context,
                                    respawn_time_property_id, &raw) != 0) {
        return Status::property_read_failed;
    }
    *value = raw;
    return Status::complete;
}

}  // namespace

Status get_delay(const Facts* facts, const Services* services,
                 DelayResult* result) {
    if (!valid_shared(facts, services, result, sizeof(*result)))
        return Status::invalid_argument;

    DelayResult completed{};
    if (facts->init_spawned_suppression == 0) {
        const auto status = read_property(services, &completed.raw_respawn_time);
        if (status != Status::complete) return status;
        completed.property_read = 1;
        completed.delay_ms = mul1000_arm(asr8(completed.raw_respawn_time));
    }
    *result = completed;
    return Status::complete;
}

Status can_respawn(const Facts* facts, const Services* services,
                   dh2::character_group::GroupState* optional_group,
                   PredicateResult* result) {
    if (!valid_shared(facts, services, result, sizeof(*result)))
        return Status::invalid_argument;

    PredicateResult completed{};
    if (facts->init_spawned_suppression != 0) {
        *result = completed;
        return Status::complete;
    }

    auto status = read_property(services, &completed.raw_respawn_time);
    if (status != Status::complete) return status;
    completed.property_read = 1;
    if (completed.raw_respawn_time <= 0) {
        *result = completed;
        return Status::complete;
    }

    if (optional_group == nullptr) {
        completed.can_respawn = 1;
        *result = completed;
        return Status::complete;
    }

    if (!valid_group_boundary(facts, services, optional_group, result))
        return Status::group_input_invalid;

    dh2::character_group::Result group_result{};
    const auto group_status = dh2::character_group::can_respawn(
        facts->character_state_id, optional_group, &services->group,
        &group_result);
    if (group_status == dh2::character_group::Status::invalid_argument)
        return Status::group_input_invalid;
    if (group_status == dh2::character_group::Status::member_query_failed)
        return Status::group_member_query_failed;

    completed.can_respawn = group_result.can_respawn;
    completed.group_predicate_called = 1;
    completed.group_members_queried = group_result.members_queried;
    completed.group_status_after = group_result.group_status_after;
    completed.group_status_observed = group_result.group_status_observed;
    *result = completed;
    return Status::complete;
}

}  // namespace dh2::character_respawn
