#include "../values.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <vector>

static std::uint32_t seed = 20261002;
static std::uint32_t next() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}
int main(int argc, char **argv) {
    assert(argc == 2);
    std::ifstream file(argv[1], std::ios::binary);
    const std::vector<std::uint8_t> original{(std::istreambuf_iterator<char>(file)), {}};
    assert(!original.empty());
    for (std::uint32_t i = 0; i < 3000; ++i) {
        auto bytes = original;
        if (i % 3 == 0)
            bytes.resize(next() % bytes.size());
        else if (i % 3 == 1)
            for (std::uint32_t j = 0, n = 1 + next() % 12; j < n; ++j)
                bytes[next() % bytes.size()] = static_cast<std::uint8_t>(next());
        const auto before = bytes;
        dh2::resources::BresView view{};
        if (dh2_bres_open(&view, bytes.data(), bytes.size()) != dh2::resources::BresError::ok)
            continue;
        const auto count = dh2_bres_library_count(&view, dh2::resources::Library::animation);
        assert(count <= bytes.size() / 32);
        for (std::uint32_t j = 0; j < count; ++j) {
            dh2::assets::Animation a{};
            if (dh2_animation_open(&a, &view, j, 0) != dh2::assets::Error::ok)
                continue;
            float out[4]{};
            dh2_animation_float_key(&a, 0, out);
            assert(dh2_animation_float_key(&a, UINT32_MAX, out) != dh2::animation::Error::ok);
            dh2_animation_float_interpolate(&a, 0, 1, 0.5f, out);
            assert(dh2_animation_float_interpolate(&a, UINT32_MAX, 0, 0.5f, out) !=
                   dh2::animation::Error::ok);
            assert(dh2_animation_float_interpolate(&a, 0, 1, std::numeric_limits<float>::infinity(),
                                                   out) != dh2::animation::Error::ok);
            dh2_animation_float_delta(&a, 0, 0, 1, 0.5f, true, out);
            assert(dh2_animation_float_delta(&a, UINT32_MAX, 0, 1, 0.5f, true, out) !=
                   dh2::animation::Error::ok);
            assert(dh2_animation_float_key(&a, 0, reinterpret_cast<float *>(bytes.data())) ==
                   dh2::animation::Error::argument);
        }
        assert(bytes == before);
    }
    auto fixture = original;
    dh2::resources::BresView view{};
    assert(dh2_bres_open(&view, fixture.data(), fixture.size()) == dh2::resources::BresError::ok);
    bool checked_defaults = false;
    const auto count = dh2_bres_library_count(&view, dh2::resources::Library::animation);
    for (std::uint32_t i = 0; i < count && !checked_defaults; ++i) {
        dh2::assets::Animation a{};
        assert(dh2_animation_open(&a, &view, i, 0) == dh2::assets::Error::ok);
        const auto type = dh2_animation_type(&a, 0);
        if (type != 9 && !(type >= 2 && type <= 4)) continue;
        const auto read_word = [&](std::size_t offset) {
            std::uint32_t value; std::memcpy(&value, fixture.data() + offset, 4); return value;
        };
        const auto write_word = [&](std::size_t offset, std::uint32_t value) {
            std::memcpy(fixture.data() + offset, &value, 4);
        };
        const auto defaults = read_word(a.record + 24);
        const auto pointer = read_word(defaults + 8);
        float value[4]{77, 77, 77, 77};
        write_word(defaults + 8, static_cast<std::uint32_t>(fixture.size() - 4));
        assert(dh2_animation_float_key(&a, 0, value) == dh2::animation::Error::range);
        assert(value[0] == 77 && value[3] == 77);
        write_word(defaults + 8, pointer);
        write_word(a.record + 24, 0);
        assert(dh2_animation_float_key(&a, 0, value) == dh2::animation::Error::unsupported);
        write_word(a.record + 24, defaults);
        const auto saved = read_word(pointer);
        write_word(pointer, 0x7fc00000);
        assert(dh2_animation_float_key(&a, 0, value) == dh2::animation::Error::nonfinite);
        assert(value[0] == 77 && value[3] == 77);
        write_word(pointer, saved);
        assert(fixture == original);
        checked_defaults = true;
    }
    using Q = dh2::math::Quaternion;
    Q values[3]{{0, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}}, out{7, 8, 9, 10};
    float weights[3]{1, 0, 0};
    assert(dh2_animation_quaternion_blend(nullptr, nullptr, 0, &out) == dh2::animation::Error::ok &&
           out.w == 1);
    assert(dh2_animation_quaternion_blend(values, weights, 257, &out) ==
           dh2::animation::Error::limit);
    assert(dh2_animation_quaternion_blend(values, weights, 3, values) ==
           dh2::animation::Error::argument);
    weights[0] = std::numeric_limits<float>::quiet_NaN();
    assert(dh2_animation_quaternion_blend(values, weights, 3, &out) ==
           dh2::animation::Error::nonfinite);
    weights[0] = 0.5f;
    weights[1] = -0.5f;
    out = {7, 8, 9, 10};
    assert(dh2_animation_quaternion_blend(values, weights, 3, &out) ==
           dh2::animation::Error::nonfinite);
    assert(out.x == 7 && out.y == 8 && out.z == 9 && out.w == 10);
    std::puts("animation values: 3000 corruption/truncation cases; range, alias and nonfinite "
              "checks passed");
    if (checked_defaults) std::puts("scalar defaults: truncated, absent and nonfinite checks passed");
}
