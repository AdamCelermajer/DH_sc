#pragma once
#include "subobjects_update.hpp"
class b2Body;
namespace dh2::physical {
// Parent world owns b2Body and its genuine shapes/proxies. Radius uses original
// PhysicalObject physics units; game getters multiply it by100.
struct NativeBody {b2Body* body;float radius;std::uint32_t pinned;};
struct NativeBodyObservation {
 float position[2],angle,radius,linear_velocity[2],angular_velocity,mass,inertia,local_center[2],world_center[2];
 std::uint32_t sleeping,frozen,dynamic,bullet,pinned,reserved;
};
// Native service bridge updates only the observable portion of the logical
// BodyState. Its force/torque/sleep-time and nonpublic flag bits are retained;
// they are not read from Box2D or claimed as genuine native force information.
struct NativeSubobjectsBridge {
 NativeBody* native;BodyState* view;TransformRequest* transform;
 void* context;dh2::subobjects::Service fallback;
};
static_assert(sizeof(NativeBody)==16&&sizeof(NativeBodyObservation)==76&&sizeof(NativeSubobjectsBridge)==40);
}
extern "C" {
// 0 completed, 1 malformed pointer/pin state. Original IEEE velocity behavior
// retained. Wake happens only for nonzero linear increments/requests; angular
// always wakes. Public oldBox2D setters alone do not wake.
int dh2_native_body_set_linear(dh2::physical::NativeBody*,const float*);
int dh2_native_body_add_linear(dh2::physical::NativeBody*,const float* xy_upper_caps);
int dh2_native_body_set_angular(dh2::physical::NativeBody*,const float*);
int dh2_native_body_wake(dh2::physical::NativeBody*);
int dh2_native_body_query(float* xy_angle_radius,const dh2::physical::NativeBody*);
int dh2_native_body_observe(dh2::physical::NativeBodyObservation*,const dh2::physical::NativeBody*);
int dh2_native_body_refresh_view(dh2::physical::BodyState*,const dh2::physical::NativeBody*);
// -1 malformed/nonfinite input, 0 frozen/outside-world, 1 success/locked no-op.
// set_position scales gameXY*.01; apply_transform already uses physics units.
int dh2_native_body_set_position(dh2::physical::NativeBody*,const float* game_xy);
int dh2_native_body_apply_transform(dh2::physical::NativeBody*,const dh2::physical::TransformRequest*);
// Stop ignores SetXForm's bool then PutToSleep clears genuine force/torque.
// Rejects nonfinite gameXY before changing the native body.
int dh2_native_body_stop(dh2::physical::NativeBody*,const float* game_xy);
int dh2_native_body_pin(dh2::physical::NativeBody*);
int dh2_native_body_unpin(dh2::physical::NativeBody*);
// For physical_update refresh public fields; transform/velocity/wake execute
// against the genuine native world/body and then refresh that observable view.
// Other services forward to caller fallback. No world step is hidden here.
std::uint32_t dh2_native_body_subobject_service(void*,std::uint32_t,float*);
}
