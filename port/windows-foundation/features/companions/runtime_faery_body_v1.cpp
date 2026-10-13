#include "runtime_faery_body_v1.hpp"

#include "../../content_paths.hpp"
#include "../../original_pose_blend.hpp"

#include <cmath>

namespace dh::foundation::companions {

std::string faery_body_model_uri_v1(std::string_view model_file) {
    return "data/3D/characters/faeries/" + std::string(model_file) + ".bdae";
}

std::array<float, 3> faery_placement_position_v1(const float master_position[3], float master_heading) noexcept {
    return {master_position[0] + std::cos(master_heading), master_position[1] + std::sin(master_heading),
            master_position[2]};
}

CharacterVisual* RuntimeFaeryBodyV1::acquire(const AssetCatalog& assets, const CharacterVisual& source,
                                             const std::string& model_uri,
                                             const std::function<void(CharacterVisual&)>& on_loaded,
                                             std::string& error) {
    error.clear();
    if (const auto found = bodies_.find(model_uri); found != bodies_.end()) return found->second.get();
    if (const auto failed = failures_.find(model_uri); failed != failures_.end()) {
        error = failed->second;
        return nullptr;
    }
    const auto* base = source.configuration();
    if (!base) {
        error = "Faery body requires the actor's loaded visual configuration";
        return nullptr;
    }
    CharacterVisualConfig config = *base;
    try {
        // Same resolver the actor profile uses for its model (original-cache or flattened package folders).
        config.model_path = resolve_content_path(assets, model_uri).lexically_relative(assets.root()).generic_string();
    } catch (const std::exception& exception) {
        error = "Faery body " + model_uri + ": " + exception.what();
        failures_[model_uri] = error;
        return nullptr;
    }
    auto body = std::make_unique<CharacterVisual>();
    std::string loadError;
    if (!body->load(assets, config, loadError)) {
        error = "Faery body " + model_uri + ": " + loadError;
        failures_[model_uri] = error;
        return nullptr;
    }
    if (on_loaded) on_loaded(*body);
    auto* raw = body.get();
    bodies_[model_uri] = std::move(body);
    return raw;
}

const CharacterVisual* RuntimeFaeryBodyV1::find(const std::string& model_uri) const {
    const auto found = bodies_.find(model_uri);
    return found == bodies_.end() ? nullptr : found->second.get();
}

bool RuntimeFaeryBodyV1::mirror_pose(CharacterVisual& body, const std::string& clip, std::int32_t sample_ms,
                                     std::string& error) {
    SkeletalPose pose;
    if (!body.sample_local_pose(clip, sample_ms, pose, error)) return false;
    return body.apply_local_pose(pose, error);
}

} // namespace dh::foundation::companions
