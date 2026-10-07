#pragma once
#include <array>
#include <cstdint>
#include <cstring>
namespace dh2::world {
// Original ARM __fixunssfsi8be2a0 followed by uxtb. Preserve saturation,
// negative/NaN and fractional behavior without undefined native casts.
inline std::uint8_t source_fog_color_byte_v68(float value)noexcept{
 std::uint32_t bits;std::memcpy(&bits,&value,4);
 if(bits&0x80000000u)return 0;
 const auto exponent=(bits>>23)&255u;
 if(exponent<127)return 0;
 if(exponent>158){
  if(exponent==255&&(bits&0x007fffffu))return 0;
  return 255;
 }
 const auto mantissa=(bits<<8)|0x80000000u;
 return static_cast<std::uint8_t>(mantissa>>(158-exponent));
}
// Native driver parameter backing on the SAME retained SceneManager owner.
// Ambient104 and fog transform458/45c/460 have explicit source C1 stores.
// Original derived flags430..432 are indeterminate in captured C1; the native
// backend starts deterministic disabled until real source setters produce
// them. This is a safety adaptation, not an alleged original C1 constant.
struct SceneManagerFrameFieldsV67 {
 std::array<float,4> ambient104{};
 std::uint8_t lighting430{},specular431{},fog432{};
 std::uint8_t batching43c_v96{};
 bool native_batching43c_produced_v96{};
 std::array<float,3> fog_transform458{0,0,1};
 std::array<float,2> fog_start_end{};
 std::array<std::uint8_t,4> fog_color{};
 std::uint64_t parameter_revision{};
};
}
