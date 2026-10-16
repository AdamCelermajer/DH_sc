// Batch g1-game-anim-CharAnimator-2af513-2: CharAnimator::Update (0x3caf3c). Declaration from dh2/CharAnimator.hpp.
//
// The control flow follows the Hex-Rays body and the assembly. The stop-loop halfword store at +0x49 (Hex-Rays
// "*(_WORD *)(this + 73) = 1") is two byte stores in the binary: m_animFinished becomes 1 and m_stopLoop becomes 0.
// The inputs that were unresolved are now declared:
//   - Character +0x520 bit 0x200 is CharStateMachine::m_stateFlags (CharStateMachine +0x24, base at Character +0x4fc).
//   - The Arrays::AnimTable record of the current anim is read through the typed host element (Structs::AnimTpl):
//     m_w2 is the step count (+0x08) and m_w4 the sequence flag (+0x10, 1 selects the step path).
//   - PushProfilingContext / PopProfilingContext are declared in dh2/global/runtime_support.hpp and bracket the body.
// The body calls IncAnimSetUsers and DecAnimSetUsers, which are partial in g1-game-anim-CharAnimator-2af513-0.cpp (the
// users counter of glitch::collada::CAnimationSet is not declared), and _SetAnim and _SetAnimStep, which are partial in
// g1-game-anim-CharAnimator-2af513-1.cpp.

#include "dh2/CharAnimator.hpp"

#include <cstdint>

// runtime_support.hpp before Character.hpp: Character.hpp renames MemoryHintState for its net-transport copy while it
// is included, so memory.hpp must be read first.
#include "dh2/global/runtime_support.hpp"       // PushProfilingContext, PopProfilingContext
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "pydata-accessors/structs_records.hpp"  // Structs::AnimStep, the step records of an AnimTable record
#include "pydata-accessors/w7_arrays_02.hpp"     // Arrays::AnimTable::members and size
#include "pydata-accessors/w7_structs_5.hpp"     // Structs::AnimTpl, the Arrays::AnimTable element

namespace {

// The Arrays::AnimTable record of animId, a host Structs::AnimTpl element (see g1-game-anim-CharAnimator-2af513-1.cpp).
const Structs::AnimTpl& AnimTableRecord(int animId) {
    const auto* records = static_cast<const Structs::AnimTpl*>(static_cast<const void*>(Arrays::AnimTable::members));
    return records[animId];
}

}  // namespace

namespace charanimator_detail {

// Switch name read by the trace calls in the original (string literal).
const char kTraceCharAnimatorSwitch[] = "isTracingCharAnimator";

// The trace read in the original: DebugSwitches::load(&s_inst), then GetSwitch(&s_inst, name) with the result discarded.
// GetSwitch inserts a false entry for an unknown name, as the original does.
void TraceCharAnimatorSwitch() {
    DebugSwitches::s_inst.load();
    (void)DebugSwitches::s_inst.GetSwitch(kTraceCharAnimatorSwitch);
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
void CharAnimator::Update() {
    PushProfilingContext("CharAnimator::Update");
    if ((m_character->m_stateFlags & 0x200u) == 0) {            // Character +0x520 (CharStateMachine m_stateFlags) & 0x200
        if (m_usersAdded) {                                     // +0x5c
            DecAnimSetUsers();
        }
        m_usersAdded = false;
        PopProfilingContext("CharAnimator::Update");
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
        const Structs::AnimTpl& record = AnimTableRecord(animId);
        m_character->RaiseEvent(39, nullptr);

        bool loopPath = false;                                  // the original's goto LABEL_13
        if (record.Type != 1) {                                 // +0x10 sequence flag
            m_character->RaiseEvent(37, nullptr);
            loopPath = true;
        } else {
            const int stepCount = static_cast<int>(record.StepsSize);  // +0x08 StepsSize
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
    PopProfilingContext("CharAnimator::Update");
}
