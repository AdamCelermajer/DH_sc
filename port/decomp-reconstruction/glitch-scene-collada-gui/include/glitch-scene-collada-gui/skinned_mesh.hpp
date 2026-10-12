// glitch::collada::CSkinnedMesh: skinned mesh with a per-buffer table of
// 0x14-byte entries at +0x5c. Only the simple accessors are reconstructed
// here; the virtual-dispatch and bounding-box paths depend on ISkinnedMesh
// and the base mesh layout, which are not yet mapped.
#pragma once

#include <cstdint>

namespace glitch::collada {

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
    std::uint32_t reserved[2] = {0, 0};          // +0x0c, +0x10
};

class CSkinnedMesh {
public:
    SSkinnedBufferEntry* buffers = nullptr;  // +0x5c

    // Each accessor writes an owning reference into `out` and increments the
    // count, as the original boost::intrusive_ptr assignment does.
    std::int32_t getType() const;                                                    // 0x663524
    CSkinnedMesh* clone() const;                                                     // 0x66352c
    void getMeshBuffer(SMeshBufferRef*& out, std::uint32_t index) const;            // 0x6635d0
    void getMaterial(SCountedRef*& out, std::uint32_t index) const;                 // 0x6635f8
    void getMaterialVertexAttributeMap(SCountedRef*& out, std::uint32_t index) const;  // 0x663620
};

}  // namespace glitch::collada
