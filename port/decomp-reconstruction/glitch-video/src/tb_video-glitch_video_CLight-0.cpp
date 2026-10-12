// Track B batch e1-glitch-video-glitch_video_CLight-0: glitch::video::CLight member functions.

#include <cstdint>
#include <cstring>

#include "glitch-video/deps/os_logging.hpp"
#include "glitch-video/light.hpp"

namespace glitch::video {

// 0x59fc00 _ZN6glitch5video6CLight25setAbsoluteTransformationERKNS_4core8CMatrix4IfEE
// PARTIAL: the destination is the pointer at +0x50. light.hpp declares that slot as a 32-bit
// uint32_t, so on a 64-bit host the stored pointer does not fit and the copy below truncates it.
// The control flow and copy size (0x41 bytes, as in the original) match the Hex-Rays body.
// Layout request: make CLight::m_sourceHandle a host-width pointer (and update the
// STransformationSource::detach and &m_sourceHandle uses in light.cpp to match).
// The 0x41-byte copy is one byte larger than sizeof(CMatrix4<float>) (64); kept as in the original.
int CLight::setAbsoluteTransformation(const glitch::core::CMatrix4<float>& transform) {
    if (m_flag54 != 0) {  // +0x54
        glitch::os::Printer::log("Can not set absolute transform on light attached to a transformation source",
                                 glitch::os::ELL_ERROR);
        return 0;
    }
    void* destination = reinterpret_cast<void*>(static_cast<std::uintptr_t>(m_sourceHandle));  // +0x50
    std::memcpy(destination, static_cast<const void*>(&transform), 0x41);
    return 1;
}

}  // namespace glitch::video
