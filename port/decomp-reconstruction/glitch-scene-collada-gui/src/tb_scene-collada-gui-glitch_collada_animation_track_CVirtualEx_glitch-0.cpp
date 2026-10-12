// Track B batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CVirtualEx_glitch-0:
// CVirtualEx<CApplyValueEx<vector3d<float>, CSceneNodeScaleComponentMixin<CSceneNodeScaleZEx<float>, 2, float>>>.
// Declarations are in glitch-scene-collada-gui/animation_track_virtual_ex.hpp.

#include <cstdint>

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"
#include "glitch-scene-collada-gui/recon/slice_13.hpp"

namespace glitch::collada::animation_track {

namespace {

using ScaleZVec3Track =
    CVirtualEx<CApplyValueEx<::glitch::core::vector3d<float>,
                             CSceneNodeScaleComponentMixin<CSceneNodeScaleZEx<float>, 2, float>>>;

}  // namespace

// 0x60ef1c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE12getValueSizeEv
// reconstructed: Hex-Rays returns 12, the size of three float words.
int ScaleZVec3Track::getValueSize() const {
    return 12;
}

// 0x610ebc _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE11getInstanceEv
// partial: the static instance and its atexit destructor are reproduced. The original stores the
// vtable address off_979B70 into the instance (+0x00) under the guard. This class declares no
// virtual methods, so that vtable word is not kept, and the static's type is the typed instance
// instead of an int*.
ScaleZVec3Track* ScaleZVec3Track::getInstance() {
    static ScaleZVec3Track instance;
    return &instance;
}

// 0x620600 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE13retrieveValueEPvSD_
// partial: Hex-Rays calls the vtable slot at +0x90 on the node, reads the three words it returns,
// copies them to out and returns that pointer. The node interface is ISceneNodeTransform in
// recon/slice_13.hpp, which names +0x90 scaleWords. That slot name is the sibling slice's
// assumption, not verified against the vtable here.
const std::uint32_t* ScaleZVec3Track::retrieveValue(void* node, void* out) const {
    const std::uint32_t* result =
        static_cast<recon_13::ISceneNodeTransform*>(node)->scaleWords();
    auto* words = static_cast<std::uint32_t*>(out);
    words[0] = result[0];
    words[1] = result[1];
    words[2] = result[2];
    return result;
}

// 0x622bf8 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
// partial: the original is a tail call to vtable slot +0x94 of its second argument (target),
// passing target as the receiver and returning the result. The interface of target is not
// mapped, so the slot cannot be called here. The body returns 0 as a placeholder.
int ScaleZVec3Track::applyValue(void* value, void* target, CApplicatorInfo* info) const {
    (void)value;
    (void)target;
    (void)info;
    return 0;  // placeholder for the unmapped slot +0x94 call on target
}

// 0x62a2e0 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE15getBlendedValueEPvPfiSD_
// reconstructed: count == 1 copies the first 3-float vector without weighting. Otherwise each
// vector is multiplied by its weight and the three components are summed over count entries.
// count == 0 writes zero. The loop is a do/while on count, as in the original.
void ScaleZVec3Track::getBlendedValue(const float* values, const float* weights, int count, float* out) const {
    if (count == 1) {
        out[0] = values[0];
        out[1] = values[1];
        out[2] = values[2];
        return;
    }
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumZ = 0.0f;
    if (count != 0) {
        const float* vec = values;
        int weightIndex = 0;
        int remaining = count;
        do {
            const float weight = weights[weightIndex];
            ++weightIndex;
            sumX = sumX + weight * vec[0];
            sumY = sumY + weight * vec[1];
            --remaining;
            sumZ = sumZ + weight * vec[2];
            vec += 3;
        } while (remaining != 0);
    }
    out[0] = sumX;
    out[1] = sumY;
    out[2] = sumZ;
}

// 0x62c3c4 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
// partial: a thunk. The original drops its own receiver and tail-calls
// CApplyValueEx<...>::applyBlendedValueEx(applicator, values, arg3, target, info), with the
// five explicit arguments in the same order. That target is not reconstructed in this category,
// so the call cannot be forwarded yet. The body returns 0 as a placeholder.
int ScaleZVec3Track::applyBlendedValue(void* applicator, float* values, int arg3, void* target,
                                       CApplicatorInfo* info) const {
    (void)applicator;
    (void)values;
    (void)arg3;
    (void)target;
    (void)info;
    return 0;  // placeholder for the unreconstructed applyBlendedValueEx target
}

}  // namespace glitch::collada::animation_track
