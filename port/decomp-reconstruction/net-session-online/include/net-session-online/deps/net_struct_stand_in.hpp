// Stand-in for NetStruct and NetStructMember (net-transport category). It holds only what
// the net-session-online NetStruct subclasses use. The real classes are in
// net-transport/net_struct.hpp; this file is replaced when the category links that header.
//
// Everything is in namespace stand_in so this stand-in cannot collide with the real global
// NetStruct / NetStructMember classes. Layout comments use the original 32-bit offsets
// (NetStruct sizeof 0x130, NetStructMember base 0x20, NetStructMemberType<T>::value_ at +0x20).
#pragma once

#include <cstdint>
#include <vector>

#include "net-session-online/deps/network_id.hpp"

namespace stand_in {

// Process-wide change counter (NetStruct::s_changeCounter in the original, 0xa33f30).
inline std::uint64_t g_netStructChangeCounter = 0;

class NetStructMember;

// Base of every replicated struct. The original keeps 64 member pointers at +0x04, the member
// count at +0x104, and a std::map packet history at +0x10c (not modelled here).
class NetStruct {
public:
    NetStruct() = default;
    virtual ~NetStruct() = default;

    // 0x81324c NetStruct::DeclareMember. Stand-in: records the member pointer.
    void DeclareMember(NetStructMember* member) { members_.push_back(member); }

protected:
    std::vector<NetStructMember*> members_;  // +0x04 (fixed array of 64 in the original)
};

// Base of every replicated member. Field offsets are the original ones relative to the member.
class NetStructMember {
public:
    // Initial values written by a derived constructor (the original writes these fields directly).
    struct Init {
        int sizeBits = 0;              // +0x04
        std::uint64_t changeStamp = 0; // +0x08
        std::uint32_t ackMask = 0;     // +0x10
        std::uint32_t sentMask = 0;    // +0x14
        std::uint32_t changeMask = 0;  // +0x18
        bool changed = false;          // +0x1c
    };

    NetStructMember() = default;
    explicit NetStructMember(const Init& init)
        : sizeBits_(init.sizeBits),
          changeStamp_(init.changeStamp),
          ackMask_(init.ackMask),
          sentMask_(init.sentMask),
          changeMask_(init.changeMask),
          changed_(init.changed) {}
    virtual ~NetStructMember() = default;

    // 0x814f84 NetStructMember::SetChanged. Stand-in: stamps the change and copies the change
    // mask into the ack and sent masks, as the field comments in net-transport describe.
    void SetChanged() {
        changeStamp_ = g_netStructChangeCounter;
        ackMask_ = changeMask_;
        sentMask_ = changeMask_;
        changed_ = true;
    }

protected:
    int sizeBits_ = 0;                    // +0x04
    std::uint64_t changeStamp_ = 0;       // +0x08
    std::uint32_t ackMask_ = 0;           // +0x10
    std::uint32_t sentMask_ = 0;          // +0x14
    std::uint32_t changeMask_ = 0;        // +0x18
    bool changed_ = false;                // +0x1c
};

// Typed member. The value sits at +0x20 of the member, as in the original.
template <typename T>
class NetStructMemberType : public NetStructMember {
public:
    NetStructMemberType() = default;
    NetStructMemberType(const Init& init, const T& value) : NetStructMember(init), value_(value) {}
    explicit NetStructMemberType(const T& value) : value_(value) {}

    // Raw access to the value slot (+0x20). The original's constructors read and write it directly.
    T& RawValue() { return value_; }
    const T& RawValue() const { return value_; }

protected:
    T value_{};  // +0x20
};

}  // namespace stand_in
