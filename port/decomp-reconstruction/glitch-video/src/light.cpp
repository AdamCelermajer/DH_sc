#include "glitch-video/light.hpp"

#include <cfloat>
#include <cstring>

namespace glitch::video {

// 0x59fc40 _ZN6glitch5video6CLightC1ERKS1_PPNS0_21STransformationSourceE
// 0x59fd48 _ZN6glitch5video6CLightC2ERKS1_PPNS0_21STransformationSourceE
// Copies every field except the count (+0x00), the source handle (+0x50) and the flag
// at +0x54. The original copies +0x04..+0x4c as words, then +0x58 (halfword) and +0x5a (byte).
CLight::CLight(const CLight& source, STransformationSource** out) {
    m_refCount = 0;
    std::memcpy(m_word04, source.m_word04, sizeof(m_word04));
    std::memcpy(m_float10, source.m_float10, sizeof(m_float10));
    std::memcpy(m_word38, source.m_word38, sizeof(m_word38));
    m_float40 = source.m_float40;
    m_word44 = source.m_word44;
    m_float48 = source.m_float48;
    m_float4c = source.m_float4c;
    m_sourceHandle = 0;
    m_flag54 = 1;
    m_word58 = source.m_word58;
    m_flag5a = source.m_flag5a;

    if (out) {
        *out = reinterpret_cast<STransformationSource*>(&m_sourceHandle);
    } else {
        STransformationSource::detach(&m_sourceHandle);
    }
}

// 0x59fe08 _ZN6glitch5video6CLightC1EPPNS0_21STransformationSourceE
// 0x59fef8 _ZN6glitch5video6CLightC2EPPNS0_21STransformationSourceE
CLight::CLight(STransformationSource** out) {
    m_refCount = 0;
    m_word04[0] = m_word04[1] = m_word04[2] = 0;
    for (float& f : m_float10) {
        f = 1.0f;
    }
    m_word38[0] = m_word38[1] = 0;
    m_float40 = FLT_MAX;
    m_word44 = 0;
    m_float48 = 40.0f;
    m_float4c = 2.0f;
    m_sourceHandle = 0;
    m_flag54 = 1;
    m_word58 = 0;
    m_flag5a = 1;

    if (out) {
        *out = reinterpret_cast<STransformationSource*>(&m_sourceHandle);
    } else {
        STransformationSource::detach(&m_sourceHandle);
    }
}

// 0x59fd00 _ZNK6glitch5video6CLight5cloneEPPNS0_21STransformationSourceE
// The original stores the new object's count as previous count + 1 (0 + 1).
boost::intrusive_ptr<CLight> CLight::clone(STransformationSource** out) const {
    CLight* copy = new CLight(*this, out);
    copy->m_refCount += 1;
    return boost::intrusive_ptr<CLight>(copy);
}

// 0x59feb8 _ZN6glitch5video6CLight8allocateEPPNS0_21STransformationSourceE
boost::intrusive_ptr<CLight> CLight::allocate(STransformationSource** out) {
    CLight* light = new CLight(out);
    light->m_refCount += 1;
    return boost::intrusive_ptr<CLight>(light);
}

}  // namespace glitch::video
