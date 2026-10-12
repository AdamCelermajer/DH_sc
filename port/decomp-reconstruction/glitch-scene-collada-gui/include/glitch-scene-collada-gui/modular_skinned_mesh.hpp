// glitch::collada::CModularSkinnedMesh: skinned mesh assembled from module
// buffers. Buffers are 0x20-byte records (SModularBuffer) in a vector at
// +0x30..+0x34; the category table is reached through a pointer at +0x1c.
// Current module ids are 8-byte records at +0x24. Only the accessors that use
// these layouts directly are reconstructed here.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

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

    // Module holder: the module name is the word at +0x04.
    struct SModuleHolder {
        std::uint32_t reserved = 0;          // +0x00
        const char* name = nullptr;          // +0x04
    };

    // One module record of a category: 8 bytes, holder pointer at +0x04.
    struct SModuleRecord {
        std::uint32_t reserved = 0;          // +0x00
        SModuleHolder* holder = nullptr;     // +0x04
    };

    // One category record of the table: 0x10 bytes. The name is the word at +0x00,
    // the module count at +0x08 and the module array at +0x0c.
    struct SCategoryEntry {
        const char* name = nullptr;          // +0x00
        std::uint32_t moduleCount = 0;       // +0x08
        SModuleRecord* modules = nullptr;    // +0x0c
    };

    struct SCategoryTable {
        std::uint32_t count = 0;                 // first word of the table object
        SCategoryEntry* entries = nullptr;       // +0x04
    };

    struct SModuleSlot {
        std::int32_t moduleId = -1;  // first word of each 8-byte record
        std::uint32_t reserved = 0;
    };

    std::vector<SModularBuffer> buffers;     // +0x30..+0x34
    SCategoryTable* categories = nullptr;    // +0x1c
    std::vector<SModuleSlot> currentModules; // +0x24
    bool needOutputBufferFlag = false;       // +0x58
    bool boundingBoxDirty = false;           // +0x59
    core::aabbox3d<float> boundingBox;       // +0x40..+0x57

    std::int32_t getType() const;                                      // 0x646a94
    CModularSkinnedMesh* clone() const;                                // 0x6469c0
    std::uint32_t getCategoryCount() const;                            // 0x646a28
    std::uint32_t getCategoryModuleCount(int category) const;          // 0x646a34
    std::int32_t getCurrentModuleId(int category) const;               // 0x646a48
    bool needOutputBuffer() const;                                     // 0x646af0
    std::uint32_t getMeshBufferCount() const;                          // 0x646b54
    int reallocate(std::uint32_t size);                                // 0x646a8c (returns 0)
    // 0x6469cc _ZNK6glitch7collada19CModularSkinnedMesh15getCategoryNameEi (null when out of range)
    const char* getCategoryName(std::int32_t category) const;
    // 0x6469e8 _ZNK6glitch7collada19CModularSkinnedMesh13getModuleNameEii (null when out of range)
    const char* getModuleName(std::int32_t category, std::int32_t module) const;
    // 0x646a54 _ZNK6glitch7collada19CModularSkinnedMesh20getCurrentModuleNameEi (null when no module is set)
    const char* getCurrentModuleName(std::int32_t category) const;
    // 0x647538 _ZNK6glitch7collada19CModularSkinnedMesh13getCategoryIdEPKc (-1 when not found)
    std::int32_t getCategoryId(const char* name) const;
    // 0x6474b8 _ZNK6glitch7collada19CModularSkinnedMesh11getModuleIdEPKc
    // Index of the module inside the first category that has a module of this name; -1 if none.
    std::int32_t getModuleId(const char* name) const;
    // 0x646cdc _ZNK6glitch7collada19CModularSkinnedMesh14getBoundingBoxEv
    // partial: the refresh call taken when the dirty flag at +0x59 is set is not reconstructed.
    const core::aabbox3d<float>& getBoundingBox() const;
    void getMaterial(SCountedRef*& out, std::uint32_t index) const;    // 0x646bd4
    void getMaterialVertexAttributeMap(SCountedRef*& out, std::uint32_t index) const;  // 0x646bf8
};

}  // namespace glitch::collada
