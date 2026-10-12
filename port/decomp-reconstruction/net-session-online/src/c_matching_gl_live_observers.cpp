#include "net-session-online/c_matching_gl_live_observers.hpp"

// ---- CMatchingGLLiveFriendObserver ----

// 0x821254 _ZN29CMatchingGLLiveFriendObserverC2Ev (C1 at 0x821280 is the same body)
CMatchingGLLiveFriendObserver::CMatchingGLLiveFriendObserver() = default;

// 0x8212ac _ZN29CMatchingGLLiveFriendObserverD2Ev (D1 at 0x8212b0 is the same empty body)
CMatchingGLLiveFriendObserver::~CMatchingGLLiveFriendObserver() = default;

// 0x8212b4 _ZN29CMatchingGLLiveFriendObserver14OnNetworkErrorEv
void CMatchingGLLiveFriendObserver::OnNetworkError() {}

// 0x8212b8 _ZN29CMatchingGLLiveFriendObserver16OnRequestTimeoutEi
void CMatchingGLLiveFriendObserver::OnRequestTimeout(std::int32_t /*requestId*/) {}

// 0x8212bc _ZN29CMatchingGLLiveFriendObserver16OnRequestSuccessEiPci
void CMatchingGLLiveFriendObserver::OnRequestSuccess(std::int32_t code, const char* /*text*/,
                                                    std::int32_t /*length*/) {
    if (code == 0x3d) {
        flag_ = true;
    }
}

// 0x8212cc _ZN29CMatchingGLLiveFriendObserver16OnRequestFailureEii
void CMatchingGLLiveFriendObserver::OnRequestFailure(std::int32_t /*code*/, std::int32_t /*detail*/) {}

// ---- CMatchingGLLiveMessageObserver ----

// 0x8241d4 _ZN30CMatchingGLLiveMessageObserver14OnNetworkErrorEv
void CMatchingGLLiveMessageObserver::OnNetworkError() {}

// 0x8241d8 _ZN30CMatchingGLLiveMessageObserver16OnRequestTimeoutEi
void CMatchingGLLiveMessageObserver::OnRequestTimeout(std::int32_t /*requestId*/) {}

// 0x8241dc _ZN30CMatchingGLLiveMessageObserver16OnRequestFailureEii
void CMatchingGLLiveMessageObserver::OnRequestFailure(std::int32_t /*code*/, std::int32_t /*detail*/) {}

// 0x8241e0 _ZN30CMatchingGLLiveMessageObserver16OnRequestSuccessEiPci
// Partial: see the header. The write to the CMatching singleton is not reconstructed.
void CMatchingGLLiveMessageObserver::OnRequestSuccess(std::int32_t /*code*/, const char* /*text*/,
                                                     std::int32_t /*length*/) {}

// ---- CMatchingGLLiveLobbyObserver ----

// 0x8212f8 _ZN28CMatchingGLLiveLobbyObserver30OnMPEstablishConnectionSuccessEv
void CMatchingGLLiveLobbyObserver::OnMPEstablishConnectionSuccess() {
    lastResult_ = 0;
    connected_ = true;
}

// 0x82130c _ZN28CMatchingGLLiveLobbyObserver16OnMPLoginSuccessEP10DataPacket
void CMatchingGLLiveLobbyObserver::OnMPLoginSuccess(DataPacket* /*packet*/) {
    lastResult_ = 0;
    loggedIn_ = true;
    state_ = 3;
}

// 0x821328 _ZN28CMatchingGLLiveLobbyObserver24OnMPKickOutPlayerSuccessEv
void CMatchingGLLiveLobbyObserver::OnMPKickOutPlayerSuccess() { lastResult_ = 0; }

// 0x821334 _ZN28CMatchingGLLiveLobbyObserver20OnMPStartGameSuccessEP10DataPacket
void CMatchingGLLiveLobbyObserver::OnMPStartGameSuccess(DataPacket* /*packet*/) {
    lastResult_ = 0;
    state_ = 8;
    gameStarted_ = false;
}

// 0x82134c _ZN28CMatchingGLLiveLobbyObserver16OnMPGetLobbyInfoEP10DataPacket
// Partial: the original prints the packet (PrintDatapacket, not reconstructed) when the
// game-started flag is set.
void CMatchingGLLiveLobbyObserver::OnMPGetLobbyInfo(DataPacket* /*packet*/) {
    if (!gameStarted_) {
        return;
    }
    state_ = 6;
}

// 0x821368 _ZN28CMatchingGLLiveLobbyObserver19OnMPSetParamSuccessEi
void CMatchingGLLiveLobbyObserver::OnMPSetParamSuccess(std::int32_t /*value*/) {}

// 0x82136c _ZN28CMatchingGLLiveLobbyObserver31OnMPProcessPushSetGameParameterEP10DataPacket
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushSetGameParameter(DataPacket* /*packet*/) {
    return 0;
}

// 0x821374 _ZN28CMatchingGLLiveLobbyObserver28OnMPProcessPushSetUserStatusEP10DataPacket
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushSetUserStatus(DataPacket* /*packet*/) {
    return 0;
}

// 0x82137c _ZN28CMatchingGLLiveLobbyObserver31OnMPProcessPushGameserverParamsEP10DataPacket
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushGameserverParams(DataPacket* /*packet*/) {
    return 0;
}

// 0x821384 _ZN28CMatchingGLLiveLobbyObserver28OnMPProcessPushAutomatchUserEP10DataPacket
std::int32_t CMatchingGLLiveLobbyObserver::OnMPProcessPushAutomatchUser(DataPacket* /*packet*/) {
    return 0;
}

// The room list (ClearRoomList, 0x8213ec) is not reconstructed. The list is not modelled
// here, so there is nothing to clear and this is a no-op until it is.
static void ClearRoomList() {}

// 0x821448 _ZN28CMatchingGLLiveLobbyObserver9TerminateEv
void CMatchingGLLiveLobbyObserver::Terminate() {
    ClearRoomList();
    state_ = 1;
    lastResult_ = 0;
    connected_ = false;
    loggedIn_ = false;
}

// 0x821470 _ZN28CMatchingGLLiveLobbyObserver21OnMPDisconnectSuccessEv
void CMatchingGLLiveLobbyObserver::OnMPDisconnectSuccess() {
    ClearRoomList();
    state_ = 1;
    lastResult_ = 0;
    connected_ = false;
    loggedIn_ = false;
}
