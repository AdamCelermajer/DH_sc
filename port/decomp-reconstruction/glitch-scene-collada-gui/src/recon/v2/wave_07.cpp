#include "glitch-scene-collada-gui/recon/v2/wave_07.hpp"

#include <cstring>

namespace glitch::collada::recon_v2_07 {

void wave_07_blend_vec3(const float* values, const float* weights, int count, float* out) {
    if (count == 1) {
        out[0] = values[0];
        out[1] = values[1];
        out[2] = values[2];
        return;
    }
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    for (int i = 0; i < count; ++i) {
        const float w = weights[i];
        x = x + w * values[3 * i + 0];
        y = y + w * values[3 * i + 1];
        z = z + w * values[3 * i + 2];
    }
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

float wave_07_lerp_float_pair(const float* base, int a2, float a4, float* out) {
    const float* key_a = base + 3 * a2;
    const float* key_b = key_a + 3;
    const float one_minus = 1.0f - a4;
    for (int c = 0; c < 3; ++c) {
        out[c] = (a4 * key_b[c]) + ((one_minus * key_a[c]) + 0.0f);
    }
    return out[0];
}

int wave_07_bind_force(void* self, std::uint32_t a2, std::uint32_t proxy_vtable, const wave_07_bind_ports& ports) {
    void* target = ports.base_of(self);
    wave_07_force_proxy* proxy = new wave_07_force_proxy{proxy_vtable, a2, 0u, a2};
    return ports.call(target, 0x58u, proxy);
}

int wave_07_bind_node(void* self, void* node, std::uint32_t proxy_vtable, const wave_07_node_ports& nodes,
                      const wave_07_bind_ports& ports) {
    void* particle_system = nodes.particle_system_of(node);
    return wave_07_bind_force(particle_system, nodes.force_address_of(self), proxy_vtable, ports);
}

// 0x626a38 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x626a38(const float* values, const float* weights, int count, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set) {
    float value[3];
    wave_07_blend_vec3(values, weights, count, value);
    return set(target, info->parameterId, value);
}

// 0x626c30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE15getBlendedValueEPvPfiSB_
void get_blended_0x626c30(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x626d14 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE15getBlendedValueEPvPfiSB_
void get_blended_0x626d14(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x626edc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x626edc(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x626fc0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x626fc0(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x627188 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x627188(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62726c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62726c(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x627434 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x627434(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x627518 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x627518(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x6276e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE15getBlendedValueEPvPfiSB_
void get_blended_0x6276e0(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x6277c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE15getBlendedValueEPvPfiSB_
void get_blended_0x6277c4(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62798c _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62798c(const float* values, const float* weights, int count, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set) {
    float value[3];
    wave_07_blend_vec3(values, weights, count, value);
    return set(target, info->parameterId, value);
}

// 0x627aa0 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x627aa0(const float* values, const float* weights, int count, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set) {
    float value[3];
    wave_07_blend_vec3(values, weights, count, value);
    return set(target, info->parameterId, value);
}

// 0x627bb4 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIsEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_0x627bb4(const wave_07_quantized<std::int16_t>& track, int a2, int a3, float a4, float* out) {
    return wave_07_lerp_quantized<std::int16_t>(track, a2, a3, a4, out);
}

// 0x627d14 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x627d14(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interp(a2, a3, a4, value);
    return call(receiver, 0x94U, value);
}

// 0x627d98 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_0x627d98(const wave_07_quantized<std::int8_t>& track, int a2, int a3, float a4, float* out) {
    return wave_07_lerp_quantized<std::int8_t>(track, a2, a3, a4, out);
}

// 0x627efc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x627efc(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interp(a2, a3, a4, value);
    return call(receiver, 0xA4U, value);
}

// 0x627f80 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIcEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_0x627f80(const wave_07_quantized<std::int8_t>& track, int a2, int a3, float a4, float* out) {
    return wave_07_lerp_quantized<std::int8_t>(track, a2, a3, a4, out);
}

// 0x6280e4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x6280e4(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interp(a2, a3, a4, value);
    return call(receiver, 0x94U, value);
}

// 0x628168 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x628168(const float* values, const float* weights, int count, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set) {
    float value[3];
    wave_07_blend_vec3(values, weights, count, value);
    return set(target, info->parameterId, value);
}

// 0x62827c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_blended_0x62827c(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x628390 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_added_0x628390(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x6285b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_added_0x6285b8(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x6287cc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x6287cc(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interp(a2, a3, a4, value);
    return call(receiver, 0xA4U, value);
}

// 0x628950 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x628950(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interp(a2, a3, a4, value);
    return call(receiver, 0x94U, value);
}

// 0x6289d4 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA3_fS6_EEEELin1EfEEfLi3ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_float_0x6289d4(const float* base, int a2, int a3, float a4, float* out) {
    (void)a3;
    return wave_07_lerp_float_pair(base, a2, a4, out);
}

// 0x628af0 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_material_0x628af0(const wave_07_key_interp& interp, int a2, int a3, float a4, void* target, const animation_track::CApplicatorInfo* info, const wave_07_param_set& set) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interp(a2, a3, a4, value);
    return set(target, info->parameterId, value);
}

// 0x628b48 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float get_key_0x628b48(const wave_07_quantized<std::int16_t>& track, int a2, int a3, float a4, float* out) {
    return wave_07_lerp_quantized<std::int16_t>(track, a2, a3, a4, out);
}

// 0x628ca8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int apply_key_0x628ca8(const wave_07_key_interp& interp, int a2, int a3, float a4, void* receiver, const wave_07_slot_call& call) {
    float value[3] = {0.0f, 0.0f, 0.0f};
    interp(a2, a3, a4, value);
    return call(receiver, 0xA4U, value);
}

// 0x628e40 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x628e40(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x628f54 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x628f54(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x629068 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x629068(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x62917c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62917c(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x629290 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x629290(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x6294b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x6294b8(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x6295cc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x6295cc(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x6296e0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x6296e0(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x6297f4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x6297f4(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x629908 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x629908(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x629b30 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x629b30(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x629c44 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_blended_0x629c44(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x629d58 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
int apply_added_0x629d58(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0xA4U, nullptr);
}

// 0x629e6c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x629e6c(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x629f50 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x629f50(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62a118 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a118(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62a1fc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a1fc(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62a3c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a3c4(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62a4a8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a4a8(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62a58c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a58c(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62a670 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE15getBlendedValueEPvPfiSD_
void get_blended_0x62a670(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62a754 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62a754(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62a838 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62a838(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62a91c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62a91c(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62aa00 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62aa00(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62aae4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62aae4(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62abc8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62abc8(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62acac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62acac(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62ad90 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62ad90(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62ae74 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62ae74(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62af58 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62af58(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b03c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b03c(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b120 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b120(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b204 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62b204(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b2e8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62b2e8(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b3cc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE13getAddedValueEPvPfiSB_
void get_added_0x62b3cc(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b4b0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b4b0(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b594 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b594(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b678 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b678(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b75c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b75c(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b840 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b840(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62b924 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62b924(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62ba08 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62ba08(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62baec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62baec(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62bbd0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62bbd0(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62bcb4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62bcb4(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62bd98 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE13getAddedValueEPvPfiSD_
void get_added_0x62bd98(const void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    wave_07_blend_vec3(values, weights, count, out);
}

// 0x62be7c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62be7c(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0x94U, nullptr);
}

// 0x62bf90 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x62bf90(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0x94U, nullptr);
}

// 0x62c0a4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62c0a4(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0x94U, nullptr);
}

// 0x62c1b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x62c1b8(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0x94U, nullptr);
}

// 0x62c3e0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x62c3e0(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0x94U, nullptr);
}

// 0x62c4f4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_blended_0x62c4f4(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0x94U, nullptr);
}

// 0x62c608 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int apply_added_0x62c608(const float* values, const float* weights, unsigned count, void* receiver, const wave_07_slot_call& call) {
    // The weighted sum over values and weights is computed and never stored in the original.
    (void)values;
    (void)weights;
    (void)count;
    return call(receiver, 0x94U, nullptr);
}

// 0x62ff08 _ZN6glitch7collada15CColladaFactory21getVertexBufferConfigERKNS0_16CColladaDatabaseEPNS0_9SGeometryE
wave_07_buffer_config* get_vertex_buffer_config_0x62ff08(const CColladaDatabase& db, const SGeometry* geom,
                                                           wave_07_buffer_config* result) {
    (void)db;
    (void)geom;
    result->byte05 = 0;
    result->word00 = 4u;
    result->byte04 = 0;
    return result;
}

// 0x62ff20 _ZN6glitch7collada15CColladaFactory20getIndexBufferConfigERKNS0_16CColladaDatabaseEPNS0_9SGeometryE
wave_07_buffer_config* get_index_buffer_config_0x62ff20(const CColladaDatabase& db, const SGeometry* geom,
                                                          wave_07_buffer_config* result) {
    (void)db;
    (void)geom;
    result->byte05 = 0;
    result->word00 = 4u;
    result->byte04 = 0;
    return result;
}

// 0x62ff80 _ZN6glitch7collada12_GLOBAL__N_132getExternalLightSceneNodeDefaultEPKc
void* get_external_light_0x62ff80(const char* name, const wave_07_resource_lookup& lookup, const wave_07_tag_of& tag_of) {
    const char* hash = std::strchr(name, '#');
    const char* key = hash ? hash + 1 : name;
    void* object = lookup(key, 0u);
    if (object != nullptr && tag_of(object) == kWave07LightTag) return object;
    return nullptr;
}

// 0x630018 _ZN6glitch2ps15IParticleSystemINS0_12GNPSParticleEE9bindForceINS0_5PWindEEEiRNT_15parameters_typeE
int bind_force_0x630018(void* self, std::uint32_t a2, const wave_07_bind_ports& ports) {
    return wave_07_bind_force(self, a2, 0x97B378U, ports);
}

// 0x630080 _ZN6glitch7collada15particle_system19CWindForceSceneNode4bindEPNS0_33CGlitchNewParticleSystemSceneNodeE
int bind_node_0x630080(void* self, void* node, const wave_07_node_ports& nodes, const wave_07_bind_ports& ports) {
    return wave_07_bind_node(self, node, 0x97B378U, nodes, ports);
}

// 0x630090 _ZN6glitch2ps15IParticleSystemINS0_9SParticleEE9bindForceINS0_5PWindEEEiRNT_15parameters_typeE
int bind_force_0x630090(void* self, std::uint32_t a2, const wave_07_bind_ports& ports) {
    return wave_07_bind_force(self, a2, 0x97B390U, ports);
}

// 0x6300f8 _ZN6glitch7collada15particle_system19CWindForceSceneNode4bindEPNS0_24CParticleSystemSceneNodeE
int bind_node_0x6300f8(void* self, void* node, const wave_07_node_ports& nodes, const wave_07_bind_ports& ports) {
    return wave_07_bind_node(self, node, 0x97B390U, nodes, ports);
}

// 0x630108 _ZN6glitch2ps15IParticleSystemINS0_12GNPSParticleEE9bindForceINS0_8PGravityEEEiRNT_15parameters_typeE
int bind_force_0x630108(void* self, std::uint32_t a2, const wave_07_bind_ports& ports) {
    return wave_07_bind_force(self, a2, 0x97B7B0U, ports);
}

// 0x630170 _ZN6glitch7collada15particle_system22CGravityForceSceneNode4bindEPNS0_33CGlitchNewParticleSystemSceneNodeE
int bind_node_0x630170(void* self, void* node, const wave_07_node_ports& nodes, const wave_07_bind_ports& ports) {
    return wave_07_bind_node(self, node, 0x97B7B0U, nodes, ports);
}

// 0x630180 _ZN6glitch2ps15IParticleSystemINS0_9SParticleEE9bindForceINS0_8PGravityEEEiRNT_15parameters_typeE
int bind_force_0x630180(void* self, std::uint32_t a2, const wave_07_bind_ports& ports) {
    return wave_07_bind_force(self, a2, 0x97B7C8U, ports);
}

// 0x6301e8 _ZN6glitch7collada15particle_system22CGravityForceSceneNode4bindEPNS0_24CParticleSystemSceneNodeE
int bind_node_0x6301e8(void* self, void* node, const wave_07_node_ports& nodes, const wave_07_bind_ports& ports) {
    return wave_07_bind_node(self, node, 0x97B7C8U, nodes, ports);
}

}  // namespace glitch::collada::recon_v2_07
