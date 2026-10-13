// Batch g1-game-anim-Animator-52d554-0: the Animator class (game-anim). Declarations come from dh2/Animator.hpp.
//
// The engine base glitch::collada::CSceneNodeAnimator is not inherited by Animator (see the note in
// dh2/Animator.hpp: the base is a byte sub-object until glitch-scene-collada-gui's layout is unified). So
// every call into the base, and the virtual getTimelineCtrl (vtable +0x44) that comes from it, is missing
// and the functions that make one are marked partial below.
//
// Compiler-generated variants are not written here: 0x366418 (deleting dtor), 0x366434 (base-object dtor
// with VTT) and 0x366518 (base-object ctor with VTT). Their bodies are the D1 and C1 rows.

#include "dh2/Animator.hpp"

namespace animator_detail {

// 0x366224: the original multiplies the float by 1000.0f in single precision (__aeabi_fmul, constant
// 0x447a0000) and truncates toward zero (__aeabi_f2iz).
int SecondsToMs(float seconds) {
    return static_cast<int>(seconds * 1000.0f);
}

// 0x366224 arithmetic on the two values read from the timeline:
//   stepMs      = SecondsToMs(timeline.lastStep)           (timeline +0x1c)
//   remainingMs = SecondsToMs(timeline.currentSec) - timeline.currentTimeMs   (timeline +0x2c, +0x04)
// The result is stepMs - remainingMs when 0 <= remainingMs < stepMs, and 0 otherwise.
int ClipTimeLeftMs(int stepMs, int remainingMs) {
    bool inside = remainingMs < stepMs;
    if (remainingMs < 0) {
        inside = false;
    }
    return inside ? stepMs - remainingMs : 0;
}

}  // namespace animator_detail

// 0x36621c _ZNK8Animator7getTypeEv
int Animator::getType() const {
    return 11;
}

// 0x366224 _ZN8Animator17_HandleAnimEndingEPN6glitch5scene19ITimelineControllerE
// partial: with a non-null timeline the original reads the timeline's fields (+0x04 int current time in
// ms, +0x1c float step, +0x2c float position in seconds) and stores
// animator_detail::ClipTimeLeftMs(...) into m_applicator.unk_10 (+0x68). glitch::scene::ITimelineController
// has no declared layout, so those reads and the unk_10 store are missing here (see the layout requests).
// The callback flag is set on every path, as in the original.
void Animator::_HandleAnimEnding(glitch::scene::ITimelineController* timeline) {
    (void)timeline;
    m_applicator.m_callbackPending = true;  // +0x88
}

// 0x366294 _ZN8Animator7_CBAnimEPN6glitch5scene19ITimelineControllerEPv
// The original swaps its two register arguments and tail-calls _HandleAnimEnding with userData as the
// Animator, so the user data written by SetCallbacks (the Animator itself) comes in as userData.
void Animator::_CBAnim(glitch::scene::ITimelineController* timeline, void* userData) {
    static_cast<Animator*>(userData)->_HandleAnimEnding(timeline);
}

// 0x3662c8 _ZN8Animator11animateNodeEPN6glitch5scene10ISceneNodeEj
// partial: the original first calls the base glitch::collada::CSceneNodeAnimator::animateNode(this, node,
// time), which is not declared in any header and cannot be called without the base. Then it calls
// AnimApplicator::AnimateNode (done here), fetches the timeline through the virtual getTimelineCtrl
// (vtable +0x44, from the base) and ends with AnimApplicator::CheckCallback(timeline) (missing).
void Animator::animateNode(glitch::scene::ISceneNode* node, unsigned int time) {
    (void)node;
    m_applicator.AnimApplicator::AnimateNode(time);
}

// 0x366308 _ZN8Animator8onUnbindEPN6glitch5scene10ISceneNodeE
// partial: the original clears the applicator's reference node, then tail-calls the base
// glitch::collada::ISceneNodeAnimator::onUnbind(this, node) (0x667d9c). The base is not inherited, so that
// tail call is missing.
void Animator::onUnbind(glitch::scene::ISceneNode* node) {
    m_applicator.AnimApplicator::SetRefNode(nullptr);
    (void)node;
}

// 0x366330 _ZN8Animator12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
// partial: the original does three things and this stores only the last one.
//   (1) It stores eventCallback and eventUserData in the engine base at +0x1c and +0x20, and the events
//       track (base +0x18) gets the same pair in its own callback slots (+0x08 and +0x0c). The base
//       sub-object is not inherited, so these stores are missing.
//   (2) It fetches the timeline through the virtual getTimelineCtrl (vtable +0x44) and writes _CBAnim
//       (+0x08) and this (+0x0c) into it. Missing for the same reason as (1).
//   (3) It calls AnimApplicator::SetCallback(animCallback, animUserData). Done here.
void Animator::SetCallbacks(AnimCallback animCallback, void* animUserData,
                            EventCallback eventCallback, void* eventUserData) {
    (void)eventCallback;
    (void)eventUserData;
    m_applicator.AnimApplicator::SetCallback(animCallback, animUserData);
}

// 0x3663b0 _ZN8AnimatorD1Ev
// partial: the original writes the three vptrs, runs the AnimApplicator destructor (the member destructor
// here, done implicitly) and then the base glitch::collada::CSceneNodeAnimator::~CSceneNodeAnimator (D2),
// which needs the base and is missing.
Animator::~Animator() {}

// 0x366490 _ZN8AnimatorC1ERKN6glitch7collada16CColladaDatabaseERNS1_22SLibraryAnimationClipsE
// partial: the original calls the base constructor CSceneNodeAnimator(database, clips) through the VTT
// (missing: the base is not inherited). The IReferenceCounted at +0x94 starts with count 1 here as in the
// original (+0x98 = 1). m_applicator takes this as its owner, as in the original; the reinterpret_cast is
// the stand-in for the base relationship (the base sub-object is at +0x00, so the pointer is the same) and
// goes away when the base is inherited.
Animator::Animator(const glitch::collada::CColladaDatabase& database,
                   glitch::collada::SLibraryAnimationClips& clips)
    : m_applicator(reinterpret_cast<glitch::collada::ISceneNodeAnimator*>(this)) {
    (void)database;
    (void)clips;
}
