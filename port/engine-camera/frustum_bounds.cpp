#include "frustum_bounds.hpp"

#include <limits>

namespace dh2::engine_camera::frustum_bounds { namespace {

bool valid_range(const void* pointer, std::size_t size, std::size_t alignment) {
    if (!pointer || reinterpret_cast<Address>(pointer) % alignment != 0) return false;
    return reinterpret_cast<Address>(pointer) <=
        std::numeric_limits<Address>::max() - size;
}

bool overlaps(const void* left, std::size_t left_size,
              const void* right, std::size_t right_size) {
    const Address a = reinterpret_cast<Address>(left);
    const Address b = reinterpret_cast<Address>(right);
    return a < b + right_size && b < a + left_size;
}

struct ActiveCall {
    const Frustum* frustum;
    ActiveCall* previous;
};
thread_local ActiveCall* active_calls = nullptr;

bool is_active(const Frustum* frustum) {
    for (const auto* call = active_calls; call; call = call->previous)
        if (call->frustum == frustum) return true;
    return false;
}

struct ActiveGuard {
    ActiveCall frame;
    explicit ActiveGuard(const Frustum* frustum)
        : frame{frustum, active_calls} { active_calls = &frame; }
    ~ActiveGuard() { active_calls = frame.previous; }
};

} // namespace

Status recalculate(Frustum* frustum, const Services* services) {
    static_assert(sizeof(Frustum) == 0x84);
    static_assert(offsetof(Frustum, planes) == 0x0c);
    static_assert(offsetof(Frustum, box_min) == 0x6c);
    static_assert(offsetof(Frustum, box_max) == 0x78);

    if (!valid_range(frustum, sizeof(*frustum), alignof(Frustum)) ||
        !valid_range(services, sizeof(*services), alignof(Services)) ||
        overlaps(frustum, sizeof(*frustum), services, sizeof(*services)))
        return Status::invalid_argument;
    if (!services->intersect_three_planes || !services->greater || !services->less)
        return Status::invalid_argument;
    if (services->context_extent &&
        (!valid_range(services->context, services->context_extent, 1) ||
         overlaps(services->context, services->context_extent,
                  frustum, sizeof(*frustum)) ||
         overlaps(services->context, services->context_extent,
                  services, sizeof(*services))))
        return Status::invalid_argument;

    if (is_active(frustum)) return Status::reentrant_call;
    ActiveGuard active(frustum);
    const Services bound = *services;

    // Source seeds both extrema from the frustum position before any helper call.
    const Word position[3] = {frustum->position[0], frustum->position[1],
                              frustum->position[2]};
    for (std::size_t axis = 0; axis != 3; ++axis) {
        frustum->box_min[axis] = position[axis];
        frustum->box_max[axis] = position[axis];
    }

    constexpr std::size_t intersections[4][3] = {
        {0, 5, 2}, {0, 5, 3}, {0, 4, 2}, {0, 4, 3}
    };
    void* const context = bound.context;
    for (const auto& planes : intersections) {
        // Each source stack vector is explicitly zeroed before the helper. Its
        // returned boolean is ignored, so a non-writing/failed helper leaves 0s.
        Word point[3]{};
        (void)bound.intersect_three_planes(
            context, frustum->planes[planes[0]], frustum->planes[planes[1]],
            frustum->planes[planes[2]], point);
        for (std::size_t axis = 0; axis != 3; ++axis)
            if (bound.greater(context, point[axis], frustum->box_max[axis]))
                frustum->box_max[axis] = point[axis];
        for (std::size_t axis = 0; axis != 3; ++axis)
            if (bound.less(context, point[axis], frustum->box_min[axis]))
                frustum->box_min[axis] = point[axis];
    }
    return Status::complete;
}

} // namespace dh2::engine_camera::frustum_bounds
