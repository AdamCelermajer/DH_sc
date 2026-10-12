#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"

#include <cstring>

namespace glitch::collada {

// 0x646a94 _ZNK6glitch7collada19CModularSkinnedMesh7getTypeEv
std::int32_t CModularSkinnedMesh::getType() const {
    return 3;
}

// 0x6469c0 _ZNK6glitch7collada19CModularSkinnedMesh5cloneEv
CModularSkinnedMesh* CModularSkinnedMesh::clone() const {
    return nullptr;
}

// 0x646a28 _ZNK6glitch7collada19CModularSkinnedMesh16getCategoryCountEv
std::uint32_t CModularSkinnedMesh::getCategoryCount() const {
    return categories->count;
}

// 0x646a34 _ZNK6glitch7collada19CModularSkinnedMesh22getCategoryModuleCountEi
std::uint32_t CModularSkinnedMesh::getCategoryModuleCount(int category) const {
    return categories->entries[category].moduleCount;
}

// 0x646a48 _ZNK6glitch7collada19CModularSkinnedMesh18getCurrentModuleIdEi
std::int32_t CModularSkinnedMesh::getCurrentModuleId(int category) const {
    return currentModules[category].moduleId;
}

// 0x646af0 _ZNK6glitch7collada19CModularSkinnedMesh16needOutputBufferEv
bool CModularSkinnedMesh::needOutputBuffer() const {
    return needOutputBufferFlag;
}

// 0x646b54 _ZNK6glitch7collada19CModularSkinnedMesh18getMeshBufferCountEv
std::uint32_t CModularSkinnedMesh::getMeshBufferCount() const {
    return static_cast<std::uint32_t>(buffers.size());
}

// 0x646a8c _ZN6glitch7collada19CModularSkinnedMesh10reallocateEj
// Original returns 0 and does nothing.
int CModularSkinnedMesh::reallocate(std::uint32_t /*size*/) {
    return 0;
}

// 0x646bd4 _ZNK6glitch7collada19CModularSkinnedMesh11getMaterialEj
void CModularSkinnedMesh::getMaterial(SCountedRef*& out, std::uint32_t index) const {
    SCountedRef* material = buffers[index].material;
    out = material;
    if (material) ++material->count;
}

// 0x646bf8 _ZNK6glitch7collada19CModularSkinnedMesh29getMaterialVertexAttributeMapEj
void CModularSkinnedMesh::getMaterialVertexAttributeMap(SCountedRef*& out, std::uint32_t index) const {
    SCountedRef* map = buffers[index].vertexAttributeMap;
    out = map;
    if (map) ++map->count;
}


// 0x6469cc _ZNK6glitch7collada19CModularSkinnedMesh15getCategoryNameEi
const char* CModularSkinnedMesh::getCategoryName(std::int32_t category) const {
    if (category >= 0 && static_cast<std::uint32_t>(category) < categories->count) {
        return categories->entries[category].name;
    }
    return nullptr;
}

// 0x6469e8 _ZNK6glitch7collada19CModularSkinnedMesh13getModuleNameEii
const char* CModularSkinnedMesh::getModuleName(std::int32_t category, std::int32_t module) const {
    if (category < 0 || static_cast<std::uint32_t>(category) >= categories->count) {
        return nullptr;
    }
    const SCategoryEntry& entry = categories->entries[category];
    if (module < 0 || static_cast<std::uint32_t>(module) >= entry.moduleCount) {
        return nullptr;
    }
    return entry.modules[module].holder->name;
}

// 0x646a54 _ZNK6glitch7collada19CModularSkinnedMesh20getCurrentModuleNameEi
const char* CModularSkinnedMesh::getCurrentModuleName(std::int32_t category) const {
    const std::int32_t moduleId = getCurrentModuleId(category);
    if (moduleId == -1) {
        return nullptr;
    }
    return categories->entries[category].modules[moduleId].holder->name;
}

// 0x647538 _ZNK6glitch7collada19CModularSkinnedMesh13getCategoryIdEPKc
std::int32_t CModularSkinnedMesh::getCategoryId(const char* name) const {
    for (std::uint32_t i = 0; i < categories->count; ++i) {
        if (std::strcmp(categories->entries[i].name, name) == 0) {
            return static_cast<std::int32_t>(i);
        }
    }
    return -1;
}

// 0x6474b8 _ZNK6glitch7collada19CModularSkinnedMesh11getModuleIdEPKc
std::int32_t CModularSkinnedMesh::getModuleId(const char* name) const {
    for (std::uint32_t category = 0; category < categories->count; ++category) {
        const SCategoryEntry& entry = categories->entries[category];
        for (std::uint32_t module = 0; module < entry.moduleCount; ++module) {
            if (std::strcmp(entry.modules[module].holder->name, name) == 0) {
                return static_cast<std::int32_t>(module);
            }
        }
    }
    return -1;
}

// 0x646cdc _ZNK6glitch7collada19CModularSkinnedMesh14getBoundingBoxEv
// partial: the refresh call taken when the dirty flag is set is not reconstructed.
const core::aabbox3d<float>& CModularSkinnedMesh::getBoundingBox() const {
    return boundingBox;
}

}  // namespace glitch::collada
