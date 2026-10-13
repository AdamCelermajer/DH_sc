// Batch g1-game-world-TriggerObject-bcf06e-0: the TriggerObject methods of libDungeonHunter2.so (the two constructors, the
// destructor, the Trigger and GameObject overrides, Interact and TestInteractiveCond). Declarations come from
// dh2/TriggerObject.hpp, which needs no change for this batch. Offsets in comments are the original 32-bit ones.
//
// Layout facts used here (named in dh2/TriggerObject.hpp, dh2/Trigger.hpp, dh2/GameObject.hpp and dh2/ObjectBase.hpp):
//   TriggerObject +0x718 m_data (std::string), +0x730 m_dataIndex, +0x734 m_script, +0x74c m_scriptId, +0x750 m_scriptOff,
//           +0x768 m_scriptOffId, +0x76c m_unlockCond, +0x784 m_unlocked, +0x788 m_unlockConditions
//   Trigger +0x3b4 m_numActivations, +0x3b8 m_timer
//   GameObject +0x274 m_spawnProb, +0x2d8 m_visualObject, +0x2e4 m_pendingInteractor, +0x300 m_script (LuaScript*),
//           +0x373 m_idleSoundMuted
//   ObjectBase +0x28 unk_28, +0x64 unk_64 (the object id passed to the scripts and the quest event), +0x84 m_static, +0x85 unk_85
//
// Name clash: TriggerObject::m_script (std::string, +0x734) hides GameObject::m_script (LuaScript*, +0x300). The Lua call in
// Interact reads GameObject::m_script.
//
// Include constraints (the binding limit of this batch). dh2/Trigger.hpp includes net-transport/net_struct.hpp, which defines
// an inline CustomFree (net-transport/deps/byte_array.hpp). The pydata record headers (pydata-accessors/w7_structs_5.hpp,
// w7_structs_2.hpp, w7_arrays27.hpp, w7_arrays_02.hpp) and dh2/Character.hpp (through CharProperties and structs_records.hpp)
// define a second inline CustomFree in pydata-accessors/deps/stream_reader.hpp, and the two cannot share a translation unit.
// core-platform/memory.hpp also clashes (glitch_core types). So the record types (Structs::TriggerObject, ColladaFile,
// v2CondAnd) and the complete Character are not included here, and the functions that need them are partial. The Arrays
// name lookups are declared by hand, as the DestructibleContainer and OpenableContainer batches do. The explicit
// instantiations for TriggerObjects and v2Conditions are in pydata-accessors/src/member_ids.cpp. dh2/IStreamBase.hpp is not
// included.

#include "dh2/TriggerObject.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

#include "dh2/AnimController.hpp"
#include "dh2/Application.hpp"
#include "dh2/ConditionList.hpp"
#include "dh2/LuaScript.hpp"
#include "dh2/PODecor.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"  // dword_99F76C, the PlayerManager instance
#include "dh2/global/ctrl_camera.hpp"        // ApplicationGetInstance(), the Application singleton accessor
#include "dh2/sfc/script/lua/Arguments.hpp"

// Engine-wide assert level (core-platform/memory.hpp is not included: see the note above).
extern int gAssertLevel;

namespace Arrays {
struct TriggerObjects;  // element table in pydata-accessors/w7_arrays27.hpp (not included here)
struct v2Conditions;    // element table in pydata-accessors/w7_arrays_02.hpp (not included here)
// The explicit instantiations are in pydata-accessors/src/member_ids.cpp.
template <class T>
int GetMemberIDByString(const char* name);
}  // namespace Arrays

namespace {

const char kTriggerObjectSource[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Triggers\\TriggerObject.cpp";

// The original ASSERT(expression) at TriggerObject.cpp:line. Level 2 stops the process (the original stores to address 0);
// level 1 prints the message.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kTriggerObjectSource, line);
    }
}

}  // namespace

// 0x399318 TriggerObject::IsUpdatable() const
bool TriggerObject::IsUpdatable() const {
    return true;
}

// 0x399320 TriggerObject::IsZonable() const
bool TriggerObject::IsZonable() const {
    return true;
}

// 0x399328 TriggerObject::IsAnimated() const
bool TriggerObject::IsAnimated() const {
    return true;
}

// 0x399330 TriggerObject::GetInteractionType(GameObject*) const
// PARTIAL. The original returns m_dataIndex (+0x730) when it is -1, and otherwise the interaction type of the data row
// (+0x4 of Arrays::TriggerObjects, stride 24). The row is Structs::TriggerObject (pydata w7_structs_5.hpp), which cannot
// share a translation unit with dh2/Trigger.hpp (see the note at the top), so the row is not read. -1 is returned in its
// place for every m_dataIndex, which is exact for the -1 case only.
int TriggerObject::GetInteractionType(GameObject* other) const {
    (void)other;
    return -1;  // +0x730 is -1 here. A data row's interaction type (row +0x4) is not bound.
}

// 0x39939c TriggerObject::InterpretIncomingNetStruct(bool)
// Trigger::InterpretIncomingNetStruct, then m_unlocked (+0x784) is cleared when the argument is set.
void TriggerObject::InterpretIncomingNetStruct(bool force) {
    Trigger::InterpretIncomingNetStruct(force);
    if (force) {
        m_unlocked = false;  // +0x784
    }
}

// 0x3993bc TriggerObject::TestInteractiveCond()
// PARTIAL. The original takes the local player's Character (PlayerInfo +0x660, unk_660 of dword_99F76C->GetLocalPlayer(0,
// true)) and tests its PlayerSavegame: the object is locked when the savegame is missing or its byte +0x14 (m_questSynced)
// is clear. That test needs the complete Character (its m_savegame at +0x14e8), which cannot share a translation unit with
// dh2/Trigger.hpp (see the note at the top), so it is not bound. The branch with no character is bound: the gate passes, the
// unlock ConditionList decides m_unlocked, and an unlocked object plays "unlock" once on its animation and clears the
// idle-sound mute (+0x373). When a character exists, m_unlocked is left as it was.
void TriggerObject::TestInteractiveCond() {
    Character* character = dword_99F76C->GetLocalPlayer(0, true)->unk_660;  // PlayerInfo +0x660
    if (character != nullptr) {
        // PARTIAL, not bound: the savegame gate (Character +0x14e8 PlayerSavegame, byte +0x14). See the function note.
        return;
    }
    if (m_unlockConditions != nullptr && !m_unlockConditions->Eval()) {  // +0x788
        m_unlocked = false;  // +0x784
        return;
    }
    m_unlocked = true;  // +0x784
    if (m_visualObject != nullptr) {  // +0x2d8
        m_visualObject->m_animController->PlayClip("unlock", false, 0, 0);  // VisualObject +0x38, vtable slot +0x20
    }
    m_idleSoundMuted = 0;  // +0x373
}

// 0x399490 TriggerObject::IsInteractive(GameObject*) const
// Trigger::CanActivate, then m_unlocked. The original returns CanActivate's zero when it is false.
bool TriggerObject::IsInteractive(GameObject* other) const {
    (void)other;
    if (Trigger::CanActivate()) {
        return m_unlocked;  // +0x784
    }
    return false;
}

// 0x3994a8 TriggerObject::Update()
// PARTIAL, inherited: the locked branch calls TestInteractiveCond (0x3993bc), which is partial (its savegame gate is not bound).
// The rest is written. The delay timer (+0x3b8) is read before Trigger::Update counts it down. An unlocked object that was
// delayed and can now activate plays "idle" (loop, no unused argument) and clears the idle-sound mute. The visual is then
// synced, and a pending interactor (+0x2e4) is passed to Interact (virtual) and cleared.
void TriggerObject::Update() {
    const int timerBefore = m_timer;  // +0x3b8
    Trigger::Update();
    if (m_unlocked) {  // +0x784
        if (timerBefore > 0 && Trigger::CanActivate()) {
            if (m_visualObject != nullptr) {  // +0x2d8
                m_visualObject->m_animController->PlayClip("idle", true, 0, 0);
            }
            m_idleSoundMuted = 0;  // +0x373
        }
    } else {
        TestInteractiveCond();
    }
    if (m_visualObject != nullptr) {
        m_visualObject->Sync();
    }
    if (m_pendingInteractor != nullptr) {  // +0x2e4
        Interact(m_pendingInteractor);     // vtable slot +0x98
        m_pendingInteractor = nullptr;
    }
}

// 0x399580 TriggerObject::Deserialize(IStreamBase*)
// PARTIAL, inherited: Trigger::Deserialize (0x39890c) is partial (its level test and the two timer reads are missing), and
// the condition test it runs is TestInteractiveCond (partial, see its note). The animation the state selects is written.
void TriggerObject::Deserialize(IStreamBase* stream) {
    Trigger::Deserialize(stream);
    TestInteractiveCond();
    if (m_visualObject == nullptr) {  // +0x2d8
        return;
    }
    AnimController* anim = m_visualObject->m_animController;  // VisualObject +0x38
    if (Trigger::CanActivate()) {
        if (m_unlocked) {  // +0x784
            anim->PlayClip("idle", true, 0, 0);
        } else {
            anim->PlayClip("idlelocked", true, m_unlocked, m_unlocked);
        }
    } else if (!anim->PlayClip("idleactive", true, 0, 0)) {
        anim->PlayClip("activate", false, 0, 0);
    }
}

// 0x3996a4 TriggerObject::Serialize(IStreamBase*)
// Thunk: a tail branch to Trigger::Serialize (0x398978) with the same arguments.
void TriggerObject::Serialize(IStreamBase* stream) {
    Trigger::Serialize(stream);
}

// 0x39993c TriggerObject::~TriggerObject()   (complete object destructor, D1)
// 0x3999f4 TriggerObject::~TriggerObject()   (base object destructor, D2): the same body.
// The unlock ConditionList is destroyed and released with CustomFree, and then the compiler destroys the four property
// strings (+0x76c, +0x750, +0x734, +0x718) in reverse declaration order and runs Trigger::~Trigger, as the original does.
// 0x3999d8 (D0, deleting) is compiler generated and is skipped; see the ledger.
TriggerObject::~TriggerObject() {
    ConditionList* conditions = m_unlockConditions;  // +0x788
    if (conditions != nullptr) {
        conditions->~ConditionList();
        CustomFree(conditions);
        m_unlockConditions = nullptr;
    }
}

// 0x399af0 TriggerObject::Interact(GameObject*)
// PARTIAL, two pieces are missing:
//   1. VoxSoundManager::Play3D for the data row's sound (row +0x10) at m_position, when m_dataIndex != -1. VoxSoundManager::
//      s_instance and Play3D are not declared in vox-audio, and the row is not readable here (see the note at the top).
//   2. The TriggerOn quest event raise on the level. The original builds a stack QE_TriggerOn (vtable _ZTV12QE_TriggerOn)
//      with eventId_ = PyDataConstants::getConstant("v2QuestObjectiveType", "TriggerOn") (+0x04), m_character null (+0x08),
//      m_targetId = unk_64 (+0x0c), m_flag0 = m_flag1 = 0 (+0x10, +0x11), m_value = -1 (+0x14) and m_matchId = m_dataIndex
//      (+0x18), then calls EventManager::RaiseAsync. The id cannot be set from here: net-session-online/i_event.hpp keeps
//      eventId_ protected (the same blocker as OpenableContainer::Interact). Layout request: QuestEvent.hpp should take
//      dh2/IEvent.hpp, which declares eventId_ public.
// Written here: the Lua OnTrigger call, Trigger::Activate, the activate animation, the script choice and the level assert.
void TriggerObject::Interact(GameObject* other) {
    if (GameObject::m_script != nullptr && Trigger::CanActivate()) {  // GameObject +0x300
        sfc::script::lua::Arguments args;
        args.pushUserData(other);
        GameObject::m_script->Call("OnTrigger", args);
    }
    Trigger::Activate();
    if (m_visualObject != nullptr) {  // +0x2d8
        m_visualObject->m_animController->PlayClip("activate", false, 0, 0);
    }
    if (m_dataIndex != -1) {
        // PARTIAL: VoxSoundManager::Play3D (see the function note).
    }
    if (m_scriptOffId == -1 || (m_numActivations & 1) == 0) {  // +0x768, +0x3b4
        if (m_scriptId != -1) {  // +0x74c
            ScriptManager::s_inst.StartScript(m_scriptId, unk_64, false);
        }
        if (ApplicationGetInstance().GetCurrentLevel() == nullptr) {
            ReportAssert("level", 286);
        }
        // PARTIAL: the TriggerOn quest event raise (see the function note).
        m_idleSoundMuted = 1;  // +0x373
    } else {
        ScriptManager::s_inst.StartScript(m_scriptOffId, unk_64, false);
    }
}

// 0x399d70 TriggerObject::DeclareProperties()
// Trigger's properties first, then the four strings: "data" (+0x718), "script" (+0x734), "scriptOff" (+0x750) and
// "unlock_cond" (+0x76c).
void TriggerObject::DeclareProperties() {
    Trigger::DeclareProperties();
    AddProperty<std::string>("data", m_data);
    AddProperty<std::string>("script", m_script);
    AddProperty<std::string>("scriptOff", m_scriptOff);
    AddProperty<std::string>("unlock_cond", m_unlockCond);
}

// 0x399df0 TriggerObject::TriggerObject()   (complete object constructor, C1)
// 0x399ef4 TriggerObject::TriggerObject()   (base object constructor, C2): the same body.
// PARTIAL, inherited: the Trigger base constructor (0x398f44) is partial (its NetStructTrigger members build with a stale
// value word, see g1-game-world-Trigger-d3f06a-0.cpp). The TriggerObject stores are complete. The original does not store
// m_scriptOffId (+0x768) here, so Interact reads it only after InitPost has set it, as in the original.
TriggerObject::TriggerObject()
    : Trigger(static_cast<ObjectBase::GO_IDS>(20), false, true) {
    m_dataIndex = -1;              // +0x730
    m_scriptId = -1;               // +0x74c
    m_static = false;              // +0x84
    unk_85 = 1;                    // +0x85 (ObjectBase updating flag)
    m_unlocked = false;            // +0x784
    m_unlockConditions = nullptr;  // +0x788
    unk_28 = 1;                    // +0x28 (ObjectBase; Trigger's constructor also stores it)
}

// 0x399ff8 TriggerObject::InitPost()
// PARTIAL. Written: the spawn check, the data row lookup (Arrays::TriggerObjects, first name match), the script ids, the
// Trigger base InitPost, the SetVisible(false) branch, the idle or idlelocked animation and the PODecor body (a static box,
// see dh2/PODecor.hpp). Not bound, each for the reason in the note at the top:
//   1. The GameObjectDict name copied into m_dae (+0x290) from the data row's dict index (row +0x14): the GameObjectDict record
//      (Structs::ColladaFile) and the data row (Structs::TriggerObject) are not includable here.
//   2. The unlock ConditionList for a named unlock_cond (Arrays::v2Conditions, row +0x4 count and +0x8 stubs): the v2CondAnd
//      record is not includable here, so no ConditionList is built and m_unlockConditions stays null.
//   3. VoxSoundManager::LoadSound for the data row's sound id (row +0x10): VoxSoundManager::s_instance and LoadSound are not
//      declared in vox-audio.
//   4. The external script load of the visible branch, LoadExternalScript(row +0xc name, "data/scripts/objects/"): the row
//      is not readable here.
void TriggerObject::InitPost() {
    const int spawn = CheckSpawnProbability();  // GameObject::CheckSpawnProbability
    if (!(spawn < m_spawnProb)) {               // +0x274
        return;
    }

    m_dataIndex = Arrays::GetMemberIDByString<Arrays::TriggerObjects>(m_data.c_str());  // +0x730, -1 when no row name matches

    m_scriptId = ScriptManager::s_inst.GetIDFromName(m_script.c_str(), false);        // +0x74c
    m_scriptOffId = ScriptManager::s_inst.GetIDFromName(m_scriptOff.c_str(), false);  // +0x768
    Trigger::InitPost();

    // PARTIAL: an unlock_cond that is neither empty nor "Invalid" builds the unlock ConditionList (item 2 above).
    if (m_unlockCond.empty() || m_unlockCond == "Invalid") {  // +0x76c
        m_unlocked = true;  // +0x784
    }

    if (m_dataIndex == -1 || !MeetCondition()) {
        SetVisible(false);  // vtable slot +0x40 (virtual)
        return;
    }

    if (m_visualObject != nullptr) {  // +0x2d8
        AnimController* anim = m_visualObject->m_animController;
        if (m_unlocked) {  // +0x784
            anim->PlayClip("idle", true, 0, 0);
        } else {
            anim->PlayClip("idlelocked", true, m_unlocked, m_unlocked);
        }
    }

    // The physics body is the static box of PODecor (group 0, category 2, mask 0xffff), owned by the object.
    PODecor* body = new PODecor(ApplicationGetInstance().m_physicalWorld, this, false);
    SetPhysicalObject(body, false);
    // PARTIAL: the sound load (item 3) and the external script load (item 4) are not bound.
}
