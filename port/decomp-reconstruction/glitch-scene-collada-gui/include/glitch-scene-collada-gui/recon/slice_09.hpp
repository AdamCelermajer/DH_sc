// Slice 09: glitch::collada::animation_track key-based value getters.
//
// The real SAnimationAccessor, CInputReader, CApplicatorInfo and CVirtualEx
// declarations are not in the tree yet. The minimal stand-ins below model only
// the members these bodies read. Member functions of the original classes are
// declared here as free functions (the class name is in the namespace path);
// their `this` argument is dropped when the body does not use it.
#pragma once

#include <cstdint>

namespace glitch::collada::recon_09 {

struct CApplicatorInfo;  // opaque; consumed by CApplicator<SLightColor>::applyValueEx

// Output stream returned by SAnimationAccessor::getOutput(). Only the key data
// pointer (+0x04) is read here.
struct SAnimationOutput {
    std::uint32_t reserved_0x00;  // +0x00 (not read by slice 09)
    const void*   data;           // +0x04 key data
};

// Stand-in for glitch::collada::SAnimationAccessor.
struct SAnimationAccessor {
    SAnimationOutput* output = nullptr;        // getOutput(0)
    const float*      scales = nullptr;        // getScales()
    const float*      offsets = nullptr;       // getOffsets()
    const float*      defaultValue = nullptr;  // getDefaultValue(); null when none

    SAnimationOutput* getOutput(int) const { return output; }
    const float* getScales() const { return scales; }
    const float* getOffsets() const { return offsets; }
    bool hasDefaultValue() const { return defaultValue != nullptr; }
    const float* getDefaultValue() const { return defaultValue; }
};

// CInputReader<T, F, N>: caches the output stream and the scale and offset
// arrays. Hex-Rays stores these in three DWORD slots; they are named here.
template <class T, class F, int N>
struct CInputReader {
    explicit CInputReader(SAnimationAccessor* accessor);  // defined in slice_09.cpp

    SAnimationOutput* output;  // [0]
    const F*          scales;  // [1]
    const F*          offsets; // [2]

    const T* keys() const { return static_cast<const T*>(output->data); }
};

// Calls into code outside this slice (other categories). Signatures are taken
// from the call sites in the pseudocode; the definitions live elsewhere.
namespace ext {
// Core::quaternion operator* (out = lhs * rhs) and quaternion slerp.
void quaternion_multiply(float out[4], const float lhs[4], const float rhs[4]);
void quaternion_slerp(float out[4], const float from[4], const float to[4], float t);
// CApplicator<SLightColor>::applyValueEx.
float applicator_light_color_applyValueEx(CApplicatorInfo* info, const std::uint8_t* value);
// CInterpreter<CSceneNodeQuaternionMixin<float>,float,4,SUseDefaultLerp<float>>::getKeyBasedValueEx.
bool interp_quat_float_getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out);
// CBlender<quaternion,1,quaternion>::getBlendedValueEx / getAddedValueEx.
bool blender_quat_getBlendedValueEx(const void* values, const float* weights, int count, void* out);
void* blender_quat_getAddedValueEx(const void* values, const float* weights, int count, void* out);
}  // namespace ext

// ---- CInterpreter<CMixin<uchar,3,SLightColor,-1,uchar>,uchar,3,...> ----
namespace interp_light_u8x3 {
unsigned getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, std::uint8_t* out);       // 0x6126c8
unsigned getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out); // 0x612874
}

// ---- CApplyValueEx<vec3,CLightColorMixin<uchar>> and its CVirtualEx ----
namespace apply_light_u8x3 {
float applyKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, CApplicatorInfo* info);  // 0x6127a4
}

namespace virtual_light_u8x3 {
float applyKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, CApplicatorInfo* info);   // 0x6127e0
unsigned getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, std::uint8_t* out);      // 0x612804
SAnimationOutput* getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, std::uint8_t* out);      // 0x612820
unsigned getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out); // 0x612908
}

// ---- CApplyValueEx<uchar[3],CMixin<uchar,3,SMaterialSetParam...>> ----
namespace virtual_material_u8x3 {
SAnimationOutput* getKeyBasedValue(SAnimationAccessor* self, int a2, std::uint8_t* out);              // 0x61292c
SAnimationOutput* getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, std::uint8_t* out);      // 0x612970
unsigned getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out); // 0x612a58
}

namespace interp_material_u8x3 {
unsigned getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out); // 0x6129c4
}

// ---- CApplyValueEx<float[2],CMixin<float,2,SMaterialSetParam...>> ----
namespace virtual_float2 {
SAnimationOutput* getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                    // 0x612a7c
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out);                        // 0x612ab0
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);       // 0x612b98
}

namespace interp_float2 {
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);     // 0x612afc
}

// ---- CApplyValueEx<uchar[4],CMixin<uchar,4,SMaterialSetParam...>> ----
namespace virtual_u8x4 {
SAnimationOutput* getKeyBasedValue(SAnimationAccessor* self, int a2, std::uint8_t* out);             // 0x612bbc
SAnimationOutput* getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, std::uint8_t* out);     // 0x612c08
unsigned getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out); // 0x612cdc
}

namespace interp_u8x4 {
unsigned getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, std::uint8_t* out); // 0x612c54
}

// ---- quaternion blend / add forwards (CVirtualEx<CApplyValueEx<quaternion,...>>) ----
namespace quat_mixin_float {
bool getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out);                 // 0x6132e4
void* getAddedValue(const void* values, const float* weights, int count, void* out);                  // 0x613588
}

namespace quat_mixin_short {
bool getBlendedValue(const void* values, const float* weights, int count, void* out);                // 0x613314
void* getAddedValue(const void* values, const float* weights, int count, void* out);                  // 0x61359c
float getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                                 // 0x6136d0
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out);                         // 0x61379c
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);        // 0x6138d0
bool getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out);                 // 0x6139b8
}

namespace quat_mixin_char {
bool getBlendedValue(const void* values, const float* weights, int count, void* out);                // 0x613328
void* getAddedValue(const void* values, const float* weights, int count, void* out);                  // 0x6135b0
float getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                                 // 0x613aa4
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out);                         // 0x613b70
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);        // 0x613ca4
bool getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out);                 // 0x613d8c
}

namespace quat_angle_float {
bool getBlendedValue(const void* values, const float* weights, int count, void* out);                // 0x61333c
void* getAddedValue(const void* values, const float* weights, int count, void* out);                  // 0x6135c4
}

namespace quat_angle_short {
bool getBlendedValue(const void* values, const float* weights, int count, void* out);                // 0x613350
void* getAddedValue(const void* values, const float* weights, int count, void* out);                  // 0x6135d8
}

namespace quat_angle_char {
bool getBlendedValue(const void* values, const float* weights, int count, void* out);                // 0x613364
void* getAddedValue(const void* values, const float* weights, int count, void* out);                  // 0x6135ec
}

// ---- quaternion interpolators ----
namespace interp_quat_short {
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                               // 0x61363c
bool getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out);               // 0x6138f4
}

namespace interpq_quat_short {
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out);                       // 0x6136e0
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);      // 0x6137b0
}

namespace interp_quat_char {
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                               // 0x613a10
bool getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out);               // 0x613cc8
}

namespace interpq_quat_char {
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out);                       // 0x613ab4
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);      // 0x613b84
}

// ---- quaternion-angle interpolators (default values for components 0..2) ----
namespace interp_quat_angle_short {
const float* getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                        // 0x614a90
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out);              // 0x614b38
}

namespace interp_quat_angle_char {
const float* getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                        // 0x614c44
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out);              // 0x614ce8
}

// ---- CInterpreter<CMixin<float,3,SSceneNodePosition>> (short and char keys) ----
namespace interp_pos_f3_short {
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                               // 0x613e5c
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out);                       // 0x613efc
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);      // 0x613fec
}

namespace virtual_pos_f3_short {
float getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                                 // 0x613eec
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out);                         // 0x613fd8
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);        // 0x614130
}

namespace interp_pos_f3_char {
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                               // 0x614488
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out);                       // 0x614528
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);      // 0x61461c
}

namespace virtual_pos_f3_char {
float getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                                 // 0x614518
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out);                         // 0x614608
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);        // 0x614768
}

// ---- CInterpreter<CSceneNodeScaleMixin<short|char>,float,3,...> ----
namespace interp_scale_f3_short {
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                               // 0x614154
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out);                       // 0x6141f4
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);      // 0x6142e4
}

namespace virtual_scale_f3_short {
float getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                                 // 0x6141e4
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out);                         // 0x6142d0
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);        // 0x614428
}

namespace interp_scale_f3_char {
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                               // 0x61478c
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out);                       // 0x61482c
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);      // 0x614920
}

namespace virtual_scale_f3_char {
float getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                                 // 0x61481c
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out);                         // 0x61490c
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);        // 0x614a6c
}

// ---- CInterpreter<CSceneNodePositionXEx<short|char>,float,3,SUseDefaultValues<0,...>> ----
namespace interp_pos_x_short {
const float* getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                        // 0x614dec
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out);              // 0x614e9c
const float* getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out);                // 0x614fc8
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);      // 0x6150ac
}

namespace virtual_pos_x_short {
const float* getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                          // 0x614e8c
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out);                // 0x614fac
const float* getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out);                  // 0x615098
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);        // 0x6151f8
}

namespace interp_pos_x_char {
const float* getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                        // 0x61521c
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out);              // 0x6152c8
const float* getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float* out);                // 0x6153ec
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);      // 0x6154c8
}

namespace virtual_pos_x_char {
const float* getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                          // 0x6152b8
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float t, float* out);                // 0x6153d0
const float* getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, float* out);                  // 0x6154b4
float getKeyBasedValue(SAnimationAccessor* self, int a2, int a3, int a4, float t, float* out);        // 0x615608
}

// ---- CInterpreter<CSceneNodePositionYEx<short>,float,3,SUseDefaultValues<1,short>> ----
namespace interp_pos_y_short {
const float* getKeyBasedValueEx(SAnimationAccessor* self, int a2, float* out);                        // 0x61562c
float getKeyBasedValueEx(SAnimationAccessor* self, int a2, int a3, float t, float* out);              // 0x6156dc
}

namespace virtual_pos_y_short {
const float* getKeyBasedValue(SAnimationAccessor* self, int a2, float* out);                          // 0x6156cc
}

// ---- CInputReader instantiations (template constructor in slice_09.cpp) ----
// 0x613600 CInputReader<short,float,4>   0x6139d4 CInputReader<char,float,4>
// 0x613da8 CInputReader<short,float,1>   0x613de4 CInputReader<char,float,1>
// 0x613e20 CInputReader<short,float,3>   0x61444c CInputReader<char,float,3>

}  // namespace glitch::collada::recon_09
