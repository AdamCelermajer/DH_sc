#include "glitch-video/buffer.hpp"

#include <cstring>

namespace glitch::video {

// 0x5a1908 _ZN6glitch5video7IBufferC2ENS0_13E_BUFFER_TYPEENS0_14E_BUFFER_USAGEEjPvh
// 0x5a197c _ZN6glitch5video7IBufferC1ENS0_13E_BUFFER_TYPEENS0_14E_BUFFER_USAGEEjPvh
// Usage value 4 is excluded from the "has data" flag; the repo does not name it.
IBuffer::IBuffer(E_BUFFER_TYPE type, E_BUFFER_USAGE usage, uint32_t size, void* data, uint8_t flags)
    : m_data(static_cast<uint8_t*>(data)),
      m_size(size),
      m_type(static_cast<uint8_t>(type)),
      m_usage(static_cast<uint8_t>(usage)),
      m_flags12(flags) {
    if (size != 0 && usage != 4 && data != nullptr) {
        m_flags12 = static_cast<uint8_t>(flags | 2);
    }
}

// 0x5a1c1c _ZN6glitch5video7IBuffer4copyEv
void IBuffer::copy() {
    if (((m_flags12 & 1) == 0 || m_data == nullptr) && m_size != 0) {
        uint8_t* src = m_data;
        uint8_t* dest = new uint8_t[m_size];
        m_data = dest;
        if (src != nullptr) {
            std::memcpy(dest, src, m_size);
        }
    }
}

// 0x637a68 _ZNK6glitch5video7IBuffer5unmapEv
void IBuffer::unmap() {
    uint8_t count = m_mapCount13;
    if ((count & 0x1f) > 1) {
        m_mapCount13 = static_cast<uint8_t>(((count & 0x1f) - 1) | (count & 0xe0));
        return;
    }
    if ((m_flags12 & 0x20) != 0) {
        // Unmap callback (vtable +0x18) is not modelled.
    }
    m_mapCount13 = 0;
}

}  // namespace glitch::video
