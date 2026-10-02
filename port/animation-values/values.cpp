#include "values.hpp"
#include <cmath>
#include <cstring>

using namespace dh2::animation;
namespace {
bool finite4(const float *v) {
    for (std::uint32_t i = 0; i < 4; ++i)
        if (!std::isfinite(v[i]))
            return false;
    return true;
}
bool overlaps(const void *p, std::size_t n, const void *q, std::size_t m) {
    const auto a = reinterpret_cast<std::uintptr_t>(p), b = reinterpret_cast<std::uintptr_t>(q);
    return a <= b ? b - a < n : a - b < m;
}
Error vector(const dh2::assets::Animation *a, float *out, dh2::assets::Vector &v,
             std::uint32_t &type) {
    if (!a || !out || !a->image.bytes)
        return Error::argument;
    if (overlaps(a->image.bytes, a->image.size, out, 16))
        return Error::argument;
    if (dh2_animation_channels(a) != 1 || dh2_animation_samplers(a) != 1 ||
        dh2_animation_offsets(a) || dh2_animation_scales(a))
        return Error::unsupported;
    type = dh2_animation_type(a, 0);
    if (type != 1 && type != 2 && type != 3 && type != 4 && type != 5 && type != 9 && type != 10)
        return Error::unsupported;
    if (!dh2_animation_vector(a, 0, true, &v))
        return Error::range;
    if (v.type != 6 || v.components != (type == 5                                 ? 4U
                                        : (type == 9 || (type >= 2 && type <= 4)) ? 1U
                                                                                  : 3U))
        return Error::unsupported;
    if (type == 9 || (type >= 2 && type <= 4)) {
        const auto *axis = dh2_animation_default(a);
        if (!dh2_animation_has_default(a) || !axis)
            return Error::unsupported;
        const auto base = reinterpret_cast<std::uintptr_t>(a->image.bytes);
        const auto pos = reinterpret_cast<std::uintptr_t>(axis);
        if (pos < base || pos - base > a->image.size || a->image.size - (pos - base) < 12)
            return Error::range;
        float value[4]{};
        std::memcpy(value, axis, 12);
        if (!finite4(value))
            return Error::nonfinite;
    }
    return Error::ok;
}
Error read(const dh2::assets::Vector &v, std::uint32_t key, float *out) {
    float tmp[16]{};
    if (key >= v.count || !dh2_vector_read(&v, key, tmp))
        return Error::range;
    if (!finite4(tmp))
        return Error::nonfinite;
    std::memcpy(out, tmp, 16);
    return Error::ok;
}
Error commit(const float *value, float *out) {
    if (!finite4(value))
        return Error::nonfinite;
    std::memcpy(out, value, 16);
    return Error::ok;
}
dh2::math::Quaternion quat(const float *value) { return {value[0], value[1], value[2], value[3]}; }
dh2::math::Quaternion angle_quat(const dh2::assets::Animation *a, float radians) {
    dh2::math::Vector3f axis{};
    std::memcpy(&axis, dh2_animation_default(a), 12);
    dh2::math::Quaternion q{};
    dh2_quat_from_angle_axis(&q, radians, &axis);
    return q;
}
void write_quat(const dh2::math::Quaternion &q, float *out) {
    out[0] = q.x;
    out[1] = q.y;
    out[2] = q.z;
    out[3] = q.w;
}
void write_component(const dh2::assets::Animation *a, std::uint32_t type, float value, float *out) {
    std::memcpy(out, dh2_animation_default(a), 12);
    out[type - 2] = value;
    out[3] = 0;
}
} // namespace
extern "C" {
Error dh2_animation_quaternion_blend(const dh2::math::Quaternion *values, const float *weights,
                                     std::uint32_t count, dh2::math::Quaternion *out) {
    if (!out || (count && (!values || !weights)))
        return Error::argument;
    if (count > 256)
        return Error::limit;
    if (count && (overlaps(values, count * sizeof(*values), out, sizeof(*out)) ||
                  overlaps(weights, count * sizeof(*weights), out, sizeof(*out))))
        return Error::argument;
    for (std::uint32_t i = 0; i < count; ++i) {
        const float q[4]{values[i].x, values[i].y, values[i].z, values[i].w};
        if (!std::isfinite(weights[i]) || !finite4(q))
            return Error::nonfinite;
    }
    dh2::math::Quaternion result{0, 0, 0, 1};
    std::uint32_t i = 0;
    for (; i < count && weights[i] == 0.0f; ++i) {
    }
    float sum = 0;
    if (i < count) {
        sum = weights[i];
        result = values[i];
        if (sum == 1.0f) {
            *out = result;
            return Error::ok;
        }
        ++i;
    }
    for (; i < count; ++i) {
        if (weights[i] == 0.0f)
            continue;
        sum = sum + weights[i];
        const float t = weights[i] / sum;
        dh2::math::Quaternion next{};
        dh2_quat_slerp(&next, &result, &values[i], t);
        result = next;
    }
    const float q[4]{result.x, result.y, result.z, result.w};
    if (!finite4(q))
        return Error::nonfinite;
    *out = result;
    return Error::ok;
}
Error dh2_animation_float_key(const dh2::assets::Animation *a, std::uint32_t key, float *out) {
    dh2::assets::Vector v{};
    std::uint32_t type = 0;
    auto e = vector(a, out, v, type);
    if (e != Error::ok)
        return e;
    float result[4]{};
    e = read(v, key, result);
    if (e == Error::ok && type == 9)
        write_quat(angle_quat(a, result[0]), result);
    if (e == Error::ok && type >= 2 && type <= 4)
        write_component(a, type, result[0], result);
    return e == Error::ok ? commit(result, out) : e;
}
Error dh2_animation_float_interpolate(const dh2::assets::Animation *a, std::uint32_t first,
                                      std::uint32_t second, float t, float *out) {
    dh2::assets::Vector v{};
    std::uint32_t type = 0;
    auto e = vector(a, out, v, type);
    if (e != Error::ok)
        return e;
    if (!std::isfinite(t))
        return Error::nonfinite;
    // Original absolute routines consume consecutive keys. Their second-key
    // integer parameter is unused; this interface makes that contract explicit.
    if (first >= v.count || first == UINT32_MAX ||
        (type != 9 && !(type >= 2 && type <= 4) && second != first + 1) || second >= v.count)
        return Error::range;
    float x[4]{}, y[4]{}, result[4]{};
    if ((e = read(v, first, x)) != Error::ok || (e = read(v, second, y)) != Error::ok)
        return e;
    const float weights[2]{1.0f - t, t};
    if (type == 9) {
        const float radians = x[0] + t * (y[0] - x[0]);
        write_quat(angle_quat(a, radians), result);
    } else if (type >= 2 && type <= 4) {
        write_component(a, type, x[0] + t * (y[0] - x[0]), result);
    } else if (type == 5) {
        const dh2::math::Quaternion qs[2]{quat(x), quat(y)};
        dh2::math::Quaternion q{};
        e = dh2_animation_quaternion_blend(qs, weights, 2, &q);
        if (e != Error::ok)
            return e;
        result[0] = q.x;
        result[1] = q.y;
        result[2] = q.z;
        result[3] = q.w;
    } else
        for (std::uint32_t i = 0; i < 3; ++i)
            result[i] = weights[0] * x[i] + weights[1] * y[i];
    return commit(result, out);
}
Error dh2_animation_float_delta(const dh2::assets::Animation *a, std::uint32_t reference,
                                std::uint32_t first, std::uint32_t second, float t,
                                bool interpolate, float *out) {
    dh2::assets::Vector v{};
    std::uint32_t type = 0;
    auto e = vector(a, out, v, type);
    if (e != Error::ok)
        return e;
    if (!std::isfinite(t))
        return Error::nonfinite;
    float base[4]{}, x[4]{}, y[4]{}, result[4]{};
    if ((e = read(v, reference, base)) != Error::ok || (e = read(v, first, x)) != Error::ok)
        return e;
    if (interpolate && (e = read(v, second, y)) != Error::ok)
        return e;
    if (type == 5 || type == 9) {
        dh2::math::Quaternion sampled = type == 9 ? angle_quat(a, x[0]) : quat(x);
        if (interpolate) {
            const auto q1 = sampled, q2 = type == 9 ? angle_quat(a, y[0]) : quat(y);
            dh2_quat_slerp(&sampled, &q1, &q2, t);
        }
        const auto reference_q = type == 9 ? angle_quat(a, base[0]) : quat(base);
        const dh2::math::Quaternion conjugate{-reference_q.x, -reference_q.y, -reference_q.z,
                                              reference_q.w};
        dh2::math::Quaternion q{};
        dh2_quat_multiply(&q, &conjugate, &sampled);
        result[0] = q.x;
        result[1] = q.y;
        result[2] = q.z;
        result[3] = q.w;
    } else if (type >= 2 && type <= 4) {
        const float first_delta = x[0] - base[0];
        const float sampled =
            interpolate ? first_delta + t * ((y[0] - base[0]) - first_delta) : first_delta;
        write_component(a, type, sampled, result);
    } else
        for (std::uint32_t i = 0; i < 3; ++i) {
            const float sampled = interpolate ? x[i] + t * (y[i] - x[i]) : x[i];
            result[i] = sampled - base[i];
        }
    return commit(result, out);
}
}
