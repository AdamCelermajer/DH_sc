// glitch::collada::CModularSkinnedMesh: skinned mesh assembled from module
// buffers. Buffers are 0x20-byte records (SModularBuffer) in a vector at
// +0x30..+0x34; the category table is reached through a pointer at +0x1c.
// Current module ids are 8-byte records at +0x24. Only the accessors that use
// these layouts directly are reconstructed here.
#pragma once

#include <cstdint>
#include <vector>

namespace glitch::collada {

struct SCountedRef {
    std::int32_t count = 0;  // +0x00 intrusive count (see skinned_mesh.hpp)
};

class CModularSkinnedMesh {
public:
    struct SModularBuffer {
        SCountedRef* meshBuffer = nullptr;          // +0x00 (mesh buffer ref, count at +0)
        SCountedRef* material = nullptr;            // +0x04
        SCountedRef* vertexAttributeMap = nullptr;  // +0x08
        std::uint32_t reserved[5] = {0, 0, 0, 0, 0};  // +0x0c..+0x1c-ish (not yet mapped)
        bool needsOutputBuffer = true;              // +0x1c flag set by the constructor
    };

    struct SCategoryTable {
        std::uint32_t count = 0;   // first word of the table object
    };

    struct SModuleSlot {
        std::int32_t moduleId = -1;  // first word of each 8-byte record
        std::uint32_t reserved = 0;
    };

    std::vector<SModularBuffer> buffers;     // +0x30..+0x34
    SCategoryTable* categories = nullptr;    // +0x1c
    std::vector<SModuleSlot> currentModules; // +0x24
    bool needOutputBufferFlag = false;       // +0x58

    std::int32_t getType() const;                                      // 0x646a94
    CModularSkinnedMesh* clone() const;                                // 0x6469c0
    std::uint32_t getCategoryCount() const;                            // 0x646a28
    std::int32_t getCurrentModuleId(int category) const;               // 0x646a48
    bool needOutputBuffer() const;                                     // 0x646af0
    std::uint32_t getMeshBufferCount() const;                          // 0x646b54
    int reallocate(std::uint32_t size);                                // 0x646a8c (returns 0)
    void getMaterial(SCountedRef*& out, std::uint32_t index) const;    // 0x646bd4
    void getMaterialVertexAttributeMap(SCountedRef*& out, std::uint32_t index) const;  // 0x646bf8
};

}  // namespace glitch::collada
