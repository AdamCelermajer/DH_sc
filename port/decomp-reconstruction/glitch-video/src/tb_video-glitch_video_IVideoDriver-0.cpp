#include "glitch-video/video_driver.hpp"

#include "glitch-video/light.hpp"
#include "glitch-video/global_parameter_manager.hpp"

namespace glitch::video {

// 0x5aaa40 _ZN6glitch5video12IVideoDriver15addDynamicLightERKN5boost13intrusive_ptrINS0_6CLightEEE
// Hex-Rays and the listing agree: when the limit (+0x3c) exceeds the count (+0x3a), the light is
// stored at parameter id (uint16)(count + base), where base is +0x38, with index 0. The count is
// incremented after the call and the call result is returned. Otherwise the limit is returned.
// Partial: the element setter 0x5be68c (IMaterialParameters<G>::setParameter<intrusive_ptr<CLight>>)
// is ledgered skipped (the CLight release is inlined with the Matrix4Pool). The null owner at
// +0xe4 is not checked, as in the original.
uint32_t IVideoDriver::addDynamicLight(const boost::intrusive_ptr<CLight>& light) {
    const uint16_t count = m_dynamicLightCount;  // +0x3a
    uint32_t result = m_dynamicLightLimit;       // +0x3c
    if (result > count) {
        result = m_globalParameters->setParameter(static_cast<uint16_t>(count + m_dynamicLightBase), 0u, light);
        ++m_dynamicLightCount;                   // +0x3a, read again after the call as in the listing
    }
    return result;
}

}  // namespace glitch::video
