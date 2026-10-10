// P17 PROCGEN isolated tests. Usage: procedural_level_v1_tests <asset-root>
// The asset root must contain data/scene/*.rule.xml and the rule folder (data/3d/modules/...).
#include "procedural_level_v1.hpp"
#include "level_manifest.hpp"
#include "../../character_state.hpp"
#include "../../save_store.hpp"

#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
#include <vector>

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
    // Backup candidates: the IDA name, then the documented alias without the rule number (shipped 007 file).
    check(procedural_backup_candidates("data/scene/007_crypt_01.rule.xml") ==
          std::vector<std::string>{"data/scene/x07_crypt_01_backup.mlx", "data/scene/x07_crypt_backup.mlx"},
          "007 IDA backup name is followed by the shipped alias");
    check(procedural_backup_candidates("data/scene/002_swamp_witchcave.rule.xml").size() == 1,
          "a name without a rule number has no alias");
    check(resolve_procedural_backup(assets, "data/scene/007_crypt_01.rule.xml") == "data/scene/x07_crypt_backup.mlx",
          "007 fallback resolves to the shipped x07_crypt_backup.mlx");
    check(resolve_procedural_backup(assets, "data/scene/002_swamp_witchcave.rule.xml") ==
          "data/scene/x02_swamp_witchcave_backup.mlx", "002 fallback resolves to its IDA name");
    check(resolve_procedural_backup(assets, "data/scene/121_debug_test2.rule.xml") ==
          "data/scene/x21_debug_test2_backup.mlx", "a rule without a backup names the IDA name (absent)");
    bool absent121 = false;
    try { assets.resolve("data/scene/x21_debug_test2_backup.mlx"); } catch (const std::exception&) { absent121 = true; }
    check(absent121, "121 has no shipped backup (the EXE reports the missing file by name)");

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
    // save/load round trip (named component, schema 4) and does not leak into another difficulty.
    CharacterState profile = make_default_character("procgen-seed", "Seed", "KnightPlayerBase");
    check(session_seed(profile, 0, std::nullopt, 77u) == 77u, "fresh seed used when nothing is saved");
    check(session_seed(profile, 0, 555u, 77u) == 555u, "forced seed wins over the fresh seed");
    record_session_seed(profile, 1, 4242u);
    check(session_seed(profile, 1, std::nullopt, 77u) == 4242u, "saved seed of the active difficulty is used");
    check(session_seed(profile, 0, std::nullopt, 77u) == 77u, "a seed of one difficulty does not leak into another");
    // Schema v4 component roster (no version bump): the seeds are the named component `procedural_seed_component_id`.
    const auto seedDir = std::filesystem::temp_directory_path() / "dh-procgen-seed-tests";
    std::error_code dirError;
    std::filesystem::create_directories(seedDir, dirError);
    const auto seedFile = seedDir / "roundtrip.save";
    const auto legacyFile = seedDir / "pre-component-v4.save";
    const auto v5File = seedDir / "old-v5.save";
    const auto malformedFile = seedDir / "malformed.save";
    std::string saveError;
    const auto readBytes = [](const std::filesystem::path& path) {
        std::ifstream in(path, std::ios::binary);
        return std::vector<char>(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    };
    const auto writeBytes = [](const std::filesystem::path& path, const std::vector<char>& bytes) {
        std::ofstream out(path, std::ios::binary | std::ios::trunc);
        out.write(bytes.data(), std::streamsize(bytes.size()));
    };
    // The file offset of the schema field (magic 8 + file format version 4).
    constexpr std::size_t schemaFieldAt = 12;

    // 1. Round trip: seeds per difficulty survive, the schema stays 4, and a re-save is byte exact.
    ProceduralSeeds stored{1234u, 98765u, 7u};
    CharacterState seeded = profile;
    store_saved_seeds(seeded, stored);
    CharacterState reloaded;
    check(save_character(seedFile, seeded, saveError) && load_character(seedFile, reloaded, saveError) &&
          saved_seeds(reloaded) == stored && reloaded.schema_version == 4 && character_schema_version == 4,
          "saved procedural seeds survive a save/load round trip at schema 4" + (saveError.empty() ? std::string() : ": " + saveError));
    const auto seedBytes = readBytes(seedFile);
    check(seedBytes.size() > schemaFieldAt && seedBytes[schemaFieldAt] == 4, "the seed component is written at schema 4 (no bump)");
    CharacterState resaved;
    check(load_character(seedFile, resaved, saveError) && save_character(legacyFile, resaved, saveError) &&
          readBytes(legacyFile) == seedBytes, "re-saving a seeded profile is byte exact");

    // 2. Old saves: a pre-component v4 file (the 4-byte component count removed) loads with zero seeds.
    CharacterState plainProfile = make_default_character("procgen-plain", "Plain", "KnightPlayerBase");
    check(save_character(legacyFile, plainProfile, saveError), "plain profile saves");
    auto preComponent = readBytes(legacyFile);
    preComponent.resize(preComponent.size() - 4);   // drop the (empty) component count
    writeBytes(legacyFile, preComponent);
    CharacterState legacy;
    check(load_character(legacyFile, legacy, saveError) && saved_seeds(legacy) == ProceduralSeeds{} &&
          legacy.components.empty() && legacy.schema_version == 4,
          "a pre-component v4 save loads with zero procedural seeds" + (saveError.empty() ? std::string() : ": " + saveError));

    // 3. Old v5 reads stay out: a file that claims schema 5 is rejected (no version bump was shipped).
    auto v5Bytes = seedBytes;
    v5Bytes[schemaFieldAt] = 5;
    writeBytes(v5File, v5Bytes);
    CharacterState v5Loaded = plainProfile;
    check(!load_character(v5File, v5Loaded, saveError) && v5Loaded.id == plainProfile.id,
          "a schema 5 save is rejected and the destination is untouched");

    // 4. Named roster: an unknown component survives the round trip next to the seed component.
    CharacterState roster = seeded;
    CharacterComponent other;
    other.id = "dh2.test.unknown.v1";
    other.bytes = {1, 2, 3};
    roster.components.push_back(other);
    CharacterState rosterLoaded;
    check(save_character(seedFile, roster, saveError) && load_character(seedFile, rosterLoaded, saveError) &&
          rosterLoaded.components.size() == 2 && saved_seeds(rosterLoaded) == stored &&
          rosterLoaded.components[1].id == other.id && rosterLoaded.components[1].bytes == other.bytes,
          "an unknown component is preserved beside the seed component");

    // 5. A malformed seed component (not 12 bytes) reads as zero seeds instead of partial values.
    CharacterState malformed = plainProfile;
    CharacterComponent broken;
    broken.id = procedural_seed_component_id;
    broken.bytes = {1, 0, 0, 0, 2};
    malformed.components.push_back(broken);
    check(saved_seeds(malformed) == ProceduralSeeds{}, "a malformed seed component reads as zero seeds");

    std::error_code removeError;
    std::filesystem::remove_all(seedDir, removeError);

    // A failing rule reports an error (caller falls back to the backup map).
    GeneratedLevel bad;
    check(!generate_level(assets, "data/scene/does_not_exist.rule.xml", 1, bad, error), "missing rule fails");
    if (argc >= 4) { if (FILE* f = std::fopen(argv[3], "wb")) { std::fwrite(a.xml.data(), 1, a.xml.size(), f); std::fclose(f); } }
    std::cout << (failures ? "procedural_level_v1 FAILED\n" : "procedural_level_v1 OK\n");
    return failures ? 1 : 0;
}
