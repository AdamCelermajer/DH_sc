#include "glitch-video/c2d_driver.hpp"

namespace glitch::video {

// 0x59efb4 _ZN6glitch5video9C2DDriverC2EPNS0_12IVideoDriverE
// 0x59f008 _ZN6glitch5video9C2DDriverC1EPNS0_12IVideoDriverE
// The vtable pointer at +0x00 is set by the compiler from the class type.
C2DDriver::C2DDriver(IVideoDriver* driver)
    : m_unknown04(1),
      m_driver(driver),
      m_unknown0c(0),
      m_unknown10(0),
      m_unknown14(0),
      m_unknown18(0),
      m_unknown1c(0),
      m_unknown1e(0) {}

}  // namespace glitch::video
