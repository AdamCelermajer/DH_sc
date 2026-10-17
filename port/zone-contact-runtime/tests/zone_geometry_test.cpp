#include "../zone_geometry.hpp"

#include <cmath>
#include <cstdio>
#include <limits>

namespace {

using dh2_zone_contact::Segment;
using dh2_zone_contact::Vec3;

bool close(float actual, float expected) {
    return std::fabs(actual - expected) <= 1e-4f;
}

bool require(bool condition, const char *description) {
    if (!condition) std::fprintf(stderr, "FAIL: %s\n", description);
    return condition;
}

struct QuerySpy {
    Segment expected;
    bool result;
    bool called;
};

bool selector_query(void *context,
                    const Segment *segment,
                    Vec3 *intersection_point) {
    QuerySpy *spy = static_cast<QuerySpy *>(context);
    spy->called = true;
    spy->expected = *segment;
    *intersection_point = {7.0f, 8.0f, 9.0f};
    return spy->result;
}

}  // namespace

int main() {
    using namespace dh2_zone_contact;
    bool okay = true;
    const Vec3 source_default_dimensions = {200.0f, 200.0f, 200.0f};
    const Vec3 ambush_scale = {1.92682f, 1.92682f, 1.0f};
    Aabb bounds{};

    okay &= require(make_zone_local_aabb(&source_default_dimensions,
                                         &ambush_scale, &bounds),
                    "reconstruct Zone::InitPost local AABB");
    okay &= require(close(bounds.min.x, -192.682f) &&
                    close(bounds.min.y, -192.682f) &&
                    close(bounds.min.z, -100.0f) &&
                    close(bounds.max.x, 192.682f) &&
                    close(bounds.max.y, 192.682f) &&
                    close(bounds.max.z, 100.0f),
                    "default dimensions times Ambush object scale, halved");

    const Vec3 custom_dimensions = {10.0f, 40.0f, 80.0f};
    const Vec3 custom_scale = {2.0f, 0.5f, 0.25f};
    okay &= require(make_zone_local_aabb(&custom_dimensions, &custom_scale,
                                         &bounds) &&
                    close(bounds.min.x, -10.0f) &&
                    close(bounds.min.y, -10.0f) &&
                    close(bounds.min.z, -10.0f) &&
                    close(bounds.max.x, 10.0f) &&
                    close(bounds.max.y, 10.0f) &&
                    close(bounds.max.z, 10.0f),
                    "each local axis uses dimensions times scale times one half");

    okay &= require(!make_zone_local_aabb(0, &ambush_scale, &bounds) &&
                    !make_zone_local_aabb(&source_default_dimensions,
                                          &ambush_scale, 0),
                    "reject invalid API pointers");

    const Aabb world_bounds = {{-1.0f, -2.0f, -3.0f},
                               {1.0f, 2.0f, 3.0f}};
    PhysicalObjectView physical = {true, 25.0f};
    const Vec3 lower_corner = {-1.0f, -2.0f, -3.0f};
    const Vec3 upper_corner = {1.0f, 2.0f, 3.0f};
    ContactResult result = project_is_inside(
        &physical, false, false, &world_bounds, 0, &lower_corner, 0, 0);
    okay &= require(result.inside && result.path == kAabbFallback,
                    "AABB fallback includes the exact minimum corner");
    result = project_is_inside(
        &physical, false, false, &world_bounds, 0, &upper_corner, 0, 0);
    okay &= require(result.inside,
                    "AABB fallback includes the exact maximum corner");

    const float below_min = std::nextafter(-1.0f,
        -std::numeric_limits<float>::infinity());
    const float above_max_y = std::nextafter(2.0f,
        std::numeric_limits<float>::infinity());
    const float above_max_z = std::nextafter(3.0f,
        std::numeric_limits<float>::infinity());
    const Vec3 outside_x = {below_min, 0.0f, 0.0f};
    const Vec3 outside_y = {0.0f, above_max_y, 0.0f};
    const Vec3 outside_z = {0.0f, 0.0f, above_max_z};
    okay &= require(!project_is_inside(
                        &physical, false, false, &world_bounds, 0,
                        &outside_x, 0, 0).inside &&
                    !project_is_inside(
                        &physical, false, false, &world_bounds, 0,
                        &outside_y, 0, 0).inside &&
                    !project_is_inside(
                        &physical, false, false, &world_bounds, 0,
                        &outside_z, 0, 0).inside,
                    "one-float-step outside any AABB face is excluded");

    const Vec3 center = {0.0f, 0.0f, 0.0f};
    physical.radius = 0.0f;
    const ContactResult radius_zero = project_is_inside(
        &physical, false, false, &world_bounds, 0, &center, 0, 0);
    physical.radius = 1000000.0f;
    const ContactResult radius_large = project_is_inside(
        &physical, false, false, &world_bounds, 0, &center, 0, 0);
    const Vec3 beyond = {1.5f, 0.0f, 0.0f};
    physical.radius = 0.0f;
    const bool outside_with_zero_radius = project_is_inside(
        &physical, false, false, &world_bounds, 0, &beyond, 0, 0).inside;
    physical.radius = 1000000.0f;
    const bool outside_with_large_radius = project_is_inside(
        &physical, false, false, &world_bounds, 0, &beyond, 0, 0).inside;
    okay &= require(radius_zero.inside == radius_large.inside &&
                    radius_zero.inside &&
                    outside_with_zero_radius == outside_with_large_radius &&
                    !outside_with_zero_radius,
                    "AABB fallback ignores the physical radius");

    const Vec3 zone_position = {1.0f, 2.0f, 3.0f};
    const Vec3 actor_position = {11.0f, 22.0f, 33.0f};
    QuerySpy spy{};
    spy.result = true;
    result = project_is_inside(
        &physical, true, true, 0, &zone_position, &actor_position,
        selector_query, &spy);
    okay &= require(result.inside && result.path == kSelectorLine &&
                    result.selector_query_performed && spy.called &&
                    close(kNativeGlobalDirection.x, 0.0f) &&
                    close(kNativeGlobalDirection.y, 0.0f) &&
                    close(kNativeGlobalDirection.z, 1.0f) &&
                    close(spy.expected.start.x, 10.0f) &&
                    close(spy.expected.start.y, 20.0f) &&
                    close(spy.expected.start.z, 130.0f) &&
                    close(spy.expected.end.x, 10.0f) &&
                    close(spy.expected.end.y, 20.0f) &&
                    close(spy.expected.end.z, -70.0f) &&
                    close(result.intersection_point.x, 7.0f) &&
                    close(result.intersection_point.y, 8.0f) &&
                    close(result.intersection_point.z, 9.0f),
                    "selector path keeps XY delta and extends exactly +/-100 on Z");

    spy.called = false;
    spy.result = false;
    result = project_is_inside(
        &physical, true, true, &world_bounds, &zone_position, &actor_position,
        selector_query, &spy);
    okay &= require(!result.inside && result.selector_query_performed &&
                    spy.called,
                    "selector path preserves a collision-manager miss");

    spy.called = false;
    result = project_is_inside(
        &physical, true, false, &world_bounds, &zone_position, &actor_position,
        selector_query, &spy);
    okay &= require(!result.inside && !result.selector_query_performed &&
                    !spy.called && result.path == kSelectorLine,
                    "selector node without a triangle selector returns false");

    PhysicalObjectView absent_physics = {false, 50.0f};
    result = project_is_inside(
        &absent_physics, false, false, &world_bounds, 0, &center, 0, 0);
    okay &= require(!result.inside && result.path == kNoPhysicalObject,
                    "missing physical object returns false before either path");
    return okay ? 0 : 1;
}
