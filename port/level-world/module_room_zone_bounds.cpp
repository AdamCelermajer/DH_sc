#include "module_room_zone_bounds.hpp"

namespace dh2::module_room_zone_bounds { namespace {
bool valid(const RoomZone* zone, const Box3* box, const Services* services,
           const Result* result) {
    if (!zone || !zone->identity || !box || !box->minimum || !box->maximum ||
        !zone->optional_visual_enabled || !services || !services->invoke || !result)
        return false;
    for (unsigned axis = 0; axis != 3; ++axis) {
        if (!zone->dimensions[axis] || !zone->relative_minimum[axis] ||
            !zone->relative_maximum[axis])
            return false;
    }
    return true;
}
} // namespace

Status initialize(RoomZone* zone, const Box3* box, const Services* services,
                  Result* result) {
    if (!valid(zone, box, services, result)) return Status::invalid_argument;

    // RoomZone::RoomZone invokes Zone::Zone(..., false, true), which writes
    // false to +0x380. Refuse other Zone subclasses because their subsequent
    // source visual/debug branch is not represented by this module kernel.
    if (*zone->optional_visual_enabled != 0)
        return Status::optional_visual_branch_unsupported;

    *result = {};
    const float width = box->maximum[0] - box->minimum[0];
    const float height = box->maximum[1] - box->minimum[1];
    const float depth = box->maximum[2] - box->minimum[2];

    // Match source stores at +0x378, +0x37c, then +0x374 after the three
    // ordered __aeabi_fsub calls (X, Y, Z).
    *zone->dimensions[1] = height;
    *zone->dimensions[2] = depth;
    *zone->dimensions[0] = width;

    for (unsigned axis = 0; axis != 3; ++axis)
        *zone->relative_minimum[axis] = box->minimum[axis];
    for (unsigned axis = 0; axis != 3; ++axis)
        *zone->relative_maximum[axis] = box->maximum[axis];

    constexpr float half = 0.5f;
    for (unsigned axis = 0; axis != 3; ++axis)
        result->center[axis] = (box->maximum[axis] + box->minimum[axis]) * half;

    const Request request{Operation::set_position, zone->identity,
                          {result->center[0], result->center[1], result->center[2]}, 1};
    result->calls = 1;
    try {
        if (services->invoke(services->context, &request) != 0)
            return Status::service_failed;
    } catch (...) {
        return Status::service_failed;
    }
    return Status::complete;
}

} // namespace dh2::module_room_zone_bounds
