// Bodies of CApplyValueEx<float, CMixin<float, 1, SMaterialSetParam<SAnimationTypes<float, float>>, -1, float>>
// (batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CApplyValueEx_flo-0).
//
// The declaration is in glitch-scene-collada-gui/include/glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_float.hpp.

#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_float.hpp"

namespace glitch::collada::animation_track {

namespace {

// Weighted sum of the first count values, as the Hex-Rays body computes it:
//   count == 1: the single value; the weights are not read.
//   count == 0: 0.0f.
//   otherwise:  acc = 0.0f, then count times acc = acc + values[i] * weights[i].
// The loop is a do-while on a down-counter, so a negative count runs the same way it does in the original.
float blendFloatValues(const float* values, const float* weights, int count) {
    if (count == 1) {
        return values[0];
    }
    if (count == 0) {
        return 0.0f;
    }
    float acc = 0.0f;
    int remaining = count;
    int index = 0;
    do {
        --remaining;
        acc = acc + values[index] * weights[index];  // element stride 4 bytes
        ++index;
    } while (remaining != 0);
    return acc;
}

}  // namespace

// 0x62008c glitch::collada::animation_track::CApplyValueEx<float, ...>::applyBlendedValueEx(void*, float*, int, void*, glitch::collada::animation_track::CApplicatorInfo*)
// partial: the blend is reconstructed, the tail call is not. The original ends with
//   IMaterialParameters<glitch::video::CMaterial, glitch::ISharedMemoryBlockHeader<glitch::video::CMaterial>>::setParameterCvt<float>(
//       target, *(uint16_t*)(info + 0x08), 0, &blended)
// and returns its result. Missing:
//   - a declaration of that setter that matches the call. glitch-video's IMaterialParameters
//     has setParameterCvt<T>(id, src, stride) and setParameterCvt(id, index, type, src), and
//     neither takes (object, id, flags, value). The first argument is the target object.
//   - the CApplicatorInfo layout at +0x08 (parameterId). layouts/animation_track.hpp has it as a
//     struct, while collada_empty_dtors.hpp declares class CApplicatorInfo, so the two cannot be
//     included together. Not read here.
// Until both are mapped the function computes the blend, discards it, and returns 0, the same
// placeholder the float CVirtualEx forwarders use.
int CApplyValueEx<float, CMixin<float, 1, SMaterialSetParam<SAnimationTypes<float, float>>, -1, float>>::applyBlendedValueEx(
    void* values, float* weights, int count, void* target, CApplicatorInfo* info) {
    const float blended = blendFloatValues(static_cast<const float*>(values), weights, count);
    (void)blended;
    (void)target;
    (void)info;
    return 0;
}

}  // namespace glitch::collada::animation_track
