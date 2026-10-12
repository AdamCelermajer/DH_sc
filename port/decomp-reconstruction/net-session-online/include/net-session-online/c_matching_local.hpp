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

class CMatchingLocal {
public:
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

    // Test seams. Not in the original.
    void SetRoomFlagsForTest(bool closed, bool hiddenFlag) {
        roomClosed_ = closed ? 1 : 0;
        roomHiddenFlag_ = hiddenFlag ? 1 : 0;
    }
    std::int32_t MemberIdForTest() const { return memberId_; }

private:
    std::int32_t memberId_ = 0;     // +0x3638
    std::uint8_t roomClosed_ = 0;   // +0x4b6d
    std::uint8_t roomHiddenFlag_ = 0;  // +0x4b8d
};
