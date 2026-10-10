#pragma once

#include "camera_design_zoom_v1.hpp"

#include <cstdint>

namespace dh2::camera_level_runtime_v1 {

struct Vec3 { float x=0.0f, y=0.0f, z=0.0f; };

// A borrowed projection of the original GameObject fields used by the camera:
// +352 is the normal world position, +736 is an optional custom anchor pointer,
// and +129 retires the CameraTarget after the frame. The adapter resolves the
// custom pointer's +12 vector exactly as GameObject::GetCameraAnchorPosition.
struct GameObjectSample {
    std::uintptr_t identity=0;
    bool present=false;
    Vec3 object_position{};
    bool custom_anchor_present=false;
    Vec3 custom_anchor_plus_12{};
    bool clear_camera_target_after_update=false;
};

bool resolve_camera_anchor(const GameObjectSample&, Vec3* out) noexcept;

struct CameraBaseOffsetInput {
    // Camera node matrix/basis values read by CameraBase::GetCenterOffset from
    // the object returned by its virtual +56 accessor, offsets +16/+20/+24.
    Vec3 camera_basis_column{};
    float vertical_fov_radians=0.0f; // camera virtual +296
    float target_z=0.0f;
};
bool camera_base_get_center_offset(const CameraBaseOffsetInput&,Vec3 camera_world_position,
                                   Vec3* out) noexcept;

enum class Status : std::uint8_t {
    invalid_input,
    no_camera_update,
    transition_position_written,
    follow_position_written
};

struct FrameInput {
    bool active_camera=false;
    bool camera_node_present=false;       // CameraTarget +4
    bool target_cam_node_present=false;  // CameraLevel +8
    bool target_game_object_present=false;
    GameObjectSample target{};            // borrowed target identity and source fields
    std::int32_t dt_ms=0;

    // Level::Update chooses GameObject +352 in this mode; otherwise it calls
    // CameraTarget::GetTargetPosition -> GameObject::GetCameraAnchorPosition.
    bool use_object_position=false;
    Vec3 animated_target_cam_offset{};    // CameraLevel +152/+156/+160 ZoomHandler pan state

    // Values produced by source providers, kept explicit so this owner does not
    // silently invent projection, multiplayer, asset-timeline or table data.
    CameraBaseOffsetInput center_offset_source{}; // raw CameraBase scene-node/FOV inputs
    Vec3 multiplayer_centering_delta{};   // CameraLevel::HandleCentering result
    camera_design_zoom_v1::Bounds design_zoom_bounds{}; // one global DesignSettingsTable row
    bool infinite_zoom=false;             // DebugSwitches::InfiniteZoom
    bool zoom_clamp_disabled=false;       // CameraLevel +134
    Vec3 camera_world_position{};         // CameraBase camera-node absolute position
};

struct FrameOutput {
    Vec3 camera_world_position{};
    Vec3 target_cam_local_position{};
    Vec3 target_before_damping{};
    bool target_was_retired=false;
};

// One-owner port of the CameraTarget/CameraLevel state transitions. Scene graph,
// animation sampling, ZoomHandler and DesignSettingsTable remain providers.
class Owner {
public:
    Owner() noexcept = default;

    // Source SetTarget(nullptr, ...) is a no-op. A positive duration captures
    // the old target's current camera anchor; nonpositive duration clears the
    // transition. A real target's identity must be stable for its lifetime.
    bool set_target(const GameObjectSample* next, std::int32_t duration_ms,
                    const GameObjectSample* old_target_sample) noexcept;
    void clear_target() noexcept;
    void enable_center_offset(bool enabled) noexcept;
    void enable_damping(bool enabled) noexcept;
    void set_damping_ratio(float ratio) noexcept;
    void set_ghost_camera_offset(Vec3 offset) noexcept;
    // Mirrors PlayAnim's successful-start path. preserve_zoom corresponds to
    // the source's a4 flag; end_animation mirrors CameraLevel::__Callback.
    void play_animation_started(bool preserve_zoom) noexcept;
    void end_animation() noexcept;
    void set_zoom(float current, float target) noexcept;
    void set_zoom_animation_state(bool active, bool preserve_zoom) noexcept;
    // NativeSetLockZoom writes CameraLevel +134; while set, HandleZoom skips
    // its normal clamp and NativePinchZoom ignores user input.
    void set_zoom_locked(bool locked) noexcept { zoom_locked_=locked; }
    void set_default_target_distance(float distance) noexcept;
    // MenuCharMenu_Map::CreateMapCamera explicitly sets CameraLevel +0x85.
    // The ordinary Level camera keeps its constructor default (false).
    void set_use_object_position(bool enabled) noexcept { use_object_position_=enabled; }

    Status update(const FrameInput&, FrameOutput*) noexcept;
    std::uintptr_t target_identity() const noexcept { return target_identity_; }
    Vec3 damping_velocity() const noexcept { return damping_velocity_; }
    float current_zoom() const noexcept { return current_zoom_; }
    float target_zoom() const noexcept { return target_zoom_; }
    float effective_zoom() const noexcept { return effective_zoom_; }
    // CameraLevel+0x85 is the original object-position/gesture mode byte.
    // Its value is supplied by the selected source frame provider.
    bool use_object_position() const noexcept { return use_object_position_; }
    std::int32_t transition_remaining_ms() const noexcept { return transition_remaining_ms_; }
    bool animation_active() const noexcept { return zoom_animation_active_; }
    bool zoom_locked() const noexcept { return zoom_locked_; }

private:
    std::uintptr_t target_identity_=0;
    Vec3 transition_start_{};
    std::int32_t transition_duration_ms_=0;
    std::int32_t transition_remaining_ms_=0;
    // CameraLevel leaves the target-camera node untouched on transition frames.
    Vec3 target_cam_local_position_{};
    bool center_offset_enabled_=false;
    bool damping_enabled_=true;
    float damping_ratio_=0.7f;
    Vec3 damping_velocity_{};
    Vec3 ghost_camera_offset_{};
    bool zoom_animation_active_=false;
    bool preserve_zoom_=false;
    bool zoom_locked_=false;
    bool use_object_position_=false;
    float current_zoom_=0.0f;
    // ZoomHandler's constructor initializes current to 0 and target to 1
    // (IDA source finding). Keep this distinct from effective_zoom_, which
    // CameraLevel derives during its first non-transition update.
    float target_zoom_=1.0f;
    float effective_zoom_=0.0f;
    float default_target_distance_=0.0f;
};

// Source-derived ZoomHandler input math (ZoomHandler::setCamera 0x381fb0,
// onEvent 0x382040, and touch onEvent 0x382bdc). Event decoding, menu hit
// testing, and native pointer tracking remain caller-owned providers.
struct DrawableTargetSize {
    std::int32_t width=0;
    std::int32_t height=0;
};

class ZoomInput {
public:
    // setCamera reads the active IRenderTarget width/height and computes
    // 1 / max(width,height). Rebinding resets the mouse drag gesture.
    bool set_camera(Owner* owner, DrawableTargetSize target) noexcept;
    // Surface resize updates ZoomHandler's live render-target sensitivity
    // without rebinding the CameraLevel or disturbing an active gesture.
    bool update_drawable_target(DrawableTargetSize target) noexcept;
    void clear_camera() noexcept;
    void set_mouse_pan_enabled(bool enabled) noexcept { mouse_pan_enabled_=enabled; }

    // SEvent type 1 / code 7: current zoom += delta * 5 * inverse max count.
    bool mouse_wheel(float delta) noexcept;
    // SEvent type 1 / code 0,6,3. `inside_map_render_zone` is supplied by the
    // original MenuCharMenu_Map hit-test; ordinary cameras do not use it.
    bool begin_mouse_pan(std::int32_t x, std::int32_t y, Vec3 camera_offset,
                         bool menu_camera, bool inside_map_render_zone) noexcept;
    bool move_mouse_pan(std::int32_t x, std::int32_t y, Vec3* camera_offset) const noexcept;
    bool end_mouse_pan() noexcept;

    // IEvent touch path: single-pointer deltas pan CameraLevel +152/+156 by
    // 50 world units per screen pixel; multi-pointer distance delta changes
    // current zoom by inverse max difficulty count.
    bool touch_pan(std::int32_t delta_x, std::int32_t delta_y,
                   Vec3* camera_offset, bool touch_pan_enabled) const noexcept;
    bool pinch_zoom(float previous_distance, float current_distance) noexcept;
    // NativePinchZoom callback: when DPad is enabled, convert its numeric
    // delta to int and apply it to CameraLevel +136 using ZoomHandler +40.
    // Unlike ZoomHandler's touch event, this authored callback is not gated
    // by CameraLevel +133 (map-camera mode).
    bool native_pinch_zoom(float input_delta,bool dpad_enabled) noexcept;

    bool mouse_pan_active() const noexcept { return mouse_pan_active_; }
    float sensitivity() const noexcept { return sensitivity_; }
    Vec3 mouse_pan_baseline() const noexcept { return mouse_pan_baseline_; }

private:
    Owner* owner_=nullptr;
    float sensitivity_=0.0f;
    bool mouse_pan_enabled_=false;
    bool mouse_pan_active_=false;
    std::int32_t mouse_start_x_=0;
    std::int32_t mouse_start_y_=0;
    Vec3 mouse_pan_baseline_{};
};

// Separate owner for MenuCharMenu_Map's interactive map camera. The HUD's
// miniature map only renders a temporary camera and does not bind gestures;
// this session models the full-screen map menu's CameraLevel/ZoomHandler pair.
// Scene activation, map-camera animation sampling, map bounds and render-pass
// placement stay with the native UI/scene providers.
class MapCameraSession {
public:
    bool open(const GameObjectSample& player, DrawableTargetSize drawable,
              float default_target_distance=0.0f) noexcept;
    bool resize(DrawableTargetSize drawable) noexcept;
    bool update(FrameInput input, FrameOutput* output) noexcept;
    bool pan(std::int32_t delta_x, std::int32_t delta_y, bool source_touch_pan_enabled) noexcept;
    bool pinch(float previous_distance, float current_distance) noexcept;
    // Source MenuCharMenu_Map::ResetZoom resets only CameraLevel current/target
    // zoom; it leaves the map pan offset and gameplay camera untouched.
    bool reset_zoom() noexcept;
    void close() noexcept;

    bool active() const noexcept { return active_; }
    std::uintptr_t target_identity() const noexcept { return target_identity_; }
    Owner& camera_owner() noexcept { return camera_; }
    const Owner& camera_owner() const noexcept { return camera_; }
    Vec3 pan_offset() const noexcept { return pan_offset_; }

private:
    Owner camera_{};
    ZoomInput zoom_input_{};
    std::uintptr_t target_identity_=0;
    Vec3 pan_offset_{};
    bool active_=false;
};

} // namespace dh2::camera_level_runtime_v1
