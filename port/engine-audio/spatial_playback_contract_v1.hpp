#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <limits>

// Typed source boundary for VoxSoundManager::Play3D. It reproduces the
// recovered stereo pan and distance model 2 gains; other distance models
// remain unsupported and fail closed.
// Evidence: Play3D 0x36b5d8, PlaySoundPackSound 0x36a7c0,
// SetListenerPos 0x369f50, DriverCallbackSourceInterface setters/getters
// 0x8909e8, 0x892114, 0x891b80 in libDungeonHunter2.so.
namespace dh2::engine_audio::spatial_playback_contract_v1 {

inline constexpr std::int32_t observed_default_distance_model_id = 2;
inline constexpr std::int32_t raw_emitter_parameter_0_id = 0;

// Original Arrays::Listeners records decoded by Arrays::Listeners::read
// (0x004b9578), with member order confirmed by Structs::Listener::read
// (0x004ed978). Selector IDs stay raw. Level::UpdateListener (0x003f0ae0)
// consumes Anchor/Orientation/UpVector and passes RefDistance, MaxDistance,
// RolloffFactor to VoxSoundManager::SetListenerPos in that argument order.
struct ListenerProfile {
    const char* name;
    std::int32_t anchor;
    std::int32_t max_distance;
    std::int32_t orientation;
    std::int32_t reference_distance;
    float rolloff_factor;
    std::int32_t up_vector;
};
inline constexpr std::array<ListenerProfile,5> listener_profiles{{
    {"AAA_DONT_DELETE_Listener",3,2500,0,3150,1.0f,0},
    {"curListener",2,1800,2,1000,1.0f,0},
    {"PlayerListener",2,1800,2,1000,1.0f,0},
    {"BAPListener",1,3500,2,1500,1.0f,0},
    {"CameraListener",0,2500,0,3150,1.0f,0},
}};
inline const ListenerProfile* listener_profile(std::int32_t id) noexcept {
    if(id<0||static_cast<std::size_t>(id)>=listener_profiles.size())return nullptr;
    return &listener_profiles[static_cast<std::size_t>(id)];
}

// Offsets recovered from VoxSoundManager::PlaySoundPackSound and the
// DriverCallbackSourceInterface::Set3DParameter switch. Keep these as raw
// wire facts; no public semantic names/units were recovered for parameters
// 1..3.
constexpr std::int32_t manager_field_offset_for_emitter_parameter(
    std::int32_t parameter_id) noexcept {
    switch (parameter_id) {
        case 1: return 0x58;
        case 2: return 0x54;
        case 3: return 0x5c;
        default: return -1;
    }
}

constexpr std::int32_t driver_field_offset_for_emitter_parameter(
    std::int32_t parameter_id) noexcept {
    switch (parameter_id) {
        case 0: return 0x90;
        case 1: return 0x94;
        case 2: return 0x98;
        case 3: return 0x9c;
        case 4: return 0xa0;
        case 5: return 0xa4;
        case 6: return 0xa8;
        case 8: return 0x6c;
        case 9: return 0x78;
        case 10: return 0x84;
        default: return -1;
    }
}

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct Listener {
    Vec3 position;
    Vec3 forward;
    Vec3 up;
};

struct EmitterParameters {
    // Raw Vox emitter parameter IDs 1, 2, and 3. At PlaySoundPackSound they
    // are supplied from VoxSoundManager fields +0x58, +0x54, and +0x5c.
    // Their public semantic names/units are not present in the local source.
    std::array<float, 3> by_source_id{};
};

enum class Validation : std::uint8_t {
    ready,
    missing_sound_id,
    missing_emitter_position,
    missing_listener,
    missing_emitter_parameters,
    missing_distance_model,
    unsupported_distance_model,
    non_finite_input,
};

struct StereoGainFloats {
    float left = 0.0f;
    float right = 0.0f;
};

enum class StereoPanStatus : std::uint8_t {
    ready,
    degenerate_center,
    invalid_request,
    non_finite_gain,
};

struct StereoPanResult {
    StereoPanStatus status = StereoPanStatus::invalid_request;
    Validation request_validation = Validation::missing_sound_id;
    float pan = 0.0f;
    StereoGainFloats gains;
    std::int32_t left_q14 = 0;
    std::int32_t right_q14 = 0;
    float distance_gain = 1.0f;
    std::int32_t distance_gain_q14 = 16384;
    bool has_q14 = false;
    bool has_distance_gain_q14 = false;
};

struct Request {
    std::uint16_t sound_id = 0;
    Vec3 emitter_position;
    Listener listener;
    EmitterParameters emitter_parameters;
    // EmitterObj::SetDefaultParameters initializes wire ID0 to integer 0.
    // GetStereoPanning takes a different path when this raw value is nonzero.
    std::int32_t raw_parameter_0 = 0;
    std::int32_t distance_model_id = observed_default_distance_model_id;
    bool has_sound_id = false;
    bool has_emitter_position = false;
    bool has_listener = false;
    bool has_emitter_parameters = false;
    bool has_distance_model = false;
};

inline bool finite(Vec3 value) noexcept {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

inline Validation validate(const Request& request) noexcept {
    if (!request.has_sound_id) return Validation::missing_sound_id;
    if (!request.has_emitter_position) return Validation::missing_emitter_position;
    if (!request.has_listener) return Validation::missing_listener;
    if (!request.has_emitter_parameters) return Validation::missing_emitter_parameters;
    if (!request.has_distance_model) return Validation::missing_distance_model;
    if (request.distance_model_id != observed_default_distance_model_id)
        return Validation::unsupported_distance_model;
    if (!finite(request.emitter_position) || !finite(request.listener.position) ||
        !finite(request.listener.forward) || !finite(request.listener.up))
        return Validation::non_finite_input;
    for (const float value : request.emitter_parameters.by_source_id)
        if (!std::isfinite(value)) return Validation::non_finite_input;
    return Validation::ready;
}

// Direct transcription of the GetStereoPanning equal-power tail at 0x891c10:
// pan+1, then *0.5 and sqrt for the right channel; square that result and
// calculate sqrt(1-right^2) for the left channel. The original does not clamp.
inline StereoGainFloats equal_power_gains(float pan) noexcept {
    const float pan_plus_one = pan + 1.0f;
    const float right_argument = pan_plus_one * 0.5f;
    const float right = std::sqrt(right_argument);
    const float right_squared = right * right;
    const float left_argument = 1.0f - right_squared;
    const float left = std::sqrt(left_argument);
    return {left, right};
}

inline float length(Vec3 vector) noexcept {
    const float x_squared = vector.x * vector.x;
    const float y_squared = vector.y * vector.y;
    const float xy_squared = x_squared + y_squared;
    const float z_squared = vector.z * vector.z;
    return std::sqrt(xy_squared + z_squared);
}

inline float source_distance(const Request& request) noexcept {
    if (request.raw_parameter_0 != 0) return length(request.emitter_position);
    return length({
        request.emitter_position.x - request.listener.position.x,
        request.emitter_position.y - request.listener.position.y,
        request.emitter_position.z - request.listener.position.z,
    });
}

// GetDistanceGain model 2 at 0x8921d0. Parameter 1 is the upper clamp,
// parameter 2 the lower/reference distance, and parameter 3 rolloff; labels
// remain raw IDs because no public source names were recovered.
inline float model_2_distance_gain(float distance,
                                   const EmitterParameters& parameters) noexcept {
    const float upper = parameters.by_source_id[0];
    const float reference = parameters.by_source_id[1];
    const float rolloff = parameters.by_source_id[2];
    float clamped = distance;
    if (reference > clamped) clamped = reference;
    if (upper < clamped) clamped = upper;
    const float offset = clamped - reference;
    const float rolled_offset = offset * rolloff;
    const float denominator = reference + rolled_offset;
    if (!(denominator > 0.0f)) return 1.0f;
    return reference / denominator;
}

// Mirrors the two branches in Vox::DriverCallbackSourceInterface::GetStereoPanning
// (0x891b80). raw_parameter_0 != 0 uses emitter.x / |emitter|. The zero path
// uses emitter-listener, normalizes it and forward x up, then dots them in XYZ
// order. All arithmetic intentionally remains float and the pan is not clamped.
inline StereoPanResult evaluate_stereo_pan(const Request& request) noexcept {
    StereoPanResult result{};
    result.request_validation = validate(request);
    if (result.request_validation != Validation::ready) {
        result.status = StereoPanStatus::invalid_request;
        return result;
    }

    bool degenerate = false;
    const float distance = source_distance(request);
    result.distance_gain = model_2_distance_gain(distance, request.emitter_parameters);
    if (request.raw_parameter_0 != 0) {
        const float emitter_length = distance;
        if (!(emitter_length > 0.0f)) {
            degenerate = true;
        } else {
            result.pan = request.emitter_position.x / emitter_length;
        }
    } else {
        const Vec3 delta{
            request.emitter_position.x - request.listener.position.x,
            request.emitter_position.y - request.listener.position.y,
            request.emitter_position.z - request.listener.position.z,
        };
        const float delta_length = length(delta);
        const Vec3 right{
            request.listener.forward.y * request.listener.up.z -
                request.listener.forward.z * request.listener.up.y,
            request.listener.forward.z * request.listener.up.x -
                request.listener.forward.x * request.listener.up.z,
            request.listener.forward.x * request.listener.up.y -
                request.listener.forward.y * request.listener.up.x,
        };
        const float right_length = length(right);
        if (!(delta_length > 0.0f) || !(right_length > 0.0f)) {
            degenerate = true;
        } else {
            const float normalized_delta_x = delta.x / delta_length;
            const float normalized_right_x = right.x / right_length;
            const float x_product = normalized_delta_x * normalized_right_x;
            const float normalized_delta_y = delta.y / delta_length;
            const float normalized_right_y = right.y / right_length;
            const float y_product = normalized_delta_y * normalized_right_y;
            const float xy_product = x_product + y_product;
            const float normalized_delta_z = delta.z / delta_length;
            const float normalized_right_z = right.z / right_length;
            const float z_product = normalized_delta_z * normalized_right_z;
            result.pan = xy_product + z_product;
        }
    }

    if (degenerate) result.pan = 0.0f;
    result.gains = equal_power_gains(result.pan);
    if (!std::isfinite(result.gains.left) || !std::isfinite(result.gains.right)) {
        result.status = StereoPanStatus::non_finite_gain;
        return result;
    }

    constexpr float q14_scale = 16384.0f;
    result.left_q14 = static_cast<std::int32_t>(result.gains.left * q14_scale);
    result.right_q14 = static_cast<std::int32_t>(result.gains.right * q14_scale);
    result.has_q14 = true;
    const float distance_gain_q14 = result.distance_gain * q14_scale;
    if (!std::isfinite(distance_gain_q14) ||
        distance_gain_q14 > static_cast<float>(std::numeric_limits<std::int32_t>::max()) ||
        distance_gain_q14 < static_cast<float>(std::numeric_limits<std::int32_t>::min())) {
        result.status = StereoPanStatus::non_finite_gain;
        result.has_q14 = false;
        return result;
    }
    result.distance_gain_q14 = static_cast<std::int32_t>(distance_gain_q14);
    result.has_distance_gain_q14 = true;
    result.status = degenerate ? StereoPanStatus::degenerate_center : StereoPanStatus::ready;
    return result;
}

} // namespace dh2::engine_audio::spatial_playback_contract_v1
