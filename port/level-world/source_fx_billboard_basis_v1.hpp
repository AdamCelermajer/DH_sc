#pragma once
#include <array>
#include <cstdint>

// P16 LEVELUP3: glitch CBillboardSceneNode camera basis, split out of source_fx_node_matrix_v4.cpp so the
// math has an isolated host test (tests/source_fx_billboard_basis_v1_tests.cpp).
//
// Decoded from libDungeonHunter2.so: CColladaDatabase::constructNode 0x61b2f4 (SNode+76 != 0 selects
// createBillboard), CBillboardSceneNode::updateAbsolutePosition 0x60cf88 (target branch: mode != 2, sub != 2).
// Per frame, with O = parent absolute position, D = normalize(eye - O), C = camera up (row 1 of the
// world-to-eye view matrix, i.e. m1,m5,m9), S = normalize(D x C), U = normalize(S x D), F = [-S, U, D]:
//   A, B = parent-rotated record axes, P = normalize(B x A), Q = normalize(A x P), Frame = [P Q A]
//   absolute = T(O) * F * Frame^T * Mrot(parent) * local
// Matrices are column-major (m[col*4+row]), as the rest of the FX graph code.
namespace dh2::fx {

using FxMatrix16 = std::array<float, 16>;

struct FxBillboardRecordV1 {
    std::int32_t mode = 0;
    std::int32_t sub = 0;
    float axis_a[3] = {0, 0, 1};
    float axis_b[3] = {0, 1, 0};
};

// Modes 2 or sub 2 are not decoded; callers keep the plain TRS composition for them.
bool source_fx_billboard_supported_v1(const FxBillboardRecordV1& record) noexcept;

FxMatrix16 source_fx_mat16_multiply_v1(const FxMatrix16& a, const FxMatrix16& b) noexcept;

// Affine inverse; false when the 3x3 part is singular.
bool source_fx_mat16_inverse_affine_v1(const FxMatrix16& m, FxMatrix16& out) noexcept;

// Billboard absolute matrix for one node. camera_scene16 is the world-to-eye view matrix already
// multiplied by the FX outer transform (scene space), with the eye position in its translation column.
// Returns false for a degenerate basis; the caller then keeps the plain TRS composition.
bool source_fx_billboard_absolute_v1(const FxBillboardRecordV1& record, const FxMatrix16& parent_world,
    bool has_parent, const FxMatrix16& local, const FxMatrix16& camera_scene, FxMatrix16& out) noexcept;

}  // namespace dh2::fx
