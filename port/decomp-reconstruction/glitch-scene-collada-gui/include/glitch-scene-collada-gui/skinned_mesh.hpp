// glitch::collada::CSkinnedMesh: skinned mesh with a per-buffer table of
// 0x14-byte entries at +0x5c. Only the simple accessors are reconstructed
// here; the virtual-dispatch and bounding-box paths depend on ISkinnedMesh
// and the base mesh layout, which are not yet mapped.
#pragma once

#include <cstdint>

namespace glitch {
class IReferenceCounted;
}

namespace glitch::video {
class IVideoDriver;
}

namespace glitch::collada {

class CColladaDatabase;
class CRootSceneNode;
struct SController;

// Intrusive reference-counted object. The count is the first word for
// materials and attribute maps.
struct SCountedRef {
    std::int32_t count = 0;  // +0x00
};

// Mesh buffers keep their count at +0x04 in the original.
struct SMeshBufferRef {
    std::int32_t reserved = 0;  // +0x00
    std::int32_t count = 0;     // +0x04
};

// One entry of the per-buffer table (0x14 bytes in the original).
struct SSkinnedBufferEntry {
    SMeshBufferRef* meshBuffer = nullptr;        // +0x00
    SCountedRef* material = nullptr;             // +0x04
    SCountedRef* vertexAttributeMap = nullptr;   // +0x08
    void* technique = nullptr;                   // +0x0c, skin technique (type not mapped yet)
    std::uint8_t requestedState = 0;             // +0x10, skin() re-applies when it differs from appliedState
    std::uint8_t appliedState = 0;               // +0x11
};

class CSkinnedMesh {
public:
    SSkinnedBufferEntry* buffers = nullptr;  // +0x5c

    // Constructor and skinning paths (batch e1-glitch-scene-collada-gui-glitch_collada_CSkinnedMesh-0).
    // The default constructor stays for existing code. Offsets are the original's 32-bit layout.
    CSkinnedMesh() = default;
    CSkinnedMesh(const CColladaDatabase& database, video::IVideoDriver* driver, SController& controller,
                 CRootSceneNode* root);

    const char* controllerName = nullptr;             // +0x08, SController name
    std::uint32_t techniqueMask = 0;                  // +0x14, one bit per buffer index (low 5 bits)
    float boundingBox[6] = {};                        // +0x24..+0x38, min then max
    std::uint8_t skinFlag = 0;                        // +0x20, passed to the technique update
    void* skinTechnique = nullptr;                    // +0x3c, CColladaHardwareTextureSkinTechnique
    glitch::IReferenceCounted* mesh = nullptr;        // +0x68, owned reference

    void computeBoundingBox();                                       // 0x663648
    void instanciateMesh(video::IVideoDriver* driver, CRootSceneNode* root);  // 0x664af8
    void skin(unsigned int index);                                   // 0x664ee0

    // Each accessor writes an owning reference into `out` and increments the
    // count, as the original boost::intrusive_ptr assignment does.
    std::int32_t getType() const;                                                    // 0x663524
    CSkinnedMesh* clone() const;                                                     // 0x66352c
    void getMeshBuffer(SMeshBufferRef*& out, std::uint32_t index) const;            // 0x6635d0
    void getMaterial(SCountedRef*& out, std::uint32_t index) const;                 // 0x6635f8
    void getMaterialVertexAttributeMap(SCountedRef*& out, std::uint32_t index) const;  // 0x663620
};

}  // namespace glitch::collada
