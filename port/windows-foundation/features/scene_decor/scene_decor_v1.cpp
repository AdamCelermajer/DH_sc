#include "scene_decor_v1.hpp"

#include <algorithm>
#include <iostream>
#include <set>

namespace dh::foundation::scene_decor {

bool SceneDecorRuntimeV1::adopt(const AssetCatalog& assets, const std::vector<ActorDefinition>& definitions,
                                const std::vector<std::string>& wanted, std::vector<std::string>& notices,
                                std::string& error) {
    error.clear();
    slots_.clear();
    const std::set<std::string> names(wanted.begin(), wanted.end());
    std::set<std::string> seen;
    for (const auto& definition : definitions) {
        if (definition.gametype != "AnimatedDecor" || !names.count(definition.name)) continue;
        if (!seen.insert(definition.name).second) continue; // one visual per named object (the first declaration)
        Slot slot;
        slot.name = definition.name;
        slot.model_path = definition.modelPath;
        slot.transform = definition.placement;
        if (slot.model_path.empty()) {
            notices.push_back("scene object " + slot.name + " has no model (dae)");
            continue;
        }
        auto visual = std::make_unique<CharacterVisual>();
        std::string loadError;
        bool ok = false;
        try {
            ok = visual->load_embedded_scene(assets, slot.model_path, loadError) && !visual->meshes().empty();
            if (ok) {
                const auto start = definition.properties.count("startanim") ? definition.properties.at("startanim") : std::string("idle");
                if (!visual->select(start, true, loadError)) {
                    // The authored start clip is missing: keep the bind pose and say so.
                    notices.push_back("scene object " + slot.name + " start clip " + start + " unavailable: " + loadError);
                    loadError.clear();
                }
            }
        } catch (const std::exception& exception) {
            ok = false;
            loadError = exception.what();
        }
        if (!ok) {
            notices.push_back("scene object " + slot.name + " model unavailable " + slot.model_path + ": " + loadError);
            continue;
        }
        slot.visual = std::move(visual);
        std::cout << "[scene] AnimatedDecor " << slot.name << " model=" << slot.model_path << " meshes="
                  << slot.visual->meshes().size() << '\n';
        slots_.push_back(std::move(slot));
    }
    return true;
}

bool SceneDecorRuntimeV1::play(const std::string& name, const std::string& clip, bool& found, std::string& error) {
    error.clear();
    found = false;
    for (auto& slot : slots_) {
        if (slot.name != name) continue;
        found = true;
        if (!slot.visual) return true;
        const bool loop = clip.rfind("idle", 0) == 0;
        if (!slot.visual->select(clip, loop, error)) {
            // Source Play of an unknown clip is a no-op on the animator; the object keeps its pose.
            std::cout << "[scene] PlayAnimByName " << name << " clip " << clip << " unavailable: " << error << '\n';
            error.clear();
            return true;
        }
        std::cout << "[scene] PlayAnimByName " << name << " clip=" << clip << " loop=" << loop << '\n';
        return true;
    }
    return true;
}

bool SceneDecorRuntimeV1::update(double seconds, std::string& error) {
    error.clear();
    for (auto& slot : slots_)
        if (slot.visual && !slot.visual->update(seconds, error)) return false;
    return true;
}

std::vector<SceneDecorView> SceneDecorRuntimeV1::views() const {
    std::vector<SceneDecorView> out;
    out.reserve(slots_.size());
    for (const auto& slot : slots_) out.push_back({slot.name, slot.visual.get(), slot.transform});
    return out;
}

} // namespace dh::foundation::scene_decor
