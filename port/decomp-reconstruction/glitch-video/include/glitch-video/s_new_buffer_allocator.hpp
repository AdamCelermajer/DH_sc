// glitch::video::detail::SNewBufferAllocator<Driver>: replaces the storage of a
// buffer with a fresh heap allocation. Only the single intrusive pointer that the
// original reads at +0x00 is modelled.
#pragma once

#include <cstdint>

#include "glitch-video/buffer.hpp"
#include "glitch-video/deps/intrusive_ptr.hpp"

namespace glitch::video {

class IVideoDriver;

namespace detail {

template <typename Driver>
struct SNewBufferAllocator {
    boost::intrusive_ptr<IBuffer> m_buffer;  // +0x00, the buffer whose storage is replaced

    // 0x5aa454 _ZNK6glitch5video6detail19SNewBufferAllocatorINS0_12IVideoDriverEE8allocateEj
    // Allocates size bytes, resets the buffer to them (owned), and returns a new reference.
    // The original does not test m_buffer for null before the reset call.
    boost::intrusive_ptr<IBuffer> allocate(uint32_t size) const;
};

}  // namespace detail

}  // namespace glitch::video
