// AnimatorBlender, batch g1-game-anim-AnimatorBlender-0caed3-0 (part 1 of 2). The deleting destructor 0x367284 is in
// g1-game-anim-AnimatorBlender-0caed3-1.cpp. Bodies follow the Hex-Rays pseudocode of libDungeonHunter2.so; asm was
// checked for the integer duration compare in Blend and for the thunk in compile. Functions marked partial in the
// ledger have a comment at the place where the missing piece goes.
#include "dh2/AnimatorBlender.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

// core-platform/memory.hpp declares this too. The header is not included here: its deps/glitch_core.hpp repeats
// the glitch::core types that AnimatorBlender.hpp already gets from glitch-core-io.
extern int gAssertLevel;

namespace {

// The source path that the original passes to its ASSERT macro.
constexpr const char* kAnimatorBlenderSource =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\Irrlicht\\Nodes\\Animators\\AnimatorBlender.cpp";

// The original ASSERT. gAssertLevel 2 stops the program (the original stores to address 0 to crash); gAssertLevel 1
// prints the message to stderr and execution continues.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kAnimatorBlenderSource, line);
    }
}

// Cross-fade step shared by animateNode and updateTime (the same block in both originals). It counts the remaining
// time down by the time since the last update, then sets the weights of the previous and current child from it.
void StepCrossFade(AnimatorBlender& blender, unsigned int timeMs) {
    if (blender.m_blendRemaining < 0) {
        return;
    }
    // Unsigned subtraction, as in the original (timeMs - m_lastTime is unsigned).
    const int remaining =
        static_cast<int>(static_cast<unsigned int>(blender.m_blendRemaining) - (timeMs - blender.m_lastTime));
    blender.m_blendRemaining = remaining;
    if (remaining <= 0) {
        blender.weights[blender.m_previousIndex] = 0.0f;
        blender.weights[blender.m_currentIndex] = 1.0f;
    } else {
        const float previousWeight = static_cast<float>(remaining) * blender.m_blendScale;
        blender.weights[blender.m_previousIndex] = previousWeight;
        blender.weights[blender.m_currentIndex] = 1.0f - previousWeight;
    }
}

}  // namespace

// 0x366620 AnimatorBlender::getType() const
int AnimatorBlender::getType() const {
    return 12;
}

// 0x366628 AnimatorBlender::_HandleAnimEnding(glitch::scene::ITimelineController*)
// partial: the current child's timeline (child vtable +0x44, getTimelineCtrl) and the timeline's +0x04, +0x1c and
// +0x2c members are not reachable from the declared types yet. Missing: if that timeline is the argument, store the
// carried-over time in m_applicator.unk_10 (+0x98), then set m_applicator.m_callbackPending (+0xb8).
void AnimatorBlender::_HandleAnimEnding(glitch::scene::ITimelineController* timeline) {
    (void)timeline;
}

// 0x3666c8 AnimatorBlender::_CBAnim(glitch::scene::ITimelineController*, void*)
// Timeline callback. The Hex-Rays argument names are shifted: the first argument is the timeline and the second is the
// blender that SetCallbacks stores as user data.
void AnimatorBlender::_CBAnim(glitch::scene::ITimelineController* timeline, void* userData) {
    static_cast<AnimatorBlender*>(userData)->AnimatorBlender::_HandleAnimEnding(timeline);
}

// 0x3666d8 AnimatorBlender::SetScale(float)
// PARTIAL. Original: for each child in animators, timeline = child->getTimelineCtrl() (vptr +0x44) and, when it is
// non-null, timeline->setScale(scale) (vptr +0x48). ITimelineController::setScale is declared
// (glitch-scene-collada-gui/scene/ITimelineController.hpp), but animators holds glitch::collada::ISceneNodeAnimator*, and
// that type (collada_misc.hpp) does not derive from glitch::scene::ISceneNodeAnimator, which declares getTimelineCtrl.
// Layout request: glitch::collada::ISceneNodeAnimator must derive from glitch::scene::ISceneNodeAnimator.
void AnimatorBlender::SetScale(float scale) {
    (void)scale;
}

// 0x366740 AnimatorBlender::BlendPost()
void AnimatorBlender::BlendPost() {}

// 0x366764 AnimatorBlender::compile(std::vector<unsigned char, glitch::core::SAllocator<unsigned char, ...>>*)
// Thunk: the original tail-branches to the engine base with the buffer argument unchanged (asm checked).
void AnimatorBlender::compile(std::vector<std::uint8_t>* buffer) {
    glitch::collada::CSceneNodeAnimatorBlender::compile(buffer);
}

// 0x366768 AnimatorBlender::BlenderApplicator::~BlenderApplicator() (D1)
// The vptr reset and the AnimApplicator destructor are the implicit destruction of the base.
AnimatorBlender::BlenderApplicator::~BlenderApplicator() {}

// No symbol of its own: the original inlines this constructor into 0x367018 and 0x3670c8.
AnimatorBlender::BlenderApplicator::BlenderApplicator(AnimatorBlender* blender)
    : AnimApplicator(blender), m_blender(blender) {}

// 0x36679c AnimatorBlender::Blend(int)
// The float that the original returns is not used: the only caller (BlendedAnimSetController::PlayClip) discards it.
void AnimatorBlender::Blend(int duration) {
    const unsigned int count = static_cast<unsigned int>(animators.size());
    if (count != 2) {
        ReportAssert("numAnimators == 2", 122);
    }
    m_previousIndex = m_currentIndex;
    // ARM unsigned divide by zero yields 0, so an empty list keeps index 0.
    m_currentIndex = count != 0 ? static_cast<int>(static_cast<unsigned int>(m_currentIndex + 1) % count) : 0;
    // The duration stored by the previous call drives this cross-fade. The original compares it as an integer.
    m_blendRemaining = m_blendDuration;
    if (m_blendDuration > 0) {
        m_blendScale = 1.0f / static_cast<float>(m_blendDuration);
    }
    m_blendDuration = duration < 0 ? 0 : duration;  // a negative duration is stored as 0
}

// 0x366888 AnimatorBlender::BlenderApplicator::AnimateNode(unsigned int)
// PARTIAL: the weighted sum over the child applicators is missing. Per child it needs GetApplicator(child) (0x369160,
// declared in dh2/global/GetApplicator.hpp, which takes glitch::scene::ISceneNodeAnimator*), the child's timeline
// (vptr +0x44, scene::ISceneNodeAnimator::getTimelineCtrl), the child's getAnimationValue (vptr +0x7c, not declared on
// either animator type) and the child applicator's CalculateDelta (declared). The child type is the blocker, as in
// SetScale. The reset of the delta at the start is complete.
void AnimatorBlender::BlenderApplicator::AnimateNode(unsigned int timeMs) {
    m_delta.X = 0.0f;  // +0x24
    m_delta.Y = 0.0f;  // +0x28
    m_delta.Z = 0.0f;  // +0x2c
    if (m_refClip != -1) {
        // partial: the child loop goes here (see above).
    }
    (void)timeMs;
}

// 0x366a68 AnimatorBlender::BlenderApplicator::ResetDelta(unsigned int)
// PARTIAL: with a reference node set, the original resets this applicator's delta from the current child's clip value:
// child = animators[m_blender->m_currentIndex], timeline = child->getTimelineCtrl() (vptr +0x44, scene base),
// GetApplicator(child), child->getAnimationValue(m_refClip, timeline +0x10, out) (vptr +0x7c, not declared), then
// AnimApplicator::ResetDelta(timeMs, out) on the child applicator. Blocked on the child type, as in SetScale.
void AnimatorBlender::BlenderApplicator::ResetDelta(unsigned int timeMs) {
    if (m_refNode != nullptr) {
        // partial: the child-based reset goes here (see above).
    }
    (void)timeMs;
}

// 0x366b80 AnimatorBlender::BlenderApplicator::SetRefNode(glitch::scene::ISceneNode*)
// PARTIAL: after the base SetRefNode, the original forwards the node to each child's applicator (GetApplicator, then the
// applicator's virtual SetRefNode, vptr +0x08), and clears this applicator's reference for a child that has none. The
// GetApplicator parameter is glitch::scene::ISceneNodeAnimator*, and the child is glitch::collada::ISceneNodeAnimator*
// (the same layout request as SetScale). The assert at line 304 is complete.
void AnimatorBlender::BlenderApplicator::SetRefNode(glitch::scene::ISceneNode* node) {
    if (m_blender == nullptr) {
        ReportAssert("blender", 304);
    }
    AnimApplicator::SetRefNode(node);
    // partial: the child forwarding loop goes here (see above).
}

// 0x366c8c AnimatorBlender::onUnbind(glitch::scene::ISceneNode*)
// partial: the base call glitch::collada::ISceneNodeAnimator::onUnbind(this, node) is not declared yet.
void AnimatorBlender::onUnbind(glitch::scene::ISceneNode* node) {
    m_applicator.BlenderApplicator::SetRefNode(nullptr);
    (void)node;
}

// 0x366cb4 AnimatorBlender::animateNode(glitch::scene::ISceneNode*, unsigned int)
// PARTIAL: the final m_applicator.CheckCallback(animators[m_currentIndex]->getTimelineCtrl()) is missing; the child type
// does not declare getTimelineCtrl (see SetScale). The cross-fade step, applyAnimationValues (vptr +0x50, declared on
// the engine base), the applicator's AnimateNode and the m_lastTime store are complete.
void AnimatorBlender::animateNode(glitch::scene::ISceneNode* node, unsigned int timeMs) {
    (void)node;
    StepCrossFade(*this, timeMs);
    applyAnimationValues(timeMs);                         // virtual, vtable +0x50
    m_applicator.BlenderApplicator::AnimateNode(timeMs);  // partial, see there
    // partial: m_applicator.CheckCallback(current child's timeline) goes here.
    m_lastTime = timeMs;
}

// 0x366d90 AnimatorBlender::updateTime(unsigned int)
// partial: the final CheckCallback with the current child's timeline is missing, because the child type does not
// declare getTimelineCtrl yet. The rest is complete.
void AnimatorBlender::updateTime(unsigned int timeMs) {
    StepCrossFade(*this, timeMs);
    const std::size_t count = animators.size();
    for (std::size_t i = 0; i != count; ++i) {
        if (weights[i] == 0.0f) {  // children with zero weight are not updated
            continue;
        }
        animators[i]->updateTime(timeMs);
    }
    normalizeWeights();
    // partial: m_applicator.CheckCallback(animators[m_currentIndex]->getTimelineCtrl()) goes here.
    m_lastTime = timeMs;
}

// 0x366eb8 AnimatorBlender::SetCallbacks(void (*)(glitch::scene::ITimelineController*, void*), void*,
//                                        void (*)(glitch::collada::STriggeredEvent const&, void*), void*)
// PARTIAL: the per-child loop is missing. For each child it writes the child's event fields (collada +0x18 eventsTrack,
// +0x1c eventCallback, +0x20 eventUserData), the events manager's callback pair (CEventsManager +0x08 and +0x0c, the class
// is incomplete) and the child timeline's callback pair (scene ITimelineController callback +0x08 and callbackData +0x0c,
// now declared as void* data, so the earlier width mismatch is gone). Remaining blockers: the child's getTimelineCtrl, the
// CEventsManager layout, and the child's eventCallback type, void(*)(const SEventCallArgs*, void*) in collada_misc.hpp,
// against the void(*)(const STriggeredEvent&, void*) that this function receives. The applicator step is complete and
// runs last, as in the original.
void AnimatorBlender::SetCallbacks(AnimCallback animCallback, void* animUserData, EventCallback eventCallback,
                                   void* eventUserData) {
    (void)eventCallback;
    (void)eventUserData;
    m_applicator.SetCallback(animCallback, animUserData);
}

// 0x367018 AnimatorBlender::AnimatorBlender()  (complete-object constructor, C1)
// The vptr writes and the base-class construction are compiler generated. The original's reference-counted sub-object
// is a virtual base; here it is a member, with the same count of 1 (IReferenceCounted default).
AnimatorBlender::AnimatorBlender()
    : glitch::collada::CSceneNodeAnimatorBlender(),
      m_currentIndex(0),
      m_previousIndex(0),
      m_blendDuration(0),
      m_blendRemaining(0),
      m_blendScale(0.0f),
      m_lastTime(0),
      m_applicator(this),
      m_refSecondary() {}

// 0x36720c AnimatorBlender::~AnimatorBlender()  (complete-object destructor, D1)
// The members and the engine base are destroyed by the implicit destructors. The original destroys the applicator and
// then the base, with the reference-counted virtual base last. Here that member is destroyed first.
AnimatorBlender::~AnimatorBlender() {}

// Not written here (see the ledger): 0x3670c8 (C2, base-object constructor taking the VTT), 0x36715c (D2, base-object
// destructor taking the VTT) and 0x3671c8 (D0, deleting destructor of BlenderApplicator). They are compiler-generated
// variants of the constructors and destructors above.
