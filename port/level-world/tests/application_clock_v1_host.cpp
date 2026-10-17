#include "../application_clock_v1.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace clock = dh2::actor::application_clock_v1;

static float as_float(std::uint32_t bits) {
    float value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}
static std::uint32_t as_bits(float value) {
    std::uint32_t bits{};
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

int main() {
    std::uint32_t last{}, now{}, scale_a{}, scale_b{}, remainder{};
    while (std::scanf("%x %x %x %x %x", &last, &now, &scale_a, &scale_b, &remainder) == 5) {
        clock::State state{last, as_float(scale_a), as_float(scale_b), as_float(remainder)};
        clock::Result result{};
        const auto status = clock::compute(&state, now, &result);
        std::printf("%u %u %08x %u\n", static_cast<unsigned>(status),
                    result.dt_ms, as_bits(result.fractional_remainder), result.scaled_dt_ms);
    }
}
