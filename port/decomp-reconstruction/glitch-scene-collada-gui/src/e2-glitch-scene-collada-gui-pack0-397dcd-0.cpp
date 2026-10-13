// Track B batch e2-glitch-scene-collada-gui-pack0-397dcd-0.
//
// Owners: glitch::scene::CCameraSceneNode (constructor only; the other camera functions are in
// tb_scene-collada-gui-glitch_scene_CCameraSceneNode-0.cpp), and glitch::collada::animation_track::CVirtualEx<CApplyValueEx<vector3d<float>,
// CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>>.
//
// The ScaleZ CVirtualEx bodies (0x60ef1c, 0x610ebc, 0x620600, 0x622bf8, 0x62a2e0, 0x62c3c4) are already defined in
// src/tb_scene-collada-gui-glitch_collada_animation_track_CVirtualEx_glitch-0.cpp (batch e1). They are not defined
// again here, to avoid duplicate symbols. Their ledger rows in this batch point at that file.

#include <cstdint>
#include <cstring>

#include "glitch-scene-collada-gui/recon/slice_13.hpp"
#include "glitch-scene-collada-gui/scene_nodes.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CApplyValueEx_ScaleYVec3f.hpp"
#include "glitch-scene-collada-gui/tb/glitch_collada_animation_track_CVirtualEx_ScaleYVec3f.hpp"

namespace glitch::scene {

namespace {

// Reinterprets a 32-bit word as the float it encodes (the original stores the raw bits).
float floatFromBits(std::uint32_t bits) {
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

}  // namespace

// 0x583734 _ZN6glitch5scene16CCameraSceneNodeC1EiRKNS_4core8vector3dIfEES6_b (complete object)
// 0x583878 _ZN6glitch5scene16CCameraSceneNodeC2EiRKNS_4core8vector3dIfEES6_b (base object)
// The ICameraSceneNode base gets (id, position, rotation (0, 0, 0), scale (1, 1, 1)). The members are then written
// as the pseudocode writes them: target (+0x138), up vector (0, 1, 0) (+0x144), fov 0x3FA0D97C (+0x154),
// aspect 0x3FAAAAAB (+0x158), near 1.0 (+0x15c), far 2048.0 = 0x45000000 (+0x160), farToInfinity (+0x164) and
// inputReceiverEnabled = true (+0x165). The SViewFrustum member (+0x168) is constructed by its own constructor.
// The orthogonal flag (+0x134) is written by the base and mag (+0x150) is not written by this constructor. The body
// then calls recalculateProjectionMatrix() (the virtual at Hex-Rays +0x158) and ISceneNode::setAutomaticCulling(0).
// PARTIAL for the complete-object variant 0x583734: that variant also stores the vtable word and the reference count
// 1 of the IReferenceCounted virtual base (+0x384/+0x388), which ISceneNode does not model here. The base-object
// variant 0x583878 never touches the virtual base, so this constructor is its full body.
CCameraSceneNode::CCameraSceneNode(int id, const core::vector3d<float>& position,
                                   const core::vector3d<float>& targetPosition, bool farToInfinityValue)
    : ICameraSceneNode(id, position, core::vector3d<float>{0.0f, 0.0f, 0.0f},
                       core::vector3d<float>{1.0f, 1.0f, 1.0f}),
      target(targetPosition),
      upVector{0.0f, 1.0f, 0.0f},
      fov(floatFromBits(0x3FA0D97Cu)),
      aspectRatio(floatFromBits(0x3FAAAAABu)),
      nearValue(1.0f),
      farValue(2048.0f),
      farToInfinity(farToInfinityValue),
      inputReceiverEnabled(true) {
    recalculateProjectionMatrix();
    setAutomaticCulling(0);  // ISceneNode::setAutomaticCulling(this, 0)
}

}  // namespace glitch::scene

namespace glitch::collada::animation_track {

namespace {

using ScaleYVec3Track =
    CVirtualEx<CApplyValueEx<::glitch::core::vector3d<float>,
                             CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>>;

}  // namespace

// 0x60ef34 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE12getValueSizeEv
// Reconstructed: returns 12, the size of three float words.
int ScaleYVec3Track::getValueSize() const noexcept {
    return 12;
}

// 0x610d00 _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE11getInstanceEv
// PARTIAL: the function-local static and its atexit destructor are reproduced. The original stores the vtable
// address off_9799A8 (its word for this instantiation) into the instance under the guard. This class declares no
// virtual methods, so that vtable word is not kept, and the static is the typed instance instead of an int*.
ScaleYVec3Track* ScaleYVec3Track::getInstance() {
    static ScaleYVec3Track instance;
    return &instance;
}

// 0x62069c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE13retrieveValueEPvSD_
// PARTIAL: Hex-Rays calls vtable slot +0x90 (144) on the source node, copies the three words it returns to out and
// returns that pointer. The node interface is ISceneNodeTransform in recon/slice_13.hpp, which names +0x90
// scaleWords. That slot name is the sibling slice's assumption and is not verified against this vtable.
const std::uint32_t* ScaleYVec3Track::retrieveValue(const void* source, std::uint32_t* out) const {
    auto* node = static_cast<recon_13::ISceneNodeTransform*>(const_cast<void*>(source));
    const std::uint32_t* result = node->scaleWords();
    out[0] = result[0];
    out[1] = result[1];
    out[2] = result[2];
    return result;
}

// 0x622c40 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
// PARTIAL: the original is a tail call to vtable slot +0x94 (148) of its target argument, passing target as the
// only argument, and returns that result. The interface of target is not mapped, so the slot cannot be called
// here. The value and the applicator info are not read. The body returns 0 as a placeholder for the unmapped call.
int ScaleYVec3Track::applyValue(const void* value, void* target, const CApplicatorInfo* info) const {
    (void)value;
    (void)target;
    (void)info;
    return 0;  // placeholder for the unmapped slot +0x94 call on target
}

// 0x62a034 _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE15getBlendedValueEPvPfiSD_
// Reconstructed: count == 1 copies the first three-float vector without weighting. Otherwise each vector is
// multiplied by its weight and the three components are summed over count entries. count == 0 writes zero. The
// loop is a do/while on count, as in the original.
void ScaleYVec3Track::getBlendedValue(const float* values, const float* weights, int count, float* out) const {
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

// 0x62ca3c _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
// PARTIAL: a thunk. The original drops its own receiver and tail-calls
// CApplyValueEx<...ScaleY...>::applyBlendedValueEx(values, weights, count, target), with the same argument order.
// The callee lives in the e1 applicator file and is partial there (it returns 0 until its vtable slot +0x94 is
// mapped), so this forward inherits that status. The info argument is passed through unread.
int ScaleYVec3Track::applyBlendedValue(const float* values, const float* weights, int count, void* target,
                                       const CApplicatorInfo* info) const {
    return CApplyValueEx<::glitch::core::vector3d<float>,
                         CSceneNodeScaleComponentMixin<CSceneNodeScaleYEx<float>, 1, float>>::
        applyBlendedValueEx(const_cast<void*>(static_cast<const void*>(values)),
                            const_cast<float*>(weights), count, target,
                            const_cast<CApplicatorInfo*>(info));
}

}  // namespace glitch::collada::animation_track
