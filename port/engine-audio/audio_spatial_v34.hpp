#pragma once
#include <cstdint>
namespace dh2::audio {
struct AudioListenerV34 {float position[3]{},velocity[3]{},front[3]{},up[3]{};};
struct AudioSpatialSourceV34 {
 float position[3]{},velocity[3]{};
 bool relative{};std::int32_t distance_model{};
 float reference_distance{},maximum_distance{},rolloff{},doppler_factor{},speed_over_doppler{};
};
struct AudioSpatialResultV34 {std::int32_t left_q14{},right_q14{},distance_q14{},doppler_q14{};};
// Whole source GetStereoPanning891b80/GetDistanceGain892114/GetDopplerPitch891ed0
// finite domain. Inputs are actual source driver/listener fields in sound units.
// No camera/orientation/listener or distance-model defaults are supplied.
bool audio_spatial_v34(const AudioListenerV34&,const AudioSpatialSourceV34&,
 AudioSpatialResultV34&) noexcept;
}
