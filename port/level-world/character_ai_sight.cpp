#include "character_ai_sight.hpp"

#include <cstddef>
#include <cstring>
#include <limits>

namespace dh2::character_ai_sight {
namespace {
struct Range { std::uintptr_t start, end; };
bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) {
    const auto at = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || at % alignment || at > UINTPTR_MAX - size) return false;
    out = {at, at + size}; return true;
}
bool overlap(Range a, Range b) { return a.start < b.end && b.start < a.end; }
bool preflight(State* state, const Services* services, Result* result, Range (&ranges)[3]) {
    if (!range(state, sizeof(*state), alignof(State), ranges[0]) ||
        !range(services, sizeof(*services), alignof(Services), ranges[1]) ||
        !range(result, sizeof(*result), alignof(Result), ranges[2])) return false;
    for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlap(ranges[i], ranges[j])) return false;
    return state->ai != 0;
}
template <class T> bool view(const T* pointer, const Range (&ranges)[3]) {
    Range borrowed{};
    if (!range(pointer, sizeof(*pointer), alignof(T), borrowed)) return false;
    for (const auto& object : ranges) if (overlap(object, borrowed)) return false;
    return true;
}
float number(std::uint32_t word) {
    float value;
    static_assert(sizeof(value) == sizeof(word) && std::numeric_limits<float>::is_iec559);
    std::memcpy(&value, &word, sizeof(value)); return value;
}
std::uint32_t word(float value) { std::uint32_t result; std::memcpy(&result, &value, sizeof(result)); return result; }
Status scalar(State* state, std::uint32_t distance, const Services& services,
              Result* result, const Range (&ranges)[3]) {
    const auto owner = state->owner;
    if (!owner) return Status::invalid_source_fact;
    if (!services.char_ai) return Status::service_unavailable;
    const ViewRadius* row = nullptr;
    ++result->props_queries;
    try {
        if (services.char_ai(services.context, state, owner, &row)) return Status::service_failed;
    } catch (...) { return Status::service_failed; }
    if (!view(row, ranges)) return Status::invalid_source_fact;
    const auto radius = number(row->word_3c);
    volatile float squared = radius * radius;
    result->distance_word = distance;
    result->squared_radius_word = word(squared);
    result->value = squared > number(distance);
    return Status::complete;
}
}  // namespace

Status evaluate_distance(State* state, std::uint32_t distance, const Services* services, Result* result) {
    Range ranges[3];
    if (!preflight(state, services, result, ranges)) return Status::invalid_argument;
    const Services bound = *services;
    *result = {};
    return scalar(state, distance, bound, result, ranges);
}

Status evaluate_object(State* state, std::uintptr_t candidate, const Services* services, Result* result) {
    Range ranges[3];
    if (!preflight(state, services, result, ranges)) return Status::invalid_argument;
    const Services bound = *services;
    *result = {};
    if (!candidate) candidate = state->target_40;
    result->candidate = candidate;
    if (!candidate) return Status::complete;
    if (!bound.target_position) return Status::service_unavailable;
    const auto owner = state->owner;
    if (!owner) return Status::invalid_source_fact;
    const Point* owner_point = nullptr;
    const Point* target_point = nullptr;
    ++result->position_queries;
    try {
        if (bound.target_position(bound.context, state, owner, &owner_point)) return Status::service_failed;
    } catch (...) { return Status::service_failed; }
    result->owner_point = reinterpret_cast<std::uintptr_t>(owner_point);
    ++result->position_queries;
    try {
        if (bound.target_position(bound.context, state, candidate, &target_point)) return Status::service_failed;
    } catch (...) { return Status::service_failed; }
    result->target_point = reinterpret_cast<std::uintptr_t>(target_point);
    // No coordinate snapshot before the second callback. Both original returned
    // pointers remain fixed; their coordinates are consumed afterwards.
    if (!view(owner_point, ranges) || !view(target_point, ranges)) return Status::invalid_source_fact;
    volatile float dx = number(owner_point->words[0]) - number(target_point->words[0]);
    volatile float dy = number(owner_point->words[1]) - number(target_point->words[1]);
    volatile float dz = number(owner_point->words[2]) - number(target_point->words[2]);
    volatile float xx = dx * dx;
    volatile float yy = dy * dy;
    volatile float xy = xx + yy;
    volatile float zz = dz * dz;
    volatile float distance = xy + zz;
    return scalar(state, word(distance), bound, result, ranges);
}

}  // namespace dh2::character_ai_sight
