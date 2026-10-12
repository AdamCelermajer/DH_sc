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
#include <string>
#include <vector>

#include "net-session-online/t_member_info.hpp"

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
    // 0x822538 _ZN28CMatchingGLLiveLobbyObserverC2Ev. Partial: the room list, member vector and
    // name map (+0x18, +0x24, +0x30) are not modelled; the scalars are initialised.
    CMatchingGLLiveLobbyObserver();
    // 0x821ed8 _ZN28CMatchingGLLiveLobbyObserverD1Ev. Partial: runs Terminate; the containers
    // are not modelled. The D2 twin at 0x821f7c has the same body.
    virtual ~CMatchingGLLiveLobbyObserver();

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
    // 0x821560 _ZN28CMatchingGLLiveLobbyObserver24OnMPProcessPushStartGameEP10DataPacket: returns 0.
    virtual std::int32_t OnMPProcessPushStartGame(DataPacket* packet);
    // 0x821654 _ZN28CMatchingGLLiveLobbyObserver16OnMPNetworkErrorEi: records the error, clears the
    // flags and queues event 12 for error -2.
    virtual void OnMPNetworkError(std::int32_t error);
    // 0x8215a4 _ZN28CMatchingGLLiveLobbyObserver17OnMPFunctionErrorEPci: maps the error code to
    // a queued event. The decompiled return value (an AddEvent result) is not used, so void.
    virtual void OnMPFunctionError(char* message, std::int32_t code);
    // 0x821448 _ZN28CMatchingGLLiveLobbyObserver9TerminateEv
    // Partial: calls ClearRoomList (not reconstructed; see the .cpp).
    void Terminate();
    // 0x821470 _ZN28CMatchingGLLiveLobbyObserver21OnMPDisconnectSuccessEv
    // Thunk: forwards to Terminate (0x821448).
    virtual void OnMPDisconnectSuccess();

    // 0x8214cc _ZN28CMatchingGLLiveLobbyObserver18GetMemberIndexByNameEPKc: first entry whose
    // nickname matches, or -1.
    std::int32_t GetMemberIndexByName(const char* name) const;
    // 0x822324 _ZN28CMatchingGLLiveLobbyObserver19GetMemberByMemberIdEi: first entry with this
    // member id. Otherwise a static sentinel with member id -1 (the original's invalidMember).
    tMemberInfo* GetMemberByMemberId(std::int32_t memberId);
    // 0x821bac _ZN28CMatchingGLLiveLobbyObserver18GetMemberIdVectorEv: the member ids, in order.
    std::vector<std::int32_t> GetMemberIdVector() const;
    // 0x822638 _ZN28CMatchingGLLiveLobbyObserver14SetPlayerParamESs. Partial: the aliasing
    // guard of the original (destination equal to the source) is not reproduced; the decompiled
    // return value is not used, so this is void.
    void SetPlayerParam(const std::string& value);
    // 0x822950 _ZN28CMatchingGLLiveLobbyObserver18CollapseMemberListEv: keeps only entries with
    // the active flag set.
    void CollapseMemberList();
    // 0x822b78 _ZN28CMatchingGLLiveLobbyObserver21OnMPLeaveSessionSuccessEv: clears the game-started
    // flag, both index fields (-1), the member list and the last result.
    std::int32_t OnMPLeaveSessionSuccess();

    // Test seams. Not in the original.
    std::int32_t StateForTest() const { return state_; }
    bool LoggedInForTest() const { return loggedIn_; }
    bool ConnectedForTest() const { return connected_; }
    bool GameStartedForTest() const { return gameStarted_; }
    std::int32_t LastResultForTest() const { return lastResult_; }
    std::size_t MemberCountForTest() const { return members_.size(); }
    tMemberInfo& MemberForTest(std::size_t index) { return members_[index]; }
    void SetMemberIndexForTest(std::int32_t index) { field0c_ = index; }
    void SetFlagsForTest(bool loggedIn, bool connected, bool gameStarted) {
        loggedIn_ = loggedIn;
        connected_ = connected;
        gameStarted_ = gameStarted;
    }

private:
    bool loggedIn_ = false;        // +0x04
    bool connected_ = false;       // +0x05
    bool gameStarted_ = false;     // +0x06
    std::int32_t field08_ = -1;    // +0x08 (the constructor sets -1; not identified)
    std::int32_t field0c_ = 0;     // +0x0c (not identified)
    std::int32_t updateTime_ = 0;  // +0x10 (time of the last list update in the original)
    std::int32_t state_ = 0;       // +0x14
    std::int32_t lastResult_ = 0;  // +0x48
    // +0x24: member list (vector of 84-byte entries in the original). Field +0x0c is the index
    // that SetPlayerParam writes to; it is not identified further (see field0c_).
    std::vector<tMemberInfo> members_;
};
