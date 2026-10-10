#pragma once
#include "renderer.hpp"
#include "original_scene.hpp"
#include "original_pose_blend.hpp"
#include "embedded_scene_clips.hpp"
#include <memory>
#include <string>
#include <utility>

namespace dh2::scene { struct Scene; }

namespace dh::foundation {
class AssetCatalog;
class AnimationMarkers;
enum class CharacterPose { idle, walk, attack };
// One independent history per original animation/applicator slot. Caller resets
// this value on same-clip NewAnim/replay; a changed named clip baselines itself.
struct RootMotionHistory {
    std::string clip_name;
    Vec3 previous;
    double source_seconds=0,cycles=0;
    std::uint32_t timestamp=0;
    bool initialized=false,loop=false;
};
struct CharacterVisualConfig {
    std::string model_path;
    std::string template_clip_path;
    std::array<std::string,3> animation_paths;
    // Named bank takes precedence over the legacy preview convenience array.
    std::vector<std::pair<std::string,std::string>> clips;
    // Empty selector includes all controllers; authored presets can select equipment.
    std::string skin_id_contains;
    unsigned expected_controller_count = 0;
    // Mixed skeletal assets may explicitly retain serialized rigid attachments.
    bool include_static_instances = false;
    // Original optimized skeletons can omit unused authored targets. Explicit opt-in
    // keeps strict standalone validation while retaining original missing-node policy.
    bool allow_missing_animation_targets = false;
    // Authored local translation target; supplied by character definition data.
    // "auto" follows original GetAnimRoot(false); empty disables extraction
    // unless consumption is enabled, which also selects the source auto policy.
    std::string motion_node_id;
    // Optional ordered authored scene names. Used only when no explicit ID/name
    // is supplied; caller can pass the source game's root-selection policy.
    std::vector<std::string> motion_node_candidates;
    bool consume_root_motion = false;
    std::vector<std::string> controller_ids;
    bool use_authored_modular_defaults=false;
};

// Standalone original-asset preview, independent of Android and combat callbacks.
// Authored model coordinates are retained. The host supplies placement/axis conversion.
class CharacterVisual {
public:
    CharacterVisual();
    ~CharacterVisual();
    CharacterVisual(CharacterVisual&&) noexcept;
    CharacterVisual& operator=(CharacterVisual&&) noexcept;
    CharacterVisual(const CharacterVisual&) = delete;
    CharacterVisual& operator=(const CharacterVisual&) = delete;
    bool load(const AssetCatalog&, const CharacterVisualConfig&, std::string& error);
    // Rigid/skinned game objects with named clips inside the same scene BDAE.
    // Reuses the existing meshes, materials, pose sampler and event machinery.
    bool load_embedded_scene(const AssetCatalog&,const std::string& model_path,std::string& error);
    void select(CharacterPose pose);
    bool select(const std::string& name, bool loop, std::string& error);
    // Explicit action entry, including replay of the same one-shot clip.
    bool restart(const std::string& name, bool loop, std::string& error);
    // B061: swap the drawn modular controllers (equipped parts) on the same Scene/skeleton.
    bool reselect_controllers(const AssetCatalog&,const std::vector<std::string>& controller_ids,std::string& error);
    bool update(double seconds, std::string& error);
    bool loaded() const;
    // Borrow the SAME retained scene; no second pose/clock. Invalidated by
    // load, move or destruction of this visual. Callers do not own this scene.
    const dh2::scene::Scene* retained_scene_borrow() const noexcept;
    const CharacterVisualConfig* configuration() const noexcept;
    const char* animation_name() const;
    const std::vector<Mesh>& meshes() const;
    std::vector<Mesh>& mutable_meshes();
    const std::vector<std::string>& texture_uris() const;
    const std::vector<OriginalMaterial>& original_materials() const;
    unsigned unbound_animation_targets() const;
    // Owned authored names/times; pointer remains valid until reload or destruction.
    // Action consumers own their MarkerCursor and attach meanings to these names.
    const AnimationMarkers* markers(const std::string& name, std::string& error) const;
    bool animation_range(const std::string& name, std::int32_t& start_ms,
                         std::int32_t& end_ms, std::string& error) const;
    double animation_elapsed_seconds() const;
    // Authored skeleton/model world, before the host actor placement transform.
    bool bone_world(const std::string& authored_name, Mat4& output, std::string& error) const;
    // Source getSceneNodeFromName: NAME target_node, case-insensitive, first
    // authored depth-first match. Known absent returns token0; unloaded fails.
    // Token is readonly, borrowed until reload/destruction; never dereference it.
    bool source_target_node(std::uintptr_t& token, std::string& error) const;
    // Validates token against this current graph. Compensated authored model
    // world only; host placement must be applied by the actual runtime owner.
    bool source_target_node_world(std::uintptr_t token, Mat4& output, std::string& error) const;
    bool socket_world(const std::string& authored_name, const Mat4& local_offset,
                      Mat4& output, std::string& error) const;
    // Per-update authored local delta (XYZ); original movement consumes XY and
    // supplies scale, facing and collision externally. Consumption clears it once.
    Vec3 root_motion_delta() const;
    Vec3 take_root_motion();
    const char* root_motion_node_id() const;
    Vec3 root_motion_rest_origin() const;
    // Original GetAnimRoot(false) prefers camera root; true omits that candidate.
    // Return value is an authored scene NAME, not a made-up skeletal target.
    const char* source_motion_root_name(bool character_only = false) const;
    // Actual drawn, indexed geometry in current compensated model pose. This is
    // render extent; original padded/scaled physical-owner bounds are separate.
    bool indexed_bounds(Vec3& minimum, Vec3& maximum, std::string& error) const;
    // Independent authored-local samples for original two-slot pose blending.
    // Neither sampling nor applying a pose advances live clip/motion clocks.
    bool sample_local_pose(const std::string& name, std::int32_t source_ms,
                           SkeletalPose& output, std::string& error) const;
    bool apply_local_pose(const SkeletalPose&, std::string& error);
    bool current_local_pose(SkeletalPose& output, std::string& error) const;
    bool root_translation_from_pose(const SkeletalPose&, Vec3&, std::string& error) const;
    bool sample_root_translation(const std::string& name, std::int32_t source_ms,
                                 Vec3&, std::string& error) const;
    // Exact original applicator target: last full-vector position binding on
    // selected root. Default-less records leave caller scratch unchanged.
    // Rejects an absent target rather than inventing pose-rest displacement.
    bool sample_source_root_translation(const std::string& name, std::int32_t source_ms,
                                        Vec3& scratch, std::string& error) const;
    // Cumulative source seconds since authored clip start, before actor scaling.
    // Same timestamp produces zero delta but still refreshes previous coordinates,
    // matching original CalculateDelta. Does not touch the live visual or clock.
    bool step_root_motion(const std::string& name, double source_seconds, bool loop,
                          std::uint32_t timestamp, RootMotionHistory&,
                          Vec3& delta, std::string& error) const;
private:
    bool load_with_ranges(const AssetCatalog&,const CharacterVisualConfig&,
                          const std::vector<EmbeddedSceneClip>&,std::string& error,bool object_scene=false);
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
}
