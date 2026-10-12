// NetStruct* members: typed fields of a replicated struct that know how to
// read and write themselves from a NetBitStream. Reconstructed from
// libDungeonHunter2.so pseudocode.
#pragma once

#include <cstdint>
#include <string>

#include "net-transport/deps/byte_array.hpp"
#include "net-transport/net_bit_stream.hpp"
#include "net-transport/net_network_id.hpp"

// NetStruct owns the process-wide change counter (symbol _ZN9NetStruct15s_changeCounterE, 0xa33f30).
// Base of every replicated game struct. Methods are declared by batch e1-net-transport-NetStruct-0;
// bodies are in src/tb_transport-NetStruct-0.cpp (partial ones are listed there).
// Layout (32-bit offsets, comments only): +0x00 vptr (Load +0x14, Erase +0x1c, GetSizeBits +0x38),
// +0x04 members (member i at +0x04 + 4*i), +0x104 member_count_, +0x108 byte (unidentified),
// +0x10c..+0x11f std::map packet history (not declared), +0x124 enabled_, +0x125 post_load_pending_,
// +0x128 post_load_arg_. sizeof 0x130.
class NetStructMember;

class NetStruct {
public:
    static constexpr int kMaxMembers = 64;  // 0x100 bytes at +0x04
    static std::uint64_t s_changeCounter;   // _ZN9NetStruct15s_changeCounterE, 0xa33f30

    NetStruct();                    // 0x81388c, 0x8138f4 (partial: packet history map not initialised)
    virtual ~NetStruct();           // 0x81395c (D1) (partial: packet history map not erased)

    void DeclareMember(NetStructMember* member);                                             // 0x81324c
    int ConditionalLoad(bool conditional, NetBitStream& stream, int a4, std::uint32_t a5);   // 0x813264
    virtual int GetSizeBits();                                                               // 0x81329c, vt +0x38
    std::uint32_t GetSizeBytes();                                                            // 0x8132f0
    bool AreChangesAcknowledgedBy(int memberId);                                             // 0x81334c (partial)
    int ResendInternal(std::uint32_t mask);                                                  // 0x8133c0 (partial)
    int Resend(int memberId);                                                                // 0x81344c (partial)
    int Resend();                                                                            // 0x81347c (partial)
    int SetAckedBy(int memberId);                                                            // 0x81349c (partial)
    int LoadInternal(bool acknowledged, NetBitStream& stream, int memberId);                 // 0x8134f4 (partial)
    virtual int Load(NetBitStream& stream, int memberId, std::uint32_t a4);                  // 0x8135f4, vt +0x14
    virtual int Erase(NetBitStream& stream);                                                 // 0x8135e4, vt +0x1c
    void SetEnabled(bool enabled);                                                           // 0x81366c (partial)
    bool AreChangesAcknowledged();                                                           // 0x813704
    bool HasDataToSend(int memberId);                                                        // 0x81375c (partial)
    std::uint32_t GetChangedBitMap();                                                        // 0x8137ec
    bool IsChanged();                                                                        // 0x81383c

protected:
    NetStructMember* members_[kMaxMembers] = {};    // +0x04
    int member_count_ = 0;                          // +0x104
    bool enabled_ = false;                          // +0x124
    bool post_load_pending_ = false;                // +0x125
    std::uint32_t post_load_arg_ = 0;               // +0x128
};

class NetStructMember {
public:
    virtual ~NetStructMember();                                              // 0x36d204 (D1)
    virtual void Read(NetBitStream& stream) = 0;                            // vtable +0x08
    virtual void Write(NetBitStream& stream) const = 0;                     // vtable +0x04
    virtual int GetSizeBits() const;                                        // 0x36d1fc (vtable +0x10)

    bool Load(NetBitStream& stream);                                        // 0x81510c
    bool Serialize(NetBitStream& stream, std::uint32_t mask);               // 0x815168

    bool IsChanged();                                                       // 0x814f70 (reads and clears)
    void SetChanged();                                                      // 0x814f84
    void SetAcked();                                                        // 0x814fd8
    void SetAcked(std::uint32_t bits);                                      // 0x814fe8
    void AcknowledgeChange(std::uint32_t bits, std::uint64_t stamp);        // 0x815004
    bool AreChangesAcknowledgedBy(std::uint32_t bits) const;                // 0x815028
    void SetSentFlag(std::uint32_t bits, bool sent, std::uint64_t stamp);   // 0x81503c
    bool HasDataToSend(std::uint32_t keep, std::uint32_t mask);             // 0x815078
    int GetFrameTime();                                                     // 0x8150a0 (partial)
    bool AreChangesAcknowledged();                                          // 0x8150b0 (partial)

protected:
    // Layout (32-bit offsets). Field names are inferred from how the methods use them.
    int sizeBits_ = 0;                  // +0x04 returned by GetSizeBits; set by derived constructors (not in this port yet)
    std::uint64_t changeStamp_ = 0;     // +0x08 NetStruct::s_changeCounter value at the last SetChanged
    std::uint32_t ackMask_ = 0;         // +0x10 bits that acknowledged the current change
    std::uint32_t sentMask_ = 0;        // +0x14 bits the current change was sent to
    std::uint32_t changeMask_ = 0;      // +0x18 mask copied into ackMask_ and sentMask_ by SetChanged
    bool changed_ = false;              // +0x1c set by SetChanged, cleared by IsChanged
};

template <typename T>
class NetStructMemberType : public NetStructMember {
public:
    virtual void SetValue(const T& value) { value_ = value; }   // vtable +0x1c
    virtual const T& GetValue() const { return value_; }        // vtable +0x24 (returns a reference: this+0x20)
    virtual bool TestValue(const T& value) const = 0;           // vtable +0x20

    // NetStructMemberType<T>::Erase(NetBitStream&): loads into the member, then restores the old value.
    // Only the int instantiation is defined (0x36dd64, see tb_transport-NetStructMemberType-0.cpp).
    int Erase(NetBitStream& stream);

protected:
    T value_{};                                                 // +0x20
};

// Explicit specializations for the instantiations in libDungeonHunter2.so. The bodies are in
// src/tb_transport-NetStructMemberType-0.cpp.
template <> void NetStructMemberType<std::int32_t>::SetValue(const std::int32_t& value);       // 0x36d9f0
template <> void NetStructMemberType<std::uint32_t>::SetValue(const std::uint32_t& value);     // 0x36da08
template <> void NetStructMemberType<bool>::SetValue(const bool& value);                       // 0x36da20
template <> int NetStructMemberType<std::int32_t>::Erase(NetBitStream& stream);                // 0x36dd64
template <> const std::int32_t& NetStructMemberType<std::int32_t>::GetValue() const;            // 0x36d2f0
template <> bool NetStructMemberType<std::int32_t>::TestValue(const std::int32_t& value) const;  // 0x36d474
template <> const std::uint32_t& NetStructMemberType<std::uint32_t>::GetValue() const;         // 0x36d2e8
template <> bool NetStructMemberType<std::uint32_t>::TestValue(const std::uint32_t& value) const; // 0x36d47c
template <> const bool& NetStructMemberType<bool>::GetValue() const;                           // 0x36d308
template <> bool NetStructMemberType<bool>::TestValue(const bool& value) const;                 // 0x36d484
template <> const std::string& NetStructMemberType<std::string>::GetValue() const;             // 0x36d2f8
template <> bool NetStructMemberType<std::string>::TestValue(const std::string& value) const;   // 0x36d46c
template <> const ByteArray& NetStructMemberType<ByteArray>::GetValue() const;                 // 0x36d390
template <> bool NetStructMemberType<ByteArray>::TestValue(const ByteArray& value) const;       // 0x36d388

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
