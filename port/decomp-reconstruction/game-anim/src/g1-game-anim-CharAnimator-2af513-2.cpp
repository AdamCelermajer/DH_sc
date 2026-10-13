// Batch g1-game-anim-CharAnimator-2af513-2: CharAnimator::Update (0x3caf3c). Declaration from dh2/CharAnimator.hpp.
//
// Partial. The control flow follows the Hex-Rays body and the assembly. The stop-loop halfword store at +0x49
// (Hex-Rays "*(_WORD *)(this + 73) = 1") is two byte stores in the binary (STRBNE at +0x4a and +0x49): m_stopLoop
// becomes 0 and m_animFinished becomes 1. Three inputs are not available in this tree and are unresolved:
//   1. Character +0x520 bit 0x200. That word is CharStateMachine m_stateFlags (CharStateMachine +0x24, base at
//      Character +0x4fc). The field is private to CharStateMachine and has no accessor. The bit is taken as set, so
//      the update body runs; the clear case is the early exit.
//   2. The Arrays::AnimTable row words, 20-byte file stride: +0x08 is the step count and +0x10 the sequence flag
//      (1 selects the step path). Rows are not decoded into host elements yet. The flag is taken as 0, so the step
//      path is not taken and the loop and stop path runs; the step path is written and waits on the rows.
//   3. PushProfilingContext / PopProfilingContext (at entry and on every exit). Their declarations are not in the
//      tree, so the calls are omitted.

#include "dh2/CharAnimator.hpp"

#include <cstdint>

#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"

namespace charanimator_detail {

// Switch name read by the trace calls in the original (string literal).
const char kTraceCharAnimatorSwitch[] = "isTracingCharAnimator";

// The trace read in the original: DebugSwitches::load on the singleton, then GetSwitch with the result discarded.
// GetInstance() calls load() and then returns the singleton, so this is the same sequence. GetSwitch inserts a false
// entry for an unknown name, as the original does.
void TraceCharAnimatorSwitch() {
    (void)DebugSwitches::GetInstance()->GetSwitch(kTraceCharAnimatorSwitch);
}

// UNRESOLVED: Character +0x520 bit 0x200 (see the top of this file). Taken as set.
bool StateFlag200Set(const Character* character) {
    (void)character;
    return true;
}

// UNRESOLVED: Arrays::AnimTable row word +0x08 for animId. The row is not decoded yet, so 0 is returned.
int AnimTableStepCount(int animId) {
    (void)animId;
    return 0;
}

// UNRESOLVED: Arrays::AnimTable row word +0x10 for animId (1 selects the step path). The row is not decoded yet,
// so 0 is returned and the step path is not taken.
int AnimTableSequenceFlag(int animId) {
    (void)animId;
    return 0;
}

// Loop counter decrement before the trace read: a positive count goes down by one; zero and negative (forever) stay.
int DecrementLoopCount(int loopCount) {
    return loopCount > 0 ? loopCount - 1 : loopCount;
}

// Loop counter stored after the re-read: a negative value is stored as -1 (forever).
int StoredLoopCount(int remaining) {
    return remaining >= 0 ? remaining : -1;
}

// Step-end test: a step index at or past the step count ends the animation. Step indices only count up from 0, so
// the signed compare gives the same result as the original's unsigned one.
bool StepReachesEnd(int step, int stepCount) {
    return stepCount <= step;
}

}  // namespace charanimator_detail

// 0x3caf3c _ZN12CharAnimator6UpdateEv
// partial: the profiler push and pop are omitted (no declaration in the tree). The three unresolved inputs are listed
// at the top of this file, and each is marked at its use.
void CharAnimator::Update() {
    if (!charanimator_detail::StateFlag200Set(m_character)) {   // Character +0x520 & 0x200 == 0
        if (m_usersAdded) {                                     // +0x5c
            DecAnimSetUsers();
        }
        m_usersAdded = false;
        return;
    }
    if (!m_usersAdded) {
        IncAnimSetUsers();
    }
    const bool stopLoopWas = m_stopLoop;                        // +0x4a
    const bool animFinishedWas = m_animFinished;                // +0x49
    m_usersAdded = true;
    if (stopLoopWas) {
        // Halfword 1 at +0x49: m_animFinished becomes true and the byte at +0x4a (m_stopLoop) becomes 0.
        m_animFinished = true;
        m_stopLoop = false;
    }

    if (stopLoopWas || animFinishedWas) {
        const unsigned int stackIdx = m_animStackIdx;           // v4, cached for the whole step
        const int animId = m_animStack[stackIdx].m_animId;      // row of Arrays::AnimTable (v6)
        m_character->RaiseEvent(39, nullptr);

        bool loopPath = false;                                  // the original's goto LABEL_13
        if (charanimator_detail::AnimTableSequenceFlag(animId) != 1) {
            m_character->RaiseEvent(37, nullptr);
            loopPath = true;
        } else {
            const int stepCount = charanimator_detail::AnimTableStepCount(animId);
            if (m_animStack[stackIdx].m_stepIndex + 1 == stepCount) {
                m_character->RaiseEvent(37, nullptr);
            }
            // Re-read after event 37 when it was raised, as in the original.
            const int nextStep = m_animStack[stackIdx].m_stepIndex + 1;
            m_animStack[stackIdx].m_stepIndex = nextStep;
            if (charanimator_detail::StepReachesEnd(nextStep, stepCount)) {
                loopPath = true;
            } else {
                charanimator_detail::TraceCharAnimatorSwitch();
                m_character->RaiseEvent(35, nullptr);
                const int step = m_animStack[stackIdx].m_stepIndex;     // re-read after event 35
                if (charanimator_detail::StepReachesEnd(step, stepCount)) {
                    CharAnimator::Update();
                } else {
                    _SetAnimStep(static_cast<unsigned int>(step));
                }
            }
        }

        if (loopPath) {                                         // LABEL_13
            const int loopCount = m_animStack[stackIdx].m_loopCount;   // +0x04 of the stack entry
            if (loopCount != 0) {
                m_animStack[stackIdx].m_loopCount = charanimator_detail::DecrementLoopCount(loopCount);
                charanimator_detail::TraceCharAnimatorSwitch();
                m_character->RaiseEvent(35, nullptr);
                const int remaining = m_animStack[stackIdx].m_loopCount;   // re-read after event 35
                if (m_pendingAnim == -1) {                      // +0x50
                    _SetAnim(m_animStack[stackIdx].m_animId, m_animStackIdx);
                }
                m_animStack[stackIdx].m_loopCount = charanimator_detail::StoredLoopCount(remaining);
            } else if (m_animStackIdx != 0) {
                charanimator_detail::TraceCharAnimatorSwitch();
                --m_animStackIdx;
                CharAnimator::Update();
            } else if (!m_stopped) {                            // +0x48
                charanimator_detail::TraceCharAnimatorSwitch();
                m_stopped = true;
                m_character->RaiseEvent(34, nullptr);
            }
        }
        m_animFinished = false;                                 // LABEL_21 (+0x49)
    }

    // LABEL_22: an animation requested by ANIM_Set while a callback was pending.
    if (m_pendingAnim != -1) {
        ANIM_Set(m_pendingAnim);
        m_pendingAnim = -1;
    }
}
