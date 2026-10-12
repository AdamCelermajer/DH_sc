// Track B batch e1-glitch-scene-collada-gui-glitch_scene_CMeshBuffer-0: glitch::scene::CMeshBuffer.
// Declarations and the 32-bit layout are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/layouts/scene.hpp
// (struct CMeshBuffer). Its target pointers are 32-bit words, so the reference-count and buffer
// work that dereferences them is not written yet. Each function says what is missing.

#include "glitch-scene-collada-gui/layouts/scene.hpp"

namespace glitch::scene {

// 0x6bc8c4 glitch::scene::CMeshBuffer::~CMeshBuffer()
// Partial. The original body, in release order:
//   1. vptr := CMeshBuffer vtable (not representable: the layout has no host vptr).
//   2. owner (+0x30): if non-null, call its vtable slot 1 (vtable +0x04), then owner := 0.
//   3. primitiveStream (+0x18): if non-null, glitch::IReferenceCounted::drop.
//   4. vertexStreams (+0x14): if non-null and its count (word 0) drops from 1, run
//      CVertexStreams::~CVertexStreams and free the object. Stays as a count-decrement only.
//   5. blockAt0x10, blockAt0x0c, blockAt0x08 (+0x10, +0x0c, +0x08), in that order: if non-null,
//      decrement the block count (word 0); at 1, delete[] the element storage (block word 3, +0x0c)
//      if non-null and set it to 0; then set the member to 0.
// Nothing is released here. Steps 2 to 5 dereference 32-bit target words, and the category has no
// host-width form of these types yet (TRACK-B.md, decision 2). Steps 2 to 5 must be written once the
// layout is moved to host pointers. Until then this destructor leaks the owned objects.
CMeshBuffer::~CMeshBuffer() {
}

// 0x6bcf80 glitch::scene::CMeshBuffer::CMeshBuffer(glitch::video::IVideoDriver*, glitch::collada::SMesh&, unsigned int, glitch::collada::SBufferConfig const&, glitch::collada::SBufferConfig const&, bool)
// Partial. Reconstructed: the prologue clears the words the original clears before it reads the
// primitive (+0x14, +0x04, +0x08, +0x0c, +0x10, +0x1c, +0x30, +0x34). Not reconstructed:
//   - the vptr store (no host vptr in the layout);
//   - the primitive record: element index primitiveIndex of mesh's primitive array (SMesh +0x10),
//     a 56-byte record (word numbers below are 32-bit words of that record). The original copies:
//     word 12 -> +0x18 (primitiveStream, refcounted); word 10 -> +0x20; word 8 -> +0x24;
//     word 9 + 1 -> +0x28; a width code (1 if word 9 < 0x10000, else 2) -> +0x2c; the
//     ColladaPrimitiveMap entry indexed by word 0 -> +0x2e. The
//     record and SMesh layouts are not declared in this category, so none of these copies are written;
//   - the vertex-stream and index-buffer creation: the IVideoDriver call through vtable +0x78
//     (slot 120) with the usage from vertexConfig/indexConfig, the onDemand path (onDemandPointer
//     of unsigned short and unsigned int, which is not declared here), boost::intrusive_ptr<IBuffer>
//     assignment, and the per-stream loop with the anonymous-namespace addStream helper;
//   - CVertexStreams::allocate and setupStreams, and the process-buffer alloc and release helpers
//     (core::allocProcessBuffer, core::releaseProcessBuffer), none of them declared here.
// Callers get an object whose primitive fields and vertex streams are unset.
CMeshBuffer::CMeshBuffer(video::IVideoDriver* driver, collada::SMesh& mesh, unsigned int primitiveIndex,
                         const collada::SBufferConfig& vertexConfig, const collada::SBufferConfig& indexConfig,
                         bool onDemand) {
    // Prologue, as in the original (reconstructed).
    referenceCount = 0;   // +0x04
    blockAt0x08 = 0;      // +0x08
    blockAt0x0c = 0;      // +0x0c
    blockAt0x10 = 0;      // +0x10
    vertexStreams = 0;    // +0x14
    primitiveWord1C = 0;  // +0x1c
    owner = 0;            // +0x30
    flag34 = 0;           // +0x34 (the pseudocode stores 0 here; the layout comment says 1)

    // Parameters are read by the parts above that are not reconstructed; keep them named for that work.
    (void)driver;
    (void)mesh;
    (void)primitiveIndex;
    (void)vertexConfig;
    (void)indexConfig;
    (void)onDemand;
}

// 0x6bd9f8 glitch::scene::CMeshBuffer::CMeshBuffer(glitch::video::IVideoDriver*, glitch::collada::SMesh&, unsigned int, glitch::collada::SBufferConfig const&, glitch::collada::SBufferConfig const&, bool)
// Skipped: the pseudocode is identical to the constructor at 0x6bcf80 (only the address comment
// differs), so the same body serves both addresses. There is one definition, above.

}  // namespace glitch::scene
