// glitch::scene::CMesh::SBuffer, declared for Track B batch e1-glitch-scene-collada-gui-glitch_scene_CMesh_SBuffer-0.
// No other header in this category declares glitch::scene::CMesh. The glitch::scene::CMeshSBuffer layout in
// layouts/scene.hpp is a plain 12-byte record for the assignment and recalculateBoundingBox paths, and the
// glitch::collada::CMesh::SBuffer in collada_scene_accessors.hpp is the same shape in the collada namespace.
// This header declares the nested type with the same member names, so the scene-namespace bodies can use it.
//
// Offsets are the original 32-bit layout, kept as comments. Only SBuffer is declared. The rest of CMesh
// (boundingBox at +0x14, the buffer vector at +0x08..+0x10, and its methods) is not mapped here and is left
// to the batch that owns CMesh.
#pragma once

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-shared/reference_counted.hpp"

namespace glitch::video {
class CMaterial;
class CMaterialVertexAttributeMap;
}  // namespace glitch::video

namespace glitch::scene {

class CMesh {
public:
    // glitch::scene::CMesh::SBuffer, 12 bytes in the original (the vector stride of the buffer array).
    struct SBuffer {
        // 0x6bbe18 _ZN6glitch5scene5CMesh7SBufferD1Ev
        // Partial: see the note on the definition in src/tb_scene-collada-gui-glitch_scene_CMesh_SBuffer-0.cpp.
        ~SBuffer();

        glitch::IReferenceCounted* buffer = nullptr;                                  // +0x00
        boost::intrusive_ptr<video::CMaterial> material;                              // +0x04
        boost::intrusive_ptr<video::CMaterialVertexAttributeMap> vertexAttributeMap;  // +0x08
    };

    // The rest of CMesh is not declared in this batch.
};

}  // namespace glitch::scene
