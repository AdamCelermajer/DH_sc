// Slice 15 reconstruction: see include/glitch-scene-collada-gui/recon/slice_15.hpp.
#include "glitch-scene-collada-gui/recon/slice_15.hpp"

namespace glitch::collada::recon_15 {

// 0x623878 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623878(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)accessor;
    (void)key;
    (void)value;
    (void)applicator;
    return 0;
}

// 0x62388c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_62388c(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable148(applicator, out);
}

// 0x6238d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_6238d0(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyKeyBasedValueEx_62388c(accessor, key, index, t, value, applicator);
}

// 0x62390c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE10applyValueEPvSB_PNS1_15CApplicatorInfoE
int applyValue_62390c(void* value, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)out;
    return applicatorVtable148(applicator);
}

// 0x623924 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE10applyValueEPvSB_PNS1_15CApplicatorInfoE
int applyValue_623924(void* value, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)out;
    return applicatorVtable148(applicator);
}

// 0x623954 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_623954(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)accessor;
    (void)key;
    (void)value;
    (void)applicator;
    return 0;
}

// 0x6239c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_6239c4(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyKeyBasedValueEx_623954(accessor, key, value, applicator);
}

// 0x6239d8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_6239d8(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable148(applicator, out);
}

// 0x623a1c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623a1c(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)accessor;
    (void)key;
    (void)value;
    (void)applicator;
    return 0;
}

// 0x623a30 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_623a30(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable164(applicator, out);
}

// 0x623a74 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623a74(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)accessor;
    (void)key;
    (void)value;
    (void)applicator;
    return 0;
}

// 0x623a88 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_623a88(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable164(applicator, out);
}

// 0x623acc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623acc(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyKeyBasedValueEx_623a88(accessor, key, index, t, value, applicator);
}

// 0x623af0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_623af0(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable164(applicator, out);
}

// 0x623b34 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623b34(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)accessor;
    (void)key;
    (void)value;
    (void)applicator;
    return 0;
}

// 0x623b48 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_623b48(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable164(applicator, out);
}

// 0x623b8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623b8c(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyKeyBasedValueEx_623b48(accessor, key, index, t, value, applicator);
}

// 0x623bb0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_623bb0(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable164(applicator, out);
}

// 0x623bf4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623bf4(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)accessor;
    (void)key;
    (void)value;
    (void)applicator;
    return 0;
}

// 0x623c08 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_623c08(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable164(applicator, out);
}

// 0x623c4c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623c4c(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyKeyBasedValueEx_623c08(accessor, key, index, t, value, applicator);
}

// 0x623c70 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_623c70(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable164(applicator, out);
}

// 0x623cb4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623cb4(const glitch::collada::SAnimationAccessor& accessor, int key, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)accessor;
    (void)key;
    (void)value;
    (void)applicator;
    return 0;
}

// 0x623cc8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_623cc8(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable164(applicator, out);
}

// 0x623d0c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_623d0c(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyKeyBasedValueEx_623cc8(accessor, key, index, t, value, applicator);
}

// 0x623d30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int applyValue_623d30(void* value, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)out;
    return applicatorVtable164(applicator);
}

// 0x623d60 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int applyValue_623d60(void* value, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)out;
    return applicatorVtable164(applicator);
}

// 0x623d78 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
int applyValue_623d78(void* value, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)out;
    return applicatorVtable164(applicator);
}

// 0x623da8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE10applyValueEPvSB_PNS1_15CApplicatorInfoE
int applyValue_623da8(void* value, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)out;
    return applicatorVtable164(applicator);
}

// 0x623dc0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE10applyValueEPvSB_PNS1_15CApplicatorInfoE
int applyValue_623dc0(void* value, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)out;
    return applicatorVtable164(applicator);
}

// 0x623df0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int applyAddedValueEx_623df0(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return applicatorVtable164(applicator);
}

// 0x623ee8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyAddedValue_623ee8(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyAddedValueEx_623df0(value, weights, key, out, applicator);
}

// 0x623f04 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE15getBlendedValueEPvPfiSD_
float* getBlendedValue_623f04(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x623ff8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE15getBlendedValueEPvPfiSD_
float* getBlendedValue_623ff8(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x6240ec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyBlendedValue_6240ec(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6241e8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyAddedValue_6241e8(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6242e4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyBlendedValue_6242e4(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6243e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyAddedValue_6243e0(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6244dc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyBlendedValue_6244dc(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6245d8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyAddedValue_6245d8(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6246d4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE15getBlendedValueEPvPfiSD_
float* getBlendedValue_6246d4(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x6247c8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE15getBlendedValueEPvPfiSD_
float* getBlendedValue_6247c8(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x6248bc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE15getBlendedValueEPvPfiSD_
float* getBlendedValue_6248bc(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x6249b0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE13getAddedValueEPvPfiSD_
float* getAddedValue_6249b0(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x624aa4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE13getAddedValueEPvPfiSD_
float* getAddedValue_624aa4(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x624b98 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi2EfEEEEE13getAddedValueEPvPfiSD_
float* getAddedValue_624b98(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x624c8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi3EfEEEEE13getAddedValueEPvPfiSD_
float* getAddedValue_624c8c(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x624d80 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE13getAddedValueEPvPfiSD_
float* getAddedValue_624d80(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return nullptr;
}

// 0x624e74 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_fS6_EEEELin1EfEEfLi4ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
int getKeyBasedValueEx_624e74(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value) {
    (void)accessor;
    (void)key;
    (void)index;
    (void)t;
    (void)value;
    return 0;
}

// 0x624f40 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
int getKeyBasedValue_624f40(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value) {
    (void)accessor;
    (void)key;
    (void)index;
    (void)t;
    (void)value;
    return 0;
}

// 0x624f5c _ZN6glitch7collada15animation_track13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_624f5c(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)accessor;
    (void)key;
    (void)index;
    (void)t;
    (void)value;
    (void)applicator;
    return 0;
}

// 0x624f90 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_624f90(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyKeyBasedValueEx_624f5c(accessor, key, index, t, value, applicator);
}

// 0x624fb4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyAddedValue_624fb4(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6250b0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyBlendedValue_6250b0(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6251ac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_fNS1_6CMixinIfLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyAddedValue_6251ac(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6252a8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE17applyBlendedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int applyBlendedValue_6252a8(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6253e8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE15applyAddedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int applyAddedValue_6253e8(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x625528 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE17applyBlendedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int applyBlendedValue_625528(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x625668 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE15applyAddedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int applyAddedValue_625668(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6257a8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE17applyBlendedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int applyBlendedValue_6257a8(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x6258e8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE15applyAddedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int applyAddedValue_6258e8(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x625a28 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE17applyBlendedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int applyBlendedValue_625a28(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x625b68 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE15applyAddedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int applyAddedValue_625b68(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x625ca8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE17applyBlendedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int applyBlendedValue_625ca8(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x625de8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE15getBlendedValueEPvPfiSF_
unsigned int getBlendedValue_625de8(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x625efc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE15getBlendedValueEPvPfiSF_
unsigned int getBlendedValue_625efc(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x626010 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE15getBlendedValueEPvPfiSF_
unsigned int getBlendedValue_626010(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x626124 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE15getBlendedValueEPvPfiSF_
unsigned int getBlendedValue_626124(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x626238 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE15getBlendedValueEPvPfiSF_
unsigned int getBlendedValue_626238(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x62634c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi0EhEEEEE13getAddedValueEPvPfiSF_
unsigned int getAddedValue_62634c(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x626460 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi1EhEEEEE13getAddedValueEPvPfiSF_
unsigned int getAddedValue_626460(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x626574 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi2EhEEEEE13getAddedValueEPvPfiSF_
unsigned int getAddedValue_626574(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x626688 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE13getAddedValueEPvPfiSF_
unsigned int getAddedValue_626688(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x62679c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE13getAddedValueEPvPfiSF_
unsigned int getAddedValue_62679c(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    return 0;
}

// 0x6268b0 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELin1EhEEhLi4ENS1_15SUseDefaultLerpIhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
unsigned int getKeyBasedValueEx_6268b0(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value) {
    (void)accessor;
    (void)key;
    (void)index;
    (void)t;
    (void)value;
    return 0;
}

// 0x6269a4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
unsigned int getKeyBasedValue_6269a4(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value) {
    (void)accessor;
    (void)key;
    (void)index;
    (void)t;
    (void)value;
    return 0;
}

// 0x6269c0 _ZN6glitch7collada15animation_track13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELin1EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_6269c0(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)accessor;
    (void)key;
    (void)index;
    (void)t;
    (void)value;
    (void)applicator;
    return 0;
}

// 0x626a14 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_626a14(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyKeyBasedValueEx_6269c0(accessor, key, index, t, value, applicator);
}

// 0x626a38 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int applyBlendedValueEx_626a38(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x626b30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyBlendedValue_626b30(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyBlendedValueEx_626a38(value, weights, key, out, applicator);
}

// 0x626c30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE15getBlendedValueEPvPfiSB_
void getBlendedValue_626c30(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x626d14 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE15getBlendedValueEPvPfiSB_
void getBlendedValue_626d14(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x626edc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE15getBlendedValueEPvPfiSD_
void getBlendedValue_626edc(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x626fc0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE15getBlendedValueEPvPfiSD_
void getBlendedValue_626fc0(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x627188 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE15getBlendedValueEPvPfiSD_
void getBlendedValue_627188(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x62726c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE15getBlendedValueEPvPfiSD_
void getBlendedValue_62726c(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x627434 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE15getBlendedValueEPvPfiSD_
void getBlendedValue_627434(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x627518 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE15getBlendedValueEPvPfiSD_
void getBlendedValue_627518(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x6276e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE15getBlendedValueEPvPfiSB_
void getBlendedValue_6276e0(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x6277c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE15getBlendedValueEPvPfiSB_
void getBlendedValue_6277c4(void* value, float* weights, int key, void* out) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
}

// 0x62798c _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int applyBlendedValueEx_62798c(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x627a84 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyBlendedValue_627a84(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyBlendedValueEx_62798c(value, weights, key, out, applicator);
}

// 0x627aa0 _ZN6glitch7collada15animation_track13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int applyAddedValueEx_627aa0(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    (void)weights;
    (void)key;
    (void)out;
    (void)applicator;
    return 0;
}

// 0x627b98 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_fNS1_6CMixinIfLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int applyAddedValue_627b98(void* value, float* weights, int key, void* out, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyAddedValueEx_627aa0(value, weights, key, out, applicator);
}

// 0x627bb4 _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIsEEfLi3ENS1_15SUseDefaultLerpIsEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValueEx_627bb4(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value) {
    (void)accessor;
    (void)key;
    (void)index;
    (void)t;
    (void)value;
    return 0;
}

// 0x627d14 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_627d14(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable148(applicator, out);
}

// 0x627d58 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValue_627d58(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    return applyKeyBasedValueEx_627d14(accessor, key, index, t, value, applicator);
}

// 0x627d7c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValue_627d7c(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value) {
    (void)accessor;
    (void)key;
    (void)index;
    (void)t;
    (void)value;
    return 0;
}

// 0x627d98 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIcEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float getKeyBasedValueEx_627d98(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value) {
    (void)accessor;
    (void)key;
    (void)index;
    (void)t;
    (void)value;
    return 0;
}

// 0x627efc _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int applyKeyBasedValueEx_627efc(const glitch::collada::SAnimationAccessor& accessor, int key, int index, float t, void* value, glitch::collada::animation_track::CApplicatorInfo* applicator) {
    (void)value;
    std::uint32_t out[3] = {};  // memset 12 bytes
    interpreterGetKeyBasedValueEx(accessor, key, out);
    return applicatorVtable164(applicator, out);
}

}  // namespace glitch::collada::recon_15
