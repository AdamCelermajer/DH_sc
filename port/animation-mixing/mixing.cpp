#include "mixing.hpp"
#include <cmath>
#include <cstring>

using namespace dh2::mixing;
namespace {
bool overlaps(const void *p, std::size_t n, const void *q, std::size_t m) {
    const auto a = reinterpret_cast<std::uintptr_t>(p), b = reinterpret_cast<std::uintptr_t>(q);
    return a <= b ? b - a < n : a - b < m;
}
bool finite(const dh2::math::Vector3f &v) {
    return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
}
bool finite(const dh2::math::Quaternion &q) {
    return std::isfinite(q.x) && std::isfinite(q.y) && std::isfinite(q.z) && std::isfinite(q.w);
}
template<class T> Error arrays(const T *values, const float *weights, std::uint32_t count, T *out) {
    if (!out || (count && (!values || !weights))) return Error::argument;
    if (count > 256) return Error::limit;
    if (count && (overlaps(values, count * sizeof(T), out, sizeof(T)) ||
                  overlaps(weights, count * sizeof(float), out, sizeof(T)))) return Error::argument;
    return Error::ok;
}
}

extern "C" Error dh2_animation_weights_normalize(float *weights, std::uint32_t count) {
    if (count > 256) return Error::limit;
    if (!weights && count) return Error::argument;
    if (!count) return Error::ok;
    float values[256];
    float sum = 0;
    for (std::uint32_t i = 0; i < count; ++i) {
        if (!std::isfinite(weights[i])) return Error::nonfinite;
        values[i] = weights[i];
        sum = sum + weights[i];
    }
    if (!std::isfinite(sum)) return Error::nonfinite;
    if (sum == 0.0f) values[0] = 1.0f;
    else for (std::uint32_t i = 0; i < count; ++i) {
        values[i] = values[i] / sum;
        if (!std::isfinite(values[i])) return Error::nonfinite;
    }
    std::memcpy(weights, values, count * sizeof(float));
    return Error::ok;
}

extern "C" Error dh2_animation_vector_mix(const dh2::math::Vector3f *values,
    const float *weights, std::uint32_t count, dh2::math::Vector3f *out) {
    const auto error = arrays(values, weights, count, out);
    if (error != Error::ok) return error;
    for (std::uint32_t i = 0; i < count; ++i)
        if (!finite(values[i]) || !std::isfinite(weights[i])) return Error::nonfinite;
    dh2::math::Vector3f result{};
    // Original position/scale blend and addition ignore the weight for one value.
    if (count == 1) result = values[0];
    else for (std::uint32_t i = 0; i < count; ++i) {
        result.x = result.x + weights[i] * values[i].x;
        result.y = result.y + weights[i] * values[i].y;
        result.z = result.z + weights[i] * values[i].z;
    }
    if (!finite(result)) return Error::nonfinite;
    *out = result;
    return Error::ok;
}

extern "C" Error dh2_animation_scalar_mix(const float *values, const float *weights,
    std::uint32_t count, float *out) {
    const auto error = arrays(values, weights, count, out);
    if (error != Error::ok) return error;
    float result = 0;
    for (std::uint32_t i = 0; i < count; ++i) {
        if (!std::isfinite(values[i]) || !std::isfinite(weights[i])) return Error::nonfinite;
        result = result + values[i] * weights[i];
    }
    if (!std::isfinite(result)) return Error::nonfinite;
    *out = result;
    return Error::ok;
}

extern "C" Error dh2_animation_quaternion_add(const dh2::math::Quaternion *values,
    const float *weights, std::uint32_t count, dh2::math::Quaternion *out) {
    const auto error = arrays(values, weights, count, out);
    if (error != Error::ok) return error;
    for (std::uint32_t i = 0; i < count; ++i)
        if (!finite(values[i]) || !std::isfinite(weights[i])) return Error::nonfinite;
    const dh2::math::Quaternion identity{0, 0, 0, 1};
    auto result = identity;
    for (std::uint32_t i = 0; i < count; ++i) {
        if (weights[i] == 0.0f) continue;
        auto value = values[i];
        const bool reverse = weights[i] < 0;
        if (reverse) { value.x = -value.x; value.y = -value.y; value.z = -value.z; }
        dh2::math::Quaternion partial{}, next{};
        dh2_quat_slerp(&partial, &identity, &value, reverse ? -weights[i] : weights[i]);
        dh2_quat_multiply(&next, &result, &partial);
        result = next;
    }
    if (!finite(result)) return Error::nonfinite;
    *out = result;
    return Error::ok;
}
