// glitch::video::CVertexStreams: the vertex stream table of a mesh. The
// original stores 16-byte SVertexStream entries in an inline-or-heap array
// with begin at +0x14 and end pointer at +0x10. Here the array is a vector;
// offsets are kept as comments.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-video/buffer.hpp"
#include "glitch-video/deps/intrusive_ptr.hpp"

namespace glitch::video {

// 16 bytes in the original.
struct SVertexStream {
    boost::intrusive_ptr<IBuffer> buffer;  // +0x00
    uint32_t offset = 0;                   // +0x04, changed by CVertexStreams::addOffset
    uint16_t attribute = 0;                // +0x08, E_VERTEX_ATTRIBUTE index
    uint16_t field0a = 0;                  // +0x0a, copied by setStream; meaning not identified
    uint16_t field0c = 0;                  // +0x0c, copied by setStream; meaning not identified
    uint16_t field0e = 0;                  // +0x0e, copied by setStream; meaning not identified

    // 0x62fe18 SVertexStream::unmapBuffer() const: releases one map reference on buffer.
    // Partial: IBuffer::unmap does not call the unmap callback (vtable +0x18).
    void unmapBuffer() const;

    // Mapped view of one stream (SVertexStream::SMapBuffer<T> in the original). The
    // original stores the stream pointer at +0x00 (the buffer is its first word) and
    // the mapped pointer, already offset by the stream offset, at +0x04.
    template <typename T>
    struct SMapBuffer {
        SVertexStream* stream = nullptr;  // +0x00
        T* data = nullptr;                // +0x04

        // 0x5a0e28 (T = void const): releases the mapping, if any.
        // Partial: see unmapBuffer.
        void reset() {
            if (data != nullptr) {
                stream->buffer->unmap();
                stream = nullptr;
                data = nullptr;
            }
        }

        // 0x6d6fe0 (vector2d<float>), 0x6d7088 (vector3d<float>), 0x6d7130 (SColor):
        // releases the old mapping, then maps stream with access. Returns the base
        // pointer of the mapping (before the stream offset is added). Partial: see
        // unmapBuffer. The original adds the offset to a null base as well; this
        // port keeps data null in that case.
        uint8_t* reset(SVertexStream& s, int32_t access) {
            reset();
            stream = &s;
            uint8_t* base = s.buffer->map(access);
            data = (base != nullptr) ? reinterpret_cast<T*>(base + s.offset) : nullptr;
            return base;
        }
    };
};

// Stream description passed to CVertexStreams::setStream (SVertexStreamData in the original).
struct SVertexStreamData {
    boost::intrusive_ptr<IBuffer> buffer;  // +0x00
    uint32_t offset = 0;                   // +0x04
    uint16_t field08 = 0;                  // +0x08, copied to SVertexStream::field0a
    uint16_t field0a = 0;                  // +0x0a, not read by setStream
    uint16_t field0c = 0;                  // +0x0c, copied to SVertexStream::field0c
    uint16_t field0e = 0;                  // +0x0e, copied to SVertexStream::field0e

    // 0x647f54 SVertexStreamData::operator=(SVertexStream&): copies buffer, offset and the
    // three fields from the stream. field0a is not written. Partial: the buffer is an
    // intrusive_ptr copy, without the reference-count updates of the original.
    SVertexStreamData& operator=(const SVertexStream& stream);
};

class CVertexStreams {
public:
    CVertexStreams() = default;

    // 0x5a113c (C1) / 0x5a148c (C2): one constructor body. Partial: the trailing block
    // of 24 * count0c + 24 bytes (copied from the caller's vector3d array, or zeroed)
    // is not modelled, and the buffers are not reference counted. A source table copies
    // buffer, offset and attribute; without one, the attributes come from the mask bits
    // in order, with field0a set to 0xff.
    CVertexStreams(uint32_t field08, uint32_t attributeMask, uint8_t count0c, uint8_t streamCount,
                   const SVertexStream* source);

    // 0x5a09e0 (D2): the original drops the buffer of each entry. Partial: no reference
    // counts are kept, so there is nothing to drop; the vector frees the entries.
    ~CVertexStreams();

    // 0x5a1270 (7-argument allocate): creates a table. Partial: the original returns it
    // through an out pointer with a reference count and the trailing block is not kept.
    static boost::intrusive_ptr<CVertexStreams> allocate(uint32_t field08, uint32_t attributeMask,
                                                         uint8_t count0c, uint8_t streamCount,
                                                         const SVertexStream* source);

    // 0x5a135c (allocate(unsigned int)): table for the mask. Bit 0 is always set. count0c
    // is the number of set bits in 1..16 and streamCount the number of set bits. Partial, as above.
    static boost::intrusive_ptr<CVertexStreams> allocate(uint32_t attributeMask);

    // 0x5a1404 (allocate(unsigned char, unsigned int)): bits 1..count are set in addition to the mask.
    // Partial, as above.
    static boost::intrusive_ptr<CVertexStreams> allocate(uint8_t count, uint32_t attributeMask);

    // 0x5a1310 (clone): copies the table and the flags. Partial, as above.
    boost::intrusive_ptr<CVertexStreams> clone() const;

    // 0x5a0c60: copies the masked streams from source into this table, with offsetDelta
    // added to each copied offset, then recomputes homogeneity with flag. Returns the mask
    // of copied attributes. Partial: buffers are not reference counted.
    uint32_t setStreams(const boost::intrusive_ptr<CVertexStreams>& source, uint32_t mask, int32_t offsetDelta,
                        bool flag);

    // 0x5a178c: assigns each entry from data (in order) when its attribute is in the mask,
    // clears the others, then recomputes homogeneity with flag. Returns the masked attributes.
    // Partial: buffers are not reference counted.
    uint32_t setupStreams(const SVertexStreamData* data, uint32_t mask, bool flag);

    // 0x7d46fc: copies the description into one stream entry, then recomputes
    // homogeneity with flag. Partial: the original takes a reference on the new
    // buffer and drops the old one; the intrusive_ptr stand-in does neither.
    void setStream(SVertexStream* stream, const SVertexStreamData& data, bool flag);

    // 0x5a0a58: bit mask of the attributes whose stream has a buffer.
    uint32_t getActiveAttributeMask() const;

    // 0x5a0aac / 0x5a0af0: first stream in [first, end) with attribute >= wanted; the
    // end pointer when the attribute is not present.
    const SVertexStream* getStream(uint32_t attribute, const SVertexStream* first, const SVertexStream* end) const;
    SVertexStream* getStream(uint32_t attribute, SVertexStream* first, SVertexStream* end);

    // 0x5a0b34: adds delta to the offset of every stream.
    void addOffset(int32_t delta);

    // 0x5a0b74: true when every attribute in mask (restricted to the attribute
    // mask +0x04) reads from the same buffer.
    bool isHomogeneous(uint32_t mask) const;

    // 0x5a0bfc: flag == false sets kFlagHomogeneous when no two neighbouring
    // streams hold different non-null buffers. flag == true clears it.
    void updateHomogeneityInternal(bool flag);

    // 0x5a0d80: rebuilds m_flags. Sets kFlagHasData on entry, then clears it when
    // a bound buffer has no data pointer. Homogeneity follows the same rule as
    // updateHomogeneityInternal.
    void updateStatesInternal(bool flag);

    // 0x5a0e24: thunk, `b #0x5a0d80`. Forwards to updateStatesInternal.
    void updateStates(bool flag);

    // Bits of m_flags, named by what the code tests them for.
    static constexpr uint16_t kFlagHomogeneous = 0x0001;  // bit 0, every bound stream shares one buffer
    static constexpr uint16_t kFlagHasData = 0x0002;      // bit 1, set on entry, cleared on a data-less buffer

    std::vector<SVertexStream> m_streams;  // +0x10..+0x24 (inline array in the original)
    uint32_t m_attributeMask = 0;          // +0x04, attributes present in the table
    uint32_t m_field08 = 0;                // +0x08, set by the constructor; meaning not identified
    uint8_t m_count0c = 0;                 // +0x0c, count of the trailing block (24 * count + 24 bytes)
    uint8_t m_streamCount0d = 0;           // +0x0d, number of stream entries
    uint16_t m_flags = 0;                  // +0x0e, see kFlag* above
};

namespace detail {

// .rodata table at 0x8e4ca4 in libDungeonHunter2.so (7 bytes, read from the ELF):
// element size in bytes of each vertex attribute value type: 1 1 2 2 4 4 4.
struct SVertexAttributeTypeInspection {
    static const uint8_t ValueTypeSize[7];
};

}  // namespace detail

// 0x5a2b18 copyVertex(vertex, src, dst, base, out, wholeVertex). With wholeVertex false, each
// stream of src copies count * ValueTypeSize[type] bytes of vertex number `vertex` from its
// mapped buffer (offset + stride * vertex) to out, and advances out. dst is not used by the
// original. Partial: the wholeVertex branch (one memcpy of the table's vertex size, a
// CVertexStreams word at +0x22 that is not in this layout) is not written. The buffers are
// not reference counted, and the mapping is not released, as in the original.
void copyVertex(uint16_t vertex, const boost::intrusive_ptr<const CVertexStreams>& src,
                const boost::intrusive_ptr<CVertexStreams>& dst, const uint8_t* base, uint8_t*& out,
                bool wholeVertex);

// 0x5a08f4: fills the 30-byte attribute map (0xff = unset) with the index of each
// stream whose attribute is the map slot. Attributes past the map are skipped here;
// the original writes without a check.
uint8_t* makeDefaultAttributeMap(const CVertexStreams* streams, uint8_t* map);

}  // namespace glitch::video
