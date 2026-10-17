#include "render_state_snapshot.hpp"

namespace dh2::scene_materials::render_state {
namespace {
constexpr std::size_t kSnapshotBytes = 8 * sizeof(std::uint32_t);

// Exact lookup data from the pinned ARM ELF's .rodata symbols:
// BlendFactorMap 0x008e003c (15 entries), BlendEquationMap 0x008e0078
// (5 entries), and CompareFuncMap 0x008e0098 (8 entries).
constexpr std::uint32_t kBlendFactors[] = {
    0x0000, // GL_ZERO
    0x0001, // GL_ONE
    0x0300, // GL_SRC_COLOR
    0x0301, // GL_ONE_MINUS_SRC_COLOR
    0x0302, // GL_SRC_ALPHA
    0x0303, // GL_ONE_MINUS_SRC_ALPHA
    0x0306, // GL_DST_COLOR
    0x0307, // GL_ONE_MINUS_DST_COLOR
    0x0304, // GL_DST_ALPHA
    0x0305, // GL_ONE_MINUS_DST_ALPHA
    0x8001, // GL_CONSTANT_COLOR
    0x8002, // GL_ONE_MINUS_CONSTANT_COLOR
    0x8003, // GL_CONSTANT_ALPHA
    0x8004, // GL_ONE_MINUS_CONSTANT_ALPHA
    0x0308, // GL_SRC_ALPHA_SATURATE
};

constexpr std::uint32_t kBlendEquations[] = {
    0x8006, // GL_FUNC_ADD
    0x800a, // GL_FUNC_SUBTRACT
    0x800b, // GL_FUNC_REVERSE_SUBTRACT
    0x8007, // GL_MIN
    0x8008, // GL_MAX
};

constexpr std::uint32_t kCompareFunctions[] = {
    0x0200, // GL_NEVER
    0x0201, // GL_LESS
    0x0202, // GL_EQUAL
    0x0203, // GL_LEQUAL
    0x0204, // GL_GREATER
    0x0205, // GL_NOTEQUAL
    0x0206, // GL_GEQUAL
    0x0207, // GL_ALWAYS
};

std::uint32_t read_le32(const std::uint8_t* bytes) noexcept {
    return std::uint32_t(bytes[0]) |
           (std::uint32_t(bytes[1]) << 8) |
           (std::uint32_t(bytes[2]) << 16) |
           (std::uint32_t(bytes[3]) << 24);
}

Value map_value(std::uint8_t ordinal, std::uint32_t gl_enum) noexcept {
    return Value{ordinal, gl_enum};
}
} // namespace

Error decode_pass_state(const std::uint8_t* bytes, std::size_t size,
                        PassState& output) noexcept {
    output = {};
    if (!bytes) return Error::argument;
    if (size != kSnapshotBytes) return Error::size;

    PassState decoded{};
    for (std::size_t i = 0; i < decoded.words.size(); ++i)
        decoded.words[i] = read_le32(bytes + i * sizeof(std::uint32_t));

    const auto packed = decoded.words[0];
    const auto flags = decoded.words[1];
    const auto src = static_cast<std::uint8_t>(packed & 0x0fu);
    const auto dst = static_cast<std::uint8_t>((packed >> 4) & 0x0fu);
    const auto equation = static_cast<std::uint8_t>((packed >> 24) & 0x07u);
    const auto compare = static_cast<std::uint8_t>((packed >> 27) & 0x07u);
    if (src >= sizeof(kBlendFactors) / sizeof(kBlendFactors[0]) ||
        dst >= sizeof(kBlendFactors) / sizeof(kBlendFactors[0]))
        return Error::blend_factor;
    if (equation >= sizeof(kBlendEquations) / sizeof(kBlendEquations[0]))
        return Error::blend_equation;
    if (compare >= sizeof(kCompareFunctions) / sizeof(kCompareFunctions[0]))
        return Error::depth_function;

    decoded.blend_enabled = (flags & (1u << 16)) != 0;
    decoded.depth_test_enabled = (flags & (1u << 19)) != 0;
    decoded.depth_write_enabled = (flags & (1u << 20)) != 0;
    decoded.blend_source = map_value(src, kBlendFactors[src]);
    decoded.blend_destination = map_value(dst, kBlendFactors[dst]);
    decoded.blend_equation = map_value(equation, kBlendEquations[equation]);
    decoded.depth_function = map_value(compare, kCompareFunctions[compare]);
    decoded.cull_face_ordinal = static_cast<std::uint8_t>(packed >> 30);
    output = decoded;
    return Error::ok;
}

const char* error_name(Error error) noexcept {
    switch (error) {
    case Error::ok: return "ok";
    case Error::argument: return "argument";
    case Error::size: return "size";
    case Error::blend_factor: return "blend_factor";
    case Error::blend_equation: return "blend_equation";
    case Error::depth_function: return "depth_function";
    }
    return "unknown";
}

} // namespace dh2::scene_materials::render_state
