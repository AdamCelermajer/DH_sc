#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"

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

}  // namespace glitch::collada
