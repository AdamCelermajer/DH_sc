// glitch::video::CPrimitiveStream::SMapBuffer<unsigned short const>: the mapped view of a
// primitive stream's index buffer. The original nests it in CPrimitiveStream; primitive_stream.hpp
// does not declare it yet, so it lives here as a standalone type until the layout request
// (see the notes of the batch) is applied.
//
// Layout of the original (from the reset clone 0x5911a0, read from the assembly):
//   +0x00  pointer to the bound stream. The stream's first word is its index buffer
//          (IBuffer*), so **this is the buffer. The port stores the address of that
//          buffer slot instead: the same word, read at the same time.
//   +0x04  mapped pointer, already offset by the stream's offset (stream word +0x04).
#pragma once

#include <cstdint>

#include "glitch-video/buffer.hpp"
#include "glitch-video/deps/intrusive_ptr.hpp"

namespace glitch::video::tb {

struct CPrimitiveStreamSMapBufferU16 {
    const boost::intrusive_ptr<IBuffer>* stream = nullptr;  // +0x00, slot of the stream's index buffer
    const uint16_t* data = nullptr;                          // +0x04, mapped base plus the stream offset

    // 0x5911a0 reset(CPrimitiveStream const&, E_BUFFER_READ_MAP_ACCESS) [clone.2]: the clone with
    // the access argument fixed to 1 (assembly: MOV R1, #1 before IBuffer::map). Releases the
    // current mapping, if any, then maps the buffer in streamBuffer with access 1 and returns the
    // base pointer of the mapping. data becomes base + streamOffset.
    // Partial: (1) the unmap callback (buffer vtable +0x18, taken when the buffer's flag 0x20 is
    // set and the map count is 1 or less) is not called, because IBuffer has no slot for it.
    // (2) The original adds the offset even to a null base. This port keeps data null in that
    // case, as vertex_streams.hpp does. (3) The stream is taken as its buffer slot and offset
    // because CPrimitiveStream does not name its +0x00 buffer and +0x04 offset yet, so the
    // CPrimitiveStream const& overload is not declared here.
    uint8_t* reset(const boost::intrusive_ptr<IBuffer>* streamBuffer, uint32_t streamOffset);
};

}  // namespace glitch::video::tb
