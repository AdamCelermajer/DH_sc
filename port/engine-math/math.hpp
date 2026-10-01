#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

// Independently reconstructed from the supplied engine's ARM instructions.
// These are explicit port types, not recovered original studio headers.
namespace dh2::math {
struct Vector3f { float x, y, z; };
struct Quaternion { float x, y, z, w; };
struct Matrix4f {
    float m[16];
    std::uint8_t identity_hint;
};
static_assert(sizeof(Vector3f) == 12 && sizeof(Quaternion) == 16);
static_assert(offsetof(Matrix4f, identity_hint) == 64 && sizeof(Matrix4f) == 68);
static_assert(std::is_standard_layout_v<Matrix4f>);
}

// C linkage gives a stable port/test interface across ARM32, ARM64 and host
// builds. Callers use the C++ types above. Output objects must be disjoint from
// inputs, except the explicitly mutating operations. Pointers must be valid.
extern "C" {
dh2::math::Vector3f* dh2_vec3_normalize(dh2::math::Vector3f* value);
dh2::math::Vector3f* dh2_vec3_divide(dh2::math::Vector3f* out,
    const dh2::math::Vector3f* a, const dh2::math::Vector3f* b);
dh2::math::Vector3f* dh2_vec3_divide_assign(dh2::math::Vector3f* a,
    const dh2::math::Vector3f* b);
void dh2_vec3_rotate_xy(dh2::math::Vector3f* value, double degrees,
    const dh2::math::Vector3f* center);
void dh2_vec3_rotate_yz(dh2::math::Vector3f* value, double degrees,
    const dh2::math::Vector3f* center);
void dh2_vec3_rotate_xz(dh2::math::Vector3f* value, double degrees,
    const dh2::math::Vector3f* center);
dh2::math::Vector3f* dh2_vec3_horizontal_angles(dh2::math::Vector3f* out,
    const dh2::math::Vector3f* value);

dh2::math::Quaternion* dh2_quat_normalize(dh2::math::Quaternion* value);
dh2::math::Quaternion* dh2_quat_from_euler(dh2::math::Quaternion* out,
    float x_radians, float y_radians, float z_radians);
dh2::math::Quaternion* dh2_quat_from_angle_axis(dh2::math::Quaternion* out,
    float radians, const dh2::math::Vector3f* axis);
// Preserves the compiler-specialized clone's literal sine/cosine constants.
dh2::math::Quaternion* dh2_quat_from_fixed_angle_axis(
    dh2::math::Quaternion* out, const dh2::math::Vector3f* axis);
dh2::math::Quaternion* dh2_quat_multiply(dh2::math::Quaternion* out,
    const dh2::math::Quaternion* a, const dh2::math::Quaternion* b);
dh2::math::Vector3f* dh2_quat_transform_vector(dh2::math::Vector3f* out,
    const dh2::math::Quaternion* q, const dh2::math::Vector3f* value);
void dh2_quat_matrix(const dh2::math::Quaternion* q, dh2::math::Matrix4f* out);
void dh2_quat_matrix_transposed(const dh2::math::Quaternion* q,
    dh2::math::Matrix4f* out);
// The original value-returning overload calls getMatrix_transposed, whereas
// the reference-output overload calls getMatrix. Preserve that distinction.
dh2::math::Matrix4f* dh2_quat_matrix_value(dh2::math::Matrix4f* out,
    const dh2::math::Quaternion* q);
dh2::math::Quaternion* dh2_quat_from_matrix(dh2::math::Quaternion* out,
    const dh2::math::Matrix4f* matrix);
dh2::math::Vector3f* dh2_matrix_rotation_degrees(dh2::math::Vector3f* out,
    const dh2::math::Matrix4f* matrix);
void dh2_quat_to_euler_degrees(const dh2::math::Quaternion* q,
    dh2::math::Vector3f* out);
dh2::math::Quaternion* dh2_quat_slerp(dh2::math::Quaternion* out,
    const dh2::math::Quaternion* first, const dh2::math::Quaternion* second,
    float t);
}
