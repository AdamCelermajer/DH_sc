// Minimal stand-in for glitch::core::vector3d<float> (the Irrlicht-derived math type the
// game passes by const reference). Only the three components are modelled; the original's
// operators and constructors are not needed by the vox-audio bodies that use it.
#pragma once

namespace glitch {
namespace core {

template <typename T>
struct vector3d {
    T X;  // +0x00
    T Y;  // +0x04
    T Z;  // +0x08
};

}  // namespace core
}  // namespace glitch
