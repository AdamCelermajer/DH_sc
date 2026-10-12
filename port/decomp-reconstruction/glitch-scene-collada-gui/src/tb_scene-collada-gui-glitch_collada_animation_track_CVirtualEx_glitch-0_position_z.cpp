// Batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CVirtualEx_glitch-0 (position Z instance).
// Members of glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>,
// CSceneNodePositionComponentMixin<CSceneNodePositionZEx<float>, 2, float>>>. The declarations and the
// PositionZVec3Ex alias are in include/glitch-scene-collada-gui/animation_track_virtual_ex.hpp.
#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"

#include <cstdint>

namespace glitch::collada::animation_track {

// 0x60efb0 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionZEx<float>, 2, float>...>::getValueSize() const
// The value is one position vector of three floats (12 bytes).
template <>
int CVirtualEx<PositionZVec3Ex>::getValueSize() const {
    return 12;
}

// 0x6107cc glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionZEx<float>, 2, float>...>::getInstance()
// partial: the original initializes the function-local word s_Instance from the address of
// off_979450 under the C++ guard, and registers ~CVirtualEx with atexit. That global is not
// identified, so s_Instance stays 0. The atexit registration is omitted because the destructor
// is empty. Missing: the value of off_979450 (0x979450) and what it points to. The original
// stores a 32-bit address in s_Instance; the 64-bit build cannot hold that address in int32_t.
template <>
int* CVirtualEx<PositionZVec3Ex>::getInstance() {
    static std::int32_t s_Instance = 0;
    return &s_Instance;
}

// 0x6203c4 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionZEx<float>, 2, float>...>::retrieveValue(void*, void*) const
// partial: the original calls the virtual at vtable +0xa0 (byte offset 160) of the source object,
// which returns a pointer to three words. Those words are copied to out, and the same pointer is
// returned. The source object type and the method behind that slot are not identified, so no copy
// is made and nullptr is returned. Missing: the interface behind vtable slot 160.
template <>
std::uint32_t* CVirtualEx<PositionZVec3Ex>::retrieveValue(void* source, void* out) const {
    (void)source;
    (void)out;
    return nullptr;
}

// 0x622cb8 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionZEx<float>, 2, float>...>::applyValue(void*, void*, glitch::collada::animation_track::CApplicatorInfo*) const
// partial: the original tail-calls the virtual at vtable +0xa4 (byte offset 164) of the third
// argument, the CApplicatorInfo*, and returns its result. The value argument (second Hex-Rays
// argument) is not read. The layout of CApplicatorInfo's vtable is not mapped, so this returns 0.
// Missing: the CApplicatorInfo virtual behind vtable slot 164.
template <>
int CVirtualEx<PositionZVec3Ex>::applyValue(void* value, void* target, CApplicatorInfo* info) const {
    (void)value;
    (void)target;
    (void)info;
    return 0;
}

// 0x628e24 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionZEx<float>, 2, float>...>::applyBlendedValue(void*, float*, int, void*, glitch::collada::animation_track::CApplicatorInfo*) const
// partial: a forwarder. The Hex-Rays body passes its values, weights, count and target arguments
// (a2, a3, a4, a5) to CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<
// CSceneNodePositionZEx<float>, 2, float>>::applyBlendedValueEx and returns that result. The
// applicator-info argument of the mangled symbol is not forwarded. The target is not declared in
// this category, so this returns 0. Missing: the applyBlendedValueEx body for this applicator.
template <>
int CVirtualEx<PositionZVec3Ex>::applyBlendedValue(void* values, float* weights, int count, void* target,
                                                   CApplicatorInfo* info) const {
    (void)values;
    (void)weights;
    (void)count;
    (void)target;
    (void)info;
    return 0;
}

// 0x627350 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionZEx<float>, 2, float>...>::getBlendedValue(void*, float*, int, void*) const
// The values are three-float position vectors, and weights holds one float per vector. A count of
// 1 copies the first vector. Otherwise the result is the weighted sum of the count vectors, each
// component accumulated as weight times value. A count of 0 writes zeros.
template <>
void CVirtualEx<PositionZVec3Ex>::getBlendedValue(void* values, float* weights, int count, void* out) const {
    const auto* vectors = static_cast<const float*>(values);
    auto* result = static_cast<float*>(out);
    if (count == 1) {
        result[0] = vectors[0];
        result[1] = vectors[1];
        result[2] = vectors[2];
        return;
    }
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if (count != 0) {
        const float* vector = vectors;
        const float* weight = weights;
        int remaining = count;
        do {
            const float w = *weight;
            ++weight;
            x = x + w * vector[0];
            y = y + w * vector[1];
            z = z + w * vector[2];
            vector += 3;
            --remaining;
        } while (remaining != 0);
    }
    result[0] = x;
    result[1] = y;
    result[2] = z;
}

}  // namespace glitch::collada::animation_track
