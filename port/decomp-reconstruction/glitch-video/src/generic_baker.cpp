#include "glitch-video/generic_baker.hpp"

#include "glitch-video/unreconstructed.hpp"
#include "glitch-video/vertex_attribute_map.hpp"

namespace glitch::video {

// 0x609d28 _ZN6glitch5video13CGenericBakerC1EPKNS0_7IShaderE
// 0x609db4 _ZN6glitch5video13CGenericBakerC2EPKNS0_7IShaderE
// The original passes a null streams pointer to the streams constructor (0x5a0958). Its null
// branch only leaves the map unset, which the streams constructor here does with a null
// stream and a count of zero.
CGenericBaker::CGenericBaker(const IShader* shader) : m_shader(shader) {
    referenceCounter = 0;  // +0x04
    m_attributeMap = boost::intrusive_ptr<CVertexAttributeMap>(
        new CVertexAttributeMap(boost::intrusive_ptr<const CVertexStreams>(), 0, nullptr, false), true);
}

// 0x60ab18 / 0x609e40 / 0x608958 / 0x609ba4 / 0x60a188: the IBatchBaker slots, not reconstructed.
void CGenericBaker::bake(const boost::intrusive_ptr<const CVertexStreams>&, const CPrimitiveStream&, const CMaterial*, uint8_t,
                         const uint8_t*, const boost::intrusive_ptr<CVertexStreams>&, CPrimitiveStream&, const CMaterial*,
                         const uint8_t*, const IVideoDriver*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
                         uint32_t) const {
    detail::unreconstructed("CGenericBaker::bake", 0x60ab18);
}
boost::intrusive_ptr<CMaterial> CGenericBaker::getBatchMaterial(const boost::intrusive_ptr<const CMaterial>&, uint8_t) const {
    detail::unreconstructed("CGenericBaker::getBatchMaterial", 0x609e40);
}
// 0x608958: returns the address of the attribute map slot at +0x0c.
const boost::intrusive_ptr<CVertexAttributeMap>& CGenericBaker::getVertexAttributeMap() const { return m_attributeMap; }
void CGenericBaker::configureAppendBuffer(const boost::intrusive_ptr<scene::CAppendMeshBuffer>&) const {
    detail::unreconstructed("CGenericBaker::configureAppendBuffer", 0x609ba4);
}
bool CGenericBaker::isCompatible(const boost::intrusive_ptr<const CMaterial>&, uint8_t) const {
    detail::unreconstructed("CGenericBaker::isCompatible", 0x60a188);
}

}  // namespace glitch::video
