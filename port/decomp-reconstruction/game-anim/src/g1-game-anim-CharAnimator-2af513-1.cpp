// Batch g1-game-anim-CharAnimator-2af513-1 (CharAnimator, part 2 of 3): the item swoosh helpers, the anim-set
// helpers, SetAnimationSet and the anim stack (0x3c94b8 to 0x3caccc). Declarations come from dh2/CharAnimator.hpp,
// which needed no change for this batch.
//
// Complete rows: SetCharacter, ANIM_Stop, ANIM_Set. ANIM_Stop was checked in the assembly: the StopClip call passes
// animIndex 0 in r2, a value the function sets with MOV R2, #0 at entry.
//
// Partial rows and what is missing (each is marked at its use):
//   Arrays::AnimTable / Arrays::CharAnimTable rows: the element classes are not identified (pydata keeps the 20-byte
//     and 160-byte listing strides). Scalar fields are read through ReadAnimTableWord, as in
//     g1-game-anim-CharAnimator-2af513-0.cpp. The step table pointer (+0x0c) is a runtime address and the 56-byte step
//     records cannot be read, so every step path is missing.
//   AnimSetManager singleton: Singleton<AnimSetManager>::s_inst has no declaration (AnimSetManager.hpp has no instance).
//   VoxSoundManager::s_instance, Play3D and LoadSound: not declared (vox-audio/include/vox-audio/vox/sound_manager.hpp).
//   VisualFXManager (singleton, PlayAnimFXSet, RegisterFXSetToLoad): waits on dh2/VisualFXManager.hpp (absent).
//   ItemInstance: waits on dh2/ItemInstance.hpp (absent), so the swoosh sound and FX ids (Item +0x14, +0x18) cannot be read.
//   CameraLevel (CanPlayShakeAnim, PlayAnim): waits on dh2/CameraLevel.hpp (absent).
//   AnimSetController and BlendedAnimSetController: wait on dh2/AnimSetController.hpp (absent).
//   glitch::collada::STriggeredEvent: not defined, so __CallbackEvent cannot read its two words.
//   AnimController::SetCallbacks takes a void event callback, but __CallbackEvent returns int (see the layout requests).
//   _SetAnim's random pick: the original passes r1 as the synced flag and never sets it on that path.
// ~CharAnimator (0x3c9af8) is the compiler-generated deleting destructor. Its body is the complete destructor and
// CustomFree(this), so no body is written here.

#include "dh2/CharAnimator.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "dh2/AnimController.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Random.hpp"
#include "dh2/VisualObject.hpp"
#include "pydata-accessors/w7_arrays_02.hpp"

// Global assert level (libDungeonHunter2.so gAssertLevel). core-platform/memory.hpp declares it too.
extern int gAssertLevel;

namespace {

// The source path that the original passes to its ASSERT macro.
constexpr const char* kCharAnimatorSource =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\Animator\\CharAnimator.cpp";

// The original ASSERT. gAssertLevel 2 stops the program (the original stores to address 0 to crash); gAssertLevel 1
// prints the message to stderr and execution continues.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kCharAnimatorSource, line);
    }
}

// Arrays::AnimTable records keep the listing stride of 20 bytes (see g1-game-anim-CharAnimator-2af513-0.cpp). Fields
// are decoded field by field as little-endian 32-bit words at their listing offsets (decisions 2 and 3 of TRACK-B.md).
constexpr std::size_t kAnimTableStride = 20;
constexpr std::size_t kAnimTableLoopCount = 0x04;   // +0x04 loop count given to a new stack entry
constexpr std::size_t kAnimTableStepCount = 0x08;   // +0x08 StepsSize
constexpr std::size_t kAnimTableKind = 0x10;        // +0x10 0 unique, 1 sequence, 2 random

std::int32_t ReadAnimTableWord(int animId, std::size_t listingOffset) {
    const std::uint8_t* record = Arrays::AnimTable::members + kAnimTableStride * static_cast<std::size_t>(animId);
    const std::uint8_t* p = record + listingOffset;
    std::uint32_t value = 0;
    for (std::size_t i = 0; i < 4; ++i) {
        value |= static_cast<std::uint32_t>(p[i]) << (8 * i);
    }
    return static_cast<std::int32_t>(value);
}

// The debug switch read used by the trace calls and the MP_MinimalRandoms test. GetInstance() calls load() and then
// returns the singleton, the same sequence as the original's load(&s_inst) followed by GetSwitch (the original's
// s_inst is private, see g1-game-anim-CharAnimator-2af513-2.cpp).
bool DebugSwitchSet(const char* name) {
    return DebugSwitches::GetInstance()->GetSwitch(name);
}

}  // namespace

// 0x3c94b8 _ZN12CharAnimator18_PlayItemSwooshSFXEP12ItemInstance
// partial: the original returns 0 for a null item, then reads the swoosh sound id from ItemInstance::GetItem() (Item
// +0x14), returns 0 when the id is -1, and otherwise calls VoxSoundManager::Play3D at the target position of m_character
// (GameObject::GetTargetPosition) and returns 1. Waits on dh2/ItemInstance.hpp (absent), and VoxSoundManager::Play3D is
// not declared, so only the null-item test is written. The stub return is 0 for every non-null item.
bool CharAnimator::_PlayItemSwooshSFX(ItemInstance* item) {
    if (item == nullptr) {
        return false;
    }
    return false;  // stub, see above
}

// 0x3c955c _ZN12CharAnimator17_PlayItemSwooshFXEP12ItemInstanceb
// partial: same shape as _PlayItemSwooshSFX. The original reads the swoosh FX id (Item +0x18), returns 0 for -1, and calls
// VisualFXManager::PlayAnimFXSet either on m_character (attached) or at the target position with the character's
// orientation field (Character +0x16c). Waits on dh2/VisualFXManager.hpp and dh2/ItemInstance.hpp (both absent).
bool CharAnimator::_PlayItemSwooshFX(ItemInstance* item, bool attached) {
    (void)attached;
    if (item == nullptr) {
        return false;
    }
    return false;  // stub, see above
}

// 0x3c9600 _ZNK12CharAnimator15ANIM_IsSequenceEj
// partial: the kind word of the stack entry's AnimTable record (+0x10) is read through ReadAnimTableWord, because the
// element class is not identified (same as ANIM_IsUnique in part 0). Assert 745 fires when m_animStackIdx < stackIdx.
// stackIdx is expected to stay below 3, the number of stack levels.
bool CharAnimator::ANIM_IsSequence(unsigned int stackIdx) const {
    if (m_stopped) {
        return false;
    }
    if (m_animStackIdx < stackIdx) {
        ReportAssert("stackIdx <= m_animStackIdx", 745);
    }
    return ReadAnimTableWord(m_animStack[stackIdx].m_animId, kAnimTableKind) == 1;
}

// 0x3c96dc _ZNK12CharAnimator13ANIM_IsRandomEj
// partial: as ANIM_IsSequence, kind 2, assert line 726.
bool CharAnimator::ANIM_IsRandom(unsigned int stackIdx) const {
    if (m_stopped) {
        return false;
    }
    if (m_animStackIdx < stackIdx) {
        ReportAssert("stackIdx <= m_animStackIdx", 726);
    }
    return ReadAnimTableWord(m_animStack[stackIdx].m_animId, kAnimTableKind) == 2;
}

// 0x3c97b8 _ZNK12CharAnimator13ANIM_IsUniqueEj
// partial: as ANIM_IsSequence, kind 0, assert line 707.
bool CharAnimator::ANIM_IsUnique(unsigned int stackIdx) const {
    if (m_stopped) {
        return false;
    }
    if (m_animStackIdx < stackIdx) {
        ReportAssert("stackIdx <= m_animStackIdx", 707);
    }
    return ReadAnimTableWord(m_animStack[stackIdx].m_animId, kAnimTableKind) == 0;
}

// 0x3c9890 _ZN12CharAnimator12SetCharacterEP9Character
// A null character asserts at line 433 (gAssertLevel 2 stops the program). The pointer is stored either way.
void CharAnimator::SetCharacter(Character* c) {
    if (c == nullptr) {
        ReportAssert("c", 433);
    }
    m_character = c;  // +0x04
}

// 0x3c9924 _ZN12CharAnimator9ANIM_StopEv
// Checked in the assembly: the stack depth is reset to 0 (+0x2c) before the visual object is tested. StopClip(true, 0) is
// the vtable slot +0x24 of the animation controller, and the event 34 is raised only when the character was playing.
void CharAnimator::ANIM_Stop() {
    m_animStackIdx = 0;                                    // +0x2c
    VisualObject* visual = m_character->m_visualObject;    // Character +0x2d8
    if (visual == nullptr) {
        return;
    }
    visual->m_animController->StopClip(true, 0);           // AnimController vtable +0x24
    if (!m_stopped) {                                      // +0x48, re-read after the call
        m_stopped = true;
        m_character->RaiseEvent(34, nullptr);
    }
}

// 0x3c9984 _ZN12CharAnimator15__CallbackEventERKN6glitch7collada15STriggeredEventEPv
// partial: the original stores the event's first word in m_lastEventData (+0x58) and raises Character event 40 with the
// event's second word. glitch::collada::STriggeredEvent has no definition, so neither word can be read here. The stub
// return is 0 (the original returns the RaiseEvent result).
int CharAnimator::__CallbackEvent(glitch::collada::STriggeredEvent const& ev, void* user) {
    (void)ev;
    (void)user;
    return 0;  // stub, see above
}

// 0x3c99a0 _ZN12CharAnimator25ANIM_AddSetToRenderObjectEv
// partial: the original returns at once when the character has no visual object. Otherwise it picks the controller
// class: AnimSetController when the character is a faerie or the AnimationBlendingDisabled switch is set (m_blended
// stays 0), BlendedAnimSetController otherwise (m_blended is 1). It then creates the controller on the visual's scene
// node with m_animSetId, installs it with VisualObject::SetAnimController and sets the callbacks through the controller's
// vtable slot +0x2c. Waits on dh2/AnimSetController.hpp and dh2/BlendedAnimSetController.hpp (both absent), and
// AnimController::SetCallbacks takes a void event callback, so only the decision and the m_blended store are written.
void CharAnimator::ANIM_AddSetToRenderObject() {
    VisualObject* visual = m_character->m_visualObject;    // Character +0x2d8
    if (visual == nullptr) {
        return;
    }
    // Same short-circuit order as the original: IsFaerie() first, the switch only when it is false.
    const bool animSetOnly = m_character->IsFaerie() || DebugSwitchSet("AnimationBlendingDisabled");
    m_blended = !animSetOnly;  // +0x54, set after the controller is created in the original
}

// 0x3c9af8 _ZN12CharAnimatorD0Ev
// skipped: the compiler-generated deleting destructor (D0). It runs the complete destructor D1 (0x3c90e8, shared with
// D2 0x3c90e4 in g1-game-anim-CharAnimator-2af513-0.cpp) and then CustomFree(this). C++ delete supplies it from the
// virtual destructor, so no body is written.

// 0x3c9b14 _ZN12CharAnimator21ANIM_AddAnimDictToSetEi
// partial: the set id is fetched once (+0x3c), and a non-negative dictionary is added to the set. The add is
// AnimSetManager::AddAnim(&Singleton<AnimSetManager>::s_inst, m_animSetId, animDict), which is missing because the
// singleton has no declaration.
void CharAnimator::ANIM_AddAnimDictToSet(int animDict) {
    if (m_animSetCompiled) {                                       // +0x38
        return;
    }
    if (m_animSetId == -1) {                                       // +0x3c
        m_animSetId = m_character->GetCharUniqueAnimSetId();
    }
    if (animDict >= 0) {
        // Missing: AnimSetManager::AddAnim(&Singleton<AnimSetManager>::s_inst, m_animSetId, animDict).
    }
}

// 0x3c9b7c _ZN12CharAnimator15_CompileAnimSetEv
// partial: the set id is fetched, the set is marked compiled, and ANIM_AddSetToRenderObject runs, in the original's order.
// The debug name, "CharAnimTable: (%d) %s || CharacterTable: (%d) %s", is missing: it needs the CharAnimTable name table
// (Arrays::CharAnimTable::m_memberNames, indexed by the anim table id), the character table id and name (Character
// +0x13c8 and +0x13c4), and AnimSetManager::DBG_SetName on the singleton.
void CharAnimator::_CompileAnimSet() {
    if (m_animSetCompiled) {                                       // +0x38
        return;
    }
    if (m_animSetId == -1) {                                       // +0x3c
        m_animSetId = m_character->GetCharUniqueAnimSetId();
    }
    m_animSetCompiled = true;                                      // +0x38
    ANIM_AddSetToRenderObject();
    // Missing: the debug name and AnimSetManager::DBG_SetName (see above).
}

// 0x3c9c7c _ZN12CharAnimator21_AddTemplateAnimTableEiij
// partial: the original does nothing for a negative anim table or an index past Arrays::AnimTable::size. Otherwise it
// asserts (line 200) when the record's StepsSize is not 1, then reads the single template step (+0x0c, the step table
// pointer, is a runtime address, so the step's Redir flag and anim id are not readable) and calls
// AnimSetManager::AddTemplateAnim on the singleton when the step is not a redirect. The add is missing.
void CharAnimator::_AddTemplateAnimTable(int animSetId, int animTable, unsigned int offset) {
    (void)animSetId;
    if (animTable < 0) {
        return;
    }
    const int index = static_cast<int>(offset) + animTable;
    if (index >= static_cast<int>(Arrays::AnimTable::size)) {
        return;
    }
    if (ReadAnimTableWord(index, kAnimTableStepCount) != 1) {
        ReportAssert("anim.StepsSize == 1", 200);
    }
    // Missing: the template step read and AnimSetManager::AddTemplateAnim(&Singleton<AnimSetManager>::s_inst, ...).
}

// 0x3c9d80 _ZN12CharAnimator13_AddAnimTableEiijjj
// partial: the range test, the stance test and the isTracingChar_Init trace read are written. The stance test is
// (stanceList & stanceMask) == stanceMask, or offset == 0 when that is false; the body runs only when it holds. The
// per-step loop is missing: a step with Redir set recurses with animId and zero offsets and stance masks, and any other
// step adds its anim to the set (AnimSetManager::AddAnim), loads its sound (VoxSoundManager::LoadSound) and registers
// its FX set (VisualFXManager::RegisterFXSetToLoad). The step records cannot be read (see the top of this file).
void CharAnimator::_AddAnimTable(int animSetId, int animTable, unsigned int offset, unsigned int stanceMask,
                                 unsigned int stanceList) {
    (void)animSetId;
    if (animTable < 0) {
        return;
    }
    const int index = static_cast<int>(offset) + animTable;
    if (index >= static_cast<int>(Arrays::AnimTable::size)) {
        return;
    }
    const bool stanceMatch = (stanceList & stanceMask) == stanceMask;
    if (!(stanceMatch || offset == 0)) {
        return;
    }
    (void)DebugSwitchSet("isTracingChar_Init");
    // Missing: the per-step loop over the anim table's step records (see above).
}

// 0x3c9f4c _ZN12CharAnimator15SetAnimationSetEv
// partial: the set id is fetched (+0x3c). The rest is missing. It needs AnimSetManager::Exists on the singleton, the stance
// count ("AnimStances", "COUNT_IPHONE" through the application's PyDataConstants, or 1 when IsPlayer() is false), the
// CharAnimTable row (fields 1 to 39 and the template id at field 36), the CharSkillList row (count at +0x04), and the
// _AddAnimTable and _AddTemplateAnimTable calls that use them.
void CharAnimator::SetAnimationSet() {
    m_animSetId = m_character->GetCharUniqueAnimSetId();  // +0x3c
    // Missing: the rest of the set build (see above).
}

// 0x3ca52c _ZN12CharAnimator16RequestAnimPurgeEv
// partial: returns at once when the character has no visual object. Otherwise the original takes the animation set from
// visual->m_animController->GetAnimationSet() (vtable +0x08) and calls AnimationSet::Purge on it, which is an empty body,
// and then releases it. The result is boost::intrusive_ptr<glitch::collada::CAnimationSet>, and the release needs the
// complete CAnimationSet, which is not declared (see IncAnimSetUsers in g1-game-anim-CharAnimator-2af513-0.cpp), so the
// call is not written.
void CharAnimator::RequestAnimPurge() {
    VisualObject* visual = m_character->m_visualObject;    // Character +0x2d8
    if (visual == nullptr) {
        return;
    }
    // Missing: GetAnimationSet() and its release (see above). AnimationSet::Purge has nothing to add.
}

// 0x3ca79c _ZN12CharAnimator12_SetAnimStepEj
// partial: the original returns when the anim has no step at that index (StepsSize <= step), otherwise it stores the
// step index in the stack entry and raises Character event 38. The step record, which decides the camera shake, the
// swoosh and item sounds, the sound and FX of the step, the timing, the redirect into _SetAnim, the animation set
// update and the controller playback, cannot be read (see the top of this file). The camera part waits on
// dh2/CameraLevel.hpp, and the item and FX calls wait on dh2/ItemInstance.hpp and dh2/VisualFXManager.hpp. The body
// stops after the event.
void CharAnimator::_SetAnimStep(unsigned int step) {
    StackEntry& top = m_animStack[m_animStackIdx];           // +0x08 + 12 * m_animStackIdx
    const unsigned int stepCount =
        static_cast<unsigned int>(ReadAnimTableWord(top.m_animId, kAnimTableStepCount));
    if (stepCount <= step) {
        return;
    }
    top.m_stepIndex = static_cast<int>(step);                // +0x10 + 12 * m_animStackIdx
    m_character->RaiseEvent(38, nullptr);
    // Missing: the step record path (see above).
}

// 0x3cab38 _ZN12CharAnimator8_SetAnimEij
// partial: the guard, the stack entry update, the loop count, the isTracingCharAnimator trace read and the event 36 are
// written. For a random anim (kind 2) without the MP_MinimalRandoms switch, the step is drawn with
// Random::GetRandom(StepsSize, synced). In the original, the synced argument is register r1, which this path never sets
// (its value is whatever the std::string cleanup left behind). The reconstruction passes false in its place. Any other
// case calls _SetAnimStep(0).
void CharAnimator::_SetAnim(int animId, unsigned int stackIdx) {
    if (!(animId >= 0 && static_cast<std::uint32_t>(animId) < Arrays::AnimTable::size && stackIdx <= 2)) {
        return;
    }
    m_currentAnim = animId;                                  // +0x4c
    m_animStackIdx = stackIdx;                               // +0x2c
    StackEntry& entry = m_animStack[stackIdx];
    entry.m_animId = animId;                                 // +0x08 + 12 * stackIdx
    entry.m_loopCount = ReadAnimTableWord(animId, kAnimTableLoopCount);  // +0x0c + 12 * stackIdx
    (void)DebugSwitchSet("isTracingCharAnimator");
    m_character->RaiseEvent(36, nullptr);
    bool randomPick = false;
    if (ReadAnimTableWord(animId, kAnimTableKind) == 2) {
        randomPick = !DebugSwitchSet("MP_MinimalRandoms");
    }
    if (randomPick) {
        const int stepsSize = ReadAnimTableWord(animId, kAnimTableStepCount);
        // partial: the synced argument (r1) is undefined in the original on this path. false is a placeholder.
        const bool syncedUndefinedPlaceholder = false;
        const unsigned int step = Random::GetRandom(stepsSize, syncedUndefinedPlaceholder);
        _SetAnimStep(step);
    } else {
        _SetAnimStep(0);
    }
}

// 0x3cacb0 _ZN12CharAnimator8ANIM_SetEi
// While an animation is finishing (m_animFinished, +0x49), the request is queued in m_pendingAnim (+0x50). Otherwise the
// speed is reset to 1.0 (+0x40) and the anim starts on stack level 0.
void CharAnimator::ANIM_Set(int animId) {
    if (m_animFinished) {                                    // +0x49
        m_pendingAnim = animId;                              // +0x50
        return;
    }
    m_speed = 1.0f;                                          // +0x40
    _SetAnim(animId, 0);
}

// 0x3caccc _ZN12CharAnimator9ANIM_SwapEii
// partial: the original first handles the old-anim test: a non-negative old id that differs from the current one swaps
// through ANIM_Set and keeps the speed. Otherwise it stores the new id in stack level 0 and checks the Loop (+0x04) and
// StepsSize (+0x08) fields against the old anim (asserts 899 and 900). With no redirect levels (m_animStackIdx == 0) the
// new id becomes m_currentAnim (+0x4c) and the function ends. With a deeper stack the original walks each level's
// redirect through the new anim's step records (Redir must be 1, assert 907), which cannot be read, so the body stops
// after level 0.
void CharAnimator::ANIM_Swap(int newAnimId, int oldAnimId) {
    if (newAnimId == -1) {
        return;
    }
    const int currentId = m_animStack[0].m_animId;           // +0x08
    if (oldAnimId != -1 && oldAnimId != currentId) {
        if (newAnimId != currentId) {
            const float speed = m_speed;                     // +0x40, read before ANIM_Set resets it
            ANIM_Set(newAnimId);
            ANIM_SetSpeed(speed);
        }
        return;
    }
    m_animStack[0].m_animId = newAnimId;                     // +0x08
    if (ReadAnimTableWord(newAnimId, kAnimTableLoopCount) != ReadAnimTableWord(currentId, kAnimTableLoopCount)) {
        ReportAssert("newAnim.Loop == oldAnim.Loop && \"New anim sequence does not match in format (Loop)\"", 899);
    }
    if (ReadAnimTableWord(newAnimId, kAnimTableStepCount) != ReadAnimTableWord(currentId, kAnimTableStepCount)) {
        ReportAssert("newAnim.StepsSize == oldAnim.StepsSize && \"New anim sequence does not match in format (Length)\"",
                     900);
    }
    if (m_animStackIdx > 0) {
        // Missing: the redirect walk through the new anim's step records (see above).
        return;
    }
    m_currentAnim = newAnimId;                               // +0x4c
}
