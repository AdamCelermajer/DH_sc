// Map page visit tracker, frustum gate, save persistence and map projection tests.
#include "map_page_v1.hpp"
#include "room_zone_visit_v1.hpp"

#include <cmath>
#include <cstdio>
#include <iostream>

using namespace dh::foundation;
using namespace dh::foundation::map_visit;

namespace {
int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::cerr << __FILE__ << ':' << __LINE__ << " CHECK failed: " #cond "\n"; ++failures; } } while (0)

LevelModuleZone zone(std::uint32_t id, std::array<float, 6> bounds, const char* name = "m") {
    LevelModuleZone z;
    z.id = id;
    z.name = name;
    z.bounds = bounds;
    return z;
}

// Camera at the origin looking down -Z, 60 degree vertical FOV, aspect 1, near 1, far 100.
Camera test_camera() {
    Camera c;
    c.eye = {0, 0, 0};
    c.target = {0, 0, -1};
    c.up = {0, 1, 0};
    c.verticalFovDegrees = 60.0f;
    c.nearPlane = 1.0f;
    c.farPlane = 100.0f;
    c.aspectRatio = 1.0f;
    return c;
}

void frustum_tests() {
    CameraBasisV1 basis;
    std::string error;
    CHECK(camera_basis_v1(test_camera(), 1.0f, basis, error));
    const auto planes = frustum_planes_v1(basis);
    // Straight ahead, inside.
    CHECK(zone_in_frustum_v1({-1, -1, -10, 1, 1, -9}, planes));
    // Behind the camera.
    CHECK(!zone_in_frustum_v1({-1, -1, 5, 1, 1, 6}, planes));
    // Beyond the far plane.
    CHECK(!zone_in_frustum_v1({-1, -1, -200, 1, 1, -150}, planes));
    // Entirely in front of the near plane.
    CHECK(!zone_in_frustum_v1({-0.2f, -0.2f, -0.6f, 0.2f, 0.2f, -0.4f}, planes));
    // Far to the side, outside the 45 degree half-angle (x = 60 at depth 10 is outside).
    CHECK(!zone_in_frustum_v1({59, -1, -11, 61, 1, -9}, planes));
    // Straddles the right side: its most-inside corner is inside, so it is in view.
    CHECK(zone_in_frustum_v1({5, -1, -11, 15, 1, -9}, planes));
    // Right boundary at depth 10 is x = 10 tan(30 deg) = 5.77 (square aspect).
    // Just outside it, wholly.
    CHECK(!zone_in_frustum_v1({6.5f, -0.5f, -10.5f, 7.0f, 0.5f, -9.5f}, planes));
    // Top boundary at depth 10 is y = 5.77.
    CHECK(!zone_in_frustum_v1({-1, 6.5f, -10.5f, 1, 7.0f, -9.5f}, planes));
    CHECK(zone_in_frustum_v1({-1, 5.0f, -10.5f, 1, 5.5f, -9.5f}, planes));
    // Degenerate camera is rejected.
    Camera bad = test_camera();
    bad.target = bad.eye;
    CHECK(!camera_basis_v1(bad, 1.0f, basis, error));
    CHECK(!error.empty());
}

void inside_tests() {
    const std::array<float, 6> b{0, 0, -5, 10, 4, 5};
    CHECK(zone_has_inside_xy_v1(b, 0, 0));      // inclusive min corner
    CHECK(zone_has_inside_xy_v1(b, 10, 4));     // inclusive max corner
    CHECK(!zone_has_inside_xy_v1(b, 10.001f, 2));
    CHECK(!zone_has_inside_xy_v1(b, -0.001f, 2));
    // Z is not part of HasInside (inclusive XY only).
    CHECK(zone_has_inside_xy_v1(b, 5, 2));
}

void tracker_tests() {
    CameraBasisV1 basis;
    std::string error;
    CHECK(camera_basis_v1(test_camera(), 1.0f, basis, error));
    const auto planes = frustum_planes_v1(basis);
    const std::array<float, 3> inFront{0, 0, -10};  // player inside zone 1 (XY)
    RoomZoneVisitTrackerV1 t;
    // Zone 1 ahead (in view), zone 2 behind the camera, zone 3 ahead but the player is not in it.
    CHECK(t.configure({zone(1, {-2, -2, -12, 2, 2, -8}), zone(2, {-2, -2, 8, 2, 2, 12}), zone(3, {50, 50, -12, 60, 60, -8})}, {}, error));
    // No player: nothing is visited.
    CHECK(t.update(planes, std::nullopt).empty());
    CHECK(!t.visited(1));
    CHECK(t.in_view(1));
    // Player inside a zone in view: visited, reported once.
    const auto first = t.update(planes, inFront);
    CHECK(first.size() == 1 && first[0] == 1);
    CHECK(t.visited(1));
    CHECK(t.update(planes, inFront).empty());  // re-entry keeps the flag and reports nothing new
    CHECK(t.visited(1));
    // Zone outside the frustum is not visited even when the player is inside its XY box.
    const std::array<float, 3> behind{0, 0, 10};
    CHECK(t.update(planes, behind).empty());
    CHECK(!t.visited(2));
    CHECK(!t.in_view(2));
    // Player in zone 3's XY box is outside the frustum (x=55) so it is not visited.
    CHECK(t.update(planes, std::array<float, 3>{55, 55, -10}).empty());
    CHECK(!t.visited(3));
    CHECK(t.visited_count() == 1);

    // Saved visited ids restore; unknown saved ids are ignored; duplicates are rejected.
    RoomZoneVisitTrackerV1 restored;
    CHECK(restored.configure({zone(1, {-2, -2, -12, 2, 2, -8}), zone(4, {0, 0, 0, 1, 1, 1})}, {1, 99}, error));
    CHECK(restored.visited(1));
    CHECK(!restored.visited(4));
    CHECK(restored.visited_count() == 1);
    RoomZoneVisitTrackerV1 dup;
    CHECK(!dup.configure({zone(7, {0, 0, 0, 1, 1, 1}), zone(7, {2, 2, 2, 3, 3, 3})}, {}, error));
    CHECK(!error.empty());
    RoomZoneVisitTrackerV1 inverted;
    CHECK(!inverted.configure({zone(8, {1, 0, 0, 0, 1, 1})}, {}, error));
}

void persistence_tests() {
    CharacterState state;
    CHECK(visited_module_ids(state, "data/scene/001_swamp.mlx").empty());  // legacy/fresh = unvisited
    record_visited_modules(state, "data/scene/001_swamp.mlx", {5, 2, 5});
    const auto ids = visited_module_ids(state, "data/scene/001_swamp.mlx");
    CHECK((ids == std::vector<std::uint32_t>{2, 5}));
    CHECK(visited_module_ids(state, "data/scene/002_other.mlx").empty());  // per level
    record_visited_modules(state, "data/scene/001_swamp.mlx", {2});          // idempotent
    CHECK(visited_module_ids(state, "data/scene/001_swamp.mlx").size() == 2);
    CHECK(state.visited_modules.size() == 2);
}

void map_tests() {
    std::vector<LevelModuleZone> zones{zone(1, {-10, -4, -2, 0, 4, 2}), zone(2, {0, -6, -1, 12, 0, 3})};
    const auto extent = map_extent_v1(zones);
    CHECK(extent.valid);
    CHECK(extent.minX == -10 && extent.maxX == 12 && extent.minY == -6 && extent.maxY == 4);
    CHECK(extent.minZ == -2 && extent.maxZ == 3);
    CHECK(!map_extent_v1({}).valid);

    MapViewV1 view;
    CHECK(view.zoom == map_zoom_min);
    view = map_zoom_step_v1(view, 1.0f);
    CHECK(view.zoom > map_zoom_min);
    for (int i = 0; i < 20; ++i) view = map_zoom_step_v1(view, 1.0f);
    CHECK(view.zoom == map_zoom_max);  // clamped
    view = map_reset_zoom_v1(view);
    CHECK(view.zoom == map_zoom_min);
    CHECK(map_zoom_step_v1(view, -1.0f).zoom == map_zoom_min);  // never below reset

    std::string error;
    Camera reset;
    CHECK(map_camera_v1(extent, std::array<float, 3>{3, 0, 0}, view, reset, error));
    // Reset view centres on the level, looks straight down and sees the whole level.
    CHECK(std::abs(reset.eye.x - 1.0f) < 1e-4f && std::abs(reset.eye.y - (-1.0f)) < 1e-4f);
    CHECK(reset.eye.z > extent.maxZ);
    CHECK(reset.target.z == extent.minZ);
    float px = 0, py = 0;
    // Level corners project inside the rectangle; a point far outside does not.
    MapRectV1 rect{100, 50, 300, 200};
    CHECK(map_project_v1(reset, rect, {-10, -6, -2}, px, py));
    CHECK(px >= rect.x && px <= rect.x + rect.width && py >= rect.y && py <= rect.y + rect.height);
    CHECK(map_project_v1(reset, rect, {12, 4, 3}, px, py));
    CHECK(!map_project_v1(reset, rect, {500, 0, 0}, px, py));
    // The level centre projects to the rectangle centre.
    CHECK(map_project_v1(reset, rect, {1, -1, 0.5f}, px, py));
    CHECK(std::abs(px - 250) < 0.5f && std::abs(py - 150) < 0.5f);

    // Zoomed view follows the player: the player projects to the centre.
    view.zoom = 3.0f;
    Camera zoomed;
    CHECK(map_camera_v1(extent, std::array<float, 3>{6, -2, 0}, view, zoomed, error));
    CHECK(map_project_v1(zoomed, rect, {6, -2, 0}, px, py));
    CHECK(std::abs(px - 250) < 0.5f && std::abs(py - 150) < 0.5f);
    CHECK(!map_camera_v1({}, std::nullopt, view, zoomed, error));
}
} // namespace

int main() {
    frustum_tests();
    inside_tests();
    tracker_tests();
    persistence_tests();
    map_tests();
    if (failures) {
        std::cerr << failures << " map visit check(s) failed\n";
        return 1;
    }
    std::cout << "map_visit tests passed\n";
    return 0;
}
