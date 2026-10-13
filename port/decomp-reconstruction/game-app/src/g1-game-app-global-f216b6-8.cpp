// Batch g1-game-app-global-f216b6-8 (<global>, part 9 of 22): the gameswf natives of the sound and music menu,
// the menu stack and the settings save and load, and IsNonDebugQuest. ELF 0x43acdc .. 0x43b4f0. Declarations:
// dh2/global/native_sound_menu.hpp.
//
// Facts shared by the bodies:
//   - Singleton<Application>::s_inst (ELF 0x99f72c) is the Application object. The fields the natives use are
//     +0x18 m_stateMachine, +0x40 m_playerManager, +0x4c m_savegameManager, +0x54 m_menuManager. The object is
//     reached through ApplicationGetInstance() (dh2/global/ctrl_camera.hpp).
//   - VoxSoundManager::s_instance (ELF 0x9a238c) is the sound manager. Its members used by the natives are not
//     declared in the tree (see the rows), so every native that drives it is PARTIAL.
//   - gameswf calls a native as void(const fn_call&). The natives return void; the Hex-Rays int return is the
//     leftover r0 of the last call.
//   - The gameswf argument and result API (fn_call::result, nargs, arg(0), as_value::to_xstring, to_number,
//     set_bool) is not in the tree. A native that reads an argument or writes its result keeps the logic that
//     does not depend on it, and its row is PARTIAL.

#include "dh2/global/native_sound_menu.hpp"

#include <cstring>

#include "core-platform/w7_global_rem.hpp"  // UpdateSavedValues (0x43a8f0)
#include "dh2/Application.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/Quest.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/global/ctrl_camera.hpp"

// 0x43acdc NativePauseAllSounds(gameswf::fn_call const&)
// PARTIAL: the original calls VoxSoundManager::PauseAllSounds on VoxSoundManager::s_instance when it is non-null.
// VoxSoundManager::s_instance and PauseAllSounds are not declared in the tree (vox-audio's sound_manager.hpp
// declares only SetIPodShuffle, UpdateIPodAsync and ConvertVisual3DToSound3D). The call is left out.
void NativePauseAllSounds(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ad04 NativePauseMusic(gameswf::fn_call const&)
// PARTIAL: the original calls VoxSoundManager::PauseMusic on VoxSoundManager::s_instance when it is non-null.
// PauseMusic and s_instance are not declared in the tree. The call is left out.
void NativePauseMusic(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ad2c NativeStopMusic(gameswf::fn_call const&)
// PARTIAL: the original calls VoxSoundManager::StopMusic(s_instance, 500) when s_instance is non-null. StopMusic(int)
// and s_instance are not declared in the tree. The call is left out.
void NativeStopMusic(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ad58 NativePlayPreviousMusic(gameswf::fn_call const&)
// PARTIAL: the original calls VoxSoundManager::RestartMusic(s_instance, 0) when s_instance is non-null.
// RestartMusic(int) and s_instance are not declared in the tree. The call is left out.
void NativePlayPreviousMusic(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ad84 NativePlayMusic(gameswf::fn_call const&)
// PARTIAL: when the call has one argument whose as_value type byte is 3 or 4 (the code range test is type - 3 <= 1; the
// type codes are not recovered), the
// original converts it with as_value::to_xstring, looks it up with Arrays::GetMemberIDByString<Arrays::Sounds>
// and, when the id is not -1, calls VoxSoundManager::PlayMusic(s_instance, id, 1, 0, 2000). Missing: the gameswf
// fn_call and as_value accessors, and VoxSoundManager::PlayMusic and s_instance. The body is left out.
void NativePlayMusic(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ae10 NativePlaySoundFX(gameswf::fn_call const&)
// PARTIAL: same argument handling as NativePlayMusic. The original calls VoxSoundManager::PlayMenu(s_instance, id, 0,
// 0, 0) when the id is not -1. Missing: fn_call and as_value accessors, PlayMenu and s_instance.
void NativePlaySoundFX(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ae9c NativeGoToMainMenu(gameswf::fn_call const&)
// asm: LDR R0 = &Singleton<Application>::s_inst; MOV R1,#0; B Application::GoToMainMenu. The event is 0 and the
// argument is ignored. The value of GoToMainMenuEvent 0 is not recovered (Application.hpp); it is passed as is.
void NativeGoToMainMenu(const gameswf::fn_call& call) {
    (void)call;
    ApplicationGetInstance().GoToMainMenu(static_cast<GoToMainMenuEvent>(0));
}

// 0x43aebc NativePushState(gameswf::fn_call const&)
// PARTIAL: when the call has one argument whose type byte is 3 or 4, its to_xstring is compared with "menu_CharacterMenu" and
// "menu_Ingame". The push runs when both comparisons differ, or when the state stack of Application +0x18
// (StateMachine m_stack: begin +0x0c, end +0x10, 8-byte StateInfo) is non-empty and its top state is
// &GSLevel::s_gsLevel. The push is then: GSFlashMenu::s_inst.m_pendingMenu (+0x0c) = MenuManager::GetMenuByName(name),
// and StateMachine::PushState(&GSFlashMenu::s_inst, false). GSLevel::s_gsLevel (ELF 0x9a25f4) is declared in
// dh2/GSLevel.hpp. Missing: the gameswf argument accessors, and a public accessor for StateMachine's m_stack, which is
// private. The body is left out.
void NativePushState(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43afb8 NativeHUDInteract(gameswf::fn_call const&)
// PARTIAL: the original reads argument 0 with as_value::to_xstring (no argument-count check) and dispatches it with
// DecideHudInteract: "quit" sets InExitConfirmation (ELF 0x9a5c18, a bool) to 1, "no" clears it to 0, and any other
// command tail-calls appDestroy. Missing: the gameswf argument accessors; an extern declaration of InExitConfirmation
// (no header declares it); and appDestroy (core-platform's ledger marks it skipped and no header declares it). The
// decision is in DecideHudInteract (tested). The body is left out.
void NativeHUDInteract(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43b04c NativeIsMultiplayerLoadCompleted(gameswf::fn_call const&)
// PARTIAL: the original sets the result to MultiplayerLoadCompleteFromFlags(online, AllLoadingDone, AllReadyToRoll),
// where online is COnline::isOnlineGame_ (+0x05, private, no getter). PlayerManager::AllLoadingDone and
// AllReadyToRoll are declared. Missing: COnline::isOnlineGame_ getter, the gameswf set_bool on *call.result, and the
// result path (as_value::set_bool is not in the tree). The body is left out.
void NativeIsMultiplayerLoadCompleted(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43b0b8 NativeMustWaitForHost(gameswf::fn_call const&)
// PARTIAL: the original sets the result to MustWaitForHostFromFlags(online, IsLocalPlayerHosting(), host word +0x1394
// (GetHostingPlayer()[1253]), host word +0x1414 (GetHostingPlayer()[1285]), PlayerManager +0x71a). Missing: the
// COnline::isOnlineGame_ getter; PlayerInfo (dh2/PlayerInfo.hpp is absent, so the words at +0x1394 and +0x1414 are not
// declared); and set_bool. The body is left out.
void NativeMustWaitForHost(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43b158 NativePopMenu(gameswf::fn_call const&)
// PARTIAL: with no argument the original calls the virtual at vtable +0x38 of the MultiMenuManager at MenuManager
// +0xf4 (PopMenu with no name). With an argument it converts it with to_xstring and calls MenuManager::PopMenu(name),
// which is declared. Missing: the gameswf argument count and accessors, and dh2/MultiMenuManager.hpp (absent). The
// body is left out.
void NativePopMenu(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43b1b4 NativePushMenu(gameswf::fn_call const&)
// PARTIAL: the original converts argument 0 with to_xstring (no argument-count check) and calls
// MenuManager::GetInstance()->PushMenu(name). PushMenu(const char*) is declared. Missing: the gameswf argument
// accessors and to_xstring. The body is left out.
void NativePushMenu(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43b1e4 NativeReviveAllPlayers(gameswf::fn_call const&)
// PARTIAL: the original reads GetCurrentLevel(). When it is non-null and Level::m_loadStep (+0x130) is 38, it branches
// on the online flag COnline::isOnlineGame_ (+0x05). Online: PlayerManager::unk_718 (+0x718) is set to 1. Offline:
// MenuManager (Application +0x54) PopMenu("menu_FadeToBlackScreen"), PushMenu("menu_FadeFromBlackScreen"), then
// PlayerManager::ReviveLocalPlayers(false). The level gate is written here. The branch needs the online flag, which
// has no getter, so the branch body is left out. GetCurrentLevel is still a stub in its own row (it returns nullptr,
// although dh2/GSLevel.hpp now declares GSLevel::s_level), so the level gate inherits that stub.
void NativeReviveAllPlayers(const gameswf::fn_call& call) {
    (void)call;
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (!level || level->m_loadStep != 38) {
        return;
    }
    // PARTIAL: the online and offline branches are left out (see above).
}

// 0x43b278 NativeSaveSettings(gameswf::fn_call const&)
// Reconstructed. The SavegameManager at Application +0x4c is read first. When its byte +0x37 (m_saveBlocked, public in
// dh2/SavegameManager.hpp) is set, the byte is cleared and MenuManager::ResetFonts() runs on MenuManager::GetInstance().
// Then saveSettings(), and setLanguage(getLanguage()). The argument is not read.
void NativeSaveSettings(const gameswf::fn_call& call) {
    (void)call;
    SavegameManager* savegame = ApplicationGetInstance().m_savegameManager;
    if (savegame->m_saveBlocked) {
        savegame->m_saveBlocked = false;
        MenuManager::GetInstance()->ResetFonts();
    }
    savegame->saveSettings();
    savegame->setLanguage(savegame->getLanguage());
}

// 0x43b2d8 NativeLoadSettings(gameswf::fn_call const&)
// PARTIAL: the original calls SavegameManager::loadSettings(sm, false) and UpdateSavedValues(). When
// VoxSoundManager::s_instance is non-null it calls SetInitialVolume(s, GetSavedOption("VolumeMusic"),
// GetSavedOption("VolumeFX"), 0.0). It ends with setLanguage(getLanguage()). The volume block is left out:
// VoxSoundManager::SetInitialVolume and s_instance are not declared in the tree.
void NativeLoadSettings(const gameswf::fn_call& call) {
    (void)call;
    SavegameManager* savegame = ApplicationGetInstance().m_savegameManager;
    savegame->loadSettings(false);
    UpdateSavedValues();
    // PARTIAL: the VoxSoundManager::SetInitialVolume block is left out, see above.
    savegame->setLanguage(savegame->getLanguage());
}

// 0x43b390 NativeOptionMusic(gameswf::fn_call const&)
// PARTIAL: the original converts argument 0 with as_value::to_number. When s_instance is non-null it calls
// VoxSoundManager::SetSoundVolume(s, 2, value) and then SavegameManager::setOption(sm, "VolumeMusic", (int)value).
// Missing: the gameswf to_number accessor, VoxSoundManager::SetSoundVolume and s_instance, and SavegameManager::setOption.
// The body is left out.
void NativeOptionMusic(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43b424 NativeOptionFX(gameswf::fn_call const&)
// PARTIAL: same as NativeOptionMusic, with channel 1 and the option name "VolumeFX". Missing: the same parts.
void NativeOptionFX(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43b4b8 NativeEnterOptionMenu(gameswf::fn_call const&)
// PARTIAL: the original calls VoxSoundManager::ResumeMusic(s_instance, 0) with no null check. ResumeMusic(int) and
// s_instance are not declared in the tree. The call is left out.
void NativeEnterOptionMenu(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43b4dc IsNonDebugQuest(Quest const*)
// asm: PUSH {R4,LR}; BL Quest::IsDebug; EOR R0,R0,#1; UXTB. The result is the logical negation of IsDebug.
bool IsNonDebugQuest(const Quest* quest) {
    return !quest->IsDebug();
}

// ---- decision helpers (not original symbols; see the header) ----

// asm of NativeHUDInteract: strcmp("quit", s) == 0 sets InExitConfirmation to 1; otherwise strcmp("no", s) == 0
// clears it to 0; otherwise appDestroy is tail-called.
HudInteractDecision DecideHudInteract(const char* command) {
    if (std::strcmp("quit", command) == 0) {
        return HudInteractDecision::kSetExitConfirmation;
    }
    if (std::strcmp("no", command) == 0) {
        return HudInteractDecision::kClearExitConfirmation;
    }
    return HudInteractDecision::kAppDestroy;
}

// NativeIsMultiplayerLoadCompleted: when online, both the loading check and the ready-to-roll check must pass.
bool MultiplayerLoadCompleteFromFlags(bool online, bool allLoadingDone, bool allReadyToRoll) {
    return !online || (allLoadingDone && allReadyToRoll);
}

// NativeMustWaitForHost: online && !IsLocalPlayerHosting && (!hostWord1394 || (localFlag71a && hostWord1394 &&
// hostWord1414)).
bool MustWaitForHostFromFlags(bool online, bool localPlayerHosting, bool hostWord1394, bool hostWord1414,
                              bool localFlag71a) {
    return online && !localPlayerHosting && (!hostWord1394 || (localFlag71a && hostWord1394 && hostWord1414));
}
