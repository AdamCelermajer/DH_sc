#pragma once
#include "physical_controls.hpp"
namespace dh2::subobjects {
// Logical fields of GameObject and its PF/auxiliary objects, not binary overlays.
struct State {
 float position[3],destination[3],heading[3],previous_position[3],rotation;
 float local_bounds[6],absolute_bounds[6],path_target[3],auxiliary_position[3];
 std::uint32_t path_count;
};
struct Policy {
 std::uint32_t position_from_visual,position_from_physics,rotation_from_visual,rotation_from_physics;
 std::uint32_t visual_with_rotation,validating_floor,validating_camera,has_visual;
 std::uint32_t has_auxiliary,auxiliary_type,auxiliary_mode,reserved;
 float speed;
};
enum Event : std::uint32_t {
 visual_update=1,physical_update=2,visual_apply_position=3,visual_sync_position=4,
 validate_position=5,apply_body_transform=6,physical_set_velocity=7,physical_wake=8,
 visual_apply_rotation=9,visual_sync_rotation=10,visual_sync_scaling=11,
 camera_get=12,camera_can_move=13,auxiliary_update=14,camera_position=15,
 camera_set_free=16,get_speed=17
};
// Service boundaries are synchronous. State/body may change in update/root
// motion/transform callbacks. validate_position edits XYZ and returns validity;
// camera_get returns presence and writes enabled to payload[0]; camera_can_move
// returns validity; camera_position writes XYZ. get_speed is an observer of the
// supplied virtual fact. apply_body_transform receives a separate Box2D XY+
// angle float[3]; the complete pending TransformRequest is available via the
// caller's context/Request::transform. The payload has no integer aliasing.
// Setter/visual events observe payloads; remaining callbacks receive nullptr.
using Service=std::uint32_t (*)(void*,std::uint32_t,float*);
struct Services {void* context;Service invoke;};
struct AuxiliaryBranchV69 {std::uint32_t type{},mode{};const float* position_c{};};
struct AuxiliaryBranchServicesV69 {void* context{};int(*borrow)(void*,AuxiliaryBranchV69*){};};
struct Request {State* state;physical::BodyState* body;physical::TransformRequest* transform;const Policy* policy;const Services* services;};
struct Result {std::uint32_t at_destination,accepted_physics_position;};
static_assert(sizeof(State)==128&&sizeof(Policy)==52&&sizeof(Services)==16&&sizeof(Request)==40&&sizeof(Result)==8);
}
extern "C" {
// 0 completed; 1 malformed pointer/overlap/reserved/flag, rejected before any
// callback or mutation. IEEE values intentionally follow original comparisons.
int dh2_subobjects_update(dh2::subobjects::Result*,const dh2::subobjects::Request*);
int dh2_subobjects_update_source_v69(dh2::subobjects::Result*,const dh2::subobjects::Request*,const dh2::subobjects::AuxiliaryBranchServicesV69*);
}
