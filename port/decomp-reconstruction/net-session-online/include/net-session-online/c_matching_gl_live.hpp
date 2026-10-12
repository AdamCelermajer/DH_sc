// CMatchingGLLive: the matching layer on top of the Gameloft Live services. This header
// declares the members and methods that this port reconstructs so far. The class is
// large in the original (the GLX service objects sit at +0x6700 and above, and the
// CMatching base is not modelled), so only the fields these bodies use are listed.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x0d room hidden flag (byte)   +0x0e room closed flag (byte)
//       (both are in the CMatching base in the original)
//   +0x3640 local member id (int)
//   +0x3644 server member id (int)
//   +0x6be0 friend list loaded flag (byte)
#pragma once

#include <cstdint>
#include <map>

#include "net-session-online/c_event_queue_base.hpp"
#include "net-session-online/c_room_attributes.hpp"
#include "net-session-online/c_room_search_filter.hpp"

class CNetworkId;
class NetBitStream;
class CMatchingGLLiveFriendObserver;
class CMatchingGLLiveLobbyObserver;
class CSignInGLLiveObserver;
class GLXPlayerComponent;
class GLXPlayerLeaderboard;
class GLXPlayerMPLobby;
class GameCenterInfo;
class GLXPlayerLogin;
class GLXPlayerUserFriend;

class CMatchingGLLive {
public:
    // Statics of the original (s_IsGameStarting, the GLLive event queue and the
    // observer singletons). Defined in src/c_matching_gl_live_glx04.cpp.
    static bool s_IsGameStarting;
    static CEventQueueBase s_eventQueueGLLiveInternal;             // global queue
    static CMatchingGLLiveFriendObserver* s_GLXPlayerUserFriendObserver;
    static CMatchingGLLiveLobbyObserver* s_GLXPlayerMPLobbyObserver;
    static CSignInGLLiveObserver* s_GLXPlayerLoginObserver;
    // Lobby singleton (0x81ef7c creates it in the original) and the Game Center record.
    // Both are defined in src/c_matching_gl_live_glx05.cpp.
    static GLXPlayerMPLobby* s_GLXPlayerMPLobby;
    static GameCenterInfo* s_GameCenterInfo;


    // 0x81bbe8 _ZN15CMatchingGLLive11SetMemberIdEi
    void SetMemberId(std::int32_t memberId) { memberId_ = memberId; }
    // 0x81bbf4 _ZN15CMatchingGLLive19GenerateNewMemberIdEv: always -1 (no generation here).
    std::int32_t GenerateNewMemberId() const;
    // 0x81bbfc _ZN15CMatchingGLLive25SendConnectRequestMessageE12tPACKET_TYPEi: returns 0.
    std::int32_t SendConnectRequestMessage(int packetType, std::int32_t target) const;
    // 0x81bc04 _ZN15CMatchingGLLive28ProcessConnectRequestMessageER10CNetworkIdR12NetBitStream: no-op.
    void ProcessConnectRequestMessage(CNetworkId& from, NetBitStream& stream) const;
    // 0x81bc74 _ZN15CMatchingGLLive29ProcessConnectResponseMessageER10CNetworkIdR12NetBitStream: no-op.
    void ProcessConnectResponseMessage(CNetworkId& from, NetBitStream& stream) const;
    // 0x81bc94 _ZN15CMatchingGLLive20ProcessClientMessageER10CNetworkIdR12NetBitStream: no-op.
    void ProcessClientMessage(CNetworkId& from, NetBitStream& stream) const;

    // 0x81bc08 _ZN15CMatchingGLLive17ReceiveInvitationEv: returns 0.
    std::int32_t ReceiveInvitation() const;
    // 0x81bc10 _ZN15CMatchingGLLive19GetInvitationRoomIdEv: returns 0 (64-bit).
    std::uint64_t GetInvitationRoomId() const;
    // 0x81bc1c _ZN15CMatchingGLLive9CloseRoomEv: returns 0.
    std::int32_t CloseRoom() const;
    // 0x81bc24 _ZN15CMatchingGLLive8OpenRoomEb: returns 0.
    std::int32_t OpenRoom(bool value) const;
    // 0x81bc2c _ZN15CMatchingGLLive8HideRoomEv: returns 0.
    std::int32_t HideRoom() const;
    // 0x81bc34 _ZN15CMatchingGLLive8ShowRoomEv: returns 0.
    std::int32_t ShowRoom() const;
    // 0x81bc3c _ZN15CMatchingGLLive12IsRoomClosedEv
    bool IsRoomClosed() const { return roomClosed_ != 0; }
    // 0x81bc44 _ZN15CMatchingGLLive12IsRoomHiddenEv
    bool IsRoomHidden() const { return roomHidden_ != 0; }
    // 0x81bc78 _ZN15CMatchingGLLive13GetMemberMaskEi: see member_mask.hpp.
    std::int32_t GetMemberMask(std::int32_t memberId) const;
    // 0x81bc4c _ZN15CMatchingGLLive19ProcessFriendEventsEv: returns 0.
    std::int32_t ProcessFriendEvents() const;
    // 0x81bc98 _ZN15CMatchingGLLive13InitGLFriendsEv: no-op.
    void InitGLFriends() const;
    // The four GL friend accessors below use the friend list at +0x6700 (glxUserFriend_),
    // which may be null.
    // 0x81c098 _ZNK15CMatchingGLLive17GetGLFriendsCountEv: count, or 0 when the list is null.
    std::int32_t GetGLFriendsCount() const;
    // 0x81c0ac _ZNK15CMatchingGLLive16GetGLFriendStateEi: state of a friend, -1 out of range.
    std::int32_t GetGLFriendState(std::int32_t index) const;
    // 0x81c0e8 _ZNK15CMatchingGLLive18SendGLFriendInviteEPKc. Partial: the original returns the
    // result of sendAddUserFriend; the port's version returns void, so this returns 0.
    std::int32_t SendGLFriendInvite(const char* name) const;
    // 0x81c0fc _ZNK15CMatchingGLLive16GetGLFriendsNameEi: name of a friend, null out of range.
    const char* GetGLFriendsName(std::int32_t index) const;
    // 0x81bc54 _ZN15CMatchingGLLive16GetTransportTypeEj: table {1, 2, 1}, 0 above 2.
    static std::int32_t GetTransportType(std::uint32_t protocolType);
    // 0x81bc9c _ZN15CMatchingGLLive19GLFriendsListLoadedEv
    bool GLFriendsListLoaded() const { return glFriendsListLoaded_ != 0; }
    // 0x81bca8 _ZN15CMatchingGLLive19GLFriendsListLoadedEb
    void GLFriendsListLoaded(bool loaded) { glFriendsListLoaded_ = loaded ? 1 : 0; }

    // 0x81bcd4 _ZN15CMatchingGLLive17SetGLLivePasswordEPKc: copies the password when it is
    // at most 15 characters (16-byte buffer). Partial: a rejected password returns nullptr;
    // the original returns the length in the same register.
    char* SetGLLivePassword(const char* password);
    // 0x81bd08 _ZN15CMatchingGLLive17SetGLLiveUserNameEPKc: same rule as the password.
    char* SetGLLiveUserName(const char* userName);
    // 0x81bd3c _ZN15CMatchingGLLive17SaveGLLiveProfileEv: writes profile.sav.
    int SaveGLLiveProfile();
    // 0x81be2c _ZN15CMatchingGLLive17LoadGLLiveProfileEv: reads profile.sav.
    int LoadGLLiveProfile();
    // 0x81bf88 _ZN15CMatchingGLLive10IsLoggedInEv. Partial: reads the sign-in logged-in flag
    // through CSignIn::LoggedInForTest() (no production accessor in c_sign_in.hpp yet).
    int IsLoggedIn();
    // 0x81bf98 _ZNK15CMatchingGLLive17GetServerMemberIdEv
    std::int32_t GetServerMemberId() const;
    // 0x81c084 _ZN15CMatchingGLLive15GLSendHighScoreEi. Partial: sendHighScore returns void
    // in the port, so the returned status is 0 and not the original's result.
    std::int32_t GLSendHighScore(std::int32_t score);
    // 0x81c18c _ZN15CMatchingGLLive6GetGGIEv: GGI of the GLX component, 0 when none.
    unsigned long GetGGI();
    // 0x81c32c _ZN15CMatchingGLLive30GetGLXPlayerUserFriendObserverEv
    CMatchingGLLiveFriendObserver* GetGLXPlayerUserFriendObserver();
    // 0x81c410 _ZN15CMatchingGLLive27GetGLXPlayerMPLobbyObserverEv
    CMatchingGLLiveLobbyObserver* GetGLXPlayerMPLobbyObserver();
    // 0x81c598 _ZN15CMatchingGLLive25GetGLXPlayerLoginObserverEv
    CSignInGLLiveObserver* GetGLXPlayerLoginObserver();
    // 0x81c880 _ZN15CMatchingGLLive9PingRoomsEv: posts event 8388624, returns 0.
    std::int32_t PingRooms();
    // 0x81c1a0 _ZN15CMatchingGLLive26ProcessChangeServerMessageEiR12NetBitStream
    void ProcessChangeServerMessage(int from, NetBitStream& stream);
    // 0x81c8b8 _ZN15CMatchingGLLive24ProcessRoomStatusMessageEiR12NetBitStream
    void ProcessRoomStatusMessage(int from, NetBitStream& stream);
    // 0x81c938 _ZN15CMatchingGLLive22PacketReceiverCallbackEiPci. Partial: the case for
    // message type 3 (ProcessMemberJoinMessage, 0x81c200) is not dispatched yet.
    void PacketReceiverCallback(int from, char* data, int size);
    // 0x81ca24 _ZN15CMatchingGLLive19ProcessGLLiveEventsEv
    std::int32_t ProcessGLLiveEvents();

    // 0x81ef7c _ZN15CMatchingGLLive19GetGLXPlayerMPLobbyEv. Partial: the original creates the
    // lobby on first use (socket URL from the login observer, a copy of the lobby observer,
    // a 0x94-byte allocation). That path is not reconstructed; this returns s_GLXPlayerMPLobby.
    GLXPlayerMPLobby* GetGLXPlayerMPLobby();
    // 0x81c5e8 _ZN15CMatchingGLLive17GetGLXPlayerLoginEv. Declared only: the body belongs to its own slice.
    GLXPlayerLogin* GetGLXPlayerLogin();
    // 0x820508 _ZN15CMatchingGLLive17GetRoomAttributesEv: &roomAttributes_[roomKey_] (creates the entry).
    // The original uses std::map<unsigned long long, CRoomAttributes>; the key is the int at +0x680c.
    CRoomAttributes* GetRoomAttributes();
    // 0x81fadc _ZN15CMatchingGLLive18SearchRoomInternalER17CRoomSearchFilterbh. Partial, see the cpp.
    std::int32_t SearchRoomInternal(CRoomSearchFilter& filter, bool flag, std::uint8_t mode);
    // 0x81f128 _ZN15CMatchingGLLive9StartGameEv
    void StartGame();
    // 0x81f178 _ZN15CMatchingGLLive10SetGCStateEi. Partial: the CSignIn event (state 1) and the
    // CreateJoinRoom (state 3) branches are not reconstructed. States 2 and 7 are.
    void SetGCState(std::int32_t state);
    // 0x81f44c _ZN15CMatchingGLLive19SendSetPlayerStatusEc. Partial: the original returns the
    // lobby call's result; the port returns 0 because mpSendSetPlayerStatus is void here.
    std::int32_t SendSetPlayerStatus(std::uint8_t status);
    // 0x81f464 _ZN15CMatchingGLLive9LeaveRoomEv
    std::int32_t LeaveRoom();
    // 0x81f524 _ZN15CMatchingGLLive6IsHostEv: the lobby's mpIsMaster (vtable slot 60).
    bool IsHost();

    // Not in the original: test seams for the room flags.
    void SetRoomFlagsForTest(bool closed, bool hidden) {
        roomClosed_ = closed ? 1 : 0;
        roomHidden_ = hidden ? 1 : 0;
    }
    std::int32_t MemberIdForTest() const { return memberId_; }
    std::int32_t ServerMemberIdForTest() const { return serverMemberId_; }
    std::int32_t RoomValue0ForTest() const { return roomValue0_; }
    std::int32_t RoomValue1ForTest() const { return roomValue1_; }
    std::uint8_t RoomStatusForTest(int index) const { return roomStatus_[index]; }
    const char* UserNameForTest() const { return userName_; }
    const char* PasswordForTest() const { return password_; }
    bool ProfileFlagForTest() const { return profileFlag_ != 0; }
    void SetProfileFlagForTest(bool value) { profileFlag_ = value ? 1 : 0; }
    bool GLLiveEventFlagForTest() const { return glLiveEventFlag_ != 0; }
    std::int32_t RoomOpForTest() const { return roomOp_; }
    std::int32_t GcStateForTest() const { return gcState_; }
    void SetGameCenterActiveForTest(bool value) { gameCenterActive_ = value; }
    void SetRoomKeyForTest(std::int32_t key) { roomKey_ = key; }
    const CRoomSearchFilter& SearchFilterForTest() const { return searchFilter_; }
    void SetGLFriendListForTest(GLXPlayerUserFriend* list) { glxUserFriend_ = list; }

private:
    std::uint8_t roomHidden_ = 0;          // +0x0d
    std::uint8_t roomClosed_ = 0;          // +0x0e
    std::uint8_t roomStatus_[2] = {0, 0};  // +0x3638, +0x3639 (fields not identified)
    std::int32_t memberId_ = 0;            // +0x3640
    std::int32_t serverMemberId_ = 0;      // +0x3644
    std::int32_t roomValue0_ = 0;          // +0x3648 (not identified)
    std::int32_t roomValue1_ = 0;          // +0x364c (not identified)
    std::uint8_t glFriendsListLoaded_ = 0; // +0x6be0
    char userName_[16] = {};               // +0x6be1 (GLLive user name, 16 bytes)
    std::uint8_t profileFlag_ = 0;         // +0x6bf1 (remember flag in profile.sav)
    char password_[16] = {};               // +0x6bf2 (GLLive password, 16 bytes)
    GLXPlayerComponent* glxComponent_ = nullptr;      // +0x6710 (GGI source)
    GLXPlayerLeaderboard* glxLeaderboard_ = nullptr;  // +0x671c (high-score sender)
    std::uint8_t glLiveEventFlag_ = 0;     // +0x6810 (set by event 5, cleared by 6)
    bool gameCenterActive_ = false;        // +0x67e8 (Game Center mode, gates SetGCState)
    std::int32_t roomOp_ = 0;              // +0x6834 (room operation state: 5 after LeaveRoom)
    std::int32_t gcState_ = 0;             // +0x6838 (Game Center state, see SetGCState)
    std::map<std::uint64_t, CRoomAttributes> roomAttributes_;  // +0x6720 (room id -> room settings)
    CRoomSearchFilter searchFilter_;       // +0x6738 (copied by SearchRoomInternal)
    std::int32_t roomKey_ = 0;             // +0x680c (room id used by GetRoomAttributes)
    GLXPlayerUserFriend* glxUserFriend_ = nullptr;  // +0x6700 (friend list, null until created)
};
