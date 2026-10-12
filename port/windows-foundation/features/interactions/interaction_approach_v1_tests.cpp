// B069: AI_IsInInteractionRange rule and the world-to-camera movement conversion.
#include "interaction_approach_v1.hpp"
#include "interaction_facing_v1.hpp"
#include "../../../level-world/character_path_commands.hpp"

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
        check(!interaction_facing_v1({0,0,0},{0,0,100},true,false,yaw)&&yaw==.7f,"same XY preserves yaw (source LookTowards, height is not heading)");
        check(!interaction_facing_v1({0,0,0},{std::nanf(""),0,0},true,false,yaw)&&yaw==.7f,"non-finite target preserves yaw");
        // B151: source GameObject::LookTowards 0x393b1c constants: +X -> 0x3fc90fdb (pi/2), -X -> 0x4096cbe4 (3pi/2),
        // otherwise atan(x/-y) (+/-pi when y>0). Yaw 0 faces -Y. The old atan2(dy,dx) form was a quarter turn off.
        check(interaction_facing_v1({0,0,0},{100,0,0},true,false,yaw)&&std::fabs(yaw-1.5707964f)<1e-6f,"+X target -> source pi/2");
        check(interaction_facing_v1({0,0,0},{-100,0,0},true,false,yaw)&&std::fabs(yaw-4.7123890f)<1e-6f,"-X target -> source 3pi/2");
        check(interaction_facing_v1({0,0,0},{0,-100,0},true,false,yaw)&&std::fabs(yaw)<1e-7f,"-Y target -> source 0");
        check(interaction_facing_v1({0,0,0},{0,100,0},true,false,yaw)&&std::fabs(std::fabs(yaw)-3.1415927f)<1e-6f,"+Y target -> source pi");
        check(interaction_facing_v1({-2198.67f,1035.187f,255},{-2198.67f,935.187f,250},true,false,yaw)&&std::fabs(yaw)<1e-6f,"side-facing chest (target at -Y) -> yaw 0");
        // Rendered forward: sourceActorWorld (main.cpp) maps local X to (cos,sin) and local Y to (-sin,cos);
        // the character mesh faces local -Y, i.e. world (sin yaw, -cos yaw). It must point at the target in every quadrant.
        int quadrant=0;
        for(const auto target: {std::array<float,3>{1,1,0},{-1,1,0},{-1,-1,0},{1,-1,0},{3,-0.5f,0},{-0.2f,7,0}}) {
            const float len=std::hypot(target[0],target[1]);
            const bool ok=interaction_facing_v1({0,0,0},target,true,false,yaw)&&
                (std::sin(yaw)*target[0]-std::cos(yaw)*target[1])>0.9999f*len;
            check(ok,quadrant++<4?"rendered forward faces the target in each quadrant":"rendered forward faces an off-axis target");
        }
        // Same arithmetic as the source LookAt main already uses for the hero (Injured Blur dh2_character_look_at_point).
        for(const auto target: {std::array<float,3>{-1229.64f,-1224.21f,250},{-7142.83f,-1597.75f,250},{900,-212,258}}) {
            const std::array<float,3> owner{-903.664f,-1235.1f,255};
            dh2::character::LookAtState16 look{{owner[0],owner[1],owner[2]},.25f};
            float facing=.25f;
            check(dh2_character_look_at_point(&look,target.data())==0&&interaction_facing_v1(owner,target,true,false,facing)&&facing==look.heading_angle,
                  "interaction facing equals the source LookAt heading");
        }
        // R6 runtime fixture (v21.6 B151): the source movement kernel walked toward the vase with yaw -1.60421; the arrival
        // turn must keep that heading (the old atan2 form jumped to 3.10818, a quarter turn to the right of the vase).
        check(interaction_facing_v1({-903.664f,-1235.1f,255},{-1229.64f,-1224.21f,250},true,false,yaw)&&
              std::fabs(std::remainder(yaw-(-1.60421f),6.2831853f))<2e-3f,"approach arrival keeps the walk kernel's heading toward the vase");
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
