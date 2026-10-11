// B069: AI_IsInInteractionRange rule and the world-to-camera movement conversion.
#include "interaction_approach_v1.hpp"
#include "interaction_facing_v1.hpp"

#include <cmath>
#include <cstdio>

using namespace dh::foundation;

namespace {
int failures = 0;
void check(bool ok, const char* name) {
    std::printf("%s %s\n", ok ? "PASS" : "FAIL", name);
    if (!ok) ++failures;
}
InteractionRangeInputV1 chest(float distance, float melee = 50.0f, float radius = 30.0f, float interact = 100.0f) {
    InteractionRangeInputV1 in;
    in.owner_target_position = {0, 0, 0};
    in.interaction_spot = {distance, 0, 0};
    in.owner_melee_radius = melee;
    in.target_radius = radius;
    in.owner_interact_radius = interact;
    in.target_type = 0;
    return in;
}
} // namespace

int main() {
    {
        float yaw=.7f;
        check(!interaction_facing_v1({0,0,0},{0,-100,0},false,false,yaw)&&yaw==.7f,"look-at not requested preserves yaw");
        check(!interaction_facing_v1({0,0,0},{0,-100,0},true,true,yaw)&&yaw==.7f,"sitting actor preserves yaw");
        check(!interaction_facing_v1({0,0,0},{0,0,100},true,false,yaw)&&yaw==.7f,"same XY preserves yaw (height is not heading)");
        check(interaction_facing_v1({0,0,0},{0,0,100},true,false,yaw,false)&&yaw==0.f,"B085 destructible keeps its existing coincident atan2 heading policy");
        check(interaction_facing_v1({-2198.67f,1035.187f,255},{-2198.67f,935.187f,250},true,false,yaw)&&std::fabs(yaw+1.5707963f)<1e-6f,"side-facing chest requests target heading");
        for(const auto target: {std::array<float,3>{1,1,0},{-1,1,0},{-1,-1,0},{1,-1,0}}) {
            check(interaction_facing_v1({0,0,0},target,true,false,yaw)&&std::cos(yaw)*target[0]+std::sin(yaw)*target[1]>1.4f,"heading faces target in each quadrant");
        }
    }
    // limit = InteractRadius (100); v = d - melee - radius = d - 80.
    check(interaction_in_range_v1(chest(180.0f)), "chest exactly at the limit is in range (v == limit)");
    check(!interaction_in_range_v1(chest(180.5f)), "chest just beyond the limit is out of range");
    check(interaction_in_range_v1(chest(40.0f)), "chest next to the hero is in range");
    {
        auto in = chest(130.0f);
        in.target_type = 8; // destructible: in range only inside the weapon reach (v <= 0)
        check(!interaction_in_range_v1(in), "destructible at v=50 is out of range (type 8 limit 0)");
        in.interaction_spot = {80.0f, 0, 0};
        check(interaction_in_range_v1(in), "destructible at v=0 is in range");
    }
    {
        auto in = chest(80.5f);
        in.has_interaction_node = true; // limit 80, no radii
        check(!interaction_in_range_v1(in), "interaction node: 80.5 is out of range");
        in.interaction_spot = {80.0f, 0, 0};
        check(interaction_in_range_v1(in), "interaction node: 80 is in range, radii ignored");
    }
    {
        auto in = chest(100.0f);
        in.owner_target_position = {0, 0, 150.0f}; // 3D distance (source uses all three axes)
        check(!interaction_in_range_v1(in), "vertical offset counts (3D distance 180.3 > 180)");
    }
    {
        auto in = chest(100.0f);
        in.owner_melee_radius = std::nanf("");
        check(!interaction_in_range_v1(in), "non-finite input is out of range");
    }
    {
        InteractionMove2DV1 m;
        // Camera right = +x, forward = +y: world +y is move (0, 1).
        check(camera_relative_move_v1(1, 0, 0, 1, 0, 5, m) && std::fabs(m.x) < 1e-5f && std::fabs(m.y - 1) < 1e-5f,
              "identity basis: world +y is forward");
        // Camera looking along world -x (forward=-x, right=+y): world +y is move (1, 0).
        check(camera_relative_move_v1(0, 1, -1, 0, 0, 3, m) && std::fabs(m.x - 1) < 1e-5f && std::fabs(m.y) < 1e-5f,
              "rotated basis: world +y is right");
        // Round trip for a skewed basis.
        const float rx = 0.8f, ry = 0.6f, fx = -0.6f, fy = 0.8f;
        check(camera_relative_move_v1(rx, ry, fx, fy, 3, -4, m), "skewed basis accepted");
        const float wx = rx * m.x + fx * m.y, wy = ry * m.x + fy * m.y;
        check(std::fabs(wx - 0.6f) < 1e-5f && std::fabs(wy + 0.8f) < 1e-5f, "round trip reproduces the world direction");
        check(!camera_relative_move_v1(1, 0, 2, 0, 1, 0, m), "degenerate basis rejected");
        check(!camera_relative_move_v1(1, 0, 0, 1, 0, 0, m), "zero direction rejected");
    }
    std::printf("%s\n", failures == 0 ? "ALL PASS" : "FAILURES");
    return failures == 0 ? 0 : 1;
}
