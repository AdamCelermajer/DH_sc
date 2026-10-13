// Batch g1-game-app-global-f216b6-6 (<global>, part 7 of 22): the gameswf natives of the online, Bluetooth,
// Wi-Fi, friends, leaderboard and Game Center menus, ELF 0x43a004 .. 0x43a6c4. Declarations:
// dh2/global/native_online_gc.hpp.
//
// Facts shared by the bodies:
//   - Application singleton (ApplicationGetInstance(), ELF Singleton<Application>::s_inst at 0x99f72c):
//     +0x40 m_playerManager, +0x4c m_savegameManager, +0x54 m_menuManager (dh2/Application.hpp).
//   - OnlineSingleton<OnlineGameState>::GetInstance() is the online game state. Its fields +0x14 (state
//     machine), +0x26, +0x28, +0x30 and +0x34 are not reachable from the tree yet (see the rows).
//   - GetOnline() (0x7fd794) is a thunk to COnline::GetInstance(). COnline::isOnlineGame_ (+0x05) is private
//     and has no getter.
//   - The gameswf value API (as_value::set_bool, to_string, to_tu_string, to_bool) and the fn_call argument
//     accessors are not in the tree. A native that writes its result or reads an argument is PARTIAL and its
//     body keeps only the part that does not depend on them.
//   - The natives return void. gameswf calls them as void(const fn_call&); the Hex-Rays int return is the
//     leftover r0 of the last call.

#include "core-platform/debug_out.hpp"
#include "core-platform/device.hpp"
#include "dh2/Application.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "dh2/global/native_app_hud_level.hpp"  // nativeOpenGLive (0x532b20)
#include "dh2/global/native_online_gc.hpp"
#include "net-session-online/online_game_state.hpp"
#include "net-session-online/tb/online_singleton.hpp"

// 0x43a004 NativeIsBluetooth(gameswf::fn_call const&)
// PARTIAL: the original sets the boolean result to (GetOnline()->isOnlineGame_ && OnlineGameState +0x34 == 2).
// Missing: COnline::isOnlineGame_ (+0x05) has no getter, OnlineGameState +0x34 is not declared, and the
// gameswf set_bool is not in the tree.
void NativeIsBluetooth(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a04c NativeIsOnlineMode(gameswf::fn_call const&)
// PARTIAL: the original sets the result to (OnlineGameState +0x28 byte && GetOnline()->isOnlineGame_). Missing:
// OnlineGameState +0x28 is not declared, COnline::isOnlineGame_ has no getter, and set_bool is not in the tree.
void NativeIsOnlineMode(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a094 NativeIsGLLiveMultiplayer(gameswf::fn_call const&)
// PARTIAL: the original sets the result to (OnlineGameState +0x34 == 3). Missing: OnlineGameState +0x34 is not
// declared and set_bool is not in the tree.
void NativeIsGLLiveMultiplayer(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a0cc NativeSendRoomSearchRequest(gameswf::fn_call const&)
// PARTIAL: the original writes 102 to OnlineGameState +0x14, the current state of its MultiplayerStateMachine
// (the state is the first field, state_). The state machine has no public setter (SetStateForTest is a test
// seam only) and OnlineGameState::stateMachine_ is private, so the write is left out.
void NativeSendRoomSearchRequest(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a0e0 NativeGLXPlayerLoggedIn(gameswf::fn_call const&)
// PARTIAL: the original sets the result to the byte at OnlineGameState +0x26. Missing: +0x26 is not declared in
// online_game_state.hpp, and set_bool is not in the tree.
void NativeGLXPlayerLoggedIn(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a0fc NativeOnlineSanityCheck(gameswf::fn_call const&)
// PARTIAL: the original clears the byte at OnlineGameState +0x26 (the GLX logged-in flag that
// NativeGLXPlayerLoggedIn reads). +0x26 is not declared, so the write is left out.
void NativeOnlineSanityCheck(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a110 NativeCreateMultiplayerLobby(gameswf::fn_call const&)
// PARTIAL: the original writes 101 to OnlineGameState +0x14, the current state of its MultiplayerStateMachine.
// No public state setter exists and stateMachine_ is private, so the write is left out (see
// NativeSendRoomSearchRequest).
void NativeCreateMultiplayerLobby(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a124 NativeSetMultitouch(gameswf::fn_call const&)
// PARTIAL: the original reads the argument count (fn_call +0x10) and, when it is 1, stores to_bool(argument 0)
// in MenuManager::unk_110 (+0x110); otherwise it stores 1. The fn_call members and to_bool are not in the tree,
// so the body is left out. MenuManager +0x110 is declared (unk_110) and can take the store once the argument
// accessors exist.
void NativeSetMultitouch(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a16c NativeIsKorean(gameswf::fn_call const&)
// PARTIAL: the original sets the result to (SavegameManager::getOption(Application +0x4c, "Language") == 5), 5
// being the Korean language index. dh2/SavegameManager.hpp does not exist yet, and set_bool is not in the tree.
void NativeIsKorean(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a1c8 NativeEnableBluetooth(gameswf::fn_call const&)
// PARTIAL: the original sets the result to Device::IsBluetoothSupported(). The value is computed here; the
// gameswf set_bool is not in the tree, so it is not written to the call. (The Hex-Rays call passes a spurious
// argument, the function is static with no parameters.)
void NativeEnableBluetooth(const gameswf::fn_call& call) {
    (void)call;
    const bool supported = Device::IsBluetoothSupported();
    (void)supported;
}

// 0x43a1e4 NativeAddFriend(gameswf::fn_call const&)
// PARTIAL: when GetOnline()->isOnlineGame_ is set and OnlineGameState +0x34 == 3, the original takes argument 0
// as a tu_string (gameswf to_tu_string), gets the CMatchingGLLive, and sends a GL friend invite to the name. The
// name is the long-string pointer at +0x0c when the first byte is -1, otherwise the inline text at +1.
// Missing: the COnline flag (COnline::isOnlineGame_ is private with no getter; Application::IsOnlineGame is a stub),
// OnlineGameState +0x34 (not declared), and the argument accessors (fn_call is incomplete in the tree, so argument 0
// cannot be read). CMatching::Get() is static with no argument, and CMatchingGLLive derives from CMatching, so the
// cast to CMatchingGLLive is available. CMatchingGLLive::SendGLFriendInvite(const char*) is public in
// net-session-online/c_matching_gl_live.hpp.
void NativeAddFriend(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a248 NativeSendGLInvite(gameswf::fn_call const&)
// PARTIAL: same gating and name handling as NativeAddFriend, but the send is
// CMatchingGLLive::SendGLFriendsGameInvite(char*). Missing: the same as NativeAddFriend.
void NativeSendGLInvite(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a2cc NativeIsMultiplayerHost(gameswf::fn_call const&)
// PARTIAL: the original sets the result to 1 when GetOnline()->isOnlineGame_ is clear. Otherwise, when
// OnlineGameState +0x34 is 3 or 4, it is CMatchingGLLive::IsHost() and, for any other value,
// PlayerManager::IsLocalPlayerHosting() on Application +0x40. Missing: the COnline flag getter, OnlineGameState
// +0x34, the CMatchingGLLive accessor, and set_bool.
void NativeIsMultiplayerHost(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a358 NativeWiFiCheck(gameswf::fn_call const&)
// PARTIAL: the original sets the result to Application::IsWifiEnabled() on the singleton. The value is computed
// here; the gameswf set_bool is not in the tree, so it is not written to the call.
void NativeWiFiCheck(const gameswf::fn_call& call) {
    (void)call;
    const bool wifi = ApplicationGetInstance().IsWifiEnabled();
    (void)wifi;
}

// 0x43a38c NativeGCAutoMatch(gameswf::fn_call const&)
// Calls OnlineGameState::Init(4) on the online game state. The original returns the Init result, which the
// void native drops. OnlineGameState::Init is PARTIAL in its own row (its matching switch and the Game Center
// branches are not reconstructed), so this call inherits that gap.
void NativeGCAutoMatch(const gameswf::fn_call& call) {
    (void)call;
    OnlineSingleton<OnlineGameState>::GetInstance()->Init(4);
}

// 0x43a3a0 NativeGCLeaderboards(gameswf::fn_call const&)
// PARTIAL: the original converts argument 0 with gameswf::as_value::to_string and discards it, then returns
// Application::IsWifiEnabled(). The to_string call is left out because the argument accessors are not in the
// tree. The Wi-Fi result is evaluated here and discarded (the void native has no result slot).
void NativeGCLeaderboards(const gameswf::fn_call& call) {
    (void)call;
    (void)ApplicationGetInstance().IsWifiEnabled();
}

// 0x43a3dc NativeMultiplayerBack(gameswf::fn_call const&)
// PARTIAL: the original stores to_string(argument 0) at OnlineGameState +0x30, then calls every handler of the
// online handler list whose key is 15 (the back button), passing the handler context at +0x10, in list order.
// Missing: to_string, the +0x30 field, and the handler list and runHandlers(15), which are private in
// online_game_state.hpp.
void NativeMultiplayerBack(const gameswf::fn_call& call) {
    (void)call;
}

// 0x43a450 NativeShowLeaderboard(gameswf::fn_call const&)
// PARTIAL: the original traces the call, then, when the Wi-Fi check passes, reads the command string (argument
// 0, gameswf to_string) and runs init, next, prev or my_rank. The branches read the GLXPlayerLeaderboard at
// CMatching::Get() + 0x671c (decimal 26396): that is CMatchingGLLive::glxLeaderboard_, a private member, so the branches
// are not reachable. The .bss globals leaderboardcount and leader are declared in dh2/global/Globals.hpp. The argument
// (to_string) needs the complete fn_call, which the tree does not have. Only the trace and the Wi-Fi guard are written.
void NativeShowLeaderboard(const gameswf::fn_call& call) {
    (void)call;
    _DEBUG_OUT("XSocket::Run(): Connecting... \n");
    if (!ApplicationGetInstance().IsWifiEnabled()) {
        return;
    }
    // Command dispatch on the string argument: not reconstructed (see the note above).
}

// 0x43a6a4 NativeLaunchTwitter(gameswf::fn_call const&)
// The original returns SavegameManager::getLanguage() of the savegame manager (Application +0x4c, the global
// dword_99F778). It does not launch Twitter; the name is the only evidence for that. getLanguage() is const and has no
// side effect. A native returns void, so the int result is dropped, as in the other natives.
void NativeLaunchTwitter(const gameswf::fn_call& call) {
    (void)call;
    (void)ApplicationGetInstance().m_savegameManager->getLanguage();
}

// 0x43a6c4 NativeGCAchievements(gameswf::fn_call const&)
// PARTIAL: the original converts argument 0 with to_string and discards it. Argument 0 is not read here, because
// gameswf::fn_call has no definition in the tree (no arg() accessor). The rest is written: the language is
// SavegameManager::getLanguage() on Application +0x4c, and nativeOpenGLive(region) is called with
// region = NativeGCAchievementsOpenGLiveId(language) (dh2/global/native_online_gc.hpp, tested). nativeOpenGLive
// (0x532b20) is the Android JNI bridge, declared in dh2/global/native_app_hud_level.hpp and not defined in the tree.
void NativeGCAchievements(const gameswf::fn_call& call) {
    (void)call;
    const int language = ApplicationGetInstance().m_savegameManager->getLanguage();
    nativeOpenGLive(NativeGCAchievementsOpenGLiveId(language));
}
