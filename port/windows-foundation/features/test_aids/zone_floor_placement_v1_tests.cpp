// v21.8 magefix2: `--player-to <zone>` places the hero on a walkable point inside the zone, not on its (possibly floorless) centre.
#include "zone_floor_placement_v1.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation::test_aids;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
}  // namespace

int main() {
    try {
        // The Mud Lord exit box (m2-scan): centre (-20810, 10892, -132.6), half (733.3, 552.5, 557.3). The boss-room floor is a
        // half plane ending at x = -20140 (floor hit at -20140, none at -20180), height -113.968.
        ZoneBoxV1 box;
        box.centre = {-20810.1f, 10892.6f, -132.611f};
        box.half = {733.339f, 552.493f, 557.277f};
        const FloorProbeV1 half_plane = [](float x, float, float, float& height) {
            if (x < -20140.0f) return false;
            height = -113.968f;
            return true;
        };
        // The centre itself has no floor: the old aid would have placed the hero there.
        float height = 0;
        check(!half_plane(box.centre[0], box.centre[1], box.centre[2], height), "fixture: the centre must be floorless");
        const auto placed = zone_floor_placement_v1(box, half_plane);
        check(placed.has_value(), "No walkable point found although the floor reaches into the box");
        check((*placed)[0] >= -20140.0f && (*placed)[0] < -20140.0f + 40.0f + 1.0f,
              "Placement is not the floor point nearest to the zone centre");
        check(std::fabs((*placed)[1] - box.centre[1]) <= 40.0f, "Placement drifted away from the centre row");
        check((*placed)[2] == -113.968f, "Placement must carry the floor height");
        check(zone_contains_v1(box, *placed), "Placement is outside the zone box");

        // A floor under the centre wins (placement = centre column, no regression for zones that were already walkable).
        const FloorProbeV1 everywhere = [](float, float, float, float& h) {h = -100.0f; return true;};
        const auto centred = zone_floor_placement_v1(box, everywhere);
        check(centred.has_value() && std::fabs((*centred)[0] - box.centre[0]) < 1e-3f && std::fabs((*centred)[1] - box.centre[1]) < 1e-3f,
              "A walkable centre must stay the placement");

        // No floor inside the box: nothing is invented.
        const FloorProbeV1 nothing = [](float, float, float, float&) { return false; };
        check(!zone_floor_placement_v1(box, nothing).has_value(), "A floorless box produced a placement");
        // A floor outside the box (past the margin) is not accepted.
        const FloorProbeV1 outside = [](float x, float, float, float& h) {
            if (x > -20000.0f) {h = -113.968f; return true;}
            return false;
        };
        check(!zone_floor_placement_v1(box, outside).has_value(), "A floor outside the zone box was accepted");

        // A rotated box (45 degrees) is honoured: the point must be inside the oriented box, not just its AABB.
        ZoneBoxV1 rotated;
        rotated.centre = {0, 0, 0};
        const float s = std::sqrt(0.5f);
        rotated.axes = {{{s, s, 0}, {-s, s, 0}, {0, 0, 1}}};
        rotated.half = {100, 100, 100};
        const FloorProbeV1 corner = [](float x, float y, float, float& h) {
            if (x > 90.0f && y > 90.0f) {h = 0; return true;}  // inside the AABB corner, outside the diamond
            return false;
        };
        check(!zone_floor_placement_v1(rotated, corner, 10.0f, 5.0f).has_value(), "The AABB corner of a rotated box was accepted");

        std::cout << "zone_floor_placement_v1 passed placed=" << (*placed)[0] << ',' << (*placed)[1] << ',' << (*placed)[2] << '\n';
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "zone_floor_placement_v1 failed: " << e.what() << '\n';
        return 1;
    }
}
