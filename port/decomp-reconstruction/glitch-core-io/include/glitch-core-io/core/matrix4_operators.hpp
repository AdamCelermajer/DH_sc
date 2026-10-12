// Out-of-class definitions for glitch::core::CMatrix4 operators that need the CMatrix4Base products.
// Include this header wherever CMatrix4::operator* is used; the declaration is in deps/glitch_core.hpp.
#pragma once

#include "glitch-core-io/core/matrix4_base.hpp"
#include "glitch-core-io/deps/glitch_core.hpp"

namespace glitch::core {

// 0x35e998 CMatrix4<float>::operator*(const CMatrix4&) const. The original copies the result of
// detail::CMatrix4Base<float>::mult (0x35e118) field by field, flag included, so this returns it.
template <typename T>
CMatrix4<T> CMatrix4<T>::operator*(const CMatrix4<T>& other) const {
    return detail::mult(*this, other);  // float only: mult is declared for CMatrix4<float>
}

// 0x6651a0 operator*<float, glitch::collada::SMatrix>(CMatrix4<float> const&, SMatrix const&): lhs
// times a 16-value rhs (the SMatrix). The rhs is read as 16 values at indices 0..15 only. Declared
// here; the definition is in src/tb_core-io-glitch_core-0.cpp. The rhs type is a template parameter
// because glitch::collada::SMatrix is not declared in this tree.
template <typename T, typename Rhs>
CMatrix4<T> operator*(const CMatrix4<T>& lhs, const Rhs& rhs);

}  // namespace glitch::core
