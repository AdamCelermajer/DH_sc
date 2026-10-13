// Bodies of CVirtualEx<CApplyValueEx<float, CMixin<float, 1, SMaterialSetParam<SAnimationTypes<float, float>>, -1, float>>>
// (batch e1-glitch-scene-collada-gui-glitch_collada_animation_track_CVirtualEx_glitch-0).
//
// The declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/animation_track_virtual_ex.hpp
// (FloatApplyValueEx section). The original file name for this batch was already holding bodies
// of the X-scale instantiation, so this batch's bodies live in this file instead.

#include "glitch-scene-collada-gui/animation_track_virtual_ex.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_float.hpp"

namespace glitch::collada::animation_track {

// 0x60ee94 glitch::collada::animation_track::CVirtualEx<glitch::collada::animation_track::CApplyValueEx<float,glitch::collada::animation_track::CMixin<float,1,glitch::collada::animation_track::SMaterialSetParam<glitch::collada::animation_track::SAnimationTypes<float,float>>,-1,float>>>::getValueSize() const
// The value is one float (4 bytes).
template <>
int CVirtualEx<FloatApplyValueEx>::getValueSize() const {
    return 4;
}

// 0x618fe4 glitch::collada::animation_track::CVirtualEx<...>::applyValue(void*, void*, glitch::collada::animation_track::CApplicatorInfo*) const
// partial: the original is a single tail call to
// glitch::video::detail::IMaterialParameters<glitch::video::CMaterial, glitch::ISharedMemoryBlockHeader<glitch::video::CMaterial>>::setParameterCvt<float>(target, 0, 0, value)
// and returns its result. The callee is not declared in a header that matches this owner
// (glitch-video declares IMaterialParameters<Owner, bool kTrackDirty> with other template
// arguments; the recon stand-ins are unconfirmed). Missing: that declaration. The info argument is
// unused by the original. Returns 0 until the callee is mapped.
// Binary check (decomp_asm 0x618fe4): the parameter id operand is LDRH R1,[R12,#8] with R12 = 0, a halfword load
// from absolute address 8 (IDA label byte_8), which Hex-Rays folds into the constant 0 of the pseudocode. The
// applicator-info pointer is never read, so the id is not a field of the info argument here. The read is of near-null
// memory, so this body is probably never executed in the original.
template <>
int CVirtualEx<FloatApplyValueEx>::applyValue(void* value, void* target, CApplicatorInfo* info) const {
    (void)value;
    (void)target;
    (void)info;
    return 0;
}

// 0x620118 glitch::collada::animation_track::CVirtualEx<...>::applyBlendedValue(void*, float*, int, void*, glitch::collada::animation_track::CApplicatorInfo*) const
// A forwarder: the original is one tail call that passes its five arguments on to
// glitch::collada::animation_track::CApplyValueEx<float, CMixin<...>>::applyBlendedValueEx(values, weights, count, target, info),
// a static member (no this argument is passed). The callee is declared in
// tb/glitch_collada_animation_track_CApplyValueEx_float.hpp (0x62008c, still partial there: its material setter
// is not mapped, so it returns 0 after computing the blend), and this forward returns whatever it returns.
template <>
int CVirtualEx<FloatApplyValueEx>::applyBlendedValue(void* values, float* weights, int count, void* target,
                                                     CApplicatorInfo* info) const {
    return FloatApplyValueEx::applyBlendedValueEx(values, weights, count, target, info);
}

// 0x620134 glitch::collada::animation_track::CVirtualEx<...>::getBlendedValue(void*, float*, int, void*) const
// Weighted sum of count floats. The assembly (decomp_asm 0x620134) branches on count:
//   count == 1: out = values[0]; the weights are not read.
//   count == 0: out = 0.0f.
//   otherwise:  acc = 0.0f, then count times acc = acc + values[i] * weights[i]; out = acc.
// The loop is a do-while on a count register, so it runs exactly `count` times for count >= 2.
template <>
void CVirtualEx<FloatApplyValueEx>::getBlendedValue(void* values, float* weights, int count, void* out) const {
    auto* in_values = static_cast<float*>(values);
    auto* result = static_cast<float*>(out);
    if (count == 1) {
        *result = *in_values;
        return;
    }
    if (count == 0) {
        *result = 0.0f;
        return;
    }
    float acc = 0.0f;
    int remaining = count;
    int index = 0;
    do {
        --remaining;
        acc = acc + in_values[index] * weights[index];  // element stride 4 bytes
        ++index;
    } while (remaining != 0);
    *result = acc;
}

}  // namespace glitch::collada::animation_track
