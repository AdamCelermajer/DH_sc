#include "glitch-video/light.hpp"

#include <cfloat>
#include <cstring>

#include "glitch-video/deps/os_logging.hpp"

namespace glitch::video {

// 0x59fc40 _ZN6glitch5video6CLightC1ERKS1_PPNS0_21STransformationSourceE
// 0x59fd48 _ZN6glitch5video6CLightC2ERKS1_PPNS0_21STransformationSourceE
// Copies every field except the count (+0x00), the transformation slot (+0x50) and the flag
// at +0x54. The original copies +0x04..+0x4c as words, then +0x58 (halfword) and +0x5a (byte).
CLight::CLight(const CLight& source, STransformationSource** out) {
    m_refCount = 0;
    m_ambientColor = source.m_ambientColor;
    m_diffuseColor = source.m_diffuseColor;
    m_specularColor = source.m_specularColor;
    m_attenuation = source.m_attenuation;
    m_radius = source.m_radius;
    m_innerConeAngle = source.m_innerConeAngle;
    m_outerConeAngle = source.m_outerConeAngle;
    m_falloff = source.m_falloff;
    m_transformationSource.matrix = nullptr;
    m_transformationSource.attached = true;
    m_type = source.m_type;
    m_castShadows = source.m_castShadows;

    if (out) {
        *out = &m_transformationSource;
    } else {
        m_transformationSource.detach();
    }
}

// 0x59fe08 _ZN6glitch5video6CLightC1EPPNS0_21STransformationSourceE
// 0x59fef8 _ZN6glitch5video6CLightC2EPPNS0_21STransformationSourceE
CLight::CLight(STransformationSource** out) {
    m_refCount = 0;
    m_ambientColor = SColorf{0.0f, 0.0f, 0.0f, 1.0f};
    m_diffuseColor = SColorf{1.0f, 1.0f, 1.0f, 1.0f};
    m_specularColor = SColorf{1.0f, 1.0f, 1.0f, 1.0f};
    m_attenuation.X = 1.0f;
    m_attenuation.Y = 0.0f;
    m_attenuation.Z = 0.0f;
    m_radius = FLT_MAX;
    m_innerConeAngle = 0.0f;
    m_outerConeAngle = 40.0f;
    m_falloff = 2.0f;
    m_transformationSource.matrix = nullptr;
    m_transformationSource.attached = true;
    m_type = 0;
    m_castShadows = 1;

    if (out) {
        *out = &m_transformationSource;
    } else {
        m_transformationSource.detach();
    }
}

CLight::~CLight() {
    if (!m_transformationSource.attached) {
        delete m_transformationSource.matrix;  // the original pushes the block back onto the Matrix4Pool
    }
}

// 0x5846f8 _ZN6glitch5video21STransformationSource6detachEv
// Partial: the block comes from new, not from the global Matrix4Pool free list.
void STransformationSource::detach() {
    if (!attached) return;
    attached = false;
    const core::CMatrix4<float>* old = matrix;
    core::CMatrix4<float>* block = new core::CMatrix4<float>(core::CMatrix4<float>::EM4CONST_IDENTITY);
    if (old != nullptr) {
        std::memcpy(static_cast<void*>(block), static_cast<const void*>(old), 0x41);
    }
    matrix = block;
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

// 0x59fc00 _ZN6glitch5video6CLight24setAbsoluteTransformationERKNS_4core9CMatrix4IfEE
int CLight::setAbsoluteTransformation(const glitch::core::CMatrix4<float>& transform) {
    if (m_transformationSource.attached) {
        os::Printer::log("Can not set absolute transform on light attached to a transformation source",
                         os::ELL_ERROR);
        return 0;
    }
    std::memcpy(static_cast<void*>(m_transformationSource.matrix), static_cast<const void*>(&transform), 0x41);
    return 1;
}

}  // namespace glitch::video
