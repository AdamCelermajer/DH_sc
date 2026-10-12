// NetStruct* members: typed fields of a replicated struct that know how to
// read and write themselves from a NetBitStream. Reconstructed from
// libDungeonHunter2.so pseudocode.
#pragma once

#include <cstdint>

#include "net-transport/net_bit_stream.hpp"
#include "net-transport/net_network_id.hpp"

class NetStructMember {
public:
    virtual ~NetStructMember() = default;
    virtual void Read(NetBitStream& stream) = 0;
    virtual void Write(NetBitStream& stream) const = 0;
};

template <typename T>
class NetStructMemberType : public NetStructMember {
public:
    virtual void SetValue(const T& value) { value_ = value; }   // vtable +0x1c
    virtual T GetValue() const { return value_; }               // vtable +0x24
    virtual bool TestValue(const T& value) const = 0;           // vtable +0x20

protected:
    T value_{};                                                 // +0x20
};

// Unsigned integer member of Bits bits on the wire.
template <unsigned Bits>
class NetStructUInt final : public NetStructMemberType<std::uint32_t> {
public:
    void Read(NetBitStream& stream) override;                         // NetStructUInt<N>::Read
    void Write(NetBitStream& stream) const override;                  // NetStructUInt<N>::Write
    bool TestValue(const std::uint32_t& value) const override;        // NetStructUInt<N>::TestValue
};

// Single bit member.
class NetStructBool final : public NetStructMember {
public:
    void Read(NetBitStream& stream) override;                         // 0x36f028
    void Write(NetBitStream& stream) const override;                  // 0x36f064
    void SetValue(bool value) { value_ = value; }
    bool GetValue() const { return value_; }

private:
    bool value_ = false;                                              // +0x1d
};

// Float in [Min, Max] sent as Bits-bit fixed point.
template <unsigned Bits, int Min, int Max>
class NetStructRangedFloat final : public NetStructMemberType<float> {
public:
    void Read(NetBitStream& stream) override;                         // NetStructRangedFloat<...>::Read
    void Write(NetBitStream& stream) const override;                  // NetStructRangedFloat<...>::Write
    bool TestValue(const float& value) const override;                // NetStructRangedFloat<...>::TestValue
};

// CNetworkId member (value at +0x20 in the original).
class NetStructNetworkId final : public NetStructMember {
public:
    void Read(NetBitStream& stream) override;                         // 0x801de0
    void Write(NetBitStream& stream) const override;                  // 0x801f14
    void SetValue(const CNetworkId& value) { value_ = value; }
    CNetworkId GetValue() const { return value_; }

private:
    CNetworkId value_;                                                // +0x20
};
