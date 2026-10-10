// P17 PROCGEN diagnostic: procedural `.rule.xml` coverage over every rule in the asset root.
// Usage: procedural_coverage <asset-root> [seed-count]
// For each rule: generates several seeds (generator + module projection), loads the first generated document
// through the fixed-map loaders (geometry, declarations, first SpawnPoint) and checks the backup map exists.
// Prints one tab-separated line per rule.
#include "procedural_level_v1.hpp"
#include "actor_definitions.hpp"
#include "assembled_level.hpp"
#include "original_scene.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::procedural;
namespace fs = std::filesystem;

namespace {
std::string one_line(std::string text) {
    std::replace(text.begin(), text.end(), '\n', ' ');
    std::replace(text.begin(), text.end(), '\t', ' ');
    return text;
}
} // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: procedural_coverage <asset-root> [seed-count]\n";
        return 2;
    }
    const std::uint32_t seedCount = argc > 2 ? static_cast<std::uint32_t>(std::stoul(argv[2])) : 8u;
    AssetCatalog assets(argv[1]);

    std::vector<std::string> rules;
    for (const auto& entry : fs::directory_iterator(assets.root() / "data" / "scene")) {
        const auto name = entry.path().filename().string();
        if (entry.is_regular_file() && name.size() > 9 && name.compare(name.size() - 9, 9, ".rule.xml") == 0)
            rules.push_back("data/scene/" + name);
    }
    std::sort(rules.begin(), rules.end());

    std::cout << "rule\tseeds_ok\tseeds\tdistinct_layouts\ttiles\tmodules\tunplaced\tgeometry_triangles\t"
                 "declarations\tfirst_spawn\tbackup_fallback\tfirst_failure\n";
    int fullyLoaded = 0;
    for (const auto& rule : rules) {
        std::set<std::uint64_t> signatures;
        std::uint32_t okCount = 0;
        std::string firstFailure;
        bool haveFirst = false;
        GeneratedLevel first;
        // Optional third argument: a rule substring; for those rules every seed is listed (failing seeds for the report).
        const bool listSeeds = argc > 3 && rule.find(argv[3]) != std::string::npos;
        for (std::uint32_t s = 1; s <= seedCount; ++s) {
            GeneratedLevel generated;
            std::string error;
            const auto seed = s * 7919u;
            if (generate_level(assets, rule, seed, generated, error)) {
                ++okCount;
                signatures.insert(layout_signature(generated));
                if (!haveFirst) { first = generated; haveFirst = true; }
                if (listSeeds) std::cout << "seed\t" << rule << '\t' << seed << "\tok\ttiles=" << generated.tiles << '\n';
            } else {
                if (firstFailure.empty()) firstFailure = error;
                if (listSeeds) std::cout << "seed\t" << rule << '\t' << seed << "\tfail\t" << one_line(error) << '\n';
            }
        }
        std::string geometry = "-", declarations = "-", spawn = "-";
        std::string loadFailure;
        if (haveFirst) {
            install_generated_level(assets, first);
            OriginalScene scene;
            std::vector<LevelModuleZone> zones;
            std::string error;
            if (load_level_with_module_zones(assets, rule, scene, zones, error)) geometry = std::to_string(scene.triangleCount);
            else loadFailure = "geometry: " + error;
            std::vector<ActorDefinition> definitions;
            if (load_actor_definitions(assets, rule, definitions, error)) {
                declarations = std::to_string(definitions.size());
                for (const auto& definition : definitions) {
                    if (definition.gametype != "SpawnPoint") continue;
                    const auto& p = definition.placement;
                    spawn = std::to_string(p[12]) + "," + std::to_string(p[13]) + "," + std::to_string(p[14]);
                    break;
                }
            } else if (loadFailure.empty()) {
                loadFailure = "declarations: " + error;
            }
        }
        // The candidate the EXE fallback would load (IDA name first, then the documented alias), or missing.
        const auto backup = resolve_procedural_backup(assets, rule);
        std::string backupState = "missing";
        try { assets.resolve(backup); backupState = backup.substr(backup.find_last_of('/') + 1); } catch (const std::exception&) {}
        const bool loaded = haveFirst && loadFailure.empty();
        if (loaded) ++fullyLoaded;
        std::string failure = firstFailure.empty() ? loadFailure : firstFailure;
        std::cout << rule << '\t' << okCount << '\t' << seedCount << '\t' << signatures.size() << '\t'
                  << (haveFirst ? first.tiles : 0) << '\t' << (haveFirst ? first.modules : 0) << '\t'
                  << (haveFirst ? first.unplaced_modules : 0) << '\t' << geometry << '\t' << declarations << '\t'
                  << spawn << '\t' << backupState << '\t' << one_line(failure.empty() ? "-" : failure) << '\n';
    }
    std::cout << "# rules=" << rules.size() << " loaded_from_generation=" << fullyLoaded << '\n';
    return 0;
}
