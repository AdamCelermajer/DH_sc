#include "glitch-video/s_new_buffer_allocator.hpp"

namespace glitch::video::detail {

// 0x5aa454 _ZNK6glitch5video6detail19SNewBufferAllocatorINS0_12IVideoDriverEE8allocateEj
template <typename Driver>
boost::intrusive_ptr<IBuffer> SNewBufferAllocator<Driver>::allocate(uint32_t size) const {
    IBuffer* buffer = m_buffer.get();
    uint8_t* data = new uint8_t[size];
    buffer->reset(size, data, true);

    // The original bumps the reference count of the buffer it returns, when there is one.
    IBuffer* result = m_buffer.get();
    if (result != nullptr) {
        ++result->m_refCount;
    }
    return boost::intrusive_ptr<IBuffer>(result);
}

// The only instance the original uses.
template struct SNewBufferAllocator<IVideoDriver>;

}  // namespace glitch::video::detail
