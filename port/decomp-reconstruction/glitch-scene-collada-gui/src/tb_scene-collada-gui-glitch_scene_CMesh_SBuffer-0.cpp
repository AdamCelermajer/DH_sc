// Track B batch e1-glitch-scene-collada-gui-glitch_scene_CMesh_SBuffer-0: glitch::scene::CMesh::SBuffer.
// Declaration is in glitch-scene-collada-gui/include/glitch-scene-collada-gui/tb/glitch_scene_CMesh_SBuffer.hpp
// (nested in CMesh).

#include "glitch-scene-collada-gui/tb/glitch_scene_CMesh_SBuffer.hpp"
#include "glitch-shared/reference_counted.hpp"

namespace glitch::scene {

// 0x6bbe18 _ZN6glitch5scene5CMesh7SBufferD1Ev
// Partial. The original body, in order (ARM at 0x6bbe18: PUSH {R4,LR}; release +0x08; release +0x04;
// if [this] != 0, IReferenceCounted::drop([this]); POP {R4,PC}):
//   1. intrusive_ptr<CMaterialVertexAttributeMap> release of +0x08 (vertexAttributeMap)
//   2. intrusive_ptr<CMaterial> release of +0x04 (material)
//   3. if the buffer at +0x00 is non-null, IReferenceCounted::drop(buffer)
// Only step 3 is reconstructed. Steps 1 and 2 need the counting intrusive_ptr (the canonical form in
// TRACK-B.md, decision 1). The category deps/intrusive_ptr.hpp stand-in holds the pointer and never
// releases it, so no release can be written yet. The member handles are destroyed after this body
// (reverse declaration order), so when the counting form lands, steps 1 and 2 must move here
// (before step 3) and must not be released twice.
// The Hex-Rays signature types `this` as CMaterialVertexAttributeMap**; the layout is the SBuffer
// record above (the +0x00 pointer is the refcounted buffer, as in layouts/scene.hpp).
CMesh::SBuffer::~SBuffer() {
    if (buffer != nullptr) {
        buffer->drop();
    }
}

}  // namespace glitch::scene
