// Observers that CMatchingGLLive hands to the GLX player services: the friend
// observer (friend-list requests), the message observer (message requests) and the
// lobby observer (multiplayer lobby callbacks). Each one is a standalone polymorphic
// class here. The original vtables are checked against the callers, but the base
// interfaces are not reconstructed.
//
// Layouts (original 32-bit offsets, kept as comments):
//   CMatchingGLLiveFriendObserver: +0x00 vptr, +0x04 flag (byte), set when the
//     request code is 0x3d.
//   CMatchingGLLiveMessageObserver: +0x00 vptr only.
//   CMatchingGLLiveLobbyObserver: +0x00 vptr, +0x04 logged-in flag (byte),
//     +0x05 connected flag (byte), +0x06 game-started flag (byte), +0x14 state (int),
//     +0x48 last result (int).
#pragma once

#include <cstdint>

class DataPacket;

class CMatchingGLLiveFriendObserver {
public:
    CMatchingGLLiveFriendObserver();                                   // 0x821254 C2, 0x821280 C1
    virtual ~CMatchingGLLiveFriendObserver();                          // 0x8212b0 D1, 0x8212ac D2

    // 0x8212b4 _ZN29CMatchingGLLiveFriendObserver14OnNetworkErrorEv: no-op.
    virtual void OnNetworkError();
    // 0x8212b8 _ZN29CMatchingGLLiveFriendObserver16OnRequestTimeoutEi: no-op.
    virtual void OnRequestTimeout(std::int32_t requestId);
    // 0x8212bc _ZN29CMatchingGLLiveFriendObserver16OnRequestSuccessEiPci: sets the flag when code is 0x3d.
    virtual void OnRequestSuccess(std::int32_t code, const char* text, std::int32_t length);
    // 0x8212cc _ZN29CMatchingGLLiveFriendObserver16OnRequestFailureEii: no-op.
    virtual void OnRequestFailure(std::int32_t code, std::int32_t detail);

    bool FlagForTest() const { return flag_; }

private:
    bool flag_ = false;  // +0x04
};

class CMatchingGLLiveMessageObserver {
public:
    virtual ~CMatchingGLLiveMessageObserver() = default;

    // 0x8241d4 _ZN30CMatchingGLLiveMessageObserver14OnNetworkErrorEv: no-op.
    virtual void OnNetworkError();
    // 0x8241d8 _ZN30CMatchingGLLiveMessageObserver16OnRequestTimeoutEi: no-op.
    virtual void OnRequestTimeout(std::int32_t requestId);
    // 0x8241dc _ZN30CMatchingGLLiveMessageObserver16OnRequestFailureEii: no-op.
    virtual void OnRequestFailure(std::int32_t code, std::int32_t detail);
    // 0x8241e0 _ZN30CMatchingGLLiveMessageObserver16OnRequestSuccessEiPci.
    // Partial: for codes 0x6f and 0x70 the original sets a byte at +0x6c02 of the
    // CMatching singleton. That layout is not reconstructed, so the body is empty.
    virtual void OnRequestSuccess(std::int32_t code, const char* text, std::int32_t length);
};

class CMatchingGLLiveLobbyObserver {
public:
    virtual ~CMatchingGLLiveLobbyObserver() = default;  // D1/D2 queued

    // 0x8212f8 _ZN28CMatchingGLLiveLobbyObserver30OnMPEstablishConnectionSuccessEv
    virtual void OnMPEstablishConnectionSuccess();
    // 0x82130c _ZN28CMatchingGLLiveLobbyObserver16OnMPLoginSuccessEP10DataPacket
    virtual void OnMPLoginSuccess(DataPacket* packet);
    // 0x821328 _ZN28CMatchingGLLiveLobbyObserver24OnMPKickOutPlayerSuccessEv
    virtual void OnMPKickOutPlayerSuccess();
    // 0x821334 _ZN28CMatchingGLLiveLobbyObserver20OnMPStartGameSuccessEP10DataPacket
    virtual void OnMPStartGameSuccess(DataPacket* packet);
    // 0x82134c _ZN28CMatchingGLLiveLobbyObserver16OnMPGetLobbyInfoEP10DataPacket
    // Partial: the packet dump (PrintDatapacket, 0x8215c4 queued) is not called.
    virtual void OnMPGetLobbyInfo(DataPacket* packet);
    // 0x821368 _ZN28CMatchingGLLiveLobbyObserver19OnMPSetParamSuccessEi: no-op.
    virtual void OnMPSetParamSuccess(std::int32_t value);
    // 0x82136c _ZN28CMatchingGLLiveLobbyObserver31OnMPProcessPushSetGameParameterEP10DataPacket: returns 0.
    virtual std::int32_t OnMPProcessPushSetGameParameter(DataPacket* packet);
    // 0x821374 _ZN28CMatchingGLLiveLobbyObserver28OnMPProcessPushSetUserStatusEP10DataPacket: returns 0.
    virtual std::int32_t OnMPProcessPushSetUserStatus(DataPacket* packet);
    // 0x82137c _ZN28CMatchingGLLiveLobbyObserver31OnMPProcessPushGameserverParamsEP10DataPacket: returns 0.
    virtual std::int32_t OnMPProcessPushGameserverParams(DataPacket* packet);
    // 0x821384 _ZN28CMatchingGLLiveLobbyObserver28OnMPProcessPushAutomatchUserEP10DataPacket: returns 0.
    virtual std::int32_t OnMPProcessPushAutomatchUser(DataPacket* packet);
    // 0x821448 _ZN28CMatchingGLLiveLobbyObserver9TerminateEv
    // Partial: calls ClearRoomList (not reconstructed; see the .cpp).
    void Terminate();
    // 0x821470 _ZN28CMatchingGLLiveLobbyObserver21OnMPDisconnectSuccessEv
    // Partial: calls ClearRoomList (not reconstructed; see the .cpp).
    virtual void OnMPDisconnectSuccess();

    // Test seams. Not in the original.
    std::int32_t StateForTest() const { return state_; }
    bool LoggedInForTest() const { return loggedIn_; }
    bool ConnectedForTest() const { return connected_; }
    bool GameStartedForTest() const { return gameStarted_; }
    std::int32_t LastResultForTest() const { return lastResult_; }
    void SetFlagsForTest(bool loggedIn, bool connected, bool gameStarted) {
        loggedIn_ = loggedIn;
        connected_ = connected;
        gameStarted_ = gameStarted;
    }

private:
    bool loggedIn_ = false;      // +0x04
    bool connected_ = false;     // +0x05
    bool gameStarted_ = false;   // +0x06
    std::int32_t state_ = 0;     // +0x14
    std::int32_t lastResult_ = 0;  // +0x48
};
