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
    // save/load round trip (named component, current schema5) and does not leak into another difficulty.
    CharacterState profile = make_default_character("procgen-seed", "Seed", "KnightPlayerBase");
    check(session_seed(profile, 0, std::nullopt, 77u) == 77u, "fresh seed used when nothing is saved");
    check(session_seed(profile, 0, 555u, 77u) == 555u, "forced seed wins over the fresh seed");
    record_session_seed(profile, 1, 4242u);
    check(session_seed(profile, 1, std::nullopt, 77u) == 4242u, "saved seed of the active difficulty is used");
    check(session_seed(profile, 0, std::nullopt, 77u) == 77u, "a seed of one difficulty does not leak into another");
    // Current schema5 component roster: the seeds are the named component `procedural_seed_component_id`.
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

    // 1. Round trip: seeds per difficulty survive and a re-save is byte exact.
    ProceduralSeeds stored{1234u, 98765u, 7u};
    CharacterState seeded = profile;
    store_saved_seeds(seeded, stored);
    CharacterState reloaded;
    check(save_character(seedFile, seeded, saveError) && load_character(seedFile, reloaded, saveError) &&
          saved_seeds(reloaded) == stored && reloaded.schema_version == 5 && character_schema_version == 5,
          "saved procedural seeds survive a schema5 round trip" + (saveError.empty() ? std::string() : ": " + saveError));
    const auto seedBytes = readBytes(seedFile);
    check(seedBytes.size() > schemaFieldAt && seedBytes[schemaFieldAt] == 5, "the seed component is written at schema5");
    CharacterState resaved;
    check(load_character(seedFile, resaved, saveError) && save_character(legacyFile, resaved, saveError) &&
          readBytes(legacyFile) == seedBytes, "re-saving a seeded profile is byte exact");

    // 2. Old saves: a pre-component v4 file (the 4-byte component count removed) loads with zero seeds.
    CharacterState plainProfile = make_default_character("procgen-plain", "Plain", "KnightPlayerBase");
    check(save_character(legacyFile, plainProfile, saveError), "plain profile saves");
    auto preComponent = readBytes(legacyFile);
    preComponent[schemaFieldAt] = 4;
    preComponent.resize(preComponent.size() - 4);   // drop the (empty) component count
    writeBytes(legacyFile, preComponent);
    CharacterState legacy;
    check(load_character(legacyFile, legacy, saveError) && saved_seeds(legacy) == ProceduralSeeds{} &&
          legacy.components.empty() && legacy.schema_version == 5,
          "a pre-component v4 save loads with zero procedural seeds" + (saveError.empty() ? std::string() : ": " + saveError));

    // 3. Unknown future schema6 reads stay out.
    auto v5Bytes = seedBytes;
    v5Bytes[schemaFieldAt] = 6;
    writeBytes(v5File, v5Bytes);
    CharacterState v5Loaded = plainProfile;
    check(!load_character(v5File, v5Loaded, saveError) && v5Loaded.id == plainProfile.id,
          "a schema 6 save is rejected and the destination is untouched");

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

    // Module visual decor (P17 PROCGEN2): Decor and AnimatedDecor children, placed by module x declaration transform.
    const std::string mvpXml =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?><Module>"
        "<GameObject gametype=\"AnimatedDecor\" name=\"water\" position=\"5,0,0\" dae=\"data/3D/AnimatedDecors/w.bdae\" xrefobject=\"_anim_water\"/>"
        "<GameObject gametype=\"Decor\" name=\"prop\" position=\"0,7,0\" dae=\"data/3D/Props/p.bdae\" xrefobject=\"_prop\"/>"
        "<GameObject gametype=\"Decor\" name=\"hidden\" visible=\"0\" dae=\"data/3D/Props/h.bdae\" xrefobject=\"_hidden\"/>"
        "<GameObject gametype=\"Decor\" name=\"nodae\" position=\"1,1,1\" xrefobject=\"_nodae\"/>"
        "<GameObject gametype=\"SpawnPoint\" name=\"spawn\" position=\"9,9,9\"/>"
        "</Module>";
    std::vector<std::uint8_t> mvpBytes(mvpXml.begin(), mvpXml.end());
    Mat4 modulePlacement{1,0,0,0, 0,1,0,0, 0,0,1,0, 100,200,0,1};
    std::vector<ModulePlacement> decor;
    std::string decorError;
    check(decode_module_decor(mvpBytes, modulePlacement, decor, decorError) && decor.size() == 2 &&
          decor[0].gametype == "AnimatedDecor" && decor[0].authoredNode == "_anim_water-node" &&
          decor[1].gametype == "Decor" && decor[1].assetPath == "data/3D/Props/p.bdae",
          "decor keeps Decor and AnimatedDecor with a dae, skipping hidden, dae-less and other gametypes" +
          (decorError.empty() ? std::string() : ": " + decorError));
    check(decor[0].placement[12] == 105.0f && decor[0].placement[13] == 200.0f && decor[1].placement[13] == 207.0f,
          "decor placement is the module placement times the declaration's own translation");
    const std::string configXml =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?><Level><GameObject gametype=\"LevelConfig\" skybox=\"data/3D/skybox/s.bdae\"/></Level>";
    check(decode_level_skybox(std::vector<std::uint8_t>(configXml.begin(), configXml.end())) == "data/3D/skybox/s.bdae",
          "LevelConfig skybox attribute is read");
    const std::string noConfigXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><Level><GameObject gametype=\"Module\"/></Level>";
    check(decode_level_skybox(std::vector<std::uint8_t>(noConfigXml.begin(), noConfigXml.end())).empty(),
          "a level without LevelConfig has no skybox");

    // A failing rule reports an error (caller falls back to the backup map).
    GeneratedLevel bad;
    check(!generate_level(assets, "data/scene/does_not_exist.rule.xml", 1, bad, error), "missing rule fails");
    if (argc >= 4) { if (FILE* f = std::fopen(argv[3], "wb")) { std::fwrite(a.xml.data(), 1, a.xml.size(), f); std::fclose(f); } }
    std::cout << (failures ? "procedural_level_v1 FAILED\n" : "procedural_level_v1 OK\n");
    return failures ? 1 : 0;
}
