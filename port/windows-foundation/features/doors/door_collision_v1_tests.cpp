#include "door_collision_v1.hpp"

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace dh::foundation::doors;

namespace {
void check(bool value, const char* message) {
    if (!value) { std::cerr << "FAIL: " << message << '\n'; std::exit(1); }
}
void plan(const std::vector<DoorPhysicalBindingV1>& before,
          const std::vector<DoorPhysicalBindingV1>& after,
          std::vector<DoorPhysicalOperationV1>& output) {
    std::string error;
    check(plan_door_physical_reconcile_v1(before, after, output, error), error.c_str());
}
}

int main() {
    std::vector<DoorPhysicalOperationV1> operations;
    const DoorPhysicalBindingV1 closed{101, true, true, true};
    const DoorPhysicalBindingV1 open{101, true, true, false};
    const DoorPhysicalBindingV1 absent{101, true, false, true};
    const DoorPhysicalBindingV1 no_collision{101, false, true, true};

    plan({}, {closed}, operations);
    check(operations.size() == 1 && operations[0].kind == DoorPhysicalOperationKindV1::create &&
          operations[0].stable_id == 101 && operations[0].filter_enabled,
          "closed present collision door creates one enabled body");
    plan({closed}, {closed}, operations);
    check(operations.empty(), "identical snapshot is idempotent");
    plan({closed}, {open}, operations);
    check(operations.size() == 1 && operations[0].kind == DoorPhysicalOperationKindV1::set_filter &&
          !operations[0].filter_enabled, "opened end-state disables existing body's filter");
    plan({open}, {closed}, operations);
    check(operations.size() == 1 && operations[0].kind == DoorPhysicalOperationKindV1::set_filter &&
          operations[0].filter_enabled, "closed end-state enables existing body's filter");
    plan({closed}, {absent}, operations);
    check(operations.size() == 1 && operations[0].kind == DoorPhysicalOperationKindV1::retire,
          "condition-absent door retires its body");
    plan({}, {no_collision}, operations);
    check(operations.empty(), "door_collision false never creates a body");
    plan({closed}, {}, operations);
    check(operations.size() == 1 && operations[0].kind == DoorPhysicalOperationKindV1::retire,
          "removed declaration retires its body");

    operations = {{DoorPhysicalOperationKindV1::set_filter, 999, true}};
    std::string error;
    check(!plan_door_physical_reconcile_v1({}, {closed, closed}, operations, error) &&
          operations.size() == 1 && operations[0].stable_id == 999,
          "duplicate current IDs fail atomically");
    check(!plan_door_physical_reconcile_v1({{0, true, true, true}}, {}, operations, error),
          "zero stable ID is rejected");

    DoorCollisionVolumeV1 wall{};
    wall.binding = {201, true, true, true};
    wall.bounds_xy[0] = 0; wall.bounds_xy[1] = 0; wall.bounds_xy[2] = 1; wall.bounds_xy[3] = 10;
    DoorActorCollisionV1 actor{};
    actor.radius_game_units = 1;
    actor.category_bits = 1; actor.mask_bits = 0xffff;
    DoorMotionResultV1 motion{};
    const float cross_from[]{-5, 5}, cross_to[]{5, 5};
    check(constrain_door_motion_v1({wall}, actor, cross_from, cross_to, motion, error), error.c_str());
    check(motion.clipped && motion.first_blocking_door == 201 && motion.position_xy[0] < -1.9f,
          "swept segment cannot tunnel through a closed door");

    // A rounded Minkowski corner must not become the square corner of an
    // expanded AABB: this segment enters both expanded slabs but stays outside
    // the source rectangle's actor-radius-plus-slop corner arc.
    const float corner_from[]{-2.0f, -2.0f}, corner_near_miss[]{-1.1f, -1.1f};
    check(constrain_door_motion_v1({wall}, actor, corner_from, corner_near_miss, motion, error), error.c_str());
    check(!motion.clipped && motion.position_xy[0] == corner_near_miss[0] &&
          motion.position_xy[1] == corner_near_miss[1],
          "diagonal near-miss remains outside the rounded source contact corner");

    const float slide_from[]{-5, 5}, slide_to[]{5, 7};
    check(constrain_door_motion_v1({wall}, actor, slide_from, slide_to, motion, error), error.c_str());
    check(motion.clipped && motion.position_xy[0] <= -1.99f && motion.position_xy[1] > 6.9f,
          "diagonal contact clips inward motion and preserves tangent sliding");

    const float touch_from[]{-2, 5}, away_to[]{-6, 5};
    check(constrain_door_motion_v1({wall}, actor, touch_from, away_to, motion, error), error.c_str());
    check(!motion.clipped && motion.position_xy[0] == away_to[0], "motion away from contact remains free");
    const float inward_to[]{4, 5};
    check(constrain_door_motion_v1({wall}, actor, touch_from, inward_to, motion, error), error.c_str());
    check(motion.clipped && motion.first_blocking_door == 201, "movement starting at contact cannot enter door");
    const float tangent_to[]{-2, 8};
    check(constrain_door_motion_v1({wall}, actor, touch_from, tangent_to, motion, error), error.c_str());
    check(!motion.clipped && motion.position_xy[1] == tangent_to[1], "motion tangent at contact remains free");

    auto open_wall = wall; open_wall.binding.filter_enabled = false;
    check(constrain_door_motion_v1({open_wall}, actor, cross_from, cross_to, motion, error), error.c_str());
    check(!motion.clipped && motion.position_xy[0] == cross_to[0], "opened door filter permits passage");
    auto no_collision_wall = wall; no_collision_wall.binding.collision_enabled = false;
    no_collision_wall.bounds_xy[0] = std::numeric_limits<float>::quiet_NaN();
    check(constrain_door_motion_v1({no_collision_wall}, actor, cross_from, cross_to, motion, error), error.c_str());
    check(!motion.clipped && motion.position_xy[0] == cross_to[0],
          "authored door_collision=false has no body or motion gate");
    auto absent_wall = wall; absent_wall.binding.present = false;
    absent_wall.bounds_xy[0] = std::numeric_limits<float>::quiet_NaN();
    check(constrain_door_motion_v1({absent_wall}, actor, cross_from, cross_to, motion, error), error.c_str());
    check(!motion.clipped && motion.position_xy[0] == cross_to[0],
          "condition-absent door has no body or motion gate");
    actor.move_collision_enabled = false;
    check(constrain_door_motion_v1({wall}, actor, cross_from, cross_to, motion, error), error.c_str());
    check(!motion.clipped && motion.position_xy[0] == cross_to[0], "MoveActor ignore-collision policy permits passage");
    actor.move_collision_enabled = true;
    actor.physical_collision_enabled = false;
    check(constrain_door_motion_v1({wall}, actor, cross_from, cross_to, motion, error), error.c_str());
    check(!motion.clipped && motion.position_xy[0] == cross_to[0], "actor without physical collision passes");
    actor.physical_collision_enabled = true;
    actor.mask_bits = 0;
    check(constrain_door_motion_v1({wall}, actor, cross_from, cross_to, motion, error), error.c_str());
    check(!motion.clipped, "source group/mask admission can reject door contact");
    actor.mask_bits = 0xffff;
    actor.group_index = -666;
    wall.group_index = -666;
    check(constrain_door_motion_v1({wall}, actor, cross_from, cross_to, motion, error), error.c_str());
    check(!motion.clipped, "equal negative source collision groups reject contact");
    actor.group_index = 4;
    wall.group_index = 4;
    actor.mask_bits = 0;
    actor.category_bits = 0;
    check(constrain_door_motion_v1({wall}, actor, cross_from, cross_to, motion, error), error.c_str());
    check(motion.clipped, "equal positive source collision groups admit contact");
    actor.group_index = 0; wall.group_index = 0;
    actor.mask_bits = 0xffff; actor.category_bits = 1;

    auto second_wall = wall; second_wall.binding.stable_id = 202;
    second_wall.bounds_xy[0] = 4; second_wall.bounds_xy[2] = 5;
    const float long_to[]{10, 5};
    check(constrain_door_motion_v1({second_wall, wall}, actor, cross_from, long_to, motion, error), error.c_str());
    check(motion.clipped && motion.first_blocking_door == 201, "nearest of multiple doors clips first");
    auto malformed = wall; malformed.bounds_xy[2] = malformed.bounds_xy[0];
    check(!constrain_door_motion_v1({malformed}, actor, cross_from, cross_to, motion, error),
          "missing source AABB fails closed");
    check(!constrain_door_motion_v1({wall, wall}, actor, cross_from, cross_to, motion, error),
          "duplicate active door IDs are rejected");
    std::cout << "PASS: door physical reconciliation branches\n";
}
