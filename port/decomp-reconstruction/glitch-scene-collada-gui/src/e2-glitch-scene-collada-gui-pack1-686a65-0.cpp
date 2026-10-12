// Bodies for the three vec3<float> CVirtualEx instantiations declared in
// glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_ScaleMixinPosXYVec3f.hpp:
// CSceneNodeScaleMixin<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>
// and CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>.
// Batch e2-glitch-scene-collada-gui-pack1-686a65-0.
//
// The three families have the same getValueSize and getBlendedValue bodies. Their getInstance, applyValue,
// retrieveValue and applyBlendedValue bodies differ only in the guest globals and vtable slots they use,
// which are not mapped, so those stay partial.
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_ScaleMixinPosXYVec3f.hpp"

namespace glitch::collada::animation_track {

namespace {

// Weighted sum of count vec3 values (stride 12 bytes), as the Hex-Rays body of 0x6275fc, 0x6270a4 and 0x626df8:
//   count == 1: out = values[0..2]; the weights are not read.
//   count == 0: out = (0, 0, 0).
//   otherwise:  acc = 0; count times (do-while): acc.x += w * v.x, acc.y += w * v.y, acc.z += w * v.z.
void blendVec3(const float* values, const float* weights, int count, float* out) {
    if (count == 1) {
        out[0] = values[0];
        out[1] = values[1];
        out[2] = values[2];
        return;
    }
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if (count != 0) {
        const float* key = values;
        const float* weight = weights;
        int remaining = count;
        do {
            --remaining;
            const float w = *weight;
            ++weight;
            x = x + w * key[0];
            y = y + w * key[1];
            z = z + w * key[2];
            key += 3;
        } while (remaining != 0);
    }
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

}  // namespace

// ---------------------------------------------------------------------------------------------------------------
// CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodeScaleMixin<float>>>
// ---------------------------------------------------------------------------------------------------------------

// 0x60ef64 glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleMixin<float>...>::getValueSize() const
// The value is one scale vector of three floats (12 bytes).
int CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>, CSceneNodeScaleMixin<float>>>::getValueSize() const {
    return 12;
}

// 0x610988 glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleMixin<float>...>::getInstance()
// partial: the original initializes the function-local word s_Instance from the guest word at off_979618 under
// the C++ guard, and registers ~CVirtualEx with atexit. That global is not identified, so s_Instance stays 0.
// The atexit registration is omitted because the word has no destructor. Missing: the value of off_979618.
std::int32_t* CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>, CSceneNodeScaleMixin<float>>>::getInstance() {
    static std::int32_t s_Instance = 0;
    return &s_Instance;
}

// 0x62393c glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleMixin<float>...>::applyValue(void*, void*, CApplicatorInfo*) const
// partial: the original tail-calls the virtual at vtable +0x94 (byte offset 148) of the second argument (object),
// passing that object as this. The method behind that slot is not identified, so this returns 0.
int CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>, CSceneNodeScaleMixin<float>>>::applyValue(
    void* value, void* object, CApplicatorInfo* applicator) const {
    (void)value;
    (void)object;
    (void)applicator;
    return 0;
}

// 0x6275fc glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleMixin<float>...>::getBlendedValue(void*, float*, int, void*) const
// Weighted sum of count vec3 values (see blendVec3).
void CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>, CSceneNodeScaleMixin<float>>>::getBlendedValue(
    void* values, float* weights, int count, void* out) const {
    blendVec3(static_cast<const float*>(values), weights, count, static_cast<float*>(out));
}

// 0x62d72c glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleMixin<float>...>::applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const
// partial: a forwarder. The original tail-calls CApplyValueEx<vector3d<float>, CSceneNodeScaleMixin<float>>::applyBlendedValueEx
// with the five arguments. That target is partial (its final virtual call is not mapped), and it is not called here,
// so this returns 0.
int CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>, CSceneNodeScaleMixin<float>>>::applyBlendedValue(
    void* values, float* weights, int count, void* target, CApplicatorInfo* applicator) const {
    (void)values;
    (void)weights;
    (void)count;
    (void)target;
    (void)applicator;
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------
// CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>>
// ---------------------------------------------------------------------------------------------------------------

// 0x60efc8 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionYEx<float>, 1, float>...>::getValueSize() const
// The value is one Y-position vector of three floats (12 bytes).
int CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                             CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>>::getValueSize() const {
    return 12;
}

// 0x610610 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionYEx<float>, 1, float>...>::getInstance()
// partial: the function-local word s_Instance is initialized from the guest word at off_979288 under the C++ guard,
// and ~CVirtualEx is registered with atexit. off_979288 is not identified, so s_Instance stays 0 and the atexit
// registration is omitted. Missing: the value of off_979288.
std::int32_t* CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                                       CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>>::getInstance() {
    static std::int32_t s_Instance = 0;
    return &s_Instance;
}

// 0x620460 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionYEx<float>, 1, float>...>::retrieveValue(void*, void*) const
// partial: the original calls the virtual at vtable +0xa0 (byte offset 160) of the source object, which returns a
// pointer to three words. Those words are copied to out, and the same pointer is returned. The method behind slot 160
// and the source type are not identified, so no copy is made and nullptr is returned.
std::uint32_t* CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                                        CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>>::retrieveValue(
    void* source, void* out) const {
    (void)source;
    (void)out;
    return nullptr;
}

// 0x623d48 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionYEx<float>, 1, float>...>::applyValue(void*, void*, CApplicatorInfo*) const
// partial: the original tail-calls the virtual at vtable +0xa4 (byte offset 164) of the second argument (object),
// passing that object as this. The method behind that slot is not identified, so this returns 0.
int CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                             CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>>::applyValue(
    void* value, void* object, CApplicatorInfo* applicator) const {
    (void)value;
    (void)object;
    (void)applicator;
    return 0;
}

// 0x6270a4 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionYEx<float>, 1, float>...>::getBlendedValue(void*, float*, int, void*) const
// Weighted sum of count vec3 values (see blendVec3).
void CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                              CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>>::getBlendedValue(
    void* values, float* weights, int count, void* out) const {
    blendVec3(static_cast<const float*>(values), weights, count, static_cast<float*>(out));
}

// 0x62949c glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionYEx<float>, 1, float>...>::applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const
// partial: a forwarder. The original tail-calls CApplyValueEx<..CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>::applyBlendedValueEx
// with the five arguments. That target is partial (its final virtual call is not mapped), and it is not called here,
// so this returns 0.
int CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                             CSceneNodePositionComponentMixin<CSceneNodePositionYEx<float>, 1, float>>>::applyBlendedValue(
    void* values, float* weights, int count, void* target, CApplicatorInfo* applicator) const {
    (void)values;
    (void)weights;
    (void)count;
    (void)target;
    (void)applicator;
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------
// CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>>
// ---------------------------------------------------------------------------------------------------------------

// 0x60efe0 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionXEx<float>, 0, float>...>::getValueSize() const
// The value is one X-position vector of three floats (12 bytes).
int CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                             CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>>::getValueSize() const {
    return 12;
}

// 0x610454 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionXEx<float>, 0, float>...>::getInstance()
// partial: the function-local word s_Instance is initialized from the guest word at off_9790C0 under the C++ guard,
// and ~CVirtualEx is registered with atexit. off_9790C0 is not identified, so s_Instance stays 0 and the atexit
// registration is omitted. Missing: the value of off_9790C0.
std::int32_t* CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                                       CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>>::getInstance() {
    static std::int32_t s_Instance = 0;
    return &s_Instance;
}

// 0x6204fc glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionXEx<float>, 0, float>...>::retrieveValue(void*, void*) const
// partial: the original calls the virtual at vtable +0xa0 (byte offset 160) of the source object, which returns a
// pointer to three words. Those words are copied to out, and the same pointer is returned. The method behind slot 160
// and the source type are not identified, so no copy is made and nullptr is returned.
std::uint32_t* CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                                        CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>>::retrieveValue(
    void* source, void* out) const {
    (void)source;
    (void)out;
    return nullptr;
}

// 0x623d90 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionXEx<float>, 0, float>...>::applyValue(void*, void*, CApplicatorInfo*) const
// partial: the original tail-calls the virtual at vtable +0xa4 (byte offset 164) of the second argument (object),
// passing that object as this. The method behind that slot is not identified, so this returns 0.
int CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                             CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>>::applyValue(
    void* value, void* object, CApplicatorInfo* applicator) const {
    (void)value;
    (void)object;
    (void)applicator;
    return 0;
}

// 0x626df8 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionXEx<float>, 0, float>...>::getBlendedValue(void*, float*, int, void*) const
// Weighted sum of count vec3 values (see blendVec3).
void CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                              CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>>::getBlendedValue(
    void* values, float* weights, int count, void* out) const {
    blendVec3(static_cast<const float*>(values), weights, count, static_cast<float*>(out));
}

// 0x629b14 glitch::collada::animation_track::CVirtualEx<...CSceneNodePositionXEx<float>, 0, float>...>::applyBlendedValue(void*, float*, int, void*, CApplicatorInfo*) const
// partial: a forwarder. The original tail-calls CApplyValueEx<..CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>::applyBlendedValueEx
// with the five arguments. That target is partial (its final virtual call is not mapped), and it is not called here,
// so this returns 0.
int CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                             CSceneNodePositionComponentMixin<CSceneNodePositionXEx<float>, 0, float>>>::applyBlendedValue(
    void* values, float* weights, int count, void* target, CApplicatorInfo* applicator) const {
    (void)values;
    (void)weights;
    (void)count;
    (void)target;
    (void)applicator;
    return 0;
}

}  // namespace glitch::collada::animation_track
