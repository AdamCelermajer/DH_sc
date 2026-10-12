#include "glitch-scene-collada-gui/ps_types.hpp"

#include <cstdlib>

namespace glitch::ps {

// 0x69ace4 _ZN6glitch2ps7PDPointC2ERKNS_4core8vector3dIfEE
// 0x69ad38 _ZN6glitch2ps7PDPointC1ERKNS_4core8vector3dIfEE
PDPoint::PDPoint(const core::vector3d<float>& value) : point(value) {}

// 0x69ad8c _ZNK6glitch2ps7PDPoint6withinERKNS_4core8vector3dIfEE
bool PDPoint::within(const core::vector3d<float>& value) const {
    return point.X == value.X && point.Y == value.Y && point.Z == value.Z;
}

// 0x69ade8 _ZNK6glitch2ps7PDPoint8generateERNS0_8PSRandomE
core::vector3d<float> PDPoint::generate(PSRandom& /*random*/) const {
    return point;
}

// 0x69ac94 _ZNK6glitch2ps7PDPoint7getTypeEv
std::uint32_t PDPoint::getType() const {
    return 3;
}

// 0x69ae4c _ZNK6glitch2ps7PDPoint4sizeEv
float PDPoint::size() const {
    return 1.0f;
}

// 0x69aca0 _ZNK6glitch2ps6PDLine7getTypeEv
std::uint32_t PDLine::getType() const {
    return 4;
}

// 0x69aec4 _ZNK6glitch2ps6PDLine4sizeEv
float PDLine::size() const {
    return sizeValue;
}

// 0x69acac _ZNK6glitch2ps5PDBox7getTypeEv
std::uint32_t PDBox::getType() const {
    return 0;
}

// 0x69b660 _ZNK6glitch2ps5PDBox4sizeEv
float PDBox::size() const {
    return sizeValue;
}

// 0x69acb8 _ZNK6glitch2ps10PDCylinder7getTypeEv
std::uint32_t PDCylinder::getType() const {
    return 2;
}

// 0x69b7fc _ZNK6glitch2ps10PDCylinder4sizeEv
float PDCylinder::size() const {
    return sizeValue;
}

// 0x69acc4 _ZNK6glitch2ps6PDCone7getTypeEv
std::uint32_t PDCone::getType() const {
    return 5;
}

// 0x69b9c0 _ZN6glitch2ps6PDCone9transformERKNS_4core8CMatrix4IfEE
void PDCone::transform(const core::CMatrix4<float>& /*matrix*/) {}

// 0x69b9c4 _ZNK6glitch2ps6PDCone4sizeEv
float PDCone::size() const {
    return sizeValue;
}

// 0x69acd0 _ZNK6glitch2ps8PDSphere7getTypeEv
std::uint32_t PDSphere::getType() const {
    return 1;
}

// 0x69be08 _ZNK6glitch2ps8PDSphere4sizeEv
float PDSphere::size() const {
    return sizeValue;
}

// 0x69acdc _ZNK6glitch2ps6PDBlob7getTypeEv
std::uint32_t PDBlob::getType() const {
    return 6;
}

// 0x69bf7c _ZNK6glitch2ps6PDBlob4sizeEv
float PDBlob::size() const {
    return 1.0f;
}

// 0x64d290 _ZN6glitch2ps6PS_NEWEj
void* PS_NEW(std::uint32_t size) {
    return std::malloc(size);
}

// 0x64d778 _ZN6glitch2ps7PS_FREEEPv
void PS_FREE(void* p) {
    std::free(p);
}

}  // namespace glitch::ps
