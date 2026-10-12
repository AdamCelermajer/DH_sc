#include "glitch-core-io/io/stream_itr_writer.hpp"

#include <cstring>

namespace glitch::io {

// 0x6b5990 _ZN6glitch2io16SStreamItrWriter15writeAndAdvanceEPNS0_10IWriteFileEb
// PARTIAL: see the header for the zero-write return value.
std::size_t SStreamItrWriter::writeAndAdvance(IWriteFile* file, bool swapBytes) {
    std::size_t result = 0;
    const unsigned int size = elementSize;
    if (!swapBytes || size <= 1) {
        // Block write: count * size bytes from data, with no swapping.
        result = file->write(data, static_cast<unsigned int>(count) * size);
    } else if (size == 2 || size == 4 || size == 8) {
        for (unsigned int i = 0; i < count; ++i) {
            // Copy one element into the scratch buffer, then write it with its bytes reversed.
            std::memcpy(swapBuffer, data + i * size, size);
            std::uint8_t reversed[8];
            for (unsigned int b = 0; b < size; ++b) reversed[b] = swapBuffer[size - 1 - b];
            std::memcpy(swapBuffer, reversed, size);
            result = file->write(swapBuffer, size);
        }
    }
    data += stride;  // the original adds the stride to the pointer after every call
    return result;
}

}  // namespace glitch::io
