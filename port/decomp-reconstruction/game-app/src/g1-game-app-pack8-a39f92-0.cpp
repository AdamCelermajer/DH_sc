// Batch g1-game-app-pack8-a39f92-0 (game-app, script command classes, part 1 of 1): Script_MoveActor, Script_OpenDoor,
// Script_PlayActorAnim, Script_PlayAnimById, Script_PlayAnimByName, Script_PlayCamera, Script_PlayEffect,
// Script_PlayLevelMusic and Script_PlaySound. Their declarations are in game/include/dh2/Script_<Class>.hpp, which this
// batch does not change (every method below is already declared there).
//
// Each Execute runs the isTracingScriptCmd probe first, subject to the skipping and music conditions in its pseudocode
// (Script_PlaySound's probe is conditional on its record, so its stub omits it; the MoveActor asm was checked). Every
// Execute also reads the command's argument record through ScriptCmdImpl::m_args (+0x0c). No class or layout for those records is declared in the tree, so the
// Execute and Init bodies are PARTIAL: the probe is reproduced and the record fields are named in each note as a layout
// request for the integrator.

#include <string>

#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Door.hpp"
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
// PARTIAL. Reproduced: the isTracingScriptCmd trace probe. Missing: the rest of the body, which reads the argument record
// (ScriptCmdImpl m_args +0x0c, no class in the tree). Layout request, Script_MoveActor argument record: +0x0c const char*
// target name, +0x10 byte collision flag, +0x18 const char* actor name, +0x1c byte flag (copied to unk_10).
// The body resolves the actor (Character) and the target (GameObject) with ApplicationGetInstance().m_objectManager,
// GetObjectByName(name, module, false, nullptr). It stores the actor in m_actor (+0x14) and the actor's m_static in
// m_wasStatic (+0x18). When both exist it sets the controller flag unk_009 (+0x09) to 1, calling DisableCollisions first
// when the collision flag is set. Skipping goes straight to v2Controller::Cmd_WarpTo(&target->m_position), where the
// position is GameObject m_position (+0x160), and then clears unk_009. Otherwise, when m_wasStatic is set and the flag is
// clear, it clears unk_009 and returns. When m_wasStatic is set and the flag is set, it calls SetPhysicalObject(actor,
// skipping, skipping) and stores skipping into the actor's m_static (asm 0x45f1e0..0x45f1fc reloads the entry skipping
// slot). The source expression for those two values is not recovered, so that branch is not written. After that it calls
// Cmd_MoveTo(target). If the actor's path is non-empty it clears unk_009 and returns. Otherwise it clears unk_10 and warps
// as above. When either object is missing, it clears unk_10 and returns.
void Script_MoveActor::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
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
// PARTIAL. Reproduced: the isTracingScriptCmd trace probe. Missing: the rest of the body, which reads the argument record
// (ScriptCmdImpl m_args +0x0c, no class in the tree). Layout request, Script_OpenDoor argument record: +0x0c const char*
// door name, +0x10 byte instant flag. The body then looks the door up with ApplicationGetInstance().m_objectManager,
// GetObjectByName(name, module, false, nullptr). When the object's m_goId (ObjectBase +0xf4) equals 2, the Door is kept in
// m_door, m_instant is set from the record, and Door::Open(skipping) runs. Otherwise m_door is cleared. The value 2 is read
// from the pseudocode; the ObjectBase::GO_IDS enumerators are not recovered (the enum is empty in the tree).
void Script_OpenDoor::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
}

// 0x459628 bool Script_PlayActorAnim::IsBlocking() const
// PARTIAL. Missing: the argument record (m_args +0x0c, no class in the tree). The original returns true when the record's
// byte at +0x1c is set, the m_actor handle (+0x10) resolves, the resolved object's m_goId (+0xf4) is 0, and
// (record +0x10) + 2 equals the actor's current animation: CharAnimator m_currentAnim (Character +0x4e8, CharAnimator +0x4c).
// Returns false (not blocking) until the record is declared.
bool Script_PlayActorAnim::IsBlocking() const {
    return false;
}

// 0x45a2ec void Script_PlayActorAnim::Init()
// PARTIAL. Missing: the argument record (m_args +0x0c, no class in the tree). Layout request, Script_PlayActorAnim argument
// record: +0x08 int dict id, +0x0c int dict id, +0x10 int anim index, +0x18 const char* actor name. The original resolves
// the actor with GetObjectByName(name, -1, false, nullptr) on ApplicationGetInstance().m_objectManager, takes its Character,
// and calls CharAnimator::ANIM_AddAnimDictToSet on the actor's CharAnimator (Character +0x49c) with +0x08 and then +0x0c.
// Nothing is added until the record is declared.
void Script_PlayActorAnim::Init() {}

// 0x45e890 void Script_PlayActorAnim::Execute(bool, int)
// PARTIAL. Reproduced: the trace probe, only when skipping is clear. Missing: the rest of the body, which reads the argument
// record (m_args +0x0c, no class in the tree). Layout request, Script_PlayActorAnim argument record: as in Init above. The
// body stores the resolved actor handle in m_actor (+0x10), stores record +0x08 at +0x08 of the struct that Arrays::AnimTable
// row (anim + 2) points to at +0x0c, and record +0x0c at +0x08 of the struct that row (anim + 8) points to at +0x0c (rows are 20
// bytes; pydata has no field accessor for this table), then calls CharStateMachine::SM_SetAnimState(anim + 2, false, true) on the
// actor (Character +0x4fc).
void Script_PlayActorAnim::Execute(bool skipping, int /*module*/) {
    if (skipping) {
        return;
    }
    TraceScriptCmdSwitch();
}

// 0x45569c bool Script_PlayAnimById::IsBlocking() const
// Never blocks: the original returns 0.
bool Script_PlayAnimById::IsBlocking() const {
    return false;
}

// 0x45e24c void Script_PlayAnimById::Execute(bool, int)
// PARTIAL. Reproduced: the trace probe, only when skipping is clear. Missing: the rest of the body, which reads the argument
// record (m_args +0x0c, no class in the tree). Layout request, Script_PlayAnimById argument record: +0x08 int clip id,
// +0x0c byte loop flag, +0x14 const char* object name. The body resolves the object with GetObjectByName(name, module, false,
// nullptr) on ApplicationGetInstance().m_objectManager, takes its GameObject, and, when that has a visual object (GameObject
// m_visualObject +0x2d8) with an animation controller (VisualObject m_animController +0x38), calls
// AnimController::PlayClip(clip, loop, 0, 0) through virtual slot +0x1c (the unsigned clip overload).
void Script_PlayAnimById::Execute(bool skipping, int /*module*/) {
    if (skipping) {
        return;
    }
    TraceScriptCmdSwitch();
}

// 0x4556a4 bool Script_PlayAnimByName::IsBlocking() const
// Never blocks: the original returns 0.
bool Script_PlayAnimByName::IsBlocking() const {
    return false;
}

// 0x45e124 void Script_PlayAnimByName::Execute(bool, int)
// PARTIAL. Reproduced: the trace probe, only when skipping is clear. Missing: the rest of the body, which reads the argument
// record (m_args +0x0c, no class in the tree). Layout request, Script_PlayAnimByName argument record: +0x0c clip name (const char*,
// inferred from the name overload at slot +0x20), +0x10 byte loop flag, +0x18 const char* object name. The body resolves the object as in Script_PlayAnimById and calls
// the name overload AnimController::PlayClip(clip name, loop, 0, 0) through virtual slot +0x20 on the object's
// animation controller (VisualObject m_animController +0x38, reached from GameObject m_visualObject +0x2d8).
void Script_PlayAnimByName::Execute(bool skipping, int /*module*/) {
    if (skipping) {
        return;
    }
    TraceScriptCmdSwitch();
}

// 0x459370 bool Script_PlayCamera::IsBlocking() const
// PARTIAL. Missing: the argument record (m_args +0x0c, no class in the tree) and CameraLevel, which is not declared (waits on
// dh2/CameraLevel.hpp). The original returns the camera's busy byte (CameraLevel +0x84) when the record's byte at +0x0c is
// set and the current level (ApplicationGetInstance().GetCurrentLevel()) and its camera (Level m_camera +0x128) exist.
// Returns false (not blocking) until both are declared.
bool Script_PlayCamera::IsBlocking() const {
    return false;
}

// 0x460110 void Script_PlayCamera::Execute(bool, int)
// PARTIAL. Reproduced: the trace probe (unconditional). Missing: the rest of the body. Layout request, Script_PlayCamera
// argument record: +0x08 int anim id, +0x0c byte flag (used by IsBlocking). The original takes the current level's camera
// (Level m_camera +0x128) and calls CameraLevel::PlayAnim(camera, id, false, false). The id is the record's +0x08 value, or,
// when skipping is set, field +0x10 of the Arrays::CamAnimSetTable row (rows are 28 bytes) selected by the camera's index
// (CameraLevel +0x80). pydata has no accessor for that table, and CameraLevel is not declared (waits on dh2/CameraLevel.hpp).
void Script_PlayCamera::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
}

// 0x4556ac bool Script_PlayEffect::IsBlocking() const
// Never blocks: the original returns 0.
bool Script_PlayEffect::IsBlocking() const {
    return false;
}

// 0x4598b4 void Script_PlayEffect::Init()
// PARTIAL. The whole original body is one call: VisualFXManager::RegisterFXSetToLoad(&Singleton<VisualFXManager>::s_inst,
// record +0x08). VisualFXManager is not declared (waits on dh2/VisualFXManager.hpp), and the argument record (m_args +0x0c,
// no class in the tree) is not declared either, so nothing is registered here.
void Script_PlayEffect::Init() {}

// 0x45fa9c void Script_PlayEffect::Execute(bool, int)
// PARTIAL. Reproduced: the trace probe, only when skipping is clear. Missing: the rest of the body, which reads the argument
// record (m_args +0x0c, no class in the tree) and VisualFXManager, which is not declared (waits on dh2/VisualFXManager.hpp).
// Layout request, Script_PlayEffect argument record: +0x08 int FX set id, +0x10 / +0x14 / +0x18 int position offsets (converted
// to float, in that order), +0x20 const char* object name. The body resolves the object with GetObjectByName(name, module, false,
// nullptr) and calls VisualFXManager::PlayAnimFXSet(FX set id, position, object, 0).
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
// PARTIAL. Reproduced: the trace probe (unconditional). Missing: the rest of the body. Layout request, Script_PlayLevelMusic
// argument record: +0x08 int music argument. The original reads the current level (ApplicationGetInstance().GetCurrentLevel()),
// calls VoxSoundManager::PlayMusic(level m_musicSoundId (+0x11c), 1, 0, record +0x08) on VoxSoundManager::s_instance, then sets
// the music state to "ambient" or "combat" from the manager's ambient byte (+0x31). VoxSoundManager::PlayMusic, SetMusicState and
// s_instance are not declared in vox-audio/include/vox-audio/vox/sound_manager.hpp, and the record is not declared.
void Script_PlayLevelMusic::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
}

// 0x455650 bool Script_PlaySound::IsBlocking() const
// Never blocks: the original returns 0.
bool Script_PlaySound::IsBlocking() const {
    return false;
}

// 0x45fefc void Script_PlaySound::Execute(bool, int)
// PARTIAL. The original runs the trace probe only when skipping is clear or the record's music byte (+0x0c) is set, then either
// VoxSoundManager::PlayMusic(cue, flag, record +0x08 != 0, 2000) when that music byte is set, or VoxSoundManager::Play(cue,
// byte +0x0d, record +0x08, byte +0x0c, byte +0x0c). Layout request, Script_PlaySound argument record: +0x08 int, +0x0c byte
// music flag, +0x0d byte flag, +0x10 int cue id. The record is not declared, and VoxSoundManager::Play and PlayMusic are not
// declared in vox-audio/include/vox-audio/vox/sound_manager.hpp, so the body is empty.
void Script_PlaySound::Execute(bool /*skipping*/, int /*module*/) {}
