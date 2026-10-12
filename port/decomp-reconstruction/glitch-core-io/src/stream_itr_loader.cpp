#include "glitch-core-io/io/stream_itr_loader.hpp"

#include <cstring>

namespace glitch::io {

// 0x6b5b74 _ZN6glitch2io16SStreamItrLoader14loadAndAdvanceEPNS0_9IReadFileEb
void SStreamItrLoader::loadAndAdvance(IReadFile* file, bool swapBytes) {
    if (swapBytes && count > 0 && (elementSize == 2 || elementSize == 4 || elementSize == 8)) {
        for (std::uint16_t i = 0; i < count; ++i) {
            file->read(readBuffer, elementSize);
            // Big-endian bytes to a host value: the first byte read is the most significant.
            std::uint64_t value = 0;
            for (std::uint16_t b = 0; b < elementSize; ++b) {
                value = (value << 8) | readBuffer[b];
            }
            // Store the value in the element width (the original writes 2, 4 or 8 bytes).
            std::uint8_t* dest = data + static_cast<std::size_t>(i) * elementSize;
            std::memcpy(dest, &value, elementSize);
        }
    }
    data += stride;
}

}  // namespace glitch::io
