#include "../mixing.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
using namespace dh2::mixing;
static std::uint32_t seed = 20261002;
static std::uint32_t next() { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; }
static float value() { return (static_cast<int>(next() % 20001) - 10000) / 10000.0f; }
template<class T> struct Guard { std::uint32_t first; T value; std::uint32_t last; };
template<class T> void check(const Guard<T> &out, const Guard<T> &before, Error e) {
    assert(out.first == before.first && out.last == before.last);
    if (e != Error::ok) assert(std::memcmp(&out, &before, sizeof(out)) == 0);
}
int main() {
    for (int test = 0; test < 6000; ++test) {
        float weights[256], scalars[256], saved[256];
        dh2::math::Vector3f vectors[256];
        dh2::math::Quaternion quats[256];
        const std::uint32_t count = test % 13 == 0 ? 257 : next() % 257;
        for (int i = 0; i < 256; ++i) {
            weights[i] = value(); scalars[i] = value();
            vectors[i] = {value(), value(), value()};
            quats[i] = {value(), value(), value(), value()};
            const auto q = quats[i];
            const float length = std::sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
            if (length == 0) quats[i] = {0, 0, 0, 1};
            else quats[i] = {q.x/length, q.y/length, q.z/length, q.w/length};
        }
        if (test % 7 == 0 && count && count <= 256)
            weights[next() % count] = std::numeric_limits<float>::quiet_NaN();
        std::memcpy(saved, weights, sizeof(saved));
        const auto normalized = dh2_animation_weights_normalize(weights, count);
        if (normalized != Error::ok) assert(std::memcmp(saved, weights, sizeof(saved)) == 0);
        // Keep the mixing inputs independent of the normalization test.
        std::memcpy(weights, saved, sizeof(saved));
        Guard<float> s{0xa5a5a5a5, 7, 0x5a5a5a5a}; const auto before_s = s;
        Guard<dh2::math::Vector3f> v{0xa5a5a5a5, {7,8,9}, 0x5a5a5a5a}; const auto before_v = v;
        Guard<dh2::math::Quaternion> q{0xa5a5a5a5, {7,8,9,10}, 0x5a5a5a5a}; const auto before_q = q;
        check(s,before_s,dh2_animation_scalar_mix(scalars,weights,count,&s.value));
        check(v,before_v,dh2_animation_vector_mix(vectors,weights,count,&v.value));
        check(q,before_q,dh2_animation_quaternion_add(quats,weights,count,&q.value));
        assert(std::memcmp(weights,saved,sizeof(saved)) == 0);
    }
    float weights[]{1,2}, scalar = 7;
    assert(dh2_animation_weights_normalize(nullptr,0) == Error::ok);
    assert(dh2_animation_weights_normalize(nullptr,1) == Error::argument);
    assert(dh2_animation_scalar_mix(weights,weights,2,weights) == Error::argument);
    assert(weights[0] == 1 && weights[1] == 2);
    const float huge[]{std::numeric_limits<float>::max(),std::numeric_limits<float>::max()};
    assert(dh2_animation_scalar_mix(huge,weights,2,&scalar) == Error::nonfinite && scalar == 7);
    std::puts("mixing safety: 6000 bounded lists, 24000 API calls; guarded/unchanged outputs and weights");
}
