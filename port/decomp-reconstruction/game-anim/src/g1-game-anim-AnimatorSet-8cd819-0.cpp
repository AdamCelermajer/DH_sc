// Batch g1-game-anim-AnimatorSet-8cd819-0 (game-anim, owner AnimatorSet).
//
// AnimatorSet derives from glitch::collada::CSceneNodeAnimatorSet (canonical, glitch-scene-collada-gui), which is
// the original base. Every body that forwards to a base method, or reads the timeline or Animation fields that
// have no declaration yet, is still partial. Those spots are written as TODO comments with the original
// address and offset. The layout requests for them are in the batch report.

#include "dh2/AnimatorSet.hpp"

#include "dh2/AnimApplicator.hpp"
#include "dh2/AnimationSet.hpp"

namespace animator_set_detail {

// Helper, not an original symbol. It is the arithmetic of AnimatorSet::_HandleAnimEnding (0x3672c8) on the
// timeline values that function reads. startSec is the float at ITimelineController +0x1c, endSec the float at
// +0x2c, currentMs the int at +0x04 (currentFrame). The result is what the original stores at AnimatorSet +0x68
// (AnimApplicator unk_10). Checked against the asm: fmul by 1000.0f, f2iz (truncation), RSB, signed compares.
int remainingAnimTimeMs(float startSec, float endSec, int currentMs) {
    const int startMs = static_cast<int>(startSec * 1000.0f);
    const int endMs = static_cast<int>(endSec * 1000.0f);
    const int rem = endMs - currentMs;
    int result = rem < startMs ? 1 : 0;
    if (rem < 0) result = 0;
    if (result != 0) result = startMs - rem;
    return result;
}

}  // namespace animator_set_detail

// 0x3672c0 _ZNK11AnimatorSet7getTypeEv
glitch::scene::E_SCENE_NODE_ANIMATOR_TYPE AnimatorSet::getType() const {
    return static_cast<glitch::scene::E_SCENE_NODE_ANIMATOR_TYPE>(13);
}

// 0x3672c8 _ZN11AnimatorSet17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
// PARTIAL: the remaining-time store (AnimatorSet +0x68 = AnimApplicator::unk_10) needs ITimelineController +0x1c and
// +0x2c, which glitch-scene-collada-gui/scene_nodes.hpp does not declare. Only the flag store is rebuilt here.
void AnimatorSet::_HandleAnimEnding(glitch::scene::ITimelineController* timeline) {
    if (timeline != nullptr) {
        // TODO(layout): startSec = timeline +0x1c (float), endSec = timeline +0x2c (float), then
        // m_applicator.unk_10 = animator_set_detail::remainingAnimTimeMs(startSec, endSec, timeline->currentFrame).
        // Written to AnimatorSet +0x68, which is AnimApplicator +0x10.
    }
    m_applicator.m_callbackPending = true;  // AnimatorSet +0x88 (AnimApplicator +0x30), set unconditionally
}

// 0x367338 _ZN11AnimatorSet7_CBAnimEPN6glitch5scene19ITimelineControllerEPv
// Timeline callback. SetCallbacks stores this function and the AnimatorSet in the timeline (+0x08 and +0x0c). The
// timeline passes the user data back, so the first argument of _HandleAnimEnding is the user data (the asm swaps
// r0 and r1 before the tail call).
void AnimatorSet::_CBAnim(glitch::scene::ITimelineController* timeline, void* userData) {
    static_cast<AnimatorSet*>(userData)->_HandleAnimEnding(timeline);
}

// 0x367368 _ZN11AnimatorSet22computeAnimationValuesEj
// PARTIAL: the base forward glitch::collada::CSceneNodeAnimatorSet::computeAnimationValues(this, time) is not reachable
// (base not declared). The store before it is not rebuilt either:
//   base currentAnimation (+0x50) = m_currentAnimation ? m_currentAnimation->+0x20 : 0
// Animation +0x20 is not declared in dh2/Animation.hpp.
void AnimatorSet::computeAnimationValues(unsigned int time) {
    // TODO(layout): AnimatorSet +0x50 = m_currentAnimation ? Animation +0x20 : 0.
    // TODO(base): glitch::collada::CSceneNodeAnimatorSet::computeAnimationValues(this, time).
    (void)time;
}

// 0x36737c _ZN11AnimatorSet20applyAnimationValuesEj
// PARTIAL: same +0x50 store as computeAnimationValues, then the base forward
// glitch::collada::CSceneNodeAnimatorSet::applyAnimationValues(this, time). Neither part is reachable yet.
void AnimatorSet::applyAnimationValues(unsigned int time) {
    // TODO(layout): AnimatorSet +0x50 = m_currentAnimation ? Animation +0x20 : 0.
    // TODO(base): glitch::collada::CSceneNodeAnimatorSet::applyAnimationValues(this, time).
    (void)time;
}

// 0x367390 _ZN11AnimatorSet17getAnimationValueEiiPv
// PARTIAL: same +0x50 store, then the base call glitch::collada::CSceneNodeAnimatorSet::getAnimationValue(this, track,
// index, out), which returns void (the canonical signature). The store and the base call are not written yet.
void AnimatorSet::getAnimationValue(int track, int index, void* out) {
    // TODO(layout): AnimatorSet +0x50 = m_currentAnimation ? Animation +0x20 : 0.
    // TODO(base): glitch::collada::CSceneNodeAnimatorSet::getAnimationValue(track, index, out).
    (void)track;
    (void)index;
    (void)out;
}

// 0x3673a4 _ZN11AnimatorSet11animateNodeEPN6glitch5scene10ISceneNodeEj
// PARTIAL: the base animateNode runs first in the original and is not reachable. The applicator step is rebuilt. The
// callback check needs the timeline from vtable slot +0x44 (ISceneNodeAnimator::getTimelineCtrl), which is not
// declared on this class yet.
void AnimatorSet::animateNode(glitch::scene::ISceneNode* node, unsigned int time) {
    // TODO(base): glitch::collada::CSceneNodeAnimatorSet::animateNode(this, node, time) runs first.
    m_applicator.AnimApplicator::AnimateNode(time);  // 0x3645e8, direct call
    // TODO(timeline): timeline = this->getTimelineCtrl() (vtable +0x44), then m_applicator.CheckCallback(timeline).
    (void)node;
}

// 0x3673e4 _ZN11AnimatorSet10updateTimeEj
// PARTIAL: same +0x50 store, the base ISceneNodeAnimator::updateTime forward, and the timeline callback check
// (vtable +0x44, then AnimApplicator::CheckCallback) are not reachable yet.
void AnimatorSet::updateTime(unsigned int time) {
    // TODO(layout): AnimatorSet +0x50 = m_currentAnimation ? Animation +0x20 : 0.
    // TODO(base): glitch::collada::ISceneNodeAnimator::updateTime(this, time).
    // TODO(timeline): m_applicator.CheckCallback(this->getTimelineCtrl()) (vtable +0x44).
    (void)time;
}

// 0x367420 _ZN11AnimatorSet19setCurrentAnimationEi
// PARTIAL: the lookup by set index, the clip check (Animation unk_20 != -1), the use counts (m_useCount +0x24 and
// unk_2c +0x2c) and the m_currentAnimation swap with the release of the previous clip are rebuilt. Missing: the base
// forward glitch::collada::CSceneNodeAnimatorSet::setCurrentAnimation(setIndex). The canonical override returns void,
// as the original's base result does, so the lookup result is no longer returned.
void AnimatorSet::setCurrentAnimation(int setIndex) {
    Animation* result = m_animationSet->GetAnimationBySetIndex(setIndex);
    if (result->unk_20 != -1) {
        ++result->m_useCount;   // +0x24
        result->unk_2c += 1;    // +0x2c
        Animation* previous = m_currentAnimation;
        m_currentAnimation = result;  // +0x98
        if (previous != nullptr) {
            --previous->m_useCount;   // +0x24 of the clip that was current
        }
        // TODO(base): glitch::collada::CSceneNodeAnimatorSet::setCurrentAnimation(setIndex) (engine base).
    }
}

// 0x367484 _ZN11AnimatorSet8onUnbindEPN6glitch5scene10ISceneNodeE
// PARTIAL: the applicator reference node is cleared (direct call, 0x36473c). The base
// glitch::collada::ISceneNodeAnimator::onUnbind forward is not reachable.
void AnimatorSet::onUnbind(glitch::scene::ISceneNode* node) {
    m_applicator.AnimApplicator::SetRefNode(nullptr);
    // TODO(base): glitch::collada::ISceneNodeAnimator::onUnbind(this, node).
    (void)node;
}

// 0x3674ac _ZN11AnimatorSet19SetCurrentAnimationEi
// PARTIAL: the lookup by AnimDict id, the clip read (Animation unk_20), the use counts, the m_currentAnimation swap and
// the release of the previous clip are rebuilt. The return value is the clip in both branches, as in the original, so
// only the base side effect is missing: glitch::collada::CSceneNodeAnimatorSet::setCurrentAnimation(this, clip), whose
// result the original discards. This class does not derive from that base (pad_04).
// The original re-reads Animation +0x20 after the base call; the stub returns the clip read before it.
int AnimatorSet::SetCurrentAnimation(int animId) {
    Animation* anim = m_animationSet->GetAnimation(animId);
    const int clip = anim->unk_20;
    if (clip != -1) {
        ++anim->m_useCount;   // +0x24
        anim->unk_2c += 1;    // +0x2c
        Animation* previous = m_currentAnimation;
        m_currentAnimation = anim;  // +0x98
        if (previous != nullptr) {
            --previous->m_useCount;  // +0x24 of the clip that was current
        }
        // TODO(base): glitch::collada::CSceneNodeAnimatorSet::setCurrentAnimation(this, clip) (engine base, result discarded).
    }
    return clip;
}

// 0x367510 _ZN11AnimatorSet12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
// PARTIAL: the event callback fields and the receiver object are base fields that are not declared. The timeline
// hookup (vtable +0x44) is not reachable either. Only the AnimApplicator callback is set.
void AnimatorSet::SetCallbacks(AnimCallback animCallback, void* animUserData,
                               EventCallback eventCallback, void* eventUserData) {
    // TODO(layout): this +0x18 receiver (when non-null: receiver +0x08 = eventCallback, +0x0c = eventUserData),
    //               this +0x1c = eventCallback, this +0x20 = eventUserData.
    // TODO(timeline): t = this->getTimelineCtrl() (vtable +0x44); if (t) { t +0x08 = &AnimatorSet::_CBAnim; t +0x0c = this; }
    (void)eventCallback;
    (void)eventUserData;
    m_applicator.SetCallback(animCallback, animUserData);  // 0x364400
}

// 0x367590 _ZN11AnimatorSetD1Ev
// PARTIAL: the release of the current clip (--Animation +0x24 when m_currentAnimation is set) and the base destructor
// are not reachable. The member destructors (m_animationSet release, AnimApplicator) run after the body, in the
// same order as the original.
AnimatorSet::~AnimatorSet() {
    // TODO(layout): if (m_currentAnimation) --m_currentAnimation +0x24 before the members are destroyed.
    // The base destructor (~glitch::collada::CSceneNodeAnimatorSet) runs after this body, implicitly.
}

// 0x36761c _ZN11AnimatorSetD0Ev
// SKIPPED: the scalar deleting destructor. It is the destructor followed by CustomFree(this) (core-platform
// memory.hpp, 0x310440). The C++ delete expression produces it, and this tree declares no class operator delete.

// 0x367638 _ZN11AnimatorSetD2Ev
// PARTIAL: the base-object destructor form with a VTT argument. Its work is the destructor body above (0x367590)
// minus the secondary vptr store at +0x9c. Not rebuilt separately.

// 0x3676b8 _ZN11AnimatorSetC1ERKN5boost13intrusive_ptrI12AnimationSetEE
// PARTIAL: the base is default constructed. The original base constructor takes the engine set (AnimationSet +0x20,
// a CDynamicAnimationSet, which is a CAnimationSet) and calls init on it, and init dereferences that set, so the
// engine set is not bound here yet. AnimApplicator is built with this as its animator, as in the original.
AnimatorSet::AnimatorSet(const boost::intrusive_ptr<AnimationSet>& animationSet)
    : glitch::collada::CSceneNodeAnimatorSet(),
      m_applicator(this),
      m_animationSet(animationSet),
      m_currentAnimation(nullptr) {
    // TODO(base): glitch::collada::CSceneNodeAnimatorSet(engine set) with the addref before and the drop after, as at
    // 0x3676b8 (asm: BL to the base C2, r1 = VTT + 4). Needs the engine set from AnimationSet +0x20 (m_animSet).
}

// 0x367790 _ZN11AnimatorSetC2ERKN5boost13intrusive_ptrI12AnimationSetEE
// PARTIAL: the base-object constructor form with a VTT argument (asm-level detail: it does not initialise the secondary
// sub-object at +0x9c and +0xa0). Not rebuilt separately; the source constructor is the one above (0x3676b8).
