#include "character_interaction_spot_owner_v1.hpp"

#include <cstddef>
#include <cstring>

namespace dh2::character_interaction_spot_owner_v1 { namespace {

struct Range { std::uintptr_t start, end; };

bool range(const void* pointer, std::size_t size, std::size_t alignment,
           Range& result) noexcept {
    const auto start = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || start % alignment || start > UINTPTR_MAX - size)
        return false;
    result = {start, start + size};
    return true;
}

bool overlaps(Range left, Range right) noexcept {
    return left.start < right.end && right.start < left.end;
}

bool source_field(const void* pointer, std::size_t size,
                  std::size_t alignment, const Range (&controls)[3],
                  Range& result) noexcept {
    if (!range(pointer, size, alignment, result)) return false;
    for (const auto control : controls)
        if (overlaps(result, control)) return false;
    return true;
}

bool borrowed_point(const Point* pointer, const Range (&controls)[3],
                    const Range (&fields)[3]) noexcept {
    Range point;
    if (!range(pointer, sizeof(*pointer), alignof(Point), point)) return false;
    for (const auto control : controls)
        if (overlaps(point, control)) return false;
    for (const auto field : fields)
        if (overlaps(point, field)) return false;
    return true;
}

} // namespace

Status get_interaction_spot(const ObjectView* object, const Services* services,
                            Result* result) {
    Range controls[3];
    if (!range(object, sizeof(*object), alignof(ObjectView), controls[0]) ||
        !range(services, sizeof(*services), alignof(Services), controls[1]) ||
        !range(result, sizeof(*result), alignof(Result), controls[2]))
        return Status::invalid_argument;
    for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < i; ++j)
            if (overlaps(controls[i], controls[j])) return Status::invalid_argument;
    if (!object->identity || !object->interaction_lookup_started_2ec ||
        !object->interaction_node_2e8)
        return Status::invalid_argument;

    Range fields[3]{};
    if (!source_field(object->interaction_lookup_started_2ec,
                      sizeof(*object->interaction_lookup_started_2ec),
                      alignof(std::uint8_t), controls, fields[0]) ||
        !source_field(object->interaction_node_2e8,
                      sizeof(*object->interaction_node_2e8),
                      alignof(std::uintptr_t), controls, fields[1]))
        return Status::invalid_argument;
    // `visual_object_2d8` is only read in the source's not-yet-looked-up arm.
    if (*object->interaction_lookup_started_2ec == 0 &&
        !source_field(object->visual_object_2d8,
                      sizeof(*object->visual_object_2d8),
                      alignof(std::uintptr_t), controls, fields[2]))
        return Status::invalid_argument;
    if (overlaps(fields[0], fields[1]) ||
        (fields[2].start && (overlaps(fields[2], fields[0]) ||
                             overlaps(fields[2], fields[1]))))
        return Status::invalid_argument;

    const Services bound = *services;
    *result = {};
    std::uintptr_t node = 0;
    if (*object->interaction_lookup_started_2ec == 0) {
        // The source captures the visual identity before setting this byte.
        const std::uintptr_t visual = *object->visual_object_2d8;
        *object->interaction_lookup_started_2ec = 1;
        if (visual) {
            if (!bound.get_specific_node) return Status::node_lookup_unsupported;
            std::uintptr_t resolved = 0;
            ++result->calls;
            result->node_lookup_performed = 1;
            try {
                if (bound.get_specific_node(bound.context, visual,
                                            "interaction_position", &resolved))
                    return Status::service_failed;
            } catch (...) {
                return Status::service_failed;
            }
            node = resolved;
            *object->interaction_node_2e8 = resolved;
            result->node_lookup_resolved = 1;
        } else {
            node = 0;
            *object->interaction_node_2e8 = 0;
            result->node_lookup_resolved = 1;
        }
    } else {
        // The initialized byte means the source considers this cached value,
        // including nullptr, resolved and never tries the visual again.
        node = *object->interaction_node_2e8;
        result->node_lookup_resolved = 1;
    }
    result->node = node;

    const Point* borrowed = nullptr;
    if (node) {
        if (!bound.absolute_position)
            return Status::absolute_position_unsupported;
        ++result->calls;
        result->used_visual_node = 1;
        try {
            if (bound.absolute_position(bound.context, node, &borrowed))
                return Status::service_failed;
        } catch (...) {
            return Status::service_failed;
        }
    } else {
        if (!bound.target_position) return Status::target_position_unsupported;
        ++result->calls;
        result->used_target_position = 1;
        try {
            if (bound.target_position(bound.context, object->identity, &borrowed))
                return Status::service_failed;
        } catch (...) {
            return Status::service_failed;
        }
    }
    if (!borrowed_point(borrowed, controls, fields))
        return Status::invalid_source_fact;
    result->point = *borrowed;
    return Status::complete;
}

} // namespace dh2::character_interaction_spot_owner_v1
