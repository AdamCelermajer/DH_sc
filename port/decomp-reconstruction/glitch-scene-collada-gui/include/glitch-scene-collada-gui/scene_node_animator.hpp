// glitch::collada::CSceneNodeAnimator: animator that drives a list of
// animation targets. Each target is looked up through an indirection record;
// the record's track id and track-ex id are read from the original's target
// block. Stride and pointer chasing are replaced by typed fields here.
#pragma once

#include <cstdint>
#include <vector>

namespace glitch::collada {

// One target block (16-byte stride in the original). The track and trackEx ids
// are the 32-bit words the original reads at +0x10 and +0x14 of the target.
struct SAnimatorTarget {
    std::uint32_t reserved[4] = {0, 0, 0, 0};  // +0x00..+0x0c (not yet mapped)
    std::uint32_t track = 0;                   // +0x10
    std::uint32_t trackEx = 0;                 // +0x14
};

namespace animation_track {
class CApplicatorInfo;
}  // namespace animation_track

class CSceneNodeAnimator {
public:
    std::vector<SAnimatorTarget> targets;  // +0x44..+0x48 (begin/end)

    std::uint32_t getAnimationTrackEx(int index) const;  // 0x65d3e8
    std::uint32_t getAnimationTrack(int index) const;    // 0x65d3f8
    std::uint32_t getTargetCount() const;                // 0x65d494
    // 0x65d544 createClone: original returns null.
    void* createClone() const;

    // 0x65d408 _ZN6glitch7collada18CSceneNodeAnimator9setTargetEiPvPKNS0_15animation_track15CApplicatorInfoE
    // partial: the 16-byte target record (target at +0x04, applicator at +0x08) is not mapped by
    // SAnimatorTarget, so the store and the applicator release and clone are not expressed (see the .cpp).
    void setTarget(int index, void* target, const animation_track::CApplicatorInfo* applicator);
    // 0x65d80c _ZN6glitch7collada18CSceneNodeAnimator22computeAnimationValuesEj
    // partial: updateTime, the virtual time source (vtable +0x44), getAnimationData and the
    // SAnimationAccessor getValue call are not expressed (see the .cpp).
    void computeAnimationValues(unsigned int time);
    // 0x65d91c _ZN6glitch7collada18CSceneNodeAnimator20applyAnimationValuesEj
    // partial: same missing pieces as computeAnimationValues, plus the applyValue call (see the .cpp).
    void applyAnimationValues(unsigned int time);
};

}  // namespace glitch::collada
