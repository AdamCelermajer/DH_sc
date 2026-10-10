// P16 LEVELUP3: host test for the glitch CBillboardSceneNode camera basis (level_up.bdae sheet).
// Checks the decoded rule: the sheet's local Y (length 1140) maps to the camera's screen-up vector U,
// its normal faces the eye, the basis is orthonormal, and parent rotation does not change U.
#include "source_fx_billboard_basis_v1.hpp"

#include <cmath>
#include <cstdio>

using namespace dh2::fx;

namespace {

int failures = 0;

void check(bool ok, const char* what) {
    if (!ok) {
        ++failures;
        std::printf("FAIL: %s\n", what);
    }
}

bool near3(const float* a, const float* b, float eps = 1e-4f) {
    return std::fabs(a[0] - b[0]) < eps && std::fabs(a[1] - b[1]) < eps && std::fabs(a[2] - b[2]) < eps;
}

FxMatrix16 identity() {
    FxMatrix16 m{};
    m[0] = m[5] = m[10] = m[15] = 1;
    return m;
}

// Scene-space camera (outer = identity): view rows R, U, -F in the matrix (row-major rows, column-major
// storage), eye in the translation column as the rebuild path stores it.
FxMatrix16 camera_from(const float R[3], const float U[3], const float F[3], const float eye[3]) {
    FxMatrix16 m = identity();
    for (unsigned c = 0; c < 3; ++c) {
        m[c * 4 + 0] = R[c];
        m[c * 4 + 1] = U[c];
        m[c * 4 + 2] = -F[c];
    }
    m[12] = eye[0];
    m[13] = eye[1];
    m[14] = eye[2];
    return m;
}

FxBillboardRecordV1 sheet_record() {
    FxBillboardRecordV1 r{};
    r.mode = 1;
    r.sub = 1;
    r.axis_a[0] = 0; r.axis_a[1] = 0; r.axis_a[2] = 1;
    r.axis_b[0] = 0; r.axis_b[1] = 1; r.axis_b[2] = 0;
    return r;
}

void column(const FxMatrix16& m, float out[3], int c) {
    out[0] = m[c * 4 + 0];
    out[1] = m[c * 4 + 1];
    out[2] = m[c * 4 + 2];
}

// Tilted camera above and behind the origin: eye (0,-800,800), forward (0,0.707,-0.707), up (0,0.707,0.707).
const float kR[3] = {1, 0, 0};
const float kU[3] = {0, 0.70710678f, 0.70710678f};
const float kF[3] = {0, 0.70710678f, -0.70710678f};
const float kEye[3] = {0, -800, 800};
const float kD[3] = {0, -0.70710678f, 0.70710678f};  // toward the eye from the origin
const float kS[3] = {-1, 0, 0};                      // S = D x C (column 0 is -S = screen right)

void test_identity_parent_faces_camera() {
    const auto camera = camera_from(kR, kU, kF, kEye);
    FxMatrix16 out{};
    const bool ok = source_fx_billboard_absolute_v1(sheet_record(), identity(), false, identity(), camera, out);
    check(ok, "identity-parent billboard basis is accepted");
    float col0[3], col1[3], col2[3];
    column(out, col0, 0);
    column(out, col1, 1);
    column(out, col2, 2);
    const float right[3] = {-kS[0], -kS[1], -kS[2]};
    check(near3(col0, right), "local X maps to -S = screen right");
    check(near3(col1, kU), "local Y (sheet length) maps to camera up U: the column is vertical on screen");
    check(near3(col2, kD), "sheet normal faces the eye (D)");
    check(col1[2] > 0.7f, "vertical column has a positive world-up component");
    // Orthonormal rotation part.
    check(std::fabs(col0[0] * col0[0] + col0[1] * col0[1] + col0[2] * col0[2] - 1) < 1e-4f, "col0 unit");
    check(std::fabs(col1[0] * col1[0] + col1[1] * col1[1] + col1[2] * col1[2] - 1) < 1e-4f, "col1 unit");
    check(std::fabs(col2[0] * col2[0] + col2[1] * col2[1] + col2[2] * col2[2] - 1) < 1e-4f, "col2 unit");
    // The sheet's top point (0,1140,0) lands 1140 units above the origin along U.
    const float top_local[4] = {0, 1140, 0, 1};
    float top[3];
    for (unsigned r = 0; r < 3; ++r) top[r] = out[0 * 4 + r] * top_local[0] + out[1 * 4 + r] * top_local[1] + out[2 * 4 + r] * top_local[2] + out[12 + r];
    const float expected_top[3] = {1140 * kU[0], 1140 * kU[1], 1140 * kU[2]};
    check(near3(top, expected_top, 1e-2f), "sheet top point is 1140 units along U from the origin");
}

void test_parent_rotation_does_not_change_up() {
    // Parent rotated 90 degrees about Z (no translation): the billboard ignores the parent rotation for U.
    FxMatrix16 parent = identity();
    parent[0] = 0; parent[1] = 1;   // column 0 = (0,1,0)
    parent[4] = -1; parent[5] = 0;  // column 1 = (-1,0,0)
    const auto camera = camera_from(kR, kU, kF, kEye);
    FxMatrix16 out{};
    bool ok = source_fx_billboard_absolute_v1(sheet_record(), parent, true, identity(), camera, out);
    check(ok, "rotated-parent billboard basis is accepted");
    float col1[3];
    column(out, col1, 1);
    check(near3(col1, kU), "parent rotation leaves the sheet up on the camera up vector");
    // Translated parent: the billboard origin is the parent absolute position.
    FxMatrix16 moved = identity();
    moved[12] = 100; moved[13] = 200; moved[14] = 300;
    ok = source_fx_billboard_absolute_v1(sheet_record(), moved, true, identity(), camera_from(kR, kU, kF, kEye), out);
    check(ok, "translated-parent billboard basis is accepted");
    check(std::fabs(out[12] - 100) < 1e-4f && std::fabs(out[13] - 200) < 1e-4f && std::fabs(out[14] - 300) < 1e-4f,
          "billboard origin is the parent absolute position");
}

void test_unsupported_and_degenerate() {
    const auto camera = camera_from(kR, kU, kF, kEye);
    FxMatrix16 out{};
    FxBillboardRecordV1 mode2 = sheet_record();
    mode2.mode = 2;
    check(!source_fx_billboard_supported_v1(mode2), "mode 2 is not decoded (plain node fallback)");
    FxBillboardRecordV1 sub2 = sheet_record();
    sub2.sub = 2;
    check(!source_fx_billboard_supported_v1(sub2), "sub 2 is not decoded (plain node fallback)");
    check(source_fx_billboard_supported_v1(sheet_record()), "mode 1 sub 1 (level-up record) is supported");

    // Eye at the origin: no view direction, the basis is degenerate.
    const float eye_at_origin[3] = {0, 0, 0};
    const auto degenerate = camera_from(kR, kU, kF, eye_at_origin);
    check(!source_fx_billboard_absolute_v1(sheet_record(), identity(), false, identity(), degenerate, out),
          "eye at the billboard origin is rejected");

    // Inverse helper round trip.
    FxMatrix16 inv{};
    check(source_fx_mat16_inverse_affine_v1(camera, inv), "camera matrix is invertible");
    const auto product = source_fx_mat16_multiply_v1(camera, inv);
    check(std::fabs(product[0] - 1) < 1e-4f && std::fabs(product[5] - 1) < 1e-4f && std::fabs(product[10] - 1) < 1e-4f,
          "camera * inverse(camera) is identity");
}

}  // namespace

int main() {
    test_identity_parent_faces_camera();
    test_parent_rotation_does_not_change_up();
    test_unsupported_and_degenerate();
    if (failures == 0) std::printf("source_fx_billboard_basis_v1: all checks passed\n");
    return failures == 0 ? 0 : 1;
}
