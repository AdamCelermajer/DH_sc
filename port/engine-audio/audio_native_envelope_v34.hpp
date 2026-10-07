#pragma once
#include <cstdint>
namespace dh2::audio {
// Source SegmentState +28/+30/+34/+38 and source life +24 (offsets in hex).
struct AudioNativeEnvelopeV34 {
 std::int32_t delay_frames{},remaining_frames{},increment_q30{},gain_q30{};
 bool stopped{};
};
// Exact original MixSegmentInBuffer8844e8 arithmetic, into a caller-owned
// interleaved int32 accumulator. No allocations; advance once per frame.
bool audio_native_envelope_mix_v34(const std::int16_t*,std::uint32_t frames,
 std::uint32_t channels,std::int32_t*,AudioNativeEnvelopeV34&) noexcept;
}
