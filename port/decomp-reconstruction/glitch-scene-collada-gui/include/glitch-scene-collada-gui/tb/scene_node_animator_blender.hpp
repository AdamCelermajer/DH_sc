// glitch::collada::CSceneNodeAnimatorBlender: blends the output of several child
// animators. Declared for Track B batch
// e1-glitch-scene-collada-gui-glitch_collada_CSceneNodeAnimatorBlender-0.
//
// Offsets are the original 32-bit offsets, kept as comments only. Each original
// std::vector is 12 bytes (begin, end, capacity); the host build uses std::vector.
// Child animators are reached through the collada ISceneNodeAnimator base, whose
// virtual slots are not declared yet (see the partial notes in the .cpp).
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/collada_empty_dtors.hpp"  // IAnimationTrackEx, animation_track::CApplicatorInfo
#include "glitch-scene-collada-gui/collada_misc.hpp"         // ISceneNodeAnimator

namespace glitch::collada {

class CSceneNodeAnimatorBlender : public ISceneNodeAnimator {
public:
    // 0x366f7c _ZN6glitch7collada25CSceneNodeAnimatorBlenderC2Ev
    // The original VTT argument is not a C++ parameter. Member defaults give the zeroed words.
    CSceneNodeAnimatorBlender();

    // 0x366594 _ZN6glitch7collada25CSceneNodeAnimatorBlender16normalizeWeightsEv
    // The original returns a float register value that no caller reads, so this is void.
    void normalizeWeights();

    // 0x65e204 _ZN6glitch7collada25CSceneNodeAnimatorBlender17getAnimationTrackEi
    std::uint32_t getAnimationTrack(int index);
    // 0x65e224 _ZN6glitch7collada25CSceneNodeAnimatorBlender19getAnimationTrackExEi
    IAnimationTrackEx* getAnimationTrackEx(int index);
    // 0x65e244 _ZN6glitch7collada25CSceneNodeAnimatorBlender9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
    void setTarget(int index, void* target, const animation_track::CApplicatorInfo* applicator);
    // 0x65e34c _ZN6glitch7collada25CSceneNodeAnimatorBlender17getAnimationValueEiiPv
    // Empty in the original.
    void getAnimationValue(int a, int b, void* c);
    // 0x65e350 _ZN6glitch7collada25CSceneNodeAnimatorBlender20applyAnimationValuesEj
    void applyAnimationValues(unsigned int time);
    // 0x65e48c _ZN6glitch7collada25CSceneNodeAnimatorBlender22computeAnimationValuesEj
    void computeAnimationValues(unsigned int time);
    // 0x65ed98 _ZN6glitch7collada25CSceneNodeAnimatorBlender7compileEPSt6vectorIhNS_4core10SAllocatorIhLNS_6memory13E_MEMORY_HINTE0EEEE
    // buffer == nullptr selects the animator's own valueBuffer (original +0x40).
    void compile(std::vector<std::uint8_t>* buffer);

    // Blender vtable slots used by compile and applyAnimationValues. The count and
    // size accessors are defined by their own batches.
    // +0x70 getTargetCount
    virtual std::uint32_t getTargetCount() const;
    // +0x74 getTargetSize(int)
    virtual std::uint32_t getTargetSize(int index) const;
    // +0x78 getTargetsSize
    virtual std::uint32_t getTargetsSize() const;
    // +0x80 ISceneNodeAnimator::isTrackEnabled (0x366214). The base returns true.
    virtual bool isTrackEnabled(int index) const { return true; }

    // +0x28 vector<ISceneNodeAnimator*>: the child animators. Index 0 is the
    // primary child used for track lookups. (+0x28 begin, +0x2c end, +0x30 cap)
    std::vector<ISceneNodeAnimator*> animators;
    // +0x34 vector<float>: one blend weight per child. (+0x34, +0x38, +0x3c)
    std::vector<float> weights;
    // +0x40 vector<uint8_t>: default output buffer for compile. (+0x40, +0x44, +0x48)
    std::vector<std::uint8_t> valueBuffer;
    // +0x4c vector<void*>: one output pointer per target. (+0x4c, +0x50, +0x54)
    std::vector<void*> outputs;
    // +0x58 vector<void*>: one target pointer per target. (+0x58, +0x5c, +0x60)
    std::vector<void*> targets;
    // +0x64 vector<CApplicatorInfo*>: one applicator per target. (+0x64, +0x68, +0x6c)
    std::vector<animation_track::CApplicatorInfo*> applicators;
};

}  // namespace glitch::collada
