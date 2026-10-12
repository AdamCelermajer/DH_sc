#include "glitch-video/image.hpp"

namespace glitch::video {

namespace {

// Halves a mip dimension, clamping at 1. Non-positive sizes are returned
// unchanged (the original tests the signed value before shifting).
uint32_t mipDimension(uint32_t size, uint32_t mipLevel) {
    if (static_cast<int32_t>(size) <= 0) {
        return size;
    }
    uint32_t shifted = size >> mipLevel;
    return shifted == 0 ? 1u : shifted;
}

}  // namespace

// 0x5ff638 _ZNK6glitch5video6CImage12getDimensionEj
core::dimension2d<uint32_t> CImage::getDimension(uint32_t mipLevel) const {
    core::dimension2d<uint32_t> result;
    result.Width = mipDimension(m_width, mipLevel);
    result.Height = mipDimension(m_height, mipLevel);
    return result;
}

}  // namespace glitch::video
