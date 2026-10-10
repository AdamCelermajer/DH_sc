// P17 MERGE: one character save carries every feature component at once (procgen seed, transition entry, and the
// component bytes of the door/gate, lifecycle and container owners). It saves at schema 4 with no version bump, the
// components reload exactly, and a re-save is byte exact. Owners of the other components write their own bytes; this
// test only checks that the save roster keeps them.
#include "../procedural/procedural_level_v1.hpp"
#include "../transitions/transition_persistence_v1.hpp"
#include "../../character_state.hpp"
#include "../../save_store.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {
int failures = 0;
void check(bool ok, const std::string& what) {
    if (!ok) {
        ++failures;
        std::cerr << "FAILED: " << what << '\n';
    }
}
std::vector<char> read_bytes(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    return std::vector<char>(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}
} // namespace

int main() {
    using namespace dh::foundation;
    std::string error;
    const auto dir = std::filesystem::temp_directory_path() / "dh_p17_all_components";
    std::filesystem::create_directories(dir);
    const auto first = dir / "all.save";
    const auto second = dir / "all-resaved.save";

    CharacterState state = make_default_character("all-components", "All", "KnightPlayerBase");
    const procedural::ProceduralSeeds seeds{1234u, 98765u, 7u};
    procedural::store_saved_seeds(state, seeds);
    transitions::set_entry_component(state, "data/scene/003_darkwood.mlx", 13);
    const std::vector<CharacterComponent> owned = {
        {"dh2.world-gate.door.v1", {1}},                              // door gate: last applied value
        {"dh2.world-gate.v1.IsAfter_Swamp_KillWitch2", {1}},          // gate of a named condition
        {"campaign_lifecycle_v1", {2, 0, 0, 0, 5, 1}},                // OPENING3 lifecycle component
        {"dh2.container.hits.v1", {4, 0, 0, 0}},                      // container hit count (u32)
        {"dh2.source-container.objs.v1", {9, 8, 7, 6, 5, 4, 3}},      // container objects (7 bytes)
    };
    state.components.insert(state.components.end(), owned.begin(), owned.end());

    check(save_character(first, state, error), "all-component save: " + error);
    CharacterState reloaded;
    check(load_character(first, reloaded, error), "all-component reload: " + error);
    check(reloaded.schema_version == 4 && character_schema_version == 4, "schema stays 4 with all components");

    const auto bytes = read_bytes(first);
    check(bytes.size() > 12 && bytes[12] == 4, "the file header says schema 4");
    check(procedural::saved_seeds(reloaded) == seeds, "procgen seed component survives the reload");
    std::string level;
    int entry = -1;
    check(transitions::entry_component(reloaded, level, entry) && level == "data/scene/003_darkwood.mlx" && entry == 13,
          "transition entry component survives the reload");
    check(reloaded.components.size() == owned.size() + 2, "every component is present after the reload");
    for (const auto& component : owned) {
        bool found = false;
        for (const auto& loaded : reloaded.components)
            if (loaded.id == component.id) found = loaded.bytes == component.bytes;
        check(found, "owned component reloads byte exact: " + component.id);
    }

    check(save_character(second, reloaded, error), "re-save of the reloaded state: " + error);
    check(read_bytes(second) == bytes, "re-saving an all-component state is byte exact");

    std::filesystem::remove_all(dir);
    if (failures == 0) {
        std::cout << "save_all_components_v4 tests passed\n";
        return 0;
    }
    return 1;
}
