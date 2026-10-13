// Batch g1-game-anim-BlendedAnimSetController-bd761d-0 (game-anim, owner BlendedAnimSetController), rebuilt from the
// Hex-Rays pseudocode of libDungeonHunter2.so (0x476680 to 0x476ddc).
//
// Status summary. Reconstructed: HasClip, GetClipDuration, PlayClip(name) (constant returns), SetCallbacks and SetScale
// (forward to the blender at GetAnim), and the destructor, whose D1 and D2 copies share one source definition. Skipped:
// the deleting destructor. Partial: GetAnimationSet, StopClip, PlayClip(index), GetNumClip and the constructor. Their
// missing pieces are marked in the function comments and are listed in the batch report as layout requests.
//
// The children of the blender are typed glitch::collada::ISceneNodeAnimator, the same type AnimSetManager::GetAnimator
// returns. The dh2 AnimatorSet does not derive from the engine's CSceneNodeAnimatorSet, and the collada animator does not
// declare getTimelineCtrl (vtable +0x44), so every call on a child beyond the animator interface stays partial.

#include "dh2/BlendedAnimSetController.hpp"

#include <cstddef>
#include <cstdlib>

#include "dh2/AnimatorBlender.hpp"

// 0x476680 _ZNK24BlendedAnimSetController7HasClipEPKcj
// reconstructed: the original returns 0 without reading its arguments.
bool BlendedAnimSetController::HasClip(const char* /*clipName*/, unsigned int /*animIndex*/) const {
    return false;
}

// 0x476688 _ZNK24BlendedAnimSetController15GetClipDurationEj
// reconstructed: the original returns -1 without reading its argument.
int BlendedAnimSetController::GetClipDuration(unsigned int /*animIndex*/) const {
    return -1;
}

// 0x476690 _ZN24BlendedAnimSetController8PlayClipEPKcbij
// reconstructed: the original returns 0 (false) without reading its arguments.
bool BlendedAnimSetController::PlayClip(const char* /*clipName*/, bool /*loop*/, int /*unused*/,
                                        unsigned int /*animIndex*/) {
    return false;
}

// 0x476698 _ZN24BlendedAnimSetController15GetAnimationSetEv
// partial. Every path aborts, as AnimController::GetAnimationSet does. The original returns the current child's animation
// set when there is a blender and a child (blender +0x28 with index +0x70, then the child's counted pointer at +0x94), and
// an empty result otherwise. No value can be produced here, and the empty result cannot be built either:
// boost::intrusive_ptr<glitch::collada::CAnimationSet> instantiates its destructor, which needs the complete type, and
// CAnimationSet is incomplete in every declared header. The dh2 AnimatorSet field at +0x94 is intrusive_ptr<AnimationSet>,
// a different class, so the return type needs an owner decision (layout request). Replace the abort with the empty return
// and the child lookup once CAnimationSet is declared as counted.
boost::intrusive_ptr<glitch::collada::CAnimationSet> BlendedAnimSetController::GetAnimationSet() {
    std::abort();
}

// 0x4766fc _ZN24BlendedAnimSetController12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
// reconstructed: forwards the four arguments to the blender at GetAnim(0), when there is one. With no blender the
// original returns the null result and makes no call. The callee AnimatorBlender::SetCallbacks (0x366eb8) is partial in
// its own row (its per-child loop is not written); this forward is complete.
void BlendedAnimSetController::SetCallbacks(AnimCallback animCallback, void* animUserData,
                                            EventCallback eventCallback, void* eventUserData) {
    glitch::collada::ISceneNodeAnimator* anim = GetAnim(0);
    if (anim != nullptr) {
        static_cast<AnimatorBlender*>(anim)->SetCallbacks(animCallback, animUserData, eventCallback, eventUserData);
    }
}

// 0x47673c _ZN24BlendedAnimSetController8SetScaleEfj
// reconstructed: gated by AnimController::s_scalingEnabled, as in the original. The blender at GetAnim(animIndex) gets
// SetScale(scale). The original makes this call without a null test on the blender, so the same is done here. The callee
// AnimatorBlender::SetScale (0x3666d8) is partial in its own row (its per-child loop is not written).
void BlendedAnimSetController::SetScale(float scale, unsigned int animIndex) {
    if (!AnimController::s_scalingEnabled) {
        return;
    }
    static_cast<AnimatorBlender*>(GetAnim(animIndex))->SetScale(scale);
}

// 0x476780 _ZN24BlendedAnimSetController8StopClipEbj
// partial. Reproduced: the blender at GetAnim(animIndex), the current child at animators[m_currentIndex], and SetScale(0)
// on the blender in the original order. With no child the original stops there (SetScale only).
// Not reproduced, when a child exists: the timeline is child->getTimelineCtrl() (child vtable +0x44). That slot is not
// declared on glitch::collada::ISceneNodeAnimator, so the timeline cannot be reached. The original then runs, when rewind
// is set and the timeline is non-null, timeline->jumpTo(timeline->getCurrentClipEnd()) (vtable +0x0c, then +0x30).
void BlendedAnimSetController::StopClip(bool rewind, unsigned int animIndex) {
    AnimatorBlender* blender = static_cast<AnimatorBlender*>(GetAnim(animIndex));
    // The original reads the blender without a null test, so a null blender faults there as well.
    glitch::collada::ISceneNodeAnimator* child =
        blender->animators[static_cast<std::size_t>(blender->m_currentIndex)];
    if (child == nullptr) {
        blender->SetScale(0.0f);
        return;
    }
    // partial: timeline = child->getTimelineCtrl() (vtable +0x44) is taken here, before SetScale in the original.
    blender->SetScale(0.0f);
    // partial: if (timeline != nullptr && rewind) timeline->jumpTo(timeline->getCurrentClipEnd()); not reproduced.
    (void)rewind;
}

// 0x47680c _ZN24BlendedAnimSetController8PlayClipEjbij
// partial. Reproduced: the blender lookup (null returns false), the cross-fade start AnimatorBlender::Blend(unk_14), and
// the early return when the clip index is -1 (the original tests a2 == -1).
// Not reproduced, after that test. The original then takes the child at animators[m_currentIndex]. Without a child it
// calls CSceneNodeAnimatorSet::getCurrentAnimation on the null child, calls GetApplicator(blender) and returns false.
// With a child it needs the child's timeline (vtable +0x44), the child's getCurrentAnimation (engine CSceneNodeAnimatorSet,
// 0x65f114), AnimatorSet::SetCurrentAnimation(clip) (dh2, 0x3674ac), the child's setAnimationClip(unk_0c) (vtable +0x30),
// the timeline flag at +0x34 (not declared), timeline setLoop (+0x40), getLoop (+0x44), setScale(1.0f) (+0x48) and
// jumpTo(GetApplicator(blender)->unk_10 + timeline->rangeStartMs) (+0x0c). It ends with RootSceneNode::NewAnim(unk_10) and
// AnimatorBlender::BlendPost. None of that is written, so the body returns false after the parts above.
bool BlendedAnimSetController::PlayClip(unsigned int clip, bool loop, int unused, unsigned int animIndex) {
    AnimatorBlender* blender = static_cast<AnimatorBlender*>(GetAnim(animIndex));
    if (blender == nullptr) {
        return false;
    }
    blender->Blend(unk_14);
    if (clip == 0xffffffffu) {  // the original tests a2 == -1
        return false;
    }
    (void)loop;
    (void)unused;
    return false;  // partial: see above
}

// 0x47695c _ZNK24BlendedAnimSetController10GetNumClipEj
// partial. Reproduced: the blender lookup, the current child, and the zero results (no blender, or no child).
// Not reproduced: with a child, the original returns CSceneNodeAnimatorSet::getAnimationCount() const (engine, 0x65f11c)
// on that child. The child type does not declare it, so the stub returns 0 for a child that exists.
int BlendedAnimSetController::GetNumClip(unsigned int animIndex) const {
    const AnimatorBlender* blender = static_cast<const AnimatorBlender*>(GetAnim(animIndex));
    if (blender == nullptr) {
        return 0;
    }
    const glitch::collada::ISceneNodeAnimator* child =
        blender->animators[static_cast<std::size_t>(blender->m_currentIndex)];
    if (child == nullptr) {
        return 0;
    }
    // partial: the child's getAnimationCount() (0x65f11c) is not reachable from the child type.
    return 0;
}

// 0x476990 _ZN24BlendedAnimSetControllerD1Ev
// 0x4769e0 _ZN24BlendedAnimSetControllerD2Ev
// Both copies have the same body, so one source definition covers both (the class has no virtual base, so the complete and
// base-object destructors are the same). The original resets the vptr (the compiler does this) and calls the base
// destructor AnimController::~AnimController (0x474684). This body is complete. The base destructor is partial in its own
// row (it does not yet drop m_node), so this destructor inherits that gap.
BlendedAnimSetController::~BlendedAnimSetController() {}

// 0x4769c4 _ZN24BlendedAnimSetControllerD0Ev
// skipped: compiler-generated deleting destructor. The original is the destructor at 0x476990 followed by CustomFree(this).
// A C++ delete expression produces the same shape, so no body is written here.

// 0x476b4c _ZN24BlendedAnimSetControllerC2EP13RootSceneNodei
// 0x476ddc _ZN24BlendedAnimSetControllerC1EP13RootSceneNodei
// partial. The two copies have the same body; this definition serves both. Reproduced: the AnimController base constructor
// with trackEvents set (the original passes 1), and the field stores in declaration order: m_animSetId (+0x08) = the set id,
// unk_0c (+0x0c) = 0, unk_10 (+0x10) = 1, unk_14 (+0x14) = 0.
// Not reproduced, all of it needs declarations that no header has yet:
//  - the two AnimSetManager::GetAnimator calls on Singleton<AnimSetManager>::s_inst (the singleton is not declared), both
//    with the set id, the first for the animator and the second for the set's second child;
//  - the check of the first child's count, CSceneNodeAnimatorSet::getAnimationCount() > 0 (engine, 0x65f11c), which asserts
//    at gAssertLevel 2 (a store to address 0) and 1 (message at line 69 of BlendedAnimSetController.cpp);
//  - the blender: operator new(208) and AnimatorBlender's constructor; before each child is appended to the blender's
//    animators, the blender's own +0x24 byte is set to 1 (undeclared on the collada base) and the child gets one count;
//    compile(nullptr) (vtable +0x88); weights[0] = 1.0f and
//    weights[1] = 0.0f (the weights vector is not known to be sized by the blender constructor); and m_node->addAnimator(blender)
//    (ISceneNode vtable +0x6c, declared, but only reachable once the blender exists);
//  - the cleanup: each animator gets its drop, the blender gets its drop, and the failure branch drops the animators that exist.
// Until then no children are attached, so GetAnim returns null for this controller.
BlendedAnimSetController::BlendedAnimSetController(RootSceneNode* node, int animSetId)
    : AnimController(node, true), m_animSetId(animSetId), unk_0c(0), unk_10(true), unk_14(0) {}
