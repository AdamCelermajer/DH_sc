// glitch::io::SStreamItrLoader: reads one run of fixed-size big-endian elements from a read file
// into host-order storage (byte-reversing elements of 2, 4 and 8 bytes), then advances the
// destination pointer by the stride. The mirror of SStreamItrWriter.
// Layout: the 8-byte read buffer at +0x00 is the first member, the rest follow the original offsets.
#pragma once

#include <cstdint>

#include "glitch-core-io/io/read_file.hpp"

namespace glitch::io {

struct SStreamItrLoader {
    std::uint8_t readBuffer[8];   // +0x00: scratch for one element as read from the file
    std::uint8_t* data = nullptr; // +0x10: destination run; advanced by stride after each call
    std::uint16_t count = 0;      // +0x14: number of elements to read
    std::uint16_t elementSize = 0;  // +0x16: bytes per element (2, 4 and 8 are read; others are not)
    std::uint16_t stride = 0;     // +0x18: bytes to advance data after the call

    // 0x6b5b74 _ZN6glitch2io16SStreamItrLoader14loadAndAdvanceEPNS0_9IReadFileEb
    // When swapBytes is set, elements of 2, 4 and 8 bytes are read one at a time and stored
    // from big-endian to host order. When it is clear, or the element size is 0, 1 or another
    // unlisted size, nothing is read. The destination advances by stride in every case.
    // PARTIAL: the caller's block read for the clear case is not in this listing.
    void loadAndAdvance(IReadFile* file, bool swapBytes);
};

}  // namespace glitch::io
