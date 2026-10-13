// Batch g1-game-app-global-f216b6-5 (<global>, part 6 of 22): the MenuBase name order, the dialog message list,
// IsPlayableClassID and the gameswf natives of the menus, HUD, zoom, device flags and online state.
// Declarations: dh2/global/app_helpers.hpp and dh2/global/native_app.hpp.
//
// Facts shared by the bodies (checked against the ARM assembly where the pseudocode passes an extra argument):
//   - Singleton<Application>::s_inst (0x99f72c) is ApplicationGetInstance(). Its +0x34 is m_stringManager,
//     +0x40 m_playerManager, +0x50 m_zoomHandler and +0x54 m_menuManager (dh2/Application.hpp).
//   - MenuManager::GetInstance(), HUDControls::GetInstance() and Device::IsHighPerformance() are static and take no
//     argument in the assembly. The pseudocode passes the native's fn_call pointer through r0 by accident.
//     MenuManager::GetHUDRoot() is a member call on the instance.
//   - Application::IsOnlineGame() (0x320668) is *(GetOnline() + 5), the byte that the natives read as GetOnline()+5.
//   - gameswf::as_value::set_bool is not in the tree, so every native that sets its result is PARTIAL.
//   - The natives return void (see dh2/global/native_app.hpp).
//   - The globals SHARP_DEVICES, HTC_DEVICES, NO_IGP and JAPANESE_BUILD are declared extern here. Their definitions
//     belong to the globals batch.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "core-platform/device.hpp"
#include "dh2/Application.hpp"
#include "dh2/DialogMsg.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/ZoomHandler.hpp"
#include "dh2/global/app_helpers.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "dh2/global/native_app.hpp"

// Globals of the original. The .bss addresses are HTC_DEVICES 0x9f6403, SHARP_DEVICES 0x9f6404, NO_IGP 0x9f6405
// and JAPANESE_BUILD 0x9f640c.
extern bool HTC_DEVICES;
extern bool SHARP_DEVICES;
extern bool NO_IGP;
extern bool JAPANESE_BUILD;

// 0x41f548 MenuBaseStar_sortlessthen(MenuBase const*, MenuBase const*)
// A null first operand returns false and a null second operand returns true. Otherwise the menu names
// (MenuFX::State::m_name, +0x08) are compared with strcmp.
bool MenuBaseStar_sortlessthen(const MenuBase* a, const MenuBase* b) {
    if (a == nullptr) {
        return false;
    }
    if (b == nullptr) {
        return true;
    }
    return std::strcmp(a->m_name, b->m_name) < 0;
}

// 0x433f04 GetDialogMsgListById(int, std::vector<DialogMsg, std::allocator<DialogMsg> >&)
// PARTIAL: not rebuilt. The function returns false on every path, so out stays unchanged. The original fills out and
// returns true for a valid dialog id. Missing:
//   - the id asserts ("dialogId < (int)Arrays::Dialogs::size" at line 126, "dialogId >= 0" at line 127, both in
//     MenuMessageManager.cpp) and the id bounds check. Arrays::Dialogs::size is declared in
//     pydata-accessors/w7_arrays27.hpp, which cannot share a translation unit with dh2/PlayerManager.hpp: both pull in an
//     IStreamBase stand-in and the two definitions conflict. Layout request: one IStreamBase for the engine stand-ins.
//   - the record Arrays::Dialogs::members[dialogId] (stepsSize at +4 of a 12-byte record, steps pointer at +8). The
//     element layout of w7_arrays27.hpp is undecided (host stride there), so the stepsSize > 0 check is missing.
//   - the step loop over the 20-byte steps (byte +4 actor, int +8 style, string id +12 text, string id +16 title).
//     It needs StringManager::getString(int) (Application +0x34, _ZNK13StringManager9getStringEi). dh2/StringManager.hpp
//     declares only getString(int, unsigned int packIdx), so the one-argument call is a declaration request. It also
//     needs ParsePlayerName (core-platform/w7_global_rem.hpp). Per step: the DialogMsg title is ParsePlayerName(getString(+16)), m_text is
//     ParsePlayerName(getString(+12)), m_style is the int at +8, and m_actorName becomes a one-character string made from
//     the signed byte at +4 (the asm has no SetActorName call). The message is then pushed onto out.
bool GetDialogMsgListById(int dialogId, std::vector<DialogMsg>& out) {
    (void)dialogId;
    (void)out;
    return false;
}

// 0x439c34 IsPlayableClassID(int)
bool IsPlayableClassID(int classId) {
    return classId == 263 || classId == 325 || classId == 290;
}

// 0x439c60 NativeSendReady(gameswf::fn_call const&)
// Empty (asm BX LR).
void NativeSendReady(const gameswf::fn_call& call) {
    (void)call;
}

// 0x439c64 NativeLaunchIGP(gameswf::fn_call const&)
// Empty (asm BX LR).
void NativeLaunchIGP(const gameswf::fn_call& call) {
    (void)call;
}

// 0x439c68 NativeSetLockZoom(bool)
// PARTIAL: the original stores the flag as one byte at +0x86 of the bound camera. The camera is ZoomHandler::m_camera
// (+0x20) of Singleton<Application>::s_inst.m_zoomHandler (+0x50, the global dword_99f77c). The store is missing
// because CameraLevel has no declaration in the tree (dh2/CameraLevel.hpp is absent). Layout request:
// "CameraLevel +0x86 byte, the zoom lock flag written by NativeSetLockZoom".
void NativeSetLockZoom(bool locked) {
    ZoomHandler* zoomHandler = ApplicationGetInstance().m_zoomHandler;
    CameraLevel* camera = zoomHandler != nullptr ? zoomHandler->m_camera : nullptr;
    if (camera != nullptr) {
        // Missing: *(camera + 0x86) = locked, see above.
    }
    (void)camera;
    (void)locked;
}

// 0x439c94 NativeUnlockZoom(gameswf::fn_call const&)
// PARTIAL: forwards to NativeSetLockZoom(false) (asm: MOV R0,#0; B NativeSetLockZoom). The target is partial.
void NativeUnlockZoom(const gameswf::fn_call& call) {
    (void)call;
    NativeSetLockZoom(false);
}

// 0x439c9c NativeLockZoom(gameswf::fn_call const&)
// PARTIAL: forwards to NativeSetLockZoom(true). The target is partial.
void NativeLockZoom(const gameswf::fn_call& call) {
    (void)call;
    NativeSetLockZoom(true);
}

// 0x439ca4 NativeTouchToMove(gameswf::fn_call const&)
// Empty (asm BX LR).
void NativeTouchToMove(const gameswf::fn_call& call) {
    (void)call;
}

// 0x439ca8 NativeIPodSetShuffle(gameswf::fn_call const&)
// Empty (asm BX LR).
void NativeIPodSetShuffle(const gameswf::fn_call& call) {
    (void)call;
}

// 0x439cac NativeGCInvite(gameswf::fn_call const&)
// Empty (asm BX LR).
void NativeGCInvite(const gameswf::fn_call& call) {
    (void)call;
}

// 0x439dd8 NativePopAllMenus(gameswf::fn_call const&)
// PARTIAL: the original calls the virtual at vtable +0x38 (PopMenu) of MenuManager::GetInstance()->m_multiMenuManager
// (+0xf4 of MenuManager) with argument 1, that is MultiMenuManager::PopMenu(true). The call is missing because
// MultiMenuManager is not declared (dh2/MultiMenuManager.hpp is absent). Layout request: "MultiMenuManager virtual
// PopMenu(bool) at vtable +0x38".
void NativePopAllMenus(const gameswf::fn_call& call) {
    (void)call;
}

// 0x439dfc NativeBeginCustom(gameswf::fn_call const&)
// PARTIAL: the original takes the HUD root (MenuManager::GetHUDRoot), reads the "HUDStyle" option, formats the HUD
// menu name "_root.menu_HUD_<style>" and then does RenderFX::Find(root, name),
// RenderFX::InvokeASCallback(root, character, "SetCustomText", nullptr, 0) and HUDControls::GetInstance()->BeginCustom().
// Only the first part is rebuilt. RenderFX::Find and RenderFX::InvokeASCallback are not declared in dh2/RenderFX.hpp.
// HUDControls::GetInstance() and BeginCustom() are declared in dh2/HUDControls.hpp, but they run after the callback, so
// that call waits for the two RenderFX methods.
void NativeBeginCustom(const gameswf::fn_call& call) {
    (void)call;
    MenuFX* hudRoot = MenuManager::GetInstance()->GetHUDRoot();
    const int hudStyle = ApplicationGetInstance().GetSavedOption("HUDStyle");
    char hudMenuName[20];
    std::snprintf(hudMenuName, sizeof hudMenuName, "_root.menu_HUD_%d", hudStyle);
    (void)hudRoot;
    (void)hudMenuName;
}

// 0x439ebc NativeIsBluetoothWifiEnabled(gameswf::fn_call const&)
// PARTIAL: the original sets the result to true. gameswf::as_value::set_bool is not in the tree.
void NativeIsBluetoothWifiEnabled(const gameswf::fn_call& call) {
    (void)call;
}

// 0x439ec8 NativeIsJapaneseVersion(gameswf::fn_call const&)
// PARTIAL: the original sets the result to the JAPANESE_BUILD byte (nonzero is true). set_bool is not in the tree.
void NativeIsJapaneseVersion(const gameswf::fn_call& call) {
    (void)call;
    const bool japaneseBuild = JAPANESE_BUILD;
    (void)japaneseBuild;
}

// 0x439f00 NativeEnableGameCenter(gameswf::fn_call const&)
// PARTIAL: the original sets the result to true. gameswf::as_value::set_bool is not in the tree.
void NativeEnableGameCenter(const gameswf::fn_call& call) {
    (void)call;
}

// 0x439f0c NativeBluetoothCheck(gameswf::fn_call const&)
// PARTIAL: the original sets the result to true. gameswf::as_value::set_bool is not in the tree.
void NativeBluetoothCheck(const gameswf::fn_call& call) {
    (void)call;
}

// 0x439f18 NativeIsMultiplayerEnabled(gameswf::fn_call const&)
// PARTIAL: the original sets the result to SHARP_DEVICES || HTC_DEVICES || (!NO_IGP && Device::IsHighPerformance()),
// tested in that order (the asm calls IsHighPerformance only when the first three are clear). set_bool is not in the tree.
void NativeIsMultiplayerEnabled(const gameswf::fn_call& call) {
    (void)call;
    const bool enabled = SHARP_DEVICES || HTC_DEVICES || (!NO_IGP && Device::IsHighPerformance());
    (void)enabled;
}

// 0x439fa4 NativeScreenIsBlack(gameswf::fn_call const&)
// PARTIAL: the only gate is Application::IsOnlineGame() (0x320668, the byte GetOnline()+5), and that function is stubbed
// to false in game-app/src/g1-game-app-Application-b291be-1.cpp. While the stub returns false, the body never reaches
// the store, so the native does nothing. The logic below matches the original: when the online game flag is set, the
// global death state of the player manager (Application +0x40, PlayerManager::m_globalDeathState at +0x714) moves from
// 1 to 2. The original returns GetOnline(), a leftover of the last call, so the native returns void.
void NativeScreenIsBlack(const gameswf::fn_call& call) {
    (void)call;
    Application& app = ApplicationGetInstance();
    if (app.IsOnlineGame()) {
        PlayerManager* playerManager = app.m_playerManager;
        if (playerManager->m_globalDeathState == 1) {
            playerManager->m_globalDeathState = 2;
        }
    }
}

// 0x439fe8 NativeIsMultiplayerGame(gameswf::fn_call const&)
// PARTIAL: the original sets the result to the online game flag, Application::IsOnlineGame() (GetOnline()+5).
// set_bool is not in the tree.
void NativeIsMultiplayerGame(const gameswf::fn_call& call) {
    (void)call;
    const bool onlineGame = ApplicationGetInstance().IsOnlineGame();
    (void)onlineGame;
}
