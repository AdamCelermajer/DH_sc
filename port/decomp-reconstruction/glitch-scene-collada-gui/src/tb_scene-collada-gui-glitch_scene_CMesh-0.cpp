// Track B batch e1-glitch-scene-collada-gui-glitch_scene_CMesh-0: glitch::scene::CMesh.
// Declarations are in glitch-scene-collada-gui/include/glitch-scene-collada-gui/scene_simple_accessors.hpp
// (class CMesh, nested SBuffer). The SBuffer array is the CMesh::buffers vector (+0x08 begin, +0x0c end,
// 12-byte stride), the same shape as glitch::collada::CMesh.
//
// The category's boost::intrusive_ptr stand-in does not count references, so the count bumps and
// releases the original performs on the stored handles are not modelled. Each function that does
// those bumps is marked PARTIAL in the ledger; the range checks and the stores are reconstructed.

#include "glitch-scene-collada-gui/scene_simple_accessors.hpp"

namespace glitch::scene {

// 0x6bbb38 glitch::scene::CMesh::getMeshBufferCount() const
// Original: (end - begin) / 12, computed as a multiply by the inverse of 3. Same as the element count.
std::int32_t CMesh::getMeshBufferCount() const {
    return static_cast<std::int32_t>(buffers.size());
}

// 0x6bbb60 glitch::scene::CMesh::getMeshBuffer(unsigned int) const
// PARTIAL: an index at or past the end gives a null handle (the original stores 0). Inside the range the
// original also bumps the buffer's reference count at +0x04. That bump is not performed here because the
// stand-in intrusive_ptr does not count.
boost::intrusive_ptr<CMeshBuffer> CMesh::getMeshBuffer(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return {};
    }
    return buffers[index].buffer;
}

// 0x6bbbb8 glitch::scene::CMesh::getMaterial(unsigned int) const
// PARTIAL: an index at or past the end gives a null handle. Inside the range the original bumps the
// material's count at +0x00. That bump is not performed (see getMeshBuffer).
boost::intrusive_ptr<video::CMaterial> CMesh::getMaterial(std::uint32_t index) const {
    if (index >= buffers.size()) {
        return {};
    }
    return buffers[index].material;
}

// 0x6bbc10 glitch::scene::CMesh::getMaterialVertexAttributeMap(unsigned int) const
// PARTIAL: an index at or past the end gives a null handle. Inside the range the original bumps the
// attribute map's count at +0x00. That bump is not performed (see getMeshBuffer).
boost::intrusive_ptr<video::CMaterialVertexAttributeMap> CMesh::getMaterialVertexAttributeMap(
        std::uint32_t index) const {
    if (index >= buffers.size()) {
        return {};
    }
    return buffers[index].vertexAttributeMap;
}

// 0x6bbd6c glitch::scene::CMesh::setMaterial(unsigned int, boost::intrusive_ptr<CMaterial> const&,
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
    slot.material = material;                      // +0x04
    slot.vertexAttributeMap = vertexAttributeMap;  // +0x08
}

// 0x6bc664 glitch::scene::CMesh::addMeshBuffer(boost::intrusive_ptr<CMeshBuffer> const&,
//          boost::intrusive_ptr<CMaterial> const&, boost::intrusive_ptr<CMaterialVertexAttributeMap> const&)
// PARTIAL: a null mesh buffer adds nothing, as in the original. Otherwise the original takes a count on
// the buffer, the material and the map (the material and map only when non-null), copies the three
// handles into a temporary SBuffer, pushes a copy into the vector, and then releases the temporary.
// The net effect is one stored entry with the three handles. The count moves are not performed here,
// so the entry holds the handles without counts (see getMeshBuffer).
void CMesh::addMeshBuffer(const boost::intrusive_ptr<CMeshBuffer>& buffer,
                          const boost::intrusive_ptr<video::CMaterial>& material,
                          const boost::intrusive_ptr<video::CMaterialVertexAttributeMap>& vertexAttributeMap) {
    if (buffer.get() == nullptr) {
        return;
    }
    SBuffer entry;
    entry.buffer = buffer;                         // +0x00
    entry.material = material;                     // +0x04
    entry.vertexAttributeMap = vertexAttributeMap; // +0x08
    buffers.push_back(entry);
}

// 0x6bc248 glitch::scene::CMesh::~CMesh()
// SKIPPED: the original sets the vtable, then destroys the SBuffer vector, which releases each buffer,
// material and map reference. The reference releases are not modelled in this category, so no body is
// written. The class has no user-declared destructor; the implicit one destroys the vector.

}  // namespace glitch::scene
