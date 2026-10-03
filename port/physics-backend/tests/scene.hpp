#pragma once
#include <cstdint>
namespace dh2::backend_audit {
struct BodyInput {
 std::uint32_t shape,sensor,fixed_rotation,sleeping,bullet,pin,group,category,mask,reserved;
 float position[2],velocity[2],angle,angular_velocity,half_extents[2],radius,local_position[2],density,friction,restitution;
};
struct Scene {std::uint32_t count,steps,iterations,actions;float dt,gravity[2],bounds[4];BodyInput bodies[4];};
struct BodySnapshot {std::uint32_t flags,type,shape_count,shape_type;float values[33];};
struct Snapshot {std::uint32_t body_count,contact_count,pair_count,added,persisted,removed,resolved;float inverse_dt;BodySnapshot bodies[4];};
static_assert(sizeof(BodyInput)==96&&sizeof(Scene)==428&&sizeof(BodySnapshot)==148&&sizeof(Snapshot)==624);
}
extern "C" int dh2_backend_audit_scene(dh2::backend_audit::Snapshot*,const dh2::backend_audit::Scene*);
