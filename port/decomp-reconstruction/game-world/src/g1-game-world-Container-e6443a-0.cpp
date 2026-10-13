// Batch g1-game-world-Container-e6443a-0 (part 1 of 2): the constant Container getters, IsDead, IsInteractive,
// the obstacle values, SetState, Spawn, the timeline callback, IsZonable (a thunk), Deserialize and Serialize.
// Declarations come from dh2/Container.hpp. The NetStructContainer members and the constructors are in part 2.
//
// Layout facts used here (original 32-bit offsets, named in dh2/Container.hpp and its bases):
//   GameObject +0x2d8  m_visualObject      VisualObject*
//   VisualObject +0x08 m_node              RootSceneNode*
//   VisualObject +0x38 m_animController    AnimController* (PlayClip is its vtable slot +0x20)
//   RootSceneNode +0x11c unk_011c          flags word of the node (bit 0x400 set by SetState, bit 0x200 cleared by __Callback)
//   Container +0x390 m_deathReset, +0x394 m_state
//   ObjectBase +0x81 m_markedForDeletion   set by ObjectBase::Delete
//
// Include constraints: this translation unit does not include dh2/Level.hpp or glitch-scene-collada-gui/scene_nodes.hpp.
// Level.hpp pulls in net-transport's stand-in IStreamBase, which redefines the class beside dh2/IStreamBase.hpp.
// scene_nodes.hpp redefines glitch::IEventReceiver and boost::intrusive_ptr beside the glitch-core-io headers that
// dh2/AnimController.hpp includes. Both pairs are engine-header conflicts outside this batch.

#include "dh2/Container.hpp"

#include "dh2/AnimController.hpp"
#include "dh2/IStreamBase.hpp"
#include "dh2/RootSceneNode.hpp"
#include "dh2/VisualObject.hpp"

// 0x39f324 _ZNK9Container11IsUpdatableEv
bool Container::IsUpdatable() const {
    return false;
}

// 0x39f32c _ZNK9Container9GetScriptEv
const char* Container::GetScript() const {
    return nullptr;
}

// 0x39f334 _ZNK9Container9GetVisualEv
int Container::GetVisual() const {
    return -1;
}

// 0x39f33c _ZNK9Container9GetDataIdEv
int Container::GetDataId() const {
    return -1;
}

// 0x39f344 _ZNK9Container7GetLootEv
int Container::GetLoot() const {
    return -1;
}

// 0x39f34c _ZNK9Container8GetSoundEv
int Container::GetSound() const {
    return -1;
}

// 0x39f354 _ZNK9Container11KeepPhysicsEv
bool Container::KeepPhysics() const {
    return false;
}

// 0x39f35c _ZNK9Container21GetLootFixedNumPowersEv
int Container::GetLootFixedNumPowers() const {
    return -1;
}

// 0x39f364 _ZNK9Container6IsDeadEv
// The original is (unsigned)(state - 3) <= 1: true for ACTIVATE (3) and OPENED (4) only.
bool Container::IsDead() const {
    return static_cast<unsigned int>(m_state) - 3u <= 1u;
}

// 0x39f37c _ZNK9Container10IsAnimatedEv
bool Container::IsAnimated() const {
    return true;
}

// 0x39f384 _ZNK9Container13IsInteractiveEP10GameObject
// True when the object is not marked for deletion (byte +0x81) and the state is IDLE. The second argument is unused.
bool Container::IsInteractive(GameObject* by) const {
    (void)by;
    return m_markedForDeletion == 0 && m_state == STATE_IDLE;
}

// 0x39f3a8 _ZNK9Container10IsObstacleEv
bool Container::IsObstacle() const {
    return true;
}

// 0x39f3b0 _ZNK9Container17GetObstacleRadiusEv
// The original returns the bit pattern 0x43160000, which is 150.0f.
float Container::GetObstacleRadius() const {
    return 150.0f;
}

// 0x39f3bc _ZNK9Container19GetObstacleStrengthEv
// The original returns the bit pattern 0x41200000, which is 10.0f.
float Container::GetObstacleStrength() const {
    return 10.0f;
}

// 0x39f3cc _ZN9Container8SetStateENS_5StateE
// Clears bit 0x400 of the visual's node flags when entering ACTIVATE, sets it for every other state, then stores
// the state. The visual and its node are dereferenced without a null check, as in the original. The original's
// int return (its first argument, r0) is not part of the void declaration.
void Container::SetState(State state) {
    RootSceneNode* node = m_visualObject->m_node;
    if (state == STATE_ACTIVATE) {
        node->unk_011c &= ~0x400u;
    } else {
        node->unk_011c |= 0x400u;
    }
    m_state = state;
}

// 0x39f3f0 _ZN9Container5SpawnEv
// From PRESPAWN with a visual: enter SPAWN and play "spawn" (PlayClip with loop 0, unused 0, animIndex 0).
// The original's int return (r0 on the fall-through path) is not part of the void declaration.
void Container::Spawn() {
    if (m_visualObject != nullptr && m_state == STATE_PRESPAWN) {
        SetState(STATE_SPAWN);
        m_visualObject->m_animController->PlayClip("spawn", false, 0, 0);
    }
}

// 0x39f44c _ZN9Container10__CallbackEPN6glitch5scene19ITimelineControllerEPv
// PARTIAL. SPAWN becomes IDLE and plays "idle"; ACTIVATE becomes OPENED and plays "idleactive". Those two branches
// are bound. The third branch is not: in any other state the original calls controller->getLoop() (vtable slot
// +0x44, glitch::scene::ITimelineController::getLoop in glitch-scene-collada-gui/scene_nodes.hpp) and, when it
// returns false, clears bit 0x200 of the visual's node flags (RootSceneNode +0x11c). scene_nodes.hpp cannot be
// included beside dh2/AnimController.hpp (see the header note above), so that branch does nothing here.
// The original's int return is not part of the void declaration.
void Container::__Callback(glitch::scene::ITimelineController* controller, void* user) {
    (void)controller;
    Container* container = static_cast<Container*>(user);
    if (container->m_state == STATE_SPAWN) {
        container->SetState(STATE_IDLE);
        container->m_visualObject->m_animController->PlayClip("idle", false, 0, 0);
    } else if (container->m_state == STATE_ACTIVATE) {
        container->SetState(STATE_OPENED);
        container->m_visualObject->m_animController->PlayClip("idleactive", false, 0, 0);
    }
}

// 0x39f534 _ZNK9Container9IsZonableEv
// Thunk: the original is a tail branch to GameObject::MeetCondition (0x38ab60).
bool Container::IsZonable() const {
    return GameObject::MeetCondition();
}

// 0x39f6f0 _ZN9Container11DeserializeEP11IStreamBase
// PARTIAL, not bound. The original calls GameObject::Deserialize, reads the state byte and applies it (SetState,
// then KeepPhysics() and SetPhysicalObject(nullptr, false) for OPENED, PlayClip "idleactive" or "idle"), but only
// when m_deathReset is set or the current level has neither Level +0xf3 (m_cameFromOtherLevel) nor +0xf4
// (m_loadingCheckpoint) set. That gate needs dh2/Level.hpp, which cannot share this translation unit with
// dh2/IStreamBase.hpp (readAs<unsigned char>) because of the net-transport IStreamBase stand-in. Until the two are
// unified the body is not written, so the stream is not read here.
void Container::Deserialize(IStreamBase* stream) {
    (void)stream;
}

// 0x39f8e0 _ZN9Container9SerializeEP11IStreamBase
// GameObject fields first, then the state as one byte.
void Container::Serialize(IStreamBase* stream) {
    GameObject::Serialize(stream);
    const unsigned char stateByte = static_cast<unsigned char>(m_state);
    stream->writeAs<unsigned char>(stateByte);
}
