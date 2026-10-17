#include "../render_state_snapshot.hpp"

#include <array>
#include <cstdint>
#include <cstdio>

namespace rs = dh2::scene_materials::render_state;

namespace {
void put_le32(std::array<std::uint8_t, 32>& bytes, std::size_t index,
              std::uint32_t value) {
    const auto offset = index * 4;
    bytes[offset + 0] = static_cast<std::uint8_t>(value);
    bytes[offset + 1] = static_cast<std::uint8_t>(value >> 8);
    bytes[offset + 2] = static_cast<std::uint8_t>(value >> 16);
    bytes[offset + 3] = static_cast<std::uint8_t>(value >> 24);
}

int fail(const char* message) {
    std::fprintf(stderr, "FAIL: %s\n", message);
    return 1;
}
} // namespace

int main() {
    // Exact 32-byte SRenderState() constructor result recovered from ARM at
    // 0x00588ddc: words 0 and 1 are packed flags; remaining words are floats.
    std::array<std::uint8_t, 32> defaults{
        0x01, 0x00, 0xff, 0x18,
        0x07, 0x00, 0x18, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x80, 0x3f,
        0x00, 0x00, 0x80, 0x3f,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x80, 0x3f,
        0x00, 0x00, 0x80, 0x3f,
    };
    rs::PassState state{};
    if (rs::decode_pass_state(defaults.data(), defaults.size(), state) != rs::Error::ok)
        return fail("constructor snapshot rejected");
    if (state.words[0] != 0x18ff0001u || state.words[1] != 0x00180007u)
        return fail("constructor words differ from ARM stores");
    if (state.blend_enabled || !state.depth_test_enabled || !state.depth_write_enabled)
        return fail("constructor enable defaults differ from packed state");
    if (state.blend_source.ordinal != 1 || state.blend_source.gl_enum != 0x0001 ||
        state.blend_destination.ordinal != 0 || state.blend_destination.gl_enum != 0x0000 ||
        state.blend_equation.ordinal != 0 || state.blend_equation.gl_enum != 0x8006 ||
        state.depth_function.ordinal != 3 || state.depth_function.gl_enum != 0x0203)
        return fail("constructor GL defaults do not match recovered lookup tables");

    // Synthetic source-field fixture. This encodes ONE/ONE, ADD, LEQUAL,
    // blending and depth test enabled, depth writes disabled. It is a decoder
    // test vector, not a recovered Material__11611 pass snapshot.
    std::array<std::uint8_t, 32> additive{};
    put_le32(additive, 0, 0x18000011u);
    put_le32(additive, 1, 0x00090007u);
    if (rs::decode_pass_state(additive.data(), additive.size(), state) != rs::Error::ok)
        return fail("synthetic ONE/ONE fixture rejected");
    if (!state.blend_enabled || !state.depth_test_enabled || state.depth_write_enabled)
        return fail("synthetic blend/depth enable bits decoded incorrectly");
    if (state.blend_source.gl_enum != 0x0001 ||
        state.blend_destination.gl_enum != 0x0001 ||
        state.blend_equation.gl_enum != 0x8006 ||
        state.depth_function.gl_enum != 0x0203)
        return fail("synthetic ONE/ONE ADD LEQUAL fields decoded incorrectly");

    // Enumerations that use noncontiguous GLES values are checked at both ends.
    put_le32(additive, 0, 0x1800004eu); // SRC_ALPHA_SATURATE / DST_COLOR / ADD / LEQUAL
    if (rs::decode_pass_state(additive.data(), additive.size(), state) != rs::Error::ok ||
        state.blend_source.gl_enum != 0x0308 ||
        state.blend_destination.gl_enum != 0x0302)
        return fail("noncontiguous blend factor mapping is incorrect");

    constexpr std::uint32_t factor_map[] = {
        0x0000, 0x0001, 0x0300, 0x0301, 0x0302,
        0x0303, 0x0306, 0x0307, 0x0304, 0x0305,
        0x8001, 0x8002, 0x8003, 0x8004, 0x0308,
    };
    for (std::uint32_t i = 0; i < sizeof(factor_map) / sizeof(factor_map[0]); ++i) {
        put_le32(additive, 0, (3u << 27) | i);
        if (rs::decode_pass_state(additive.data(), additive.size(), state) != rs::Error::ok ||
            state.blend_source.ordinal != i || state.blend_source.gl_enum != factor_map[i])
            return fail("blend source factor lookup table differs from recovered ELF");
        put_le32(additive, 0, (3u << 27) | (i << 4) | 1u);
        if (rs::decode_pass_state(additive.data(), additive.size(), state) != rs::Error::ok ||
            state.blend_destination.ordinal != i ||
            state.blend_destination.gl_enum != factor_map[i])
            return fail("blend destination factor lookup table differs from recovered ELF");
    }
    constexpr std::uint32_t equation_map[] = {0x8006, 0x800a, 0x800b, 0x8007, 0x8008};
    for (std::uint32_t i = 0; i < sizeof(equation_map) / sizeof(equation_map[0]); ++i) {
        put_le32(additive, 0, (3u << 27) | (i << 24));
        if (rs::decode_pass_state(additive.data(), additive.size(), state) != rs::Error::ok ||
            state.blend_equation.ordinal != i || state.blend_equation.gl_enum != equation_map[i])
            return fail("blend equation lookup table differs from recovered ELF");
    }
    constexpr std::uint32_t compare_map[] = {
        0x0200, 0x0201, 0x0202, 0x0203, 0x0204, 0x0205, 0x0206, 0x0207,
    };
    for (std::uint32_t i = 0; i < sizeof(compare_map) / sizeof(compare_map[0]); ++i) {
        put_le32(additive, 0, i << 27);
        if (rs::decode_pass_state(additive.data(), additive.size(), state) != rs::Error::ok ||
            state.depth_function.ordinal != i || state.depth_function.gl_enum != compare_map[i])
            return fail("depth comparison lookup table differs from recovered ELF");
    }

    if (rs::decode_pass_state(nullptr, 32, state) != rs::Error::argument)
        return fail("null input did not fail closed");
    if (rs::decode_pass_state(defaults.data(), 31, state) != rs::Error::size ||
        rs::decode_pass_state(defaults.data(), 33, state) != rs::Error::size ||
        rs::decode_pass_state(defaults.data(), 0, state) != rs::Error::size)
        return fail("non-32-byte snapshot did not fail closed");

    auto invalid = defaults;
    put_le32(invalid, 0, (0x18ff0001u & ~0x0fu) | 0x0fu);
    if (rs::decode_pass_state(invalid.data(), invalid.size(), state) != rs::Error::blend_factor)
        return fail("out-of-range blend factor ordinal was accepted");
    invalid = defaults;
    put_le32(invalid, 0, (0x18ff0001u & ~(0x07u << 24)) | (5u << 24));
    if (rs::decode_pass_state(invalid.data(), invalid.size(), state) != rs::Error::blend_equation)
        return fail("out-of-range blend equation ordinal was accepted");

    std::printf("{\"validation\":\"PASS\",\"snapshot_bytes\":32,"
                "\"constructor_blend\":false,\"constructor_depth_test\":true,"
                "\"constructor_depth_write\":true,\"synthetic_additive_one_one\":true,"
                "\"al_runtime_state_recovered\":false}\n");
    return 0;
}
