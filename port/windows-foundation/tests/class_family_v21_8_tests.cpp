// v21.8 subclass: the family mapping used for player presentation lookups, and the original CharacterTable fact it
// depends on (a subclass row shares the base row's model, animation table, AI, sounds, scale and class id).
#include "../asset_catalog.hpp"
#include "../original_actor_properties.hpp"
#include "../original_class_family.hpp"
#include "../original_melee_bindings.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace dh::foundation;

namespace {
void require(bool ok, const std::string& message) {
    if (!ok) {
        std::cerr << "FAIL " << message << "\n";
        std::exit(1);
    }
}

std::int32_t field(const OriginalPropertyDatabase& db, const std::string& row, const std::string& name) {
    const auto row_it = std::find(db.characters.names.begin(), db.characters.names.end(), row);
    const auto column = std::find(db.characters.fields.begin(), db.characters.fields.end(), name);
    require(row_it != db.characters.names.end() && column != db.characters.fields.end(), "missing " + row + "." + name);
    return db.characters.rows[std::size_t(row_it - db.characters.names.begin())][std::size_t(column - db.characters.fields.begin())];
}
} // namespace

int main(int argc, char** argv) {
    const std::string repo = argc > 1 ? argv[1] : ".";

    // 1. Mapping (pure string rule; the subclass rows are "<Base>_<Spec>").
    require(playable_base_class_row("KnightPlayerBase") == "KnightPlayerBase", "base row changed");
    require(playable_base_class_row("KnightPlayerBase_Berserker") == "KnightPlayerBase", "Berserker family");
    require(playable_base_class_row("KnightPlayerBase_Paladin") == "KnightPlayerBase", "Paladin family");
    require(playable_base_class_row("RoguePlayerBase_Archer") == "RoguePlayerBase", "Archer family");
    require(playable_base_class_row("RoguePlayerBase_Assassin") == "RoguePlayerBase", "Assassin family");
    require(playable_base_class_row("MagePlayerBase_Illusionist") == "MagePlayerBase", "Illusionist family");
    require(playable_base_class_row("MagePlayerBase_Necromancer") == "MagePlayerBase", "Necromancer family");
    require(playable_base_class_row("KingPlayer") == "KingPlayer", "KingPlayer changed");
    require(playable_base_class_row("Swamp_LizadMan_Type1") == "Swamp_LizadMan_Type1", "non-player row changed");

    // 2. The original CharacterTable: each subclass row shares the base row's presentation fields.
    AssetCatalog assets(std::filesystem::path(repo) / ".local-inputs/windows-shared-assets");
    OriginalPropertyDatabase properties;
    std::string error;
    require(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
    const std::vector<std::pair<std::string, std::vector<std::string>>> families = {
        {"KnightPlayerBase", {"KnightPlayerBase_Berserker", "KnightPlayerBase_Paladin"}},
        {"RoguePlayerBase", {"RoguePlayerBase_Archer", "RoguePlayerBase_Assassin"}},
        {"MagePlayerBase", {"MagePlayerBase_Illusionist", "MagePlayerBase_Necromancer"}},
    };
    for (const auto& family : families) {
        for (const auto& sub : family.second) {
            for (const char* name : {"ModelFile", "AnimTable", "AI", "AIFaction", "Sounds", "ClassID", "ModularVisual",
                                     "Scale_X", "Scale_Y", "Scale_Z"}) {
                require(field(properties, sub, name) == field(properties, family.first, name),
                        sub + " " + name + " differs from " + family.first);
            }
            require(field(properties, sub, "SkillTree") != field(properties, family.first, "SkillTree"),
                    sub + " has the base SkillTree");
        }
    }

    // 3. The subclass actors of the exported bindings resolve to the base presentation (same model and state trees).
    OriginalMeleeBindings bindings;
    AssetCatalog metadata(std::filesystem::path(repo) / ".local-inputs/windows-melee-bindings");
    require(bindings.load(metadata, "original-melee-bindings.xml", error), error);
    for (const auto& family : families) {
        const auto* base = bindings.find_actor(family.first);
        require(base != nullptr, "base binding missing: " + family.first);
        for (const auto& sub : family.second) {
            const auto* actor = bindings.find_actor(sub);
            require(actor != nullptr, "subclass binding missing: " + sub);
            require(actor->model == base->model && actor->states.size() == base->states.size(),
                    sub + " binding does not share the base model/state set");
        }
    }
    std::cout << "PASS class family: subclass rows map to their base presentation row; original CharacterTable and "
                 "melee bindings share model/AnimTable/AI/sounds/scale\n";
    return 0;
}
