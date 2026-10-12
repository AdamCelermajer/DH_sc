#include "glitch-scene-collada-gui/skinned_mesh.hpp"

namespace glitch::collada {

// 0x663524 _ZNK6glitch7collada12CSkinnedMesh7getTypeEv
std::int32_t CSkinnedMesh::getType() const {
    return 2;
}

// 0x66352c _ZNK6glitch7collada12CSkinnedMesh5cloneEv
// The original stores a null result.
CSkinnedMesh* CSkinnedMesh::clone() const {
    return nullptr;
}

// 0x6635d0 _ZNK6glitch7collada12CSkinnedMesh13getMeshBufferEj
void CSkinnedMesh::getMeshBuffer(SMeshBufferRef*& out, std::uint32_t index) const {
    SMeshBufferRef* buffer = buffers[index].meshBuffer;
    out = buffer;
    if (buffer) ++buffer->count;
}

// 0x6635f8 _ZNK6glitch7collada12CSkinnedMesh11getMaterialEj
void CSkinnedMesh::getMaterial(SCountedRef*& out, std::uint32_t index) const {
    SCountedRef* material = buffers[index].material;
    out = material;
    if (material) ++material->count;
}

// 0x663620 _ZNK6glitch7collada12CSkinnedMesh29getMaterialVertexAttributeMapEj
void CSkinnedMesh::getMaterialVertexAttributeMap(SCountedRef*& out, std::uint32_t index) const {
    SCountedRef* map = buffers[index].vertexAttributeMap;
    out = map;
    if (map) ++map->count;
}

}  // namespace glitch::collada
