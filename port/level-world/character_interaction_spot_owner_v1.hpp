#pragma once

#include "character_ai_sight.hpp"

#include <cstdint>

namespace dh2::character_interaction_spot_owner_v1 {

using Point = character_ai_sight::Point;

// Borrowed view of one canonical GameObject projection. The two cache fields
// remain owned by that object; this owner never creates a second node cache.
struct ObjectView {
    std::uintptr_t identity{};
    const std::uintptr_t* visual_object_2d8{};
    std::uint8_t* interaction_lookup_started_2ec{};
    std::uintptr_t* interaction_node_2e8{};
};

struct Services {
    void* context{};
    // A successful lookup may resolve to nullptr. This is distinct from an
    // absent callback, which is an unsupported source boundary.
    std::int32_t (*get_specific_node)(void*, std::uintptr_t visual,
                                      const char* node_name,
                                      std::uintptr_t* node_out){};
    // Returned points are canonical borrowed positions. They must remain live
    // through this call; the kernel copies the three binary32 words to Result.
    std::int32_t (*absolute_position)(void*, std::uintptr_t node,
                                      const Point** borrowed){};
    // Must return the canonical current GameObject::GetTargetPosition value;
    // that owner preserves the source visibility/live-position selection.
    std::int32_t (*target_position)(void*, std::uintptr_t object,
                                    const Point** borrowed){};
};

struct Result {
    Point point{};
    std::uintptr_t node{};
    std::uint32_t calls{};
    std::uint8_t node_lookup_performed{};
    std::uint8_t node_lookup_resolved{};
    std::uint8_t used_visual_node{};
    std::uint8_t used_target_position{};
};

enum class Status : std::int32_t {
    complete,
    invalid_argument,
    node_lookup_unsupported,
    absolute_position_unsupported,
    target_position_unsupported,
    service_failed,
    invalid_source_fact,
};

// GameObject::GetInteractionSpot at 0x38b228. The lookup-started byte is set
// before the named-node callback, and its result (including nullptr) is then
// cached in the object's +0x2e8 projection. A node position or the current
// GameObject::GetTargetPosition result is copied by value to `result`.
Status get_interaction_spot(const ObjectView*, const Services*, Result*);

// ObjectView, Services and Result controls are disjoint and remain live for
// this call. Cache fields and callback-returned positions remain borrowed and
// stable in identity/storage; providers may update their values synchronously.
// Completed cache writes survive service failures. No renderer object, handle,
// visibility filter, interaction range, or additional cache is constructed.

} // namespace dh2::character_interaction_spot_owner_v1
