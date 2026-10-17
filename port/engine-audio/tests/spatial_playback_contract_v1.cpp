#include "../spatial_playback_contract_v1.hpp"

#include <cmath>
#include <limits>

using namespace dh2::engine_audio::spatial_playback_contract_v1;

static_assert(raw_emitter_parameter_0_id == 0);
static_assert(manager_field_offset_for_emitter_parameter(1) == 0x58);
static_assert(manager_field_offset_for_emitter_parameter(2) == 0x54);
static_assert(manager_field_offset_for_emitter_parameter(3) == 0x5c);
static_assert(manager_field_offset_for_emitter_parameter(0) == -1);
static_assert(driver_field_offset_for_emitter_parameter(0) == 0x90);
static_assert(driver_field_offset_for_emitter_parameter(1) == 0x94);
static_assert(driver_field_offset_for_emitter_parameter(2) == 0x98);
static_assert(driver_field_offset_for_emitter_parameter(3) == 0x9c);
static_assert(driver_field_offset_for_emitter_parameter(4) == 0xa0);
static_assert(driver_field_offset_for_emitter_parameter(5) == 0xa4);
static_assert(driver_field_offset_for_emitter_parameter(6) == 0xa8);
static_assert(driver_field_offset_for_emitter_parameter(8) == 0x6c);
static_assert(driver_field_offset_for_emitter_parameter(9) == 0x78);
static_assert(driver_field_offset_for_emitter_parameter(10) == 0x84);
static_assert(driver_field_offset_for_emitter_parameter(7) == -1);

int main() {
    Request request{};
    if (validate(request) != Validation::missing_sound_id) return 1;

    request.has_sound_id = true;
    request.sound_id = 118;
    if (validate(request) != Validation::missing_emitter_position) return 2;

    request.has_emitter_position = true;
    request.emitter_position = {1.0f, 2.0f, 3.0f};
    if (validate(request) != Validation::missing_listener) return 3;

    request.has_listener = true;
    request.listener = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}};
    if (validate(request) != Validation::missing_emitter_parameters) return 4;

    request.has_emitter_parameters = true;
    request.emitter_parameters.by_source_id = {50.0f, 1.0f, 1.0f};
    if (validate(request) != Validation::missing_distance_model) return 5;

    request.has_distance_model = true;
    if (validate(request) != Validation::ready) return 6;

    request.distance_model_id = 3;
    if (validate(request) != Validation::unsupported_distance_model) return 7;

    request.distance_model_id = observed_default_distance_model_id;
    request.listener.up.y = std::numeric_limits<float>::quiet_NaN();
    if (validate(request) != Validation::non_finite_input) return 8;

    request.listener.up.y = 1.0f;
    request.emitter_parameters.by_source_id[1] = std::numeric_limits<float>::infinity();
    if (validate(request) != Validation::non_finite_input) return 9;

    request.emitter_parameters.by_source_id[1] = 1.0f;
    request.listener = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}};
    request.emitter_position = {-2.0f, 0.0f, 0.0f};
    StereoPanResult pan = evaluate_stereo_pan(request);
    if (pan.status != StereoPanStatus::ready || pan.pan != 1.0f ||
        pan.left_q14 != 0 || pan.right_q14 != 16384 ||
        !pan.has_distance_gain_q14 || pan.distance_gain_q14 != 8192) return 10;

    request.emitter_position = {2.0f, 0.0f, 0.0f};
    pan = evaluate_stereo_pan(request);
    if (pan.status != StereoPanStatus::ready || pan.pan != -1.0f ||
        pan.left_q14 != 16384 || pan.right_q14 != 0 ||
        pan.distance_gain_q14 != 8192) return 11;

    request.emitter_position = {0.0f, 0.0f, 0.0f};
    pan = evaluate_stereo_pan(request);
    if (pan.status != StereoPanStatus::degenerate_center || pan.pan != 0.0f ||
        pan.left_q14 != 11585 || pan.right_q14 != 11585 ||
        pan.distance_gain_q14 != 16384) return 12;

    request.listener.forward = {0.0f, 0.0f, 0.0f};
    pan = evaluate_stereo_pan(request);
    if (pan.status != StereoPanStatus::degenerate_center ||
        pan.left_q14 != 11585 || pan.right_q14 != 11585) return 13;

    request.raw_parameter_0 = 1;
    request.emitter_position = {3.0f, 4.0f, 0.0f};
    pan = evaluate_stereo_pan(request);
    if (pan.status != StereoPanStatus::ready || std::fabs(pan.pan - 0.6f) > 0.00001f ||
        pan.left_q14 != 7327 || pan.right_q14 != 14654 ||
        pan.distance_gain_q14 != 3276) return 14;

    const StereoGainFloats out_of_range = equal_power_gains(1.5f);
    if (!std::isnan(out_of_range.left) || !(out_of_range.right > 1.0f)) return 15;
    return 0;
}
