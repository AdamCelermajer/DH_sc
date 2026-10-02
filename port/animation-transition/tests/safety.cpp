#include "../transition.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <limits>
using namespace dh2::transition;
static std::uint32_t seed = 20261002;
static std::uint32_t next() { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; }
int main() {
    for (int i = 0; i < 5000; ++i) {
        State s{}; assert(dh2_transition_init(&s, 2) == Error::ok);
        for (int j = 0; j < 20; ++j) {
            if (i % 3 == 0) reinterpret_cast<unsigned char *>(&s)[next() % sizeof(s)] = next();
            const auto before = s;
            std::uint32_t active = 0xa5a5a5a5;
            const auto error = j % 4 == 0 ? dh2_transition_begin(&s, static_cast<std::int32_t>(next()))
                : dh2_transition_update(&s, next(), &active);
            if (error != Error::ok) {
                assert(std::memcmp(&s, &before, sizeof(s)) == 0);
                assert(active == 0xa5a5a5a5);
            }
        }
    }
    State s{}; assert(dh2_transition_init(&s, 2) == Error::ok);
    const auto before = s;
    assert(dh2_transition_update(&s, 1, &s.current) == Error::argument);
    assert(std::memcmp(&s, &before, sizeof(s)) == 0);
    assert(dh2_transition_init(&s, 9) == Error::limit);
    assert(dh2_transition_update(nullptr, 0, nullptr) == Error::argument);
    std::puts("transition safety: 5000 sequences, 100000 valid/corrupted operations; unchanged error outputs");
}
