// Bodies of glitch::scene::CMeshBuffer (canonical class: scene/CMeshBuffer.hpp). This file replaces
// src/tb_scene-collada-gui-glitch_scene_CMeshBuffer-0.cpp, which wrote against the 32-bit layout struct of layouts/scene.hpp.
#include "glitch-scene-collada-gui/scene/CMeshBuffer.hpp"

namespace glitch::scene {

// 0x6bcf80 _ZN6glitch5scene11CMeshBufferC2EPNS_5video12IVideoDriverERNS_7collada5SMeshEjRKNS5_13SBufferConfigES9_b (base object)
// 0x6bd9f8 (C1, skipped: identical)
// PARTIAL. Reconstructed: the prologue clears the words the original clears (+0x04 reference count, +0x08, +0x0c, +0x10 blocks,
// +0x14 vertex streams, +0x1c, +0x30 owner, +0x34 flag). Not reconstructed: the primitive record copy (SMesh primitive array
// element `primitiveIndex`, 56 bytes: stream, words, index width), the vertex stream and index buffer creation (IVideoDriver
// vtable +0x78, the on-demand path, CVertexStreams::allocate and setupStreams) and the process-buffer helpers.
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
// Reconstructed: the original, in order, runs the owner's deleting destructor (vtable +0x04) and clears it; drops the primitive
// stream's index buffer; releases the vertex streams (count at word 0, deleting the table at zero); and decrements the three
// blocks, freeing their element storage at zero. The members release the streams and the index buffer; the three blocks are
// opaque pointers that nothing in this port allocates, so they are not released.
CMeshBuffer::~CMeshBuffer() {
    if (owner != nullptr) {
        delete owner;
        owner = nullptr;
    }
}

// 0x6bc6d8 _ZN6glitch5scene11CMeshBuffer8onDeleteEv
// Reconstructed: forwards to the owner's onDelete slot (vtable +0x08).
void CMeshBuffer::onDelete() {
    if (owner != nullptr) {
        owner->onDelete();
    }
}

// 0x6bcb90 _ZNK6glitch5scene11CMeshBuffer5cloneEv
// Reconstructed: operator new(0x38); the blocks and the owner stay null, the vertex stream table is cloned
// (CVertexStreams::clone 0x5a1310), the primitive stream (and a use of its index buffer) and the flag byte are copied. The
// returned handle holds the first use.
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
