#include "runtime_source_fx_asset_v1.hpp"

#include "../../../engine-resources/resources.hpp"
#include "../../../scene-materials/particle_scene_v1.hpp"

#include <iostream>
#include <stdexcept>

namespace {
constexpr const char* kUri =
    "data/3D/interface/skill_dh2_prince_warrior_bash_down.bdae";
constexpr const char* kEffectHash =
    "21a31d374a80b9b6f7d1f9b7f273c62459dde9bfd9a2fabfa629fc2fae83a483";
constexpr const char* kAnimationHash =
    "087068efba8eb4626510292c4cf9257e119e209965e7ab591e5250ea2609be0b";

void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

void decode_authored_scene(const std::vector<std::uint8_t>& bytes) {
    dh2::resources::BresView view{};
    check(dh2_bres_open(&view, bytes.data(), bytes.size()) == dh2::resources::BresError::ok,
          "Exact source FX bytes are not a valid BRES resource");
    dh2::scene::Scene scene;
    std::string error;
    check(dh2::scene::load_particle_scene_v1(view, scene, error),
          ("Exact source FX scene failed to decode: " + error).c_str());
    check(scene.nodes > 0 && !scene.graph.empty(),
          "Exact source FX resource has no authored nodes");
    check(!scene.instances.empty(), "Exact source FX resource has no authored mesh nodes");
}
} // namespace

int main(int argc, char** argv) try {
    check(argc == 5,
          "Usage: runtime_source_fx_asset_v1_tests good-root missing-root wrong-root cache-root");
    check(dh::foundation::effects::is_runtime_source_fx_uri_v1(kUri),
          "Canonical authored FX URI was not recognized");
    check(!dh::foundation::effects::is_runtime_source_fx_uri_v1(
              "data/3D/characters/prince/animations/skill_dh2_prince_warrior_bash_down.bdae"),
          "Animation URI was incorrectly classified as an interface FX URI");

    std::string error, resolved;
    std::vector<std::uint8_t> bytes;
    const dh::foundation::AssetCatalog good(argv[1]);
    check(dh::foundation::effects::read_runtime_source_fx_asset_v1(
              good, kUri, bytes, resolved, error),
          ("Exact canonical FX lookup failed: " + error).c_str());
    check(resolved == "data/3d/interface/skill_dh2_prince_warrior_bash_down.bdae",
          "FX resolver did not preserve the complete canonical URI");
    check(bytes.size() == 34420, "Resolved source FX resource size changed");
    check(dh::foundation::effects::verify_runtime_source_fx_sha256_v1(
              bytes, kEffectHash, error),
          ("Resolved source FX resource has wrong SHA-256: " + error).c_str());
    check(!dh::foundation::effects::verify_runtime_source_fx_sha256_v1(
              bytes, kAnimationHash, error),
          "Wrong animation-clip SHA-256 was accepted for the FX resource");
    decode_authored_scene(bytes);

    const dh::foundation::AssetCatalog missing(argv[2]);
    check(!dh::foundation::effects::read_runtime_source_fx_asset_v1(
              missing, kUri, bytes, resolved, error),
          "Basename-only animation fallback was accepted when canonical FX was missing");
    check(error.find("basename fallback disabled") != std::string::npos,
          "Missing exact FX diagnostic did not explain strict lookup");

    const dh::foundation::AssetCatalog wrong(argv[3]);
    check(dh::foundation::effects::read_runtime_source_fx_asset_v1(
              wrong, kUri, bytes, resolved, error),
          "Exact URI fixture should be read before provenance validation");
    check(!dh::foundation::effects::verify_runtime_source_fx_sha256_v1(
              bytes, kEffectHash, error),
          "Wrong same-basename animation resource passed expected FX hash validation");
    check(error.find("SHA-256 mismatch") != std::string::npos,
          "Wrong resource hash rejection lacked a diagnostic");

    const dh::foundation::AssetCatalog cache(argv[4]);
    check(dh::foundation::effects::read_runtime_source_fx_asset_v1(
              cache, kUri, bytes, resolved, error),
          ("Full original-cache URI lookup failed: " + error).c_str());
    check(resolved == "original-cache/data/3d/interface/skill_dh2_prince_warrior_bash_down.bdae",
          "Original-cache lookup did not preserve the full authored URI");
    check(dh::foundation::effects::verify_runtime_source_fx_sha256_v1(
              bytes, kEffectHash, error),
          "Original-cache full-URI candidate did not return the authored effect");
    decode_authored_scene(bytes);

    std::cout << "PASS | URI=" << kUri << " | bytes=34420 | sha256="
              << kEffectHash << " | nodes and mesh instances decoded"
              << " | basename animation fallback rejected"
              << " | original-cache/full-URI candidate verified"
              << " | wrong resource hash rejected\n";
    return 0;
} catch (const std::exception& e) {
    std::cerr << "FAIL | " << e.what() << '\n';
    return 1;
}
