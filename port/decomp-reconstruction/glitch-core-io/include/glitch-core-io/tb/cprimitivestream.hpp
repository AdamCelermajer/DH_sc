// glitch::video::CPrimitiveStream, io view. The class is declared nowhere in glitch-core-io, so
// this header holds the fields the io code reads and writes, at the original's 32-bit offsets
// (comments only: the port's layout is its own). The owner is glitch-video (primitive_stream.hpp,
// which lays out only m_count and the primitive type); getPrimitiveCount is defined there.
#pragma once

#include <cstdint>

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-core-io/deps/glitch_core_objects.hpp"

namespace glitch::video {

class CPrimitiveStream {
public:
    // 0x5a0300 (glitch-video): primitives for the stream's element count, by primitive type.
    std::uint32_t getPrimitiveCount() const;

    boost::intrusive_ptr<IBuffer> buffer;  // +0x00: the buffer; its data (+0x08 of the IBuffer) is the stream base
    std::uint32_t offset = 0;              // +0x04: byte offset of the stream within buffer->data
    std::uint32_t count = 0;               // +0x08: element count (indices)
    std::uint32_t unk_0c = 0;              // +0x0c: header word written by save and read by loadPS (meaning not identified)
    std::uint32_t unk_10 = 0;              // +0x10: header word, as unk_0c
    std::uint16_t indexType = 0;           // +0x14: 0 none, 1 16-bit indices, 2 32-bit indices
    std::uint16_t primitiveType = 0;       // +0x16: E_PRIMITIVE_TYPE
};

}  // namespace glitch::video
