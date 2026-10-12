// IRenderTarget viewport clamp. The rectangle words are read in the order UL.X, UL.Y, LR.X, LR.Y.
#include "glitch-video/render_target.hpp"

namespace glitch::video {

// 0x6dc5a8 glitch::video::IRenderTarget::setViewport(glitch::core::rect<int> const&)
void IRenderTarget::setViewport(const core::rect<int32_t>& rect) {
    m_viewport = rect;  // a1[5..8] = a2[0..3]

    // Far corner: x1 is limited to the width, y1 to the height.
    auto& ul = m_viewport.UpperLeftCorner;
    auto& lr = m_viewport.LowerRightCorner;
    if (m_width < lr.X) lr.X = m_width;
    if (m_height < lr.Y) lr.Y = m_height;

    // Near corner: x0 and y0 are raised to 0, then pulled back to the far corner.
    if (ul.X < 0) ul.X = 0;
    if (ul.Y < 0) ul.Y = 0;
    if (ul.Y > lr.Y) ul.Y = lr.Y;
    if (ul.X > lr.X) ul.X = lr.X;

    // Not recovered: the original then tests whether this target is the driver's current one
    // (driver +0xcc list, entry -4) and, if so, calls the driver virtual at vtable +0x1e8 with
    // &a1[5]. The slot and the list are not identified, so the clamped rectangle is kept here
    // and not sent to the driver.
}

}  // namespace glitch::video
