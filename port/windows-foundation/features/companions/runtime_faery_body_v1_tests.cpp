// v21.8 item 8: the player's own faery body. Pure rules plus, when the staged package is present, a real load of all
// five faery models through the Celest clip bank (the shared AnimTable 23 "Faery" clips).
#include "runtime_faery_body_v1.hpp"
#include "../../content_paths.hpp"

#include <cmath>
#include <filesystem>
#include <iostream>

using namespace dh::foundation;
using namespace dh::foundation::companions;
namespace fs = std::filesystem;

namespace {
int failures = 0;
void check(bool ok, const char* what) {
    if (!ok) { std::cerr << "FAIL: " << what << '\n'; ++failures; }
}
} // namespace

int main() {
    check(faery_body_model_uri_v1("faeries_03_rocky") == "data/3D/characters/faeries/faeries_03_rocky.bdae", "model uri");

    // PlaceFaeryAndFollowers: master + UNIT look vector, z unchanged.
    const float master[3] = {100.0f, -50.0f, 37.0f};
    const auto east = faery_placement_position_v1(master, 0.0f);
    check(std::fabs(east[0] - 101.0f) < 1e-4f && std::fabs(east[1] + 50.0f) < 1e-4f && east[2] == 37.0f, "placement east");
    const auto north = faery_placement_position_v1(master, 1.5707963f);
    check(std::fabs(north[0] - 100.0f) < 1e-3f && std::fabs(north[1] + 49.0f) < 1e-3f, "placement north");
    const auto diagonal = faery_placement_position_v1(master, 0.6f);
    const float dx = diagonal[0] - master[0], dy = diagonal[1] - master[1];
    check(std::fabs(std::sqrt(dx * dx + dy * dy) - 1.0f) < 1e-4f, "placement is a unit step");

    const fs::path root = fs::path(".local-inputs") / "windows-source-clock-v21.8-staged" / "assets";
    if (!fs::exists(root / "data/3D/characters/faeries/faeries_00_hotty.bdae")) {
        std::cout << "SKIP asset part: staged v21.8 package not present\n";
        return failures ? 1 : 0;
    }
    AssetCatalog assets(root);
    CharacterVisualConfig config;
    const auto rel = [&](const char* uri) { return resolve_content_path(assets, uri).lexically_relative(assets.root()).generic_string(); };
    config.model_path = rel("data/3D/characters/faeries/faeries_02_celeste.bdae");
    config.template_clip_path = rel("data/3D/characters/faeries/animations/faeries_template_anim.bdae");
    config.clips = {{"Idle", rel("data/3D/characters/faeries/animations/faeries_celeste_idle.bdae")},
                    {"Walk", rel("data/3D/characters/faeries/animations/faeries_celeste_walk.bdae")}};
    config.allow_missing_animation_targets = true;
    config.include_static_instances = true;
    config.use_authored_modular_defaults = true;
    CharacterVisual celest;
    std::string error;
    check(celest.load(assets, config, error), ("celest load: " + error).c_str());

    RuntimeFaeryBodyV1 bodies;
    int loadedCallbacks = 0;
    for (const char* name : {"faeries_00_hotty", "faeries_01_wetty", "faeries_03_rocky", "faeries_04_windy", "faeries_02_celeste"}) {
        const auto uri = faery_body_model_uri_v1(name);
        auto* body = bodies.acquire(assets, celest, uri, [&](CharacterVisual&) { ++loadedCallbacks; }, error);
        check(body != nullptr, (std::string("acquire ") + name + ": " + error).c_str());
        if (!body) continue;
        check(body->meshes().size() > 0, (std::string("body has meshes ") + name).c_str());
        check(RuntimeFaeryBodyV1::mirror_pose(*body, "Idle", 500, error), (std::string("idle pose ") + name + ": " + error).c_str());
        check(RuntimeFaeryBodyV1::mirror_pose(*body, "Walk", 300, error), (std::string("walk pose ") + name + ": " + error).c_str());
        check(bodies.acquire(assets, celest, uri, nullptr, error) == body, "second acquire returns the same body");
    }
    check(loadedCallbacks == 5 && bodies.loaded_count() == 5, "each model loads once");
    check(bodies.acquire(assets, celest, "data/3D/characters/faeries/missing.bdae", nullptr, error) == nullptr && !error.empty(),
          "a missing model fails");
    const auto firstFailure = error;
    check(bodies.acquire(assets, celest, "data/3D/characters/faeries/missing.bdae", nullptr, error) == nullptr && error == firstFailure,
          "a failed model is remembered");
    if (failures == 0) std::cout << "runtime_faery_body_v1: PASS\n";
    return failures ? 1 : 0;
}
