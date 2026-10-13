// Wave 06 (recon v2): glitch::collada::animation_track key-based interpolators and
// applicators, rebuilt against the canonical SAnimationAccessor (collada/SAnimationAccessor.hpp).
//
// The key array is the data of output record 0, the default value is SAnimationAccessor::getDefaultValue. Out-parameters that
// the original writes through a float* are host word buffers here (std::uint32_t*), written with word_of().
//
// Externs (bound by the integrator or test doubles):
//   set_parameter_cvt_vec2/vec3/vec4 (IMaterialParameters::setParameterCvt).
#ifndef GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_06_HPP
#define GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_06_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "glitch-scene-collada-gui/layouts/animation_track.hpp"

namespace glitch::collada::recon_v2_06 {

// IMaterialParameters::setParameterCvt for vector2/3/4 (not in the layout headers).
int set_parameter_cvt_vec2(void* target, std::uint16_t parameter_id, int zero, const std::uint32_t* value);
int set_parameter_cvt_vec3(void* target, std::uint16_t parameter_id, int zero, const std::uint32_t* value);
int set_parameter_cvt_vec4(void* target, std::uint16_t parameter_id, int zero, const std::uint32_t* value);

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
int apply_f4_k3(SAnimationAccessor* a1, int a2, int a3, float a4, void* a5, const animation_track::CApplicatorInfo* a6);
// 0x61e620
int apply_f2_k0(SAnimationAccessor* a1, int a2, int a3, float a4, void* a5, const animation_track::CApplicatorInfo* a6);
// 0x61e94c
int apply_f2_k1(SAnimationAccessor* a1, int a2, int a3, float a4, void* a5, const animation_track::CApplicatorInfo* a6);
// 0x61ec94
int apply_f3_k0(SAnimationAccessor* a1, int a2, int a3, float a4, void* a5, const animation_track::CApplicatorInfo* a6);
// 0x61effc
int apply_f4_k0(SAnimationAccessor* a1, int a2, int a3, float a4, void* a5, const animation_track::CApplicatorInfo* a6);
// 0x61f38c
int apply_f4_k1(SAnimationAccessor* a1, int a2, int a3, float a4, void* a5, const animation_track::CApplicatorInfo* a6);

}  // namespace glitch::collada::recon_v2_06

#endif  // GLITCH_SCENE_COLLADA_GUI_RECON_V2_WAVE_06_HPP
