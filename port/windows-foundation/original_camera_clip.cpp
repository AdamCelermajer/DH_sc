#include "original_camera_clip.hpp"
#include "asset_catalog.hpp"
#include "content_paths.hpp"
#include "../level-world/gameplay_camera_scene_v3.hpp"
#include "../engine-animation/animation.hpp"
#include "../engine-resources/resources.hpp"
#include "../asset-payloads/payloads.hpp"
#include "../game-data/data.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace dh::foundation {
namespace {
// Camera node of the level camera config (OriginalCameraConfig::node default).
constexpr const char* kCameraNode = "PlayerCamera_Default";

dh2::data::Bytes bytes_of(const std::vector<std::uint8_t>& v) { return {v.data(), v.size()}; }
bool finite(CameraVec3 v) { return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z); }
} // namespace

bool CameraClipLibrary::dictionary_path(const AssetCatalog& assets, std::int32_t id, std::string& path, std::string& error) {
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
            error = "Animation dictionary id absent: " + std::to_string(id);
            return false;
        }
        path = paths_[std::size_t(id)];
        if (path.size() < 5 || path.compare(path.size() - 5, 5, ".bdae") != 0) {
            error = "Animation dictionary entry is not a BDAE scene: " + path;
            return false;
        }
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

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
    // P16 OPENING4: the xfov channel (scalar keys, absolute clip milliseconds). The Player binds transform tracks only.
    std::vector<std::int32_t> fov_times;
    std::vector<float> fov_values;
};

namespace {
// Reads the first scalar track whose target names the camera xfov (the cs_* clips animate <camera>-camera/xfov).
void read_fov_channel(const std::uint8_t* data, std::size_t size, std::vector<std::int32_t>& times, std::vector<float>& values) {
    dh2::resources::BresView view{};
    if (!data || !size || dh2_bres_open(&view, data, size) != dh2::resources::BresError::ok) return;
    const auto count = dh2_bres_library_count(&view, dh2::resources::Library::animation);
    for (std::uint32_t i = 0; i < count && i < 10000; ++i) {
        dh2::assets::Animation a{};
        if (dh2_animation_open(&a, &view, std::int32_t(i), 0) != dh2::assets::Error::ok) continue;
        const char* target = dh2_animation_target(&a);
        if (!target || !std::strstr(target, "xfov")) continue;
        if (dh2_animation_channels(&a) != 1 || dh2_animation_samplers(&a) != 1) continue;
        dh2::assets::Vector keyValues{}, keyTimes{};
        if (!dh2_animation_vector(&a, 0, true, &keyValues) || !dh2_animation_vector(&a, 0, false, &keyTimes)) continue;
        if (keyValues.components != 1 || keyValues.count != keyTimes.count || !keyValues.count || keyValues.count > 100000) continue;
        times.clear();
        values.clear();
        for (std::uint32_t k = 0; k < keyValues.count; ++k) {
            float v[4]{};
            if (!dh2_vector_read(&keyValues, k, v) || !std::isfinite(v[0])) { times.clear(); values.clear(); break; }
            times.push_back(dh2_animation_key_time(&a, 0, std::int32_t(k)));
            values.push_back(v[0]);
        }
        if (!times.empty()) return;
    }
}
} // namespace

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
    read_fov_channel(clip_bytes.data(), clip_bytes.size(), next->fov_times, next->fov_values);
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

// P16 OPENING4: up and FOV of the clip (see header). The up vector is the authored 'upvector' node relative to the camera
// node after the same pose sample as eye/target; the FOV is the xfov channel, linearly interpolated, clamped at its ends.
bool OriginalCameraClip::sample_view(std::int32_t elapsed_ms, View& view, std::string& error) {
    CameraVec3 eye{}, target{};
    if (!sample(elapsed_ms, eye, target, error)) return false;
    auto& i = *impl_;
    view = View{};
    view.eye = eye;
    view.target = target;
    // Up: the 'upvector' node minus the camera node (world translations, same frame as eye).
    const auto& graph = i.scene.graph().graph;
    const auto cameraNode = i.scene.cameras()[i.selected].node;
    const auto upNode = std::find_if(graph.begin(), graph.end(), [](const dh2::scene::Node& n) { return n.name == "upvector"; });
    if (upNode != graph.end() && cameraNode < graph.size()) {
        const float dx = upNode->world[12] - graph[cameraNode].world[12];
        const float dy = upNode->world[13] - graph[cameraNode].world[13];
        const float dz = upNode->world[14] - graph[cameraNode].world[14];
        const float length = std::sqrt(dx * dx + dy * dy + dz * dz);
        if (std::isfinite(length) && length > 1e-4f) {
            view.up = {dx / length, dy / length, dz / length};
            view.has_up = true;
        }
    }
    // FOV: the xfov channel at the same absolute clip time the pose used.
    if (!i.fov_times.empty()) {
        const std::int64_t wanted = std::int64_t(i.player.start) + std::max<std::int32_t>(0, elapsed_ms);
        const auto time = std::int32_t(std::min<std::int64_t>(wanted, i.player.end));
        const auto& times = i.fov_times;
        std::size_t k = 0;
        while (k + 1 < times.size() && times[k + 1] <= time) ++k;
        float value = i.fov_values[k];
        if (k + 1 < times.size() && time > times[k] && times[k + 1] > times[k]) {
            const float t = float(time - times[k]) / float(times[k + 1] - times[k]);
            value = i.fov_values[k] + (i.fov_values[k + 1] - i.fov_values[k]) * t;
        }
        view.vertical_fov_degrees = value;
        view.has_fov = std::isfinite(value);
    }
    error.clear();
    return true;
}

} // namespace dh::foundation
