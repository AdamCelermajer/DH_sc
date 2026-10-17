#pragma once

#include <cstdint>

namespace dh2::character_ai_sight {

struct State { std::uintptr_t ai, owner, target_40; };
// Borrowed projected values, not overlays of original GameObject/AIProps.
struct Point { std::uint32_t words[3]; };
struct ViewRadius { std::uint32_t word_3c; };
struct Services {
    void* context;
    std::int32_t (*target_position)(void*, State*, std::uintptr_t object, const Point**);
    // Genuine Character::GetCharAI result, including its table capture/getter
    // semantics. The returned radius stays live until the source read on return.
    std::int32_t (*char_ai)(void*, State*, std::uintptr_t owner, const ViewRadius**);
};
struct Result {
    std::uint32_t value, position_queries, props_queries;
    std::uint32_t distance_word, squared_radius_word;
    std::uintptr_t candidate, owner_point, target_point;
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3, invalid_source_fact = 4,
};

// Original float overload: supplied raw binary32 squared-distance, fresh owner
// GetCharAI then radius+0x3c, rounded square and strict greater-than. No clamps.
Status evaluate_distance(State*, std::uint32_t squared_distance_word, const Services*, Result*);
// Original object overload: null argument falls back to current AI+0x40; absent
// candidate returns false without providers. Captures owner's borrowed point,
// then target's point, and only then reads both live coordinate values. Tail
// scalar phase reloads owner. Every sub/mul/add rounds separately to binary32.
Status evaluate_object(State*, std::uintptr_t candidate, const Services*, Result*);

// All services return zero on success. One owning thread retains captured AI,
// candidate, each owner, returned point/radius storage and context until return,
// including retired storage after pointer/table replacement. Providers may
// update owner/target/values synchronously; no result/service overwrite or
// borrowed-view destruction. Independent nested outputs are permitted. Errors
// preserve completed effects; invalid top aliases reject before effects.
// Float NaN payload propagation and external soft-float library implementation
// are not claimed; IEEE binary32 arithmetic/predicates and source order are.

}  // namespace dh2::character_ai_sight
