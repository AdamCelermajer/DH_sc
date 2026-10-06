#pragma once
#include <cstddef>
#include <cstdint>
namespace dh2::physical {
// Native logical view of fields read/written by the recovered routines. This
// is not a binary overlay of either ARM32 or ARM64 Box2D's body allocation.
struct BodyState {
 std::uint32_t flags;
 float position[2],angle,linear_velocity[2],angular_velocity,force[2],torque,sleep_time,radius;
};
struct TransformRequest {float position[2],angle;std::uint32_t pending;};
constexpr std::uint32_t sleeping_flag=8;
static_assert(sizeof(void*)==8,"Native physical controls require 64-bit pointers");
static_assert(sizeof(BodyState)==48&&offsetof(BodyState,linear_velocity)==16&&offsetof(BodyState,sleep_time)==40&&offsetof(BodyState,radius)==44);
static_assert(sizeof(TransformRequest)==16&&offsetof(TransformRequest,pending)==12);
}
extern "C" {
// 0 completed, 1 malformed pointer/overlap, atomically rejected. IEEE inputs
// (including signed zero/infinity/NaN) retain original arithmetic behavior.
int dh2_physical_wake(dh2::physical::BodyState*);
int dh2_physical_set_linear(dh2::physical::BodyState*,const float* xy);
int dh2_physical_add_linear(dh2::physical::BodyState*,const float* xy_upper_caps);
int dh2_physical_set_angular(dh2::physical::BodyState*,const float* angular);
// Output x*100,y*100,current angle,radius*100; output cannot overlap state.
int dh2_physical_query(float* xy_angle_radius,const dh2::physical::BodyState*);
// Emits exact b2Body::SetXForm args: x*.01,y*.01,current angle. Does not apply
// the transform, synchronize shape proxies, collide, or step a Box2D world.
int dh2_physical_request_position(dh2::physical::TransformRequest*,const dh2::physical::BodyState*,const float* xy);
// Original Stop's physical sequence: begin zeroes linear/angular velocity,
// waking via setAngularVelocity, and emits transform. Caller applies transform
// before finish sets sleep and clears velocity/forces/torque/sleep time.
int dh2_physical_stop_begin(dh2::physical::TransformRequest*,dh2::physical::BodyState*,const float* xy);
int dh2_physical_stop_finish(dh2::physical::BodyState*);
}
