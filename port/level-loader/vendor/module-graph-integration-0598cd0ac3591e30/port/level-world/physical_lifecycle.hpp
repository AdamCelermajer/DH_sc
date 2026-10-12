#pragma once
#include "body_transform.hpp"
namespace dh2::physical {
struct MassData {float mass,center[2],inertia;};
// Logical original fields mass+74, inverse mass+78, inertia+7c, inverse+80,
// body type uint16+2 and PhysicalObject pin byte+27. No binary overlay.
struct LifecycleBody {TransformBody* body;float mass,inverse_mass,inertia,inverse_inertia;std::uint32_t type,pinned;};
struct MassCallbacks {
 void (*compute_mass)(void*,void*,MassData*);
 void (*update_sweep_radius)(void*,void*,const float*);
 void (*refilter_proxy)(void*,void*,void*,const BodyTransform*);
};
static_assert(sizeof(MassData)==16&&sizeof(LifecycleBody)==32&&sizeof(MassCallbacks)==24);
}
extern "C" {
// 0 completed (including world-locked no-op); 1 malformed native caller before
// mutation. IEEE mass/center/inertia words are accepted as the original does.
// Original shape mass, sweep-radius and proxy services are explicit callbacks.
// Mass changes do not adjust linear/angular velocity in this binary.
int dh2_physical_set_mass(dh2::physical::LifecycleBody*,const dh2::physical::MassData*,const dh2::physical::TransformWorld*,const dh2::physical::MassCallbacks*);
int dh2_physical_mass_from_shapes(dh2::physical::LifecycleBody*,const dh2::physical::TransformWorld*,const dh2::physical::MassCallbacks*);
int dh2_physical_pin(dh2::physical::LifecycleBody*,const dh2::physical::TransformWorld*,const dh2::physical::MassCallbacks*);
int dh2_physical_unpin(dh2::physical::LifecycleBody*,const dh2::physical::TransformWorld*,const dh2::physical::MassCallbacks*);
}
