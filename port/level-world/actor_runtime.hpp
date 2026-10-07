#pragma once
#include "actor_rotation.hpp"
#include "move_state.hpp"
#include "native_body.hpp"
#include "navigation_controller.hpp"
#include "visual_motion.hpp"
#include <string>

namespace dh2::actor {
// Original base GameObject::GetSpeed returns4. Character virtual overrides,
// camera/FSM policy and animation speed are explicit caller facts.
inline constexpr float base_virtual_speed=4.f;
struct RuntimeState {
 subobjects::State subobjects;
 navigation::NavigationObject object;
 navigation::PathObject path;
 navigation::PathController controller;
 RotationState rotation;
 physical::BodyState body;
 physical::TransformRequest transform;
 float previous_position[3],previous_rotation[3],target_position[3];
};
struct RuntimePolicy {
 // update_path is replaced by the actual character flag decoder. Other
 // virtual/debug facts are retained, including physical Stop eligibility.
 navigation::ControllerPolicy path;
 std::uint32_t validating_camera,has_auxiliary,auxiliary_type,auxiliary_mode;
 float virtual_speed;
};
struct RuntimeRequest {
 RuntimeState* state;
 physical::NativeBody* native_body;
 visual::SceneBinding* binding;
 scene::Scene* scene;
 const navigation::CollisionWorld* geometry;
 const navigation::Graph* graph;
 navigation::ObstacleRegistry* registry;
 const navigation::MotionPolicy* motion_policy;
 navigation::ControllerWorkspace* workspace;
 const navigation::AvoidanceScene* avoidance;
 const std::int32_t* resolved224;
 const RuntimePolicy* policy;
 // Handles explicit camera/auxiliary, visual update/rotation-query/scaling
 // virtual services. Always explicit, including an absent camera/auxiliary.
 // UINT_MAX reports a service failure;0 remains a valid camera/validation bool.
 const subobjects::Services* services;
 // Null is the original absent target node; otherwise its cached absoluteXYZ.
 const float* target_absolute_position;
 std::uint64_t key;
 std::uint32_t character_flags,dt_ms;
};
enum Phase : std::uint32_t {not_started=0,path_phase=1,rotation_phase=2,subobjects_phase=3,target_phase=4,completed=5};
struct RuntimeResult {
 navigation::ControllerResult path;
 subobjects::Result subobjects;
 std::uint32_t phase,failed_event,visual_rotation_requested,physical_stop_applied;
};
static_assert(sizeof(RuntimeState)==472&&sizeof(RuntimePolicy)==36&&sizeof(RuntimeRequest)==128&&sizeof(RuntimeResult)==104);
// Borrowed actor phase: pre-frame snapshots -> path/Stop -> rotation ->
// subobjects -> target cache. No scene sample, world Step, AI/FSM or ownership.
// 0 completed,1 malformed,2 capacity,3 service failure. Malformed top-level
// requests do not mutate state or invoke services. Later failures report the
// reached phase; native world/scene side effects are not rolled back.
int update_actor(RuntimeResult&,const RuntimeRequest&,std::string& error);
// Generic GameObject successor: explicit actual virtual policies replace
// Character+520/Property224 decoding. Borrows the SAME generic Root storage;
// no Character animation-root identity or synthetic property sheet exists.
struct GenericVisualBorrowV4 {
 void* context{};visual::Root* root{};
 bool(*update_world)(void*,std::string&){};
 bool(*set_rotation)(void*,const float*,std::string&){};
};
struct GenericTargetPositionServicesV80 {void* context{};int(*borrow)(void*,const float**){};};
struct GenericRuntimeRequestV4 {
 RuntimeRequest actor;
 move::Policy actual_virtual_policy{};
 float actual_rotation_speed{};
 GenericVisualBorrowV4 visual;
 const navigation::ControllerSourceServicesV69* source_path{};
 const subobjects::AuxiliaryBranchServicesV69* source_auxiliary{};
 const GenericTargetPositionServicesV80* source_target{};
};
int update_gameobject_v4(RuntimeResult&,const GenericRuntimeRequestV4&,std::string& error);
}
