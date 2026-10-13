// Batch g1-game-app-global-f216b6-7 (<global>, part 8 of 22): gameswf natives 0x43a758 .. 0x43ac90 for the HUD,
// minimap, on-screen keyboard, zoom, orientation, iPod, save slot, level pause, menu pop and script skip.
// Declarations: dh2/global/native_app_hud_level.hpp. The natives return void: gameswf ignores the r0 left by the
// original, so no value is returned.
//
// Globals of the batch (Application is the singleton, Singleton<Application>::s_inst at 0x99f72c, ApplicationGetInstance()):
//   dword_99F73C = +0x10 m_device       dword_99F744 = +0x18 m_stateMachine   dword_99F76C = +0x40 m_playerManager
//   dword_99F778 = +0x4c m_savegameManager   dword_99F77C = +0x50 m_zoomHandler   dword_99F780 = +0x54 m_menuManager
//   byte_9A24D4  = GSKeyboard::s_inst +0x04 (GSKeyboard::m_querying)
//
// The as_value accessors of gameswf are not declared in this tree, so every native that reads an argument or stores a
// result is PARTIAL and keeps its body empty; the part that does not depend on them is written where it exists.
#include "dh2/global/native_app_hud_level.hpp"

#include "dh2/global/ctrl_camera.hpp"  // ApplicationGetInstance()
#include "dh2/Application.hpp"
#include "dh2/GSKeyboard.hpp"
#include "dh2/Level.hpp"
#include "dh2/OnScreenKeyboard.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/ZoomHandler.hpp"

// 0x43a758 NativeGLLive(gameswf::fn_call const&)
// Reconstructed. The language is SavegameManager::getLanguage() on Application::m_savegameManager, and the native
// passes GLLiveLanguageArg(language) to nativeOpenGLive. nativeOpenGLive is the Android JNI bridge (core-platform row
// 0x532b20), not defined in the tree yet.
void NativeGLLive(const gameswf::fn_call& /*call*/) {
    const int language = ApplicationGetInstance().m_savegameManager->getLanguage();
    nativeOpenGLive(GLLiveLanguageArg(language));
}

// 0x43a810 NativeRefreshHudManager(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original calls InfoHUDManager::GetInstance() (static), clears its m_cachesReady flag (+0x04),
// then calls HUDControls::GetInstance() (static) and HUDControls::Touch(). dh2/HUDControls.hpp and dh2/ZoomHandler.hpp
// cannot share a translation unit: both declare glitch::IEventReceiver (glitch-core-io deps/glitch_core_objects.hpp and
// glitch-scene-collada-gui deps/event_stand_ins.hpp), and the second definition does not compile. Integrator: keep one
// glitch::IEventReceiver. This batch's natives that need ZoomHandler (NativeResetZoom) are unaffected.
void NativeRefreshHudManager(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a82c NativeDoWeHaveInternet(gameswf::fn_call const&)
// PARTIAL: the value is Application::IsInternetAccessEnabled(), computed here. Missing: gameswf::as_value::set_bool on the
// result (fn_call +0x00), which needs the as_value declaration.
void NativeDoWeHaveInternet(const gameswf::fn_call& call) {
    const bool internet = ApplicationGetInstance().IsInternetAccessEnabled();
    (void)internet;
    (void)call;
}

// 0x43a860 NativeCloseKeyboard(gameswf::fn_call const&)
// Reconstructed. While GSKeyboard::s_inst.m_querying is set (byte_9A24D4), the keyboard is released and the state
// machine (Application::m_stateMachine, dword_99F744) is updated with dt 0.0. The return value is not part of the
// contract. ReleaseKeyboard is static in dh2/OnScreenKeyboard.hpp. The receiver GSKeyboard::s_inst is defined by the
// GSKeyboard batch.
void NativeCloseKeyboard(const gameswf::fn_call& /*call*/) {
    if (GSKeyboard::s_inst.m_querying) {
        OnScreenKeyboard::ReleaseKeyboard();
        ApplicationGetInstance().m_stateMachine->Update(0.0);
    }
}

// 0x43a8b0 NativeUpdateOrientation(gameswf::fn_call const&)
// Reconstructed. ResetOrientation(GetSavedOption("AutoOrientation")) on the Application singleton. Application::ResetOrientation
// is an empty body in the Application batch (0x31f748).
void NativeUpdateOrientation(const gameswf::fn_call& /*call*/) {
    Application& app = ApplicationGetInstance();
    const int autoOrientation = app.GetSavedOption("AutoOrientation");
    app.ResetOrientation(autoOrientation);
}

// 0x43a930 NativeIPodControl(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original converts argument 0 with gameswf::as_value::to_string and calls
// VoxSoundManager::IPodControl(const char*) on VoxSoundManager::s_instance. The as_value accessors and those vox-audio
// declarations are not in the tree (dh2 has no VoxSoundManager declaration of them).
void NativeIPodControl(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a974 NativeUseIpodPlayer(gameswf::fn_call const&)
// PARTIAL: the value is nativeIsSupportMM() == 1, computed here (the JNI bridge is declared in the header). Missing:
// gameswf::as_value::set_bool on the result (fn_call +0x00).
void NativeUseIpodPlayer(const gameswf::fn_call& call) {
    const bool supported = nativeIsSupportMM() == 1;
    (void)supported;
    (void)call;
}

// 0x43a9a8 NativeResetZoom(gameswf::fn_call const&)
// Reconstructed. ZoomHandler::ResetZoom() on Application::m_zoomHandler (+0x50, dword_99F77C).
void NativeResetZoom(const gameswf::fn_call& /*call*/) {
    ApplicationGetInstance().m_zoomHandler->ResetZoom();
}

// 0x43aa18 NativeShowStatusBar(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original converts argument 0 with gameswf::as_value::to_bool and calls
// Application::ShowStatubBar(bool), declared in dh2/Application.hpp. The as_value accessors are not in the tree.
void NativeShowStatusBar(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43aa58 NativeShowMinimapLegend(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original converts argument 0 with gameswf::as_value::to_bool, then stores it at byte
// +0x1e4 (484) of MenuCharMenu_Map::GetInstance(). dh2/MenuCharMenu_Map.hpp does not exist yet.
void NativeShowMinimapLegend(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43aa84 NativeResetMapZoom(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original calls MenuCharMenu_Map::ResetZoom() on MenuCharMenu_Map::GetInstance(). Waits on
// dh2/MenuCharMenu_Map.hpp, which does not exist yet.
void NativeResetMapZoom(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43aa94 NativeHideMinimap(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original calls HUDMinimap::GetInstance() (static), then the virtual at vtable +0x10, which
// tools/decomp_vtable.py names HUDMinimap::Hide(). Waits on dh2/HUDMinimap.hpp, which does not exist yet.
void NativeHideMinimap(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43aaac NativeShowMinimap(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original calls HUDMinimap::GetInstance() (static), then the virtual at vtable +0x0c, which
// tools/decomp_vtable.py names HUDMinimap::Show(). Waits on dh2/HUDMinimap.hpp, which does not exist yet.
void NativeShowMinimap(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43aac4 AssignSaveSlotToPlayer(int,int)
// PARTIAL: the original stores slot in SavegameManager::m_lastSlot (+0x08, dword_99F778), gets the local player with
// PlayerManager::GetLocalPlayer(localPlayerId, false) (dword_99F76C), then stores slot in the record at result[1].unknown_000.
// Both stores are missing: m_lastSlot is private in dh2/SavegameManager.hpp with no setter, and the layout of the record
// (Hex-Rays DH2_PlayerInfoCharacterPrefix, element 1 after the returned player) is not recovered.
// The return type is PlayerInfo*, as GetLocalPlayer returns it.
PlayerInfo* AssignSaveSlotToPlayer(int slot, int localPlayerId) {
    (void)slot;
    return ApplicationGetInstance().m_playerManager->GetLocalPlayer(localPlayerId, false);
}

// 0x43ab00 NativeEndLoading(gameswf::fn_call const&)
// Reconstructed. When a current level exists and its m_loadStep (+0x130) is 36, the step is advanced to 37.
// Depends on Application::GetCurrentLevel(), which is a partial stub in the Application batch (returns null).
void NativeEndLoading(const gameswf::fn_call& /*call*/) {
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level != nullptr && level->m_loadStep == 36) {
        ++level->m_loadStep;
    }
}

// 0x43ab40 NativeSetLevelPause(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original reads argument 0 only when call.nargs (fn_call +0x10) is 1, converts it with
// gameswf::as_value::to_bool, and stores it at Level +0x1a8 (424, Level::unk_1a8) when GetCurrentLevel() is non-null.
// The nargs test and the to_bool conversion are missing.
void NativeSetLevelPause(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ab98 NativeAwayFromHud(gameswf::fn_call const&)
// PARTIAL: when a current level exists, its in-game view flag (Level +0x198, m_isInGameView) is cleared and the zoom
// finger map is reset. The original then returns MultiMenuManager::ResetUpdateCursor(MenuManager::m_multiMenuManager,
// +0xf4 of dword_99F780). dh2/MultiMenuManager.hpp does not exist yet, so that call is missing.
void NativeAwayFromHud(const gameswf::fn_call& /*call*/) {
    Application& app = ApplicationGetInstance();
    Level* level = app.GetCurrentLevel();
    if (level != nullptr) {
        level->m_isInGameView = false;
        app.m_zoomHandler->ResetFingerMap();
        // Missing: MultiMenuManager::ResetUpdateCursor(app.m_menuManager->m_multiMenuManager).
    }
}

// 0x43abe8 NativeDebugConsole(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original calls the virtual at vtable +0x38 on MenuManager::m_multiMenuManager (+0xf4) with
// argument 0, takes the result as a GSConsole*, and calls GSConsole::Toggle(). tools/decomp_vtable.py names +0x38
// MultiMenuManager::PopMenu(bool), which does not fit a GSConsole* result, so the slot is not confirmed. Waits on
// dh2/MultiMenuManager.hpp.
void NativeDebugConsole(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ac28 NativePopAllAbove(gameswf::fn_call const&)
// PARTIAL: stub, no body. The original takes argument 0 only when its type byte (+0x01) is 3 or 4, converts it with
// gameswf::as_value::to_xstring, then calls the virtual at vtable +0x3c on MenuManager::m_multiMenuManager with
// (name, true), which tools/decomp_vtable.py names MultiMenuManager::PopMenu(char const*, bool). The as_value accessors
// and dh2/MultiMenuManager.hpp are not in the tree.
void NativePopAllAbove(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43ac90 NativeSkipScript(gameswf::fn_call const&)
// Reconstructed. ScriptManager::SkipScript(-2, false) on ScriptManager::s_inst, declared in dh2/ScriptManager.hpp by
// this batch. The definition of s_inst belongs to the ScriptManager batch.
void NativeSkipScript(const gameswf::fn_call& /*call*/) {
    ScriptManager::s_inst.SkipScript(-2, false);
}
