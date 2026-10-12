#include "glitch-video/generic_baker.hpp"

#include "glitch-video/vertex_attribute_map.hpp"

namespace glitch::video {

// 0x609d28 _ZN6glitch5video13CGenericBakerC1EPKNS0_7IShaderE
// 0x609db4 _ZN6glitch5video13CGenericBakerC2EPKNS0_7IShaderE
// The original passes a null streams pointer to the streams constructor (0x5a0958). Its null
// branch only leaves the map unset, which the streams constructor here does with a null
// stream and a count of zero.
CGenericBaker::CGenericBaker(const IShader* shader) : m_shader(shader) {
    m_attributeMap = new CVertexAttributeMap(boost::intrusive_ptr<const CVertexStreams>(), 0, nullptr, false);
    ++m_attributeMap->m_unknown00;
}

}  // namespace glitch::video
