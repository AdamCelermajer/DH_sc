// Slice 13 reconstruction (glitch::collada::animation_track value wrappers). Free functions for
// the rebuilt bodies plus the minimal stand-in types they need. Interpolators and applicator
// helpers that live outside this slice are declared below as unattached externals; their owning
// slices define them. Existing headers are not edited.
#pragma once

#include <cstddef>
#include <cstdint>

namespace glitch::collada::recon_13 {

struct SAnimationAccessor;  // opaque SAnimationAccessor of the collada animation set

// Output table of an accessor. The value array is the pointer at +0x04.
struct SAnimationOutput {
    std::uint32_t reserved;  // +0x00
    void* values;            // +0x04
};

// Parameter binding handed to the applicators. The parameter id is the 16-bit word at +0x08.
struct SParameterBinding {
    std::uint32_t reserved[2];  // +0x00..+0x07
    std::uint16_t parameterId;  // +0x08
};

// Scene-node transform source. The original reads the component words through the virtual slots
// at +0xa0 (position) and +0x90 (scale); the slot order is not mapped, so they are named here.
class ISceneNodeTransform {
public:
    virtual ~ISceneNodeTransform() = default;
    virtual const std::uint32_t* positionWords() = 0;  // vtable +0xa0
    virtual const std::uint32_t* scaleWords() = 0;     // vtable +0x90
};

// Receives an interpolated value. The original calls vtable slot +0x9c on the target.
class IAnimationTarget {
public:
    virtual ~IAnimationTarget() = default;
    virtual int applyValue(const void* value) = 0;  // vtable +0x9c
};

// ---- Unattached externals (defined by other slices or the accessor code) ----

// Output table lookup: SAnimationAccessor::getOutput(accessor, index).
SAnimationOutput* getOutput(SAnimationAccessor* accessor, int index);

// IMaterialParameters::setParameterCvt<T> for the value types used by these tracks.
int setParameterCvtFloat(void* parameters, std::uint16_t parameterId, int index, const void* value);
int setParameterCvtColor(void* parameters, std::uint16_t parameterId, int index, const std::uint8_t* value);
int setParameterCvtVec2(void* parameters, std::uint16_t parameterId, int index, const float* value);

// CApplicator<SLightColor>::applyValueEx(applicator, value).
float applicatorLightColorApplyValueEx(int applicator, const std::uint8_t* value);

// CInterpreter<...>::getKeyBasedValueEx overloads of the position and quaternion tracks.
void* interpPosYFloat3Ptr(SAnimationAccessor* accessor, int a3, int a4, void* a5);
float interpPosYFloat3(SAnimationAccessor* accessor, int a3, int a4, int a5, float a6, void* a7);
void* interpPosZFloat3Ptr(SAnimationAccessor* accessor, int a3, void* a4);
float interpPosZFloat3(SAnimationAccessor* accessor, int a3, int a4, float a5, void* a6);
int interpQuatMixinFloat4(SAnimationAccessor* accessor, int a2, int a3, float a4, float* a5);
int interpQuatMixinShort4(SAnimationAccessor* accessor, int a2, float* a3);
int interpQuatMixinShort4T(SAnimationAccessor* accessor, int a2, int a3, float a4, float* a5);
int interpQuatMixinChar4(SAnimationAccessor* accessor, int a2, float* a3);
int interpQuatMixinChar4T(SAnimationAccessor* accessor, int a2, int a3, float a4, float* a5);
int interpQuatAngleFloat(SAnimationAccessor* accessor, int a2, float* a3);
int interpQuatAngleFloatT(SAnimationAccessor* accessor, int a2, int a3, float a4, float* a5);
int interpQuatAngleShort(SAnimationAccessor* accessor, int a2, float* a3);
int interpQuatAngleShortT(SAnimationAccessor* accessor, int a2, int a3, float a4, float* a5);
int interpQuatAngleChar(SAnimationAccessor* accessor, int a2, float* a3);

// CBlender<quaternion, 1, quaternion>::getBlendedValueEx / getAddedValueEx.
void blenderQuatGetBlendedValueEx(const void* values, const void* weights, int count, float* out);
void blenderQuatGetAddedValueEx(const void* values, const void* weights, int count, float* out);

// ---- Shared helpers ----

// Calls vtable slot +0x9c of the target with the value.
int callValueSlot(void* target, const void* value);

// Sets the identity quaternion (0, 0, 0, 1).
void setIdentityQuaternion(float (&quaternion)[4]);

// Weighted sum over `count` keys of `Components` values each. One key is copied unweighted.
template <std::size_t Components, typename T>
void accumulateWeighted(const T* values, const float* weights, int count, float (&sum)[Components]) {
    if (count == 1) {
        for (std::size_t c = 0; c < Components; ++c) {
            sum[c] = static_cast<float>(values[c]);
        }
        return;
    }
    for (std::size_t c = 0; c < Components; ++c) {
        sum[c] = 0.0f;
    }
    for (int key = 0; key < count; ++key) {
        for (std::size_t c = 0; c < Components; ++c) {
            sum[c] += static_cast<float>(values[static_cast<std::size_t>(key) * Components + c]) * weights[key];
        }
    }
}

// Byte-valued components are stored by truncating the accumulated float to an unsigned integer.
inline std::uint8_t truncateToByte(float value) {
    return static_cast<std::uint8_t>(static_cast<std::uint32_t>(value));
}

// ---- Declarations of the rebuilt bodies (definitions in src/recon/slice_13.cpp) ----
// Each name keeps the demangled member; the addresses and mangled names are in the cpp.

void* posYFloat_getKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, int a4, void* a5);
float posYFloat_getKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, int a5, float a6, void* a7);
void* posZFloat_getKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* a4);
float posZFloat_getKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float a5, void* a6);
void float1_getAddedValue(void* self, const void* values, const float* weights, int count, float* out);
float float1_interp_getKeyBasedValueEx(SAnimationAccessor* accessor, int key, int a3, float t, float* out);
float float1_getKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, int a4, float t, float* out);
int float1_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, int a3, float t, void* parameters, void* binding);
int float1_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, int a4, float t, void* parameters, void* binding);
const std::uint32_t* posZChar_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* posZShort_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* posYChar_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* posYShort_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* posXChar_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* posXShort_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* posMixinChar_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* posMixinShort_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* scaleZChar_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* scaleZShort_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* scaleYChar_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* scaleYShort_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* scaleXChar_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* scaleXShort_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* scaleMixinChar_retrieveValue(void* self, void* node, void* out);
const std::uint32_t* scaleMixinShort_retrieveValue(void* self, void* node, void* out);
int quatAngleChar_applyValue(void* self, void* a2, void* target);
int quatAngleShort_applyValue(void* self, void* a2, void* target);
int quatAngleFloat_applyValue(void* self, void* a2, void* target);
int quatChar_applyValue(void* self, void* a2, void* target);
int quatShort_applyValue(void* self, void* a2, void* target);
int quatFloat_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target);
int quatFloat_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, void* target);
int quatFloat_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target);
int quatFloat_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target);
int quatFloat_applyAddedValueEx(const void* values, const void* weights, int count, void* target);
int quatFloat_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target);
int quatShort_applyBlendedValueEx(void* values, const void* weights, int count, void* target);
int quatShort_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target);
int quatShort_applyAddedValueEx(const void* values, const void* weights, int count, void* target);
int quatShort_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target);
int quatShort_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target);
int quatShort_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target);
int quatShort_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target);
int quatShort_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target);
int quatChar_applyBlendedValueEx(void* values, const void* weights, int count, void* target);
int quatChar_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target);
int quatChar_applyAddedValueEx(const void* values, const void* weights, int count, void* target);
int quatChar_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target);
int quatChar_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target);
int quatChar_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target);
int quatChar_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target);
int quatChar_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target);
int quatAngleFloat_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target);
int quatAngleFloat_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target);
int quatAngleFloat_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target);
int quatAngleFloat_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target);
int quatAngleFloat_applyBlendedValueEx(void* values, const void* weights, int count, void* target);
int quatAngleFloat_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target);
int quatAngleFloat_applyAddedValueEx(const void* values, const void* weights, int count, void* target);
int quatAngleFloat_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target);
int quatAngleShort_applyBlendedValueEx(void* values, const void* weights, int count, void* target);
int quatAngleShort_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target);
int quatAngleShort_applyAddedValueEx(const void* values, const void* weights, int count, void* target);
int quatAngleShort_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target);
int quatAngleShort_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target);
int quatAngleShort_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target);
int quatAngleShort_applyKeyBasedValueExT(SAnimationAccessor* accessor, int a2, int a3, float t, void* target);
int quatAngleShort_applyKeyBasedValueT(void* self, SAnimationAccessor* accessor, int a3, int a4, float t, void* target);
int quatAngleChar_applyBlendedValueEx(void* values, const void* weights, int count, void* target);
int quatAngleChar_applyBlendedValue(void* self, void* values, const void* weights, int count, void* target);
int quatAngleChar_applyAddedValueEx(const void* values, const void* weights, int count, void* target);
int quatAngleChar_applyAddedValue(void* self, const void* values, const void* weights, int count, void* target);
int quatAngleChar_applyKeyBasedValueEx(SAnimationAccessor* accessor, int key, void* target);
int quatAngleChar_applyKeyBasedValue(void* self, SAnimationAccessor* accessor, int a3, void* target);
std::uintptr_t lightColorGetBlendedValue(void* self, const std::uint8_t* values, const float* weights, int count, std::uint8_t* out);
std::uintptr_t lightColorGetAddedValue(void* self, const std::uint8_t* values, const float* weights, int count, std::uint8_t* out);
float lightColorApplyBlendedValueEx(const std::uint8_t* values, const float* weights, int count, int applicator);
float lightColorApplyBlendedValue(void* self, const std::uint8_t* values, const float* weights, int count, int applicator);
float lightColorApplyAddedValueEx(const std::uint8_t* values, const float* weights, int count, int applicator);
float lightColorApplyAddedValue(void* self, const std::uint8_t* values, const float* weights, int count, int applicator);
std::uintptr_t colorGetBlendedValue(void* self, const std::uint8_t* values, const float* weights, int count, std::uint8_t* out);
int colorApplyBlendedValue(void* self, const std::uint8_t* values, const float* weights, int count, void* parameters, void* binding);
std::uintptr_t colorInterpGetKeyBasedValueEx(SAnimationAccessor* accessor, int key, int a3, float t, std::uint8_t* out);
std::uintptr_t colorGetKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, int a4, float t, std::uint8_t* out);
int colorApplyKeyBasedValueEx(SAnimationAccessor* accessor, int key, int a3, float t, void* parameters, void* binding);
int colorApplyKeyBasedValue(void* self, SAnimationAccessor* accessor, int key, int a4, float t, void* parameters, void* binding);
int colorApplyAddedValue(void* self, const std::uint8_t* values, const float* weights, int count, void* parameters, void* binding);
std::uintptr_t colorGetAddedValue(void* self, const std::uint8_t* values, const float* weights, int count, std::uint8_t* out);
int vec2UnitApplyAddedValueEx(const float* values, const void* weights, int count, void* parameters, void* binding);
int vec2UnitApplyAddedValue(void* self, const float* values, const void* weights, int count, void* parameters, void* binding);
int vec2UnitApplyBlendedValueEx(const float* values, const void* weights, int count, void* parameters, void* binding);
int vec2UnitApplyBlendedValue(void* self, const float* values, const void* weights, int count, void* parameters, void* binding);
int vec2ApplyAddedValueEx(const float* values, const void* weights, int count, void* parameters, void* binding);
int vec2ApplyAddedValue(void* self, const float* values, const void* weights, int count, void* parameters, void* binding);
int vec2ApplyBlendedValueEx(const float* values, const void* weights, int count, void* parameters, void* binding);
int vec2ApplyBlendedValue(void* self, const float* values, const void* weights, int count, void* parameters, void* binding);
void vec2UnitGetBlendedValue(void* self, const float* values, const float* weights, int count, float* out);
void vec2GetBlendedValue(void* self, const float* values, const float* weights, int count, float* out);
void vec2ZeroGetAddedValue(void* self, const float* values, const float* weights, int count, float* out);

}  // namespace glitch::collada::recon_13
