// Batch g1-game-world-Container-e6443a-1 (part 2 of 2): the Container and NetStructContainer constructors and
// destructors, the life-cycle functions InitPost and InitFinal, the net struct functions PopulateOutgoingNetStruct and
// InterpretIncomingNetStruct, DeclareProperties, DoOpen, Interact and the animation event callback. Part 1 (the getters,
// SetState, Spawn, __Callback, Serialize) is in g1-game-world-Container-e6443a-0.cpp. Declarations come from
// dh2/Container.hpp, which needs no change for this batch.
//
// Redo pass (batch r2a-game-world-Container-e6443a-0): PODecor, ItemObject, LuaScript and sfc::script::lua::Arguments
// now have headers, and ObjectBase declares +0x104 as unk_104 (NetStruct*). The Container constructors and the OnOpen
// Lua call in DoOpen are bound from them.
//
// Gaps, each named again where it occurs:
//   PhysicalWorld global dword_99F770 has no declared name in any header: InitFinal and InterpretIncomingNetStruct
//   (the PODecor body; dh2/PODecor.hpp exists). Layout request, not added here.
//   ItemObject::DropLootTable has an unsettled signature (see the note on DoOpen): DoOpen.
//   pydata Structs::ColladaFile (the GameObjectDict record) cannot be included with net-transport (duplicate inline
//   CustomFree): InitPost (m_dae).
//   VoxSoundManager declares neither s_instance, LoadSound(int) nor Play3D: InitPost and Interact.
//   glitch::collada::STriggeredEvent has no definition (only a forward declaration): __EventCallback.
//   The stack NetStruct temporaries of the original read uninitialised storage: PopulateOutgoingNetStruct and the
//   NetStructContainer constructor. The Container constructors inherit that gap through their member constructors.

#include <cstdint>
#include <cstring>

#include "dh2/AnimController.hpp"
#include "dh2/Container.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/LuaScript.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"
#include "dh2/sfc/script/lua/Arguments.hpp"

namespace {

// The pseudocode's (unsigned)(state - 3) <= 1, the ACTIVATE or OPENED state. Container::IsDead() (0x39f364) tests the
// same two states, but the original inlines the test in these bodies, so the test is repeated here rather than calling
// the virtual IsDead().
bool IsActivatedOrOpened(const Container& container) {
    return container.m_state == Container::STATE_ACTIVATE || container.m_state == Container::STATE_OPENED;
}

}  // namespace

// 0x39f910 _ZN9Container8InitPostEv
// PARTIAL. Bound: the spawn-probability gate, the data id (+0x374), the spawn animation chain, the timeline callbacks,
// the mesh box, the script load and the closed state when the condition fails. Not bound: (1) the m_dae name (GameObject
// +0x290) from the GameObjectDict record `visual` (the name is the record's string buffer at +0x08, the same field that
// Projectile's dae name reads as Structs::ColladaFile::m_w2, pydata w7_structs_5.hpp). That header cannot be included in
// this unit: it and net-transport's deps/byte_array.hpp both define inline CustomFree(void*), and Container.hpp pulls in
// net-transport (layout/dep request). (2) The sound load, VoxSoundManager::s_instance->LoadSound(GetSound()), which runs
// when s_instance is set. vox-audio/vox/sound_manager.hpp declares neither the static nor LoadSound(int).
void Container::InitPost() {
    if (GameObject::CheckSpawnProbability() < m_spawnProb) {  // +0x274
        m_dataId = GetDataId();                                // +0x374
        const int visual = GetVisual();
        if (visual != -1 && m_dataId != -1) {
            // PARTIAL: m_dae (GameObject +0x290) = the GameObjectDict record `visual`, name at record +0x08 (see the note).
        }
        GameObject::InitPost();
        if (GameObject::MeetCondition()) {
            VisualObject* visualObject = m_visualObject;  // +0x2d8
            if (visualObject != nullptr) {
                AnimController* anim = visualObject->m_animController;  // VisualObject +0x38
                anim->SetCallbacks(&Container::__Callback, this, &Container::__EventCallback, this);
                // Each later PlayClip gets 0 as loop and unused: the original passes the previous return value,
                // which is 0 on every path that reaches the call.
                if (anim->PlayClip("prespawn", false, 0, 0)) {
                    SetState(STATE_PRESPAWN);
                } else if (anim->PlayClip("spawn", false, 0, 0)) {
                    SetState(STATE_SPAWN);
                } else if (anim->PlayClip("idle", false, 0, 0)) {
                    SetState(STATE_IDLE);
                }
                visualObject->ApplyMeshBox();
            }
            // PARTIAL: the sound load (see the function note).
            LoadExternalScript(GetScript(), "data/scripts/objects/");
        } else {
            // vtable +0x40. The original's r1 is the MeetCondition() result (SUBS R1, R0, #0 before the branch), which
            // is 0 on this path, so the argument is false.
            SetVisible(false);
            SetState(STATE_OPENED);
        }
    }
}

// 0x39fc98 _ZN9Container9InitFinalEv
// PARTIAL. Bound: the spawn-probability gate, GameObject::InitFinal(), the Update() call on a container that is not
// ACTIVATE or OPENED, and the MeetCondition() test. Not bound: the PODecor body. The original allocates 40 bytes,
// constructs PODecor on dword_99F770 (the PhysicalWorld*) with this as the owner, then calls SetPhysicalObject(body, false).
// dh2/PODecor.hpp now declares PODecor(PhysicalWorld*, GameObject*, bool), and its bool is never read. What is missing is
// a declared name for dword_99F770: no header declares that global (layout request). Once it is declared the body is
// `new PODecor(dword_99F770, this, false)` followed by SetPhysicalObject(body, false).
void Container::InitFinal() {
    if (GameObject::CheckSpawnProbability() < m_spawnProb) {
        GameObject::InitFinal();
        if (!IsActivatedOrOpened(*this)) {
            Update();  // vtable +0x2c
        }
        if (GameObject::MeetCondition()) {
            // PARTIAL: the PODecor body and SetPhysicalObject(body, false) (see the function note).
        }
    }
}

// 0x39fd38 _ZN9Container26InterpretIncomingNetStructEb
// PARTIAL. Bound: the incoming opened flag (m_netIn +0x150), the state gates, the opener lookup, the call to Interact, the
// release of the physics body, the activate, idleactive and idle animation calls, and the copy of m_netIn.unk_180 into
// unk_fc (ObjectBase +0xfc) when forced. Not bound: the PODecor body on the dead-and-closed path, which waits on the same
// dword_99F770 declaration as InitFinal.
void Container::InterpretIncomingNetStruct(bool force) {
    const std::uint32_t openedIn = m_netIn.m_opened.GetValue();  // m_netIn +0x130 member, value at +0x150
    if (openedIn != 1 || IsActivatedOrOpened(*this)) {
        if (!force) {
            return;
        }
        if (IsActivatedOrOpened(*this) && openedIn == 0) {
            SetState(STATE_IDLE);
            // PARTIAL: the PODecor body and its SetPhysicalObject(body, false) call are missing here.
            SetState(STATE_IDLE);
            // The original calls this with no null check on the visual.
            m_visualObject->m_animController->PlayClip("idle", false, 0, 0);
        }
        unk_fc = static_cast<std::int32_t>(m_netIn.unk_180.GetValue());
        return;
    }
    ObjectHandle handle = dword_99F764->GetObjectHandleByNetworkId(m_netIn.m_openerId.GetValue());
    GameObject* opener = handle;  // ObjectHandle::operator GameObject*()
    if (opener != nullptr && !force) {
        Interact(opener);  // vtable +0x98
        return;
    }
    if (!IsActivatedOrOpened(*this)) {
        m_opener = opener;  // +0x398
        SetState(STATE_OPENED);
        if (!KeepPhysics()) {  // vtable +0xdc
            SetPhysicalObject(nullptr, false);
        }
        if (m_visualObject != nullptr) {
            if (!force) {
                SetState(STATE_ACTIVATE);
                m_visualObject->m_animController->PlayClip("activate", false, 0, 0);
                return;
            }
            SetState(STATE_OPENED);
            m_visualObject->m_animController->PlayClip("idleactive", false, 0, 0);
        }
    }
    if (force) {
        unk_fc = static_cast<std::int32_t>(m_netIn.unk_180.GetValue());
    }
}

// 0x3a0110 _ZN9Container25PopulateOutgoingNetStructEb
// PARTIAL. Bound: the three values written to m_netOut, in the original order: m_opened (1 when ACTIVATE or OPENED, else 0),
// m_openerId (the network id of m_opener) and, when forced, unk_180 (= unk_fc). Not bound: each value goes through a stack
// NetStruct temporary. The original compares the temporary's value word, which the temporary's constructor never writes,
// with the new value. When they differ it stores the new value and calls NetStructMember::SetChanged on the temporary.
// SetChanged advances NetStruct::s_changeCounter, so that conditional advance is not reproduced. The value that reaches
// each SetValue is the new value on every path, so the member values themselves match.
void Container::PopulateOutgoingNetStruct(bool force) {
    m_netOut.m_opened.SetValue(IsActivatedOrOpened(*this) ? 1u : 0u);                    // +0x130 of m_netOut
    m_netOut.m_openerId.SetValue(dword_99F764->GetNetworkIdByObjectBase(m_opener));       // +0x158 of m_netOut
    if (force) {
        m_netOut.unk_180.SetValue(static_cast<std::uint32_t>(unk_fc));                    // +0x180 of m_netOut
    }
}

// 0x39ff58 _ZN9Container18NetStructContainerC1Ev
// PARTIAL. Bound: the NetStruct base and the three member declarations, m_opened (+0x130, 1 bit), m_openerId (+0x158,
// 16 bits) and unk_180 (+0x180, 32 bits). The sizes come from the members' own constructors. The original first reads
// each member's value word (+0x20 of the member), zeroes it, and calls NetStructMember::SetChanged when the old word was
// non-zero. That read is of storage this constructor never writes, so the conditional SetChanged depends on earlier
// memory contents and is not reproduced. The members start at zero and unchanged.
Container::NetStructContainer::NetStructContainer() : NetStruct() {
    DeclareMember(&m_opened);
    DeclareMember(&m_openerId);
    DeclareMember(&unk_180);
}

// 0x3a0324 _ZN9Container18NetStructContainerD1Ev
// The pseudocode resets the vtable pointers and clears the NetStruct packet history (the std::map at +0x10c). In C++ the
// history is destroyed by ~NetStruct() and the members by their own destructors, so the body is empty.
Container::NetStructContainer::~NetStructContainer() {}

// 0x3a0598 _ZN9ContainerD2Ev (base-object destructor)
// 0x3a0430 _ZN9ContainerD1Ev (complete-object destructor)
// Both addresses hold the same body. The pseudocode resets the vtables, clears the packet histories of m_netIn and
// m_netOut, frees the m_dataDesc buffer when it is not the inline one, and calls ~GameObject(). In C++ each of those is a
// member or base destructor, so the body is empty.
Container::~Container() {}

// 0x3a06dc _ZN9ContainerC1EN10ObjectBase6GO_IDSE
// 0x3a0788 _ZN9ContainerC2EN10ObjectBase6GO_IDSE
// PARTIAL, inherited. Bound: the GameObject base constructor with the same id (the id register is passed through
// unchanged), the ObjectBase fields that the original sets (+0x28 = 1, +0x84 m_static = 0, +0xf8 = 2), m_netStruct =
// &m_netOut (+0x100), unk_104 = &m_netIn (+0x104, declared NetStruct* by ObjectBase.hpp), m_deathReset = false,
// m_state = STATE_IDLE and m_opener = nullptr. The data string and the vtables are built by C++. The two
// NetStructContainer members are built by NetStructContainer::NetStructContainer (0x39ff58), which stays partial
// (conditional SetChanged on value words it never writes), so this constructor inherits that gap.
Container::Container(ObjectBase::GO_IDS id) : GameObject(id) {
    unk_28 = 1;
    m_static = 0;
    unk_f8 = 2;
    m_netStruct = &m_netOut;
    unk_104 = &m_netIn;
    m_deathReset = false;
    m_state = STATE_IDLE;
    m_opener = nullptr;
}

// 0x3a083c _ZN9Container17DeclarePropertiesEv
// Declares "data" (int, default -1, m_dataId), "data_desc" (string, default "", m_dataDesc) and "death_reset" (bool,
// default false, m_deathReset) after the GameObject properties, in the original order. The typed AddProperty templates
// are the ones the original's inline Property objects match.
void Container::DeclareProperties() {
    GameObject::DeclareProperties();
    AddProperty("data", m_dataId, -1);
    AddProperty("data_desc", m_dataDesc);
    AddProperty("death_reset", m_deathReset, false);
}

// 0x3a0a98 _ZN9Container6DoOpenEv
// PARTIAL. Bound: the OnOpen Lua call. When m_script (+0x300) is set, the original builds an sfc::script::lua::Arguments,
// pushes m_opener (+0x398) when it is set, and calls LuaScript::Call(m_script, "OnOpen", args).
// Not bound: ItemObject::DropLootTable (0x3ecba0), called with the loot id (GetLoot()), this, m_opener, GetLootFixedNumPowers()
// and two more arguments (0 and an unassigned bool). ItemObject.hpp declares the five-argument form, but its own note says
// the second parameter of that overload is an ItemInventory* the declaration omits, and the sixth argument has no source
// in the pseudocode. The signature is unsettled, so the call waits on the header (integrator decision).
void Container::DoOpen() {
    // PARTIAL: the ItemObject::DropLootTable call (see the function note).
    if (m_script != nullptr) {  // +0x300
        sfc::script::lua::Arguments args;
        if (m_opener != nullptr) {  // +0x398
            args.pushUserData(m_opener);
        }
        m_script->Call("OnOpen", args);
    }
}

// 0x3a0b38 _ZN9Container8InteractEP10GameObject
// PARTIAL. Bound: the state gate, the opener, SetState(OPENED), the physics release, the activate animation or DoOpen(),
// and the final Update() call. Not bound: the 3D sound, VoxSoundManager::s_instance->Play3D(GetSound(), m_position, false,
// 1, -1.0f, -1.0f). vox-audio/vox/sound_manager.hpp declares neither s_instance nor Play3D. The signature read from the
// call site is Play3D(int, glitch::core::vector3d<float> const&, bool, int, float, float).
void Container::Interact(GameObject* by) {
    if (IsActivatedOrOpened(*this)) {
        return;
    }
    m_opener = by;  // +0x398
    SetState(STATE_OPENED);
    if (!KeepPhysics()) {  // vtable +0xdc
        SetPhysicalObject(nullptr, false);
    }
    if (m_visualObject != nullptr) {
        SetState(STATE_ACTIVATE);
        m_visualObject->m_animController->PlayClip("activate", false, 0, 0);
    } else {
        DoOpen();
    }
    // PARTIAL: the Play3D call (see the function note).
    Update();  // vtable +0x2c
}

// 0x3a0c64 _ZN9Container15__EventCallbackERKN6glitch7collada15STriggeredEventEPv
// PARTIAL, body not bound. The original reads the event name (a const char* in the STriggeredEvent at +0x04) and compares
// it with "opened" through strcmp. On "opened" it calls DoOpen() on the user pointer. Otherwise, when m_script is set, it
// pushes the name into an sfc::script::lua::Arguments and calls LuaScript::Call(m_script, "OnAnimEvent", args). The
// STriggeredEvent definition does not exist in the port, and sfc::script::lua::Arguments has no header.
void Container::__EventCallback(const glitch::collada::STriggeredEvent& event, void* user) {
    (void)event;
    (void)user;
}
