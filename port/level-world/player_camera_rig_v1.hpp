#pragma once

#include "../engine-animation/animation.hpp"
#include "../scene-materials/scene.hpp"
#include "visual_timeline.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace dh2::player_camera_rig_v1 {

struct Assets {
    const std::uint8_t* camera_scene=nullptr;
    std::size_t camera_scene_size=0;
    const std::uint8_t* idle_animation=nullptr;
    std::size_t idle_animation_size=0;
    // Most source camera rigs name Root_Camera-node and upvector-node. The
    // minimap rig instead names its root Player_minimap-node and encodes up
    // in the camera-node basis, so callers may supply that exact variant.
    const char* root_node=nullptr;
    const char* up_vector_node=nullptr;
    bool derive_up_from_camera=false;
    // Optional LevelConfig::camera_name. When supplied, select the exact
    // authored scene node and require its Camera instance; null preserves
    // legacy unique-camera selection. Pose matrices remain relative to root_node.
    const char* camera_name=nullptr;
};

// Raw values authored in the BRES Camera record. source_fov_value is retained
// without guessing whether the caller's projection API expects degrees or
// radians; aspect and clip distances are source fields as well.
struct Projection {
    float source_fov_value=0.0f;
    float aspect_ratio=0.0f;
    float near_clip=0.0f;
    float far_clip=0.0f;
};

struct Pose {
    // All matrices are relative to Root_Camera-node, in column-major order.
    std::array<float,16> camera{};
    std::array<float,16> target{};
    std::array<float,16> up_vector{};
    // The sampled camera node's authored scene-world transform before the
    // root-relative conversion above. CameraBase::GetCameraLookAtVec/UpVec
    // read its basis columns [4..6] and [8..10] from the active node matrix.
    // CameraLevel updates only node positions, so these basis values remain
    // valid for listener orientation after follow/zoom updates.
    std::array<float,16> camera_world{};
    // CameraLevel::Update writes a local position to the target node. A local
    // translation is transformed by the node's parent, not its own rotation.
    // Keep that parent-space Z basis explicit for the native zoom projection.
    std::array<float,3> target_parent_z_axis{0.0f,0.0f,1.0f};
};

// Loads a camera-only visual graph plus its authored idle animation. Sampling
// has no clock or loop policy: the caller supplies an absolute clip time.
class Rig {
public:
    bool load(const Assets&,std::string& error);
    // Replace the active source clip on the already-loaded Camera scene graph.
    // The one Playback owner must be restarted after a successful replacement.
    bool load_animation(const std::uint8_t*,std::size_t,std::string& error);
    bool sample(std::int32_t milliseconds,Pose*,std::string& error);

    const Projection& projection() const noexcept { return projection_; }
    // CameraLevel::CalculateDefaultTargetDistance runs during Load, before
    // PlayAnim. It measures target-node absolute position from its parent,
    // rather than the animated camera-to-target distance.
    float source_default_target_distance() const noexcept { return default_target_distance_; }
    std::int32_t animation_start() const noexcept { return animation_.start; }
    std::int32_t animation_end() const noexcept { return animation_.end; }
    unsigned track_count() const noexcept { return animation_.track_count(); }
    unsigned skipped_tracks() const noexcept { return animation_.skipped; }
    unsigned unbound_tracks() const noexcept { return animation_.unbound; }

private:
    scene::Scene scene_;
    animation::Player animation_;
    Projection projection_{};
    std::size_t root_=0,camera_=0,target_=0,up_vector_=0;
    bool up_from_camera_=false;
    float default_target_distance_=0.0f;
    bool loaded_=false;
};

// Runtime for CameraLevel's currently selected clip. CameraLevel::PlayAnim
// selects each clip with loop=false and speed=1; start() restarts this single
// timeline at the active clip's authored range. The caller advances it from
// game-frame deltas rather than a wall-clock epoch.
class Playback {
public:
    bool start(const Rig&,std::string& error);
    bool advance(Rig&,std::uint32_t dt_ms,Pose*,std::string& error);
    std::int32_t current_time_ms() const noexcept { return timeline_.current_ms; }
    bool completed() const noexcept { return timeline_.ended!=0; }

private:
    timeline::State timeline_{};
    std::uint32_t source_clock_ms_=0;
    bool started_=false;
};

} // namespace dh2::player_camera_rig_v1
