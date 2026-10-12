#include "net-session-online/c_matching_gl_live.hpp"

#include "net-session-online/game_center_info.hpp"
#include "net-session-online/glx_player_mp_lobby.hpp"

// Single definitions of the statics this slice owns. The other statics of the class
// (s_IsGameStarting, s_eventQueueGLLiveInternal, the observers) are defined by their own slice.
GLXPlayerMPLobby* CMatchingGLLive::s_GLXPlayerMPLobby = nullptr;
GameCenterInfo* CMatchingGLLive::s_GameCenterInfo = nullptr;

// 0x81ef7c _ZN15CMatchingGLLive19GetGLXPlayerMPLobbyEv
// Partial: the original creates the lobby on first use. That path needs the login observer
// accessor and the socket URL, which are not reconstructed. This returns the existing instance,
// which is what the original returns on its "already created" branch.
GLXPlayerMPLobby* CMatchingGLLive::GetGLXPlayerMPLobby() { return s_GLXPlayerMPLobby; }

// 0x81f128 _ZN15CMatchingGLLive9StartGameEv
void CMatchingGLLive::StartGame() {
    if (s_IsGameStarting) {
        return;
    }
    s_eventQueueGLLiveInternal.AddEvent(11, nullptr, 0);
    GetGLXPlayerMPLobby()->mpSendLaunchLobbyGame();
}

// 0x81f178 _ZN15CMatchingGLLive10SetGCStateEi
// Partial: state 1 (a CSignIn event) and state 3 (CMatching::CreateJoinRoom with a copy of the
// room attributes) do nothing here, because neither is reconstructed. State 2 chooses 3 or 4
// from the Game Center record, and state 7 starts the game.
void CMatchingGLLive::SetGCState(std::int32_t state) {
    if (!gameCenterActive_ || gcState_ == state) {
        return;
    }
    gcState_ = state;
    switch (state) {
        case 2:
            SetGCState(s_GameCenterInfo != nullptr ? 3 : 4);
            break;
        case 7:
            StartGame();
            break;
        default:
            break;
    }
}

// 0x81f44c _ZN15CMatchingGLLive19SendSetPlayerStatusEc
// Partial: the original returns the lobby call's result. mpSendSetPlayerStatus is void in
// glx_player_mp_lobby.hpp, so the port returns 0.
std::int32_t CMatchingGLLive::SendSetPlayerStatus(std::uint8_t status) {
    GetGLXPlayerMPLobby()->mpSendSetPlayerStatus(status);
    return 0;
}

// 0x81f464 _ZN15CMatchingGLLive9LeaveRoomEv
std::int32_t CMatchingGLLive::LeaveRoom() {
    GetGLXPlayerMPLobby()->mpSendLeaveLobby();
    roomOp_ = 5;
    return 0;
}

// 0x81f524 _ZN15CMatchingGLLive6IsHostEv
// Virtual slot 60 of GLXPlayerMPLobby is mpIsMaster (checked against the vtable dump).
bool CMatchingGLLive::IsHost() { return GetGLXPlayerMPLobby()->mpIsMaster(); }
