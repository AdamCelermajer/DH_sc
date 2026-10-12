#include "glitch-scene-collada-gui/ps_types.hpp"

namespace glitch::ps {

// 0x69ac94 _ZNK6glitch2ps7PDPoint7getTypeEv
std::uint32_t PDPoint::getType() const {
    return 3;
}

// 0x69aca0 _ZNK6glitch2ps6PDLine7getTypeEv
std::uint32_t PDLine::getType() const {
    return 4;
}

// 0x69acac _ZNK6glitch2ps5PDBox7getTypeEv
std::uint32_t PDBox::getType() const {
    return 0;
}

// 0x69acb8 _ZNK6glitch2ps10PDCylinder7getTypeEv
std::uint32_t PDCylinder::getType() const {
    return 2;
}

}  // namespace glitch::ps
