// Wave 06 (recon v2): glitch::collada::animation_track key-based interpolators and
// applicators, rebuilt against the layouts/animation_track.hpp member layouts.
//
// Guest memory model: layout pointers are 32-bit guest words. Guest reads go through
// guest_load_u32 (extern). Out-parameters that the original writes through a guest
// int/float* are host word buffers here (std::uint32_t*), written with word_of().
//
// Externs (bound by the integrator or test doubles, not in the layout headers):
//   guest_load_u32, accessor_has_default_value, accessor_get_default_value,
//   set_parameter_cvt_vec2/vec3/vec4 (IMaterialParameters::setParameterCvt).
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_06_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_06_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"

namespace glitch::collada::recon_v2_06 {

// Keyframe table entry stride and output-index offset (layout comment: 28-byte entries,
// output index uint32 at entry+0x18). No named member exists in the layout header.
constexpr std::uint32_t kKeyframeStride = 28;
constexpr std::uint32_t kKeyframeOutputIndexOffset = 0x18;

// Externs. Guest word load at a guest address.
std::uint32_t guest_load_u32(std::uint32_t guest_addr);

// SAnimationAccessor::hasDefaultValue (0x669e54) reads track +0x18, which has no member in the
// layout header, so it stays extern. Same for getDefaultValue (0x669e68), which returns the
// guest word at *(track +0x18) + 8.
bool accessor_has_default_value(const SAnimationAccessor* a1);
std::uint32_t accessor_get_default_value(const SAnimationAccessor* a1);

// IMaterialParameters::setParameterCvt for vector2/3/4 (not in the layout headers).
int set_parameter_cvt_vec2(std::uint32_t target, std::uint16_t parameter_id, int zero, const std::uint32_t* value);
int set_parameter_cvt_vec3(std::uint32_t target, std::uint16_t parameter_id, int zero, const std::uint32_t* value);
int set_parameter_cvt_vec4(std::uint32_t target, std::uint16_t parameter_id, int zero, const std::uint32_t* value);

// Bit helpers (C++17, no bit_cast).
inline std::uint32_t word_of(float f) {
    std::uint32_t w;
    std::memcpy(&w, &f, sizeof w);
    return w;
}
inline float float_of(std::uint32_t w) {
    float f;
    std::memcpy(&f, &w, sizeof f);
    return f;
}
inline float guest_load_f32(std::uint32_t guest_addr) { return float_of(guest_load_u32(guest_addr)); }

// SAnimationAccessor::getOutput (0x669e24), rebuilt from the layout members:
// outputs + 8 * keyframes[key].outputIndex + 4.
std::uint32_t accessor_get_output(const SAnimationAccessor* a1, int key);

// Interpolators. Each keeps the pseudocode signature; out-buffers are word arrays.
// 0x61d97c  f4 K3, two keys
float interp_f4_k3_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5);
// 0x61e560  f2 K0, two keys
float interp_f2_k0_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5);
// 0x61e6ec  f2 K0, three keys (relative)
float interp_f2_k0_relative3(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint32_t* a6);
// 0x61e88c  f2 K1, two keys
float interp_f2_k1_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5);
// 0x61ea18  f2 K1, three keys (relative)
float interp_f2_k1_relative3(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint32_t* a6);
// 0x61ebc0  f3 K0, two keys
float interp_f3_k0_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5);
// 0x61ed6c  f3 K0, three keys (relative)
float interp_f3_k0_relative3(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint32_t* a6);
// 0x61ef2c  f4 K0, two keys
float interp_f4_k0_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5);
// 0x61f0dc  f4 K0, three keys (relative)
float interp_f4_k0_relative3(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint32_t* a6);
// 0x61f2a4  f4 K1, two keys
float interp_f4_k1_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5);

// Applicators (interpolate, then setParameterCvt on the target with the applicator's parameter id).
// 0x61da4c
int apply_f4_k3(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6);
// 0x61e620
int apply_f2_k0(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6);
// 0x61e94c
int apply_f2_k1(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6);
// 0x61ec94
int apply_f3_k0(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6);
// 0x61effc
int apply_f4_k0(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6);
// 0x61f38c
int apply_f4_k1(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6);

}  // namespace glitch::collada::recon_v2_06

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_06_HPP
