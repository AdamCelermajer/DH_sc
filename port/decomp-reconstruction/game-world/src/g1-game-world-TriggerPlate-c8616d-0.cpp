// Batch g1-game-world-TriggerPlate-c8616d-0 (part 1 of 1): the TriggerPlate row readers, the contact counting, the
// object test, Update, the data-row lookup (partial), the constructors, the destructors, DeclareProperties and
// InitPost (partial). Declarations come from dh2/TriggerPlate.hpp, whose members sit at their original offsets.
//
// Layout facts used here (original 32-bit offsets, named in dh2/TriggerPlate.hpp and its bases):
//   ObjectBase +0x64 unk_64 (the object id passed to ScriptManager::StartScript), +0xf4 m_goId
//   GameObject +0x274 m_spawnProb, +0x2d8 m_visualObject; VisualObject +0x38 m_animController
//   Zone +0x381 m_playerOnly; ZoneEx +0x388 m_objectsInside (the set node count at +0x398)
//   Trigger +0x3a8 m_triggerCount, +0x3b8 m_timer, +0x3c0 unk_3c0 (the replicated contact count)
//   TriggerPlate +0x730 m_dataIndex, +0x734 m_numberOfObjects, +0x768 m_triggerType, +0x76c m_scriptOnId,
//                +0x770 m_scriptOffId, +0x774 m_lastNumContacts, +0x778 m_activated, +0x779 m_enabled
//   Arrays::TriggerPlates rows: file stride 32, read as host Structs::TriggerPlate elements (w7_arrays27.hpp):
//     m_w1 +0x04 behaviour, m_w2 +0x08 enabled, m_w3 +0x0c sound off, m_w4 +0x10 sound on,
//     m_w5 +0x14 specific object, m_w6 +0x18 trigger type, m_w7 +0x1c visual (a GameObjectDict index).
//
// Object type 6 is LiftableObject (the GO id its factory passes, game/include/dh2/global/object_factory.hpp), and its
// vtable slot 0xc8 is LiftableObject::GetDataId. IsObjectActivating relies on both facts.
//
// The original's ASSERT in Update prints through ReportAssert, as the other batches do.

// Trigger.hpp (through net-transport's net_struct.hpp) defines CustomFree and MemoryHintState, and so do the pydata
// headers included below (stream_reader.hpp). As in Character.hpp, the net-transport copies are renamed while they are
// included, so the two sets do not clash in this translation unit.
#define CustomFree CustomFree_net_transport
#define MemoryHintState MemoryHintState_net_transport
#include "dh2/TriggerPlate.hpp"
#undef CustomFree
#undef MemoryHintState

#include <cstdio>
#include <cstdlib>

#include "dh2/AnimController.hpp"
#include "dh2/Application.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/LiftableObject.hpp"
#include "dh2/ObjectBase.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"  // dword_99F76C, the PlayerManager instance
#include "dh2/global/ctrl_camera.hpp"        // Application& ApplicationGetInstance()
#include "pydata-accessors/structs_records.hpp"  // Structs::TriggerPlate, the host element of Arrays::TriggerPlates
#include "pydata-accessors/w7_arrays27.hpp"      // Arrays::TriggerPlates::members and size

// Global assert level (libDungeonHunter2.so gAssertLevel). Declared the same way as in CameraLevel.cpp.
extern int gAssertLevel;

namespace {

// The source path the original's ASSERT messages print (TriggerPlate.cpp in the Visual Studio project).
constexpr const char kTriggerPlateSource[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Triggers\\TriggerPlate.cpp";

// The original ASSERT. gAssertLevel 2 stops the program (the original stores to address 0 to crash), level 1 prints
// the message and the function goes on.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kTriggerPlateSource, line);
    }
}

// Row `index` of Arrays::TriggerPlates as the host element type. The caller has checked index != -1.
const Structs::TriggerPlate& TriggerPlateRow(int index) {
    return reinterpret_cast<const Structs::TriggerPlate*>(Arrays::TriggerPlates::members)[index];
}

}  // namespace

// 0x39a394 _ZNK12TriggerPlate10IsAnimatedEv
bool TriggerPlate::IsAnimated() const {
    return true;
}

// 0x39a39c _ZNK12TriggerPlate11IsUpdatableEv
bool TriggerPlate::IsUpdatable() const {
    return true;
}

// 0x39a3a4 _ZNK12TriggerPlate9GetVisualEv
// The visual is row m_w7 (+0x1c), a GameObjectDict index. -1 when m_dataIndex is -1.
int TriggerPlate::GetVisual() const {
    if (m_dataIndex == -1) {
        return -1;
    }
    return TriggerPlateRow(m_dataIndex).m_w7;
}

// 0x39a3d8 _ZNK12TriggerPlate10GetSoundOnEv
// The sound played on activation is row m_w4 (+0x10). -1 when m_dataIndex is -1.
int TriggerPlate::GetSoundOn() const {
    if (m_dataIndex == -1) {
        return -1;
    }
    return TriggerPlateRow(m_dataIndex).m_w4;
}

// 0x39a40c _ZNK12TriggerPlate11GetSoundOffEv
// The sound played on deactivation is row m_w3 (+0x0c). -1 when m_dataIndex is -1.
int TriggerPlate::GetSoundOff() const {
    if (m_dataIndex == -1) {
        return -1;
    }
    return TriggerPlateRow(m_dataIndex).m_w3;
}

// 0x39a440 _ZNK12TriggerPlate14GetTriggerTypeEv
// The trigger type is row m_w6 (+0x18). -1 when m_dataIndex is -1.
int TriggerPlate::GetTriggerType() const {
    if (m_dataIndex == -1) {
        return -1;
    }
    return TriggerPlateRow(m_dataIndex).m_w6;
}

// 0x39a474 _ZNK12TriggerPlate11GetBehaviorEv
// The behaviour is row m_w1 (+0x04). -1 when m_dataIndex is -1. Update compares it with 5.
int TriggerPlate::GetBehavior() const {
    if (m_dataIndex == -1) {
        return -1;
    }
    return TriggerPlateRow(m_dataIndex).m_w1;
}

// 0x39a4a8 _ZNK12TriggerPlate17GetSpecificObjectEv
// The specific object is row m_w5 (+0x14). -1 when m_dataIndex is -1.
int TriggerPlate::GetSpecificObject() const {
    if (m_dataIndex == -1) {
        return -1;
    }
    return TriggerPlateRow(m_dataIndex).m_w5;
}

// 0x39a510 _ZNK12TriggerPlate18IsObjectActivatingEi
// Walks m_objectsInside in set order. An object is activating when its handle resolves, its GO id is 6 (LiftableObject)
// and its vtable slot 0xc8 (LiftableObject::GetDataId) equals specificObject. -1 never matches.
bool TriggerPlate::IsObjectActivating(int specificObject) const {
    if (specificObject == -1) {
        return false;
    }
    for (GameObject* object : m_objectsInside) {
        ObjectHandle handle = object->GetHandle();
        ObjectBase* resolved = handle.GetObject(false);
        if (resolved != nullptr && static_cast<int>(resolved->m_goId) == 6 &&
            static_cast<LiftableObject*>(resolved)->GetDataId() == specificObject) {
            return true;
        }
    }
    return false;
}

// 0x39a5ec _ZNK12TriggerPlate29GetNumValidContactsActivatingEv
// The count depends on the trigger type (the virtual GetTriggerType, slot 0xe8): 0 the objects inside (the set size,
// +0x398), 1 and 2 the players inside, 3 the objects that are not characters, 4 the objects activating with the
// specific object, anything else 0.
int TriggerPlate::GetNumValidContactsActivating() const {
    switch (GetTriggerType()) {
        case 0:
            return static_cast<int>(m_objectsInside.size());
        case 1:
        case 2:
            return GetNumPlayers();
        case 3:
            return GetNumObjects();
        case 4:
            return IsObjectActivating(GetSpecificObject());
        default:
            return 0;
    }
}

// 0x39a668 _ZN12TriggerPlate6UpdateEv
// The plate counts its valid contacts, then activates when the count reaches m_numberOfObjects (after the delay timer
// runs out) and deactivates when it falls below. The count is kept in unk_3c0 and the previous count in m_lastNumContacts.
//
// PARTIAL, three branches are not made (each stays a note at its place below):
//   1. The online test: the original reads GetOnline()->isOnlineGame_ (COnline +0x05), which is protected in
//      net-session-online/c_online.hpp. The offline path is the only one written here, so the replicated count of an
//      online game is not read.
//   2. The objective event: the QE_TriggerPlate event (the v2QuestObjectiveType "TriggerPlate" id, the object's
//      unk_64, GetDataId() and the -1 value) is not raised. IEvent::eventId_ is protected in
//      net-session-online/i_event.hpp, which QuestEvent.hpp includes, so the event id cannot be set here.
//   3. The sounds: VoxSoundManager::s_instance and Play3D are not declared in vox-audio, so the activation and
//      deactivation sounds are not played.
void TriggerPlate::Update() {
    if (!m_enabled) {
        return;
    }
    // The original tests !m_activated || GetBehavior() != 5 and runs the body when that holds. The GetBehavior call
    // (the virtual at slot 0xec) happens only when the plate is activated.
    if (m_activated && GetBehavior() == 5) {
        return;
    }

    ZoneEx::Update();
    if (GetTriggerType() == 2) {
        m_numberOfObjects = dword_99F76C->GetNumPlayers();
    }

    // PARTIAL (online branch, see above): the offline count is always computed here.
    const int numContacts = GetNumValidContactsActivating();
    unk_3c0 = numContacts;

    // m_numberOfObjects <= count, or else the delay timer runs down and must reach zero.
    bool ready = m_numberOfObjects <= numContacts;
    if (!ready) {
        Trigger::UpdateTimer();
        ready = m_timer <= 0;
    }
    if (!ready) {
        return;
    }

    if (Trigger::CanActivate()) {
        const int lastContacts = m_lastNumContacts;
        if (lastContacts != numContacts) {
            const int required = m_numberOfObjects;
            if (required <= numContacts) {
                if (lastContacts < required && !m_activated) {
                    Trigger::Activate();
                    if (ApplicationGetInstance().GetCurrentLevel() == nullptr) {
                        ReportAssert("level", 230);
                    }
                    // PARTIAL (objective event, see above): the QE_TriggerPlate event is not raised.
                    // PARTIAL (sound, see above): when m_dataIndex is not -1 the original plays GetSoundOn() at
                    // m_position through VoxSoundManager::Play3D.
                    if (m_visualObject != nullptr) {
                        m_visualObject->m_animController->PlayClip("activate", false, 0, 0);
                    }
                    if (m_scriptOnId != -1) {
                        ScriptManager::s_inst.StartScript(m_scriptOnId, unk_64, false);
                    }
                    m_activated = true;
                }
            } else if (lastContacts >= required) {
                // PARTIAL (sound, see above): when m_dataIndex is not -1 the original plays GetSoundOff() at m_position.
                if (m_visualObject != nullptr) {
                    m_visualObject->m_animController->PlayClip("deactivate", false, 0, 0);
                }
                if (m_scriptOffId != -1) {
                    ScriptManager::s_inst.StartScript(m_scriptOffId, unk_64, false);
                }
                m_activated = false;
            }
            m_lastNumContacts = numContacts;
        }
    }
    GameObject::RequireOnlineUpdate();
}

// 0x39aa68 _ZNK12TriggerPlate9GetDataIdEv
// PARTIAL: the original compares m_data with each name of Arrays::TriggerPlates::m_memberNames, for the first size
// names, and returns the first matching index, or -1. The name table is declared in pydata-accessors/member_names.hpp
// and members and size in w7_arrays27.hpp, both as struct Arrays::TriggerPlates, so one translation unit cannot include
// both (the same conflict as ProjectileTrap). Until the pydata headers are merged, no name is matched and -1 is returned.
int TriggerPlate::GetDataId() const {
    return -1;
}

// 0x39aae4 _ZN12TriggerPlateD1Ev (complete object destructor, D1)
// 0x39ab6c _ZN12TriggerPlateD2Ev (base object destructor, D2; the same code)
// Nothing is done here. The original resets the vptrs, frees the three std::string members (m_scriptOff, m_scriptOn,
// m_data, in reverse declaration order) and then runs Trigger::~Trigger. The compiler does those steps in the same order.
TriggerPlate::~TriggerPlate() {}

// 0x39ab50 _ZN12TriggerPlateD0Ev (deleting destructor, D0)
// skipped: the compiler generates the deleting destructor (~TriggerPlate() followed by CustomFree(this)). A delete
// expression on a TriggerPlate does the same, so there is no separate body.

// 0x39ae64 _ZN12TriggerPlate17DeclarePropertiesEv
// Trigger's properties first, then "data" (m_data), "number_of_objects" (default 1), "script_on", "script_off" and
// "trigger_type" (default 0).
void TriggerPlate::DeclareProperties() {
    Trigger::DeclareProperties();
    AddProperty<std::string>("data", m_data);
    AddProperty<int>("number_of_objects", m_numberOfObjects, 1);
    AddProperty<std::string>("script_on", m_scriptOn);
    AddProperty<std::string>("script_off", m_scriptOff);
    AddProperty<int>("trigger_type", m_triggerType, 0);
}

// 0x39af04 _ZN12TriggerPlateC1EN10ObjectBase6GO_IDSE (complete object constructor, C1)
// 0x39afe0 _ZN12TriggerPlateC2EN10ObjectBase6GO_IDSE (base object constructor, C2; the same code)
// Trigger(id, hasPhysics = true, playerOnly = false), then the plate's defaults: no row (m_dataIndex -1), one object
// needed, no script ids, trigger type -1, not activated, and no previous contact count.
// The original does not store m_enabled (+0x779). It is cleared here, and InitPost sets it from the row when there is one.
TriggerPlate::TriggerPlate(ObjectBase::GO_IDS id) : Trigger(id, true, false) {
    m_dataIndex = -1;          // +0x730
    m_numberOfObjects = 1;     // +0x734
    m_triggerType = -1;        // +0x768
    m_scriptOnId = -1;         // +0x76c
    m_scriptOffId = -1;        // +0x770
    m_lastNumContacts = 0;     // +0x774
    m_activated = false;       // +0x778
    m_enabled = false;         // +0x779 (not stored by the original constructor)
}

// 0x39b0bc _ZN12TriggerPlate8InitPostEv
// PARTIAL: the data-row lookup is not made. The original searches m_data in Arrays::TriggerPlates::m_memberNames
// (see GetDataId), copies the row's GameObjectDict name into m_dae (+0x290), and stores the index in m_dataIndex. Those
// three steps need the name table, which is not reachable (see GetDataId). m_dataIndex therefore keeps its constructor
// value of -1, so the m_dae copy and the row's enabled flag are skipped below. The rest of the function is reproduced.
void TriggerPlate::InitPost() {
    if (CheckSpawnProbability() < m_spawnProb) {  // +0x274
        m_triggerCount = -1;                      // +0x3a8, the plate is unlimited
        m_playerOnly = false;                     // +0x381 (Zone)

        // PARTIAL (data-row lookup, see above): m_dataIndex is not set and m_dae is not copied.
        Trigger::InitPost();

        m_scriptOnId = ScriptManager::s_inst.GetIDFromName(m_scriptOn.c_str(), false);    // +0x76c
        m_scriptOffId = ScriptManager::s_inst.GetIDFromName(m_scriptOff.c_str(), false);  // +0x770

        if (m_visualObject != nullptr) {
            m_visualObject->m_animController->PlayClip("idle", true, 0, 0);
        }
        if (m_dataIndex >= 0) {
            m_enabled = TriggerPlateRow(m_dataIndex).m_w2;  // +0x779 from row +0x08
        }
        if (GetTriggerType() == 1) {
            m_numberOfObjects = 1;                    // +0x734
        }
    }
}
