// Wave 06 (recon v2). Key-based interpolators and applicators for
// glitch::collada::animation_track. Bodies follow the Hex-Rays pseudocode in
// recovered/native/hexrays-all. Default-value accessors are externs (see header).

#include "glitch-scene-collada-gui/recon/v2/wave_06.hpp"

namespace glitch::collada::recon_v2_06 {

namespace {

// Float array base for key i: base + 4 * i (guest address arithmetic).
inline float key_at(std::uint32_t base, int i) {
    return guest_load_f32(base + 4u * static_cast<std::uint32_t>(i));
}

// v[a2] + a4 * (v[a3] - v[a2])
inline float blend2(std::uint32_t base, int a2, int a3, float a4) {
    const float lo = key_at(base, a2);
    return lo + static_cast<float>(a4 * static_cast<float>(key_at(base, a3) - lo));
}

// v12 + a5 * (v13 - v12), v12 = v[a3] - v[a2], v13 = v[a4] - v[a2]
inline float relative3(std::uint32_t base, int a2, int a3, int a4, float a5) {
    const float v11 = key_at(base, a2);
    const float v12 = key_at(base, a3) - v11;
    const float v13 = key_at(base, a4) - v11;
    return v12 + static_cast<float>(a5 * static_cast<float>(v13 - v12));
}

// *(_DWORD*)(getOutput(a1, 0) + 4): base of the float key array.
inline std::uint32_t key_base(const SAnimationAccessor* a1) {
    return guest_load_u32(accessor_get_output(a1, 0) + 4u);
}

}  // namespace

std::uint32_t accessor_get_output(const SAnimationAccessor* a1, int key) {
    // keyframes = track->keyframes; index = keyframes[key].outputIndex; outputs + 8 * index + 4
    const std::uint32_t keyframes =
        guest_load_u32(a1->track + static_cast<std::uint32_t>(offsetof(SAnimationAccessorTrack, keyframes)));
    const std::uint32_t index =
        guest_load_u32(keyframes + kKeyframeStride * static_cast<std::uint32_t>(key) + kKeyframeOutputIndexOffset);
    return a1->outputs + 8u * index + 4u;
}

// 0x61d97c
float interp_f4_k3_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5) {
    const std::uint32_t v9 = key_base(a1);
    if (accessor_has_default_value(a1)) {
        const std::uint32_t def = accessor_get_default_value(a1);
        for (int i = 0; i != 3; ++i) {
            a5[i] = guest_load_u32(def + 4u * static_cast<std::uint32_t>(i));
        }
        const float result = blend2(v9, a2, a3, a4);
        a5[3] = word_of(result);
        return result;
    }
    const float result = blend2(v9, a2, a3, a4);
    a5[0] = word_of(result);
    return result;
}

// 0x61e560
float interp_f2_k0_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5) {
    const std::uint32_t v9 = key_base(a1);
    if (accessor_has_default_value(a1)) {
        a5[0] = word_of(blend2(v9, a2, a3, a4));
        // COERCE_FLOAT(getDefaultValue): the returned float carries the default pointer bits.
        const std::uint32_t def = accessor_get_default_value(a1);
        a5[1] = guest_load_u32(def + 4u);
        return float_of(def);
    }
    const float result = blend2(v9, a2, a3, a4);
    a5[0] = word_of(result);
    return result;
}

// 0x61e6ec
float interp_f2_k0_relative3(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint32_t* a6) {
    const std::uint32_t v10 = key_base(a1);
    const float result = relative3(v10, a2, a3, a4, a5);
    if (accessor_has_default_value(a1)) {
        const std::uint32_t def = accessor_get_default_value(a1);
        a6[0] = word_of(result);
        a6[1] = guest_load_u32(def + 4u);
    } else {
        a6[0] = word_of(result);
    }
    return result;
}

// 0x61e88c
float interp_f2_k1_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5) {
    const std::uint32_t v9 = key_base(a1);
    if (accessor_has_default_value(a1)) {
        a5[0] = guest_load_u32(accessor_get_default_value(a1));
        const float result = blend2(v9, a2, a3, a4);
        a5[1] = word_of(result);
        return result;
    }
    const float result = blend2(v9, a2, a3, a4);
    a5[0] = word_of(result);
    return result;
}

// 0x61ea18
float interp_f2_k1_relative3(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint32_t* a6) {
    const std::uint32_t v10 = key_base(a1);
    const float result = relative3(v10, a2, a3, a4, a5);
    if (accessor_has_default_value(a1)) {
        a6[0] = guest_load_u32(accessor_get_default_value(a1));
        a6[1] = word_of(result);
    } else {
        a6[0] = word_of(result);
    }
    return result;
}

// 0x61ebc0
float interp_f3_k0_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5) {
    const std::uint32_t v9 = key_base(a1);
    if (accessor_has_default_value(a1)) {
        a5[0] = word_of(blend2(v9, a2, a3, a4));
        a5[1] = guest_load_u32(accessor_get_default_value(a1) + 4u);
        // The original reassigns result from getDefaultValue, so the return carries the pointer bits.
        const std::uint32_t def = accessor_get_default_value(a1);
        a5[2] = guest_load_u32(def + 8u);
        return float_of(def);
    }
    const float result = blend2(v9, a2, a3, a4);
    a5[0] = word_of(result);
    return result;
}

// 0x61ed6c
float interp_f3_k0_relative3(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint32_t* a6) {
    const std::uint32_t v9 = key_base(a1);
    const float result = relative3(v9, a2, a3, a4, a5);
    if (accessor_has_default_value(a1)) {
        const std::uint32_t def = accessor_get_default_value(a1);
        a6[0] = word_of(result);
        a6[1] = guest_load_u32(def + 4u);
        a6[2] = guest_load_u32(def + 8u);
    } else {
        a6[0] = word_of(result);
    }
    return result;
}

// 0x61ef2c
float interp_f4_k0_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5) {
    const std::uint32_t v9 = key_base(a1);
    if (accessor_has_default_value(a1)) {
        a5[0] = word_of(blend2(v9, a2, a3, a4));
        for (std::uint32_t v11 = 1; v11 != 4; ++v11) {
            // The loop reassigns result each pass, so the return carries the pointer bits.
            const std::uint32_t def = accessor_get_default_value(a1);
            a5[v11] = guest_load_u32(def + 4u * v11);
        }
        return float_of(accessor_get_default_value(a1));
    }
    const float result = blend2(v9, a2, a3, a4);
    a5[0] = word_of(result);
    return result;
}

// 0x61f0dc
float interp_f4_k0_relative3(SAnimationAccessor* a1, int a2, int a3, int a4, float a5, std::uint32_t* a6) {
    const std::uint32_t v10 = key_base(a1);
    const float result = relative3(v10, a2, a3, a4, a5);
    if (accessor_has_default_value(a1)) {
        const std::uint32_t def = accessor_get_default_value(a1);
        a6[0] = word_of(result);
        a6[1] = guest_load_u32(def + 4u);
        a6[2] = guest_load_u32(def + 8u);
        a6[3] = guest_load_u32(def + 12u);
    } else {
        a6[0] = word_of(result);
    }
    return result;
}

// 0x61f2a4
float interp_f4_k1_blend(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t* a5) {
    const std::uint32_t v9 = key_base(a1);
    if (accessor_has_default_value(a1)) {
        a5[0] = guest_load_u32(accessor_get_default_value(a1));
        a5[1] = word_of(blend2(v9, a2, a3, a4));
        a5[2] = guest_load_u32(accessor_get_default_value(a1) + 8u);
        // result = COERCE_FLOAT(getDefaultValue) is the return; a5[3] takes its word +12.
        const std::uint32_t def = accessor_get_default_value(a1);
        a5[3] = guest_load_u32(def + 12u);
        return float_of(def);
    }
    const float result = blend2(v9, a2, a3, a4);
    a5[0] = word_of(result);
    return result;
}

// 0x61da4c
int apply_f4_k3(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6) {
    std::uint32_t v7[4] = {};  // zero-filled: unwritten words of the stack buffer are unspecified in the original
    interp_f4_k3_blend(a1, a2, a3, a4, v7);
    return set_parameter_cvt_vec4(a5, a6->parameterId, 0, v7);
}

// 0x61e620
int apply_f2_k0(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6) {
    std::uint32_t v7[2] = {};  // zero-filled: unwritten words of the stack buffer are unspecified in the original
    interp_f2_k0_blend(a1, a2, a3, a4, v7);
    return set_parameter_cvt_vec2(a5, a6->parameterId, 0, v7);
}

// 0x61e94c
int apply_f2_k1(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6) {
    std::uint32_t v7[2] = {};  // zero-filled: unwritten words of the stack buffer are unspecified in the original
    interp_f2_k1_blend(a1, a2, a3, a4, v7);
    return set_parameter_cvt_vec2(a5, a6->parameterId, 0, v7);
}

// 0x61ec94
int apply_f3_k0(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6) {
    std::uint32_t v7[3] = {};  // zero-filled: unwritten words of the stack buffer are unspecified in the original
    interp_f3_k0_blend(a1, a2, a3, a4, v7);
    return set_parameter_cvt_vec3(a5, a6->parameterId, 0, v7);
}

// 0x61effc
int apply_f4_k0(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6) {
    std::uint32_t v7[4] = {};  // zero-filled: unwritten words of the stack buffer are unspecified in the original
    interp_f4_k0_blend(a1, a2, a3, a4, v7);
    return set_parameter_cvt_vec4(a5, a6->parameterId, 0, v7);
}

// 0x61f38c
int apply_f4_k1(SAnimationAccessor* a1, int a2, int a3, float a4, std::uint32_t a5, const animation_track::CApplicatorInfo* a6) {
    std::uint32_t v7[4] = {};  // zero-filled: unwritten words of the stack buffer are unspecified in the original
    interp_f4_k1_blend(a1, a2, a3, a4, v7);
    return set_parameter_cvt_vec4(a5, a6->parameterId, 0, v7);
}

}  // namespace glitch::collada::recon_v2_06
