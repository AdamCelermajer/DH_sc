#pragma once

#include "door_runtime_v1.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation::doors {

enum class DoorPhysicalOperationKindV1 : std::uint8_t { create, retire, set_filter };

struct DoorPhysicalOperationV1 {
    DoorPhysicalOperationKindV1 kind = DoorPhysicalOperationKindV1::create;
    std::uint64_t stable_id = 0;
    bool filter_enabled = true;
};

// Plans an idempotent diff between the last successfully applied snapshot and the
// current DoorRuntime snapshot. The caller executes operations through the actual
// source Door physical owner and commits current as its new previous snapshot only
// after every operation succeeds. No geometry is synthesized here.
bool plan_door_physical_reconcile_v1(const std::vector<DoorPhysicalBindingV1>& previous,
                                     const std::vector<DoorPhysicalBindingV1>& current,
                                     std::vector<DoorPhysicalOperationV1>& operations,
                                     std::string& error);

struct DoorCollisionVolumeV1 {
    DoorPhysicalBindingV1 binding;
    // Exact owner absolute AABB after source VisualObject::ApplyMeshBox, in game units.
    // The caller borrows this from the same CanonicalDoorV27 used to create its physical body.
    float bounds_xy[4]{}; // minX, minY, maxX, maxY
    std::int32_t group_index = 0;
    std::uint16_t category_bits = 2, mask_bits = 0xffff;
};

struct DoorActorCollisionV1 {
    float radius_game_units = 0;
    std::int32_t group_index = 0;
    std::uint16_t category_bits = 0, mask_bits = 0;
    bool physical_collision_enabled = true;
    // Source MoveActor's ignore-collision policy is passed through by the caller.
    bool move_collision_enabled = true;
};

struct DoorMotionResultV1 {
    float position_xy[2]{};
    bool clipped = false;
    std::uint64_t first_blocking_door = 0;
};

// Swept-circle movement against the exact source door AABBs, with continuous
// segment clipping and tangent projection for sliding. Input positions/radii
// are in game units; the 0.5-unit allowance is the source solver's 0.005m
// contact tolerance. Disabled actor collision and source filter rejects pass.
bool constrain_door_motion_v1(const std::vector<DoorCollisionVolumeV1>& doors,
                              const DoorActorCollisionV1& actor,
                              const float from_xy[2], const float desired_xy[2],
                              DoorMotionResultV1& result, std::string& error);

} // namespace dh::foundation::doors
