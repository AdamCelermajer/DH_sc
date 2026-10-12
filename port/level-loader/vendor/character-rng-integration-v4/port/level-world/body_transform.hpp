#pragma once
#include "physical_controls.hpp"
#include <cstddef>
#include <cstdint>
namespace dh2::physical {
struct BodyTransform {float position[2],rotation[4];};
struct TransformBody {
 BodyState* state;
 float rotation[4],local_center[2],center[2],previous_center[2],previous_angle;
 std::uint32_t reserved;
};
struct TransformShape {void* identity;TransformShape* next;};
struct TransformWorld {
 void* context;void* broadphase;TransformShape* first_shape;
 std::uint8_t locked,reserved[7];
};
struct TransformCallbacks {
 std::uint32_t (*synchronize)(void*,void*,void*,const BodyTransform*,const BodyTransform*);
 void (*destroy_proxy)(void*,void*,void*);
 void (*commit)(void*,void*);
};
static_assert(sizeof(void*)==8);
static_assert(sizeof(BodyTransform)==24&&offsetof(BodyTransform,rotation)==8);
static_assert(sizeof(TransformBody)==56&&offsetof(TransformBody,local_center)==24&&offsetof(TransformBody,previous_angle)==48);
static_assert(sizeof(TransformShape)==16&&sizeof(TransformWorld)==32&&sizeof(TransformCallbacks)==24);
}
extern "C" {
// Original SetXForm return: 1 success (including locked no-op), 0 frozen or
// synchronization failure. -1 malformed native arguments. Synchronize receives
// the SAME new transform twice. Failure freezes, clears velocity and destroys
// all proxies without commit. Callbacks own actual shapes/broadphase services.
int dh2_body_set_transform(dh2::physical::TransformBody*,const dh2::physical::TransformRequest*,dh2::physical::TransformWorld*,const dh2::physical::TransformCallbacks*);
}
