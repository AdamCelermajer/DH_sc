// AnimController, part 1 of 2 (batch g1-game-anim-AnimController-83ef73-0), rebuilt from libDungeonHunter2.so.
//
// The complete-object constructor 0x474d30 has the same body as 0x474e44. The part-2 batch defines that body in
// game-anim/src/g1-game-anim-AnimController-83ef73-1.cpp, so it is not defined again here.
//
// Status summary: reconstructed for the two do-nothing callbacks and the SetCallbacks thunk. Partial for the rest.
// Blockers, after the redo pass (dh2/RootSceneNode.hpp now exists):
//   - glitch::collada::ISceneNodeAnimator (glitch-scene-collada-gui/collada_misc.hpp) declares none of the timeline
//     and clip slots. The canonical glitch::scene::ISceneNodeAnimator (scene/ISceneNodeAnimator.hpp) has
//     getTimelineCtrl (+0x40 const, +0x44), setAnimationClip(int) (+0x30) and getAnimationClipIndex (+0x38), but the
//     collada class does not derive from it. GetApplicator takes the scene type.
//   - RootSceneNode derives from glitch::collada::CRootSceneNode, which has no ISceneNode base. The animator list at
//     ISceneNode +0xfc (getAnimators) is therefore not reachable from m_node.
// Partial bodies keep the decoded control flow in comments and stub the calls they cannot make yet.

#include "dh2/AnimController.hpp"

#include <cstdlib>

// Static data member, initialised from the .data byte at 0x999841 (value 1, read with IDA get_global_value).
// Written by Script_EnterCutSceneMode::Execute (0x45a384) and Script_ExitCutSceneMode::Execute (0x4599ac);
// read by AnimController::SetScale and BlendedAnimSetController::SetScale.
bool AnimController::s_scalingEnabled = true;

// 0x474678 _ZN14AnimController15GetAnimationSetEv
// partial: the original stores 0 into the hidden return slot, so the result is an empty intrusive_ptr. That value
// cannot be produced here. glitch::collada::CAnimationSet is incomplete in every declared header and has no
// reference-counted base (the recon stand-in is in a recon namespace), and boost::intrusive_ptr<T> needs T complete
// for its destructor, which every return of the type instantiates. The body therefore aborts, so no caller gets a
// wrong value silently. A definition must still exist because the vtable of this class is emitted in this file.
// Replace the body with the empty return once CAnimationSet is declared with an IReferenceCounted base.
boost::intrusive_ptr<glitch::collada::CAnimationSet> AnimController::GetAnimationSet() {
    std::abort();
}

// 0x474684 _ZN14AnimControllerD2Ev
// 0x4746c8 _ZN14AnimControllerD1Ev
// Both bodies are identical. The original resets the vptr (the compiler does this) and then drops m_node through
// the glitch::IReferenceCounted subobject of the node. That subobject is RootSceneNode's second one: its offset 0x20c
// is read from the node's vtable at vptr-12, and its count is RootSceneNode::m_secondaryRefCount (+0x210).
// partial: the drop is not written. Missing: the count decrement and, at zero, the onDelete call and the deleting
// destructor that IReferenceCounted::drop makes through the subobject's vtable (+0x08, +0x04). dh2/RootSceneNode.hpp
// does not declare that vtable. The node is left referenced until this is written.
AnimController::~AnimController() {}

// 0x47470c _ZN14AnimController17_CBAnim_DoNothingEPN6glitch5scene19ITimelineControllerEPv
void AnimController::_CBAnim_DoNothing(glitch::scene::ITimelineController*, void*) {}

// 0x474710 _ZN14AnimController18_CBEvent_DoNothingERKN6glitch7collada15STriggeredEventEPv
void AnimController::_CBEvent_DoNothing(const glitch::collada::STriggeredEvent&, void*) {}

// 0x474734 _ZNK14AnimController10GetNumAnimEv
// partial: the original counts the animator list of m_node (ISceneNode +0xfc, intrusive list). Waits on
// dh2/RootSceneNode.hpp and on a host-width list model for ISceneNode::getAnimators.
int AnimController::GetNumAnim() const {
    return 0;
}

// 0x474770 _ZNK14AnimController7GetAnimEj
// partial: the index check is rebuilt. The original returns null when index >= GetNumAnim() (virtual slot +0x0c). The
// element walk is not written: the original returns the element at that index of m_node's animator list (ISceneNode
// +0xfc, intrusive list; the element's value is at +0x08). That list is not reachable from RootSceneNode (no ISceneNode
// base in dh2/RootSceneNode.hpp's base). GetNumAnim is still a stub (0x474734), so this returns null for every index.
// The body at 0x4748b8 is identical; it is the non-const overload.
const glitch::collada::ISceneNodeAnimator* AnimController::GetAnim(unsigned int index) const {
    if (static_cast<unsigned int>(GetNumAnim()) <= index) {
        return nullptr;
    }
    // Element walk not written (see the note above).
    return nullptr;
}

// 0x4747d8 _ZNK14AnimController15GetClipDurationEj
// partial: the original takes anim = GetAnim(index). If anim and anim->getTimelineCtrl() const (slot +0x40) are
// both non-null, it returns the timeline's getCurrentClipLength() const (slot +0x34). Otherwise it returns -1.
// Waits on dh2/RootSceneNode.hpp, the list model, and the collada animator and ITimelineController declarations.
int AnimController::GetClipDuration(unsigned int index) const {
    return -1;
}

// 0x474814 _ZNK14AnimController7HasClipEPKcj
// partial: stub returns false. The original takes anim = GetAnim(index) (const) and tc = anim->getTimelineCtrl() const
// (slot +0x40). When tc is non-null and tc->getClipCount() const (ITimelineController +0x1c, declared) is above 0, it
// returns tc->getClipIndex(clipName) const (+0x18, declared) != -1. Otherwise false. Blocked on the collada animator:
// glitch::collada::ISceneNodeAnimator does not declare getTimelineCtrl (see the file header), and GetAnim is partial.
bool AnimController::HasClip(const char* clipName, unsigned int index) const {
    return false;
}

// 0x47487c _ZNK14AnimController10GetNumClipEj
// partial: stub returns 0. The original returns tc->getClipCount() const (ITimelineController +0x1c) for
// tc = GetAnim(index)->getTimelineCtrl() const (collada animator +0x40), or 0 when either is missing. The timeline call
// is blocked on the collada animator declaration (see the file header).
int AnimController::GetNumClip(unsigned int index) const {
    return 0;
}

// 0x4748b8 _ZN14AnimController7GetAnimEj
// partial: the index check is rebuilt, the element walk is not (see 0x474770). Non-const, so its result is not const.
glitch::collada::ISceneNodeAnimator* AnimController::GetAnim(unsigned int index) {
    if (static_cast<unsigned int>(GetNumAnim()) <= index) {
        return nullptr;
    }
    // Element walk not written (see 0x474770).
    return nullptr;
}

// 0x474920 _ZN14AnimController8SetScaleEfj
// partial: the scaling gate is rebuilt. When s_scalingEnabled is clear the function returns. Otherwise the original
// takes anim = GetAnim(index), tc = anim->getTimelineCtrl() (collada animator +0x44, not declared) and calls
// tc->setScale(scale) (ITimelineController +0x48, declared). Blocked on the collada animator declaration and on GetAnim.
void AnimController::SetScale(float scale, unsigned int animIndex) {
    if (!s_scalingEnabled) {
        return;
    }
}

// 0x474988 _ZN14AnimController8StopClipEbj
// partial: stub does nothing. The original takes anim = GetAnim(index) and tc = anim->getTimelineCtrl() (+0x44). When tc
// is non-null it calls tc->setScale(0) (+0x48). When rewind is set it calls tc->jumpTo(tc->getCurrentClipEnd()) (jumpTo
// +0x0c, getCurrentClipEnd +0x30; both declared in ITimelineController). Blocked on the collada animator's getTimelineCtrl.
void AnimController::StopClip(bool rewind, unsigned int animIndex) {}

// 0x4749f4 _ZN14AnimController8PlayClipEPKcbij
// partial: the original takes anim = GetAnim(index) and applicator = GetApplicator(anim), then:
//   - anim null: GetApplicator(null) and return false
//   - tc = anim->getTimelineCtrl() (slot +0x44); tc non-null, count above 0 and getClipIndex(name) == -1: return false
//   - v12 = tc->getCurrentClipIndex() (slot +0x38); v13 = anim->getAnimationClipIndex(name) (slot +0x38);
//     anim->setAnimationClip(v13) (slot +0x30)
//   - if v12 == v13 and !tc->getLoop() (slot +0x44): tc->jumpTo(applicator offset + tc +0x10) (slot +0x0c)
//   - tc->setLoop(loop) (slot +0x40); tc->setScale(1.0f) (slot +0x48)
//   - m_node->NewAnim(false); m_node flags |= 0x200 (ISceneNode flags, +0x11c); return true
// Waits on dh2/RootSceneNode.hpp, GetApplicator (no declaration found in the repository), and the declarations above.
bool AnimController::PlayClip(const char* clipName, bool loop, int unused, unsigned int animIndex) {
    return false;
}

// 0x474b50 _ZN14AnimController8PlayClipEjbij
// partial: stub returns false. Same flow as the named overload, with the clip given as an index. The original:
//   anim = GetAnim(animIndex) (non-const). Null: GetApplicator(0) and return false.
//   applicator = GetApplicator(anim) (dh2/global/GetApplicator.hpp, declared). tc = anim->getTimelineCtrl() (+0x44).
//   If tc is non-null and tc->getClipCount() (+0x1c) <= clip: return false. The original dereferences a null tc here.
//   v12 = tc->getCurrentClipIndex() (+0x38). anim->setAnimationClip(clip) (+0x30, the index overload; not the name lookup).
//   If v12 == clip and !tc->getLoop() (+0x44): tc->jumpTo(applicator ? applicator->unk_10 : 0, plus tc->rangeStartMs)
//     (jumpTo +0x0c; unk_10 is AnimApplicator +0x10, dh2/AnimApplicator.hpp).
//   tc->setLoop(loop) (+0x40); tc->setScale(1.0f) (+0x48).
//   m_node->NewAnim(false) (declared in dh2/RootSceneNode.hpp); m_node->unk_011c |= 0x200; return true.
// Blocked on the collada animator: getTimelineCtrl (+0x44) and setAnimationClip (+0x30) are not declared on
// glitch::collada::ISceneNodeAnimator, and GetApplicator takes the glitch::scene type.
bool AnimController::PlayClip(unsigned int clip, bool loop, int unused, unsigned int animIndex) {
    return false;
}

// 0x474c78 _ZN14AnimController12SetCallbacksEPN6glitch7collada18ISceneNodeAnimatorEPFvPNS0_5scene19ITimelineControllerEPvES7_PFvRKNS1_15STriggeredEventES7_ES7_
// partial: when the animator is non-null the original calls Animator::SetCallbacks on the same pointer, with no
// base adjustment in the pseudocode. Animator.hpp does not declare its base (glitch::collada::CSceneNodeAnimator), so
// the downcast is not written and the call is stubbed.
void AnimController::SetCallbacks(glitch::collada::ISceneNodeAnimator* animator, AnimCallback animCallback,
                                  void* animUserData, EventCallback eventCallback, void* eventUserData) {
    if (animator == nullptr) {
        return;
    }
}

// 0x474cac _ZN14AnimController17SetCallbacksOnAllEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
// partial: stub does nothing. The original walks m_node's animator list (ISceneNode +0xfc, element value at +0x08, see
// GetAnim) and calls the per-animator SetCallbacks (0x474c78) with each element. The list is not reachable from
// RootSceneNode (no ISceneNode base), and the per-animator SetCallbacks is partial, so the walk is not written.
void AnimController::SetCallbacksOnAll(AnimCallback animCallback, void* animUserData, EventCallback eventCallback,
                                       void* eventUserData) {}

// 0x474d10 _ZN14AnimController12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
// Thunk: the original is a single branch to SetCallbacksOnAll (0x474cac). It is forwarded, not reimplemented.
void AnimController::SetCallbacks(AnimCallback animCallback, void* animUserData, EventCallback eventCallback,
                                  void* eventUserData) {
    SetCallbacksOnAll(animCallback, animUserData, eventCallback, eventUserData);
}

// 0x474d14 _ZN14AnimControllerD0Ev is the compiler-generated deleting destructor (D1, then CustomFree of this).
// Not written here. It is skipped and the compiler produces it from the virtual destructor above.

// 0x474d30 _ZN14AnimControllerC1EP13RootSceneNodeb is defined in game-anim/src/g1-game-anim-AnimController-83ef73-1.cpp
// together with its C2 twin 0x474e44. Not defined again here.
