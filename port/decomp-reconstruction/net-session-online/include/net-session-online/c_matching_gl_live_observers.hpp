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
//     +0x05 connected flag (byte), +0x06 game-started flag (byte), +0x07 flag (byte, set by
//     the list-session handler), +0x08 int (-1 at construction; the create handler reads the
//     host id here), +0x0c local member index (int), +0x10 update time (int),
//     +0x14 state (int), +0x18 room list (vector of tGLRoomInfo*), +0x24 member list (vector of
//     tMemberInfo, 84 bytes each in the original), +0x30 name map (std::map<std::string, int>),
//     +0x48 last result (int).
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "net-session-online/t_member_info.hpp"

class DataPacket;
class tGLRoomInfo;
class tMemberInfo;

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
    // Partial: for codes 0x6f and 0x70 the original sets the byte at +0x6c02 of the CMatching
    // singleton. The port writes CMatchingGLLive::unk_6c02, treating the singleton as the GLLive
    // object that owns this observer (see the .cpp).
    virtual void OnRequestSuccess(std::int32_t code, const char* text, std::int32_t length);
};

class CMatchingGLLiveLobbyObserver {
public:
    // 0x822538 _ZN28CMatchingGLLiveLobbyObserverC2Ev and 0x8225b8 C1Ev. Reconstructed: the scalars,
    // the empty room list (+0x18), the empty member list (+0x24) and the empty name map (+0x30).
    CMatchingGLLiveLobbyObserver();
    // 0x821ed8 _ZN28CMatchingGLLiveLobbyObserverD1Ev. Reconstructed: runs Terminate, which destroys the
    // room records. The D2 twin at 0x821f7c has the same body.
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
    // Partial: PrintDatapacket (0x8212d0, file-local in the tb file) is not called here; see the .cpp.
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
    // 0x821448 _ZN28CMatchingGLLiveLobbyObserver9TerminateEv. Reconstructed: calls ClearRoomList, then
    // clears the state and the flags.
    void Terminate();
    // 0x821470 _ZN28CMatchingGLLiveLobbyObserver21OnMPDisconnectSuccessEv
    // Thunk: forwards to Terminate (0x821448).
    virtual void OnMPDisconnectSuccess();

    // 0x82153c _ZN28CMatchingGLLiveLobbyObserver31OnMPProcessPushSameAccountLoginEP10DataPacket:
    // queues sign-in event 9 on the CSignIn singleton. The packet is not read.
    std::int32_t OnMPProcessPushSameAccountLogin(DataPacket* packet);

    // Room and member list handling. The room list (+0x18) holds tGLRoomInfo records and the member
    // list (+0x24) holds tMemberInfo entries (layouts in their own headers).
    // 0x8213ec _ZN28CMatchingGLLiveLobbyObserver13ClearRoomListEv. Reconstructed: pops each record,
    // runs its destructor (0x82138c) and CustomFree, as the original does.
    void ClearRoomList();
    // 0x821474 _ZN28CMatchingGLLiveLobbyObserver18GetRoomIndexByNameEPKc. Reconstructed.
    std::int32_t GetRoomIndexByName(const char* name) const;
    // 0x8214cc _ZN28CMatchingGLLiveLobbyObserver20GetMemberIndexByNameEPKc. Reconstructed.
    std::int32_t GetMemberIndexByName(const char* name) const;
    // 0x821bac _ZN28CMatchingGLLiveLobbyObserver17GetMemberIdVectorEv. Reconstructed.
    std::vector<std::int32_t> GetMemberIdVector() const;
    // 0x822324 _ZN28CMatchingGLLiveLobbyObserver19GetMemberByMemberIdEi. Reconstructed.
    tMemberInfo* GetMemberByMemberId(std::int32_t memberId);
    // 0x822638 _ZN28CMatchingGLLiveLobbyObserver14SetPlayerParamESs. Reconstructed (the mangled name
    // has no return type, so it is void).
    void SetPlayerParam(std::string value);
    // 0x822950 _ZN28CMatchingGLLiveLobbyObserver18CollapseMemberListEv. Reconstructed.
    void CollapseMemberList();

    // Session and push handlers. Vtable slots as in the dump (virtual where the slot is known).
    // 0x8216a8 _ZN28CMatchingGLLiveLobbyObserver29OnMPProcessPushGameserverAddrEP10DataPacket. Partial.
    std::int32_t OnMPProcessPushGameserverAddr(DataPacket* packet);
    // 0x8217a8 _ZN28CMatchingGLLiveLobbyObserver22OnMPListSessionSuccessEP10DataPacket (slot +0x18). Partial.
    virtual void OnMPListSessionSuccess(DataPacket* packet);
    // 0x822b78 _ZN28CMatchingGLLiveLobbyObserver23OnMPLeaveSessionSuccessEv (slot +0x20). Partial.
    virtual std::int32_t OnMPLeaveSessionSuccess();
    // 0x822c58 _ZN28CMatchingGLLiveLobbyObserver28OnMPProcessPushKickOutPlayerEP10DataPacket. Partial.
    std::int32_t OnMPProcessPushKickOutPlayer(DataPacket* packet);
    // 0x822db4 _ZN28CMatchingGLLiveLobbyObserver32OnMPProcessPushJoinLobbyLaunchedEP10DataPacket. Partial.
    std::int32_t OnMPProcessPushJoinLobbyLaunched(DataPacket* packet);
    // 0x822fa4 _ZN28CMatchingGLLiveLobbyObserver24OnMPCreateSessionSuccessEP10DataPacket (slot +0x14). Partial.
    virtual void OnMPCreateSessionSuccess(DataPacket* packet);
    // 0x8231e0 _ZN28CMatchingGLLiveLobbyObserver22OnMPJoinSessionSuccessEP10DataPacket (slot +0x1c). Partial.
    virtual void OnMPJoinSessionSuccess(DataPacket* packet);
    // 0x823510 _ZN28CMatchingGLLiveLobbyObserver31OnMPProcessPushSetUserParameterEP10DataPacket. Partial.
    std::int32_t OnMPProcessPushSetUserParameter(DataPacket* packet);
    // 0x8235f8 _ZN28CMatchingGLLiveLobbyObserver34OnMPProcessPushJoinLobbyUnlaunchedEP10DataPacket. Partial.
    std::int32_t OnMPProcessPushJoinLobbyUnlaunched(DataPacket* packet);
    // 0x823808 _ZN28CMatchingGLLiveLobbyObserver25OnMPProcessPushRejoinGameEP10DataPacket. Partial.
    std::int32_t OnMPProcessPushRejoinGame(DataPacket* packet);
    // 0x823ac4 _ZN28CMatchingGLLiveLobbyObserver25OnMPProcessPushLeaveLobbyEP10DataPacket. Partial.
    std::int32_t OnMPProcessPushLeaveLobby(DataPacket* packet);
    // 0x823fd8 _ZN28CMatchingGLLiveLobbyObserver23OnMPProcessPushMessagesEP10DataPacketi (slot +0x3c). Partial.
    virtual std::int32_t OnMPProcessPushMessages(DataPacket* packet, std::int32_t type);

    // Test seams. Not in the original.
    std::int32_t StateForTest() const { return state_; }
    bool LoggedInForTest() const { return loggedIn_; }
    bool ConnectedForTest() const { return connected_; }
    bool GameStartedForTest() const { return gameStarted_; }
    std::int32_t LastResultForTest() const { return lastResult_; }
    bool Flag07ForTest() const { return flag07_; }
    void SetFlagsForTest(bool loggedIn, bool connected, bool gameStarted) {
        loggedIn_ = loggedIn;
        connected_ = connected;
        gameStarted_ = gameStarted;
    }
    void SetLocalMemberIndexForTest(std::int32_t index) { field0c_ = index; }
    void PushRoomForTest(tGLRoomInfo* room) { roomList_.push_back(room); }
    std::size_t RoomCountForTest() const { return roomList_.size(); }
    void PushMemberForTest(const tMemberInfo& member) { memberList_.push_back(member); }
    std::size_t MemberCountForTest() const { return memberList_.size(); }

private:
    bool loggedIn_ = false;        // +0x04
    bool connected_ = false;       // +0x05
    bool gameStarted_ = false;     // +0x06
    bool flag07_ = false;          // +0x07 (set by OnMPListSessionSuccess; meaning not identified)
    std::int32_t field08_ = -1;    // +0x08 (the constructor sets -1; the create handler reads the host id here)
    std::int32_t field0c_ = 0;     // +0x0c (local member index: SetPlayerParam indexes the member list with it)
    std::int32_t updateTime_ = 0;  // +0x10 (time of the last list update in the original)
    std::int32_t state_ = 0;       // +0x14
    std::vector<tGLRoomInfo*> roomList_;    // +0x18 (records are CustomAlloc'd in the original)
    std::vector<tMemberInfo> memberList_;   // +0x24 (84-byte entries in the original)
    std::map<std::string, std::int32_t> nameMap_;  // +0x30 (not used by the reconstructed bodies)
    std::int32_t lastResult_ = 0;  // +0x48
};
