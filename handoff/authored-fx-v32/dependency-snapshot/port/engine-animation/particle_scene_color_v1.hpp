#pragma once
#include <cstdint>
namespace dh2::animation {
struct ParticleSceneColorServicesV1 {
 void* context{};
 // Required original scene-node/light-manager lookup for driver types&7!=0.
 int (*query)(void*,std::uint32_t* actual_color_word){};
};
// Source CNew::onAnimate 64dafc..64db68. actual_driver_type must come from
// the actual source IVideoDriver receiver; GLES2's original getter returns8.
// Output is written before a reached provider failure, preserving white prefix.
int particle_scene_color_v1(std::uint32_t actual_driver_type,
 const ParticleSceneColorServicesV1&,std::uint32_t& color);
}
