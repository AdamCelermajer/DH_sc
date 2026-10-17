#include "../ending.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <limits>
using namespace dh2::ending;
static std::uint32_t seed = 20261002;
static std::uint32_t next() { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; }
int main() {
    for (int i = 0; i < 10000; ++i) {
        Sample sample{};
        const std::uint32_t bits[3] = {next(), next(), next()};
        std::memcpy(&sample, bits, sizeof(sample));
        std::int32_t extra = 123;
        const auto error = dh2_animation_extra_time(&extra, &sample);
        if (error != Error::ok) assert(extra == 123);
        Notice notice{123, 255};
        const auto notified = dh2_animation_end_notice(&notice, 1, 1, &sample);
        if (notified != Error::ok) assert(notice.extra_ms == 123 && notice.pending == 255);
        assert(notified == error);
        assert(dh2_animation_end_notice(&notice, 1, 2, nullptr) == Error::ok);
    }
    Sample sample{}; const auto before = sample;
    assert(dh2_animation_extra_time(&sample.current_ms, &sample) == Error::argument);
    assert(std::memcmp(&sample, &before, sizeof(sample)) == 0);
    Notice notice{456, 255};
    assert(dh2_animation_end_notice(&notice, 0, 0, nullptr) == Error::ok && notice.extra_ms == 456 && notice.pending == 1);
    assert(dh2_animation_end_notice(&notice, 1, 1, nullptr) == Error::argument);
    assert(dh2_animation_extra_time(&notice.extra_ms, nullptr) == Error::ok && notice.extra_ms == 456);
    assert(dh2_animation_end_notice(nullptr, 0, 0, nullptr) == Error::argument);
    std::puts("ending safety: 10000 arbitrary sample pairs, 20000 calculations/notifications; checked casts and unchanged errors");
}
