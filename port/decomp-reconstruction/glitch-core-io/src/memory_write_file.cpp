#include "glitch-core-io/io/memory_write_file.hpp"

#include <cstring>

namespace glitch::io {

// 0x6b4d04 _ZN6glitch2io16CMemoryWriteFileC1Ej
CMemoryWriteFile::CMemoryWriteFile(std::uint32_t reserve) {
    buffer.reserve(reserve);
}

// 0x6b4dac _ZN6glitch2io16CMemoryWriteFile4seekElb
// PARTIAL: the pseudocode only has the relative branch. The absolute case
// (relative == false) does nothing here and is not confirmed.
// Moving before the start clamps to 0. Moving past the end grows the buffer
// to pos + 1 bytes, reserving (pos + 1) * 2 first when capacity is short.
bool CMemoryWriteFile::seek(std::int32_t offset, bool relative) {
    if (relative) {
        std::int32_t newPos = position + offset;
        if (newPos < 0) {
            position = 0;
        } else {
            position = newPos;
            if (static_cast<std::int32_t>(buffer.size()) < newPos) {
                if (static_cast<std::int32_t>(buffer.capacity()) < newPos) {
                    buffer.reserve(static_cast<std::size_t>(newPos + 1) * 2);
                }
                buffer.resize(static_cast<std::size_t>(position) + 1);
            }
        }
    }
    return true;
}

// 0x6b4e44 _ZN6glitch2io16CMemoryWriteFile5writeEPKvj
// PARTIAL: the pseudocode copies the data at the current position but never
// advances position afterwards. Kept as shown; to be confirmed against the
// binary.
bool CMemoryWriteFile::write(const void* data, std::uint32_t size) {
    std::size_t needed = static_cast<std::size_t>(position) + size;
    if (buffer.size() < needed) {
        if (buffer.capacity() < needed) {
            buffer.reserve(needed * 2);
            needed = static_cast<std::size_t>(position) + size;
        }
        buffer.resize(needed);
    }
    std::memcpy(buffer.data() + position, data, size);
    return true;
}

}  // namespace glitch::io
