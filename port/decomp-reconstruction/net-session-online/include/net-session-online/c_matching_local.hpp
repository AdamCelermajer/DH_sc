// CMatchingLocal: the matching layer for a local (LAN or Bluetooth-free) session. Only
// the small accessors are reconstructed here. The room list, the server list, the
// packet callbacks and the destructors are queued.
//
// Layout (original 32-bit offsets, kept as comments):
//   +0x3638 local member id (int)
//   +0x4b6d room closed flag (byte)
//   +0x4b8d room hidden flag (byte). When set, IsRoomHidden reads true.
#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "net-session-online/deps/matching_stand_ins.hpp"
#include "net-session-online/deps/net_mutex.hpp"
#include "net-session-online/t_matching_peer.hpp"

class CMatchingLocal {
public:
    // 0x807e44 _ZN14CMatchingLocalC1Ev: sets the counters and empty containers. The original
    // also runs Reset() and the NetStruct member constructors, which are not in this port yet.
    CMatchingLocal();
    // 0x807f80 _ZN14CMatchingLocalD1Ev: the members release themselves. The NetStruct member
    // teardown of the original is not in this port yet.
    ~CMatchingLocal();

    // 0x801b08 _ZN14CMatchingLocal12IsRoomClosedEv
    bool IsRoomClosed() const { return roomClosed_ != 0; }
    // 0x801b14 _ZN14CMatchingLocal12IsRoomHiddenEv: the hidden flag, else the closed flag.
    bool IsRoomHidden() const;
    // 0x801b3c _ZN14CMatchingLocal11SetMemberIdEi
    void SetMemberId(std::int32_t memberId) { memberId_ = memberId; }
    // 0x805664 _ZN14CMatchingLocal13GetMemberMaskEi: see member_mask.hpp.
    std::int32_t GetMemberMask(std::int32_t memberId) const;
    // 0x805694 _ZN14CMatchingLocal19GetInvitationRoomIdEv: returns 0 (64-bit).
    std::uint64_t GetInvitationRoomId() const { return 0; }
    // 0x8056a0 _ZN14CMatchingLocal16GetTransportTypeE14tPROTOCOL_TYPE: table {1, 2, 1}
    // indexed by the protocol type; any type above 2 gives 0. The object is not read.
    static std::int32_t GetTransportType(std::uint32_t protocolType);
    // 0x801fb0 _ZN14CMatchingLocal14SendInvitationEPKcS1_b: posts event 0x800015, returns 0.
    // The original's names are not used by the body.
    std::int32_t SendInvitation(const char* from, const char* to, bool flag);
    // 0x801fe8 _ZN14CMatchingLocal17ReceiveInvitationEv: posts event 0x800017, returns 0.
    std::int32_t ReceiveInvitation();
    // 0x805998 _ZN14CMatchingLocal26ProcessChangeServerMessageEiR12NetBitStream: reads the
    // 4-byte server id into +0x363c, then posts event 0x80000b carrying it.
    void ProcessChangeServerMessage(std::int32_t from, class NetBitStream& stream);

    // Test seams. Not in the original.
    void SetRoomFlagsForTest(bool closed, bool hiddenFlag) {
        roomClosed_ = closed ? 1 : 0;
        roomHiddenFlag_ = hiddenFlag ? 1 : 0;
    }
    std::int32_t MemberIdForTest() const { return memberId_; }
    std::int32_t ServerMemberIdForTest() const { return serverMemberId_; }

private:
    std::int32_t memberId_ = 0;     // +0x3638
    std::int32_t serverMemberId_ = 0;  // +0x363c (name inferred, as in CMatchingGLLive)
    std::uint8_t roomClosed_ = 0;   // +0x4b6d
    std::uint8_t roomHiddenFlag_ = 0;  // +0x4b8d
};
