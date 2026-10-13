// CRoomAttributes: the room settings that travel with a server record.
//
// Source addresses (32-bit ARM, Hex-Rays bodies; symbols from the function index):
//   ctor 0x819184 (C1/C2 0x8191c0), copy ctor 0x819050, dtor 0x818c14, Clear 0x818a28,
//   Copy 0x818900, operator= 0x81892c, NetRoomAttributes ctor 0x818dac, NetRoomAttributes::Copy 0x8186ac,
//   SetAttributeInt 0x818544, EraseAttributeInt 0x8183e0, SetAttributeBin 0x818cb0, EraseAttributeBin 0x818948,
//   IsAttibuteActive(int) 0x817d7c, IsAttibuteActive(bin) reads the bin mask (asm 0x817d7c+bin overload),
//   GetAttributeInt 0x817dac, GetAttributeBin 0x817dbc, GetMaxBufferSize 0x817e00,
//   Serialize(NetBitStream&, uint) 0x8190d8, Serialize(char*, size, flags) 0x81912c,
//   Serialize(int, int, NetBitStream&) 0x817ebc, Unserialize(NetBitStream&) 0x817e5c,
//   Unserialize(int, NetBitStream&) 0x817fec, ProcessLostPacket 0x817ef8, ProcessAcknowledgedPacket 0x817ef0,
//   HasChanged 0x817ee8, Resend 0x817e4c, AreChangesAcknowledged 0x817e54, Reload 0x817d78 (empty).
//
// Layout (original 32-bit offsets, kept as comments; sizeof is 916 on 32-bit):
//   +0x00 vptr (virtual dtor)
//   +0x08 NetRoomAttributes, a NetStruct-derived block of 908 bytes (the Serialize frame size).
//         Its own offsets are relative to +0x08; the comments below use CRoomAttributes offsets.
//   +0x138 + 40*i   int attribute i (i = 0..7): NetStructMember (40 bytes). Value (int) at +0x158 + 40*i.
//   +0x278 + 40*i   bin attribute i (i = 0..4): NetStructByteArray<256> (40 bytes). Data and size inside
//                   the record are not identified.
//   +0x340          int active-mask member (NetStructMember). Cached value (uint32, bit i = int attribute i)
//                   at +0x360. SetAttributeInt and EraseAttributeInt update it.
//   +0x368          bin active-mask member. Cached value (uint32, bit i = bin attribute i) at +0x388.
//   +0x35c          byte flag, zeroed by the NetRoomAttributes ctor. Meaning not identified.
//   +0x384          byte flag, zeroed by the NetRoomAttributes ctor. Meaning not identified.
//
// Copy masks (NetRoomAttributes::Copy, flags in Serialize and CRoomAttributes::Copy = 15):
//   flag 0x1 gates all int attributes. flag 0x2 gates bin attribute 0. flag 0x8 gates bin 1 and 2.
//   flag 0x4 gates bin 3 and 4. The group names are not identified.
//
// Not identified / not ported:
//   - Virtual slot +0x10 (called first by CRoomAttributes::Copy; the target is not identified).
//   - Names of the 8 int and 5 bin attribute ids (tROOM_ATTRIBUTES_INT / _BIN).
//   - Size table CSWTCH_280 used by GetMaxBufferSize (values not recovered).
//   - NetStruct wire format. Serialize and Unserialize need NetStruct::Serialize / Load, which are not
//     in the port, so they are declared only (BLOCKED).
//   - NetStruct::ProcessLostPacket / ProcessAcknowledgedPacket: the forwarders are inline no-op stand-ins.
#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include "net-transport/net_bit_stream.hpp"

class CRoomAttributes {
public:
    static constexpr int kIntAttributeCount = 8;   // ctor loop over the 8 int entries (NR+304..NR+624)
    static constexpr int kBinAttributeCount = 5;   // ctor loop over 200 bytes / 40 (NR+624..NR+824)

    // CRoomAttributes::CRoomAttributes() 0x819184: NetRoomAttributes ctor, then Clear().
    CRoomAttributes() { Clear(); }
    // CRoomAttributes::CRoomAttributes(const CRoomAttributes&) 0x819050: NetRoomAttributes ctor, then Copy.
    CRoomAttributes(const CRoomAttributes& other) { CopyWithFlags(other, 15); }
    // CRoomAttributes::operator=(const CRoomAttributes&) 0x81892c. Partial: Clear() stands in for the
    // unidentified virtual slot +0x10 that Copy calls first.
    CRoomAttributes& operator=(const CRoomAttributes& other) {
        if (this != &other) {
            Clear();
            CopyWithFlags(other, 15);
        }
        return *this;
    }
    virtual ~CRoomAttributes() = default;

    // CRoomAttributes::Clear 0x818a28: erases all int and bin attributes, then disables the NetStruct.
    void Clear() {
        for (int i = 0; i < kIntAttributeCount; ++i) EraseAttributeInt(i);
        for (int i = 0; i < kBinAttributeCount; ++i) EraseAttributeBin(i);
        netEnabled_ = false;  // NetStruct::SetEnabled(this+8, 0)
    }

    // CRoomAttributes::SetAttributeInt 0x818544: sets the value and the active bit.
    void SetAttributeInt(int attribute, int value) {
        intActiveMask_ |= (1u << attribute);
        intValues_[attribute] = value;
    }
    // CRoomAttributes::EraseAttributeInt 0x8183e0: value becomes 0 and the active bit is cleared.
    void EraseAttributeInt(int attribute) {
        intValues_[attribute] = 0;
        intActiveMask_ &= ~(1u << attribute);
    }
    // CRoomAttributes::GetAttributeInt 0x817dac: reads the value (no active check).
    int GetAttributeInt(int attribute) const { return intValues_[attribute]; }
    // CRoomAttributes::IsAttibuteActive(tROOM_ATTRIBUTES_INT) 0x817d7c: tests the int active mask.
    bool IsAttributeActiveInt(int attribute) const { return (intActiveMask_ & (1u << attribute)) != 0; }

    // CRoomAttributes::SetAttributeBin 0x818cb0: copies the bytes and sets the active bit.
    void SetAttributeBin(int attribute, const void* data, std::uint32_t size) {
        binActiveMask_ |= (1u << attribute);
        const auto* bytes = static_cast<const std::uint8_t*>(data);
        binValues_[attribute].assign(bytes, bytes + size);
    }
    // CRoomAttributes::EraseAttributeBin 0x818948: empties the bytes and clears the active bit.
    void EraseAttributeBin(int attribute) {
        binValues_[attribute].clear();
        binActiveMask_ &= ~(1u << attribute);
    }
    // CRoomAttributes::GetAttributeBin 0x817dbc: copies the bytes out and returns the size.
    std::uint32_t GetAttributeBin(int attribute, void* out, std::uint32_t outSize) const {
        const auto& value = binValues_[attribute];
        const std::uint32_t n = static_cast<std::uint32_t>(value.size()) < outSize
                                    ? static_cast<std::uint32_t>(value.size())
                                    : outSize;
        if (n != 0) std::copy(value.begin(), value.begin() + n, static_cast<std::uint8_t*>(out));
        return static_cast<std::uint32_t>(value.size());
    }
    // CRoomAttributes::IsAttibuteActive(tROOM_ATTRIBUTES_BIN) 0x817d7c (bin overload): tests the bin mask.
    bool IsAttributeActiveBin(int attribute) const { return (binActiveMask_ & (1u << attribute)) != 0; }

    // CRoomAttributes::Copy with a flags argument (NetRoomAttributes::Copy 0x8186ac). Copies only the
    // attributes whose group flag is set; destination entries outside those groups are left alone.
    void CopyWithFlags(const CRoomAttributes& other, unsigned flags) {
        std::uint32_t intMask = (flags & 0x1) ? other.intActiveMask_ : 0u;
        std::uint32_t binMask = other.binActiveMask_;
        if ((flags & 0x2) == 0) binMask &= ~1u;
        if ((flags & 0x4) == 0) binMask &= ~0x18u;
        if ((flags & 0x8) == 0) binMask &= ~0x06u;
        for (int i = 0; i < kIntAttributeCount; ++i) {
            if (intMask & (1u << i)) intValues_[i] = other.intValues_[i];
        }
        intActiveMask_ = intMask;
        for (int i = 0; i < kBinAttributeCount; ++i) {
            if (binMask & (1u << i)) binValues_[i] = other.binValues_[i];
        }
        binActiveMask_ = binMask;
    }

    // Serialize(NetBitStream&, uint) 0x8190d8 and Serialize(int, int, NetBitStream&) 0x817ebc.
    // BLOCKED: the NetStruct wire format is not ported. Declared only.
    bool Serialize(NetBitStream& stream, unsigned flags);
    // Unserialize(NetBitStream&) 0x817e5c. BLOCKED: declared only.
    bool Unserialize(NetBitStream& stream);

    // ProcessLostPacket(int, int) 0x817ef8 and ProcessAcknowledgedPacket(int, int) 0x817ef0.
    // Stand-in: the NetStruct bodies are not ported, so these do nothing.
    void ProcessLostPacket(int /*a1*/, int /*a2*/) {}
    void ProcessAcknowledgedPacket(int /*a1*/, int /*a2*/) {}

    // NetStruct::SetEnabled(this + 8, enabled) (0x81366c, net-transport). Stand-in: records the flag.
    // CMatchingGLLive::WritePacketData sets it to IsHost() before it serializes the room attributes.
    void SetNetEnabled(bool enabled) { netEnabled_ = enabled; }
    bool NetEnabled() const { return netEnabled_; }

    // Test access to the layout values.
    std::uint32_t IntActiveMask() const { return intActiveMask_; }
    std::uint32_t BinActiveMask() const { return binActiveMask_; }

private:
    int intValues_[kIntAttributeCount] = {};                        // int entries, value at +0x158 + 40*i
    std::vector<std::uint8_t> binValues_[kBinAttributeCount];       // bin entries (NetStructByteArray<256>)
    std::uint32_t intActiveMask_ = 0;                               // cached value at +0x360
    std::uint32_t binActiveMask_ = 0;                               // cached value at +0x388
    bool netEnabled_ = false;  // stand-in for NetStruct::SetEnabled(this+8); the bit's offset is not identified
};
