// P17 PROCGEN isolated tests. Usage: procedural_level_v1_tests <asset-root>
// The asset root must contain data/scene/*.rule.xml and the rule folder (data/3d/modules/...).
#include "procedural_level_v1.hpp"
#include "level_manifest.hpp"
#include "../../character_state.hpp"
#include "../../save_store.hpp"

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>

using namespace dh::foundation;
using namespace dh::foundation::procedural;

namespace {
int failures = 0;
void check(bool condition, const std::string& what) {
    std::cout << (condition ? "PASS " : "FAIL ") << what << '\n';
    if (!condition) ++failures;
}
} // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: procedural_level_v1_tests <asset-root>\n";
        return 2;
    }
    check(is_procedural_level_uri("data/scene/002_swamp_witchcave.rule.xml"), "rule uri recognised");
    check(!is_procedural_level_uri("data/scene/001_swamp.mlx"), "fixed map is not procedural");
    check(procedural_backup_uri("data/scene/002_swamp_witchcave.rule.xml") ==
          "data/scene/x02_swamp_witchcave_backup.mlx", "backup uri follows IDA rule (x + name + _backup.mlx)");

    AssetCatalog assets(argv[1]);
    const std::string rule = "data/scene/002_swamp_witchcave.rule.xml";

    GeneratedLevel a, b, c;
    std::string error;
    const bool okA = generate_level(assets, rule, 1234, a, error);
    check(okA, "witch cave generates for seed 1234" + (okA ? std::string() : ": " + error));
    if (!okA) return 1;
    check(generate_level(assets, rule, 1234, b, error), "witch cave generates again for seed 1234");
    check(a.xml == b.xml, "same seed gives identical level document");
    check(a.tiles == b.tiles && a.modules == b.modules, "same seed gives identical tile and module counts");
    check(generate_level(assets, rule, 98765, c, error), "witch cave generates for seed 98765");
    check(a.xml != c.xml, "different seed gives a different level document");
    std::cout << "info seed=1234 tiles=" << a.tiles << " modules=" << a.modules
              << " unplaced=" << a.unplaced_modules << " xml_bytes=" << a.xml.size() << '\n';
    std::cout << "info seed=98765 tiles=" << c.tiles << " modules=" << c.modules
              << " unplaced=" << c.unplaced_modules << '\n';
    check(a.modules > 0 && a.tiles >= a.modules, "modules emitted for generated tiles");

    // The generated document is served under the rule's own path and decodes as a fixed map.
    install_generated_level(assets, a);
    std::vector<ModulePlacement> placements;
    const auto bytes = assets.read(rule);
    check(bytes == a.xml, "virtual level file is returned by AssetCatalog::read");
    check(decode_level_manifest(bytes, placements, error), "generated document decodes through the level manifest" +
          (error.empty() ? std::string() : ": " + error));
    check(placements.size() == a.modules, "manifest module count equals emitted modules");

    // Session seed: a forced seed wins, the saved per-difficulty seed next, and a fresh seed last. The seed survives a
    // save/load round trip (schema v5 tail) and does not leak into another difficulty.
    CharacterState profile = make_default_character("procgen-seed", "Seed", "KnightPlayerBase");
    check(session_seed(profile, 0, std::nullopt, 77u) == 77u, "fresh seed used when nothing is saved");
    check(session_seed(profile, 0, 555u, 77u) == 555u, "forced seed wins over the fresh seed");
    record_session_seed(profile, 1, 4242u);
    check(session_seed(profile, 1, std::nullopt, 77u) == 4242u, "saved seed of the active difficulty is used");
    check(session_seed(profile, 0, std::nullopt, 77u) == 77u, "a seed of one difficulty does not leak into another");
    const auto seedFile = std::filesystem::temp_directory_path() / "dh-procgen-seed-roundtrip.save";
    std::string saveError;
    CharacterState reloaded;
    check(save_character(seedFile, profile, saveError) && load_character(seedFile, reloaded, saveError) &&
          reloaded.procedural_seed_by_difficulty == profile.procedural_seed_by_difficulty &&
          reloaded.schema_version == character_schema_version,
          "saved procedural seeds survive a save/load round trip" + (saveError.empty() ? std::string() : ": " + saveError));
    std::error_code removeError;
    std::filesystem::remove(seedFile, removeError);

    // A failing rule reports an error (caller falls back to the backup map).
    GeneratedLevel bad;
    check(!generate_level(assets, "data/scene/does_not_exist.rule.xml", 1, bad, error), "missing rule fails");
    if (argc >= 4) { if (FILE* f = std::fopen(argv[3], "wb")) { std::fwrite(a.xml.data(), 1, a.xml.size(), f); std::fclose(f); } }
    std::cout << (failures ? "procedural_level_v1 FAILED\n" : "procedural_level_v1 OK\n");
    return failures ? 1 : 0;
}
