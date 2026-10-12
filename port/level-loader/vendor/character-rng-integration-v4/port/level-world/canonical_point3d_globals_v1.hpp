#pragma once
#include <array>
namespace dh2::world {
// Actual _GLOBAL__I_Utils_Point3D.cpp312e00 stores these source shared globals.
// Same immutable native projection available to properties and loot geometry.
inline const std::array<float,3>& canonical_vec3_origin_v1()noexcept {static const std::array<float,3> value{{0.f,0.f,0.f}};return value;}
inline const std::array<float,3>& canonical_vec3_i_v1()noexcept {static const std::array<float,3> value{{1.f,0.f,0.f}};return value;}
inline const std::array<float,3>& canonical_vec3_j_v1()noexcept {static const std::array<float,3> value{{0.f,1.f,0.f}};return value;}
inline const std::array<float,3>& canonical_vec3_k_v1()noexcept {static const std::array<float,3> value{{0.f,0.f,1.f}};return value;}
}
