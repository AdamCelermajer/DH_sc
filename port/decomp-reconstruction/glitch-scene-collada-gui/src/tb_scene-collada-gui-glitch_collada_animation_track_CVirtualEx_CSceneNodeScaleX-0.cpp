// Bodies for the CSceneNodeScaleXEx<float> instantiation of CVirtualEx<CApplyValueEx<vector3d<float>, ...>>.
// Batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CVirtualEx_glitch-0 (see the header).
// Named by the class, because the batch id is shared by several batches.
#include "glitch-scene-collada-gui/tb/animation_track_cvirtual_scale_x.hpp"

namespace glitch::collada::animation_track {

namespace {
using ScaleXVirtual =
    CVirtualEx<CApplyValueEx<glitch::core::vector3d<float>,
                             CSceneNodeScaleComponentMixin<CSceneNodeScaleXEx<float>, 0, float>>>;
}  // namespace

// 0x60ef4c glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleXEx<float>...>::getValueSize() const
// The value is one X-scale vector of three floats (12 bytes).
int ScaleXVirtual::getValueSize() const {
    return 12;
}

// 0x610b44 glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleXEx<float>...>::getInstance()
// partial: the original initializes the function-local word s_Instance from the guest word at
// off_9797E0 under the C++ guard, and registers ~CVirtualEx with atexit. That global is not
// identified, so s_Instance stays 0. The atexit registration is omitted because the word has no
// destructor. Missing: the value of off_9797E0 (0x9797E0) and what it points to.
std::int32_t* ScaleXVirtual::getInstance() {
    static std::int32_t s_Instance = 0;
    return &s_Instance;
}

// 0x620738 glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleXEx<float>...>::retrieveValue(void*, void*) const
// partial: the original calls the virtual at vtable +0x90 (byte offset 144) of the source
// object, which returns a pointer to three words. Those words are copied to out, and the same
// pointer is returned. The slot's method and the source object type are not identified, so no
// copy is made and nullptr is returned. Missing: the interface behind vtable slot 144.
std::uint32_t* ScaleXVirtual::retrieveValue(void* source, void* out) const {
    (void)source;
    (void)out;
    return nullptr;
}

// 0x6238f4 glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleXEx<float>...>::applyValue(void*, void*, glitch::collada::animation_track::CApplicatorInfo*) const
// partial: the original tail-calls the virtual at vtable +0x94 (byte offset 148) of the second
// argument, passing that object as this. The Hex-Rays prototype shows only two arguments after
// this, and the assembly reads only the second. The mangled symbol has a third CApplicatorInfo*
// argument that is not read. Missing: the interface behind vtable slot 148, so this returns 0.
int ScaleXVirtual::applyValue(void* value, void* object, CApplicatorInfo* applicator) const {
    (void)value;
    (void)object;
    (void)applicator;
    return 0;
}

// 0x62d0b4 glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleXEx<float>...>::applyBlendedValue(void*, float*, int, void*, glitch::collada::animation_track::CApplicatorInfo*) const
// partial: a forwarder. The assembly drops the wrapper's own this, passes the five declared
// arguments on, and tail-calls CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<
// CSceneNodeScaleXEx<float>, 0, float>>::applyBlendedValueEx (symbol
// _ZN6glitch7collada15animation_track13CApplyValueEx...19applyBlendedValueEx...PNS1_15CApplicatorInfoE).
// That target is not reconstructed in this batch, so this returns 0. Missing: the target body.
int ScaleXVirtual::applyBlendedValue(void* self, float* weights, int count, void* target,
                                     CApplicatorInfo* applicator) const {
    (void)self;
    (void)weights;
    (void)count;
    (void)target;
    (void)applicator;
    return 0;
}

// 0x6278a8 glitch::collada::animation_track::CVirtualEx<...CSceneNodeScaleXEx<float>...>::getBlendedValue(void*, float*, int, void*) const
// The values are three-float vectors, and weights holds one float per vector. A count of 1 copies
// the first vector. A count of 0 writes zeros. Otherwise the result is the weighted sum of the
// count vectors, with each weight multiplied before it is added.
void ScaleXVirtual::getBlendedValue(void* values, float* weights, int count, void* out) const {
    const auto* keys = static_cast<const float*>(values);
    auto* result = static_cast<float*>(out);
    if (count == 1) {
        result[0] = keys[0];
        result[1] = keys[1];
        result[2] = keys[2];
        return;
    }
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if (count != 0) {
        const float* key = keys;
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
    result[0] = x;
    result[1] = y;
    result[2] = z;
}

}  // namespace glitch::collada::animation_track
