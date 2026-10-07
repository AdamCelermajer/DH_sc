#pragma once
#include "../../../audio_spatial_v34.hpp"
#include <cstdint>
#include <cstring>
namespace dh2::audio {
// Exact original static .data initializers. A later source setter overrides
// these values; this is not a current-state getter or an initialized GS proof.
struct OriginalVoxGeneralV40 {
    std::int32_t distance_model{2};
    float doppler_factor{1.f};
    float speed_over_doppler{343.3f};
};
inline OriginalVoxGeneralV40 original_vox_boot_general_v40() noexcept { return {}; }
// Reached VoxSoundManager::Initialize36c3d8 sets integer parameter2 to4.
inline void original_vox_manager_general_init_v40(OriginalVoxGeneralV40& general) noexcept {
    general.distance_model=4;
}
// Fresh EmitterObj constructor/Reset3D only. Actual PlaySoundPackSound field
// writes and listener authority must be applied after these defaults.
inline AudioSpatialSourceV34 original_fresh_emitter_fields_v40(const OriginalVoxGeneralV40& general) noexcept {
    AudioSpatialSourceV34 result{};
    result.reference_distance=100.f;
    const std::uint32_t maximum_bits=0x7f7fffff;
    std::memcpy(&result.maximum_distance,&maximum_bits,4);
    result.rolloff=1.f;
    result.distance_model=general.distance_model;
    result.doppler_factor=general.doppler_factor;
    result.speed_over_doppler=general.speed_over_doppler;
    return result;
}
inline constexpr float original_fresh_emitter_gain_v40() noexcept { return 1.f; }
inline constexpr float original_fresh_emitter_pitch_v40() noexcept { return 1.f; }
}
