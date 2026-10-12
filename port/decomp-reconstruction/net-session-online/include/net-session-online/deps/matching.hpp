// Stand-in for CMatching (the room / member layer, owned by another category).
// Only the members the message layer calls are declared. The vtable slot
// numbers come from the original `_ZTV9CMatching` dump; the names are inferred
// from how the callers use them (see NOTES.md).
#pragma once

#include <cstdint>

#include "net-session-online/deps/net_bit_stream.hpp"

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

    // Not in the stand-in's original: the alpha-server switch, a byte at +0x67ed of the
    // matching object (CMatchingGLLive). The name is not identified. Default: false.
    virtual bool UseAlphaServer() const { return false; }
    // CMatchingGLLive::SetGCState (0x81f178) is not in the port. Default: no-op.
    virtual void SetGCState(std::int32_t /*state*/) {}

    // The original reaches the matching object through a static accessor.
    static CMatching* Get() { return Current(); }
    static void Set(CMatching* matching) { Current() = matching; }

    // Added for the lib-online-glx slice (glx-01). Names of the non-virtual members follow the
    // mangled symbols. The vtable slots used by the static forwarders are exact byte offsets;
    // the member names are not identified. Defaults return 0 so tests can override them.
    enum MATCHING_PROVIDERS : std::int32_t {};
    // CMatching::s_matchingProvider (set by SetMatchingProvider). 0x7fe4c0.
    static inline std::int32_t s_matchingProvider = 0;
    static void SetMatchingProvider(MATCHING_PROVIDERS provider);       // 0x7fe4c0
    std::int32_t ReceiveGameBootInvitation() const;                     // 0x7fe4a4, returns 0
    std::uint64_t GetRoomId() const;                                    // 0x7fe4ac, returns 0
    std::int32_t LimitRoomSlots(std::uint32_t slots) const;             // 0x7fe4b8, returns 0
    bool IsMemberServer(std::int32_t memberId) const;                   // 0x7fe544
    std::int32_t GetMemberIndex(std::int32_t memberId) const;           // 0x7fe59c, lowest set bit
    // Static forwarders to the matching object (s_Matching). Null instance returns 0 here.
    static std::int32_t sReadPacketData(std::int32_t a1, std::int32_t a2, NetBitStream& stream);    // 0x7fe628
    static std::int32_t sWritePacketData(std::int32_t a1, std::int32_t a2, NetBitStream& stream);   // 0x7fe66c
    static std::int32_t sProcessAcknowledgedPacket(std::int32_t a1, std::int32_t a2);               // 0x7fe6b0
    static std::int32_t sProcessLostPacket(std::int32_t a1, std::int32_t a2);                       // 0x7fe6ec
    // Vtable slots +0x0c, +0x10, +0x14, +0x18 (names not identified).
    virtual std::int32_t vtWritePacketData(std::int32_t, std::int32_t, NetBitStream&) { return 0; }
    virtual std::int32_t vtReadPacketData(std::int32_t, std::int32_t, NetBitStream&) { return 0; }
    virtual std::int32_t vtProcessAcknowledgedPacket(std::int32_t, std::int32_t) { return 0; }
    virtual std::int32_t vtProcessLostPacket(std::int32_t, std::int32_t) { return 0; }

private:
    static CMatching*& Current() {
        static CMatching* instance = nullptr;
        return instance;
    }
};
