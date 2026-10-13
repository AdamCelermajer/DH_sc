// Batch g1-game-anim-AnimSetController-be4322-0 (game-anim, owner AnimSetController), rebuilt from libDungeonHunter2.so.
//
// AnimSetController plays the clips of one animation set on a root scene node. Its animator is read through
// AnimController::GetAnim, which is partial in g1-game-anim-AnimController-83ef73-0.cpp (it returns null until the
// RootSceneNode animator list is modelled). Every body that reaches the animator through GetAnim is therefore partial
// here, and so is the constructor, which needs Singleton<AnimSetManager>::s_inst (no declaration in the repository).
//
// Notes on the layouts used below (asm checked):
//   - The animator returned by AnimController::GetAnim is the AnimatorSet built by AnimSetManager::GetAnimator (0xa4
//     bytes). AnimatorSet is not derived from glitch::collada::ISceneNodeAnimator in its header, so the cast is a
//     reinterpret_cast, as the original's direct call of AnimatorSet::SetCallbacks on that pointer.
//   - ISceneNode::addAnimator is the vtable slot at vptr+0x6c (decomp_vtable lists it at +0x78 from the vtable base).

#include "dh2/AnimSetController.hpp"

#include <cstdlib>

#include "dh2/AnimatorSet.hpp"

// 0x474f58 _ZNK17AnimSetController7HasClipEPKcj
// Stub in the original: returns false (asm: MOV R0,#0; BX LR). The clip name and index are not read.
bool AnimSetController::HasClip(const char* /*clipName*/, unsigned int /*animIndex*/) const {
    return false;
}

// 0x474f60 _ZNK17AnimSetController15GetClipDurationEj
// Stub in the original: returns -1 (asm: MVN R0,#0; BX LR).
int AnimSetController::GetClipDuration(unsigned int /*animIndex*/) const {
    return -1;
}

// 0x474f68 _ZN17AnimSetController8PlayClipEPKcbij
// Stub in the original: returns false (asm: MOV R0,#0; BX LR).
bool AnimSetController::PlayClip(const char* /*clipName*/, bool /*loop*/, int /*unused*/,
                                 unsigned int /*animIndex*/) {
    return false;
}

// 0x474f70 _ZN17AnimSetController15GetAnimationSetEv
// partial: the original calls AnimController::GetAnim(0) (non-const, partial: null in this tree). For a null animator it
// stores an empty intrusive_ptr in the hidden return slot. For a non-null one it stores the AnimatorSet's animation set
// (AnimatorSet +0x94, a game AnimationSet) and adds a reference through its IReferenceCounted count (+0x04).
// The result cannot be produced here: the declared return type is boost::intrusive_ptr<glitch::collada::CAnimationSet>,
// and CAnimationSet has no reference count in any declared header, while the stored object is the game AnimationSet.
// The same decision as AnimController::GetAnimationSet (0x474678): the body aborts, so no caller gets a wrong value
// silently. Waits on the return-type decision in the layout requests.
boost::intrusive_ptr<glitch::collada::CAnimationSet> AnimSetController::GetAnimationSet() {
    std::abort();
}

// 0x474fb0 _ZN17AnimSetController12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
// partial: the body is complete (asm checked: a tail call to AnimatorSet::SetCallbacks with the four callback arguments
// when GetAnim(0) is non-null, nothing otherwise). Its effect waits on AnimController::GetAnim (partial, null in this
// tree, so the call does nothing yet) and on AnimatorSet::SetCallbacks (partial: only the AnimApplicator callback is set).
void AnimSetController::SetCallbacks(AnimCallback animCallback, void* animUserData, EventCallback eventCallback,
                                     void* eventUserData) {
    glitch::collada::ISceneNodeAnimator* animator = GetAnim(0);
    if (animator != nullptr) {
        reinterpret_cast<AnimatorSet*>(animator)->SetCallbacks(animCallback, animUserData, eventCallback,
                                                               eventUserData);
    }
}

// 0x474ff0 _ZN17AnimSetController8PlayClipEjbij
// partial, body not rebuilt (returns false). The flow, decoded from the asm (0x474ff0 .. 0x4750d8):
//   1. clip == -1: return 0.
//   2. animator = GetAnim(animIndex) (non-const). Null: GetApplicator(null), return 0. The original also calls
//      CSceneNodeAnimatorSet::getCurrentAnimation on the null pointer (a load of +0x50, result discarded); that load is
//      not reproduced.
//   3. timeline = animator->getTimelineCtrl() (vptr+0x44). cur = CSceneNodeAnimatorSet::getCurrentAnimation (+0x50).
//      applicator = GetApplicator(animator). index = AnimatorSet::SetCurrentAnimation(animator, clip). index == -1: return 0.
//   4. If the timeline word at +0x34 is nonzero (CTimelineController field, not declared; see the layout requests),
//      animator->setAnimationClip(unk_0c) (vptr+0x30).
//   5. If cur == index and !timeline->getLoop() (vptr+0x44): offset = applicator ? applicator->unk_10 : 0;
//      timeline->jumpTo(offset + timeline->rangeStartMs) (vptr+0x0c).
//   6. timeline->setLoop(loop) (vptr+0x40); timeline->setScale(1.0f) (vptr+0x48).
//   7. m_node->NewAnim(unk_10); return 1.
// Waits on: the +0x34 field, AnimController::GetAnim (partial), AnimatorSet::SetCurrentAnimation (a stub that returns 0,
// partial), and the GetApplicator body (partial).
bool AnimSetController::PlayClip(unsigned int /*clip*/, bool /*loop*/, int /*unused*/, unsigned int /*animIndex*/) {
    return false;
}

// 0x475114 _ZNK17AnimSetController10GetNumClipEj
// partial: the original calls AnimController::GetAnim(index) (const overload, 0x474770; partial, null in this tree). For a
// null animator it returns 0. For a non-null one it returns the result of
// glitch::collada::CSceneNodeAnimatorSet::getAnimationCount() const (_ZNK6glitch7collada21CSceneNodeAnimatorSet17getAnimationCountEv),
// which collada_scene_accessors.hpp does not declare yet. Until it does, the non-null case returns 0.
int AnimSetController::GetNumClip(unsigned int animIndex) const {
    const glitch::collada::ISceneNodeAnimator* animator = GetAnim(animIndex);
    if (animator == nullptr) {
        return 0;
    }
    // TODO(declaration): return reinterpret_cast<const glitch::collada::CSceneNodeAnimatorSet*>(animator)
    //                        ->getAnimationCount();  (not declared yet)
    return 0;
}

// 0x475130 _ZN17AnimSetControllerD1Ev
// 0x475180 _ZN17AnimSetControllerD2Ev
// Both bodies are identical (asm checked): the vptr is reset (the compiler does this) and then the base destructor
// AnimController::~AnimController (0x474684) runs. Nothing else is written here. The D2 body is the same code, so it has
// no body of its own.
AnimSetController::~AnimSetController() {}

// 0x475164 _ZN17AnimSetControllerD0Ev
// SKIPPED: the scalar deleting destructor. It calls the destructor above and then CustomFree(this) (asm checked). The
// C++ delete expression produces it, as for AnimatorSet (0x36761c).

// 0x4751b4 _ZN17AnimSetControllerC1EP13RootSceneNodei
// 0x475248 _ZN17AnimSetControllerC2EP13RootSceneNodei
// partial: the base constructor (AnimController(node, true)), the set id and the two fields (unk_0c = 0, unk_10 = true)
// are rebuilt, in the original's order. The animator is not attached: the original calls
// AnimSetManager::GetAnimator(animSetId) on Singleton<AnimSetManager>::s_inst, and that instance has no declaration in the
// repository (layout request). When an animator comes back, the original calls m_node->addAnimator(animator)
// (ISceneNode vptr+0x6c) and then drops the animator's glitch::IReferenceCounted base. The controller keeps no
// reference to the animator. The C2 body is the same code as C1.
AnimSetController::AnimSetController(RootSceneNode* node, int animSetId)
    : AnimController(node, true), m_animSetId(animSetId), unk_0c(0), unk_10(true) {
    // TODO(singleton): animator = Singleton<AnimSetManager>::s_inst.GetAnimator(m_animSetId);
    //                  if (animator != nullptr) { m_node->addAnimator(animator); animator's IReferenceCounted::drop(); }
}
