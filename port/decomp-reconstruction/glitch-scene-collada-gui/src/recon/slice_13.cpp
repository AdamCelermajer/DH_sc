// Slice 13 reconstruction (glitch::collada::animation_track value wrappers). See the header for
// the stand-in types and the unattached externals.
#include "glitch-scene-collada-gui/recon/slice_13.hpp"

namespace glitch::collada::recon_13 {

namespace {

std::uint16_t parameterIdOf(const void* binding) {
    return static_cast<const SParameterBinding*>(binding)->parameterId;  // +0x08
}

// Colour bytes packed for SColor: the three blended bytes and an opaque alpha of 0xff.
void packColorWithAlpha(const std::uint8_t (&rgb)[3], std::uint8_t (&color)[4]) {
    color[0] = rgb[0];
    color[1] = rgb[1];
    color[2] = rgb[2];
    color[3] = 0xff;
}

// Blends three byte components over `count` keys. One key is copied unweighted.
// Returns the truncated third component as an unsigned integer, matching the original.
std::uintptr_t blendColorBytes(const std::uint8_t* values, const float* weights, int count, std::uint8_t* out) {
    if (count == 1) {
        out[0] = values[0];
        out[1] = values[1];
        out[2] = values[2];
        return reinterpret_cast<std::uintptr_t>(out);
    }
    float sum[3];
    accumulateWeighted<3>(values, weights, count, sum);
    out[0] = truncateToByte(sum[0]);
    out[1] = truncateToByte(sum[1]);
    out[2] = truncateToByte(sum[2]);
    return static_cast<std::uintptr_t>(static_cast<std::uint32_t>(sum[2]));
}

// Blended colour for the SColor parameter path, packed with alpha.
void blendColorForParameter(const std::uint8_t* values, const float* weights, int count, std::uint8_t (&color)[4]) {
    std::uint8_t rgb[3];
    blendColorBytes(values, weights, count, rgb);
    packColorWithAlpha(rgb, color);
}

// Blended light colour (vector3d with CLightColorMixin<unsigned char>), zero alpha, for the
// applicator path.
void blendLightColor(const std::uint8_t* values, const float* weights, int count, std::uint8_t (&value)[4]) {
    std::uint8_t rgb[3];
    blendColorBytes(values, weights, count, rgb);
    value[0] = rgb[0];
    value[1] = rgb[1];
    value[2] = rgb[2];
    value[3] = 0;
}

}  // namespace

// ---- Shared helpers ----

int callValueSlot(void* target, const void* value) {
    return static_cast<IAnimationTarget*>(target)->applyValue(value);  // vtable +0x9c
}

void setIdentityQuaternion(float (&quaternion)[4]) {
    quaternion[0] = 0.0f;
    quaternion[1] = 0.0f;
    quaternion[2] = 0.0f;
    quaternion[3] = 1.0f;  // +0x0c, unit w
}

const std::uint32_t* retrieveVector3Words(const std::uint32_t* source, std::uint32_t* out) {
    out[0] = source[0];
    out[1] = source[1];
    out[2] = source[2];
    return source;
}

// ---- Float track (CMixin<float, 1>) ----

// 0x61fe30 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiPv
void* posYFloat_getKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, int a4, void* a5) {
    (void)self;
    return interpPosYFloat3Ptr(accessor, a3, a4, a5);
}

// 0x61ff18 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiiifPv
float posYFloat_getKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, int a5, float a6, void* a7) {
    (void)self;
    return interpPosYFloat3(accessor, a3, a4, a5, a6, a7);
}

// 0x61ffac _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
void* posZFloat_getKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* a4) {
    (void)self;
    return interpPosZFloat3Ptr(accessor, a3, a4);
}

// 0x620070 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float posZFloat_getKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float a5, void* a6) {
    (void)self;
    return interpPosZFloat3(accessor, a3, a4, a5, a6);
}

// 0x6201a0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE13getAddedValueEPvPfiSC_
void float1_getAddedValue(void* self, const void* values, const float* weights, int count, float* out) {
    (void)self;
    const auto* value = static_cast<const float*>(values);
    if (count == 1) {
        *out = value[0];
        return;
    }
    float sum = 0.0f;
    for (int key = 0; key < count; ++key) {
        sum += value[key] * weights[key];
    }
    *out = sum;
}

// 0x62020c _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEfLi1ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
float float1_interp_getKeyBasedValueEx(SAnimationAccessor* accessor, int key, int a3, float t, float* out) {
    (void)a3;
    const auto* values = static_cast<const float*>(getOutput(accessor, 0)->values);
    float result = (values[key] * (1.0f - t) + 0.0f) + t * values[key + 1];
    *out = result;
    return result;
}

// 0x620284 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
float float1_getKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, int a4, float t, float* out) {
    (void)self;
    return float1_interp_getKeyBasedValueEx(accessor, key, a4, t, out);
}

// 0x6202a0 _ZN6glitch7collada15animation_track13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int float1_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, int a3, float t, void* parameters, void* binding) {
    float value = 0.0f;
    float1_interp_getKeyBasedValueEx(accessor, key, a3, t, &value);
    return setParameterCvtFloat(parameters, parameterIdOf(binding), 0, &value);
}

// 0x6202d4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int float1_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, int a4, float t, void* parameters, void* binding) {
    (void)self;
    return float1_applyKeyBasedValueEx(accessor, key, a4, t, parameters, binding);
}

// 0x62035c..0x62076c: position and scale component retrieveValue. The three words are copied
// from the transform source. Position members read the position words; scale members read the
// scale words.

// 0x62035c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIcEELi2EcEEEEE13retrieveValueEPvSD_
const std::uint32_t* posZChar_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->positionWords(), static_cast<std::uint32_t*>(out));
}

// 0x620390 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIsEELi2EsEEEEE13retrieveValueEPvSD_
const std::uint32_t* posZShort_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->positionWords(), static_cast<std::uint32_t*>(out));
}

// 0x6203f8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIcEELi1EcEEEEE13retrieveValueEPvSD_
const std::uint32_t* posYChar_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->positionWords(), static_cast<std::uint32_t*>(out));
}

// 0x62042c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIsEELi1EsEEEEE13retrieveValueEPvSD_
const std::uint32_t* posYShort_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->positionWords(), static_cast<std::uint32_t*>(out));
}

// 0x620494 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIcEELi0EcEEEEE13retrieveValueEPvSD_
const std::uint32_t* posXChar_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->positionWords(), static_cast<std::uint32_t*>(out));
}

// 0x6204c8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIsEELi0EsEEEEE13retrieveValueEPvSD_
const std::uint32_t* posXShort_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->positionWords(), static_cast<std::uint32_t*>(out));
}

// 0x620530 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIcEEEEE13retrieveValueEPvSB_
const std::uint32_t* posMixinChar_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->positionWords(), static_cast<std::uint32_t*>(out));
}

// 0x620564 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIsEEEEE13retrieveValueEPvSB_
const std::uint32_t* posMixinShort_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->positionWords(), static_cast<std::uint32_t*>(out));
}

// 0x620598 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIcEELi2EcEEEEE13retrieveValueEPvSD_
const std::uint32_t* scaleZChar_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->scaleWords(), static_cast<std::uint32_t*>(out));
}

// 0x6205cc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIsEELi2EsEEEEE13retrieveValueEPvSD_
const std::uint32_t* scaleZShort_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->scaleWords(), static_cast<std::uint32_t*>(out));
}

// 0x620634 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIcEELi1EcEEEEE13retrieveValueEPvSD_
const std::uint32_t* scaleYChar_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->scaleWords(), static_cast<std::uint32_t*>(out));
}

// 0x620668 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIsEELi1EsEEEEE13retrieveValueEPvSD_
const std::uint32_t* scaleYShort_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->scaleWords(), static_cast<std::uint32_t*>(out));
}

// 0x6206d0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIcEELi0EcEEEEE13retrieveValueEPvSD_
const std::uint32_t* scaleXChar_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->scaleWords(), static_cast<std::uint32_t*>(out));
}

// 0x620704 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIsEELi0EsEEEEE13retrieveValueEPvSD_
const std::uint32_t* scaleXShort_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->scaleWords(), static_cast<std::uint32_t*>(out));
}

// 0x62076c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIcEEEEE13retrieveValueEPvSB_
const std::uint32_t* scaleMixinChar_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->scaleWords(), static_cast<std::uint32_t*>(out));
}

// 0x6207a0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIsEEEEE13retrieveValueEPvSB_
const std::uint32_t* scaleMixinShort_retrieveValue(void* self, void* node, void* out) {
    (void)self;
    return retrieveVector3Words(static_cast<ISceneNodeTransform*>(node)->scaleWords(), static_cast<std::uint32_t*>(out));
}

// ---- Quaternion applyValue (no value argument, see notes) ----

// 0x6207d4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE10applyValueEPvSA_PNS1_15CApplicatorInfoE
int quatAngleChar_applyValue(void* self, void* a2, void* target) {
    (void)self;
    (void)a2;
    return callValueSlot(target, nullptr);
}

// 0x6207ec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE10applyValueEPvSA_PNS1_15CApplicatorInfoE
int quatAngleShort_applyValue(void* self, void* a2, void* target) {
    (void)self;
    (void)a2;
    return callValueSlot(target, nullptr);
}

// 0x620804 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE10applyValueEPvSA_PNS1_15CApplicatorInfoE
int quatAngleFloat_applyValue(void* self, void* a2, void* target) {
    (void)self;
    (void)a2;
    return callValueSlot(target, nullptr);
}

// 0x62081c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE10applyValueEPvSA_PNS1_15CApplicatorInfoE
int quatChar_applyValue(void* self, void* a2, void* target) {
    (void)self;
    (void)a2;
    return callValueSlot(target, nullptr);
}

// 0x620834 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE10applyValueEPvSA_PNS1_15CApplicatorInfoE
int quatShort_applyValue(void* self, void* a2, void* target) {
    (void)self;
    (void)a2;
    return callValueSlot(target, nullptr);
}

// ---- Quaternion float track ----

// 0x620864 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatFloat_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target) {
    const auto* words = static_cast<const std::uint32_t*>(getOutput(accessor, 0)->values);
    std::uint32_t value[4];
    for (int i = 0; i < 4; ++i) {
        value[i] = words[4 * key + i];  // one 16-byte key record
    }
    return callValueSlot(target, value);
}

// 0x6208e4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatFloat_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, void* target) {
    (void)self;
    return quatFloat_applyKeyBasedValueEx(accessor, key, target);
}

// 0x6208f8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatFloat_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatMixinFloat4(accessor, a2, a3, t, value);
    return callValueSlot(target, value);
}

// 0x620944 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatFloat_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target) {
    (void)self;
    return quatFloat_applyKeyBasedValueExT(accessor, a3, a4, t, target);
}

// 0x620bd8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEE17applyAddedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatFloat_applyAddedValueEx(const void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetAddedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620c24 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE15applyAddedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatFloat_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatFloat_applyAddedValueEx(values, weights, count, target);
}

// ---- Quaternion short track ----

// 0x6209d0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEE19applyBlendedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatShort_applyBlendedValueEx(void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetBlendedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620a1c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE17applyBlendedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatShort_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatShort_applyBlendedValueEx(values, weights, count, target);
}

// 0x620c40 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEE17applyAddedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatShort_applyAddedValueEx(const void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetAddedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620c8c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE15applyAddedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatShort_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatShort_applyAddedValueEx(values, weights, count, target);
}

// 0x620e48 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatShort_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatMixinShort4(accessor, key, value);
    return callValueSlot(target, value);
}

// 0x620e94 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatShort_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target) {
    (void)self;
    return quatShort_applyKeyBasedValueEx(accessor, a3, target);
}

// 0x620ea8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatShort_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatMixinShort4T(accessor, a2, a3, t, value);
    return callValueSlot(target, value);
}

// 0x620ef4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatShort_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target) {
    (void)self;
    return quatShort_applyKeyBasedValueExT(accessor, a3, a4, t, target);
}

// ---- Quaternion char track ----

// 0x620a38 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEE19applyBlendedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatChar_applyBlendedValueEx(void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetBlendedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620a84 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE17applyBlendedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatChar_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatChar_applyBlendedValueEx(values, weights, count, target);
}

// 0x620ca8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEE17applyAddedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatChar_applyAddedValueEx(const void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetAddedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620cf4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE15applyAddedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatChar_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatChar_applyAddedValueEx(values, weights, count, target);
}

// 0x620f18 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatChar_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatMixinChar4(accessor, key, value);
    return callValueSlot(target, value);
}

// 0x620f64 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatChar_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target) {
    (void)self;
    return quatChar_applyKeyBasedValueEx(accessor, a3, target);
}

// 0x620f78 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatChar_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatMixinChar4T(accessor, a2, a3, t, value);
    return callValueSlot(target, value);
}

// 0x620fc4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatChar_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target) {
    (void)self;
    return quatChar_applyKeyBasedValueExT(accessor, a3, a4, t, target);
}

// ---- Quaternion angle float track ----

// 0x620fe8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatAngleFloat_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatAngleFloat(accessor, key, value);
    return callValueSlot(target, value);
}

// 0x621034 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatAngleFloat_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target) {
    (void)self;
    return quatAngleFloat_applyKeyBasedValueEx(accessor, a3, target);
}

// 0x621048 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatAngleFloat_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatAngleFloatT(accessor, a2, a3, t, value);
    return callValueSlot(target, value);
}

// 0x621094 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatAngleFloat_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target) {
    (void)self;
    return quatAngleFloat_applyKeyBasedValueExT(accessor, a3, a4, t, target);
}

// 0x620aa0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEE19applyBlendedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatAngleFloat_applyBlendedValueEx(void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetBlendedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620aec _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE17applyBlendedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatAngleFloat_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatAngleFloat_applyBlendedValueEx(values, weights, count, target);
}

// 0x620d10 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEE17applyAddedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatAngleFloat_applyAddedValueEx(const void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetAddedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620d5c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE15applyAddedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatAngleFloat_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatAngleFloat_applyAddedValueEx(values, weights, count, target);
}

// ---- Quaternion angle short track ----

// 0x620b08 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEE19applyBlendedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatAngleShort_applyBlendedValueEx(void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetBlendedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620b54 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE17applyBlendedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatAngleShort_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatAngleShort_applyBlendedValueEx(values, weights, count, target);
}

// 0x620d78 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEE17applyAddedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatAngleShort_applyAddedValueEx(const void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetAddedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620dc4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE15applyAddedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatAngleShort_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatAngleShort_applyAddedValueEx(values, weights, count, target);
}

// 0x6210b8 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatAngleShort_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatAngleShort(accessor, key, value);
    return callValueSlot(target, value);
}

// 0x621104 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatAngleShort_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target) {
    (void)self;
    return quatAngleShort_applyKeyBasedValueEx(accessor, a3, target);
}

// 0x621118 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatAngleShort_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatAngleShortT(accessor, a2, a3, t, value);
    return callValueSlot(target, value);
}

// 0x621164 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIsEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int quatAngleShort_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target) {
    (void)self;
    return quatAngleShort_applyKeyBasedValueExT(accessor, a3, a4, t, target);
}

// ---- Quaternion angle char track ----

// 0x620b70 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEE19applyBlendedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatAngleChar_applyBlendedValueEx(void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetBlendedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620bbc _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE17applyBlendedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatAngleChar_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatAngleChar_applyBlendedValueEx(values, weights, count, target);
}

// 0x620de0 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEE17applyAddedValueExEPvPfiS8_PNS1_15CApplicatorInfoE
int quatAngleChar_applyAddedValueEx(const void* values, const void* weights, int count, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    blenderQuatGetAddedValueEx(values, weights, count, value);
    return callValueSlot(target, value);
}

// 0x620e2c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE15applyAddedValueEPvPfiSA_PNS1_15CApplicatorInfoE
int quatAngleChar_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target) {
    (void)self;
    return quatAngleChar_applyAddedValueEx(values, weights, count, target);
}

// 0x621188 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatAngleChar_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target) {
    float value[4];
    setIdentityQuaternion(value);
    interpQuatAngleChar(accessor, key, value);
    return callValueSlot(target, value);
}

// 0x6211d4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIcEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
int quatAngleChar_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target) {
    (void)self;
    return quatAngleChar_applyKeyBasedValueEx(accessor, a3, target);
}

// ---- Light colour (vector3d with CLightColorMixin<unsigned char>) ----

// 0x6211e8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE15getBlendedValueEPvPfiSB_
std::uintptr_t lightColorGetBlendedValue(void* self, const std::uint8_t* values, const float* weights, int count, std::uint8_t* out) {
    (void)self;
    return blendColorBytes(values, weights, count, out);
}

// 0x6213c8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE13getAddedValueEPvPfiSB_
std::uintptr_t lightColorGetAddedValue(void* self, const std::uint8_t* values, const float* weights, int count, std::uint8_t* out) {
    (void)self;
    return blendColorBytes(values, weights, count, out);
}

// 0x621958 _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEE19applyBlendedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
float lightColorApplyBlendedValueEx(const std::uint8_t* values, const float* weights, int count, int applicator) {
    std::uint8_t value[4];
    blendLightColor(values, weights, count, value);
    return applicatorLightColorApplyValueEx(applicator, value);
}

// 0x621a60 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE17applyBlendedValueEPvPfiSB_PNS1_15CApplicatorInfoE
float lightColorApplyBlendedValue(void* self, const std::uint8_t* values, const float* weights, int count, int applicator) {
    (void)self;
    return lightColorApplyBlendedValueEx(values, weights, count, applicator);
}

// 0x621a7c _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEE17applyAddedValueExEPvPfiS9_PNS1_15CApplicatorInfoE
float lightColorApplyAddedValueEx(const std::uint8_t* values, const float* weights, int count, int applicator) {
    std::uint8_t value[4];
    blendLightColor(values, weights, count, value);
    return applicatorLightColorApplyValueEx(applicator, value);
}

// 0x621b84 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_16CLightColorMixinIhEEEEE15applyAddedValueEPvPfiSB_PNS1_15CApplicatorInfoE
float lightColorApplyAddedValue(void* self, const std::uint8_t* values, const float* weights, int count, int applicator) {
    (void)self;
    return lightColorApplyAddedValueEx(values, weights, count, applicator);
}

// ---- Colour (unsigned char [3] with SColor parameter) ----

// 0x6212d8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE15getBlendedValueEPvPfiSF_
std::uintptr_t colorGetBlendedValue(void* self, const std::uint8_t* values, const float* weights, int count, std::uint8_t* out) {
    (void)self;
    return blendColorBytes(values, weights, count, out);
}

// 0x6214b8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE17applyBlendedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int colorApplyBlendedValue(void* self, const std::uint8_t* values, const float* weights, int count, void* parameters, void* binding) {
    (void)self;
    std::uint8_t color[4];
    blendColorForParameter(values, weights, count, color);
    return setParameterCvtColor(parameters, parameterIdOf(binding), 0, color);
}

// 0x6215d8 _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA3_hNS_5video6SColorEEEEELin1EhEEhLi3ENS1_15SUseDefaultLerpIhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
std::uintptr_t colorInterpGetKeyBasedValueEx(SAnimationAccessor* accessor, int key, int a3, float t, std::uint8_t* out) {
    (void)a3;
    const auto* values = static_cast<const std::uint8_t*>(getOutput(accessor, 0)->values);
    const float weights[2] = {1.0f - t, t};
    return blendColorBytes(values + static_cast<std::size_t>(key) * 3, weights, 2, out);
}

// 0x6216b4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
std::uintptr_t colorGetKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, int a4, float t, std::uint8_t* out) {
    (void)self;
    return colorInterpGetKeyBasedValueEx(accessor, key, a4, t, out);
}

// 0x6216d0 _ZN6glitch7collada15animation_track13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELin1EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int colorApplyKeyBasedValueEx(SAnimationAccessor* accessor, int key, int a3, float t, void* parameters, void* binding) {
    std::uint8_t rgb[3];
    colorInterpGetKeyBasedValueEx(accessor, key, a3, t, rgb);
    std::uint8_t color[4];
    packColorWithAlpha(rgb, color);
    return setParameterCvtColor(parameters, parameterIdOf(binding), 0, color);
}

// 0x621724 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
int colorApplyKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, int a4, float t, void* parameters, void* binding) {
    (void)self;
    return colorApplyKeyBasedValueEx(accessor, key, a4, t, parameters, binding);
}

// 0x621748 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE15applyAddedValueEPvPfiSF_PNS1_15CApplicatorInfoE
int colorApplyAddedValue(void* self, const std::uint8_t* values, const float* weights, int count, void* parameters, void* binding) {
    (void)self;
    std::uint8_t color[4];
    blendColorForParameter(values, weights, count, color);
    return setParameterCvtColor(parameters, parameterIdOf(binding), 0, color);
}

// 0x621868 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA3_hNS1_6CMixinIhLi3ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELin1EhEEEEE13getAddedValueEPvPfiSF_
std::uintptr_t colorGetAddedValue(void* self, const std::uint8_t* values, const float* weights, int count, std::uint8_t* out) {
    (void)self;
    return blendColorBytes(values, weights, count, out);
}

// ---- Two-component float track (float [2]) ----

// 0x621ba0 _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi1EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int vec2UnitApplyAddedValueEx(const float* values, const void* weights, int count, void* parameters, void* binding) {
    float sum[2];
    accumulateWeighted<2>(values, static_cast<const float*>(weights), count, sum);
    return setParameterCvtVec2(parameters, parameterIdOf(binding), 0, sum);
}

// 0x621c64 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int vec2UnitApplyAddedValue(void* self, const float* values, const void* weights, int count, void* parameters, void* binding) {
    (void)self;
    return vec2UnitApplyAddedValueEx(values, weights, count, parameters, binding);
}

// 0x621c80 _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELi1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int vec2UnitApplyBlendedValueEx(const float* values, const void* weights, int count, void* parameters, void* binding) {
    float sum[2];
    accumulateWeighted<2>(values, static_cast<const float*>(weights), count, sum);
    return setParameterCvtVec2(parameters, parameterIdOf(binding), 0, sum);
}

// 0x621d44 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int vec2UnitApplyBlendedValue(void* self, const float* values, const void* weights, int count, void* parameters, void* binding) {
    (void)self;
    return vec2UnitApplyBlendedValueEx(values, weights, count, parameters, binding);
}

// 0x621d60 _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE17applyAddedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int vec2ApplyAddedValueEx(const float* values, const void* weights, int count, void* parameters, void* binding) {
    float sum[2];
    accumulateWeighted<2>(values, static_cast<const float*>(weights), count, sum);
    return setParameterCvtVec2(parameters, parameterIdOf(binding), 0, sum);
}

// 0x621e24 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE15applyAddedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int vec2ApplyAddedValue(void* self, const float* values, const void* weights, int count, void* parameters, void* binding) {
    (void)self;
    return vec2ApplyAddedValueEx(values, weights, count, parameters, binding);
}

// 0x621e40 _ZN6glitch7collada15animation_track13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_S3_EEEELin1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
int vec2ApplyBlendedValueEx(const float* values, const void* weights, int count, void* parameters, void* binding) {
    float sum[2];
    accumulateWeighted<2>(values, static_cast<const float*>(weights), count, sum);
    return setParameterCvtVec2(parameters, parameterIdOf(binding), 0, sum);
}

// 0x621f04 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
int vec2ApplyBlendedValue(void* self, const float* values, const void* weights, int count, void* parameters, void* binding) {
    (void)self;
    return vec2ApplyBlendedValueEx(values, weights, count, parameters, binding);
}

// 0x621f20 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi1EfEEEEE15getBlendedValueEPvPfiSD_
void vec2UnitGetBlendedValue(void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    float sum[2];
    accumulateWeighted<2>(values, weights, count, sum);
    out[0] = sum[0];
    out[1] = sum[1];
}

// 0x621fc4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELin1EfEEEEE15getBlendedValueEPvPfiSD_
void vec2GetBlendedValue(void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    float sum[2];
    accumulateWeighted<2>(values, weights, count, sum);
    out[0] = sum[0];
    out[1] = sum[1];
}

// 0x622068 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA2_fNS1_6CMixinIfLi2ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_S4_EEEELi0EfEEEEE13getAddedValueEPvPfiSD_
void vec2ZeroGetAddedValue(void* self, const float* values, const float* weights, int count, float* out) {
    (void)self;
    float sum[2];
    accumulateWeighted<2>(values, weights, count, sum);
    out[0] = sum[0];
    out[1] = sum[1];
}

}  // namespace glitch::collada::recon_13
