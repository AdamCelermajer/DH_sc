// Stand-in for CMatching (the room / member layer, owned by another category).
// Only the members the message layer calls are declared. The vtable slot
// numbers come from the original `_ZTV9CMatching` dump; the names are inferred
// from how the callers use them (see NOTES.md).
#pragma once

#include <cstdint>

class CMatching {
public:
    virtual ~CMatching() = default;

    // vtable +0x84: bit for a member id in a destination/acknowledgement mask.
    virtual std::uint32_t MemberBit(int memberId) const = 0;
    // vtable +0x6c: id of the local player.
    virtual int LocalMemberId() const = 0;
    // vtable +0x74: id of the room server.
    virtual int ServerMemberId() const = 0;
    // Non-virtual in the original; exposed virtual so tests can fake it.
    virtual bool IsServer() const = 0;
    virtual std::uint32_t GetRoomMask() const = 0;

    // The original reaches the matching object through a static accessor.
    static CMatching* Get() { return Current(); }
    static void Set(CMatching* matching) { Current() = matching; }

private:
    static CMatching*& Current() {
        static CMatching* instance = nullptr;
        return instance;
    }
};
