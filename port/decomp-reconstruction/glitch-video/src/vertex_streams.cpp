#include "glitch-video/vertex_streams.hpp"

namespace glitch::video {

// 0x5a0a58 _ZNK6glitch5video14CVertexStreams22getActiveAttributeMaskEv
uint32_t CVertexStreams::getActiveAttributeMask() const {
    uint32_t mask = 0;
    for (const SVertexStream& s : m_streams) {
        if (s.buffer.get() != nullptr) {
            mask |= 1u << (s.attribute & 0xff);
        }
    }
    return mask;
}

// 0x5a0aac _ZNK6glitch5video14CVertexStreams9getStreamENS0_18E_VERTEX_ATTRIBUTEEPKNS0_13SVertexStreamES5_
const SVertexStream* CVertexStreams::getStream(uint32_t attribute, const SVertexStream* first,
                                               const SVertexStream* end) const {
    for (const SVertexStream* p = first; p != end; ++p) {
        if (static_cast<int>(attribute) <= static_cast<int>(p->attribute)) {
            return attribute == p->attribute ? p : end;
        }
    }
    return end;
}

// 0x5a0af0 _ZN6glitch5video14CVertexStreams9getStreamENS0_18E_VERTEX_ATTRIBUTEEPNS0_13SVertexStreamES4_
SVertexStream* CVertexStreams::getStream(uint32_t attribute, SVertexStream* first, SVertexStream* end) {
    for (SVertexStream* p = first; p != end; ++p) {
        if (static_cast<int>(attribute) <= static_cast<int>(p->attribute)) {
            return attribute == p->attribute ? p : end;
        }
    }
    return end;
}

// 0x5a0b34 _ZN6glitch5video14CVertexStreams9addOffsetEi
void CVertexStreams::addOffset(int32_t delta) {
    for (SVertexStream& s : m_streams) {
        s.offset += static_cast<uint32_t>(delta);
    }
}

// 0x5a0b74 _ZNK6glitch5video14CVertexStreams13isHomogeneousEj
// The original scans the bits of (mask & +0x04) and reads the buffer of each
// matching stream. When a lookup misses, the original reads the sentinel
// after the array; here a miss is treated as a null buffer. Partial for that
// reason.
bool CVertexStreams::isHomogeneous(uint32_t mask) const {
    mask &= m_attributeMask;
    if (mask == 0) return true;
    const SVertexStream* first = m_streams.data();
    const SVertexStream* end = first + m_streams.size();
    IBuffer* common = nullptr;
    for (uint32_t bit = 0; mask != 0; ++bit) {
        uint32_t b = 1u << (bit & 0x1f);
        if ((b & mask) != 0) {
            const SVertexStream* found = getStream(bit, first, end);
            IBuffer* buf = (found == end) ? nullptr : found->buffer.get();
            if (common == nullptr) {
                common = buf;
            } else if (common != buf) {
                return false;
            }
            if (found != end) first = found + 1;  // original advances past the sentinel; clamped here
            mask &= ~b;
        }
    }
    return true;
}

// 0x5a0bfc _ZN6glitch5video14CVertexStreams25updateHomogeneityInternalEb
// Only consecutive pairs of non-null buffers are compared, so a null entry
// between two streams never makes the table inhomogeneous. With no streams the
// original reads past the end of its inline array; this port treats the table
// as homogeneous, which is the only reading that does not touch memory it does
// not own.
void CVertexStreams::updateHomogeneityInternal(bool flag) {
    if (!flag) {
        bool homogeneous = true;
        for (size_t i = 0; i + 1 < m_streams.size(); ++i) {
            IBuffer* a = m_streams[i].buffer.get();
            IBuffer* b = m_streams[i + 1].buffer.get();
            if (a != nullptr && b != nullptr && a != b) {
                homogeneous = false;
                break;
            }
        }
        if (homogeneous) {
            m_flags = static_cast<uint16_t>(m_flags | kFlagHomogeneous);
            return;
        }
    }
    m_flags = static_cast<uint16_t>(m_flags & ~kFlagHomogeneous);
}

// 0x5a0d80 _ZN6glitch5video14CVertexStreams20updateStatesInternalEb
void CVertexStreams::updateStatesInternal(bool flag) {
    uint16_t flags = flag ? static_cast<uint16_t>(m_flags & ~kFlagHomogeneous)
                          : static_cast<uint16_t>(m_flags | kFlagHomogeneous);
    m_flags = static_cast<uint16_t>(flags | kFlagHasData);

    IBuffer* prev = nullptr;
    for (const SVertexStream& s : m_streams) {
        IBuffer* cur = s.buffer.get();
        if (cur != nullptr) {
            if (cur->m_data == nullptr) {
                m_flags = static_cast<uint16_t>(m_flags & ~kFlagHasData);
            }
            if (prev != nullptr && prev != cur) {
                m_flags = static_cast<uint16_t>(m_flags & ~kFlagHomogeneous);
            }
        }
        prev = cur;
    }
}

// 0x7d46fc _ZN6glitch5video14CVertexStreams9setStreamEPNS0_13SVertexStreamERKNS0_17SVertexStreamDataEb
void CVertexStreams::setStream(SVertexStream* stream, const SVertexStreamData& data, bool flag) {
    stream->buffer = data.buffer;
    stream->offset = data.offset;
    stream->field0a = data.field08;
    stream->field0c = data.field0c;
    stream->field0e = data.field0e;
    updateHomogeneityInternal(flag);
}

// 0x5a0e24 _ZN6glitch5video14CVertexStreams12updateStatesEb
// Thunk: the ARM body is `b #0x5a0d80`, so this forwards to updateStatesInternal.
void CVertexStreams::updateStates(bool flag) { updateStatesInternal(flag); }

}  // namespace glitch::video
