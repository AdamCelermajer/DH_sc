// glitch::video::CDriverBinding: the per-draw-source slot a driver keeps for process buffers (IVideoDriver::ensureBinding
// creates one through createBinding, vtable +0x1f8, and getProcessBuffer reuses its buffer when the request repeats).
// Canonical definition (engine unification wave u1-video); the forward declarations elsewhere are replaced by this
// header.
//
// Layout of the original object (32-bit, 0x24 bytes; IVideoDriver::createBinding 0x5aa2e0, getProcessBuffer<IVideoDriver>
// 0x5abfa8):
//   +0x00  vptr (vtable 0x98ae50: ~CDriverBinding D1 0x6dc71c, D0 0x6dc7a4, and an onDelete override 0x6dc684 that is empty)
//   +0x04  intrusive_ptr<IBuffer> process buffer (zero after construction)
//   +0x08  last request: vertex count      +0x0c  last request: index count
//   +0x10  last request: CVertexStreams*   +0x14  uint16, last request: attribute word (-1 style defaults)
//   +0x18  zero after construction         +0x1c  zero after construction
//   +0x20  IVideoDriver*
// The reference count is not the first member: CDriverBinding is IReferenceCounted-shaped (vptr, onDelete) but keeps
// the process buffer at +0x04, so it does NOT derive from glitch::IReferenceCounted; a binding is owned by the
// stream table that created it and destroyed through its virtual destructor.
#pragma once

#include <cstdint>

#include "glitch-video/buffer.hpp"
#include "glitch-video/video_types.hpp"

namespace glitch::video {

class CDriverBinding {
public:
    explicit CDriverBinding(IVideoDriver* driver) : m_driver(driver) {}
    virtual ~CDriverBinding() = default;  // 0x6dc71c (D1) / 0x6dc7c0 (D2) / 0x6dc7a4 (D0)
    virtual void onDelete() {}            // +0x08, 0x6dc684 (empty)

    // 0x5abfa8 getProcessBuffer<IVideoDriver>(IVideoDriver*, uint, uint, uint, intrusive_ptr<CVertexStreams> const&, bool):
    // reuses the buffer of the last request when the request repeats, otherwise gets a new one from the driver's process
    // buffer heap. Declared only (needs the heap of IVideoDriver::getProcessBuffer).
    template <typename Driver>
    E_DRIVER_ALLOCATION_RESULT getProcessBuffer(Driver* driver, uint32_t vertexCount, uint32_t indexCount, uint32_t size,
                                                const boost::intrusive_ptr<CVertexStreams>& streams, bool create);

    boost::intrusive_ptr<IBuffer> m_processBuffer;  // +0x04
    uint32_t m_lastVertexCount = 0;                  // +0x08
    uint32_t m_lastIndexCount = 0;                   // +0x0c
    const CVertexStreams* m_lastStreams = nullptr;   // +0x10
    uint16_t m_lastAttributes = 0;                   // +0x14
    uint32_t unk_18 = 0;                             // +0x18
    uint32_t unk_1c = 0;                             // +0x1c
    IVideoDriver* m_driver = nullptr;                // +0x20
};

}  // namespace glitch::video
