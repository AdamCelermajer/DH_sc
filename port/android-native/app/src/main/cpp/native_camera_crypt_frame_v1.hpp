#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string_view>

#include "projection_matrix.hpp"

namespace dh2::native::crypt_camera_frame_v1 {

using Vec3 = std::array<float, 3>;
using Matrix = std::array<float, 16>;

// Source player-camera frames are bounded to Crypt and SWAMP. IDA confirms
// Level::_LoadCamera (0x3f1008) calls CameraBase::SetData (0x40e9a8) with the
// same FOV immediate 0x3edbf877 (0.429630011 rad) on both routes. Near/far
// remain level-specific camera_znear/camera_zfar values from LevelConfig.
// Crypt's 007_crypt_01.rule.xml values are 900/5000 from +0x294/+0x298.
// Those runtime SetData values replace the playercamera.bdae Camera record
// (45, 1.5, 600, 3800); do not feed the BRES values into the gameplay frame.
// The old runtime also supplies aspect 0x3fd578e9 (1.6677524), but this port
// deliberately uses the current surface aspect in build() for modern displays.
// Authored player-camera rig transforms replace only the fallback eye offset;
// the CameraLevel target/animation timeline remains a separate provider.
inline constexpr std::string_view kLevelName = "GOTHICUS_CRYPT_01";
inline constexpr std::string_view kLevelFile = "007_crypt_01.rule.xml";
inline constexpr Vec3 kEyeFromTarget{1380.0f, -1380.0f, 2450.0f};
inline constexpr float kVerticalFovRadians = 0.42963001132011414f;
inline constexpr float kNearPlane = 900.0f;
inline constexpr float kFarPlane = 5000.0f;
inline constexpr float kInputYawRadians = -0.7853981633974483f;
// Character::InitCam constructs AnchorForward from the three
// CharacterDesign/ForwardCamera_* constants. AnchorForward::Update adds the
// Distance_PerSec member directly on each update (the observed function does
// not multiply this term by dt), caps the look-ahead at Max_Distance while
// moving, and caps the retained distance at 40% while idle.
inline constexpr float kForwardAnchorMaxDistance = 640.0f;
inline constexpr float kForwardAnchorDistancePerFrame = 17.0f;
inline constexpr float kForwardAnchorIdleCap = kForwardAnchorMaxDistance * 0.4f;
inline constexpr float kForwardAnchorTurnThresholdRadians = 1.0f;
inline constexpr float kForwardAnchorTurnRetreat = kForwardAnchorDistancePerFrame * 0.25f;
inline constexpr float kForwardAnchorTranslationSquaredThreshold = 0.05f;
inline float input_pitch_radians() noexcept {
    const float horizontal = std::hypot(kEyeFromTarget[0], kEyeFromTarget[1]);
    return std::atan2(kEyeFromTarget[2], horizontal);
}

inline bool verified_crypt_route(std::string_view level_name,
                                 std::string_view level_file) noexcept {
    return level_name == kLevelName && level_file == kLevelFile;
}

// Other authored static MLX levels carry the same LevelConfig camera shape
// (empty camera_file -> CameraTests.bdae); the renderer registers its static
// level table here so they take the same source camera route as SWAMP.
inline bool (*static_mlx_camera_route)(std::string_view, std::string_view) = nullptr;

inline bool verified_swamp_default_camera_route(std::string_view level_name,
                                                std::string_view level_file) noexcept {
    return (level_name == "SWAMP" && level_file == "001_swamp.mlx") ||
           (static_mlx_camera_route && static_mlx_camera_route(level_name, level_file));
}

// CameraTests supplies optional authored rig animation for SWAMP. When it is
// absent, the renderer still has verified static projection and eye-offset
// data and can keep gameplay alive on that bounded fallback route.
inline bool source_swamp_static_camera_fallback_ready(
        std::string_view level_name, std::string_view level_file,
        bool level_projection_ready) noexcept {
    return verified_swamp_default_camera_route(level_name, level_file) &&
           level_projection_ready;
}

// Exact static camera-input routes whose scene and runtime LevelConfig
// projection are both source-backed. Dynamic CameraLevel ownership remains a
// separate provider; this only gates the authored idle rig used for framing.
inline bool source_camera_rig_route_ready(std::string_view level_name,
                                          std::string_view level_file,
                                          bool level_projection_ready,
                                          bool authored_rig_ready) noexcept {
    return (verified_crypt_route(level_name, level_file) ||
            verified_swamp_default_camera_route(level_name, level_file)) &&
           level_projection_ready && authored_rig_ready;
}

// Both routes require their exact LevelConfig clip planes and the authored
// LevelConfig camera resource plus selected CamAnimSet idle. An empty
// camera_file resolves to CameraTests.bdae and an empty camera_animset to
// Default, as supplied by LevelConfig::InitPost/DeclareProperties.
inline bool source_player_camera_route_ready(std::string_view level_name,
                                             std::string_view level_file,
                                             bool level_projection_ready,
                                             bool authored_rig_ready) noexcept {
    return source_camera_rig_route_ready(level_name, level_file,
                                         level_projection_ready, authored_rig_ready);
}

struct Frame {
    Vec3 eye{};
    Vec3 target{};
    Matrix view_projection{};
    float aspect{};
    float input_yaw{};
    float input_pitch{};
};

struct ForwardAnchorState {
    float distance{};
    float previous_heading{};
    bool has_previous_heading{};
    Vec3 previous_actor_look_at{};
    bool has_previous_actor_look_at{};
    Vec3 previous_actor_position{};
    Vec3 target{};
    bool has_target{};
    // Native semantic equivalent of GameObject::SetCameraAnchorObject's
    // owned +0x2e0 pointer. The anchor remains part of this existing camera
    // state, but is tied to the canonical Character identity and can be
    // retired only by that same owner.
    std::uintptr_t character_identity{};
    bool source_initialized{};
    bool use_static_camera{};
    float source_max_distance{kForwardAnchorMaxDistance};
    float source_distance_per_frame{kForwardAnchorDistancePerFrame};
    float source_threshold{};
};

// Character::InitCam (0x3b4bc4) constructs AnchorBase when UseStaticCamera
// is set, otherwise AnchorForward(character, maxDistance, distancePerSec,
// threshold * 0.01). SetCameraAnchorObject (0x394378) transfers ownership to
// the Character's +0x2e0 field and destroys any previous anchor. `identity`
// is the port's stable semantic Character token, never an original pointer.
inline bool initialize_character_camera_anchor(
        ForwardAnchorState* state, std::uintptr_t identity, Vec3 character_position,
        bool use_static_camera, float max_distance, float distance_per_frame,
        float source_threshold) noexcept {
    if (!state || identity == 0) return false;
    for (float component : character_position)
        if (!std::isfinite(component)) return false;
    if (!use_static_camera &&
        (!std::isfinite(max_distance) || max_distance < 0.0f ||
         !std::isfinite(distance_per_frame) || distance_per_frame < 0.0f ||
         !std::isfinite(source_threshold) || source_threshold < 0.0f ||
         source_threshold > 100.0f)) return false;

    ForwardAnchorState next{};
    next.character_identity = identity;
    next.source_initialized = true;
    next.use_static_camera = use_static_camera;
    next.source_max_distance = use_static_camera ? kForwardAnchorMaxDistance : max_distance;
    next.source_distance_per_frame = use_static_camera
        ? kForwardAnchorDistancePerFrame : distance_per_frame;
    next.source_threshold = use_static_camera ? 0.0f : source_threshold * 0.01f;
    next.target = character_position; // AnchorBase::Reset copies GameObject position.
    next.has_target = true;
    *state = next;
    return true;
}

inline bool retire_character_camera_anchor(ForwardAnchorState* state,
                                           std::uintptr_t identity) noexcept {
    if (!state || identity == 0 || !state->source_initialized ||
        state->character_identity != identity) return false;
    *state = ForwardAnchorState{};
    return true;
}

// Bounded Character::InitCam/AnchorForward projection. The original anchor
// updates before CameraLevel consumes CameraTarget each frame. Its exact
// CameraLevel animation, multiplayer branch and damped target transition are
// separate. The angle-only overload below is a compatibility approximation;
// the vector overload follows the source Point3D angle inputs.
inline bool update_forward_anchor(ForwardAnchorState* state, float heading,
                                  bool moving_or_attacking,
                                  bool player_displaced_enough) noexcept {
    if (!state || !std::isfinite(heading) || !std::isfinite(state->distance) ||
        state->distance < 0.0f || state->distance > kForwardAnchorMaxDistance) return false;
    if (moving_or_attacking) {
        float turn = 0.0f;
        if (state->has_previous_heading) {
            const float delta = std::remainder(heading - state->previous_heading,
                                               6.2831853071795864769f);
            turn = std::fabs(delta);
        }
        if (turn > kForwardAnchorTurnThresholdRadians) {
            state->distance = std::max(0.0f, state->distance - kForwardAnchorTurnRetreat);
        } else {
            const float limit = player_displaced_enough
                ? kForwardAnchorMaxDistance : kForwardAnchorMaxDistance * 0.5f;
            if (state->distance < kForwardAnchorMaxDistance * 0.5f) {
                state->distance = kForwardAnchorMaxDistance * 0.5f;
            } else {
                state->distance = std::min(limit,
                    state->distance + kForwardAnchorDistancePerFrame);
            }
        }
    } else {
        state->distance = std::min(state->distance, kForwardAnchorIdleCap);
    }
    state->previous_heading = heading;
    state->has_previous_heading = true;
    return true;
}

inline bool point3d_angle(Vec3 a, Vec3 b, float* radians) noexcept {
    if (!radians) return false;
    for (float component : a) if (!std::isfinite(component)) return false;
    for (float component : b) if (!std::isfinite(component)) return false;
    const float a_length = std::hypot(std::hypot(a[0], a[1]), a[2]);
    const float b_length = std::hypot(std::hypot(b[0], b[1]), b[2]);
    if (!(a_length > 0.0f) || !(b_length > 0.0f) ||
        !std::isfinite(a_length) || !std::isfinite(b_length)) return false;
    const float cosine = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) /
                         (a_length * b_length);
    if (!std::isfinite(cosine)) return false;
    *radians = std::acos(std::clamp(cosine, -1.0f, 1.0f));
    return std::isfinite(*radians);
}

// Source-backed AnchorForward::Update path. It compares the current normalized
// actor heading (+0x1B8) to the look-at vector saved by the prior update, then
// refreshes that saved vector with this update's GetLookAtVec after projecting
// the target. This differs from comparing two consecutive actor headings.
inline float source_point3d_angle(Vec3 a, Vec3 b) noexcept {
    const float dot = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    const float a_length = std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    const float b_length = std::sqrt(b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);
    // Point3D::angleCos divides without a zero-length guard. Preserve the
    // resulting NaN comparison behavior (NaN > 1 is false) for AnchorForward's
    // zero-initialized previous look-at vector on its first active frame.
    return std::acos(dot / (a_length * b_length));
}

// GameObject::SetHeadingDirection writes +0x1b5 from x*x+y*y > 0.0001,
// after setting the stored heading Z component to zero (IDA 0x393c48–0x393c60).
// That Character byte, rather than the controller's active bit, gates the
// AnchorForward moving/attacking branch.
inline bool source_game_object_heading_set(Vec3 heading) noexcept {
    return std::isfinite(heading[0]) && std::isfinite(heading[1]) &&
           heading[0] * heading[0] + heading[1] * heading[1] > 0.0001f;
}

inline bool update_forward_anchor(ForwardAnchorState* state,
                                  Vec3 actor_position,
                                  Vec3 current_heading_direction,
                                  Vec3 current_actor_look_at,
                                  bool,
                                  bool moving,
                                  bool attacking) noexcept {
    if (!state) return false;
    const float max_distance = state->source_initialized
        ? state->source_max_distance : kForwardAnchorMaxDistance;
    const float distance_per_frame = state->source_initialized
        ? state->source_distance_per_frame : kForwardAnchorDistancePerFrame;
    if (!std::isfinite(max_distance) || max_distance < 0.0f ||
        !std::isfinite(distance_per_frame) || distance_per_frame < 0.0f ||
        !std::isfinite(state->distance) || state->distance < 0.0f ||
        state->distance > max_distance) return false;
    for (float component : actor_position)
        if (!std::isfinite(component)) return false;
    for (float component : current_heading_direction)
        if (!std::isfinite(component)) return false;
    for (float component : current_actor_look_at)
        if (!std::isfinite(component)) return false;

    if (!state->has_target) {
        // AnchorBase::Reset seeds its prior target from GameObject position;
        // the first live call is the first point at which that owner is known.
        state->target = actor_position;
        state->has_target = true;
    }

    if (state->source_initialized && state->use_static_camera) {
        state->distance = 0.0f;
        state->target = actor_position;
        return true;
    } else if (source_game_object_heading_set(current_heading_direction) &&
               (moving || attacking)) {
        const float heading_length = std::hypot(
            std::hypot(current_heading_direction[0], current_heading_direction[1]),
            current_heading_direction[2]);
        if (!(heading_length > 0.0f) || !std::isfinite(heading_length)) return false;
        const Vec3 direction{current_heading_direction[0] / heading_length,
                             current_heading_direction[1] / heading_length,
                             current_heading_direction[2] / heading_length};
        const float turn = source_point3d_angle(current_heading_direction,
                                                 state->previous_actor_look_at);
        const float dx = actor_position[0] - state->previous_actor_position[0];
        const float dy = actor_position[1] - state->previous_actor_position[1];
        const float dz = actor_position[2] - state->previous_actor_position[2];
        const float target_limit = moving && dx * dx + dy * dy + dz * dz >
                kForwardAnchorTranslationSquaredThreshold
            ? max_distance : max_distance * 0.5f;

        if (turn > kForwardAnchorTurnThresholdRadians) {
            state->distance = std::max(0.0f,
                state->distance - distance_per_frame * 0.25f);
        } else if (state->distance < max_distance * 0.5f) {
            state->distance = max_distance * 0.5f;
        } else {
            state->distance = std::min(target_limit,
                state->distance + distance_per_frame);
        }

        const Vec3 full_target{actor_position[0] + direction[0] * target_limit,
                               actor_position[1] + direction[1] * target_limit,
                               actor_position[2] + direction[2] * target_limit};
        const float full_dx = full_target[0] - state->target[0];
        const float full_dy = full_target[1] - state->target[1];
        const float full_dz = full_target[2] - state->target[2];
        if (full_dx * full_dx + full_dy * full_dy + full_dz * full_dz <= 0.0f ||
            target_limit - state->distance <= 0.05f) {
            state->target = full_target;
        } else {
            state->target = {actor_position[0] + direction[0] * state->distance,
                             actor_position[1] + direction[1] * state->distance,
                             actor_position[2] + direction[2] * state->distance};
        }
    } else {
        state->distance = std::min(state->distance, max_distance * 0.4f);
        state->target = {
            actor_position[0] + state->distance * state->previous_actor_look_at[0],
            actor_position[1] + state->distance * state->previous_actor_look_at[1],
            actor_position[2] + state->distance * state->previous_actor_look_at[2]};
    }

    // AnchorForward::Update compares against +0x2c..+0x34 at entry and writes
    // the current GameObject position back to those fields at its common exit
    // (IDA 0x4773fc, 0x477760..0x47777c). This uses a per-frame
    // squared-displacement threshold (0.05 world-units squared),
    // not a cumulative distance; do not accumulate small frames.
    state->previous_actor_position = actor_position;
    state->previous_actor_look_at = current_actor_look_at;
    state->has_previous_actor_look_at = true;
    return true;
}

inline bool update_character_camera_anchor(
        ForwardAnchorState* state, std::uintptr_t identity,
        Vec3 actor_position, Vec3 current_heading_direction,
        Vec3 current_actor_look_at, bool,
        bool moving, bool attacking) noexcept {
    if (!state || !state->source_initialized || identity == 0 ||
        state->character_identity != identity) return false;
    if (state->use_static_camera) {
        for (float component : actor_position)
            if (!std::isfinite(component)) return false;
        state->target = actor_position;
        state->distance = 0.0f;
        state->has_target = true;
        return true;
    }
    return update_forward_anchor(state, actor_position, current_heading_direction,
                                 current_actor_look_at, false,
                                 moving, attacking);
}

inline Vec3 forward_anchor_target(const ForwardAnchorState* state,
                                  Vec3 actor_position) noexcept {
    if (!state || !state->has_target) return actor_position;
    return state->target;
}

// Horizontal compatibility helper. The live AnchorForward path stores its
// source target in ForwardAnchorState and uses forward_anchor_target().
inline Vec3 player_camera_anchor_from_direction(Vec3 actor_position,
                                                 Vec3 heading_direction,
                                                 float distance) noexcept {
    if (!std::isfinite(heading_direction[0]) ||
        !std::isfinite(heading_direction[1]) || !std::isfinite(distance))
        return actor_position;
    const float length = std::hypot(heading_direction[0], heading_direction[1]);
    if (!(length > 0.0f) || !std::isfinite(length)) return actor_position;
    const float direction_x = heading_direction[0] / length;
    const float direction_y = heading_direction[1] / length;
    return {actor_position[0] + direction_x * distance,
            actor_position[1] + direction_y * distance,
            actor_position[2]};
}

// Compatibility helper for callers that only have GameObject rotation:
// GetLookAtVec writes (sin(rotation), -cos(rotation), 0). The live game
// forward-anchor callsite should use the recovered actor heading vector above.
inline Vec3 player_camera_anchor(Vec3 actor_position, float heading,
                                 float distance) noexcept {
    if (!std::isfinite(heading)) return actor_position;
    return player_camera_anchor_from_direction(
        actor_position, {std::sin(heading), -std::cos(heading), 0.0f}, distance);
}

inline Vec3 add(Vec3 a, Vec3 b) noexcept {
    return {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
}
inline Vec3 subtract(Vec3 a, Vec3 b) noexcept {
    return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}
inline float dot(Vec3 a, Vec3 b) noexcept {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
// AnchorForward::Update compares the full previous/current GameObject position
// delta against 0.05 (squared units), not just its ground-plane projection.
inline bool player_displaced_enough_for_forward_anchor(Vec3 delta) noexcept {
    for (float component : delta) if (!std::isfinite(component)) return false;
    return dot(delta, delta) > kForwardAnchorTranslationSquaredThreshold;
}
inline Vec3 cross(Vec3 a, Vec3 b) noexcept {
    return {a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]};
}
inline bool normalize(Vec3& v) noexcept {
    const float n2 = dot(v, v);
    if (!(n2 > 0.0f) || !std::isfinite(n2)) return false;
    const float inverse = 1.0f / std::sqrt(n2);
    for (float& component : v) component *= inverse;
    return std::isfinite(v[0]) && std::isfinite(v[1]) && std::isfinite(v[2]);
}
// CameraLevel writes zoom to the target-camera node's local position Z. Apply
// that local translation through the authored node basis; the camera eye stays
// at its authored position. The basis is relative to Root_Camera-node and is
// supplied from the target node's world matrix column 2.
inline bool apply_target_camera_local_z(Vec3 target_from_root,
                                        Vec3 target_local_z_axis,
                                        float local_z,
                                        Vec3* out_target_from_root) noexcept {
    if (!out_target_from_root || !std::isfinite(local_z)) return false;
    for (float value : target_from_root) if (!std::isfinite(value)) return false;
    for (float value : target_local_z_axis) if (!std::isfinite(value)) return false;
    Vec3 result{};
    for (std::size_t i = 0; i < result.size(); ++i) {
        result[i] = target_from_root[i] + target_local_z_axis[i] * local_z;
        if (!std::isfinite(result[i])) return false;
    }
    *out_target_from_root = result;
    return true;
}
inline Matrix multiply(const Matrix& a, const Matrix& b) noexcept {
    Matrix result{};
    for (int column = 0; column < 4; ++column)
        for (int row = 0; row < 4; ++row)
            for (int k = 0; k < 4; ++k)
                result[column * 4 + row] += a[k * 4 + row] * b[column * 4 + k];
    return result;
}

// Build from the shared authored player-camera rig and a level's runtime
// projection. Level::_LoadCamera supplies the FOV; LevelConfig supplies the
// clip planes. Aspect follows the current surface as the modern display policy.
inline bool build_player_frame(Vec3 target, int width, int height, Frame* out,
                               float vertical_fov_radians, float near_plane,
                               float far_plane,
                               Vec3 eye_from_target,
                               Vec3 up_direction,
                               Vec3 target_from_anchor = Vec3{0.0f, 0.0f, 0.0f},
                               float camera_local_z_offset = 0.0f,
                               Vec3 target_camera_local_z_axis = Vec3{0.0f,0.0f,1.0f}) noexcept {
    if (!out || width <= 1 || height <= 1 ||
        !std::isfinite(vertical_fov_radians) ||
        !(vertical_fov_radians > 0.0f && vertical_fov_radians < 3.14159265358979323846f) ||
        !std::isfinite(near_plane) || !std::isfinite(far_plane) ||
        !(near_plane > 0.0f && far_plane > near_plane))
        return false;
    for (float value : target) if (!std::isfinite(value)) return false;
    for (float value : eye_from_target) if (!std::isfinite(value)) return false;
    for (float value : up_direction) if (!std::isfinite(value)) return false;
    for (float value : target_from_anchor) if (!std::isfinite(value)) return false;
    for (float value : target_camera_local_z_axis) if (!std::isfinite(value)) return false;

    Frame frame{};
    const Vec3 unzoomed_target = add(target,target_from_anchor);
    frame.target = unzoomed_target;
    if (!apply_target_camera_local_z(frame.target,target_camera_local_z_axis,
                                     camera_local_z_offset,&frame.target)) return false;
    frame.eye = add(unzoomed_target,eye_from_target);
    frame.aspect = static_cast<float>(width) / static_cast<float>(height);
    const Vec3 eye_from_adjusted_target = subtract(frame.eye,frame.target);
    frame.input_yaw = std::atan2(eye_from_adjusted_target[1],eye_from_adjusted_target[0]);
    frame.input_pitch = std::atan2(eye_from_adjusted_target[2],
        std::hypot(eye_from_adjusted_target[0],eye_from_adjusted_target[1]));

    Vec3 forward = subtract(frame.target, frame.eye);
    if (!normalize(forward)) return false;
    // Keep the recovered positive-forward side axis (forward × up), then adapt
    // its vertical axis for this GLES projection: positive clip Y must follow
    // the authored up direction. The engine's positive-forward matrix stores
    // the opposite vertical sign, so use side × forward at this GL boundary.
    if (!normalize(up_direction)) return false;
    Vec3 side = cross(forward, up_direction);
    if (!normalize(side)) return false;
    const Vec3 vertical = cross(side, forward);
    const Matrix view{
        side[0], vertical[0], forward[0], 0.0f,
        side[1], vertical[1], forward[1], 0.0f,
        side[2], vertical[2], forward[2], 0.0f,
        -dot(side, frame.eye), -dot(vertical, frame.eye), -dot(forward, frame.eye), 1.0f};

    // Use the recovered Irrlicht LH projection producer and the original
    // GLES driver's z-[0,1] to z-[-1,1] fix-up. This retains the source
    // call order at the live frame boundary.
    using ProjectionStatus = dh2::engine_camera::projection_matrix::Status;
    using SourceMatrix = dh2::engine_camera::camera_math::Matrix4;
    std::uint32_t fov_word = 0, aspect_word = 0, near_word = 0, far_word = 0;
    std::memcpy(&fov_word, &vertical_fov_radians, sizeof(fov_word));
    std::memcpy(&aspect_word, &frame.aspect, sizeof(aspect_word));
    std::memcpy(&near_word, &near_plane, sizeof(near_word));
    std::memcpy(&far_word, &far_plane, sizeof(far_word));
    SourceMatrix source_projection{};
    if (dh2::engine_camera::projection_matrix::perspective_fov(
            fov_word, aspect_word, near_word, far_word, &source_projection) !=
            ProjectionStatus::complete ||
        dh2::engine_camera::projection_matrix::fix_up_for_gles_default_orientation(
            &source_projection) != ProjectionStatus::complete)
        return false;
    Matrix projection{};
    for (std::size_t i = 0; i < projection.size(); ++i)
        std::memcpy(&projection[i], &source_projection.elements[i], sizeof(float));
    frame.view_projection = multiply(projection, view);
    for (float value : frame.view_projection) if (!std::isfinite(value)) return false;
    *out = frame;
    return true;
}

// Crypt's verified route wrapper retains the original source-specific
// projection receipt for callers which do not have LevelConfig available.
inline bool build(std::string_view level_name, std::string_view level_file,
                  Vec3 target, int width, int height, Frame* out,
                  Vec3 eye_from_target = kEyeFromTarget,
                  Vec3 up_direction = Vec3{0.0f, 0.0f, 1.0f},
                  Vec3 target_from_anchor = Vec3{0.0f, 0.0f, 0.0f}) noexcept {
    if (!verified_crypt_route(level_name, level_file)) return false;
    return build_player_frame(target, width, height, out, kVerticalFovRadians,
                              kNearPlane, kFarPlane, eye_from_target,
                              up_direction, target_from_anchor);
}

} // namespace dh2::native::crypt_camera_frame_v1
