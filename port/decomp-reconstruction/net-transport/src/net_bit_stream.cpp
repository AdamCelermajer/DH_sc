#include "net-transport/net_bit_stream.hpp"

#include <cstring>
#include <new>

namespace {
constexpr std::uint32_t kReadOverflow = 1;
constexpr std::uint32_t kWriteOverflow = 2;

std::uint32_t BytesFor(std::uint32_t bits) {
    return (bits >> 3) + ((bits & 7) != 0 ? 1 : 0);
}
}  // namespace

// 0x80ea50 _ZN12NetBitStreamC2Ej
// 0x80e908 _ZN12NetBitStreamC1Ej
NetBitStream::NetBitStream(std::uint32_t size) {
    buffer_ = new std::uint8_t[size]();
    if (buffer_ != nullptr) {
        size_ = size;
    }
}

// 0x80ef8c _ZN12NetBitStreamC2ERKS_
// 0x80eef4 _ZN12NetBitStreamC1ERKS_
// Partial: the original allocates with nothrow new and then memsets the result even
// when the allocation failed (a crash). Here the copy is skipped on a null buffer
// and a throwing allocation is not used. Copies buffer, size and both positions.
NetBitStream::NetBitStream(const NetBitStream& other) {
    buffer_ = new (std::nothrow) std::uint8_t[other.size_]();
    if (buffer_ != nullptr) {
        std::memcpy(buffer_, other.buffer_, other.size_);
        size_ = other.size_;
        read_pos_ = other.read_pos_;
        write_pos_ = other.write_pos_;
    }
}

// 0x80ee5c _ZN12NetBitStreamaSERKS_
// Partial: nothrow allocation; the original memsets the buffer even when it is null.
// Here that memset is skipped. Copies the bytes, then size, read and write position.
NetBitStream& NetBitStream::operator=(const NetBitStream& other) {
    if (this != &other) {
        std::uint32_t n = other.size_;
        if (size_ < n) {
            if (buffer_ != nullptr) {
                delete[] buffer_;
                buffer_ = nullptr;
            }
            buffer_ = new (std::nothrow) std::uint8_t[n]();
        }
        if (buffer_ != nullptr) {
            std::memset(buffer_, 0, n);
            std::memcpy(buffer_, other.buffer_, other.size_);
            size_ = other.size_;
            read_pos_ = other.read_pos_;
            write_pos_ = other.write_pos_;
        }
    }
    return *this;
}

// 0x80e7f4 _ZN12NetBitStreamD2Ev
// 0x80e790 _ZN12NetBitStreamD1Ev
NetBitStream::~NetBitStream() {
    delete[] buffer_;
}

// 0x80f024 _ZN12NetBitStreameqERKS_
// Hex-Rays: memcmp(...) == 0 over the written bytes, so equal streams compare true.
bool NetBitStream::operator==(const NetBitStream& other) const {
    if (this == &other) {
        return true;
    }
    if (write_pos_ != other.write_pos_) {
        return false;
    }
    return std::memcmp(buffer_, other.buffer_, BytesFor(write_pos_)) == 0;
}

// 0x80e410 _ZN12NetBitStream14SetRevertPointEv
void NetBitStream::SetRevertPoint() {
    flags_ = 0;
    revert_read_ = read_pos_;
    revert_write_ = write_pos_;
}

// 0x80e83c _ZN12NetBitStream6RevertEv
void NetBitStream::Revert() {
    std::uint32_t bits = revert_write_;
    read_pos_ = revert_read_;
    write_pos_ = bits;
    flags_ = 0;
    std::uint32_t used = BytesFor(bits);
    if (used > 0) {
        // Keep only the written bits of the last partial byte.
        unsigned shift = (8u - (bits & 7u)) & 7u;
        buffer_[used - 1] &= static_cast<std::uint8_t>(0xffu << shift);
    }
    if (used < size_) {
        std::memset(buffer_ + used, 0, size_ - used);
    }
}

// 0x80e89c _ZN12NetBitStream5ResetEv
void NetBitStream::Reset() {
    write_pos_ = 0;
    read_pos_ = 0;
    std::memset(buffer_, 0, size_);
    flags_ = 0;
    revert_write_ = 0;
    revert_read_ = 0;
}

// 0x80e42c _ZN12NetBitStream8WriteBitEj
void NetBitStream::WriteBit(std::uint32_t bit) {
    if ((flags_ & kWriteOverflow) == 0 && write_pos_ < size_ * 8) {
        std::uint32_t pos = write_pos_;
        std::uint8_t mask = static_cast<std::uint8_t>(1u << (7 - (pos & 7)));
        if (bit != 0) {
            buffer_[pos >> 3] |= mask;
        } else {
            buffer_[pos >> 3] &= static_cast<std::uint8_t>(~mask);
        }
        write_pos_ = pos + 1;
    } else {
        flags_ |= kWriteOverflow;
    }
}

// 0x80e498 _ZN12NetBitStream7ReadBitEv
int NetBitStream::ReadBit() {
    std::uint32_t pos = read_pos_;
    if ((pos >> 3) < size_) {
        read_pos_ = pos + 1;
        return (buffer_[pos >> 3] >> (7 - (pos & 7))) & 1;
    }
    flags_ |= kReadOverflow;
    return 0;
}

// 0x80e4e8 _ZN12NetBitStream9WriteByteEhj
void NetBitStream::WriteByte(std::uint8_t value, std::uint32_t bits) {
    if ((flags_ & kWriteOverflow) == 0) {
        std::uint32_t pos = write_pos_;
        std::uint32_t avail = size_ * 8 - pos;
        // Original condition: bits <= avail and avail > 8.
        if (bits <= avail && avail > 8) {
            std::uint32_t i = pos >> 3;
            unsigned sh = pos & 7;
            std::uint8_t shifted = static_cast<std::uint8_t>((value << ((8 - bits) & 0xff)) & 0xff);
            buffer_[i] |= static_cast<std::uint8_t>(shifted >> sh);
            if (i + 1 < size_) {
                buffer_[i + 1] = static_cast<std::uint8_t>(shifted << ((8 - sh) & 0xff));
            }
            write_pos_ = pos + bits;
            return;
        }
    }
    flags_ |= kWriteOverflow;
}

// 0x80e564 _ZN12NetBitStream8ReadByteEj
std::uint32_t NetBitStream::ReadByte(std::uint32_t bits) {
    std::uint32_t pos = read_pos_;
    if (write_pos_ < pos + bits) {  // unsigned (write - read) < bits in the original
        flags_ |= kReadOverflow;
        return 0;
    }
    if (bits == 0) {
        return 0;
    }
    std::uint32_t i = pos >> 3;
    unsigned sh = pos & 7;
    std::uint32_t b0 = buffer_[i];
    std::uint32_t b1 = (i + 1 < size_) ? buffer_[i + 1] : 0;
    std::uint32_t window = ((b1 >> ((8 - sh) & 0xff)) | (b0 << sh)) & 0xff;
    std::uint32_t result = (window >> ((8 - bits) & 0xff)) & ((1u << bits) - 1);
    read_pos_ = pos + bits;
    return result;
}

// 0x80e5dc _ZN12NetBitStream8WriteU32Ejj
// Hex-Rays: ceil(bits / 8) calls to WriteByte, each with min(remaining, 8) bits.
void NetBitStream::WriteU32(std::uint32_t value, std::uint32_t bits) {
    std::uint32_t remaining = bits;
    while (remaining > 0) {
        std::uint32_t chunk = remaining < 8 ? remaining : 8;
        WriteByte(static_cast<std::uint8_t>(value & 0xff), chunk);
        value >>= 8;
        remaining -= chunk;
    }
}

// 0x80e630 _ZN12NetBitStream7ReadU32Ej
std::uint32_t NetBitStream::ReadU32(std::uint32_t bits) {
    std::uint32_t result = 0;
    std::uint32_t remaining = bits;
    std::uint32_t index = 0;
    while (remaining > 0) {
        std::uint32_t chunk = remaining < 8 ? remaining : 8;
        result |= ReadByte(chunk) << ((index & 0x1f) << 3);
        remaining -= chunk;
        ++index;
    }
    if (bits < 32) {
        result &= (1u << bits) - 1;
    }
    return result;
}

// 0x80eac8 _ZN12NetBitStream12ShiftMemCopyEPvjPKvji (write direction, a6 = shift >= 0)
// Partial: one function in the original, split here into CopyBitsIn and CopyBitsOut.
// Hex-Rays (re-run) argument roles: the write direction (shift >= 0) uses the bit count a5;
// the read direction (shift < 0) uses a3; shift 0 uses min(a5, a3). The bounds guards below
// are the port's addition (the original reads and writes one byte past the end).
// dst is the byte holding the next write bit, shift = that bit offset in the byte.
// shift 0: memcpy of ceil(bits/8) bytes, then the last byte keeps only its top bits.
// shift > 0: the first byte is OR-ed into the existing data, following bytes are
// assigned, as in the original. Bounds guards: the original writes one byte past the
// end when a write ends at the buffer end; that byte is skipped here.
void NetBitStream::CopyBitsIn(std::uint8_t* dst, std::uint8_t* dst_end, unsigned shift,
                              const std::uint8_t* src, std::uint32_t bits) {
    std::uint32_t count = BytesFor(bits);
    if (count == 0) {
        return;
    }
    if (shift == 0) {
        std::memcpy(dst, src, count);
        if ((bits & 7) != 0) {
            dst[count - 1] &= static_cast<std::uint8_t>(0xffu << (8 - (bits & 7)));
        }
        return;
    }
    std::uint8_t carry = dst < dst_end ? dst[0] : 0;
    for (std::uint32_t k = 0; k < count; ++k) {
        if (dst + k < dst_end) {
            dst[k] = static_cast<std::uint8_t>(carry | (src[k] >> shift));
        }
        carry = static_cast<std::uint8_t>((src[k] << (8 - shift)) & 0xff);
        if (dst + k + 1 < dst_end) {
            dst[k + 1] = carry;
        }
    }
}

// 0x80eac8 (read direction, a6 = -shift in the original): dst receives `bits` bits
// starting at src bit offset shift. shift 0: memcpy plus last-byte mask, as the
// original. shift > 0: each byte is (src[k] << shift) | (src[k+1] >> (8 - shift)), with
// no mask on the last byte (the original does not clear it). The byte after the
// last one is read only inside src_end (bounds guard, see CopyBitsIn).
void NetBitStream::CopyBitsOut(std::uint8_t* dst, const std::uint8_t* src, const std::uint8_t* src_end,
                               unsigned shift, std::uint32_t bits) {
    std::uint32_t count = BytesFor(bits);
    if (count == 0) {
        return;
    }
    if (shift == 0) {
        std::memcpy(dst, src, count);
        if ((bits & 7) != 0) {
            dst[count - 1] &= static_cast<std::uint8_t>(0xffu << (8 - (bits & 7)));
        }
        return;
    }
    for (std::uint32_t k = 0; k < count; ++k) {
        std::uint8_t lo = src[k];
        std::uint8_t hi = src + k + 1 < src_end ? src[k + 1] : 0;
        dst[k] = static_cast<std::uint8_t>((lo << shift) | (hi >> (8 - shift)));
    }
}

// 0x80ebc8 _ZN12NetBitStream21ReadByteArray_BitSizeEPvj
void NetBitStream::ReadByteArray_BitSize(void* dst, std::uint32_t bits) {
    if (write_pos_ - read_pos_ < bits) {
        flags_ |= kReadOverflow;
        return;
    }
    std::uint32_t pos = read_pos_;
    CopyBitsOut(static_cast<std::uint8_t*>(dst), buffer_ + (pos >> 3), buffer_ + size_, pos & 7, bits);
    read_pos_ = pos + bits;
}

// 0x80ec28 _ZN12NetBitStream13ReadByteArrayEPvj
void NetBitStream::ReadByteArray(void* dst, std::uint32_t bytes) {
    ReadByteArray_BitSize(dst, bytes << 3);
}

// 0x80ec30 _ZN12NetBitStream10ReadStringEv
std::string NetBitStream::ReadString() {
    std::uint32_t length = ReadByte(8);
    char text[260];
    std::memset(text, 0, sizeof(text));
    ReadByteArray(text, length);
    return std::string(text);
}

// 0x80ece8 _ZN12NetBitStream22WriteByteArray_BitSizeEPKvj
void NetBitStream::WriteByteArray_BitSize(const void* src, std::uint32_t bits) {
    if ((flags_ & kWriteOverflow) == 0) {
        std::uint32_t pos = write_pos_;
        std::uint32_t avail = size_ * 8 - pos;
        if (bits <= avail) {
            std::uint8_t* base = buffer_ + (pos >> 3);
            CopyBitsIn(base, buffer_ + size_, pos & 7, static_cast<const std::uint8_t*>(src), bits);
            write_pos_ = pos + bits;
            return;
        }
    }
    flags_ |= kWriteOverflow;
}

// 0x80eda8 _ZN12NetBitStream14WriteByteArrayEPKvj
void NetBitStream::WriteByteArray(const void* src, std::uint32_t bytes) {
    WriteByteArray_BitSize(src, bytes << 3);
}

// 0x80ed54 _ZN12NetBitStream11WriteStreamERS_
void NetBitStream::WriteStream(const NetBitStream& other) {
    WriteByteArray_BitSize(other.buffer_, other.write_pos_);
}

// 0x80edb0 _ZN12NetBitStream11WriteStringESsj
// Partial: Hex-Rays (re-run) takes the length from the string object's own fields (end minus
// start, capped by a second field when that is nonzero). The string layout is not recovered
// here, so the length is text.size(). The third argument is never read by the original body,
// so no cap is applied to it here (the earlier cap was a misreading). The length is written as
// one byte, then the bytes.
void NetBitStream::WriteString(const std::string& text, std::uint32_t max_len) {
    (void)max_len;
    std::uint32_t length = static_cast<std::uint32_t>(text.size());
    WriteByte(static_cast<std::uint8_t>(length & 0xff), 8);
    WriteByteArray(text.data(), length);
}

// 0x80ee0c _ZN12NetBitStream9GetBufferEPcj
// The second argument is not used by the original body (the copy length is the write position).
void NetBitStream::GetBuffer(char* out, std::uint32_t) {
    std::memcpy(out, buffer_, BytesFor(write_pos_));
}

// 0x80ee3c _ZN12NetBitStream9SetBufferEPKcj
// The original copies the bytes and sets the write position to 8 * length (the store at +0x10 of
// the disassembly). The copy is clamped to the buffer here, so the position is clamped too.
void NetBitStream::SetBuffer(const char* src, std::size_t length) {
    if (length > size_) {
        length = size_;
    }
    std::memcpy(buffer_, src, length);
    write_pos_ = static_cast<std::uint32_t>(length * 8);
}
