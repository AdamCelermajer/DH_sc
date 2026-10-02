#include "../motion.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <limits>
using namespace dh2::motion;
static std::uint32_t seed = 20261002;
static std::uint32_t next() { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; }
int main() {
    for (int i = 0; i < 5000; ++i) {
        State s{};
        for (int j = 0; j < 20; ++j) {
            float floats[3];
            for (auto &v : floats) { const auto bits = next(); std::memcpy(&v, &bits, 4); }
            const dh2::math::Vector3f position{floats[0], floats[1], floats[2]};
            if (i % 3 == 0) reinterpret_cast<unsigned char *>(&s)[next() % sizeof(s)] = next();
            const auto before = s;
            const auto error = j % 4 == 0 ? dh2_motion_reset(&s, next(), &position)
                : dh2_motion_calculate(&s, next(), &position);
            if (error != Error::ok) assert(std::memcmp(&s, &before, sizeof(s)) == 0);
        }
    }
    State s{}; const auto before = s;
    assert(dh2_motion_calculate(&s, 1, &s.previous) == Error::argument);
    assert(std::memcmp(&s, &before, sizeof(s)) == 0);
    const dh2::math::Vector3f maximum{std::numeric_limits<float>::max(), 0, 0};
    const dh2::math::Vector3f minimum{-std::numeric_limits<float>::max(), 0, 0};
    assert(dh2_motion_reset(&s, 0, &minimum) == Error::ok);
    const auto stored = s;
    assert(dh2_motion_calculate(&s, 1, &maximum) == Error::nonfinite);
    assert(std::memcmp(&s, &stored, sizeof(s)) == 0);
    assert(dh2_motion_calculate(nullptr, 0, nullptr) == Error::argument);
    std::puts("motion safety: 5000 sequences, 100000 valid/corrupted operations; unchanged error state");
}
