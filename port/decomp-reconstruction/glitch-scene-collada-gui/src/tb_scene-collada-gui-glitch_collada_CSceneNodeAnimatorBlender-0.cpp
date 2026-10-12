#include "glitch-scene-collada-gui/tb/scene_node_animator_blender.hpp"

#include <algorithm>
#include <cstddef>

namespace glitch::collada {

// 0x366f7c _ZN6glitch7collada25CSceneNodeAnimatorBlenderC2Ev
// partial: the original also stores the vtable pointer of a virtual base subobject at
// this + (vbase offset). The offset comes from the VTT argument, which is not a C++
// parameter, and no virtual base is declared, so that store is not expressed. The
// base constructor call and the zeroed vector words are reconstructed.
CSceneNodeAnimatorBlender::CSceneNodeAnimatorBlender() : ISceneNodeAnimator() {}

// 0x366594 _ZN6glitch7collada25CSceneNodeAnimatorBlender16normalizeWeightsEv
// The weights are divided by their sum. A zero sum with at least one weight sets
// weights[0] to 1.0. The original's float return value is not read by any caller.
void CSceneNodeAnimatorBlender::normalizeWeights() {
    const std::size_t count = weights.size();
    if (count == 0) {
        return;
    }
    float sum = 0.0f;
    for (std::size_t i = 0; i != count; ++i) {
        sum = sum + weights[i];
    }
    if (sum != 0.0f) {
        for (std::size_t i = 0; i != count; ++i) {
            weights[i] = weights[i] / sum;
        }
    } else {
        weights[0] = 1.0f;
    }
}

// 0x65e204 _ZN6glitch7collada25CSceneNodeAnimatorBlender17getAnimationTrackEi
// partial: the original returns animators[0]->getAnimationTrack(index), which is the
// child's vtable slot +0x54. ISceneNodeAnimator does not declare that virtual yet, so
// this returns 0. Missing: a declaration of getAnimationTrack(int) on ISceneNodeAnimator.
std::uint32_t CSceneNodeAnimatorBlender::getAnimationTrack(int index) {
    (void)index;
    return 0;
}

// 0x65e224 _ZN6glitch7collada25CSceneNodeAnimatorBlender19getAnimationTrackExEi
// partial: the original returns animators[0]->getAnimationTrackEx(index), which is the
// child's vtable slot +0x58. ISceneNodeAnimator does not declare that virtual yet, so
// this returns null. Missing: a declaration of getAnimationTrackEx(int) on ISceneNodeAnimator.
IAnimationTrackEx* CSceneNodeAnimatorBlender::getAnimationTrackEx(int index) {
    (void)index;
    return nullptr;
}

// 0x65e244 _ZN6glitch7collada25CSceneNodeAnimatorBlender9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
// partial: the original releases the previous applicator through its vtable slot +0x04
// (deleting destructor) and stores a clone of the new applicator from slot +0x08. The
// clone and the virtual release are not expressed, since CApplicatorInfo declares no
// virtuals. The previous slot is cleared and the new applicator is not stored.
void CSceneNodeAnimatorBlender::setTarget(int index, void* target, const animation_track::CApplicatorInfo* applicator) {
    targets[index] = target;
    applicators[index] = nullptr;
    (void)applicator;
}

// 0x65e34c _ZN6glitch7collada25CSceneNodeAnimatorBlender17getAnimationValueEiiPv
// Empty in the original.
void CSceneNodeAnimatorBlender::getAnimationValue(int a, int b, void* c) {
    (void)a;
    (void)b;
    (void)c;
}

// 0x65e350 _ZN6glitch7collada25CSceneNodeAnimatorBlender20applyAnimationValuesEj
// partial. Reconstructed: the weight-gated loop over the children, the normalisation,
// and the per-target loop gated by isTrackEnabled and a non-null target. Missing:
//   - animators[i]->computeAnimationValues(time): child vtable +0x4c.
//   - animators[0]->getAnimationTrackEx(i): child vtable +0x58.
//   - track->applyBlendedValue(outputs[i], weights, count, targets[i], applicators[i]):
//     IAnimationTrackEx vtable +0x18 (declared, but the call needs the child's return).
void CSceneNodeAnimatorBlender::applyAnimationValues(unsigned int time) {
    for (std::size_t i = 0; i < animators.size(); ++i) {
        if (weights[i] == 0.0f) {
            continue;
        }
        // animators[i]->computeAnimationValues(time) is not expressed (see above).
        (void)time;
    }
    normalizeWeights();
    for (std::size_t i = 0; i < targets.size(); ++i) {
        if (!isTrackEnabled(static_cast<int>(i))) {
            continue;
        }
        if (targets[i] == nullptr) {
            continue;
        }
        // The track apply call is not expressed (see above).
    }
}

// 0x65e48c _ZN6glitch7collada25CSceneNodeAnimatorBlender22computeAnimationValuesEj
// partial. Reconstructed: the weight-gated child loop, the normalisation, and the
// per-target loop gated by isTrackEnabled and a non-null target. Missing:
//   - animators[i]->computeAnimationValues(time): child vtable +0x4c.
//   - animators[0]->getAnimationTrackEx(i): child vtable +0x58.
//   - track->vtable +0x10(outputs[i], weights, count, targets[i]): the concrete track
//     class's slot +0x10 is pure in IAnimationTrackEx and its name is not mapped.
void CSceneNodeAnimatorBlender::computeAnimationValues(unsigned int time) {
    for (std::size_t i = 0; i < animators.size(); ++i) {
        if (weights[i] == 0.0f) {
            continue;
        }
        // animators[i]->computeAnimationValues(time) is not expressed (see above).
        (void)time;
    }
    normalizeWeights();
    for (std::size_t i = 0; i < targets.size(); ++i) {
        if (!isTrackEnabled(static_cast<int>(i))) {
            continue;
        }
        if (targets[i] == nullptr) {
            continue;
        }
        // The track call is not expressed (see above).
    }
}

// 0x65ed98 _ZN6glitch7collada25CSceneNodeAnimatorBlender7compileEPSt6vectorIhNS_4core10SAllocatorIhLNS_6memory13E_MEMORY_HINTE0EEEE
// partial. Reconstructed: the buffer and vector sizing, the weight and buffer zeroing,
// the per-target output pointers (target stride from getTargetSize, child block of
// childCount * stride), and the final clearing of the flag byte. Missing:
//   - child 0 setTarget(t, outputs[t], nullptr): child vtable +0x68.
//   - child 0 getAnimationTrack(t): child vtable +0x54. Its id feeds the next call.
//   - child k (k >= 1) call with (trackId, outputs[t] + k * stride): child vtable +0x60
//     (signature not mapped).
//   - the byte at this+0x24 (in the base region, not mapped) is cleared in the original.
//   - ISceneNodeAnimator::forceBind(this) when there are children: not declared.
void CSceneNodeAnimatorBlender::compile(std::vector<std::uint8_t>* buffer) {
    const std::size_t childCount = animators.size();
    const std::uint32_t targetsSize = getTargetsSize();
    const std::uint32_t targetCount = getTargetCount();

    if (buffer == nullptr) {
        buffer = &valueBuffer;
        valueBuffer.resize(childCount * targetsSize, 0);
    }

    weights.resize(childCount, 0.0f);
    std::fill(weights.begin(), weights.end(), 0.0f);

    outputs.resize(targetCount, nullptr);

    std::fill(buffer->begin(), buffer->end(), static_cast<std::uint8_t>(0));

    std::size_t offset = 0;  // the original's running byte offset into the buffer
    for (std::uint32_t t = 0; t < targetCount; ++t) {
        const std::uint32_t stride = getTargetSize(static_cast<int>(t));
        outputs[t] = buffer->data() + offset;
        // Child calls for this target are not expressed (see above).
        offset += childCount * static_cast<std::size_t>(stride);
    }

    targets.resize(targetCount, nullptr);
    applicators.resize(targetCount, nullptr);

    // *(this + 0x24) = 0 in the original (base-region byte, not mapped).

    // ISceneNodeAnimator::forceBind(this) runs when there are children (not expressed).
}

}  // namespace glitch::collada
