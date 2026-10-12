// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CMesh-0.
// Mesh buffer slots are the CMesh::buffers vector (+0x18 begin, +0x1c end, 12-byte SBuffer stride).
// The category's boost::intrusive_ptr stand-in does not count references, so the count bumps and
// releases the original performs on the stored handles are not modelled. Each affected function is
// marked PARTIAL in the ledger.
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

namespace glitch::collada {

// 0x644a44 glitch::collada::CMesh::getMeshBufferCount() const
// Original: (end - begin) / 12, computed as a multiply by the inverse of 3. Same as the element count.
std::uint32_t CMesh::getMeshBufferCount() const {
    return static_cast<std::uint32_t>(buffers.size());
}

// 0x644a6c glitch::collada::CMesh::getMeshBuffer(unsigned int) const
// PARTIAL: an index at or past the end gives a null handle (the original stores 0). Inside the range the
// original also bumps the buffer's reference count at +0x04. That bump is not performed here: the
// buffer type is not complete in this category and the handle does not count.
glitch::IReferenceCounted* CMesh::getMeshBuffer(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return nullptr;
    }
    return buffers[index].buffer;
}

// 0x644ac8 glitch::collada::CMesh::getMaterial(unsigned int) const
// PARTIAL: an index at or past the end gives a null handle. Inside the range the original bumps the
// material's count at +0x00. That bump is not performed (see getMeshBuffer).
boost::intrusive_ptr<video::CMaterial> CMesh::getMaterial(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return {};
    }
    return buffers[index].material;
}

// 0x644b24 glitch::collada::CMesh::getMaterialVertexAttributeMap(unsigned int) const
// PARTIAL: an index at or past the end gives a null handle. Inside the range the original bumps the
// attribute map's count at +0x00. That bump is not performed (see getMeshBuffer).
boost::intrusive_ptr<video::CMaterialVertexAttributeMap> CMesh::getMaterialVertexAttributeMap(
        std::uint32_t index) const {
    if (index >= buffers.size()) {
        return {};
    }
    return buffers[index].vertexAttributeMap;
}

// 0x644bdc glitch::collada::CMesh::setMaterial(unsigned int, boost::intrusive_ptr<CMaterial> const&,
//          boost::intrusive_ptr<CMaterialVertexAttributeMap> const&)
// PARTIAL: an index past the end is ignored, as in the original. Inside the range the original takes a
// count on each new handle, then releases the replaced one. Neither count move is performed here. The
// order of the stores (material first, then attribute map) is kept.
void CMesh::setMaterial(std::uint32_t index, const boost::intrusive_ptr<video::CMaterial>& material,
                        const boost::intrusive_ptr<video::CMaterialVertexAttributeMap>& vertexAttributeMap) {
    if (index >= buffers.size()) {
        return;
    }
    SBuffer& slot = buffers[index];
    slot.material = material;              // +0x04
    slot.vertexAttributeMap = vertexAttributeMap;  // +0x08
}

// 0x644cfc glitch::collada::CMesh::~CMesh()
// SKIPPED: the original releases the SBuffer vector (dropping each buffer, material and map reference)
// and then runs the CColladaDatabase base destructor at +0x0c. The reference releases are not
// modelled in this category, and the base chain is not part of this class, so no body is written.

// 0x645588 glitch::collada::CMesh::CMesh(CColladaDatabase const&, IVideoDriver*, SGeometry const&,
//          SBufferConfig const&, SBufferConfig const&, bool)
// SKIPPED: the body is about 7 KB. It needs the onDemand<T>::get readers for the index and vertex
// buffer records (glitch::res::onDemand, CResFileManager::Inst), the CMeshBuffer constructor, and the
// SBufferConfig layout, none of which is mapped in this category. Writing a partial constructor would
// leave the buffer list empty and wrong, so no body is written.

}  // namespace glitch::collada
