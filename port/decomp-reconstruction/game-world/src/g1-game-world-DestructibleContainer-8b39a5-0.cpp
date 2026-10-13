// Batch g1-game-world-DestructibleContainer-8b39a5-0: the DestructibleContainer methods of libDungeonHunter2.so (the
// constructors and destructors, the Container overrides, Interact and the animation event callback).
// Declarations come from dh2/DestructibleContainer.hpp, which needs no change for this batch. Offsets in comments are
// the original 32-bit ones.
//
// Pydata row fields (Structs::DestructibleContainer, pydata-accessors/w7_structs_5.hpp; the element is 68 bytes in the
// original with its vptr at +0x00, and the field m_wN sits at +4*N). The names here are the uses in these bodies:
//   m_w7  (+0x1c) int32  sound id         GetSound
//   m_w8  (+0x20) bool   keep physics     KeepPhysics
//   m_w10 (+0x28) int32  loot table id    GetLoot
//   m_w12 (+0x30) char*  script name      GetScript (the listing zeroes it, see w7_arrays27.cpp)
//   m_w15 (+0x3c) int32  visual index     GetVisual
// A row is reached through the typed element pointer, never by byte offset (Track B decision 3).
//
// Include constraints: pydata-accessors/member_names.hpp is not included here. It redefines CustomFree, which
// net-transport's deps/byte_array.hpp also defines (dh2/Container.hpp pulls net-transport in). The Arrays lookups are
// declared by hand instead, as game-app and game-world do. dh2/Level.hpp is not included either: the level is only
// tested against null here, so the Application declaration is enough.

#include <cstdio>
#include <cstdlib>
#include <string>

#include "dh2/AnimController.hpp"
#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DestructibleContainer.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/TrophyManager.hpp"
#include "dh2/VisualObject.hpp"
#include "pydata-accessors/w7_arrays27.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

// Engine-wide assert level (core-platform/memory.hpp, not included: see the note above).
extern int gAssertLevel;

namespace Arrays {
struct TrophyTable;  // defined in pydata-accessors/w7_arrays_00.hpp
// The explicit instantiation for DestructibleContainers is in pydata-accessors/src/member_ids.cpp; the one for TrophyTable
// is in pydata-accessors/src/tb_accessors-Arrays-0.cpp.
template <class T>
int GetMemberIDByString(const char* name);
}  // namespace Arrays

namespace {

const char kDestructibleContainerSource[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Containers\\DestructibleContainer.cpp";

// The original ASSERT(expression) at DestructibleContainer.cpp:line. Level 2 stops the process (the original stores to
// address 0); level 1 prints the message.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kDestructibleContainerSource, line);
    }
}

// Row `index` of Arrays::DestructibleContainers as the host element type. The caller has checked index != -1.
const Structs::DestructibleContainer& TableRow(int index) {
    return reinterpret_cast<const Structs::DestructibleContainer*>(Arrays::DestructibleContainers::members)[index];
}

}  // namespace

// 0x3a0cec _ZNK21DestructibleContainer9GetScriptEv
// The script name is the row's m_w12 string buffer. Returns null when the object has no row (m_dataId is -1).
const char* DestructibleContainer::GetScript() const {
    if (m_dataId == -1) {  // +0x374
        return nullptr;
    }
    return TableRow(m_dataId).m_w12;
}

// 0x3a0d28 _ZNK21DestructibleContainer9GetVisualEv
// The visual index is the row's m_w15 (+0x3c). Without a row the original returns m_dataId itself, which is -1.
int DestructibleContainer::GetVisual() const {
    if (m_dataId != -1) {  // +0x374
        return TableRow(m_dataId).m_w15;
    }
    return m_dataId;
}

// 0x3a0d60 _ZNK21DestructibleContainer18GetInteractionTypeEP10GameObject
// Constant 8 (the other argument is unused).
int DestructibleContainer::GetInteractionType(GameObject* other) const {
    (void)other;
    return 8;
}

// 0x3a0d68 _ZN21DestructibleContainer9DoEffectsEv
// Empty in the original (a bare return). The "fx" animation event calls it.
void DestructibleContainer::DoEffects() {}

// 0x3a0da0 _ZN21DestructibleContainer8InteractEP10GameObject
// PARTIAL. Bound: the hit path (the hit counter at +0x6f4 is decremented first, then the visual plays the clip
// total - hits left, with loop false, unused 0 and animation index 0, when the visual is set), the destroy path's
// level assert, the call to Container::Interact, and the trophy: when the interacting GameObject is a Character that
// IsPlayer(), its property 217 is incremented, and when the value passes 199 for the local player, the trophy
// "destroy_200_breakables" is unlocked (UnlockTrophy with -1 when the name is not found).
// Not bound, because the declarations are missing:
//   (1) the 3D sound of a hit, VoxSoundManager::s_instance->Play3D(GetSound(), m_position, false, 1, -1.0f, -1.0f).
//       vox-audio/vox/sound_manager.hpp declares neither s_instance nor Play3D.
//   (2) the DestroyGameObject quest event that the destroy path raises through EventManager::RaiseAsync on the level:
//       a stack QE_DestroyGameObject with eventId_ = PyDataConstants::getConstant("v2QuestObjectiveType",
//       "DestroyGameObject") (+0x04), m_character = other (+0x08), m_targetId = unk_64 (+0x0c), m_flag0 = 0 (+0x10),
//       m_flag1 = 0 (+0x11), m_value = -1 (+0x14) and m_matchId = GetDataId() (+0x18). eventId_ is IEvent's field, and
//       net-session-online/i_event.hpp declares it protected; dh2/QuestEvent.hpp takes IEvent from there.
//       dh2/IEvent.hpp declares it public. The event is destroyed before Container::Interact runs.
void DestructibleContainer::Interact(GameObject* other) {
    if (unk_6f4 != 0) {  // +0x6f4 hits left
        const int hitsLeft = unk_6f4 - 1;
        unk_6f4 = hitsLeft;
        if (m_visualObject != nullptr) {  // +0x2d8
            const unsigned int clip = static_cast<unsigned int>(unk_6f0) - static_cast<unsigned int>(hitsLeft);
            // VisualObject +0x38 m_animController, vtable +0x1c PlayClip(clip, loop = false, unused = 0, animIndex = 0).
            m_visualObject->m_animController->PlayClip(clip, false, 0, 0);
        }
        // PARTIAL: the Play3D sound (see the function note). The hit path returns here.
        return;
    }

    // Destroy path: the level is needed for the DestroyGameObject event.
    if (ApplicationGetInstance().GetCurrentLevel() == nullptr) {
        ReportAssert("level", 231);
    }
    // PARTIAL: the DestroyGameObject event raise (see the function note).

    Container::Interact(other);

    // ObjectBase::GetHandle of the interacting object, then ObjectHandle::operator Character*.
    ObjectHandle handle = other->GetHandle();
    Character* character = handle;
    if (character == nullptr || !character->IsPlayer()) {  // vtable +0x28 IsPlayer
        return;
    }
    // Character +0x560 is its CharProperties base; property 217 has no name in the tree (the breakables counter).
    character->PROPS_AddInt(217, 1);
    if (character->PROPS_GetInt(217, false) <= 199) {
        return;
    }
    // dword_99F76C is Application::m_playerManager (+0x40).
    if (!ApplicationGetInstance().m_playerManager->IsLocalPlayer(character)) {
        return;
    }
    // The original walks Arrays::TrophyTable::m_memberNames with strcmp; the port's lookup for that table is the same
    // search (-1 when no name matches or the table is empty).
    const int trophy = Arrays::GetMemberIDByString<Arrays::TrophyTable>("destroy_200_breakables");
    TrophyManager::s_instance->UnlockTrophy(trophy);
}

// 0x3a10f8 _ZNK21DestructibleContainer11KeepPhysicsEv
// The row is found by the data_desc string (m_dataDesc, +0x378; its data pointer is at +0x38c, the same word
// GetSound reads). Returns false when no row matches.
bool DestructibleContainer::KeepPhysics() const {
    const int row = Arrays::GetMemberIDByString<Arrays::DestructibleContainers>(m_dataDesc.c_str());
    if (row == -1) {
        return false;
    }
    return TableRow(row).m_w8;  // +0x20
}

// 0x3a1140 _ZNK21DestructibleContainer8GetSoundEv
// The sound id is the row's m_w7 (+0x1c). Returns -1 when no row matches.
int DestructibleContainer::GetSound() const {
    const int row = Arrays::GetMemberIDByString<Arrays::DestructibleContainers>(m_dataDesc.c_str());
    if (row != -1) {
        return TableRow(row).m_w7;  // +0x1c
    }
    return row;
}

// 0x3a1180 _ZNK21DestructibleContainer7GetLootEv
// The loot table id is the row's m_w10 (+0x28). Returns -1 when no row matches.
int DestructibleContainer::GetLoot() const {
    const int row = Arrays::GetMemberIDByString<Arrays::DestructibleContainers>(m_dataDesc.c_str());
    if (row != -1) {
        return TableRow(row).m_w10;  // +0x28
    }
    return row;
}

// 0x3a11c0 _ZNK21DestructibleContainer9GetDataIdEv
// -1 when the data_desc string is empty, otherwise the row found by that string. The original compares the string's
// two pointer words at +0x388 and +0x38c, which is the empty test of its std::string (the same layout as
// ObjectBase::m_minDifficulty at +0xd4, see g1-game-world-ObjectBase-670ee4-1.cpp).
int DestructibleContainer::GetDataId() const {
    if (m_dataDesc.empty()) {
        return -1;
    }
    return Arrays::GetMemberIDByString<Arrays::DestructibleContainers>(m_dataDesc.c_str());
}

// 0x3a11dc _ZN21DestructibleContainer8InitPostEv
// Container::InitPost first. When the visual is set, the timeline callbacks are installed (Container's animation
// callback, this class's event callback), and the clip count of the animation (GetNumClip(0), vtable +0x10) gives the
// hit counter: when it is above 3, the total hits (+0x6f0) and the hits left (+0x6f4) are both set to count - 3.
void DestructibleContainer::InitPost() {
    Container::InitPost();
    VisualObject* visual = m_visualObject;  // +0x2d8
    if (visual != nullptr) {
        AnimController* anim = visual->m_animController;  // VisualObject +0x38
        // vtable +0x2c SetCallbacks(animCallback, user, eventCallback, user)
        anim->SetCallbacks(&Container::__Callback, this, &DestructibleContainer::__EventCallback, this);
        const unsigned int clipCount = static_cast<unsigned int>(anim->GetNumClip(0));  // vtable +0x10
        if (clipCount > 3) {
            const int hits = static_cast<int>(clipCount - 3);
            unk_6f0 = hits;  // +0x6f0 total hits
            unk_6f4 = hits;  // +0x6f4 hits left
        }
    }
}

// 0x3a1270 _ZN21DestructibleContainer17DeclarePropertiesEv
// Thunk: a tail branch to Container::DeclareProperties (0x3a083c).
void DestructibleContainer::DeclareProperties() {
    Container::DeclareProperties();
}

// 0x3a1274 _ZN21DestructibleContainer15__EventCallbackERKN6glitch7collada15STriggeredEventEPv
// PARTIAL, body not bound. The original reads the event name (a const char* at +0x04 of the STriggeredEvent) and
// compares it with "fx", then with "opened". "fx" calls DoEffects on the user pointer. Any other name is forwarded to
// Container::__EventCallback. "opened" asserts that a level exists (line 69), raises the DestroyGameObject quest event
// with a null character and value -1 (the same event as in Interact, see there), and then forwards to
// Container::__EventCallback. glitch::collada::STriggeredEvent has no definition in the tree, so the name cannot be read.
// The quest event part also waits on the IEvent::eventId_ declaration (see Interact). The body is empty until then.
void DestructibleContainer::__EventCallback(const glitch::collada::STriggeredEvent& event, void* user) {
    (void)event;
    (void)user;
}

// 0x3a1418 _ZN21DestructibleContainerD1Ev (complete-object destructor)
// 0x3a147c _ZN21DestructibleContainerD2Ev (base-object destructor, same body)
// The pseudocode resets the three vtable pointers and calls Container::~Container. In C++ those are the implicit steps of
// the destructor chain, so the body is empty.
DestructibleContainer::~DestructibleContainer() {}

// 0x3a14bc _ZN21DestructibleContainerC1EN10ObjectBase6GO_IDSE
// 0x3a1508 _ZN21DestructibleContainerC2EN10ObjectBase6GO_IDSE
// PARTIAL, inherited. The two stores are bound: the hit counters +0x6f4 and +0x6f0 start at zero. The Container base
// constructor (0x3a06dc) is partial: its NetStructContainer members do a conditional SetChanged on value words the
// constructor never writes (see g1-game-world-Container-e6443a-1.cpp), and this constructor inherits that gap.
DestructibleContainer::DestructibleContainer(ObjectBase::GO_IDS id) : Container(id) {
    unk_6f4 = 0;  // +0x6f4 hits left
    unk_6f0 = 0;  // +0x6f0 total hits
}
