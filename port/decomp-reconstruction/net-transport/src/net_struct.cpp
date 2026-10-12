#include "net-transport/net_struct.hpp"

#include <cmath>

// --- NetStructUInt<Bits> -----------------------------------------------------

// NetStructUInt<8>::Read 0x36da38, <1> 0x39f5a0, <32> 0x39f5d8, <5> 0x817f34
template <unsigned Bits>
void NetStructUInt<Bits>::Read(NetBitStream& stream) {
    SetValue(stream.ReadU32(Bits));
}

// NetStructUInt<8>::Write 0x36dbf0, <1> 0x39f610, <32> 0x39f624, <5> 0x817f6c
template <unsigned Bits>
void NetStructUInt<Bits>::Write(NetBitStream& stream) const {
    stream.WriteU32(value_, Bits);
}

// NetStructUInt<8>::TestValue 0x36d404, <1> 0x39f518, <32> 0x39f52c (always true), <5> 0x817e28
template <unsigned Bits>
bool NetStructUInt<Bits>::TestValue(const std::uint32_t& value) const {
    if constexpr (Bits >= 32) {
        return true;
    } else {
        return value < (1u << Bits);
    }
}

template class NetStructUInt<1>;
template class NetStructUInt<5>;
template class NetStructUInt<8>;
template class NetStructUInt<32>;

// --- NetStructBool -----------------------------------------------------------

// 0x36f028 _ZN13NetStructBool4ReadER12NetBitStream
void NetStructBool::Read(NetBitStream& stream) {
    SetValue(stream.ReadBit() != 0);
}

// 0x36f064 _ZN13NetStructBool5WriteER12NetBitStream
void NetStructBool::Write(NetBitStream& stream) const {
    stream.WriteBit(value_ ? 1 : 0);
}

// --- NetStructRangedFloat<Bits, Min, Max> ------------------------------------

// Read: the original divides the raw value by 2^Bits - 1 (constant 0x487fffc0 for
// 18 bits, 0x477fff00 for 16 bits). Mapping that fraction onto [Min, Max] is
// inferred from the matching +Min offset in Write.
template <unsigned Bits, int Min, int Max>
void NetStructRangedFloat<Bits, Min, Max>::Read(NetBitStream& stream) {
    std::uint32_t raw = stream.ReadU32(Bits);
    float fraction = static_cast<float>(raw) / static_cast<float>((1u << Bits) - 1);
    SetValue(static_cast<float>(Min) + fraction * static_cast<float>(Max - Min));
}

// Write: the original adds -Min (0x48435000 = +200000 for Min -200000); the
// scaling to Bits-bit fixed point is inferred.
template <unsigned Bits, int Min, int Max>
void NetStructRangedFloat<Bits, Min, Max>::Write(NetBitStream& stream) const {
    float fraction = (value_ - static_cast<float>(Min)) / static_cast<float>(Max - Min);
    float scaled = std::round(fraction * static_cast<float>((1u << Bits) - 1));
    if (scaled < 0.f) {
        scaled = 0.f;
    }
    float top = static_cast<float>((1u << Bits) - 1);
    if (scaled > top) {
        scaled = top;
    }
    stream.WriteU32(static_cast<std::uint32_t>(scaled), Bits);
}

// NetStructRangedFloat<..>::TestValue 0x3a360c (16/-1..1), 0x3a3654 (16/+-50000), 0x3a3698 (18/+-200000)
template <unsigned Bits, int Min, int Max>
bool NetStructRangedFloat<Bits, Min, Max>::TestValue(const float& value) const {
    return value >= static_cast<float>(Min) && value <= static_cast<float>(Max);
}

template class NetStructRangedFloat<16, -1, 1>;
template class NetStructRangedFloat<16, -50000, 50000>;
template class NetStructRangedFloat<18, -200000, 200000>;

// --- NetStructNetworkId ------------------------------------------------------

// 0x801de0 _ZN18NetStructNetworkId4ReadER12NetBitStream
void NetStructNetworkId::Read(NetBitStream& stream) {
    CNetworkId id;
    id.Reset();
    id.Load(stream);
    SetValue(id);
}

// 0x801f14 _ZN18NetStructNetworkId5WriteER12NetBitStream
void NetStructNetworkId::Write(NetBitStream& stream) const {
    value_.Serialize(stream);
}
