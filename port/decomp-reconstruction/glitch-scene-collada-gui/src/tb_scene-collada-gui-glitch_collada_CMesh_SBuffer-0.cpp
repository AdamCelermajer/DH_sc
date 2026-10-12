// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CMesh_SBuffer-0: glitch::collada::CMesh::SBuffer.
// Layout and declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/collada_scene_accessors.hpp
// (nested in CMesh).

#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"
#include "glitch-shared/reference_counted.hpp"

namespace glitch::collada {

// 0x644c88 _ZN6glitch7collada5CMesh7SBufferD1Ev
// Partial. The original body, in order:
//   1. intrusive_ptr<CMaterialVertexAttributeMap> release of +0x08 (vertexAttributeMap)
//   2. intrusive_ptr<CMaterial> release of +0x04 (material)
//   3. if the buffer at +0x00 is non-null, IReferenceCounted::drop(buffer)
// Only step 3 is reconstructed. Steps 1 and 2 need the counting intrusive_ptr (the canonical form in
// TRACK-B.md, decision 1). The category deps/intrusive_ptr.hpp stand-in holds the pointer and never
// releases it, so no release can be written yet. The member handles are destroyed after this body
// (reverse declaration order), so when the counting form lands, steps 1 and 2 must move here
// (before step 3) and must not be released twice.
CMesh::SBuffer::~SBuffer() {
    // +0x00: buffer (refcounted). The original's IReferenceCounted::drop is a const member, so the
    // call is the same.
    if (buffer != nullptr) {
        buffer->drop();
    }
}

}  // namespace glitch::collada
