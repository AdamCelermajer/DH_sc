// glitch::video::CImage. Only the members needed by getDimension are laid out
// here; the rest of the class is not reconstructed yet.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_core.hpp"

namespace glitch::video {

class CImage {
public:
    CImage() = default;
    // Port helper (not an original function): sets the base size.
    CImage(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

    // Dimension of the given mip level; zero-sized axes stay zero.
    core::dimension2d<uint32_t> getDimension(uint32_t mipLevel) const;

private:
    // Members other than these two are not reconstructed.
    uint32_t m_width = 0;   // +0x10
    uint32_t m_height = 0;  // +0x14
};

}  // namespace glitch::video
