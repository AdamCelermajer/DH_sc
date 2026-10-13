// Batch g1-game-app-pack6-496562-0 (game-app, nine script command classes): the IsBlocking and Execute bodies of
// Script_EndGame, Script_EnqueueCharMenuTutorialMessage, Script_EnqueueTutorialMessage, Script_EnterCutSceneMode,
// Script_EnterSafeZone, Script_ExecScript, Script_ExitCutSceneMode, Script_FlushMessages and Script_HideActor
// (ELF 0x455638 .. 0x460b1c).
//
// Facts shared by the commands:
//   - Each Execute reads its argument record from ScriptCmdImpl::m_args (+0x0c). The record is the pydata-accessors
//     Structs::<Name> of the command (pydata-accessors/script_cmd.hpp), whose member m_wN is the word at 4*N.
//   - Four Executes read the tracing switch first: DebugSwitches::load(), then GetSwitch("isTracingScriptCmd") with the
//     result discarded (Script_EndGame, Script_EnterSafeZone, Script_ExecScript, Script_HideActor). The other five do not.
//     TraceScriptCmdSwitch below does what the original does.
//   - The named lookup is ObjectManager::GetObjectByName(name, module, false, nullptr) on the global manager, which is
//     Application::m_objectManager (+0x38, the global dword_99F764).
//   - The message queues are m_queues[0] of Singleton<MenuMessageManager<T, N>>::s_inst. The asm loads each singleton
//     pointer through the GOT and reads its deque at +4 (begin) and +0x14 (finish). No singleton pointer is declared in
//     the tree (see global-f216b6-12.cpp), so the queue bodies stay partial.
//
// Partial bodies (the ledger notes say what is missing):
//   - Script_EnqueueCharMenuTutorialMessage, Script_EnqueueTutorialMessage and Script_FlushMessages need the queue
//     singletons. Script_EnqueueTutorialMessage also needs dh2/TutorialMsg.hpp, and Script_FlushMessages needs
//     dh2/StatusMsg.hpp and dh2/OnlineStatusMsg.hpp, none of which is in the tree.
//   - Script_EnterCutSceneMode and Script_ExitCutSceneMode need RenderFX::InvokeASCallback and RenderFX::GetRoot (not
//     declared), the byte global byte_9A600C (not declared), the PlayerInfo virtual at vtable +0x50 (not declared), the
//     Level flag at +0x198, the COnline flag at +0x05 (private, no getter), and PlayerManager +0x6c4 (the player count
//     the loops read; not declared).
//   - Script_EnterSafeZone needs VoxSoundManager::s_instance and VoxSoundManager::SetInSafeZoneMusic(bool), which
//     vox-audio/include/vox-audio/vox/sound_manager.hpp does not declare.
//   - Script_ExecScript needs the record fields +0x08, +0x0c, +0x10, +0x14 (a pointer) and +0x18. Structs::ExecScript
//     declares only m_w5 at +0x14, as uint32_t.

#include <string>

#include "pydata-accessors/script_cmd.hpp"

#include "vox-audio/vox/sound_manager.hpp"
#include "dh2/AnimController.hpp"
#include "dh2/Application.hpp"
#include "dh2/CharMenuTutorialMsg.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/ScriptCmdImpl.hpp"
#include "dh2/Script_EndGame.hpp"
#include "dh2/Script_EnqueueCharMenuTutorialMessage.hpp"
#include "dh2/Script_EnqueueTutorialMessage.hpp"
#include "dh2/Script_EnterCutSceneMode.hpp"
#include "dh2/Script_EnterSafeZone.hpp"
#include "dh2/Script_ExecScript.hpp"
#include "dh2/Script_ExitCutSceneMode.hpp"
#include "dh2/Script_FlushMessages.hpp"
#include "dh2/Script_HideActor.hpp"
#include "dh2/global/ctrl_camera.hpp"

// Start of every Execute that traces: DebugSwitches::load() and GetSwitch("isTracingScriptCmd"), result discarded.
// GetInstance() is the only public route to DebugSwitches::s_inst (s_inst is private in dh2/DebugSwitches.hpp), and the
// load is repeated on each call as in the original.
static void TraceScriptCmdSwitch() {
    DebugSwitches::GetInstance()->load();
    DebugSwitches::GetInstance()->GetSwitch(std::string("isTracingScriptCmd"));
}

// ---- Script_EndGame ----

// 0x455910 Script_EndGame::IsBlocking() const
bool Script_EndGame::IsBlocking() const {
    return false;
}

// 0x45ca80 Script_EndGame::Execute(bool, int)
// PARTIAL. Traces the switch, then works on the local player's character (PlayerManager::GetLocalPlayer(0, 1)->character_660,
// which is PlayerManager::GetLocalPlayerCharacter, 0x330794). Its difficulty (SG_GetGameDifficulty, read twice) is kept, and
// the unlocked difficulty is that value plus one, capped at 2. The character gets SG_SetLevelId(45, difficulty),
// SG_SetLevelEntryPoint(5, difficulty), SG_SetGameDifficultyUnlocked(unlocked), SG_Save() and SG_SetGameDifficulty(unlocked),
// in that order. Then Application::m_stateMachine (+0x18) switches to GSEndGame::s_inst with owned = false.
// Not rebuilt: the character lookup. dh2/PlayerManager.hpp cannot be included in this translation unit. Its StreamBuffer
// member needs the net-transport IStreamBase, which clashes with the pydata-accessors IStreamBase that the other commands
// here need (the same conflict as Script_SaveGame in g1-game-app-pack9-857bfe-0.cpp). The sequence above is kept as a
// comment until the two IStreamBase definitions are merged.
void Script_EndGame::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
    // PARTIAL: the character sequence described above is not rebuilt (see the comment).
}

// ---- Script_EnqueueCharMenuTutorialMessage ----

// 0x455950 Script_EnqueueCharMenuTutorialMessage::IsBlocking() const
bool Script_EnqueueCharMenuTutorialMessage::IsBlocking() const {
    return false;
}

// 0x460b1c Script_EnqueueCharMenuTutorialMessage::Execute(bool, int)
// PARTIAL. Builds the CharMenuTutorialMsg from the record: the id from m_w6 (+0x18), the frame name from m_w3 (+0x0c)
// and the menu name from m_w5 (+0x14). The original then pushes it onto the CharMenuTutorialMsg queue (the global at
// dword_99FBB8, m_queues[0] of the MenuMessageManager<CharMenuTutorialMsg, 1> singleton). That singleton pointer is not
// declared in the tree, so the push is not rebuilt and the message is dropped here.
void Script_EnqueueCharMenuTutorialMessage::Execute(bool /*skipping*/, int /*module*/) {
    auto* args = static_cast<Structs::EnqueueCharMenuTutorialMessage*>(m_args);
    CharMenuTutorialMsg message(-1, std::string(), std::string());
    message.m_stringId = args->m_w6;
    message.m_frame.assign(args->m_w3);
    message.m_menu.assign(args->m_w5);
    // PARTIAL: push_back(message) on the CharMenuTutorialMsg queue is missing (see above).
    (void)message;
}

// ---- Script_EnqueueTutorialMessage ----

// 0x455938 Script_EnqueueTutorialMessage::IsBlocking() const
bool Script_EnqueueTutorialMessage::IsBlocking() const {
    return false;
}

// 0x4613d4 Script_EnqueueTutorialMessage::Execute(bool, int)
// PARTIAL. When the record's m_w3 (+0x0c) is not negative, the original pushes the pair {m_w3, m_w2 (+0x08)} onto the
// TutorialMsg queue (m_queues[0] of Singleton<MenuMessageManager<TutorialMsg, 1>>::s_inst; asm: begin +4, finish +0x14).
// When the queue then holds exactly one entry, it calls MenuMessageManager<TutorialMsg, 1>::Invoke with the start
// function name TutorialMsg::s_StartFuncName (asm: the string "onTutorialMessage"). Missing: dh2/TutorialMsg.hpp (no header
// in the tree, so the element type cannot be named), the queue singleton, and the queue argument of Invoke.
void Script_EnqueueTutorialMessage::Execute(bool /*skipping*/, int /*module*/) {
    auto* args = static_cast<Structs::EnqueueTutorialMessage*>(m_args);
    // PARTIAL: the push of {m_w3, m_w2} and the start call are not rebuilt (see above).
    (void)args;
}

// ---- Script_EnterCutSceneMode ----

// 0x455638 Script_EnterCutSceneMode::IsBlocking() const
bool Script_EnterCutSceneMode::IsBlocking() const {
    return false;
}

// 0x45a384 Script_EnterCutSceneMode::Execute(bool, int)
// PARTIAL. The original, in order:
//   1. When the current level is set and either its byte at +0x198 (Level declares no such field) is clear or
//      MenuBase::s_igmOpened is set: MenuManager::GetInstance()->m_multiMenuManager (+0xf4) PopMenu(true) (vtable +0x38),
//      Character::ReloadSkills() on the local player's character when it is set, and ShowStatubBar(false).
//   2. When the COnline flag at +0x05 is set, the local player's character is set and its IsDead() (vtable +0x34) is true,
//      it sets a NetStructMember of that player to -1 and marks it changed. Not rebuilt.
//   3. The HUD callback "StartCutscene" on the HUD root. RenderFX::InvokeASCallback and RenderFX::GetRoot are not declared.
//   4. byte_9A600C = 1 (global not declared). AnimController::s_scalingEnabled = false is reconstructed below.
//   5. When the COnline flag is set, each player (count from PlayerManager +0x6c4, not declared) is put in the cutscene:
//      PlayerInfo::SetInCutscene(true) when its virtual at +0x50 returns true. Otherwise its character (+0x660) gets the
//      virtual at +0x40 with no bool shown by Hex-Rays. Not rebuilt.
void Script_EnterCutSceneMode::Execute(bool /*skipping*/, int /*module*/) {
    AnimController::s_scalingEnabled = false;
}

// ---- Script_EnterSafeZone ----

// 0x455668 Script_EnterSafeZone::IsBlocking() const
bool Script_EnterSafeZone::IsBlocking() const {
    return false;
}

// 0x45fc80 Script_EnterSafeZone::Execute(bool, int)
// The tracing switch, then VoxSoundManager::SetInSafeZoneMusic(true) on VoxSoundManager::s_instance (0x9a238c).
void Script_EnterSafeZone::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
    VoxSoundManager::s_instance->SetInSafeZoneMusic(true);
}

// ---- Script_ExecScript ----

// 0x455c08 Script_ExecScript::IsBlocking() const
// PARTIAL. The original tests the record's flag at +0x18 (pydata m_w6, not declared in Structs::ExecScript). When the
// flag is set it returns ScriptManager::s_inst.IsScriptRunning(unk_10), and when it is clear it returns the flag. Only the
// clear answer is written, so this command never waits on the script it started.
bool Script_ExecScript::IsBlocking() const {
    return false;
}

// 0x4607f4 Script_ExecScript::Execute(bool, int)
// PARTIAL. The original picks the script id. When the record's count at +0x10 is non-zero it draws
// Random::s_seed = (59051 * Random::s_seed + 177149) % 0xdaf26b, takes the draw modulo the count, counts the draw in
// Random::s_debugRandomCounters, and reads the id from the list at +0x14 (a pointer). Otherwise the id is the record's
// single id at +0x0c. When the flag at +0x08 is clear the debug offset dword_9A5FE4 is added. The id is stored in unk_10
// and run through ScriptManager::s_inst.StartScript(id, module, true). Missing: the record fields +0x08, +0x0c, +0x10,
// +0x14 and +0x18 (Structs::ExecScript declares only m_w5 at +0x14, as uint32_t), and the global dword_9A5FE4 (not
// declared). Only the tracing switch is rebuilt.
void Script_ExecScript::Execute(bool /*skipping*/, int /*module*/) {
    TraceScriptCmdSwitch();
    // PARTIAL: the id selection and StartScript are not rebuilt (see above).
}

// ---- Script_ExitCutSceneMode ----

// 0x455640 Script_ExitCutSceneMode::IsBlocking() const
bool Script_ExitCutSceneMode::IsBlocking() const {
    return false;
}

// 0x4599ac Script_ExitCutSceneMode::Execute(bool, int)
// PARTIAL. Reconstructed: AnimController::s_scalingEnabled = true. Not rebuilt:
//   1. The HUD callback "StopCutscene" on the HUD root. The chain is MenuManager::GetInstance and GetHUDRoot (twice),
//      RenderFX::GetRoot, gameswf::root::get_root_movie, then RenderFX::InvokeASCallback. The header return types do not
//      match the chain, and RenderFX::InvokeASCallback and RenderFX::GetRoot are not declared.
//   2. byte_9A600C = 0 (global not declared).
//   3. When the COnline flag at +0x05 (private, no getter) is set: a CMsgScriptCmd message (CMessage::CreateMessage with
//      true, then the fields at CMessage +0x50 = 1, +0x54 = -2 and +0x58 = -1, none declared) is sent through
//      CMessaging::SendMsg on CMessaging::Get(GetOnline()).
//   4. Each player (count from PlayerManager +0x6c4, not declared): when the PlayerInfo virtual at +0x50 returns true,
//      PlayerInfo::SetInCutscene(false). Otherwise its character (+0x660) gets the virtual at +0x40 with true.
void Script_ExitCutSceneMode::Execute(bool /*skipping*/, int /*module*/) {
    AnimController::s_scalingEnabled = true;
}

// ---- Script_FlushMessages ----

// 0x455978 Script_FlushMessages::IsBlocking() const
bool Script_FlushMessages::IsBlocking() const {
    return false;
}

// 0x460a58 Script_FlushMessages::Execute(bool, int)
// PARTIAL. The asm gives this order: MenuMessageManager<DialogMsg, 1>::FlushEnqueuedMessages(int) (a GCC clone,
// .clone.21; the asm sets no argument register before the call), then the AchievementMsg queue popped until empty, then
// the same DialogMsg flush again, then the StatusMsg queue, then the OnlineStatusMsg queue. Each queue is m_queues[0] of
// its Singleton<MenuMessageManager<T, N>>::s_inst (GOT pointers; deque begin at +4, finish at +0x14). Missing: the singleton
// pointers (none declared in the tree), dh2/StatusMsg.hpp and dh2/OnlineStatusMsg.hpp (no header in the tree, so the
// element types cannot be named), and a Flush accessor usable without a singleton.
void Script_FlushMessages::Execute(bool /*skipping*/, int /*module*/) {
    // PARTIAL: nothing is rebuilt here (see above).
}

// ---- Script_HideActor ----

// 0x455828 Script_HideActor::IsBlocking() const
bool Script_HideActor::IsBlocking() const {
    return false;
}

// 0x45e028 Script_HideActor::Execute(bool, int)
// Looks the named object up (record m_w3, the name at +0x0c) on the global ObjectManager. When the handle resolves to an
// object, it calls that object's virtual SetVisible(false) (vtable +0x40, GameObject::SetVisible).
void Script_HideActor::Execute(bool /*skipping*/, int module) {
    auto* args = static_cast<Structs::HideActor*>(m_args);
    TraceScriptCmdSwitch();
    ObjectHandle handle =
        ApplicationGetInstance().m_objectManager->GetObjectByName(args->m_w3, module, false, nullptr);
    if (handle.GetObject(false)) {
        GameObject* object = handle;
        if (object) {
            object->SetVisible(false);
        }
    }
}
