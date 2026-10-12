// Slice 14 reconstruction. Bodies follow the pseudocode of the rows in part_14.tsv.
// The applicator, accessor and target layouts are not mapped; see slice_14.hpp.
#include "glitch-scene-collada-gui/recon/slice_14.hpp"

namespace glitch::collada::recon_14 {

// 0x622254 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA2_fS6_EEEELin1EfEEfLi2ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float CInterpreter_f2mn1_getKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key,
                                               int /*unused*/, float t, float* out) {
    // Output keys are float[2] pairs, 8 bytes each; the key and the next key are read.
    const float* data = reinterpret_cast<const float*>(accessorOutputData(accessor, 0));
    const float* k0 = data + 2 * key;
    const float* k1 = k0 + 2;
    const float inv = 1.0f - t;
    const float x = static_cast<float>(static_cast<float>(inv * k0[0]) + 0.0f) + static_cast<float>(t * k1[0]);
    out[1] = static_cast<float>(inv * k0[1] + 0.0f) + static_cast<float>(t * k1[1]);
    out[0] = x;
    return x;
}

// 0x62210c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE13getAddedValueEPvPfiSD_
void CVirtualEx_f2_m1_getAddedValue_a5(const void* self, const float* values, const float* weights, int count, float* out) {
    weightedSumFloat2(values, weights, count, out);
}


// 0x6221b0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE13getAddedValueEPvPfiSD_
void CVirtualEx_f2_mn1_getAddedValue_a5(const void* self, const float* values, const float* weights, int count, float* out) {
    weightedSumFloat2(values, weights, count, out);
}


// 0x622308 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float CVirtualEx_f2_mn1_getKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, float* out) {
    return CInterpreter_f2mn1_getKeyBasedValueEx_a5(accessor, key, unused, t, out);
}


// 0x622324 _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_f2_mn1_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    float value[2] = {0.0f, 0.0f};
    CInterpreter_f2mn1_getKeyBasedValueEx_a5(accessor, key, unused, t, value);
    return setParameterCvt<glitch::core::vector2d<float>>(target, applicatorParameterIndex(info), 0, value);
}


// 0x622358 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_f2_mn1_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    return CApplyValueEx_f2_mn1_applyKeyBasedValueEx_a6(accessor, key, unused, t, target, info);
}


// 0x62237c _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int CApplyValueEx_f2_m0_applyAddedValueEx_a5(const float* values, const float* weights, int count, void* target, const SApplicatorInfo* info) {
    float value[2] = {0.0f, 0.0f};
    weightedSumFloat2(values, weights, count, value);
    return setParameterCvt<glitch::core::vector2d<float>>(target, applicatorParameterIndex(info), 0, value);
}


// 0x622440 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f2_m0_applyAddedValue_a6(const void* self, const float* values, const float* weights, int count, void* target, const SApplicatorInfo* info) {
    return CApplyValueEx_f2_m0_applyAddedValueEx_a5(values, weights, count, target, info);
}


// 0x62245c _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int CApplyValueEx_f2_m0_applyBlendedValueEx_a5(const float* values, const float* weights, int count, void* target, const SApplicatorInfo* info) {
    float value[2] = {0.0f, 0.0f};
    weightedSumFloat2(values, weights, count, value);
    return setParameterCvt<glitch::core::vector2d<float>>(target, applicatorParameterIndex(info), 0, value);
}


// 0x622520 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int CVirtualEx_f2_m0_applyBlendedValue_a6(const void* self, const float* values, const float* weights, int count, void* target, const SApplicatorInfo* info) {
    return CApplyValueEx_f2_m0_applyBlendedValueEx_a5(values, weights, count, target, info);
}


// 0x62253c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE10applyValueEPvSF_PNS1_15CApplicatorInfoE
int CVirtualEx_u4_m0_applyValue_a3(const void* self, const std::uint8_t* value, void* target) {
    return applyUChar4Value(value, target);
}


// 0x622584 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_u4_m0_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info) {
    return applyKeyBasedUChar4<UChar4Interp<0>>(accessor, key, target, info);
}


// 0x6225e0 _ZN6glitch7collada15animation_track13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELi0EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_u4_m0_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    return applyKeyBasedUChar4Ex<UChar4Interp<0>>(accessor, key, unused, t, target, info);
}


// 0x622634 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_u4_m0_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    return CApplyValueEx_u4_m0_applyKeyBasedValueEx_a6(accessor, key, unused, t, target, info);
}


// 0x622658 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE15applyAddedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int CVirtualEx_u4_m0_applyAddedValue_a6(const void* self, const std::uint8_t* values, const float* weights, int count, void* target, const SApplicatorInfo* info) {
    return applyAddedUChar4(values, weights, count, target, info);
}


// 0x622798 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE10applyValueEPvSF_PNS1_15CApplicatorInfoE
int CVirtualEx_u4_m1_applyValue_a3(const void* self, const std::uint8_t* value, void* target) {
    return applyUChar4Value(value, target);
}


// 0x6227e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_u4_m1_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info) {
    return applyKeyBasedUChar4<UChar4Interp<1>>(accessor, key, target, info);
}


// 0x62283c _ZN6glitch7collada15animation_track13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELi1EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_u4_m1_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    return applyKeyBasedUChar4Ex<UChar4Interp<1>>(accessor, key, unused, t, target, info);
}


// 0x622890 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_u4_m1_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    return CApplyValueEx_u4_m1_applyKeyBasedValueEx_a6(accessor, key, unused, t, target, info);
}


// 0x6228b4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE10applyValueEPvSF_PNS1_15CApplicatorInfoE
int CVirtualEx_u4_m2_applyValue_a3(const void* self, const std::uint8_t* value, void* target) {
    return applyUChar4Value(value, target);
}


// 0x6228fc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_u4_m2_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info) {
    return applyKeyBasedUChar4<UChar4Interp<2>>(accessor, key, target, info);
}


// 0x622958 _ZN6glitch7collada15animation_track13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELi2EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_u4_m2_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    return applyKeyBasedUChar4Ex<UChar4Interp<2>>(accessor, key, unused, t, target, info);
}


// 0x6229ac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_u4_m2_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    return CApplyValueEx_u4_m2_applyKeyBasedValueEx_a6(accessor, key, unused, t, target, info);
}


// 0x6229d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE10applyValueEPvSF_PNS1_15CApplicatorInfoE
int CVirtualEx_u4_m3_applyValue_a3(const void* self, const std::uint8_t* value, void* target) {
    return applyUChar4Value(value, target);
}


// 0x622a18 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_u4_m3_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info) {
    return applyKeyBasedUChar4<UChar4Interp<3>>(accessor, key, target, info);
}


// 0x622a74 _ZN6glitch7collada15animation_track13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELi3EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_u4_m3_applyKeyBasedValueEx_a6(const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    return applyKeyBasedUChar4Ex<UChar4Interp<3>>(accessor, key, unused, t, target, info);
}


// 0x622ac8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_u4_m3_applyKeyBasedValue_a7(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target, const SApplicatorInfo* info) {
    return CApplyValueEx_u4_m3_applyKeyBasedValueEx_a6(accessor, key, unused, t, target, info);
}


// 0x622aec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE10applyValueEPvSF_PNS1_15CApplicatorInfoE
int CVirtualEx_u4_mn1_applyValue_a3(const void* self, const std::uint8_t* value, void* target) {
    return applyUChar4Value(value, target);
}


// 0x622b34 _ZN6glitch7collada15animation_track13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELin1EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_u4_mn1_applyKeyBasedValueEx_a4(const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info) {
    return applyKeyBasedUChar4Direct(accessor, key, target, info);
}


// 0x622bb4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_u4_mn1_applyKeyBasedValue_a5(const void* self, const SAnimationAccessor* accessor, int key, void* target, const SApplicatorInfo* info) {
    return CApplyValueEx_u4_mn1_applyKeyBasedValueEx_a4(accessor, key, target, info);
}


// 0x622bc8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleZ_char_applyValue_a3(const void* self, const void* value, void* target) {
    return invokeVirtualSlot<148>(target);
}


// 0x622be0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleZ_short_applyValue_a3(const void* self, const void* value, void* target) {
    return invokeVirtualSlot<148>(target);
}


// 0x622c10 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleY_char_applyValue_a3(const void* self, const void* value, void* target) {
    return invokeVirtualSlot<148>(target);
}


// 0x622c28 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleY_short_applyValue_a3(const void* self, const void* value, void* target) {
    return invokeVirtualSlot<148>(target);
}


// 0x622c58 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleX_char_applyValue_a3(const void* self, const void* value, void* target) {
    return invokeVirtualSlot<148>(target);
}


// 0x622c70 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleX_short_applyValue_a3(const void* self, const void* value, void* target) {
    return invokeVirtualSlot<148>(target);
}


// 0x622c88 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionZ_char_applyValue_a3(const void* self, const void* value, void* target) {
    return invokeVirtualSlot<164>(target);
}


// 0x622ca0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionZ_short_applyValue_a3(const void* self, const void* value, void* target) {
    return invokeVirtualSlot<164>(target);
}


// 0x622cd0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionY_char_applyValue_a3(const void* self, const void* value, void* target) {
    return invokeVirtualSlot<164>(target);
}


// 0x622ce8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionMixin_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3Direct<164>(accessor, key, target);
}


// 0x622d58 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionMixin_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_PositionMixin_float_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x622d6c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionMixin_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CMixin<float, 3, SSceneNodePosition, -1, float>, float, 3, SUseDefaultLerp<short>>, 164>(accessor, key, target);
}


// 0x622db0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionMixin_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_PositionMixin_short_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x622dc4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionMixin_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CMixin<float, 3, SSceneNodePosition, -1, float>, float, 3, SUseDefaultLerp<char>>, 164>(accessor, key, target);
}


// 0x622e08 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionMixin_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_PositionMixin_char_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x622e1c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionX_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>, 164>(accessor, key, target);
}


// 0x622e60 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionX_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_PositionX_float_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x622e74 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionX_float_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodePositionXEx<float>, float, 3, SUseDefaultValues<0, float>>, 164>(accessor, key, unused, t, target);
}


// 0x622eb8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionX_float_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_PositionX_float_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x622edc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionX_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodePositionXEx<short>, float, 3, SUseDefaultValues<0, short>>, 164>(accessor, key, target);
}


// 0x622f20 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionX_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_PositionX_short_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x622f34 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionX_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodePositionXEx<short>, float, 3, SUseDefaultValues<0, short>>, 164>(accessor, key, unused, t, target);
}


// 0x622f78 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionX_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_PositionX_short_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x622f9c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionX_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodePositionXEx<char>, float, 3, SUseDefaultValues<0, char>>, 164>(accessor, key, target);
}


// 0x622fe0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionX_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_PositionX_char_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x622ff4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionX_char_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodePositionXEx<char>, float, 3, SUseDefaultValues<0, char>>, 164>(accessor, key, unused, t, target);
}


// 0x623038 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionX_char_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_PositionX_char_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x62305c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionY_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodePositionYEx<float>, float, 3, SUseDefaultValues<1, float>>, 164>(accessor, key, target);
}


// 0x6230a0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionY_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_PositionY_float_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x6230b4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionY_float_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodePositionYEx<float>, float, 3, SUseDefaultValues<1, float>>, 164>(accessor, key, unused, t, target);
}


// 0x6230f8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionY_float_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_PositionY_float_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x62311c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionY_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodePositionYEx<short>, float, 3, SUseDefaultValues<1, short>>, 164>(accessor, key, target);
}


// 0x623160 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionY_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_PositionY_short_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x623174 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_PositionY_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodePositionYEx<short>, float, 3, SUseDefaultValues<1, short>>, 164>(accessor, key, unused, t, target);
}


// 0x6231b8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_PositionY_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_PositionY_short_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x6231dc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleZ_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleZEx<short>, float, 3, SUseDefaultValues<2, short>>, 148>(accessor, key, target);
}


// 0x623220 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleZ_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_ScaleZ_short_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x623234 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleZ_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodeScaleZEx<short>, float, 3, SUseDefaultValues<2, short>>, 148>(accessor, key, unused, t, target);
}


// 0x623278 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleZ_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_ScaleZ_short_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x62329c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleZ_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleZEx<char>, float, 3, SUseDefaultValues<2, char>>, 148>(accessor, key, target);
}


// 0x6232e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleZ_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_ScaleZ_char_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x6232f4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleZ_char_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodeScaleZEx<char>, float, 3, SUseDefaultValues<2, char>>, 148>(accessor, key, unused, t, target);
}


// 0x623338 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleZ_char_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_ScaleZ_char_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x62335c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleMixin_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleMixin<char>, float, 3, SUseDefaultLerp<char>>, 148>(accessor, key, target);
}


// 0x6233a0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleMixin_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_ScaleMixin_char_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x6233b4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleX_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float>>, 148>(accessor, key, target);
}


// 0x6233f8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleX_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_ScaleX_float_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x62340c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleX_float_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodeScaleXEx<float>, float, 3, SUseDefaultValues<0, float>>, 148>(accessor, key, unused, t, target);
}


// 0x623450 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleX_float_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_ScaleX_float_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x623474 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleX_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleXEx<short>, float, 3, SUseDefaultValues<0, short>>, 148>(accessor, key, target);
}


// 0x6234b8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleX_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_ScaleX_short_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x6234cc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleX_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodeScaleXEx<short>, float, 3, SUseDefaultValues<0, short>>, 148>(accessor, key, unused, t, target);
}


// 0x623510 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleX_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_ScaleX_short_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x623534 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleX_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleXEx<char>, float, 3, SUseDefaultValues<0, char>>, 148>(accessor, key, target);
}


// 0x623578 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleX_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_ScaleX_char_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x62358c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleX_char_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodeScaleXEx<char>, float, 3, SUseDefaultValues<0, char>>, 148>(accessor, key, unused, t, target);
}


// 0x6235d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleX_char_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_ScaleX_char_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x6235f4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleY_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleYEx<float>, float, 3, SUseDefaultValues<1, float>>, 148>(accessor, key, target);
}


// 0x623638 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleY_float_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_ScaleY_float_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x62364c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleY_float_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodeScaleYEx<float>, float, 3, SUseDefaultValues<1, float>>, 148>(accessor, key, unused, t, target);
}


// 0x623690 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleY_float_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_ScaleY_float_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x6236b4 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleY_short_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleYEx<short>, float, 3, SUseDefaultValues<1, short>>, 148>(accessor, key, target);
}


// 0x6236f8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleY_short_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_ScaleY_short_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x62370c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleY_short_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodeScaleYEx<short>, float, 3, SUseDefaultValues<1, short>>, 148>(accessor, key, unused, t, target);
}


// 0x623750 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleY_short_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_ScaleY_short_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x623774 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleY_char_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleYEx<char>, float, 3, SUseDefaultValues<1, char>>, 148>(accessor, key, target);
}


// 0x6237b8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleY_char_applyKeyBasedValue_a4(const void* self, const SAnimationAccessor* accessor, int key, void* target) {
    return CApplyValueEx_v3_ScaleY_char_applyKeyBasedValueEx_a3(accessor, key, target);
}


// 0x6237cc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleY_char_applyKeyBasedValueEx_a5(const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return applyKeyBasedVec3Ex<CInterpreter<CSceneNodeScaleYEx<char>, float, 3, SUseDefaultValues<1, char>>, 148>(accessor, key, unused, t, target);
}


// 0x623810 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int CVirtualEx_v3_ScaleY_char_applyKeyBasedValue_a6(const void* self, const SAnimationAccessor* accessor, int key, int unused, float t, void* target) {
    return CApplyValueEx_v3_ScaleY_char_applyKeyBasedValueEx_a5(accessor, key, unused, t, target);
}


// 0x623834 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int CApplyValueEx_v3_ScaleZ_float_applyKeyBasedValueEx_a3(const SAnimationAccessor* accessor, int key, void* target) {
    return applyKeyBasedVec3<CInterpreter<CSceneNodeScaleZEx<float>, float, 3, SUseDefaultValues<2, float>>, 148>(accessor, key, target);
}

}  // namespace glitch::collada::recon_14
