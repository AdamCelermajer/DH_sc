// NetStructInt<Bits>: signed integer member of Bits bits on the wire. The magnitude
// takes Bits-1 bits and one sign bit follows (1 = negative). Reconstructed from the
// NetStructInt<N> methods of libDungeonHunter2.so. The value sits at +0x20, inherited
// from NetStructMemberType<std::int32_t>.
#pragma once

#include <cstdint>

#include "net-transport/net_struct.hpp"

template <unsigned Bits>
class NetStructInt final : public NetStructMemberType<std::int32_t> {
public:
    ~NetStructInt() override = default;                                 // 0x36d254 / 0x36d274 / 0x36d278 / 0x36d310 (D1)
    void Read(NetBitStream& stream) override;                           // NetStructInt<N>::Read(NetBitStream&)
    void Write(NetBitStream& stream) const override;                    // NetStructInt<N>::Write(NetBitStream&)
    bool TestValue(const std::int32_t& value) const override;           // NetStructInt<N>::TestValue(int const&)
};
