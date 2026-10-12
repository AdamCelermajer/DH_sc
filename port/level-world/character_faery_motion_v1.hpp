#pragma once

#include "actor_runtime.hpp"
#include "character_faery_controller_v1.hpp"
#include "character_physical_component_v1.hpp"
#include "visual_motion.hpp"
#include "../engine-animation/animation.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_faery_motion_v1 {

// How the shipped Faery Character translates (verified against
// libDungeonHunter2.so):
//
//   * The Faery is an ordinary Character (vtable _ZTV9Character) whose body is
//     a POCharacter/POFaerie. Its translation is GameObject::Update 0x38cbe8 =
//     UpdatePath, UpdateRotation, UpdateSubObjects 0x3943cc, UpdateTargetPosition.
//   * UpdateSubObjects only drives the body with PhysicalObject::
//     setLinearVelocity(dir * GameObject::GetSpeed 0x3400e4 (4.0)) while
//     Character::IsUpdatingPositionFromPhysics (flags bit1) holds. No
//     CharStateMachine state sets that bit for a Faery (CSMove::OnFocus writes
//     0x23c1, CSIdle 0x2380), so the Faery does NOT move by velocity. In CSMove
//     the position-from-visual bit (flags bit0) owns translation: the walk clip
//     (faeries_*_walk.bdae) travels its animated root node (root_camera,
//     about -663 units/s along local Y) and RootSceneNode::_HandleDisplacement
//     0x35cf48 moves the owner node by that delta each animation step;
//     UpdateSubObjects then calls VisualObject::ApplyPosition 0x47124c ->
//     GameObject::SetPosition, mirrors the position into the body with
//     PhysicalObject::setPosition, and validates it with
//     PFWorld::ValidatePosition.
//
// The same GameObject::Update trio and UpdateSubObjects kernel already serve
// the Player and monsters (dh2::actor::update_actor). This module only adapts
// the Faery's retained controller/body/visual owners to that one runtime; it
// owns the scene-phase root-motion bookkeeping and the cross-frame snapshots
// but no body, path, registry or Character of its own.

struct State {
    // Root-motion owner for the Faery's animated root node. The binding's Root
    // is the VisualObject owner node: position is the GameObject position,
    // helper cancels the animated root translation exactly as
    // RootSceneNode::_HandleDisplacement does.
    visual::SceneBinding binding;
    // Private world-space copy of the pose graph: VisualObject world updates
    // performed by the shared runtime land here. The renderer-owned pose scene
    // stays in model space and is placed with `placement_offset()`.
    scene::Scene world_scene;
    bool bound = false;

    // Scene-phase clip tracking (dictionary, last sampled time, delta key).
    std::int32_t tracked_dictionary = -1;
    std::int32_t tracked_ms = -1;
    std::uint32_t sample_serial = 0;

    // GameObject::Update trio state retained between frames.
    actor::RuntimeState runtime{};
    bool runtime_ready = false;
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    not_bound,
    actor_failed,
};

// Binds the VisualObject owner node to the Faery's pose graph. The owner node
// starts at the canonical GameObject position/rotation/scale; the pose scene
// itself stays in model space (placement + offset are applied by the renderer).
bool bind(State&, const scene::Scene& pose_scene, const float position[3],
          const float quaternion[4], const float scale[3], std::string& error);

// Scene phase (RootSceneNode::onAnimate + _HandleDisplacement). The caller has
// already sampled its model-space pose at `ms` with `clip`; this tracks the
// animated root's delta for the same clip and time. `replay` marks a clip
// (re)start, `displacement` is the position-from-visual policy at that time.
bool track_root_motion(State&, const animation::Player& clip,
                       std::int32_t dictionary, std::int32_t ms, bool replay,
                       bool displacement, std::string& error);

// GameObject::SetPosition-style placement (Level::PlaceFaeryAndFollowers,
// WarpTo): the retained trio snapshots and the VisualObject owner position
// follow the canonical GameObject position immediately.
void place(State&, const float position[3]) noexcept;

// Translation applied to the model-space pose scene on top of the owner
// placement: VisualObject owner node * helper * graph (see SceneBinding::
// update_world). Zero until the first displaced animation step.
const float* placement_offset(const State&) noexcept;

// Visual owner rotation (quaternion x,y,z,w) maintained by the shared runtime.
const float* owner_quaternion(const State&) noexcept;

struct Request {
    character_faery_controller_v1::State* controller{};
    character_physical_component_v1::Owner* body{};
    const navigation::CollisionWorld* geometry{};
    const navigation::Graph* graph{};
    navigation::ObstacleRegistry* registry{};
    const navigation::MotionPolicy* motion_policy{};
    const std::int32_t* resolved224{};
    std::uint32_t character_flags{};
    std::uint32_t dt_ms{};
    std::uint64_t key{};
    // GameObject+0x160..0x168 in, accepted position out.
    float position[3]{};
};

struct Result {
    actor::RuntimeResult actor{};
    float position[3]{};
    float previous_position[3]{};
    float rotation_z = 0.f;
    bool rotation_changed = false;
    bool visual_rotation_requested = false;
};

// GameObject::Update for the Faery: UpdatePath -> UpdateRotation ->
// UpdateSubObjects. The controller's canonical NavigationObject, PathObject,
// heading, destination and rotation are marshalled into the shared runtime
// state and written back; the physical component supplies the single
// retained POFaerie body.
Status update(State&, const Request&, Result*, std::string& error);

} // namespace dh2::character_faery_motion_v1
