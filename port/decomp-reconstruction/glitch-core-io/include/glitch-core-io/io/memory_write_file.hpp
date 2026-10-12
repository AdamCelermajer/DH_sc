// glitch::io::CMemoryWriteFile: a write target backed by an in-memory byte
// vector. Layout from the original: +0x08 position, +0x0c..+0x14 buffer vector.
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::io {

class CMemoryWriteFile : public glitch::IReferenceCounted {
public:
    // +0x08: current position
    std::int32_t position = 0;
    // +0x0c..+0x14: buffer (begin, end, capacity end in the original)
    std::vector<std::uint8_t> buffer;

    explicit CMemoryWriteFile(std::uint32_t reserve);  // 0x6b4d04 (C1), 0x6b4d58 (C2)

    void flush() {}                                    // 0x6b4c60
    std::int32_t getPos() const { return position; }   // 0x6b4c64
    const char* getFileName() const { return "<memory>"; }  // 0x6b4c6c
    const char* getFullPath() const { return "<memory>"; }  // 0x6b4c7c
    const std::uint8_t* getBuffer() const { return buffer.data(); }  // 0x6b4c8c
    std::int32_t getBufferSize() const { return static_cast<std::int32_t>(buffer.size()); }  // 0x6b4c94

    bool seek(std::int32_t offset, bool relative);     // 0x6b4dac (partial)
    bool write(const void* data, std::uint32_t size);  // 0x6b4e44 (partial)
};

}  // namespace glitch::io
