#pragma once
#include <cstdint>
namespace dh2::animation {
// Full uchar4 source sampler, distinct from component3/alpha interpreter.
struct MaterialColorAccessorV3 { const std::uint8_t* values{};std::uint32_t count{}; };
}
extern "C" int dh2_material_color_between_v3(std::uint8_t output[4],const dh2::animation::MaterialColorAccessorV3*,std::uint32_t key,std::uint32_t next,float fraction);
