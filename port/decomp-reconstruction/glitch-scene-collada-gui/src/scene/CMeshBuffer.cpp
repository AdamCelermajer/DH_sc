// Bodies of glitch::scene::CMeshBuffer (canonical class: scene/CMeshBuffer.hpp). This file replaces
// src/tb_scene-collada-gui-glitch_scene_CMeshBuffer-0.cpp, which wrote against the 32-bit layout struct of layouts/scene.hpp.
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"

namespace glitch::scene {

// 0x6bcf80 _ZN6glitch5scene11CMeshBufferC2EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigES9_b (base object)
// 0x6bd9f8 (C1, skipped: identical)
// PARTIAL. Reconstructed: the prologue clears the words the original clears (+0x20 reference count, +0x04, +0x08, +0x0c,
// +0x10, +0x14, +0x1c, +0x30, +0x34) and this port stores a 0 count for the rest.
// The listing maps the collada records the rest of the body walks (verified against Hex-Rays 0x6bcf80):
//   collada::SMesh:        +0x00 the buffer count, +0x04 SMeshBuffer records (56 bytes each), +0x08 the vertex
//                          CMeshBufferWithAttributeData table (an array of pointers to an object whose +0x20 is a pointer
//                          holding the vertex stream table at +0x04 and the index data at +0x24)
//   SMeshBuffer (56 byt.): +0x00 primitive type index (through the anonymous ColladaPrimitiveMap at 0x8eb338), +0x08 the
//                          vertex count, +0x0c the index count, +0x10..+0x2f the stream attribute bytes (texture coordinate
//                          sets at +0x10, the normal at +0x11, the colour at +0x12, +0x14 for tangents, +0x18 for binormals,
//                          -1 = unused), +0x24 the index type (1 for 16-bit when the vertex count is below 0x10000, else 2 for
//                          32-bit), +0x28 the vertex count + 1, +0x30 the existing vertex IBuffer, +0x2c the existing index
//                          IBuffer / on-demand pointer
//   SBufferConfig:         a byte at +0x04 (on-demand) and a byte at +0x05 (usage)
// Not reconstructed: that whole body - the primitive record copy, the CVertexStreams masks accumulated from the attribute
// bytes, operator new(0xc) buffers and the anonymous addStream (0x6bccf0) / ColladaPrimitiveMap helpers. It also needs
// collada::SMesh and collada::SBufferConfig to be declared in this tree, which they are not (CMeshBuffer.hpp only forward
// declares them). Note for whoever takes it: the driver call it makes is a SEVEN-argument vtable +0x78 (createBuffer with
// type, usage, a flag, a size, a data pointer and one more argument), while the port's IVideoDriver::createBuffer takes
// (type, usage, size, data, flag).
CMeshBuffer::CMeshBuffer(video::IVideoDriver* /*driver*/, collada::SMesh& /*mesh*/, std::uint32_t /*primitiveIndex*/,
                         const collada::SBufferConfig& /*vertexConfig*/, const collada::SBufferConfig& /*indexConfig*/,
                         bool /*onDemand*/) {
    referenceCounter = 0;
}

// Not part of the original (see the header): the empty buffer.
CMeshBuffer::CMeshBuffer() {
    referenceCounter = 0;
}

// 0x6b88a8, 0x6d11d4 and 0x6d5ed4 _ZN6glitch5scene11CMeshBufferC1EjRKNS_5video16CPrimitiveStreamEb (three listings)
// Reconstructed: clears the count and the three blocks, allocates the vertex stream table for the mask, copies the primitive
// stream (taking a use of its index buffer), clears the owner and stores the bool argument in the flag byte at +0x34 (the
// listings at 0x6b88a8 and 0x6d11d4 are clones of it with the argument folded to 1 and to 0).
CMeshBuffer::CMeshBuffer(std::uint32_t attributeMask, const video::CPrimitiveStream& stream, bool flag)
    : vertexStreams(video::CVertexStreams::allocate(attributeMask)), primitiveStream(stream) {
    referenceCounter = 0;
    flag_34 = flag ? 1 : 0;
}

// 0x6bc8c4 _ZN6glitch5scene11CMeshBufferD1Ev
// 0x6bca38 _ZN6glitch5scene11CMeshBufferD2Ev
// 0x6bca1c _ZN6glitch5scene11CMeshBufferD0Ev (skipped)
// Verified against Hex-Rays 0x6bc8c4: the owner at +0x30 is called through its vtable +0x04 (the deleting destructor) and the
// word is cleared; the primitive stream's index buffer (+0x24) is dropped; the vertex stream table at +0x14 loses a use and is
// freed with its CVertexStreams destructor at zero; and each of the three blocks (+0x08, +0x0c, +0x10) loses a use and, at
// zero, operator delete[]s its element storage at +0x0c of the block. In the port the primitive stream and the vertex streams
// are members that release themselves and the owner is a raw pointer deleted here; the three blocks are opaque pointers that
// nothing in this category allocates, so their storage is not freed.
CMeshBuffer::~CMeshBuffer() {
    if (owner != nullptr) {
        delete owner;  // vtable +0x04, the original's call through the owner's first slot
        owner = nullptr;
    }
}

// 0x6bc6d8 _ZN6glitch5scene11CMeshBuffer8onDeleteEv
// Verified against Hex-Rays 0x6bc6d8: forwards to the owner's vtable +0x08 slot, nothing when +0x30 is null.
void CMeshBuffer::onDelete() {
    if (owner != nullptr) {
        owner->onDelete();
    }
}

// 0x6bcb90 _ZNK6glitch5scene11CMeshBuffer5cloneEv
// Verified against Hex-Rays 0x6bcb90: operator new(0x38); +0x04 count 0, +0x08/+0x0c/+0x10 blocks and +0x30 owner null, +0x14
// the CVertexStreams::clone (0x5a1310) of the source table (a use is taken as well, matching the original's temporary
// reference), the primitive stream copied from +0x18..+0x2f with a use of the index buffer at +0x24, and the flag byte at
// +0x34 copied. The returned handle holds the first use.
boost::intrusive_ptr<CMeshBuffer> CMeshBuffer::clone() const {
    auto* copy = new CMeshBuffer();
    if (vertexStreams) {
        copy->vertexStreams = vertexStreams->clone();
    }
    copy->primitiveStream = primitiveStream;
    copy->flag_34 = flag_34;
    return boost::intrusive_ptr<CMeshBuffer>(copy, true);
}

}  // namespace glitch::scene
