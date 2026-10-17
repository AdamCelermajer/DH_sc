#pragma once

#include "spatial_playback_contract_v1.hpp"
#include <cstddef>
#include <cstdint>

// Source Level::UpdateListener (ELF 0x003f0ae0) and its one process-lifetime
// cached Arrays::Listeners row. Callers borrow the selected Level's listener
// index and the actual Arrays::Listeners records; this owner does not choose
// a camera mode or synthesize a fallback position.
namespace dh2::engine_audio::native_level_listener_v1 {

using spatial_playback_contract_v1::Listener;
using spatial_playback_contract_v1::ListenerProfile;
using spatial_playback_contract_v1::Request;
using spatial_playback_contract_v1::Vec3;

enum class Status : std::uint8_t {
    updated,
    skipped_without_camera_node,
    invalid_request,
    listener_row_unavailable,
    listener_table_changed,
    provider_missing,
    provider_failed,
};

enum class Provider : std::uint8_t {
    none,
    camera_position,
    player_position,
    world_to_screen,
    active_render_target_height,
    screen_to_world,
    screen_dimensions,
    player_look_at,
    camera_look_at,
    camera_up,
    player_visual_up,
};

// Level::UpdateListener's function-local static caches the first
// Arrays::Listeners row selected from Level+0xe4 for the process lifetime.
// Keep that choice once, while reading its row from the same borrowed table on
// each update. The source Arrays table is process-owned and must stay stable.
struct State {
    const ListenerProfile* rows = nullptr;
    std::size_t row_count = 0;
    std::int32_t selected_index = -1;
};

// The one retained VoxSoundManager state consumed by source Play3D. These
// fields correspond to SetListenerPos writes at manager+0x54/+0x58/+0x5c and
// to the listener vectors sent to VoxSoundManager::SetListenerPos.
struct VoxState {
    Listener listener{};
    std::int32_t reference_distance = 0; // manager +0x54
    std::int32_t max_distance = 0;       // manager +0x58
    float rolloff_factor = 0.0f;         // manager +0x5c
    bool listener_ready = false;

    void set_listener_pos(const Listener& next, std::int32_t reference,
                          std::int32_t maximum, float rolloff) noexcept {
        listener = next;
        reference_distance = reference;
        max_distance = maximum;
        rolloff_factor = rolloff;
        listener_ready = true;
    }

    spatial_playback_contract_v1::EmitterParameters emitter_parameters() const noexcept {
        spatial_playback_contract_v1::EmitterParameters result{};
        // PlaySoundPackSound sets emitter parameter 1 from manager+0x58,
        // parameter 2 from +0x54, and parameter 3 from +0x5c.
        result.by_source_id = {static_cast<float>(max_distance),
                               static_cast<float>(reference_distance),
                               rolloff_factor};
        return result;
    }
};

// The source has one process-wide Level::UpdateListener cache and one
// VoxSoundManager singleton. Native callers share these owners instead of
// constructing per-actor or per-callback copies.
inline State& process_state() noexcept {
    static State value{};
    return value;
}
inline VoxState& process_vox_state() noexcept {
    static VoxState value{};
    return value;
}

struct FrameRequest {
    // Level+0xe4, read only when State selects its first row.
    std::int32_t level_listener_index = -1;
    const ListenerProfile* listener_rows = nullptr;
    std::size_t listener_row_count = 0;

    // Source Width_Screen/Height_Screen used by anchor selector 3.
    std::int32_t screen_width = 0;
    std::int32_t screen_height = 0;
    bool screen_dimensions_present = false;
    // The active IRenderTarget height used by anchor selector 1 after
    // GetScreenPosFromWorldPos returns the Player's screen X coordinate.
    std::int32_t active_render_target_height = 0;
    bool active_render_target_height_present = false;

    // Level::UpdateListener only calls SetListenerPos when CameraBase+8 exists.
    bool camera_node_present = false;
    // PlayerManager::GetLocalPlayer(...)->object+0x660 exists.
    bool player_object_present = false;
};

struct Services {
    void* context = nullptr;
    bool (*camera_absolute_position)(void*, Vec3&) = nullptr;
    bool (*player_target_position)(void*, Vec3&) = nullptr;
    bool (*world_to_screen)(void*, const Vec3&, std::int32_t& x,
                            std::int32_t& y) = nullptr;
    bool (*screen_to_world)(void*, float x, float y, float depth,
                            Vec3&) = nullptr;
    bool (*player_look_at)(void*, Vec3&) = nullptr;
    bool (*camera_look_at)(void*, Vec3&) = nullptr;
    bool (*camera_up)(void*, Vec3&) = nullptr;
    // Source up-selector 1 reads the player's visual node's basis vector at
    // indices 8..10. The source stores this into the forward vector scratch,
    // leaving the up-vector scratch untouched; preserve that behavior.
    bool (*player_visual_up)(void*, Vec3&) = nullptr;
};

struct Result {
    Status status = Status::invalid_request;
    Provider failed_provider = Provider::none;
    std::int32_t selected_listener_index = -1;
    ListenerProfile profile{};
    Listener output{};
};

inline bool request_valid(const FrameRequest& request) noexcept {
    return request.listener_rows && request.listener_row_count != 0;
}

inline bool require(bool available, Provider provider, Result& result) noexcept {
    if (available) return true;
    result.status = Status::provider_missing;
    result.failed_provider = provider;
    return false;
}

inline bool service_result(bool succeeded, Provider provider,
                           Result& result) noexcept {
    if (succeeded) return true;
    result.status = Status::provider_failed;
    result.failed_provider = provider;
    return false;
}

inline std::int32_t truncate_half(std::int32_t value) noexcept {
    // ARM signed division in Level::UpdateListener rounds toward zero.
    return value / 2;
}

// Source Level::UpdateListener (0x003f0ae0) transcribed from its switch and
// call sites. Projection callbacks must use the active source camera's
// GetScreenPosFromWorldPos/GetWorldPosFromScreenPos services.
inline Result update(const FrameRequest& request, const Services& services,
                     State& state, VoxState& vox) noexcept {
    Result result{};
    if (!request_valid(request)) {
        result.status = Status::invalid_request;
        return result;
    }

    if (state.selected_index < 0) {
        const auto index = request.level_listener_index;
        if (index < 0 || static_cast<std::size_t>(index) >= request.listener_row_count) {
            result.status = Status::listener_row_unavailable;
            return result;
        }
        state.rows = request.listener_rows;
        state.row_count = request.listener_row_count;
        state.selected_index = index;
    } else if (state.rows != request.listener_rows ||
               state.row_count != request.listener_row_count) {
        result.status = Status::listener_table_changed;
        result.selected_listener_index = state.selected_index;
        return result;
    }

    const auto index = state.selected_index;
    if (index < 0 || static_cast<std::size_t>(index) >= state.row_count) {
        result.status = Status::listener_row_unavailable;
        result.selected_listener_index = index;
        return result;
    }
    result.selected_listener_index = index;
    result.profile = state.rows[static_cast<std::size_t>(index)];

    const auto& row = result.profile;
    Listener listener{}; // source initializes all position/look/up scratch to 0
    Vec3 player_position{};

    // Position selector at Listener+4.
    switch (row.anchor) {
    case 0: // Camera scene node absolute position.
        if (request.camera_node_present) {
            if (!require(services.camera_absolute_position != nullptr,
                         Provider::camera_position, result) ||
                !service_result(services.camera_absolute_position(
                                    services.context, listener.position),
                                Provider::camera_position, result)) return result;
        }
        break;
    case 1: // Player target projected to screen, then unprojected at bottom edge.
        if (request.player_object_present) {
            if (!require(services.player_target_position != nullptr,
                         Provider::player_position, result) ||
                !service_result(services.player_target_position(
                                    services.context, player_position),
                                Provider::player_position, result) ||
                !require(services.world_to_screen != nullptr,
                         Provider::world_to_screen, result)) return result;
            std::int32_t screen_x = 0, screen_y = 0;
            if (!service_result(services.world_to_screen(services.context,
                                    player_position, screen_x, screen_y),
                                Provider::world_to_screen, result)) return result;
            if (!request.active_render_target_height_present) {
                result.status = Status::provider_missing;
                result.failed_provider = Provider::active_render_target_height;
                return result;
            }
            if (!require(services.screen_to_world != nullptr,
                         Provider::screen_to_world, result) ||
                !service_result(services.screen_to_world(services.context,
                                    static_cast<float>(screen_x),
                                    static_cast<float>(request.active_render_target_height),
                                    0.0f, listener.position),
                                Provider::screen_to_world, result)) return result;
        }
        break;
    case 2: // Player GameObject::GetTargetPosition.
        if (request.player_object_present) {
            if (!require(services.player_target_position != nullptr,
                         Provider::player_position, result) ||
                !service_result(services.player_target_position(
                                    services.context, listener.position),
                                Provider::player_position, result)) return result;
        }
        break;
    case 3: { // World position at Width_Screen/2, Height_Screen/2, depth 0.
        if (!request.screen_dimensions_present) {
            result.status = Status::provider_missing;
            result.failed_provider = Provider::screen_dimensions;
            return result;
        }
        if (!require(services.screen_to_world != nullptr,
                     Provider::screen_to_world, result) ||
            !service_result(services.screen_to_world(services.context,
                                static_cast<float>(truncate_half(request.screen_width)),
                                static_cast<float>(truncate_half(request.screen_height)),
                                0.0f, listener.position),
                            Provider::screen_to_world, result)) return result;
        break;
    }
    default:
        // Source has no default branch; its initialized zero vector survives.
        break;
    }

    // Orientation selector at Listener+12. Selector 2 deliberately forces Z
    // to zero; selector 0 keeps CameraBase's complete look-at vector.
    if (row.orientation == 1 && request.player_object_present) {
        if (!require(services.player_look_at != nullptr,
                     Provider::player_look_at, result) ||
            !service_result(services.player_look_at(services.context,
                                listener.forward),
                            Provider::player_look_at, result)) return result;
    } else if ((row.orientation == 0 || row.orientation == 2) &&
               request.camera_node_present) {
        if (!require(services.camera_look_at != nullptr,
                     Provider::camera_look_at, result) ||
            !service_result(services.camera_look_at(services.context,
                                listener.forward),
                            Provider::camera_look_at, result)) return result;
        if (row.orientation == 2) listener.forward.z = 0.0f;
    }

    // Up selector at Listener+24. The source's selector-1 branch obtains the
    // visual up basis but writes it into the same scratch passed as look-at.
    if (row.up_vector == 0 && request.camera_node_present) {
        if (!require(services.camera_up != nullptr,
                     Provider::camera_up, result) ||
            !service_result(services.camera_up(services.context, listener.up),
                            Provider::camera_up, result)) return result;
    } else if (row.up_vector == 1 && request.player_object_present) {
        Vec3 source_visual_up{};
        if (!require(services.player_visual_up != nullptr,
                     Provider::player_visual_up, result) ||
            !service_result(services.player_visual_up(services.context,
                                source_visual_up),
                            Provider::player_visual_up, result)) return result;
        listener.forward = source_visual_up;
    }

    // Level::UpdateListener returns without calling SetListenerPos unless
    // CameraBase has a live scene node, even when another selector supplied
    // otherwise complete vectors.
    if (!request.camera_node_present) {
        result.status = Status::skipped_without_camera_node;
        result.output = listener;
        return result;
    }

    vox.set_listener_pos(listener, row.reference_distance, row.max_distance,
                         row.rolloff_factor);
    result.status = Status::updated;
    result.output = listener;
    return result;
}

// Construct the exact spatial contract inputs used by PlaySoundPackSound.
// Caller supplies the source Play3D position and raw Sounds-table id.
inline bool make_spatial_request(const VoxState& vox, std::uint16_t sound_id,
                                const Vec3& emitter_position,
                                Request& request) noexcept {
    if (!vox.listener_ready) return false;
    request = {};
    request.sound_id = sound_id;
    request.emitter_position = emitter_position;
    request.listener = vox.listener;
    request.emitter_parameters = vox.emitter_parameters();
    request.distance_model_id = spatial_playback_contract_v1::
        observed_default_distance_model_id;
    request.has_sound_id = true;
    request.has_emitter_position = true;
    request.has_listener = true;
    request.has_emitter_parameters = true;
    request.has_distance_model = true;
    return spatial_playback_contract_v1::validate(request) ==
           spatial_playback_contract_v1::Validation::ready;
}

} // namespace dh2::engine_audio::native_level_listener_v1
