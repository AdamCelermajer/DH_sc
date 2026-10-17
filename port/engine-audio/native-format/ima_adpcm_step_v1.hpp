#ifndef DH2_ENGINE_AUDIO_NATIVE_FORMAT_IMA_ADPCM_STEP_V1_HPP
#define DH2_ENGINE_AUDIO_NATIVE_FORMAT_IMA_ADPCM_STEP_V1_HPP

#include <array>
#include <cstdint>

// Source-backed single-nibble primitive from VoxNativeSubDecoderIMAADPCM::DecodeBlock.
// This is not a VoxN block/segment/container decoder.
namespace dh2::engine_audio::native_format {

struct ImaAdpcmStateV1 {
    std::int16_t predictor;
    std::uint8_t step_index;
};

inline constexpr std::array<std::int16_t, 89> kImaAdpcmStepTableV1{{
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31,
    34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130,
    143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449,
    494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282, 1411,
    1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026,
    4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487,
    12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
    32767,
}};

inline constexpr std::array<std::int8_t, 16> kImaAdpcmIndexTableV1{{
    -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8,
}};

// DecodeBlock reads the low nibble first, uses arithmetic shifts for the
// IMA delta terms, clamps predictor to int16 and index to [0, 88].
inline std::int16_t ima_adpcm_decode_nibble_v1(ImaAdpcmStateV1* state,
                                                std::uint8_t nibble) noexcept {
    if (state == nullptr || nibble > 0x0f || state->step_index >= kImaAdpcmStepTableV1.size())
        return 0;

    const std::int32_t step = kImaAdpcmStepTableV1[state->step_index];
    std::int32_t delta = step >> 3;
    if ((nibble & 4u) != 0) delta += step;
    if ((nibble & 2u) != 0) delta += step >> 1;
    if ((nibble & 1u) != 0) delta += step >> 2;

    std::int32_t predictor = state->predictor;
    predictor += (nibble & 8u) != 0 ? -delta : delta;
    if (predictor < -32768) predictor = -32768;
    if (predictor > 32767) predictor = 32767;

    std::int32_t index = static_cast<std::int32_t>(state->step_index) +
                         kImaAdpcmIndexTableV1[nibble];
    if (index < 0) index = 0;
    if (index > 88) index = 88;
    state->predictor = static_cast<std::int16_t>(predictor);
    state->step_index = static_cast<std::uint8_t>(index);
    return state->predictor;
}

} // namespace dh2::engine_audio::native_format
#endif
