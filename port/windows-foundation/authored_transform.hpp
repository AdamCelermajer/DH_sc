#pragma once
#include "renderer.hpp"
#include "../engine-math/math.hpp"
#include <array>
#include <cmath>
#include <stdexcept>

namespace dh::foundation {
// VisualObject::SetRotation (0x472874) -> ISceneNode::getRelativeTransformation
// (0x598908): authored degrees Y,-X,-Z, quaternion getMatrix_transposed, postScale.
inline Mat4 authored_transform(const std::array<float, 3>& position,
                              const std::array<float, 3>& rotation,
                              std::array<float, 3> scale) {
    constexpr float radians = 0.01745329251994329577f;
    dh2::math::Quaternion q{};
    dh2_quat_from_euler(&q, rotation[1]*radians, -rotation[0]*radians, -rotation[2]*radians);
    dh2::math::Matrix4f matrix{};
    dh2_quat_matrix_transposed(&q, &matrix);
    Mat4 result{};
    for (unsigned c=0; c<3; ++c) {
        if (std::abs(scale[c]) < 0.0001f) scale[c]=1;
        for (unsigned row=0; row<3; ++row) matrix.m[c*4+row]*=scale[c];
        matrix.m[12+c]=position[c];
    }
    for (unsigned i=0; i<16; ++i) {
        if (!std::isfinite(matrix.m[i])) throw std::runtime_error("Authored transform overflow");
        result[i]=matrix.m[i];
    }
    return result;
}
}
