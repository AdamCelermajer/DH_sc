#include "original_camera_clip.hpp"
#include "asset_catalog.hpp"
#include "content_paths.hpp"
#include "../level-world/gameplay_camera_scene_v3.hpp"
#include "../engine-animation/animation.hpp"
#include "../game-data/data.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>

namespace dh::foundation {
namespace {
// Camera node of the level camera config (OriginalCameraConfig::node default).
constexpr const char* kCameraNode = "PlayerCamera_Default";

dh2::data::Bytes bytes_of(const std::vector<std::uint8_t>& v) { return {v.data(), v.size()}; }
bool finite(CameraVec3 v) { return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z); }
} // namespace

bool CameraClipLibrary::read(const AssetCatalog& assets, std::int32_t id, std::vector<std::uint8_t>& clip,
                             std::vector<std::uint8_t>& scene, std::string& path, std::string& error) {
    try {
        if (!loaded_) {
            const auto names = read_content(assets, "data/pydata/animations_dictionary_pyarraynames.bin");
            const auto values = read_content(assets, "data/pydata/animations_dictionary_pyarray.bin");
            dh2::data::Dictionary dictionary;
            if (!dh2::data::load_dictionary(bytes_of(names), bytes_of(values), dictionary, error)) return false;
            paths_ = std::move(dictionary.values);
            loaded_ = true;
        }
        if (id < 0 || std::size_t(id) >= paths_.size()) {
            error = "Camera clip dictionary id absent: " + std::to_string(id);
            return false;
        }
        path = paths_[std::size_t(id)];
        if (path.size() < 5 || path.compare(path.size() - 5, 5, ".bdae") != 0) {
            error = "Camera clip dictionary entry is not a BDAE scene: " + path;
            return false;
        }
        clip = read_content(assets, path);
        scene = read_content(assets, scene_file_);
        error.clear();
        return true;
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }
}

struct OriginalCameraClip::Impl {
    dh2::camera::GameplayCameraSceneV3 scene;
    dh2::animation::Player player;
    std::uint32_t selected = 0;
};

OriginalCameraClip::OriginalCameraClip() = default;
OriginalCameraClip::~OriginalCameraClip() = default;
OriginalCameraClip::OriginalCameraClip(OriginalCameraClip&&) noexcept = default;
OriginalCameraClip& OriginalCameraClip::operator=(OriginalCameraClip&&) noexcept = default;

bool OriginalCameraClip::load(std::vector<std::uint8_t> scene_bytes, std::vector<std::uint8_t> clip_bytes, std::string& error) {
    impl_.reset();
    auto next = std::make_unique<Impl>();
    // The Player keeps its own copy of the animation image; the scene retains the BRES bytes.
    if (!next->scene.load(std::move(scene_bytes), error)) return false;
    if (!next->scene.select(kCameraNode, next->selected, error)) return false;
    if (!next->player.load(clip_bytes.data(), clip_bytes.size(), next->scene.graph(), error,
                           dh2::animation::MissingTargets::ignore)) return false;
    if (next->player.end < next->player.start) {
        error = "Camera clip range is inverted";
        return false;
    }
    impl_ = std::move(next);
    error.clear();
    return true;
}

bool OriginalCameraClip::loaded() const noexcept { return impl_ != nullptr; }
std::int32_t OriginalCameraClip::start_ms() const noexcept { return impl_ ? impl_->player.start : 0; }
std::int32_t OriginalCameraClip::end_ms() const noexcept { return impl_ ? impl_->player.end : 0; }

bool OriginalCameraClip::sample(std::int32_t elapsed_ms, CameraVec3& eye, CameraVec3& target, std::string& error) {
    if (!loaded()) {
        error = "Camera clip is not loaded";
        return false;
    }
    auto& i = *impl_;
    const std::int64_t wanted = std::int64_t(i.player.start) + std::max<std::int32_t>(0, elapsed_ms);
    const auto time = std::int32_t(std::min<std::int64_t>(wanted, i.player.end));
    if (!i.player.sample(i.scene.graph(), time, error)) return false;
    // Zero instance offset, as the level camera does at zoom 0.
    const float child[3]{0, 0, 0};
    if (!i.scene.set_camera_instance_position(i.selected, child, error)) return false;
    if (!i.scene.update_selected_absolute_v67(i.selected, error)) return false;
    float e[3]{}, t[3]{};
    if (!i.scene.eye_and_target(i.selected, e, t, error)) return false;
    eye = {e[0], e[1], e[2]};
    target = {t[0], t[1], t[2]};
    if (!finite(eye) || !finite(target)) {
        error = "Nonfinite camera clip view";
        return false;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation
