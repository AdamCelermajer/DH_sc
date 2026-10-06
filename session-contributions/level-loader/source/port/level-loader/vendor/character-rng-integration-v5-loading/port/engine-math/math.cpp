#include "math.hpp"

#include <cmath>
#include <cstring>

using dh2::math::Matrix4f;
using dh2::math::Quaternion;
using dh2::math::Vector3f;

namespace {
// Keep each original __aeabi operation's rounding boundary. Build with
// contraction disabled and without fast-math; do not algebraically simplify.
float add(float a, float b) { return a + b; }
float sub(float a, float b) { return a - b; }
float mul(float a, float b) { return a * b; }
float divf(float a, float b) { return a / b; }
float literal(std::uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}
constexpr double radians_per_degree = 0x1.1df46a2529d39p-6;
constexpr double degrees_per_radian = 0x1.ca5dc1a63c1f8p+5;
constexpr double euler_cos_threshold = 0x1.5798ee2308c3ap-27;

float wrap_degrees(float degrees) {
    if (degrees < 0.0f) degrees = add(degrees, 360.0f);
    if (degrees >= 360.0f) degrees = sub(degrees, 360.0f);
    return degrees;
}

void rotate_plane(float& a, float& b, const float& ca, const float& cb,
                  double degrees, bool restore_second_first) {
    const double angle = degrees * radians_per_degree;
    const float c = static_cast<float>(::cos(angle));
    const float s = static_cast<float>(::sin(angle));
    a = sub(a, ca);
    b = sub(b, cb);
    const float old_a = a, old_b = b;
    const float new_a = sub(mul(c, old_a), mul(s, old_b));
    const float new_b = add(mul(s, old_a), mul(c, old_b));
    a = new_a;
    b = new_b;
    if (restore_second_first) {
        b = add(new_b, cb);
        a = add(new_a, ca);
    } else {
        a = add(new_a, ca);
        b = add(new_b, cb);
    }
}
}

extern "C" Vector3f* dh2_vec3_normalize(Vector3f* v) {
    const float length2 = add(add(mul(v->x, v->x), mul(v->y, v->y)),
                              mul(v->z, v->z));
    if (length2 == 0.0f) return v;
    const float scale = divf(1.0f, ::sqrtf(length2));
    v->x = mul(v->x, scale);
    v->y = mul(v->y, scale);
    v->z = mul(v->z, scale);
    return v;
}

extern "C" Vector3f* dh2_vec3_divide(Vector3f* out, const Vector3f* a,
                                    const Vector3f* b) {
    const float y = divf(a->y, b->y), z = divf(a->z, b->z);
    const float x = divf(a->x, b->x);
    out->y = y; out->x = x; out->z = z;
    return out;
}
extern "C" Vector3f* dh2_vec3_divide_assign(Vector3f* a, const Vector3f* b) {
    a->x = divf(a->x, b->x);
    a->y = divf(a->y, b->y);
    a->z = divf(a->z, b->z);
    return a;
}
extern "C" void dh2_vec3_rotate_xy(Vector3f* v, double d, const Vector3f* c) {
    rotate_plane(v->x, v->y, c->x, c->y, d, false);
}
extern "C" void dh2_vec3_rotate_yz(Vector3f* v, double d, const Vector3f* c) {
    rotate_plane(v->y, v->z, c->y, c->z, d, true);
}
extern "C" void dh2_vec3_rotate_xz(Vector3f* v, double d, const Vector3f* c) {
    rotate_plane(v->x, v->z, c->x, c->z, d, false);
}
extern "C" Vector3f* dh2_vec3_horizontal_angles(Vector3f* out,
                                                const Vector3f* v) {
    out->x = out->y = out->z = 0.0f;
    out->y = wrap_degrees(static_cast<float>(
        static_cast<double>(::atan2f(v->x, v->z)) * degrees_per_radian));
    const float horizontal = ::sqrtf(add(mul(v->x, v->x), mul(v->z, v->z)));
    const double pitch = ::atan2(static_cast<double>(horizontal),
                                static_cast<double>(v->y));
    out->x = wrap_degrees(static_cast<float>(pitch * degrees_per_radian - 90.0));
    return out;
}

extern "C" Quaternion* dh2_quat_normalize(Quaternion* q) {
    const float length2 = add(add(add(mul(q->x, q->x), mul(q->y, q->y)),
                                  mul(q->z, q->z)), mul(q->w, q->w));
    if (length2 == 1.0f) return q;
    // Unlike vector normalization, the original quaternion has no zero guard.
    const float scale = divf(1.0f, ::sqrtf(length2));
    q->x = mul(q->x, scale); q->y = mul(q->y, scale);
    q->z = mul(q->z, scale); q->w = mul(q->w, scale);
    return q;
}
extern "C" Quaternion* dh2_quat_from_euler(Quaternion* q, float x, float y,
                                          float z) {
    const double ax = static_cast<double>(x) * 0.5;
    const double ay = static_cast<double>(y) * 0.5;
    const double az = static_cast<double>(z) * 0.5;
    const double sx = ::sin(ax), cx = ::cos(ax);
    const double sy = ::sin(ay), cy = ::cos(ay);
    const double sz = ::sin(az), cz = ::cos(az);
    const double cycz = cy * cz, sycz = sy * cz;
    const double cysz = cy * sz, sysz = sy * sz;
    q->x = static_cast<float>(sx * cycz - cx * sysz);
    q->y = static_cast<float>(cx * sycz + sx * cysz);
    q->z = static_cast<float>(cx * cysz - sx * sycz);
    q->w = static_cast<float>(cx * cycz + sx * sysz);
    return dh2_quat_normalize(q);
}
extern "C" Quaternion* dh2_quat_from_angle_axis(Quaternion* q, float radians,
                                                const Vector3f* axis) {
    const float angle = mul(radians, 0.5f);
    const float s = ::sinf(angle);
    q->w = ::cosf(angle);
    q->x = mul(axis->x, s); q->y = mul(axis->y, s); q->z = mul(axis->z, s);
    return q;
}
extern "C" Quaternion* dh2_quat_from_fixed_angle_axis(Quaternion* q,
                                                      const Vector3f* axis) {
    q->w = literal(0x3f067b80);
    const float s = literal(0x3f59d4d0);
    q->x = mul(axis->x, s); q->y = mul(axis->y, s); q->z = mul(axis->z, s);
    return q;
}
extern "C" Quaternion* dh2_quat_multiply(Quaternion* out, const Quaternion* a,
                                        const Quaternion* b) {
    *out = {0.0f, 0.0f, 0.0f, 1.0f};
    // Original operand order is b Hamilton-multiplied by a.
    out->w = sub(sub(sub(mul(a->w, b->w), mul(a->x, b->x)), mul(a->y, b->y)),
                 mul(a->z, b->z));
    out->x = sub(add(add(mul(a->x, b->w), mul(a->w, b->x)), mul(a->z, b->y)),
                 mul(a->y, b->z));
    out->y = sub(add(add(mul(a->y, b->w), mul(a->w, b->y)), mul(a->x, b->z)),
                 mul(a->z, b->x));
    out->z = sub(add(add(mul(a->z, b->w), mul(a->w, b->z)), mul(a->y, b->x)),
                 mul(a->x, b->y));
    return out;
}
extern "C" Vector3f* dh2_quat_transform_vector(Vector3f* out, const Quaternion* q,
                                               const Vector3f* v) {
    const float ux = add(mul(-q->y, v->z), mul(q->z, v->y));
    const float uy = add(mul(-q->z, v->x), mul(q->x, v->z));
    const float uz = add(mul(v->y, -q->x), mul(q->y, v->x));
    const float twice_w = add(q->w, q->w);
    const float xx = add(mul(-q->y, uz), mul(q->z, uy));
    const float yy = add(mul(ux, -q->z), mul(q->x, uz));
    const float zz = add(mul(uy, -q->x), mul(q->y, ux));
    out->x = add(add(xx, xx), add(v->x, mul(twice_w, ux)));
    out->y = add(add(yy, yy), add(v->y, mul(twice_w, uy)));
    out->z = add(add(zz, zz), add(v->z, mul(twice_w, uz)));
    return out;
}

extern "C" void dh2_quat_matrix(const Quaternion* q, Matrix4f* out) {
    const float tx = add(q->x, q->x), ty = add(q->y, q->y), tz = add(q->z, q->z);
    const float xx = mul(q->x, tx), zz = mul(q->z, tz);
    const float xy = mul(tx, q->y), xz = mul(tx, q->z), xw = mul(tx, q->w);
    const float yz = mul(ty, q->z), yw = mul(ty, q->w), zw = mul(tz, q->w);
    const float one_minus_yy = sub(1.0f, mul(q->y, ty));
    out->identity_hint = 0;
    out->m[0] = sub(one_minus_yy, zz);
    out->m[1] = add(xy, zw); out->m[2] = sub(xz, yw); out->m[3] = 0.0f;
    out->m[4] = sub(xy, zw); out->m[5] = sub(sub(1.0f, xx), zz);
    out->m[6] = add(yz, xw); out->m[7] = 0.0f;
    out->m[8] = add(xz, yw); out->m[9] = sub(yz, xw);
    out->m[10] = sub(one_minus_yy, xx); out->m[11] = 0.0f;
    out->m[12] = out->m[13] = out->m[14] = 0.0f; out->m[15] = 1.0f;
}
extern "C" void dh2_quat_matrix_transposed(const Quaternion* q, Matrix4f* out) {
    Matrix4f normal;
    dh2_quat_matrix(q, &normal);
    for (int row = 0; row != 4; ++row)
        for (int column = 0; column != 4; ++column)
            out->m[4 * row + column] = normal.m[4 * column + row];
    out->identity_hint = 0;
}
extern "C" Matrix4f* dh2_quat_matrix_value(Matrix4f* out, const Quaternion* q) {
    dh2_quat_matrix_transposed(q, out);
    return out;
}
extern "C" Quaternion* dh2_quat_from_matrix(Quaternion* q, const Matrix4f* m) {
    const float a = m->m[0], b = m->m[5], c = m->m[10];
    const float trace = add(add(a, b), c);
    float root, scale;
    if (trace > 0.0f) {
        root = ::sqrtf(add(trace, 1.0f));
        q->w = mul(root, 0.5f); scale = divf(0.5f, root);
        q->x = mul(sub(m->m[9], m->m[6]), scale);
        q->y = mul(sub(m->m[2], m->m[8]), scale);
        q->z = mul(sub(m->m[4], m->m[1]), scale);
    } else if (a > b && a > c) {
        root = ::sqrtf(sub(sub(add(a, 1.0f), b), c));
        q->x = mul(root, 0.5f); scale = divf(0.5f, root);
        q->y = mul(add(m->m[1], m->m[4]), scale);
        q->z = mul(add(m->m[8], m->m[2]), scale);
        q->w = mul(sub(m->m[9], m->m[6]), scale);
    } else if (b > c) {
        root = ::sqrtf(sub(sub(add(b, 1.0f), a), c));
        q->y = mul(root, 0.5f); scale = divf(0.5f, root);
        q->x = mul(add(m->m[1], m->m[4]), scale);
        q->z = mul(add(m->m[6], m->m[9]), scale);
        q->w = mul(sub(m->m[2], m->m[8]), scale);
    } else {
        root = ::sqrtf(sub(sub(add(c, 1.0f), a), b));
        q->z = mul(root, 0.5f); scale = divf(0.5f, root);
        q->x = mul(add(m->m[2], m->m[8]), scale);
        q->y = mul(add(m->m[6], m->m[9]), scale);
        q->w = mul(sub(m->m[4], m->m[1]), scale);
    }
    return dh2_quat_normalize(q);
}

extern "C" Vector3f* dh2_matrix_rotation_degrees(Vector3f* out, const Matrix4f* m) {
    const float yf = -::asinf(m->m[2]);
    const double y_radians = static_cast<double>(yf);
    const double c = ::cos(y_radians);
    double y = y_radians * degrees_per_radian, x, z;
    if (::fabs(c) > euler_cos_threshold) {
        const double inverse = 1.0 / c;
        x = ::atan2(static_cast<double>(m->m[6]) * inverse,
                    static_cast<double>(m->m[10]) * inverse) * degrees_per_radian;
        z = ::atan2(static_cast<double>(m->m[1]) * inverse,
                    static_cast<double>(m->m[0]) * inverse) * degrees_per_radian;
    } else {
        x = 0.0;
        z = ::atan2(static_cast<double>(-m->m[4]),
                    static_cast<double>(m->m[5])) * degrees_per_radian;
    }
    if (x < 0.0) x += 360.0;
    if (y < 0.0) y += 360.0;
    if (z < 0.0) z += 360.0;
    out->x = static_cast<float>(x); out->y = static_cast<float>(y);
    out->z = static_cast<float>(z);
    return out;
}
extern "C" void dh2_quat_to_euler_degrees(const Quaternion* q, Vector3f* out) {
    Matrix4f matrix;
    dh2_quat_matrix(q, &matrix);
    dh2_matrix_rotation_degrees(out, &matrix);
}
extern "C" Quaternion* dh2_quat_slerp(Quaternion* out, const Quaternion* a,
                                      const Quaternion* b, float t) {
    // Original arguments are by value. Take copies before writing out.
    Quaternion first = *a, second = *b;
    float dot = add(add(add(mul(first.x, second.x), mul(first.y, second.y)),
                        mul(first.z, second.z)), mul(first.w, second.w));
    if (dot < 0.0f) {
        dot = -dot;
        first.x = -first.x; first.y = -first.y;
        first.z = -first.z; first.w = -first.w;
    }
    const float epsilon = literal(0x3d4ccccd);
    float first_scale, second_scale;
    if (add(dot, 1.0f) > epsilon) {
        if (sub(1.0f, dot) >= epsilon) {
            const float angle = ::acosf(dot);
            const float inverse_sine = divf(1.0f, ::sinf(angle));
            first_scale = mul(::sinf(mul(angle, sub(1.0f, t))), inverse_sine);
            second_scale = mul(::sinf(mul(angle, t)), inverse_sine);
        } else {
            first_scale = sub(1.0f, t); second_scale = t;
            out->x = add(mul(first_scale, first.x), mul(second_scale, second.x));
            out->y = add(mul(first_scale, first.y), mul(second_scale, second.y));
            out->z = add(mul(first_scale, first.z), mul(second_scale, second.z));
            out->w = add(mul(first_scale, first.w), mul(second_scale, second.w));
            return dh2_quat_normalize(out);
        }
    } else {
        // Retained legacy orthogonal branch (normally unreachable for finite
        // dot products after the sign adjustment, reachable for NaNs).
        const float pi = literal(0x40490fdb);
        second = {-first.y, first.x, -first.w, first.z};
        first_scale = ::sinf(mul(sub(0.5f, t), pi));
        second_scale = ::sinf(mul(t, pi));
    }
    out->x = add(mul(first_scale, first.x), mul(second_scale, second.x));
    out->y = add(mul(first_scale, first.y), mul(second_scale, second.y));
    out->z = add(mul(first_scale, first.z), mul(second_scale, second.z));
    out->w = add(mul(first_scale, first.w), mul(second_scale, second.w));
    return out;
}
