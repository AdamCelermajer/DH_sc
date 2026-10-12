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

class CNetworkId;
class NetBitStream;

class CMatchingGLLive {
public:
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
    // 0x81bc9c _ZN15CMatchingGLLive19GLFriendsListLoadedEv
    bool GLFriendsListLoaded() const { return glFriendsListLoaded_ != 0; }
    // 0x81bca8 _ZN15CMatchingGLLive19GLFriendsListLoadedEb
    void GLFriendsListLoaded(bool loaded) { glFriendsListLoaded_ = loaded ? 1 : 0; }

    // Not in the original: test seams for the room flags.
    void SetRoomFlagsForTest(bool closed, bool hidden) {
        roomClosed_ = closed ? 1 : 0;
        roomHidden_ = hidden ? 1 : 0;
    }
    std::int32_t MemberIdForTest() const { return memberId_; }

private:
    std::uint8_t roomHidden_ = 0;          // +0x0d
    std::uint8_t roomClosed_ = 0;          // +0x0e
    std::int32_t memberId_ = 0;            // +0x3640
    std::uint8_t glFriendsListLoaded_ = 0; // +0x6be0
};
