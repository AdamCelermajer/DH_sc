#pragma once
// P16 CINE2: scripted camera clips (PlayCamera, kind 5), CameraLevel::PlayAnim path.
//
// IDA evidence (libDungeonHunter2.so/pseudocode-all.c):
// - Script_PlayCamera::Execute (0x460110): clip id = command scalar 8; CameraLevel::PlayAnim(level->camera, id, 0, 0).
//   With the SKIP flag the level camera set's own clip is played instead (the follow camera returns).
// - CameraLevel::PlayAnim (0x40f904): starts the camera animator; playing flag (CameraLevel +132) = 1.
// - CameraLevel::__Callback (0x40f8f8): timeline completion clears the flag.
// - Script_PlayCamera::IsBlocking (0x459370): blocks while command byte +12 is set AND the playing flag is set.
// The dictionary id is an index into animations_dictionary (scene01 of cs_swamp_intro is id 359).
//
// The clip is the cs_* BRES scene (its own PlayerCamera_Default node and animation). Sampling drives that node
// with the same Player/scene code as the level camera (OriginalGameplayCamera). Only the eye and target are
// taken from the clip; up and FOV stay with the follow camera (the clip's xfov channel is not applied, see report).
#include "camera.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace dh::foundation {
class AssetCatalog;

// Resolves an animations_dictionary id to its clip path and bytes (dictionary parsed once), and reads the
// level camera scene the clip drives (the LevelConfig camera file; default CameraTests with node PlayerCamera_Default).
class CameraClipLibrary {
public:
    void set_scene_file(std::string file) { scene_file_ = std::move(file); }
    bool read(const AssetCatalog& assets, std::int32_t dictionary_id, std::vector<std::uint8_t>& clip,
              std::vector<std::uint8_t>& scene, std::string& path, std::string& error);
    // P16 OPENING: the BDAE path of an animations_dictionary id (PlayActorAnim clips use the same dictionary).
    bool dictionary_path(const AssetCatalog& assets, std::int32_t dictionary_id, std::string& path, std::string& error);
private:
    std::vector<std::string> paths_;
    std::string scene_file_ = "data/3D/camera/CameraTests.bdae";
    bool loaded_ = false;
};

class OriginalCameraClip {
public:
    OriginalCameraClip();
    ~OriginalCameraClip();
    OriginalCameraClip(OriginalCameraClip&&) noexcept;
    OriginalCameraClip& operator=(OriginalCameraClip&&) noexcept;

    // scene: the level camera scene (its PlayerCamera_Default node is driven). clip: the cs_* animation BRES,
    // whose channels bind to scene nodes by name (the clip itself has no camera graph). Replaces any previous clip.
    bool load(std::vector<std::uint8_t> scene, std::vector<std::uint8_t> clip, std::string& error);
    bool loaded() const noexcept;
    // Clip timeline in milliseconds (authored start..end). Duration is end - start.
    std::int32_t start_ms() const noexcept;
    std::int32_t end_ms() const noexcept;
    std::int32_t duration_ms() const noexcept { return end_ms() - start_ms(); }
    // Eye and target of the authored PlayerCamera node at elapsed_ms after the clip start (clamped to the end).
    bool sample(std::int32_t elapsed_ms, CameraVec3& eye, CameraVec3& target, std::string& error);

    // P16 OPENING4: the full view of the clip at elapsed_ms: eye and target as above, the up vector from the authored
    // 'upvector' node (its position relative to the camera node; the clip animates that node), and the field of view from
    // the clip's xfov channel (degrees; has_fov=false when the clip has no xfov channel, then the follow FOV stays).
    struct View {
        CameraVec3 eye{}, target{}, up{0.0f, 1.0f, 0.0f};
        float vertical_fov_degrees = 0.0f;
        bool has_up = false, has_fov = false;
    };
    bool sample_view(std::int32_t elapsed_ms, View& view, std::string& error);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace dh::foundation
