#pragma once

// RIFF/WAVE PCM16 image builder (Preview 15). Portable: produces the bytes that
// audio_sample_open_v34 accepts; no platform calls.

#include <cstdint>
#include <vector>

namespace dh::foundation::startup {

// Interleaved int16 samples -> RIFF/WAVE PCM16 file image (44-byte header).
std::vector<std::uint8_t> make_pcm16_wav(const std::vector<std::int16_t>& pcm, std::uint32_t rate, std::uint32_t channels);

} // namespace dh::foundation::startup
