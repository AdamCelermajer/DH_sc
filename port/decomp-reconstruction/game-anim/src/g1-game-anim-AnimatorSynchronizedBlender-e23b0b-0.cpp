// Batch g1-game-anim-AnimatorSynchronizedBlender-e23b0b-0 (AnimatorSynchronizedBlender, part 1 of 1), redone in place by
// batch r2a-game-anim-AnimatorSynchronizedBlender-e23b0b-0. Bodies follow the Hex-Rays pseudocode of libDungeonHunter2.so;
// asm was checked for the bare-return functions, the D0 thunks and the child lookup of the per-target loop in AnimateNode.
//
// The main gap is still the engine base glitch::collada::CSceneNodeAnimatorSynchronizedBlender (+0x04 .. +0xb3). Its
// segment vector (+0x30 begin, +0x34 end), its weight vector (+0x3c begin, +0x40 end), its dirty flag (+0xb1), its
// per-target arrays (+0x54, +0x60, +0x6c) and its methods are not declared with members in a canonical header.
// glitch-scene-collada-gui/synchronized_blender.hpp declares only two helpers and no layout, and
// glitch-scene-collada-gui/layouts/animation_track_members.hpp is 32-bit guest-width evidence that cannot be used for
// host-width pointers. The canonical scene::ISceneNodeAnimator and scene::ITimelineController headers now exist, so the
// timeline calls are expressible once the segment vector is declared. Every body that walks the segments or the weights
// stays partial. The layout requests are in the ledger notes. The own fields (+0xb4 .. +0x12f) are all declared in the
// owner header; the engine base ctor writes +0xb4 and +0xb8 and the owner ctor (0x3688f0) overwrites them with its own fields.
#include "dh2/AnimatorSynchronizedBlender.hpp"

#include <cstdint>
#include <vector>

// 0x367b64 AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::ResetDelta(unsigned int)
// The original is a bare return (bx lr, asm checked).
void AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::ResetDelta(unsigned int timeMs) {
    (void)timeMs;
}

// 0x367b68 AnimatorSynchronizedBlender::_CBAnim(glitch::scene::ITimelineController*, void*)
// The original is a bare return (bx lr, asm checked). SetCallbacks installs it as the timeline callback with this as data.
void AnimatorSynchronizedBlender::_CBAnim(glitch::scene::ITimelineController* timeline, void* userData) {
    (void)timeline;
    (void)userData;
}

// 0x367b6c AnimatorSynchronizedBlender::getType() const
int AnimatorSynchronizedBlender::getType() const {
    return 14;
}

// 0x367b74 AnimatorSynchronizedBlender::SetCallbacks(void (*)(glitch::scene::ITimelineController*, void*), void*,
//                                                    void (*)(glitch::collada::STriggeredEvent const&, void*), void*)
// partial: the per-segment loop is missing. For each segment (engine base vector +0x30, element record whose +0x04 is the
// child animator) the original stores eventCallback at the child's +0x1c and eventUserData at +0x20 (both declared on
// glitch::collada::ISceneNodeAnimator), then stores both on the child's events manager at +0x18 (callback +0x08, user data
// +0x0c; the events manager layout is not declared), then takes the child's timeline (vtable +0x44, getTimelineCtrl) and
// sets its callback (+0x08) to _CBAnim and its callback data (+0x0c) to this. Done: the animation callback pair is stored
// last, as in the original.
void AnimatorSynchronizedBlender::SetCallbacks(AnimCallback animCallback, void* animUserData,
                                               EventCallback eventCallback, void* eventUserData) {
    (void)eventCallback;
    (void)eventUserData;
    // partial: the per-segment loop goes here (see above).
    m_animCallbackUser = animUserData;  // +0xd4
    m_animCallback = animCallback;      // +0xd0
}

// 0x367c38 AnimatorSynchronizedBlender::SetScale(float)
// partial: for each segment, the child's timeline (child vtable +0x44, getTimelineCtrl) gets setScale(scale) through its
// vtable +0x48 when the timeline exists. The segment vector (engine base +0x30) is not declared.
void AnimatorSynchronizedBlender::SetScale(float scale) {
    (void)scale;
}

// 0x367ca4 AnimatorSynchronizedBlender::BlendPost()
// partial: the current child's timeline is missing. The original takes segment[m_currentIndex] (engine base vector +0x30),
// its +0x04 child animator, the timeline (vtable +0x44). When the timeline exists: length = getCurrentClipLength() (vtable
// +0x34); when length <= m_clipWrapOffset (signed compare) the offset becomes offset % length; then
// timeline->jumpTo(getCurrentClipStart() (vtable +0x2c) + offset) (vtable +0x0c). Done: the offset is cleared at the end.
void AnimatorSynchronizedBlender::BlendPost() {
    // partial: the timeline jump goes here (see above).
    m_clipWrapOffset = 0;  // +0xcc
}

// 0x367d4c AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::~SynchronizedBlenderApplicator() (D1)
// The original writes the applicator vptr, frees the weight buffer (this+0x40, the std::vector storage) and runs the
// AnimApplicator destructor. Here the vector and the base are destroyed implicitly, in the same order. The base's
// reference-node release is partial in its own row (0x364838).
AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::~SynchronizedBlenderApplicator() {}

// 0x367d94 AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::ApplyValues(unsigned int, unsigned int)
// partial: the body is two checks, and both need engine base sizes. Check 1: the blender's weight count (engine base
// +0x3c .. +0x40) must equal m_weights.size(), else ASSERT "weights.size() == blender->Weights.size()" at line 225.
// Check 2: animatorId < segment count (engine base +0x30 .. +0x34), else ASSERT "animatorId < blender->Animators.size()"
// at line 226. The ASSERT macro: gAssertLevel (core-platform/memory.hpp) 2 aborts (the original stores through address 0),
// 1 prints "ASSERT(%s) FAILED: %s:%d" to the FILE at _sF + 2 (stderr, as in the gc_singleton.hpp pattern).
// The timeMs argument is unused. Neither caller reads the return value.
void AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::ApplyValues(unsigned int animatorId,
                                                                            unsigned int timeMs) {
    (void)animatorId;
    (void)timeMs;
    // partial: the two checks go here (see above).
}

// 0x367ec8 AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::AnimateNode(unsigned int)
// partial. With no reference node (m_refNode == 0) the original returns after calling the engine base
// glitch::collada::CSceneNodeAnimatorSynchronizedBlender::animateNode(blender, nullptr, time), which is not declared.
// With a reference node: ASSERT weights count (line 355), then m_delta X, Y and Z (+0x24, +0x28, +0x2c) are cleared, then
// normalizeWeights (engine base), then for each segment with a positive weight: ApplyValues(segment, time), child applicator
// = GetApplicator(child animator), its virtual AnimateNode(time) (vtable +0x10), and weight * child delta added into m_delta
// (a null child applicator is ASSERT "aa" at line 380). Then the per-target loop over the engine base arrays +0x54, +0x60
// and +0x6c. Last, m_time = time (+0x14). Done: the clear of m_delta and the final store, both on the reference-node path.
// The per-target loop, for each index i whose +0x60 entry is non-zero, takes its child animator from segment 0 (not from
// m_currentIndex; asm 0x3680a4: blender +0x30, first element, +0x04), calls the child's vtable +0x58 with (child, i), and
// then the returned target's vtable +0x18 with (+0x54[i], weights begin, weight count, +0x60[i], +0x6c[i]). Neither slot is
// declared (the canonical ISceneNodeAnimator header ends at +0x50).
void AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::AnimateNode(unsigned int timeMs) {
    if (m_refNode != nullptr) {
        // partial: the weights-count assert (line 355) goes here.
        m_delta.X = 0.0f;  // +0x24
        m_delta.Y = 0.0f;  // +0x28
        m_delta.Z = 0.0f;  // +0x2c
        // partial: normalizeWeights, the segment loop and the per-target loop go here (see above).
        m_time = timeMs;   // +0x14, the last store of the original
    }
    // partial: the no-reference-node path calls the engine base animateNode (see above).
}

// 0x36820c AnimatorSynchronizedBlender::animateNode(glitch::scene::ISceneNode*, unsigned int)
// partial. Done: the cross-fade countdown. The original takes the unsigned step (time - m_lastTime), subtracts it from
// m_blendRemaining when that is not negative, and stores the int result. Missing, in order:
//  (1) remaining <= 0: weight[previous] = 0 and weight[current] = 1 in the engine base weight vector (+0x3c), and the engine
//      base dirty flag (+0xb1, 16-bit) is set to 1 and +0xb2 cleared whenever a weight changes;
//  (2) remaining > 0: weight[previous] = remaining * m_blendScale and weight[current] = 1 - weight[previous], same flag rule;
//  (3) m_animCallback set and the current child's timeline (segment +0x04 animator, getTimelineCtrl vtable +0x44) exists and
//      step + timeline->currentTimeMs + m_blendDuration >= timeline->getCurrentClipEnd() (vtable +0x30): m_clipWrapOffset =
//      that sum minus getCurrentClipEnd(), applicator AnimateNode(time), m_lastTime = time, then
//      m_animCallback(timeline, m_animCallbackUser) and return;
//  (4) otherwise applicator AnimateNode(time) (the non-virtual SynchronizedBlenderApplicator call) and m_lastTime = time.
// (1), (2), (3) and (4) need the engine base segment vector and weight vector, which are not declared.
void AnimatorSynchronizedBlender::animateNode(glitch::scene::ISceneNode* node, unsigned int timeMs) {
    (void)node;
    if (m_blendRemaining >= 0) {
        // Unsigned subtraction, as in the original; the result is stored in the int field.
        m_blendRemaining = static_cast<int>(static_cast<unsigned int>(m_blendRemaining) - (timeMs - m_lastTime));
        // partial: (1) and (2) go here, selected by m_blendRemaining <= 0 (see above).
    }
    // partial: (3) and (4) go here (see above).
}

// 0x3683bc AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::CustomResetDelta()
// partial. With m_refNode set, the original takes the current child animator (segment[m_blender->m_currentIndex] +0x04,
// engine base vector +0x30) and its timeline (vtable +0x44). When the timeline exists it calls jumpTo(getCurrentClipStart())
// (vtable +0x0c, +0x2c), then ApplyValues(m_blender->m_currentIndex, m_time), then the child applicator (GetApplicator)
// ResetDelta(m_time) (vtable +0x0c). A child without an applicator is ASSERT "aa" at line 326. The engine base vector is
// not declared, so the whole body is missing; the return value of the original is not read by any caller.
void AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::CustomResetDelta() {
    if (m_refNode != nullptr) {
        // partial: the child lookup, the timeline jump, ApplyValues and the child ResetDelta go here (see above).
    }
}

// 0x3684d8 AnimatorSynchronizedBlender::Blend(int)
// partial: the segment count (engine base +0x30 .. +0x34) is missing. The original asserts numAnimators == 2 (line 89),
// sets currentIndex = (currentIndex + 1) % count (unsigned; a zero count gives 0 on ARM), and runs the applicator's
// CustomResetDelta. Done: previous = current before the step, the remaining time from the stored duration, the scale
// 1 / duration when the stored duration is positive, and the stored duration = max(argument, 0). The float return is a
// register residue that the only caller discards.
void AnimatorSynchronizedBlender::Blend(int duration) {
    m_previousIndex = m_currentIndex;  // +0xb8
    // partial: the numAnimators == 2 assert (line 89), the index step (+0xb4) and the applicator CustomResetDelta go here.
    m_blendRemaining = m_blendDuration;  // +0xc0
    if (m_blendDuration > 0) {
        m_blendScale = 1.0f / static_cast<float>(m_blendDuration);  // +0xc4
    }
    m_blendDuration = duration < 0 ? 0 : duration;  // +0xbc
}

// 0x3685cc AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::SetRefNode(glitch::scene::ISceneNode*)
// partial. Done: the base AnimApplicator::SetRefNode(node) call comes first, as in the original. Missing, in order:
// ASSERT "blender" at line 271 when m_blender is null; for each segment, GetApplicator(child animator) and that
// applicator's virtual SetRefNode (vtable +0x08) with node, or AnimApplicator::SetRefNode(nullptr) when the child has
// no applicator; then unk_4c = -1 and, for each entry of the engine base array at +0x60 equal to node, the child state
// from the engine base virtual at vtable +0x54 (a record whose +0x08 is compared with 1) stores that index in unk_4c.
void AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::SetRefNode(glitch::scene::ISceneNode* node) {
    AnimApplicator::SetRefNode(node);
    // partial: the blender check, the segment loop and the unk_4c search go here (see above).
}

// No symbol of its own: the original inlines this construction into the blender constructors 0x3688f0 and 0x3689cc.
// partial: the original passes the owning blender as the ISceneNodeAnimator of AnimApplicator. The blender does not derive
// from ISceneNodeAnimator in this tree yet, so the base receives nullptr and m_animator (+0x04) stays null until the
// engine base is declared. The rest is complete: m_blender, an empty weight vector, unk_4c = -1.
AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::SynchronizedBlenderApplicator(
    AnimatorSynchronizedBlender* blender)
    : AnimApplicator(nullptr), m_blender(blender), m_weights(), unk_4c(-1) {}

// 0x3688f0 AnimatorSynchronizedBlender::AnimatorSynchronizedBlender()  (C1, complete-object constructor)
// partial: the engine base constructor glitch::collada::CSceneNodeAnimatorSynchronizedBlender is not declared, so the
// engine base area +0x04 .. +0xb3 is left uninitialised. The vptr writes (+0x00, +0x04, +0x24, +0x128) are compiler
// generated. Done: the own fields (+0xb4 .. +0xcc, +0xd0, +0xd4 zeroed), the applicator (see its constructor), and the
// reference-counted sub-object (count 1 from its default constructor).
AnimatorSynchronizedBlender::AnimatorSynchronizedBlender()
    : m_currentIndex(0),
      m_previousIndex(0),
      m_blendDuration(0),
      m_blendRemaining(0),
      m_blendScale(0.0f),
      m_lastTime(0),
      m_clipWrapOffset(0),
      m_animCallback(nullptr),
      m_animCallbackUser(nullptr),
      m_applicator(this),
      m_refSecondary() {}

// 0x3689cc AnimatorSynchronizedBlender::AnimatorSynchronizedBlender()  (C2, base-object constructor taking the VTT)
// skipped: compiler-generated base-object variant. Same body as C1 (0x3688f0), with the primary vptr taken from the VTT.

// 0x368ba4 AnimatorSynchronizedBlender::compile(std::vector<unsigned char, glitch::core::SAllocator<unsigned char, ...>>*)
// partial: the original calls the engine base compile with the buffer argument unchanged (glitch::collada::
// CSceneNodeAnimatorSynchronizedBlender::compile, not declared), then resizes the applicator weight vector (this+0x118)
// to the engine base weight count (+0x3c .. +0x40) filled with 0.0f. Both need the engine base.
void AnimatorSynchronizedBlender::compile(std::vector<std::uint8_t>* buffer) {
    (void)buffer;
    // partial: the engine base compile and the m_applicator.m_weights resize go here (see above).
}

// 0x368be4 AnimatorSynchronizedBlender::~AnimatorSynchronizedBlender()  (D2, base-object destructor taking the VTT)
// skipped: compiler-generated base-object variant. Same body as the complete-object destructor D1 (0x368cd0), with the
// vptr writes taken from the VTT. Its engine base step is partial in D1.

// 0x368c70 AnimatorSynchronizedBlender::SynchronizedBlenderApplicator::~SynchronizedBlenderApplicator()  (D0)
// skipped: compiler-generated deleting destructor. It calls D1 (0x367d4c) and then CustomFree(this). Delete on the
// virtual destructor runs the same sequence.

// 0x368cd0 AnimatorSynchronizedBlender::~AnimatorSynchronizedBlender()  (D1, complete-object destructor)
// partial: the original frees the weight buffer through the applicator, runs the AnimApplicator destructor, then the
// engine base destructor glitch::collada::CSceneNodeAnimatorSynchronizedBlender::~ (not declared). Here the applicator,
// its weight vector and the reference-counted sub-object are destroyed implicitly; the engine base step is missing.
AnimatorSynchronizedBlender::~AnimatorSynchronizedBlender() {
    // partial: the engine base destructor goes here, last (see above).
}

// 0x368d70 AnimatorSynchronizedBlender::~AnimatorSynchronizedBlender()  (D0, deleting destructor)
// skipped: compiler-generated deleting destructor. It calls D1 (0x368cd0) and then CustomFree(this), the same shape as
// the other deleting destructors in this category.
