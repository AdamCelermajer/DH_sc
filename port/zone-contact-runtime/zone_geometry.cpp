#include "zone_geometry.hpp"

#include <cmath>

namespace dh2_zone_contact {

const Vec3 kNativeGlobalDirection = {0.0f, 0.0f, 1.0f};

namespace {

bool finite_vec(const Vec3 *value) {
    return value != 0 && std::isfinite(value->x) &&
        std::isfinite(value->y) && std::isfinite(value->z);
}

Vec3 add(const Vec3 &left, const Vec3 &right) {
    return {left.x + right.x, left.y + right.y, left.z + right.z};
}

Vec3 subtract(const Vec3 &left, const Vec3 &right) {
    return {left.x - right.x, left.y - right.y, left.z - right.z};
}

Vec3 scale(const Vec3 &value, float factor) {
    return {value.x * factor, value.y * factor, value.z * factor};
}

ContactResult empty_result(ContactPath path) {
    ContactResult result{};
    result.path = path;
    return result;
}

bool contains_position_inclusive(const Aabb *bounds, const Vec3 *position) {
    return bounds != 0 && position != 0 &&
        bounds->min.x <= position->x && position->x <= bounds->max.x &&
        bounds->min.y <= position->y && position->y <= bounds->max.y &&
        bounds->min.z <= position->z && position->z <= bounds->max.z;
}

}  // namespace

bool make_zone_local_aabb(const Vec3 *dimensions,
                          const Vec3 *object_scale,
                          Aabb *local_bounds) {
    if (!finite_vec(dimensions) || !finite_vec(object_scale) ||
        local_bounds == 0) {
        return false;
    }

    const float half_x = dimensions->x * object_scale->x * 0.5f;
    const float half_y = dimensions->y * object_scale->y * 0.5f;
    const float half_z = dimensions->z * object_scale->z * 0.5f;
    if (!std::isfinite(half_x) || !std::isfinite(half_y) ||
        !std::isfinite(half_z)) {
        return false;
    }

    local_bounds->min = {-half_x, -half_y, -half_z};
    local_bounds->max = {half_x, half_y, half_z};
    return true;
}

ContactResult project_is_inside(
    const PhysicalObjectView *physical_object,
    bool selector_node_present,
    bool triangle_selector_present,
    const Aabb *zone_world_bounds,
    const Vec3 *zone_position,
    const Vec3 *actor_position,
    SelectorLineQuery selector_query,
    void *selector_query_context) {
    if (physical_object == 0 || !physical_object->present) {
        return empty_result(kNoPhysicalObject);
    }

    if (!selector_node_present) {
        ContactResult result = empty_result(kAabbFallback);
        result.inside = contains_position_inclusive(zone_world_bounds,
                                                   actor_position);
        return result;
    }

    ContactResult result = empty_result(kSelectorLine);
    if (!triangle_selector_present || selector_query == 0 ||
        !finite_vec(zone_position) || !finite_vec(actor_position)) {
        return result;
    }

    const Vec3 delta = subtract(*actor_position, *zone_position);
    const Vec3 offset = scale(kNativeGlobalDirection, 100.0f);
    result.segment.start = add(delta, offset);
    result.segment.end = subtract(delta, offset);
    if (!finite_vec(&result.segment.start) || !finite_vec(&result.segment.end)) {
        result.segment = {};
        return result;
    }

    result.selector_query_performed = true;
    result.inside = selector_query(selector_query_context, &result.segment,
                                   &result.intersection_point);
    return result;
}

}  // namespace dh2_zone_contact
