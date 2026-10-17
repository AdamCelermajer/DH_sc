#include "character_aggro_candidate_events.hpp"

#include <cstddef>
#include <limits>

namespace dh2::character_aggro_candidate_events {
namespace {
using dh2::character::aggro_search::TargetInfo;
using dh2::character::aggro_search::TargetList;
struct Range { std::uintptr_t begin, end; };
bool range(const void* p, std::size_t bytes, Range& out) {
    const auto first = reinterpret_cast<std::uintptr_t>(p);
    if (!p || first > std::numeric_limits<std::uintptr_t>::max() - bytes)
        return false;
    out = {first, first + bytes}; return true;
}
bool overlap(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
bool aligned(const void* p, std::uintptr_t alignment) {
    return p && reinterpret_cast<std::uintptr_t>(p) % alignment == 0;
}

Status relation(State* state, const Services& services, Relation kind,
                std::uintptr_t candidate, Result* result, std::uint32_t& value) {
    if (!services.classify) return Status::service_unavailable;
    const auto owner = state->owner;
    if (!owner) return Status::invalid_argument;
    ++result->relationship_queries;
    try {
        if (services.classify(services.context, state, kind, owner, candidate, &value))
            return Status::service_failed;
    } catch (...) { return Status::service_failed; }
    return Status::complete;
}
Status raise(State* state, const Services& services, std::uint32_t event,
             std::uintptr_t payload) {
    if (!services.raise_event) return Status::service_unavailable;
    const auto owner = state->owner;
    if (!owner) return Status::invalid_argument;
    try {
        if (services.raise_event(services.context, state, owner, event, payload))
            return Status::service_failed;
    } catch (...) { return Status::service_failed; }
    return Status::complete;
}
}  // namespace

Status consume(State* state, TargetList* list, const Services* services,
               Result* result) {
    Range ranges[7];
    if (!aligned(state, alignof(State)) || !aligned(list, alignof(TargetList)) ||
        !aligned(services, alignof(Services)) || !aligned(result, alignof(Result)) ||
        !range(state, sizeof(*state), ranges[0]) ||
        !range(list, sizeof(*list), ranges[1]) ||
        !range(services, sizeof(*services), ranges[2]) ||
        !range(result, sizeof(*result), ranges[3])) return Status::invalid_argument;
    if (!state->ai || !list->capacity || list->capacity > 65536 ||
        list->count > list->capacity || !aligned(list->heap, alignof(TargetInfo)) ||
        !aligned(list->owner, alignof(dh2::character::aggro_search::Character)) ||
        list->sort_type != dh2::character::aggro_search::kSourceSortClosest || list->reserved)
        return Status::invalid_argument;
    const auto bytes = static_cast<std::uint64_t>(list->capacity) * sizeof(TargetInfo);
    if (bytes > std::numeric_limits<std::size_t>::max()) return Status::invalid_argument;
    if (!range(list->heap, static_cast<std::size_t>(bytes), ranges[4]) ||
        !range(list->owner, sizeof(*list->owner), ranges[5]))
        return Status::invalid_argument;
    for (unsigned i = 0; i < 6; ++i)
        for (unsigned j = i + 1; j < 6; ++j)
            if (overlap(ranges[i], ranges[j])) return Status::invalid_argument;
    if (!aligned(list->owner->object, alignof(dh2::character::aggro_search::GameObject)) ||
        !range(list->owner->object, sizeof(*list->owner->object), ranges[6]))
        return Status::invalid_argument;
    for (unsigned i = 0; i < 6; ++i)
        if (overlap(ranges[i], ranges[6])) return Status::invalid_argument;
    for (std::uint32_t i = 0; i < list->count; ++i)
        if (!(list->heap[i].flags & 1u) || !list->heap[i].object_identity)
            return Status::invalid_argument;

    *result = {};
    const Services captured = *services;
    bool no_enemy = true;
    while (list->count) {
        // TargetInfo+0 is the room's GameObject identity. A separately resolved
        // Character projection supplied validation metadata, not a replacement
        // identity for original relationship calls or Character event payloads.
        const auto candidate = list->heap[0].object_identity;
        std::uint32_t value = 0;
        auto status = relation(state, captured, Relation::enemy, candidate, result, value);
        if (status != Status::complete) return status;
        if (value) {
            status = raise(state, captured, 9, candidate);
            if (status != Status::complete) return status;
            ++result->enemy_events; no_enemy = false;
        } else {
            status = relation(state, captured, Relation::friend_, candidate, result, value);
            if (status != Status::complete) return status;
            if (value) {
                status = raise(state, captured, 7, candidate);
                if (status != Status::complete) return status;
                ++result->friend_events;
            } else {
                status = relation(state, captured, Relation::neutral, candidate, result, value);
                if (status != Status::complete) return status;
                if (value) {
                    status = raise(state, captured, 8, candidate);
                    if (status != Status::complete) return status;
                    ++result->neutral_events;
                }
            }
        }
        TargetInfo popped{};
        if (dh2::character::aggro_search::dh2_aggro_target_pop(list, &popped) != 0)
            return Status::target_list_failed;
        ++result->candidates_consumed;
    }
    if (no_enemy) {
        const auto identity = state->source_identity_40;
        if (identity) {
            const auto status = raise(state, captured, 0x0c, identity);
            if (status != Status::complete) return status;
            result->source_event_12 = 1;
        }
    }
    return Status::complete;
}

}  // namespace dh2::character_aggro_candidate_events
