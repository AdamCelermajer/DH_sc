// Batch g1-game-anim-CharAnimator-2af513-1 (CharAnimator, part 2 of 3): the item swoosh helpers, the anim-set helpers,
// SetAnimationSet and the anim stack (0x3c94b8 to 0x3caccc). Declarations come from dh2/CharAnimator.hpp.
//
// Complete rows: _PlayItemSwooshFX, ANIM_IsSequence/ANIM_IsRandom/ANIM_IsUnique (uint), ANIM_AddSetToRenderObject,
// ANIM_AddAnimDictToSet, _AddTemplateAnimTable, ANIM_Swap, and SetCharacter, ANIM_Stop, ANIM_Set.
//
// Partial rows and what is missing (each is marked at its use):
//   The VoxSoundManager calls (Play3D, LoadSound) are written in _PlayItemSwooshSFX, _AddAnimTable and _SetAnimStep.
//   _CompileAnimSet: the debug name needs the 32-bit const char* at Character +0x13c4. That word falls inside
//     Character::m_charPropsName (std::string at +0x13b0), and the CharAnimTable name table is not read.
//   SetAnimationSet: the CharSkillList loop. Character::GetCharSkillList() returns void*, and no record type exists
//     for the skill list (count at +0x04, entries read through GetCharSkill).
//   _SetAnim: the random pick's synced argument is register r1, which the original does not set on that path. false is
//     a placeholder until the call is settled.
//
// Arrays::AnimTable records are host Structs::AnimTpl elements reached through the typed pointer (pydata
// element_table.hpp), never through 20-byte strides on the table bytes. Step tables are host Structs::AnimStep
// elements (structs_records.hpp), reached by typed index.
//
// ~CharAnimator (0x3c9af8) is the compiler-generated deleting destructor. Its body is the complete destructor and
// CustomFree(this), so no body is written here.

#include "dh2/CharAnimator.hpp"

#include "vox-audio/vox/sound_manager.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "dh2/AnimController.hpp"
#include "dh2/AnimationSet.hpp"
#include "glitch-scene-collada-gui/collada/CEventsManager.hpp"
#include "pydata-accessors/arrays.hpp"
#include "dh2/AnimSetController.hpp"
#include "dh2/AnimSetManager.hpp"
#include "dh2/Application.hpp"
#include "dh2/BlendedAnimSetController.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/ItemInstance.hpp"
#include "dh2/ItemInventory.hpp"
#include "dh2/Level.hpp"
#include "dh2/Random.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/VisualFXManager.hpp"
#include "dh2/VisualObject.hpp"
#include "pydata-accessors/structs_records.hpp"  // Structs::AnimStep, Structs::CharAnim, Structs::ItemBase
#include "pydata-accessors/w7_arrays_02.hpp"     // Arrays::AnimTable::members and size
#include "pydata-accessors/w7_pydata12.hpp"      // PyDataConstants::getConstant
#include "pydata-accessors/w7_structs_2.hpp"     // Structs::Item, the record ItemInstance::GetItem returns
#include "pydata-accessors/w7_structs_5.hpp"     // Structs::AnimTpl, the Arrays::AnimTable element

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

// The Arrays::AnimTable record of animId, a host Structs::AnimTpl element. m_w1 is the loop count (+0x04), m_w2 the
// step count (+0x08), m_w3 the step table (+0x0c) and m_w4 the kind (+0x10): 0 unique, 1 sequence, 2 random.
const Structs::AnimTpl& AnimTableRecord(int animId) {
    const auto* records = static_cast<const Structs::AnimTpl*>(static_cast<const void*>(Arrays::AnimTable::members));
    return records[animId];
}

// The step table of a record: host Structs::AnimStep elements. The fields follow the listing offsets: m_w1 swoosh
// attach flag (+0x04), m_w2 anim dictionary (+0x08), m_w3 time (+0x0c), m_w4 camera anim (+0x10), m_w6 FX set (+0x18),
// m_w7 flag (+0x1c), m_w8 and m_w9 camera anim array (+0x20, +0x24), m_w10 Redir (+0x28), m_w11 sound (+0x2c),
// m_w12 step speed (+0x30), m_w13 swoosh flag (+0x34).
const Structs::AnimStep* StepTable(const Structs::AnimTpl& record) {
    return static_cast<const Structs::AnimStep*>(record.Steps);
}

// The debug switch read in the original: DebugSwitches::load(&s_inst), then GetSwitch(&s_inst, name).
bool DebugSwitchSet(const char* name) {
    DebugSwitches::s_inst.load();
    return DebugSwitches::s_inst.GetSwitch(name);
}

// AnimController::EventCallback returns void and the controller discards any result, while __CallbackEvent returns the
// RaiseEvent result. This adapter is the void-typed form of the pointer the original registers.
void CallbackEventAdapter(const glitch::collada::STriggeredEvent& event, void* user) {
    (void)CharAnimator::__CallbackEvent(event, user);
}

}  // namespace

// 0x3c94b8 _ZN12CharAnimator18_PlayItemSwooshSFXEP12ItemInstance
// Returns 0 for a null item and for a sound id of -1 (Item +0x14, Structs::ItemBase m_w5). Otherwise it plays the sound
// in 3D at the target position of m_character (VoxSoundManager::Play3D(sound, pos, false, 1, -1, -1)) and returns 1.
// The original calls Play3D with no null check on s_instance; the check here only avoids a null dereference.
bool CharAnimator::_PlayItemSwooshSFX(ItemInstance* item) {
    if (item == nullptr) {
        return false;
    }
    const int soundId = item->GetItem()->SwooshSoundFX;  // Item +0x14
    if (soundId == -1) {
        return false;
    }
    const Point3D<float>& target = m_character->GetTargetPosition();
    const glitch::core::vector3d<float> at{target.X, target.Y, target.Z};
    if (VoxSoundManager::s_instance != nullptr) {
        VoxSoundManager::s_instance->Play3D(soundId, at, false, 1, -1.0f, -1.0f);
    }
    return true;
}

// 0x3c955c _ZN12CharAnimator17_PlayItemSwooshFXEP12ItemInstanceb
// Returns 0 for a null item and for a FX set id of -1 (Item +0x18, Structs::ItemBase m_w6). Attached, the set is played
// on m_character. Detached, it is played at the target position with the character's rotation (Character +0x16c) as
// origin. Both calls play through VisualFXManager::s_inst and return 1.
bool CharAnimator::_PlayItemSwooshFX(ItemInstance* item, bool attached) {
    if (item == nullptr) {
        return false;
    }
    const int fxId = item->GetItem()->SwooshFX;  // Item +0x18
    if (fxId == -1) {
        return false;
    }
    VisualFXManager& fx = VisualFXManager::s_inst;
    if (attached) {
        fx.PlayAnimFXSet(fxId, m_character, nullptr);
    } else {
        const Point3D<float>& target = m_character->GetTargetPosition();
        fx.PlayAnimFXSet(fxId, target, m_character->m_rotation, nullptr, nullptr);
    }
    return true;
}

// 0x3c9600 _ZNK12CharAnimator15ANIM_IsSequenceEj
// Not stopped and the kind (m_w4) of the AnimTable record of stack entry stackIdx is 1. Assert 745 fires when
// m_animStackIdx < stackIdx.
bool CharAnimator::ANIM_IsSequence(unsigned int stackIdx) const {
    if (m_stopped) {
        return false;
    }
    if (m_animStackIdx < stackIdx) {
        ReportAssert("stackIdx <= m_animStackIdx", 745);
    }
    return AnimTableRecord(m_animStack[stackIdx].m_animId).Type == 1;
}

// 0x3c96dc _ZNK12CharAnimator13ANIM_IsRandomEj
// As ANIM_IsSequence, kind 2, assert line 726.
bool CharAnimator::ANIM_IsRandom(unsigned int stackIdx) const {
    if (m_stopped) {
        return false;
    }
    if (m_animStackIdx < stackIdx) {
        ReportAssert("stackIdx <= m_animStackIdx", 726);
    }
    return AnimTableRecord(m_animStack[stackIdx].m_animId).Type == 2;
}

// 0x3c97b8 _ZNK12CharAnimator13ANIM_IsUniqueEj
// As ANIM_IsSequence, kind 0, assert line 707.
bool CharAnimator::ANIM_IsUnique(unsigned int stackIdx) const {
    if (m_stopped) {
        return false;
    }
    if (m_animStackIdx < stackIdx) {
        ReportAssert("stackIdx <= m_animStackIdx", 707);
    }
    return AnimTableRecord(m_animStack[stackIdx].m_animId).Type == 0;
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
// user is the CharAnimator. The event's first word (the time offset) is kept in m_lastEventData (+0x58) and Character event 40
// is raised with the second word (the event name).
int CharAnimator::__CallbackEvent(glitch::collada::STriggeredEvent const& ev, void* user) {
    CharAnimator* self = static_cast<CharAnimator*>(user);
    self->m_lastEventData = ev.timeOffset;  // +0x58
    return self->m_character->RaiseEvent(40, const_cast<char*>(ev.eventName));
}

// 0x3c99a0 _ZN12CharAnimator25ANIM_AddSetToRenderObjectEv
// Returns at once when the character has no visual object. Otherwise AnimSetController is chosen when the character is a
// faerie or the AnimationBlendingDisabled switch is set (m_blended stays 0), and BlendedAnimSetController otherwise
// (m_blended is 1). The controller is built on the visual's scene node with m_animSetId and installed with
// VisualObject::SetAnimController, which deletes the previous controller. The animation and event callbacks are then
// registered through the controller's vtable slot +0x2c.
void CharAnimator::ANIM_AddSetToRenderObject() {
    VisualObject* visual = m_character->m_visualObject;    // Character +0x2d8
    if (visual == nullptr) {
        return;
    }
    // Same short-circuit order as the original: IsFaerie() first, the switch only when it is false.
    const bool animSetOnly = m_character->IsFaerie() || DebugSwitchSet("AnimationBlendingDisabled");
    auto* node = visual->m_node;                           // VisualObject +0x08
    if (animSetOnly) {
        visual->SetAnimController(new AnimSetController(node, m_animSetId));
        m_blended = false;                                 // +0x54
    } else {
        visual->SetAnimController(new BlendedAnimSetController(node, m_animSetId));
        m_blended = true;                                  // +0x54
    }
    visual->m_animController->SetCallbacks(&CharAnimator::__Callback, this, &CallbackEventAdapter, this);  // slot +0x2c
}

// 0x3c9af8 _ZN12CharAnimatorD0Ev
// skipped: the compiler-generated deleting destructor (D0). It runs the complete destructor D1 (0x3c90e8, shared with
// D2 0x3c90e4 in g1-game-anim-CharAnimator-2af513-0.cpp) and then CustomFree(this). C++ delete supplies it from the
// virtual destructor, so no body is written.

// 0x3c9b14 _ZN12CharAnimator21ANIM_AddAnimDictToSetEi
// The set id is fetched once (+0x3c), and a non-negative dictionary is added to the set on the singleton.
void CharAnimator::ANIM_AddAnimDictToSet(int animDict) {
    if (m_animSetCompiled) {                                       // +0x38
        return;
    }
    if (m_animSetId == -1) {                                       // +0x3c
        m_animSetId = m_character->GetCharUniqueAnimSetId();
    }
    if (animDict >= 0) {
        AnimSetManager::s_inst.AddAnim(m_animSetId, animDict);
    }
}

// 0x3c9b7c _ZN12CharAnimator15_CompileAnimSetEv
// Not yet compiled: the set id is fetched, the set is marked compiled (+0x38), ANIM_AddSetToRenderObject runs and the set gets
// its debug name "CharAnimTable: (id) name || CharacterTable: (id) name" (Character +0x13c4 is the c_str of m_charPropsName).
void CharAnimator::_CompileAnimSet() {
    if (m_animSetCompiled) {                                       // +0x38
        return;
    }
    if (m_animSetId == -1) {                                       // +0x3c
        m_animSetId = m_character->GetCharUniqueAnimSetId();
    }
    m_animSetCompiled = true;                                      // +0x38
    ANIM_AddSetToRenderObject();
    char name[1024];
    const int tableId = m_character->GetCharAnimTableId();
    std::sprintf(name, "CharAnimTable: (%d) %s || CharacterTable: (%d) %s", tableId,
                 Arrays::CharAnimTable::m_memberNames.names[m_character->GetCharAnimTableId()],
                 static_cast<int>(m_character->m_charPropsId), m_character->m_charPropsName.c_str());
    AnimSetManager::s_inst.DBG_SetName(m_animSetId, name);
}

// 0x3c9c7c _ZN12CharAnimator21_AddTemplateAnimTableEiij
// The original does nothing for a negative anim table or an index past Arrays::AnimTable::size. Otherwise it asserts
// (line 200) when the record's StepsSize is not 1, then reads the first step and, when that step is not a redirect,
// adds its anim to the set through AnimSetManager::AddTemplateAnim.
void CharAnimator::_AddTemplateAnimTable(int animSetId, int animTable, unsigned int offset) {
    if (animTable < 0) {
        return;
    }
    const std::uint32_t index = offset + static_cast<std::uint32_t>(animTable);
    if (index >= Arrays::AnimTable::size) {
        return;
    }
    const Structs::AnimTpl& record = AnimTableRecord(static_cast<int>(index));
    if (record.StepsSize != 1) {
        ReportAssert("anim.StepsSize == 1", 200);
    }
    if (record.Steps == nullptr) {
        // Not reached with StepsSize 1. The original would read the null step table here and fault.
        return;
    }
    const Structs::AnimStep& first = StepTable(record)[0];
    if (first.Redir == 0) {  // Redir (+0x28) clear
        AnimSetManager::s_inst.AddTemplateAnim(animSetId, first.Anim);  // +0x08
    }
}

// 0x3c9d80 _ZN12CharAnimator13_AddAnimTableEiijjj
// The range test, the stance test ((stanceList & stanceMask) == stanceMask, or offset 0 when that is false), the
// isTracingChar_Init trace read and the step loop are written. A redirect step recurses with its anim and zero offsets
// and stance masks. Any other step adds its anim to the set (AnimSetManager::AddAnim) and registers its FX set
// (VisualFXManager::RegisterFXSetToLoad), and its sound (step +0x2c) is loaded when s_instance is set.
void CharAnimator::_AddAnimTable(int animSetId, int animTable, unsigned int offset, unsigned int stanceMask,
                                 unsigned int stanceList) {
    if (animTable < 0) {
        return;
    }
    const std::uint32_t index = offset + static_cast<std::uint32_t>(animTable);
    if (index >= Arrays::AnimTable::size) {
        return;
    }
    const bool stanceMatch = (stanceList & stanceMask) == stanceMask;
    if (!(stanceMatch || offset == 0)) {
        return;
    }
    const Structs::AnimTpl& record = AnimTableRecord(static_cast<int>(index));
    (void)DebugSwitchSet("isTracingChar_Init");
    const Structs::AnimStep* steps = StepTable(record);
    for (std::uint32_t i = 0; i < record.StepsSize; ++i) {
        const Structs::AnimStep& step = steps[i];
        if (step.Redir != 0) {  // Redir (+0x28)
            _AddAnimTable(animSetId, step.Anim, 0, 0, 0);
        } else {
            AnimSetManager::s_inst.AddAnim(animSetId, step.Anim);
            if (VoxSoundManager::s_instance != nullptr) {
                VoxSoundManager::s_instance->LoadSound(step.Sound);  // +0x2c
            }
            if (step.FX >= 0) {  // FX set (+0x18)
                VisualFXManager::s_inst.RegisterFXSetToLoad(step.FX);
            }
        }
    }
}

// 0x3c9f4c _ZN12CharAnimator15SetAnimationSetEv
// partial: the set id is fetched (+0x3c) and, when the set does not exist yet, the anim sets are built from the character's
// CharAnimTable row (Structs::CharAnim, typed words) in the original's order: the template and fixed anim tables, then one
// pass of _AddAnimTable per stance (the stance count is AnimStances COUNT_IPHONE for a player, else 1). Missing: the
// CharSkillList loop, which needs a record type for the skill list (GetCharSkillList returns void*). The skill list is
// fetched and discarded.
void CharAnimator::SetAnimationSet() {
    m_animSetId = m_character->GetCharUniqueAnimSetId();  // +0x3c
    const int setId = m_animSetId;
    if (AnimSetManager::s_inst.Exists(setId)) {
        return;
    }
    PyDataConstants* constants = Singleton<Application>::s_inst.m_pyDataConstants;  // Application +0x2c
    const int stanceCount = m_character->IsPlayer() ? constants->getConstant("AnimStances", "COUNT_IPHONE") : 1;
    const int stanceList = constants->getConstant("AnimStancedAnim", "SL__LIST_IPHONE");
    const Structs::CharAnim* table = m_character->GetCharAnimTable();
    const auto* skillList = static_cast<const Structs::SkillList*>(m_character->GetCharSkillList());  // host record: the count is ListSize (a raw int32[1] read the vptr half)

    int templateId = table->Template;  // +0x90
    if (templateId == -1) {
        (void)m_character->GetCharAnimTableId();
        templateId = table->Template;
    }
    _AddTemplateAnimTable(setId, templateId, 0);
    _AddAnimTable(setId, table->Limbus, 0, 0, 0);
    _AddAnimTable(setId, table->PreSpawn, 0, 0, 0);
    _AddAnimTable(setId, table->Spawn, 0, 0, 0);
    _AddAnimTable(setId, table->MenuIdle, 0, 0, 0);
    if (stanceCount != 0) {
        std::uint32_t stance = 0;
        do {
            _AddAnimTable(setId, table->Idle, stance, 0x2u, stanceList);
            _AddAnimTable(setId, table->Walk, stance, 0x10u, stanceList);
            _AddAnimTable(setId, table->Run, stance, 0x20u, stanceList);
            _AddAnimTable(setId, table->Attack, stance, 0x40u, stanceList);
            _AddAnimTable(setId, table->AttackStatic, stance, 0x80u, stanceList);
            _AddAnimTable(setId, table->Scared, stance, 0x100u, stanceList);
            _AddAnimTable(setId, table->Stunned, stance, 0x200u, stanceList);
            _AddAnimTable(setId, table->KnockedBack, stance, 0x400u, stanceList);
            _AddAnimTable(setId, table->GreatKnockedBack, stance, 0x800u, stanceList);
            _AddAnimTable(setId, table->Injured, stance, 0x1000u, stanceList);
            _AddAnimTable(setId, table->Blocking, stance, 0x2000u, stanceList);
            _AddAnimTable(setId, table->Dodging, stance, 0x4000u, stanceList);
            _AddAnimTable(setId, table->Died, stance, 0x8000u, stanceList);
            _AddAnimTable(setId, table->Despawn, stance, 0x10000u, stanceList);
            _AddAnimTable(setId, table->DeadlyGreatKB, stance, 0x20000u, stanceList);
            _AddAnimTable(setId, table->DespawnGreatKB, stance, 0x40000u, stanceList);
            _AddAnimTable(setId, table->Revived, stance, 0x100000u, stanceList);
            _AddAnimTable(setId, table->Reviving, stance, 0x80000u, stanceList);
            _AddAnimTable(setId, table->Walk180, stance, 0u, stanceList);
            _AddAnimTable(setId, table->Run180, stance, 0u, stanceList);
            _AddAnimTable(setId, table->IdleOOC, stance, 0x4u, stanceList);
            _AddAnimTable(setId, table->IdleToOOC, stance, 0x8u, stanceList);
            _AddAnimTable(setId, table->IdleFromOOC, stance, 0x8u, stanceList);
            _AddAnimTable(setId, table->IdleSneak, stance, 0u, stanceList);
            _AddAnimTable(setId, table->WalkSneak, stance, 0u, stanceList);
            _AddAnimTable(setId, table->RunSneak, stance, 0u, stanceList);
            _AddAnimTable(setId, table->LiftIdle, stance, 0x1000000u, stanceList);
            _AddAnimTable(setId, table->LiftMove, stance, 0x1000000u, stanceList);
            const auto* group = static_cast<const std::int32_t*>(table->Interact);  // +0x44 int32 array, m_w16 entries
            for (std::uint32_t i = 0; i < table->InteractSize; ++i) {
                _AddAnimTable(setId, group[i], stance, 0x800000u, stanceList);
            }
            const auto* extra = static_cast<const std::int32_t*>(table->Spells);  // +0x88 int32 array, m_w33 entries
            for (std::uint32_t i = 0; i < table->SpellsSize; ++i) {
                _AddAnimTable(setId, extra[i], stance, 0x400000u, stanceList);
            }
            for (std::int32_t i = 0; i < static_cast<std::int32_t>(skillList->ListSize); ++i) {
                const auto* skill = static_cast<const Structs::Skill*>(m_character->GetCharSkill(i));  // anim table: Structs::Skill::Anim
                if (skill == nullptr) continue;
                _AddAnimTable(setId, skill->Anim, stance, 0x200000u, stanceList);
            }
            ++stance;
        } while (stance != static_cast<std::uint32_t>(stanceCount));
    }
}

// 0x3ca52c _ZN12CharAnimator16RequestAnimPurgeEv
// Without a visual object nothing happens; otherwise the animation set of its anim controller is purged.
void CharAnimator::RequestAnimPurge() {
    VisualObject* visual = m_character->m_visualObject;    // Character +0x2d8
    if (visual == nullptr) {
        return;
    }
    boost::intrusive_ptr<AnimationSet> set = visual->m_animController->GetAnimationSet();
    set->Purge();
}

// 0x3ca79c _ZN12CharAnimator12_SetAnimStepEj
// The step index is stored in the top stack entry and event 38 is raised, as in the original. A redirect step (Redir,
// +0x28 == 1) pushes its anim one level up through _SetAnim. Otherwise the step plays its camera shake, swoosh and item
// sounds, its sound and FX, its controller timing and its playback. The camera part uses Level::m_camera (Level +0x128)
// and CameraLevel::CanPlayShakeAnim/PlayAnim. When the step has no fixed clip (m_w4 == -1) and a random set (count m_w8,
// int32 array m_w9), the clip is picked with Random::GetRandom(count, synced): the original passes -1 for `synced`, and
// GetRandom tests it for truth, so it takes the synced stream.
void CharAnimator::_SetAnimStep(unsigned int step) {
    StackEntry& top = m_animStack[m_animStackIdx];                     // +0x08 + 12 * m_animStackIdx
    const Structs::AnimTpl& record = AnimTableRecord(top.m_animId);
    if (static_cast<unsigned int>(record.StepsSize) <= step) {               // StepsSize <= step
        return;
    }
    top.m_stepIndex = static_cast<int>(step);                           // +0x10 + 12 * m_animStackIdx
    m_character->RaiseEvent(38, nullptr);
    const Structs::AnimStep& s = StepTable(record)[step];

    if (s.Redir == 1) {                                                 // Redir: push the redirect one level up
        _SetAnim(s.Anim, m_animStackIdx + 1);
        return;
    }

    if (s.Cam != -1 || s.RandomCamSize != 0) {                                  // camera shake anim, or a random set of them
        Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
        CameraLevel* camera = (level != nullptr) ? level->m_camera : nullptr;  // Level +0x128
        if (camera != nullptr && camera->CanPlayShakeAnim(m_character)) {
            if (s.Cam != -1) {
                camera->PlayAnim(s.Cam, 0, true);
            } else if (s.RandomCamSize != 0) {
                const auto* clips = static_cast<const std::int32_t*>(s.RandomCam);
                camera->PlayAnim(clips[Random::GetRandom(static_cast<int>(s.RandomCamSize), true)], 0, true);
            }
        }
    }

    m_swooshActive = s.Swoosh;                                           // +0x30, copy of the step's swoosh flag
    bool playStepSound = true;                                          // a clear swoosh flag falls through to the sound
    bool playStepFx = true;                                             // v12 in the original
    if (s.Swoosh) {
        ItemInstance* mainHand = m_character->GetEquippedItem(1u);
        ItemInstance* offHand = m_character->GetEquippedItem(2u);
        const bool mainSfxMissing = !_PlayItemSwooshSFX(mainHand);
        const bool mainFxPlayed = _PlayItemSwooshFX(mainHand, s.AnchorFX);
        playStepFx = !mainFxPlayed;
        playStepSound = mainSfxMissing;
        if (mainSfxMissing) {
            playStepSound = !_PlayItemSwooshSFX(offHand);
        }
        if (playStepFx) {
            playStepSound = !_PlayItemSwooshFX(offHand, s.AnchorFX);
        }
    }

    if (playStepSound && VoxSoundManager::s_instance != nullptr) {
        const Point3D<float>& target = m_character->GetTargetPosition();
        const glitch::core::vector3d<float> at{target.X, target.Y, target.Z};
        VoxSoundManager::s_instance->Play3D(s.Sound, at, false, 1, -1.0f, -1.0f);  // +0x2c
    }

    if (playStepFx && s.FX != -1) {                                   // FX set (+0x18)
        VisualFXManager& fx = VisualFXManager::s_inst;
        if (s.AnchorFX) {
            fx.PlayAnimFXSet(s.FX, m_character, nullptr);
        } else {
            fx.PlayAnimFXSet(s.FX, m_character->GetTargetPosition(), m_character->m_rotation, nullptr, nullptr);
        }
    }

    m_stepSpeed = s.Speed;                                              // +0x34, step speed (+0x30)
    VisualObject* visual = m_character->m_visualObject;                 // Character +0x2d8
    if (visual == nullptr || s.Anim == -1) {
        ANIM_Stop();
        return;
    }
    m_stopped = false;                                                  // +0x48
    _CompileAnimSet();
    AnimController* controller = visual->m_animController;              // VisualObject +0x38
    if (m_blended) {
        auto* blended = static_cast<BlendedAnimSetController*>(controller);
        blended->unk_14 = (m_animFinished || m_stopLoop) ? 0 : s.BlendOut;  // +0x14 cross-fade, zero while finishing
        blended->unk_0c = 0;                                            // +0x0c
        blended->unk_10 = s.MoveGO;                                       // +0x10
    } else {
        auto* animSet = static_cast<AnimSetController*>(controller);
        animSet->unk_0c = static_cast<int>(m_blended);                  // +0x0c, m_blended is 0 on this path
        animSet->unk_10 = s.MoveGO;                                       // +0x10
    }
    controller->PlayClip(static_cast<unsigned int>(s.Anim), false, m_extraTime, 0u);  // vtable +0x1c, loop 0
    controller->SetScale(m_speed * m_stepSpeed, 0u);                                  // vtable +0x28
}

// 0x3cab38 _ZN12CharAnimator8_SetAnimEij
// The guard, the stack entry update, the loop count (Loop, +0x04), the isTracingCharAnimator trace read and the
// event 36 are written. For a random anim (kind 2) without the MP_MinimalRandoms switch the step is drawn with
// Random::GetRandom(StepsSize, synced), and otherwise _SetAnimStep(0) runs.
//
// RESOLVED (this was the only blocker): the synced argument is register r1 and the original NEVER SETS IT on this
// path. The listing has LDR R0,[R8,#8] at 0x3cac78 and BL Random::GetRandom at 0x3cac7c with no MOV R1 anywhere
// between the last R1 write (MOV R1,R7 at 0x3cac58, for the earlier GetSwitch call) and that branch, so the
// original reads whatever the register happens to hold. The body is faithful; a host port must pass something, so
// it passes false. That is a divergence inherent to the original's undefined behaviour, recorded rather than
// guessed at.
void CharAnimator::_SetAnim(int animId, unsigned int stackIdx) {
    if (!(animId >= 0 && static_cast<std::uint32_t>(animId) < Arrays::AnimTable::size && stackIdx <= 2)) {
        return;
    }
    m_currentAnim = animId;                                 // +0x4c
    m_animStackIdx = stackIdx;                              // +0x2c
    const Structs::AnimTpl& record = AnimTableRecord(animId);
    StackEntry& entry = m_animStack[stackIdx];
    entry.m_animId = animId;                                // +0x08 + 12 * stackIdx
    entry.m_loopCount = record.Loop;                        // +0x0c + 12 * stackIdx (Loop, +0x04 of the record)
    (void)DebugSwitchSet("isTracingCharAnimator");
    m_character->RaiseEvent(36, nullptr);
    bool randomPick = false;
    if (record.Type == 2) {
        randomPick = !DebugSwitchSet("MP_MinimalRandoms");
    }
    if (randomPick) {
        const int stepsSize = static_cast<int>(record.StepsSize);
        // The original passes r1 here without ever setting it (see the note above); a host port must pass
        // something, so it passes false.
        const bool syncedUndefinedInOriginal = false;
        const unsigned int step = Random::GetRandom(stepsSize, syncedUndefinedInOriginal);
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
// A non-negative old id that differs from the current one swaps through ANIM_Set and keeps the speed. Otherwise the new
// id is stored level by level down the stack. Each level's record is checked against the old one (Loop, assert 899, and
// StepsSize, assert 900). A level below the top takes the next id from the step at its step index, which must be a
// redirect (assert 907). The top level sets m_currentAnim.
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
    int newId = newAnimId;
    int oldId = currentId;
    for (unsigned int level = 0;; ++level) {
        m_animStack[level].m_animId = newId;                 // +0x08 + 12 * level
        const Structs::AnimTpl& newRecord = AnimTableRecord(newId);
        const Structs::AnimTpl& oldRecord = AnimTableRecord(oldId);
        if (newRecord.Loop != oldRecord.Loop) {
            ReportAssert("newAnim.Loop == oldAnim.Loop && \"New anim sequence does not match in format (Loop)\"", 899);
        }
        if (newRecord.StepsSize != oldRecord.StepsSize) {
            ReportAssert("newAnim.StepsSize == oldAnim.StepsSize && \"New anim sequence does not match in format (Length)\"",
                         900);
        }
        const unsigned int top = m_animStackIdx;             // +0x2c
        if (top > level) {
            const Structs::AnimStep& step = StepTable(newRecord)[m_animStack[level].m_stepIndex];
            if (step.Redir != 1) {                           // Redir (+0x28)
                ReportAssert("newAnim.Steps[info.stepIdx].Redir == Constants::AnimRedir::IsRedir && "
                             "\"New anim sequence does not match in format (Depth)\"",
                             907);
            }
            newId = step.Anim;
        } else {
            m_currentAnim = newId;                           // +0x4c
        }
        if (level + 1 > top) {
            break;
        }
        oldId = m_animStack[level + 1].m_animId;
    }
}
