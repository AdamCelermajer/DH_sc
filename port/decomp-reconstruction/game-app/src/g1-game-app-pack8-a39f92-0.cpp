// Batch g1-game-app-pack8-a39f92-0 (game-app, script command classes, part 1 of 1): Script_MoveActor, Script_OpenDoor,
// Script_PlayActorAnim, Script_PlayAnimById, Script_PlayAnimByName, Script_PlayCamera, Script_PlayEffect,
// Script_PlayLevelMusic and Script_PlaySound. Their declarations are in game/include/dh2/Script_<Class>.hpp, which this
// batch does not change (every method below is already declared there).
//
// Each Execute runs the isTracingScriptCmd probe first, subject to the skipping and music conditions in its pseudocode
// (Script_PlaySound's probe is conditional on its record, so its stub omits it; the MoveActor asm was checked). Every
// Execute also reads the command's argument record through ScriptCmdImpl::m_args (+0x0c). The records are the pydata
// Structs:: types (pydata-accessors/script_cmd.hpp). Where a record member or a sound, FX or table callee is still not
// declared, the body is PARTIAL and the note names it.

#include <cstdint>
#include <string>

#include "pydata-accessors/script_cmd.hpp"

#include "vox-audio/vox/sound_manager.hpp"
#include "dh2/AnimController.hpp"
#include "dh2/Application.hpp"
#include "dh2/CameraLevel.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Door.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/ObjectBase.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PFObject.hpp"
#include "dh2/Script_MoveActor.hpp"
#include "dh2/Script_OpenDoor.hpp"
#include "dh2/Script_PlayActorAnim.hpp"
#include "dh2/Script_PlayAnimById.hpp"
#include "dh2/Script_PlayAnimByName.hpp"
#include "dh2/Script_PlayCamera.hpp"
#include "dh2/Script_PlayEffect.hpp"
#include "dh2/Script_PlayLevelMusic.hpp"
#include "dh2/Script_PlaySound.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/v2Controller.hpp"

namespace {

// The trace probe that the original runs first in these Execute bodies: DebugSwitches::load() on the singleton, then
// GetSwitch("isTracingScriptCmd") with the result discarded. GetInstance() calls load() before it returns the singleton,
// which is the same sequence (see game-app/src/g1-game-app-DebugSwitches-d73440-0.cpp). DebugSwitches::s_inst is private
// in dh2/DebugSwitches.hpp, so GetInstance() supplies the object.
void TraceScriptCmdSwitch() {
    (void)DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingScriptCmd"));
}

}  // namespace

// 0x459698 bool Script_MoveActor::IsBlocking() const
// Blocks while the actor walks its path: the actor's path list is not empty. The list is GameObject m_pfObject (+0x1c8),
// then PFObject m_path (+0x38), so it sits at Character +0x200. When the list is empty the actor's controller is stopped
// and, if the actor was static before the move (m_wasStatic, +0x18), it is made static again and revived. Every field and
// call here is declared: unk_10 (+0x10), m_actor (+0x14), m_static (+0x84), m_controller (+0x378) and Revive.
bool Script_MoveActor::IsBlocking() const {
    if (!unk_10) {
        return false;
    }
    Character* actor = m_actor;
    if (actor == nullptr) {
        return false;
    }
    if (!actor->m_pfObject.m_path.empty()) {
        return true;
    }
    actor->m_controller->Cmd_Stop();
    if (m_wasStatic != 0) {
        actor->m_static = 1;
        actor->Revive(nullptr, true);
    }
    return false;
}

// 0x45f0a8 void Script_MoveActor::Execute(bool, int)
// The trace probe, then the actor (record +0x18, Structs::MoveActor m_w6) and the target (record +0x0c, m_w3) are looked up with
// ObjectManager::GetObjectByName(name, module, false, nullptr). The actor is stored in m_actor (+0x14), the record flag (+0x1c)
// in unk_10 (+0x10), and the actor's m_static in m_wasStatic (+0x18). When both exist, the record's collision flag (+0x10)
// disables the actor's collisions, and the controller flag +0x09 is set. Unless skipping, a static actor whose record flag is
// clear returns at once. A static actor with the flag set is made non-static after SetPhysicalObject(null, false): the asm
// (0x45f1e0) passes the entry skipping value, which is zero on this path, as both arguments. Then Cmd_MoveTo(target) runs, and
// when the actor's path is non-empty the command returns. Otherwise unk_10 is cleared. The skipping path and that last case warp
// with Cmd_WarpTo(target->m_position) (asm 0x45f274, target +0x160), then clear the flag. When either object is missing, unk_10
// is cleared and nothing else happens.
void Script_MoveActor::Execute(bool skipping, int module) {
    const Structs::MoveActor* record = static_cast<const Structs::MoveActor*>(m_args);
    TraceScriptCmdSwitch();
    ObjectManager* objects = ApplicationGetInstance().m_objectManager;

    ObjectHandle actorHandle = objects->GetObjectByName(record->m_w6, module, false, nullptr);
    Character* actor = nullptr;
    if (actorHandle.GetObject(false) != nullptr) {
        actor = static_cast<Character*>(actorHandle);
    }
    ObjectHandle targetHandle = objects->GetObjectByName(record->m_w3, module, false, nullptr);
    GameObject* target = nullptr;
    if (targetHandle.GetObject(false) != nullptr) {
        target = static_cast<GameObject*>(targetHandle);
    }

    unk_10 = record->m_w7;
    m_actor = actor;
    m_wasStatic = actor != nullptr ? static_cast<std::uint8_t>(actor->m_static) : 0;
    if (actor == nullptr || target == nullptr) {
        unk_10 = false;
        return;
    }

    v2Controller* controller = actor->m_controller;
    if (record->m_w4) {
        actor->DisableCollisions();
    }
    controller->unk_009 = true;
    if (!skipping) {
        if (m_wasStatic != 0) {
            if (!record->m_w7) {
                controller->unk_009 = false;
                return;
            }
            actor->SetPhysicalObject(nullptr, false);
            actor->m_static = false;
        }
        controller->Cmd_MoveTo(target);
        if (!actor->m_pfObject.m_path.empty()) {
            controller->unk_009 = false;
            return;
        }
        unk_10 = false;
    }
    controller->Cmd_WarpTo(target->m_position);
    controller->unk_009 = false;
}

// 0x455848 bool Script_OpenDoor::IsBlocking() const
// Blocks while an instant open is pending: the door resolves, the instant flag (+0x14) is set, and the door is neither
// DOOR_OPENED (1) nor DOOR_OPENING (3). Door m_state is at Door +0x3a8 (declared, public).
bool Script_OpenDoor::IsBlocking() const {
    if (m_door == nullptr || !m_instant) {
        return false;
    }
    const Door::DoorStates state = m_door->m_state;
    return !(state == Door::DOOR_OPENED || state == Door::DOOR_OPENING);
}

// 0x45d75c void Script_OpenDoor::Execute(bool, int)
// The trace probe, then the door named by the record (+0x0c, Structs::OpenDoor m_w3) is looked up with
// ObjectManager::GetObjectByName(name, module, false, nullptr) and ObjectHandle::GetObject(false). When the object's m_goId is
// GO_DOOR (2) it is stored in m_door, the record's instant flag (+0x10) is copied to m_instant, and Door::Open(skipping) runs.
// Otherwise m_door is cleared and m_instant is left as it was.
void Script_OpenDoor::Execute(bool skipping, int module) {
    const Structs::OpenDoor* record = static_cast<const Structs::OpenDoor*>(m_args);
    TraceScriptCmdSwitch();
    ObjectHandle doorHandle = ApplicationGetInstance().m_objectManager->GetObjectByName(record->m_w3, module, false, nullptr);
    ObjectBase* object = doorHandle.GetObject(false);
    if (object != nullptr && object->m_goId == ObjectBase::GO_DOOR) {
        m_door = static_cast<Door*>(object);
        m_instant = record->m_w4;
        m_door->Open(skipping);
    } else {
        m_door = nullptr;
    }
}

// 0x459628 bool Script_PlayActorAnim::IsBlocking() const
// PARTIAL. True when the record's flag at +0x1c (Structs::PlayActorAnim m_w7) is set, the m_actor handle (+0x10) resolves to an
// object whose m_goId (ObjectBase +0xf4) is 0, and (record +0x10, m_w4) + 2 equals that object's current animation: the
// CharAnimator m_currentAnim (Character +0x4e8, the CharAnimator base at +0x49c, field +0x4c). Gap: ObjectBase::GO_IDS names only
// GO_DOOR, so the cast to Character assumes that a goId of 0 is a Character. The tree does not confirm that.
bool Script_PlayActorAnim::IsBlocking() const {
    const Structs::PlayActorAnim* record = static_cast<const Structs::PlayActorAnim*>(m_args);
    if (!record->m_w7) {
        return false;
    }
    const ObjectBase* object = m_actor.GetObject(false);
    if (object == nullptr || static_cast<std::int32_t>(object->m_goId) != 0) {
        return false;
    }
    return record->m_w4 + 2 == static_cast<const Character*>(object)->m_currentAnim;
}

// 0x45a2ec void Script_PlayActorAnim::Init()
// Resolves the actor named by the record (+0x18, Structs::PlayActorAnim m_w6) with GetObjectByName(name, -1, false, nullptr).
// When it is a Character, the record's two animation dictionaries (+0x08, then +0x0c) are added to its CharAnimator
// (Character +0x49c) with ANIM_AddAnimDictToSet.
void Script_PlayActorAnim::Init() {
    const Structs::PlayActorAnim* record = static_cast<const Structs::PlayActorAnim*>(m_args);
    ObjectHandle actorHandle = ApplicationGetInstance().m_objectManager->GetObjectByName(record->m_w6, -1, false, nullptr);
    if (actorHandle.GetObject(false) == nullptr) {
        return;
    }
    Character* actor = static_cast<Character*>(actorHandle);
    if (actor == nullptr) {
        return;
    }
    CharAnimator* animator = actor;
    animator->ANIM_AddAnimDictToSet(record->m_w2);
    animator->ANIM_AddAnimDictToSet(record->m_w3);
}

// 0x45e890 void Script_PlayActorAnim::Execute(bool, int)
// PARTIAL. When skipping is clear, the trace probe runs, then the actor named by the record (+0x18) is looked up with
// GetObjectByName(name, module, false, nullptr) and stored in m_actor (+0x10). Not reproduced: the rest of the body. When the
// actor is a Character, the original writes record +0x08 into the struct that the Arrays::AnimTable row (anim + 2) points to at
// +0x0c, writes record +0x0c into the struct that row (anim + 8) points to, where anim is record +0x10, and then calls
// CharStateMachine::SM_SetAnimState(anim + 2, false, true) on the actor (Character +0x4fc). Gap: pydata gives Arrays::AnimTable
// only raw member bytes and no row accessor (rows are 20 bytes, with the pointer at row +0x0c).
void Script_PlayActorAnim::Execute(bool skipping, int module) {
    if (skipping) {
        return;
    }
    const Structs::PlayActorAnim* record = static_cast<const Structs::PlayActorAnim*>(m_args);
    TraceScriptCmdSwitch();
    m_actor = ApplicationGetInstance().m_objectManager->GetObjectByName(record->m_w6, module, false, nullptr);
}

// 0x45569c bool Script_PlayAnimById::IsBlocking() const
// Never blocks: the original returns 0.
bool Script_PlayAnimById::IsBlocking() const {
    return false;
}

// 0x45e24c void Script_PlayAnimById::Execute(bool, int)
// When skipping is clear: the trace probe, then the object named by the record (+0x14, Structs::PlayAnimById m_w5) is looked up
// with GetObjectByName(name, module, false, nullptr). When it resolves to a GameObject with a visual object (m_visualObject,
// +0x2d8) that has an animation controller (VisualObject m_animController, +0x38), the clip id (record +0x08, m_w2) is played with
// AnimController::PlayClip(clip, loop, 0, 0), where loop is record +0x0c (m_w3). That is the unsigned overload, virtual slot +0x1c.
void Script_PlayAnimById::Execute(bool skipping, int module) {
    if (skipping) {
        return;
    }
    const Structs::PlayAnimById* record = static_cast<const Structs::PlayAnimById*>(m_args);
    TraceScriptCmdSwitch();
    ObjectHandle objectHandle = ApplicationGetInstance().m_objectManager->GetObjectByName(record->m_w5, module, false, nullptr);
    if (objectHandle.GetObject(false) == nullptr) {
        return;
    }
    GameObject* object = static_cast<GameObject*>(objectHandle);
    if (object == nullptr || object->m_visualObject == nullptr) {
        return;
    }
    AnimController* controller = object->m_visualObject->m_animController;
    if (controller == nullptr) {
        return;
    }
    (void)controller->PlayClip(static_cast<unsigned int>(record->m_w2), record->m_w3, 0, 0);
}

// 0x4556a4 bool Script_PlayAnimByName::IsBlocking() const
// Never blocks: the original returns 0.
bool Script_PlayAnimByName::IsBlocking() const {
    return false;
}

// 0x45e124 void Script_PlayAnimByName::Execute(bool, int)
// When skipping is clear: the trace probe, then the object named by the record (+0x18, Structs::PlayAnimByName m_w6) is looked up
// as in Script_PlayAnimById. Its animation controller (VisualObject m_animController, +0x38, reached from GameObject m_visualObject
// +0x2d8) plays the clip named by the record (+0x0c, m_w3) with AnimController::PlayClip(name, loop, 0, 0), where loop is record
// +0x10 (m_w4). That is the name overload, virtual slot +0x20.
void Script_PlayAnimByName::Execute(bool skipping, int module) {
    if (skipping) {
        return;
    }
    const Structs::PlayAnimByName* record = static_cast<const Structs::PlayAnimByName*>(m_args);
    TraceScriptCmdSwitch();
    ObjectHandle objectHandle = ApplicationGetInstance().m_objectManager->GetObjectByName(record->m_w6, module, false, nullptr);
    if (objectHandle.GetObject(false) == nullptr) {
        return;
    }
    GameObject* object = static_cast<GameObject*>(objectHandle);
    if (object == nullptr || object->m_visualObject == nullptr) {
        return;
    }
    AnimController* controller = object->m_visualObject->m_animController;
    if (controller == nullptr) {
        return;
    }
    (void)controller->PlayClip(record->m_w3, record->m_w4, 0, 0);
}

// 0x459370 bool Script_PlayCamera::IsBlocking() const
// Blocks while the level camera is playing a clip. The record flag at +0x0c (Structs::PlayCamera m_w3) must be set, and the
// current level and its camera (Level m_camera, +0x128) must exist. The result is the camera's busy flag (CameraLevel
// m_zoomAnimActive, +0x84).
bool Script_PlayCamera::IsBlocking() const {
    const auto* args = static_cast<const Structs::PlayCamera*>(m_args);
    if (!args->m_w3) {
        return false;
    }
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level == nullptr || level->m_camera == nullptr) {
        return false;
    }
    return level->m_camera->m_zoomAnimActive;
}

// 0x460110 void Script_PlayCamera::Execute(bool, int)
// PARTIAL. The trace probe runs first. When the current level and its camera (Level m_camera, +0x128) exist and skipping is
// clear, the clip is played with CameraLevel::PlayAnim(record m_w2 (+0x08), 0, false): the original passes 0 and 0 for
// controllerArg and preserveZoom. When skipping is set, the clip id is field +0x10 of the Arrays::CamAnimSetTable row
// camera->m_camTableIndex (CameraLevel +0x80, rows of 28 bytes). That table has no element type in the tree (pydata gives
// Arrays::CamAnimSetTable only raw member bytes and declares no Structs::CamAnimSet), so the skipping branch makes no call.
void Script_PlayCamera::Execute(bool skipping, int /*module*/) {
    const auto* args = static_cast<const Structs::PlayCamera*>(m_args);
    TraceScriptCmdSwitch();
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level == nullptr || level->m_camera == nullptr) {
        return;
    }
    if (skipping) {
        // PARTIAL: the clip id comes from Arrays::CamAnimSetTable (see above).
        return;
    }
    level->m_camera->PlayAnim(args->m_w2, 0, false);
}

// 0x4556ac bool Script_PlayEffect::IsBlocking() const
// Never blocks: the original returns 0.
bool Script_PlayEffect::IsBlocking() const {
    return false;
}

// 0x4598b4 void Script_PlayEffect::Init()
// PARTIAL. The whole original body is one call: VisualFXManager::RegisterFXSetToLoad(&Singleton<VisualFXManager>::s_inst,
// record +0x08). VisualFXManager::RegisterFXSetToLoad(int) and s_inst are declared (dh2/VisualFXManager.hpp). Missing: the FX
// set id at record +0x08. pydata Structs::PlayEffect declares only m_w3 (+0x0c) and m_w8 (+0x20), so the id has no member and
// nothing is registered here. Layout request, Structs::PlayEffect: m_w2 int at +0x08 (FX set id).
void Script_PlayEffect::Init() {}

// 0x45fa9c void Script_PlayEffect::Execute(bool, int)
// PARTIAL. Reproduced: the trace probe, only when skipping is clear. Missing: the record fields and the call. The original reads
// the FX set id (record +0x08), the position offsets (+0x10, +0x14, +0x18, converted to float in that order) and the object
// name (+0x20), resolves the object with GetObjectByName(name, module, false, nullptr), and calls
// VisualFXManager::PlayAnimFXSet(id, position, object, nullptr), which is declared. Layout request, Structs::PlayEffect: m_w2 int
// at +0x08, m_w4, m_w5 and m_w6 int at +0x10, +0x14 and +0x18, and m_w8 at +0x20 as const char* (it is uint32_t now).
void Script_PlayEffect::Execute(bool skipping, int /*module*/) {
    if (skipping) {
        return;
    }
    TraceScriptCmdSwitch();
}

// 0x455660 bool Script_PlayLevelMusic::IsBlocking() const
// Never blocks: the original returns 0.
bool Script_PlayLevelMusic::IsBlocking() const {
    return false;
}

// 0x45fd20 void Script_PlayLevelMusic::Execute(bool, int)
// The tracing switch, then the current level. When there is a level: VoxSoundManager::PlayMusic(s_instance, level
// m_musicSoundId (+0x11c), true, false, record m_w2 (+0x08)), then SetMusicState "ambient" or "combat", chosen by the
// manager's music_slot_ byte (+0x31).
void Script_PlayLevelMusic::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level == nullptr) {
        return;
    }
    const auto* args = static_cast<const Structs::PlayLevelMusic*>(m_args);
    VoxSoundManager* sound = VoxSoundManager::s_instance;
    sound->PlayMusic(level->m_musicSoundId, true, false, args->m_w2);
    sound->SetMusicState(sound->music_slot_ ? "ambient" : "combat");
}

// 0x455650 bool Script_PlaySound::IsBlocking() const
// Never blocks: the original returns 0.
bool Script_PlaySound::IsBlocking() const {
    return false;
}

// 0x45fefc void Script_PlaySound::Execute(bool, int)
// PARTIAL. The trace probe runs only when skipping is clear or the record's music flag (Structs::PlaySound m_w3, +0x0c) is
// set. Then the dispatch is rebuilt: with the music flag set, VoxSoundManager::PlayMusic(record m_w4 (+0x10), record m_off13
// (+0x0d), record m_w2 != 0, 2000) on VoxSoundManager::s_instance; otherwise VoxSoundManager::Play(m_w4, m_off13, m_w2, m_w3,
// m_w3). Those two calls are not rebuilt: VoxSoundManager::Play, PlayMusic and s_instance are not declared in
// vox-audio/include/vox-audio/vox/sound_manager.hpp.
void Script_PlaySound::Execute(bool skipping, int /*module*/) {
    const auto* args = static_cast<const Structs::PlaySound*>(m_args);
    if (!skipping || args->m_w3) {
        TraceScriptCmdSwitch();
    }
    if (args->m_w3) {
        // PARTIAL: VoxSoundManager::PlayMusic (see above).
    } else {
        // PARTIAL: VoxSoundManager::Play (see above).
    }
}
