// Batch e1-glitch-video-glitch_video_CPrimitiveStream_SMapBuffer_unsigne-0: one function, the reset clone
// of CPrimitiveStream::SMapBuffer<unsigned short const>. See the header for the layout and the partial notes.
#include "glitch-video/tb/cprimitivestream_smapbuffer_ushort.hpp"

#include <cstdint>

namespace glitch::video::tb {

// 0x5911a0 _ZN6glitch5video16CPrimitiveStream10SMapBufferIKtE5resetERKS1_NS0_24E_BUFFER_READ_MAP_ACCESSE.clone.2
// Access is 1 in this clone (E_BUFFER_READ_MAP_ACCESS value passed as MOV R1, #1).
uint8_t* CPrimitiveStreamSMapBufferU16::reset(const boost::intrusive_ptr<IBuffer>* streamBuffer,
                                              uint32_t streamOffset) {
    constexpr int32_t kAccess = 1;

    // Release the current mapping (only when data is set; the stream slot alone is not enough).
    if (data != nullptr) {
        IBuffer* buffer = stream->get();  // **this: the buffer of the old stream, read now
        const uint8_t count = buffer->m_mapCount13;
        if ((count & 0x1f) <= 1) {
            // Partial: the unmap callback (vtable +0x18) runs here when the buffer flag 0x20 is set.
            // IBuffer does not name that slot, so it is skipped.
            buffer->m_mapCount13 = 0;
        } else {
            buffer->m_mapCount13 = static_cast<uint8_t>(((count & 0x1f) - 1) | (count & 0xe0));
        }
        stream = nullptr;
        data = nullptr;
    }

    // Bind the new stream, map its buffer, and offset the mapped pointer by the stream offset.
    stream = streamBuffer;
    uint8_t* base = (*streamBuffer)->map(kAccess);
    data = (base != nullptr) ? reinterpret_cast<const uint16_t*>(base + streamOffset) : nullptr;
    return base;
}

}  // namespace glitch::video::tb
