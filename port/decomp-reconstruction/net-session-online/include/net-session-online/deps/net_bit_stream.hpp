// Minimal stand-in for NetBitStream (owned by the net-transport category).
// Byte-aligned only: enough for the message layer and its tests. The bit-level
// API keeps the original signatures so the real class can replace this file.
#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>

class NetBitStream {
public:
    explicit NetBitStream(std::uint32_t capacityBytes) { data_.reserve(capacityBytes); }

    std::uint8_t ReadByte(unsigned /*bits*/) {
        if (pos_ >= data_.size()) return 0;
        return data_[pos_++];
    }
    void WriteByte(std::uint8_t value, unsigned /*bits*/) { data_.push_back(value); }

    void ReadByteArray(void* dst, std::uint32_t count) {
        const std::size_t avail = data_.size() - std::min(pos_, data_.size());
        const std::size_t n = std::min<std::size_t>(count, avail);
        if (n != 0) std::memcpy(dst, data_.data() + pos_, n);
        pos_ += n;
    }
    void WriteByteArray(const void* src, std::uint32_t count) {
        const auto* bytes = static_cast<const std::uint8_t*>(src);
        data_.insert(data_.end(), bytes, bytes + count);
    }

    void Rewind() { pos_ = 0; }

private:
    std::vector<std::uint8_t> data_;
    std::size_t pos_ = 0;
};
