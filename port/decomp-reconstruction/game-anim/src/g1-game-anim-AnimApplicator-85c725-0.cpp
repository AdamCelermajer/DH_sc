// AnimApplicator functions of libDungeonHunter2.so (game-anim batch g1-game-anim-AnimApplicator-85c725-0).
// Rebuilt from the Hex-Rays pseudocode in recovered/native/hexrays-all. Class layout and declarations:
// game/include/dh2/AnimApplicator.hpp.
#include "dh2/AnimApplicator.hpp"

namespace {

// Sets a vector to (0, 0, 0). The original writes the three float words directly.
void SetZero(glitch::core::vector3d<float>& v) {
    v.X = 0.0f;
    v.Y = 0.0f;
    v.Z = 0.0f;
}

// PARTIAL seam. Reads the reference-node pose for `time` into `out` and returns true when it is available.
// The original reads it one of two ways:
//   m_refClip == -1: m_refNode->getPosition() (ISceneNode vptr+0xa0, returns a vector3d<float>&).
//   otherwise:       m_animator->getTimelineCtrl() (vptr+0x44), then the animator's pure sample slot
//                    (vptr+0x7c) with the clip index, a time value read from the timeline (+0x04 in
//                    AnimateNode, +0x10 in ResetDelta(uint); the argument time when the timeline is null)
//                    and the output vector.
// None of those virtual slots is declared in a header yet (see the layout requests), so this seam reports no
// pose and the callers skip the pose-dependent writes.
bool SampleRefPose(const AnimApplicator& self, unsigned int time, glitch::core::vector3d<float>& out) {
    (void)self;
    (void)time;
    (void)out;
    return false;
}

}  // namespace

// 0x364330 AnimApplicator::AnimApplicator(glitch::collada::ISceneNodeAnimator*)
// 0x364398 AnimApplicator::AnimApplicator(glitch::collada::ISceneNodeAnimator*)
// C2 and C1 have the same body. The constructor does not write m_refClip (+0x0c): SetRefNode writes it
// before any read, because it is only read while m_refNode is set.
AnimApplicator::AnimApplicator(glitch::collada::ISceneNodeAnimator* animator)
    : m_animator(animator) {
    m_refNode = nullptr;        // +0x08
    unk_10 = 0;                 // +0x10
    m_time = 0;                 // +0x14
    SetZero(m_prevPos);         // +0x18..+0x20
    SetZero(m_delta);           // +0x24..+0x2c
    m_callbackPending = false;  // +0x30
    m_callback = nullptr;       // +0x34
    m_callbackUser = nullptr;   // +0x38
}

// 0x364400 AnimApplicator::SetCallback(void (*)(glitch::scene::ITimelineController*, void*), void*)
void AnimApplicator::SetCallback(AnimCallback animCallback, void* userData) {
    m_callbackUser = userData;  // +0x38
    m_callback = animCallback;  // +0x34
}

// 0x36440c AnimApplicator::CheckCallback(glitch::scene::ITimelineController*)
void AnimApplicator::CheckCallback(glitch::scene::ITimelineController* timeline) {
    if (!m_callbackPending) return;
    if (m_callback == nullptr) return;  // the pending flag stays set until a callback is installed
    m_callback(timeline, m_callbackUser);
    m_callbackPending = false;
}

// 0x364444 AnimApplicator::CalculateDelta(unsigned int, glitch::core::vector3d<float> const&)
// Returns the X delta. The original returns r0 unchanged on the same-time path, which holds the this pointer
// (register residue); this port returns the zeroed X delta on that path instead. The only caller
// (AnimatorBlender, Hex-Rays chunk_004.c:19616) discards the result.
float AnimApplicator::CalculateDelta(unsigned int time, const glitch::core::vector3d<float>& position) {
    if (m_time == time) {
        SetZero(m_delta);  // +0x24..+0x2c
    } else {
        m_delta.X = position.X - m_prevPos.X;
        m_delta.Y = position.Y - m_prevPos.Y;
        m_delta.Z = position.Z - m_prevPos.Z;
    }
    m_prevPos.X = position.X;  // +0x18
    m_prevPos.Y = position.Y;  // +0x1c
    m_prevPos.Z = position.Z;  // +0x20
    m_time = time;             // +0x14
    return m_delta.X;
}

// 0x3644cc AnimApplicator::ResetDelta(unsigned int, glitch::core::vector3d<float> const&)
void AnimApplicator::ResetDelta(unsigned int time, const glitch::core::vector3d<float>& position) {
    m_prevPos.X = position.X;  // +0x18
    m_prevPos.Y = position.Y;  // +0x1c
    m_prevPos.Z = position.Z;  // +0x20
    m_time = time;             // +0x14
    SetZero(m_delta);          // +0x24..+0x2c
}

// 0x3644fc AnimApplicator::ResetDelta(unsigned int)
// PARTIAL: with a reference node, the pose has to come from SampleRefPose, and the previous pose is then written
// exactly as ResetDelta(time, pose) does. The seam reports no pose, so the previous pose is left unchanged on that
// path. Without a reference node the original's tail is complete: m_time = time and the delta is zeroed.
void AnimApplicator::ResetDelta(unsigned int time) {
    glitch::core::vector3d<float> pose;
    if (m_refNode != nullptr && SampleRefPose(*this, time, pose)) {
        ResetDelta(time, pose);
        return;
    }
    m_time = time;
    SetZero(m_delta);
}

// 0x3645e8 AnimApplicator::AnimateNode(unsigned int)
// PARTIAL: with a reference node, the pose has to come from SampleRefPose. The original then computes the delta
// the same way CalculateDelta does (same-time test, delta = pose - previous, previous = pose) and sets m_time.
// Without a reference node the original only writes m_time (its LABEL_11). The float the original leaves in r0 is
// the register residue of its last float; the header declares the function void.
void AnimApplicator::AnimateNode(unsigned int time) {
    glitch::core::vector3d<float> pose;
    if (m_refNode != nullptr && SampleRefPose(*this, time, pose)) {
        CalculateDelta(time, pose);
        return;
    }
    m_time = time;  // +0x14
}

// 0x36473c AnimApplicator::SetRefNode(glitch::scene::ISceneNode*)
// PARTIAL. Written: the new node is stored at +0x08, and for a non-null node the clip index is set to -1 (+0x0c).
// Not written:
//   - the release of the old node and the grab of the new one. Both go through glitch::IReferenceCounted at the
//     ISceneNode virtual base (vtable word at vptr-12 is 0x130; reference count at base +0x04). ISceneNode has no
//     base declared yet.
//   - the clip search. The original counts the clips with the animator's pure slot at vptr+0x70, reads each clip
//     with the pure slot at vptr+0x54, compares the clip name (+0x04) with ISceneNode::getUID (vptr+0x54), and sets
//     m_refClip to each matching index whose flag (+0x08) is 1. The result is the last such index, or -1.
//     m_refClip therefore stays -1 here.
void AnimApplicator::SetRefNode(glitch::scene::ISceneNode* node) {
    m_refNode = node;  // +0x08
    if (node == nullptr) return;
    m_refClip = -1;  // +0x0c
}

// 0x364838 AnimApplicator::~AnimApplicator()   (D1)
// 0x364870 AnimApplicator::~AnimApplicator()   (D2, same body)
// PARTIAL: the destructor's effect on the reference node is the SetRefNode release, which is partial (see above).
// With no reference node set, the body is complete.
AnimApplicator::~AnimApplicator() {
    AnimApplicator::SetRefNode(nullptr);
}
