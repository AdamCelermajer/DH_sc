// Batch g1-game-anim-CharAnimator-2af513-0 (CharAnimator, part 1 of 3).
// Layout and declarations: dh2/CharAnimator.hpp (no header change was needed for this batch).
//
// Two constructors (C1 0x3c906c, C2 0x3c8ff4) and two destructors (D1 0x3c90e8, D2 0x3c90e4) have identical bodies
// in the original, so each pair shares one definition below and both addresses are written in front of it.
//
// Partial rows and what is missing:
//   CalculateExtraTime               glitch::scene::ITimelineController has no definition in any header (fields +0x04, +0x1c, +0x2c).
//   IncAnimSetUsers/DecAnimSetUsers  glitch::collada::CAnimationSet has no users counter (+0x24) declared; the set is incomplete.
//
// AnimTable reads (ANIM_IsUnique/IsRandom/IsSequence, ANIM_GetStepCount, ANIM_GetCurrentlyPlayedAnimDict) go through the
// host element types of pydata: Arrays::AnimTable elements are Structs::AnimTpl records (pydata-accessors/w7_structs_5.hpp),
// and their step tables (AnimTpl::m_w3) are Structs::AnimStep elements (pydata-accessors/structs_records.hpp). Elements are
// reached through the typed pointer, never as 20-byte or 56-byte listing strides on bytes (pydata element_table.hpp).

#include "dh2/CharAnimator.hpp"
#include "dh2/AnimController.hpp"
#include "dh2/AnimationSet.hpp"
#include "glitch-scene-collada-gui/collada/CTimelineController.hpp"
#include "glitch-scene-collada-gui/scene/ITimelineController.hpp"
#include "dh2/Character.hpp"
#include "dh2/VisualObject.hpp"
#include "pydata-accessors/structs_records.hpp"  // Structs::AnimStep, the step records of an AnimTable record
#include "pydata-accessors/w7_arrays_02.hpp"     // Arrays::AnimTable::members
#include "pydata-accessors/w7_structs_5.hpp"     // Structs::AnimTpl, the Arrays::AnimTable element

namespace {

// The AnimTable record of animId (Structs::AnimTpl, element animId of the table). Fields by listing offset: m_w2 is the
// step count (+0x08), m_w3 the step table of Structs::AnimStep elements (+0x0c), m_w4 the kind (+0x10): 0 unique,
// 1 sequence, 2 random. The table is read only when a clip is playing, as in the original.
const Structs::AnimTpl& AnimTableRecord(int animId) {
    const auto* records = static_cast<const Structs::AnimTpl*>(static_cast<const void*>(Arrays::AnimTable::members));
    return records[animId];
}

}  // namespace

// 0x3c8ff4 _ZN12CharAnimatorC2Ev (base-object constructor C2)
// 0x3c906c _ZN12CharAnimatorC1Ev (complete-object constructor C1)
// The original constructor stores the vptr (off_966778, set by the compiler here) and these members. It does not write
// m_currentAnim (+0x4c) or the three m_animStack entries (+0x08..+0x2c); those stay indeterminate until ANIM_Set.
CharAnimator::CharAnimator() {
    m_character = nullptr;      // +0x04
    m_stopped = true;           // +0x48 no anim is playing
    m_animStackIdx = 0;         // +0x2c
    m_swooshActive = false;     // +0x30
    m_stepSpeed = 1.0f;         // +0x34
    m_animSetCompiled = false;  // +0x38
    m_animSetId = -1;           // +0x3c
    m_speed = 1.0f;             // +0x40
    m_extraTime = 0;            // +0x44
    m_animFinished = false;     // +0x49
    m_stopLoop = false;         // +0x4a
    m_pendingAnim = -1;         // +0x50
    m_blended = false;          // +0x54
    m_lastEventData = 0;        // +0x58
    m_usersAdded = false;       // +0x5c
}

// 0x3c90e4 _ZN12CharAnimatorD2Ev (base-object destructor D2)
// 0x3c90e8 _ZN12CharAnimatorD1Ev (complete-object destructor D1)
// Both are a bare return in the original.
CharAnimator::~CharAnimator() {}

// 0x3c90ec _ZN12CharAnimator10__CallbackEPN6glitch5scene19ITimelineControllerEPv
// user is the CharAnimator itself. The timeline controller is not read.
void CharAnimator::__Callback(glitch::scene::ITimelineController* /*controller*/, void* user) {
    static_cast<CharAnimator*>(user)->m_animFinished = true;  // +0x49
}

// 0x3c90f8 _ZN12CharAnimator18CalculateExtraTimeEPN6glitch5scene19ITimelineControllerE
// Same timeline arithmetic as Animator::_HandleAnimEnding: the step time (lastStep, +0x1c) minus the distance of the current
// time (+0x2c) to the timeline position (+0x04), when that distance lies inside the step; 0 otherwise.
void CharAnimator::CalculateExtraTime(glitch::scene::ITimelineController* controller) {
    if (controller == nullptr) {
        return;
    }
    m_extraTime = 0;  // +0x44, stored first in the original
    const glitch::collada::CTimelineController* timeline = static_cast<const glitch::collada::CTimelineController*>(controller);
    const int limitMs = static_cast<int>(timeline->lastStep * 1000.0f);
    const int delta = static_cast<int>(timeline->currentSec * 1000.0f) - controller->currentTimeMs;
    if (delta >= 0 && delta < limitMs) {
        m_extraTime = limitMs - delta;
    }
}

// 0x3c9168 _ZN12CharAnimator15IncAnimSetUsersEv
// The users counter (+0x24) of the game AnimationSet of the character's anim controller is raised by one.
void CharAnimator::IncAnimSetUsers() {
    // The original (listing 0x3c9174..0x3c91bc) tests the visual object (Character +0x2d8) and the returned set for null and does nothing
    // otherwise: characters whose data name is not in Arrays::CharacterTable have no visual (004_root_den _prim_Bandit_03 'RD_Bandit1').
    if (m_character->m_visualObject == nullptr) {
        return;
    }
    boost::intrusive_ptr<AnimationSet> set = m_character->m_visualObject->m_animController->GetAnimationSet();  // VisualObject +0x38
    if (set) {
        ++set->m_users;  // +0x24
    }
}

// 0x3c91c8 _ZN12CharAnimator15DecAnimSetUsersEv
// Same chain as IncAnimSetUsers, lowering the counter.
void CharAnimator::DecAnimSetUsers() {
    if (m_character->m_visualObject == nullptr) {
        return;
    }
    boost::intrusive_ptr<AnimationSet> set = m_character->m_visualObject->m_animController->GetAnimationSet();
    if (set) {
        --set->m_users;  // +0x24
    }
}

// 0x3c9228 _ZNK12CharAnimator13ANIM_IsUniqueEv
// Not stopped and the kind of the current AnimTable record (Structs::AnimTpl::m_w4, listing +0x10) is 0. The record is
// read only while playing, as the original's short-circuit does.
bool CharAnimator::ANIM_IsUnique() const {
    return !m_stopped && AnimTableRecord(m_animStack[m_animStackIdx].m_animId).Type == 0;
}

// 0x3c927c _ZNK12CharAnimator13ANIM_IsRandomEv
// Not stopped and the kind (m_w4, listing +0x10) of the current AnimTable record is 2.
bool CharAnimator::ANIM_IsRandom() const {
    return !m_stopped && AnimTableRecord(m_animStack[m_animStackIdx].m_animId).Type == 2;
}

// 0x3c92d4 _ZNK12CharAnimator15ANIM_IsSequenceEv
// Not stopped and the kind (m_w4, listing +0x10) of the current AnimTable record is 1.
bool CharAnimator::ANIM_IsSequence() const {
    return !m_stopped && AnimTableRecord(m_animStack[m_animStackIdx].m_animId).Type == 1;
}

// 0x3c932c _ZNK12CharAnimator17ANIM_GetStepIndexEv
int CharAnimator::ANIM_GetStepIndex() const {
    if (m_stopped) {
        return -1;
    }
    return m_animStack[m_animStackIdx].m_stepIndex;  // +0x10 + 12 * m_animStackIdx
}

// 0x3c934c _ZNK12CharAnimator17ANIM_GetStepCountEv
// 0 while stopped, else the step count of the current AnimTable record (Structs::AnimTpl::m_w2, listing +0x08).
int CharAnimator::ANIM_GetStepCount() const {
    if (m_stopped) {
        return 0;
    }
    return static_cast<int>(AnimTableRecord(m_animStack[m_animStackIdx].m_animId).StepsSize);
}

// 0x3c9398 _ZNK12CharAnimator31ANIM_GetCurrentlyPlayedAnimDictEv
// -1 while stopped. Else the original returns steps[ANIM_GetStepIndex()].animDict: steps is the step table of the current
// AnimTable record (Structs::AnimTpl::m_w3, listing +0x0c), and animDict is the Structs::AnimStep word m_w2 (listing +0x08).
// The step is reached through the typed pointer, not as 56 * index bytes, because the host AnimStep is not 56 bytes. No
// bounds check, as in the original.
int CharAnimator::ANIM_GetCurrentlyPlayedAnimDict() const {
    if (m_stopped) {
        return -1;
    }
    const Structs::AnimTpl& record = AnimTableRecord(m_animStack[m_animStackIdx].m_animId);
    const auto* steps = static_cast<const Structs::AnimStep*>(record.Steps);  // +0x0c
    return steps[ANIM_GetStepIndex()].Anim;                                  // +0x08 animDict
}

// 0x3c93fc _ZN12CharAnimator13ANIM_SetSpeedEf
// The original stores the speed (+0x40), then, when the character has a visual object with an animation controller, calls
// the controller's SetScale slot (AnimController vtable +0x28) with speed * m_stepSpeed and animation index 0.
void CharAnimator::ANIM_SetSpeed(float speed) {
    m_speed = speed;                                       // +0x40
    VisualObject* visual = m_character->m_visualObject;    // Character +0x2d8
    if (visual != nullptr) {
        visual->m_animController->SetScale(speed * m_stepSpeed, 0);  // VisualObject +0x38, vtable +0x28
    }
}

// 0x3c9444 _ZN12CharAnimator17ANIM_SkipNextStepEj
// Adds one to the step index of stack entry stackIdx (+0x10 + 12 * stackIdx) while playing.
void CharAnimator::ANIM_SkipNextStep(unsigned int stackIdx) {
    if (!m_stopped) {
        ++m_animStack[stackIdx].m_stepIndex;
    }
}

// 0x3c9464 _ZN12CharAnimator17ANIM_SkipNextStepEv
void CharAnimator::ANIM_SkipNextStep() {
    ANIM_SkipNextStep(m_animStackIdx);  // +0x2c
}

// 0x3c946c _ZN12CharAnimator12ANIM_SetStepEjj
// Sets the step index of stack entry stackIdx (+0x10 + 12 * stackIdx) while playing.
void CharAnimator::ANIM_SetStep(unsigned int step, unsigned int stackIdx) {
    if (!m_stopped) {
        m_animStack[stackIdx].m_stepIndex = static_cast<int>(step);
    }
}

// 0x3c9484 _ZN12CharAnimator12ANIM_SetStepEj
void CharAnimator::ANIM_SetStep(unsigned int step) {
    ANIM_SetStep(step, m_animStackIdx);  // +0x2c
}

// 0x3c948c _ZN12CharAnimator13ANIM_StopLoopEb
// While playing, the loop count of the current stack entry (+0x04 in the entry, +0x0c + 12 * m_animStackIdx in the object) is
// set to the m_stopped byte. That byte is 0 inside this branch, so the loop count becomes 0. endNow sets m_stopLoop (+0x4a).
void CharAnimator::ANIM_StopLoop(bool endNow) {
    if (!m_stopped) {
        m_animStack[m_animStackIdx].m_loopCount = static_cast<int>(m_stopped);
        if (endNow) {
            m_stopLoop = true;  // +0x4a
        }
    }
}
