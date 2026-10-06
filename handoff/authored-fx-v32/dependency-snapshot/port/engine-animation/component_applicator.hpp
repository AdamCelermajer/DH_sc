#pragma once
#include "../scene-materials/scene.hpp"
#include <cstdint>

namespace dh2::animation {
struct ComponentNodeState {float position[3],scale[3];std::uint32_t dirty;};
static_assert(sizeof(ComponentNodeState)==28&&sizeof(void*)==8);
// Scene::Node has no original dirty field. The owner supplies dirty storage;
// this bridge writes local graph values in target call order and leaves world
// matrices untouched until the owner explicitly invokes scene::update_world.
int apply_component(scene::Node&,std::uint32_t& dirty,std::uint32_t type,const float* vector3);
int apply_blended_component(scene::Node&,std::uint32_t& dirty,std::uint32_t type,
                            const float* slot_vectors3,const float* weights,std::int32_t count);
}
extern "C" {
// Types2/3/4 positionX/Y/Z,11/12/13 scaleX/Y/Z. All carry FULL12-byte vector
// values. Applying a component writes all XYZ, with no live-axis getter/merge.
// Position setter ORs dirty8; scale setter ORs2. Other state is preserved.
// Return0 success,1 malformed atomically. Reject state/input overlap, unaligned
// pointers and counts outside0..65536. Blend output must be disjoint from reads.
// Count1 copies bits/ignores weight; count0 sets +0XYZ; otherwise ordered
// separate f32 multiply/add for all3 components, including zero-weight slots.
// IEEE values are accepted. ArithmeticNaNs have classification parity.
int dh2_animation_component_blend(float* output3,std::uint32_t type,
                                  const float* slot_vectors3,const float* weights,std::int32_t count);
int dh2_animation_component_apply(dh2::animation::ComponentNodeState*,std::uint32_t type,const float* vector3);
int dh2_animation_component_apply_blended(dh2::animation::ComponentNodeState*,std::uint32_t type,
                                          const float* slot_vectors3,const float* weights,std::int32_t count);
}
