#include "source_fx_billboard_basis_v1.hpp"

#include <cmath>
#include <cstring>

namespace dh2::fx {
namespace {

void normalize3(float v[3]) {
    const float l = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (l > 0) {
        v[0] /= l;
        v[1] /= l;
        v[2] /= l;
    }
}

void cross3(const float a[3], const float b[3], float o[3]) {
    o[0] = a[1] * b[2] - a[2] * b[1];
    o[1] = a[2] * b[0] - a[0] * b[2];
    o[2] = a[0] * b[1] - a[1] * b[0];
}

bool nonzero3(const float v[3]) {
    return std::fabs(v[0]) + std::fabs(v[1]) + std::fabs(v[2]) > 1e-9f;
}

}  // namespace

bool source_fx_billboard_supported_v1(const FxBillboardRecordV1& record) noexcept {
    return record.mode != 2 && record.sub != 2;
}

FxMatrix16 source_fx_mat16_multiply_v1(const FxMatrix16& a, const FxMatrix16& b) noexcept {
    FxMatrix16 o{};
    for (unsigned c = 0; c < 4; ++c)
        for (unsigned r = 0; r < 4; ++r) {
            float s = 0;
            for (unsigned k = 0; k < 4; ++k) s += a[k * 4 + r] * b[c * 4 + k];
            o[c * 4 + r] = s;
        }
    return o;
}

bool source_fx_mat16_inverse_affine_v1(const FxMatrix16& m, FxMatrix16& out) noexcept {
    const float a = m[0], b = m[4], c = m[8], d = m[1], e = m[5], f = m[9], g = m[2], h = m[6], i = m[10];
    const float det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
    if (!(std::fabs(det) > 1e-12f)) return false;
    const float k = 1.0f / det;
    const float r00 = (e * i - f * h) * k, r01 = (c * h - b * i) * k, r02 = (b * f - c * e) * k;
    const float r10 = (f * g - d * i) * k, r11 = (a * i - c * g) * k, r12 = (c * d - a * f) * k;
    const float r20 = (d * h - e * g) * k, r21 = (b * g - a * h) * k, r22 = (a * e - b * d) * k;
    out = {};
    out[0] = r00; out[1] = r10; out[2] = r20;
    out[4] = r01; out[5] = r11; out[6] = r21;
    out[8] = r02; out[9] = r12; out[10] = r22;
    const float tx = m[12], ty = m[13], tz = m[14];
    out[12] = -(r00 * tx + r01 * ty + r02 * tz);
    out[13] = -(r10 * tx + r11 * ty + r12 * tz);
    out[14] = -(r20 * tx + r21 * ty + r22 * tz);
    out[15] = 1;
    return true;
}

bool source_fx_billboard_absolute_v1(const FxBillboardRecordV1& record, const FxMatrix16& parent_world,
    bool has_parent, const FxMatrix16& local, const FxMatrix16& camera_scene, FxMatrix16& out) noexcept {
    float O[3] = {0, 0, 0};
    FxMatrix16 mrot{};
    mrot[0] = mrot[5] = mrot[10] = mrot[15] = 1;
    if (has_parent) {
        O[0] = parent_world[12];
        O[1] = parent_world[13];
        O[2] = parent_world[14];
        mrot = parent_world;
        mrot[12] = mrot[13] = mrot[14] = 0;
        mrot[15] = 1;
    }
    auto rotate = [&](const float v[3], float o[3]) {
        for (unsigned k = 0; k < 3; ++k) o[k] = mrot[0 + k] * v[0] + mrot[4 + k] * v[1] + mrot[8 + k] * v[2];
    };
    float A[3], B[3], P[3], Q[3], D[3], S[3], U[3], C[3];
    rotate(record.axis_a, A);
    rotate(record.axis_b, B);
    if (!nonzero3(A) || !nonzero3(B)) return false;
    normalize3(A);
    normalize3(B);
    cross3(B, A, P);
    if (!nonzero3(P)) return false;
    normalize3(P);
    cross3(A, P, Q);
    if (!nonzero3(Q)) return false;
    normalize3(Q);
    C[0] = camera_scene[1];
    C[1] = camera_scene[5];
    C[2] = camera_scene[9];
    for (unsigned k = 0; k < 3; ++k) D[k] = camera_scene[12 + k] - O[k];
    if (!nonzero3(D)) return false;
    normalize3(D);
    cross3(D, C, S);
    if (!nonzero3(S)) return false;
    normalize3(S);
    cross3(S, D, U);
    if (!nonzero3(U)) return false;
    normalize3(U);
    // Frame^T (orthonormal inverse of [P Q A]): column j = (P[j], Q[j], A[j]).
    FxMatrix16 frame_t{};
    for (unsigned j = 0; j < 3; ++j) {
        frame_t[j * 4 + 0] = P[j];
        frame_t[j * 4 + 1] = Q[j];
        frame_t[j * 4 + 2] = A[j];
    }
    frame_t[15] = 1;
    // F = [-S, U, D] columns.
    FxMatrix16 facing{};
    for (unsigned k = 0; k < 3; ++k) {
        facing[0 + k] = -S[k];
        facing[4 + k] = U[k];
        facing[8 + k] = D[k];
    }
    facing[15] = 1;
    FxMatrix16 translate{};
    translate[0] = translate[5] = translate[10] = translate[15] = 1;
    translate[12] = O[0];
    translate[13] = O[1];
    translate[14] = O[2];
    out = source_fx_mat16_multiply_v1(
        source_fx_mat16_multiply_v1(source_fx_mat16_multiply_v1(translate, facing),
                                    source_fx_mat16_multiply_v1(frame_t, mrot)),
        local);
    return true;
}

}  // namespace dh2::fx
