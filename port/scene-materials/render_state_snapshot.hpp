#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

// A checked reader for the original 32-byte glitch::video::detail::renderpass
// SRenderState snapshot. This is not a C++ ABI view and it does not bind a
// snapshot to a serialized BRES technique selector.
namespace dh2::scene_materials::render_state {

enum class Error : std::uint8_t {
    ok,
    argument,
    size,
    blend_factor,
    blend_equation,
    depth_function,
};

struct Value {
    std::uint8_t ordinal = 0;
    std::uint32_t gl_enum = 0;
};

struct PassState {
    // Eight words copied by CMaterialRenderer::setRenderState. Keep all raw
    // words so fields not yet modeled remain inspectable.
    std::array<std::uint32_t, 8> words{};

    bool blend_enabled = false;
    bool depth_test_enabled = false;
    bool depth_write_enabled = false;

    Value blend_source{};
    Value blend_destination{};
    Value blend_equation{};
    Value depth_function{};
    std::uint8_t cull_face_ordinal = 0;
};

// The input must be exactly the 32 bytes consumed by CMaterialRenderer's
// memcmp/copy path. Multi-byte values are decoded as little-endian words; no
// source bytes are reinterpreted as native structs or pointers.
Error decode_pass_state(const std::uint8_t* bytes, std::size_t size,
                        PassState& output) noexcept;

const char* error_name(Error error) noexcept;

} // namespace dh2::scene_materials::render_state
