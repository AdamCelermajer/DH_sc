#pragma once

#include <cstddef>
#include <cstdint>

// Checked reconstruction of the original video::SRenderState ->
// renderpass::SRenderState copy constructor. This is a byte-level conversion,
// not either original C++ ABI or a native struct overlay.
namespace dh2::scene_materials::source_state {

enum class Error : std::uint8_t { ok, argument, input_size, output_size };

constexpr std::size_t kVideoStateBytes = 0x4c;
constexpr std::size_t kRenderPassStateBytes = 0x20;

// Replays the field extraction/packing in the pinned ARM constructor at ELF
// 0x005d7a10. Both spans are exact-size little-endian byte sequences.
Error convert_video_state(const std::uint8_t* source, std::size_t source_size,
                          std::uint8_t* output, std::size_t output_size) noexcept;

const char* error_name(Error error) noexcept;

} // namespace dh2::scene_materials::source_state
