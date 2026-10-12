// Stand-in for CNetworkId (net-transport category). It keeps the original 28-byte
// record (seven 32-bit words) so that the blocks that copy it keep their size.
// Reset() zeroes the record and IsEqual() compares it byte for byte. The real class
// (net-transport/net_network_id.hpp) compares the presence flags and replaces this file.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

class CNetworkId {
public:
    static constexpr std::size_t kSize = 28;

    // 0x7fc384 _ZN10CNetworkId5ResetEv (stand-in: zeroes the record)
    void Reset() { std::memset(words_, 0, sizeof words_); }

    // 0x7fb93c _ZNK10CNetworkId7IsEqualERS_ (stand-in: byte compare)
    bool IsEqual(const CNetworkId& other) const {
        return std::memcmp(words_, other.words_, sizeof words_) == 0;
    }

    const std::uint8_t* Bytes() const { return reinterpret_cast<const std::uint8_t*>(words_); }
    std::uint8_t* Bytes() { return reinterpret_cast<std::uint8_t*>(words_); }

private:
    std::uint32_t words_[7] = {};  // +0x00..+0x1b
};

static_assert(sizeof(CNetworkId) == CNetworkId::kSize, "CNetworkId stand-in must be 28 bytes");
