// Batch g1-game-world-Door-478d47-0 (part 0 of 2): the Door predicates, the Lua bindings, the open and
// close state changes, the net-struct and serialization hooks, and the InitFinal and InitPost lifecycle
// hooks. Declarations come from dh2/Door.hpp. Original 32-bit size 0x6d0.
//
// Partial functions and what is missing:
//   - Opened, Closed, Disabled, Enabled: PFWorld::FlagFloorAsDeadEnd(Singleton<PFWorld>::s_inst, m_pfObject, bool).
//     The tree declares no Singleton template and no accessor for the PFWorld instance.
//   - Open, Close: VoxSoundManager::Play3D for the door sound. dh2/vox-audio does not declare Play3D or s_instance.
//   - createBindings: sfc::script::lua::Binder::bindFunction and bindMethod. Binder is forward-declared only.
//   - InitPost: the door-name lookup, the two LoadSound calls, the physics body and the failed-MeetCondition branch.
//     See the comment on InitPost.
//
// Byte at +0x373 (Door::Opened sets it, Door::Closed clears it): the same byte GameObject.hpp names
// m_idleSoundMuted, which it says Disabled() sets. The Door meaning of the byte is not verified.

#include "dh2/Door.hpp"

#include "dh2/AnimController.hpp"
#include "dh2/IStreamBase.hpp"
#include "dh2/PhysicalObject.hpp"
#include "dh2/VisualObject.hpp"

// 0x3e7498 _ZNK4Door11IsUpdatableEv
bool Door::IsUpdatable() const {
    return true;
}

// 0x3e74a0 _ZNK4Door10IsAnimatedEv
bool Door::IsAnimated() const {
    return true;
}

// 0x3e74a8 _ZNK4Door13IsInteractiveEP10GameObject
// The argument is ignored: a door is never interactive.
bool Door::IsInteractive(GameObject* /*by*/) const {
    return false;
}

// 0x3e74b0 _ZNK4Door9IsZonableEv
bool Door::IsZonable() const {
    return true;
}

// 0x3e74b8 _ZNK4Door13getUDTypeNameEv
const char* Door::getUDTypeName() const {
    return "Door";
}

// 0x3e74c8 _ZN4Door14createBindingsERN3sfc6script3lua6BinderE
// PARTIAL: the four Lua registrations are missing. The original calls
//   Binder::bindFunction(binder, "Open", _Open, this);  Binder::bindMethod(binder, "Open", _Open);
//   Binder::bindFunction(binder, "Close", _Close, this); Binder::bindMethod(binder, "Close", _Close);
// after GameObject::createBindings. No sfc::script::lua::Binder declaration exists in the tree.
void Door::createBindings(sfc::script::lua::Binder& binder) {
    GameObject::createBindings(binder);
}

// 0x3e7560 _ZN4Door6UpdateEv
void Door::Update() {
    // +0x370 m_idleSoundId (int16, -1 = no idle sound)
    if (m_idleSoundId >= 0) {
        GameObject::UpdateIdleSound();
    }
    // +0x2d8 m_visualObject
    if (m_visualObject != nullptr) {
        m_visualObject->Sync();
    }
    GameObject::RequireOnlineUpdate();
}

// 0x3e7598 _ZN4Door6ClosedEb
// PARTIAL: PFWorld::FlagFloorAsDeadEnd(Singleton<PFWorld>::s_inst, m_pfObject, true) is not emitted.
void Door::Closed(bool silent) {
    m_state = DOOR_CLOSED;                                  // +0x3a8
    VisualObject* visual = m_visualObject;                  // +0x2d8
    PhysicalObject* physical = m_physicalObject;            // +0x2dc
    if (visual != nullptr && !silent) {
        // VisualObject +0x38 m_animController, AnimController vtable +0x20 PlayClip(name, loop, unused, animIndex)
        visual->m_animController->PlayClip("IdleClosed", true, 0, 0);
    }
    if (physical != nullptr) {
        physical->enableFilter();
    }
    m_idleSoundMuted = 0;                                   // +0x373 (see the file comment)
}

// 0x3e763c _ZN4Door5CloseEb
// PARTIAL: the door sound is not emitted. The original calls
//   VoxSoundManager::Play3D(s_instance, Arrays::Doors::members[m_doorIndex].+12, m_position, false, 1, -1.0f, -1.0f)
// when m_doorIndex != -1, and VoxSoundManager does not declare Play3D.
void Door::Close(bool instant) {
    // !m_moving && m_state == DOOR_OPENED, otherwise nothing happens
    if (m_moving || m_state != DOOR_OPENED) {
        return;
    }
    VisualObject* visual = m_visualObject;                  // +0x2d8
    // Closes at once when there is no visual, the close is instant, or the door is zonable with zoning
    // enabled (+0x2ee) and nobody inside (+0x2f0). Otherwise the closing animation plays.
    if (visual == nullptr || instant || (IsZonable() && m_zoningEnabled != 0 && m_inZone == 0)) {
        Closed(false);
    } else {
        m_state = DOOR_CLOSING;                             // +0x3a8
        m_moving = true;                                    // +0x3ac
        unk_85 = 0;                                         // ObjectBase +0x85
        visual->m_animController->PlayClip("Close", false, 0, 0);
    }
}

// 0x3e777c _ZN4Door6_CloseERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// The Lua callback receives the Door as its user pointer.
void Door::_Close(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* user) {
    static_cast<Door*>(user)->Close(false);
}

// 0x3e7788 _ZN4Door6OpenedEb
// PARTIAL: PFWorld::FlagFloorAsDeadEnd(Singleton<PFWorld>::s_inst, m_pfObject, false) is not emitted.
void Door::Opened(bool silent) {
    m_state = DOOR_OPENED;                                  // +0x3a8
    VisualObject* visual = m_visualObject;                  // +0x2d8
    PhysicalObject* physical = m_physicalObject;            // +0x2dc
    if (visual != nullptr && !silent) {
        // The original passes loop = 1 (R2) and unused = animIndex = silent, which is 0 on this path.
        visual->m_animController->PlayClip("IdleOpened", true, 0, 0);
    }
    if (physical != nullptr) {
        physical->disableFilter();
    }
    m_idleSoundMuted = 1;                                   // +0x373 (see the file comment)
}

// 0x3e7828 _ZN4Door26InterpretIncomingNetStructEb
// Moves the door to match the incoming open flag (m_netIn.m_open, the value byte at +0x68d). Its own logic
// matches the pseudocode; it calls Opened and Closed, which are partial.
void Door::InterpretIncomingNetStruct(bool force) {
    if (!force) {
        return;
    }
    const bool netOpen = m_netIn.m_open.GetValue();         // Door +0x540 m_netIn, +0x130 m_open, value +0x1d
    const bool settling = (m_state == DOOR_OPENED || m_state == DOOR_OPENING);
    if (netOpen) {
        if (!settling) {
            Opened(false);
        }
    } else {
        if (settling) {
            Closed(netOpen);
        }
    }
}

// 0x3e786c _ZN4Door4OpenEb
// PARTIAL: the door sound is not emitted. The original calls
//   VoxSoundManager::Play3D(s_instance, Arrays::Doors::members[m_doorIndex].+16, m_position, false, 1, -1.0f, -1.0f)
// when m_doorIndex != -1, and VoxSoundManager does not declare Play3D.
void Door::Open(bool instant) {
    // !m_moving && m_state == DOOR_CLOSED, otherwise nothing happens
    if (m_moving || m_state != DOOR_CLOSED) {
        return;
    }
    VisualObject* visual = m_visualObject;                  // +0x2d8
    if (visual == nullptr || instant || (IsZonable() && m_zoningEnabled != 0 && m_inZone == 0)) {
        Opened(false);
    } else {
        m_state = DOOR_OPENING;                             // +0x3a8
        m_moving = true;                                    // +0x3ac
        unk_85 = 0;                                         // ObjectBase +0x85
        visual->m_animController->PlayClip("Open", false, 0, 0);
    }
}

// 0x3e79a8 _ZN4Door5_OpenERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
// The Lua callback receives the Door as its user pointer.
void Door::_Open(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/, void* user) {
    static_cast<Door*>(user)->Open(false);
}

// 0x3e7a6c _ZN4Door11DeserializeEP11IStreamBase
// The state is read as a 4-byte DoorStates, then the door is settled without playing the animation.
void Door::Deserialize(IStreamBase* stream) {
    GameObject::Deserialize(stream);
    stream->readAs<DoorStates>(m_state);                    // +0x3a8
    if (m_state == DOOR_OPENED || m_state == DOOR_OPENING) {
        Opened(false);
    } else {
        Closed(false);
    }
}

// 0x3e7b74 _ZN4Door9SerializeEP11IStreamBase
void Door::Serialize(IStreamBase* stream) {
    GameObject::Serialize(stream);
    stream->writeAs<DoorStates>(m_state);                   // +0x3a8
}

// 0x3e7b94 _ZN4Door8DisabledEv
// PARTIAL: when m_state != DOOR_OPENED the original also calls
// PFWorld::FlagFloorAsDeadEnd(Singleton<PFWorld>::s_inst, m_pfObject, false). Singleton<PFWorld> is not declared.
void Door::Disabled() {
    GameObject::Disabled();
}

// 0x3e7bd8 _ZN4Door7EnabledEv
// PARTIAL: when m_state != DOOR_OPENED the original also calls
// PFWorld::FlagFloorAsDeadEnd(Singleton<PFWorld>::s_inst, m_pfObject, true). Singleton<PFWorld> is not declared.
void Door::Enabled() {
    GameObject::Enabled();
}

// 0x3e7c1c _ZN4Door9InitFinalEv
void Door::InitFinal() {
    if (GameObject::CheckSpawnProbability() < m_spawnProb) {  // +0x274
        GameObject::InitFinal();
        if (GameObject::MeetCondition()) {
            if (m_opened) {                                    // +0x3a4
                Opened(false);
            } else {
                Closed(m_opened);
            }
        }
    }
}

// 0x3e7da8 _ZN4Door8InitPostEv
// PARTIAL, four pieces are missing:
//   1. The door-name lookup (m_doorIndex at +0x3a0, and the GameObject m_dae at +0x290 from the GameObjectDict
//      name). The original reads the name through the word at +0x39c, which the constructor sets to the address of
//      the m_data string object (+0x388). The string layout that makes that a C string is not recovered.
//      Arrays::Doors is declared twice (member_names.hpp and w7_arrays27.hpp) and its 24-byte element is not typed.
//   2. The physics body for m_doorCollision (+0x3a5). The PhysicalWorld pointer dword_99F770 has no declared name,
//      and POZone has no constructor, so the POZone vtable (off_968F88) cannot be installed on the new
//      PhysicalObject (operator new(0x28), PhysicalObject(world, this, 1, 0, 0, 0, 0, 2, 0xFFFF, 1)).
//   3. VoxSoundManager::LoadSound for the open and close sounds (not declared).
//   4. When MeetCondition is false, the original calls vtable slot +0x40 (GameObject::SetVisible) on this with its
//      bool argument (R1) unset in the listing. That call is not emitted.
void Door::InitPost() {
    if (GameObject::CheckSpawnProbability() < m_spawnProb) {   // +0x274
        Zone::InitPost();
        if (GameObject::MeetCondition()) {
            VisualObject* visual = m_visualObject;             // +0x2d8
            if (visual != nullptr) {
                // VisualObject +0x38 m_animController, vtable +0x2c SetCallbacks(animCallback, user, eventCallback, user)
                visual->m_animController->SetCallbacks(&Door::__Callback, this, &Door::__EventCallback, this);
                visual->ApplyMeshBox();
            }
        }
    }
}
