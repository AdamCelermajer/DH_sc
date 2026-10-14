// v21.8 subclass: TEST-ONLY save writer (not part of the product, not a CTest). It turns a character save into a
// subclass hero save for a real hidden-window run: class_id = the CharacterTable row (e.g. RoguePlayerBase_Assassin),
// the skill list = that row's original SkillTree list at rank 0, the chosen skill at rank 1 on hotbar slot 0, and the
// HP/MP sheets resolved for that row at the chosen level (as the original level-up grants them).
//
// usage: subclass_save_writer_v21_8 <repo-root> <in.save> <out.save> <CharacterTable row> <skill name> <level>
#include "../asset_catalog.hpp"
#include "../features/generic_skills/runtime_skill_activation_v1.hpp"
#include "../features/generic_skills/runtime_skill_cast_prepare_v1.hpp"
#include "../original_actor_properties.hpp"
#include "../save_store.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace dh::foundation;

namespace {
dh2::data::Bytes bytes_of(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}
} // namespace

int main(int argc, char** argv) {
    if (argc != 7 && argc != 8) {
        std::cerr << "usage: subclass_save_writer_v21_8 <repo-root> <in.save> <out.save> <row> <skill> <level>\n";
        return 2;
    }
    const std::filesystem::path repo = argv[1];
    const std::string row = argv[4];
    // w2-paladin test seed: "RANKED+SLOT" ranks RANKED (a passive) and puts SLOT (an active) on hotbar slot 0.
    const std::string skill_arg = argv[5];
    const auto plus_at = skill_arg.find('+');
    const std::string skill = plus_at == std::string::npos ? skill_arg : skill_arg.substr(0, plus_at);
    const std::string slot_skill = plus_at == std::string::npos ? skill : skill_arg.substr(plus_at + 1);
    const auto level = static_cast<std::int32_t>(std::stol(argv[6]));
    if (level < 1) { std::cerr << "level must be positive\n"; return 2; }
    std::string error;

    AssetCatalog assets(repo / ".local-inputs/windows-source-clock-v21.8-staged/assets");
    OriginalPropertyDatabase properties;
    if (!load_original_property_tables(assets, "original-cache/data/pydata", properties, error)) {
        std::cerr << "table load failed: " << error << "\n";
        return 1;
    }
    const auto skill_data = assets.read("original-cache/data/pydata/skills_pyarray.bin");
    const auto skill_names = assets.read("original-cache/data/pydata/skills_pyarraynames.bin");
    const auto skill_schema = assets.read("original-cache/data/pydata/skills_pystructnames.bin");
    dh2::data::SkillTables skill_owner;
    if (!skill_owner.load(bytes_of(skill_data), bytes_of(skill_names), bytes_of(skill_schema), error)) {
        std::cerr << "skill table load failed: " << error << "\n";
        return 1;
    }
    const auto skill_tables = skill_owner.borrow();

    const auto& characters = properties.characters;
    const auto row_it = std::find(characters.names.begin(), characters.names.end(), row);
    const auto tree_column = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
    if (row_it == characters.names.end() || tree_column == characters.fields.end()) {
        std::cerr << "CharacterTable row or SkillTree field is missing: " << row << "\n";
        return 1;
    }
    const auto tree_id = characters.rows[std::size_t(row_it - characters.names.begin())][std::size_t(tree_column - characters.fields.begin())];
    if (tree_id < 0 || std::size_t(tree_id) >= skill_tables.lists().size()) {
        std::cerr << "row has no SkillList: " << row << "\n";
        return 1;
    }
    const auto& source_list = skill_tables.lists()[std::size_t(tree_id)];
    std::size_t chosen = source_list.size();
    for (std::size_t i = 0; i < source_list.size(); ++i) {
        const auto id = source_list[i];
        if (id >= 0 && std::size_t(id) < skill_tables.skill_names().size() && skill_tables.skill_names()[std::size_t(id)] == skill) {
            chosen = i;
            break;
        }
    }
    if (chosen == source_list.size()) {
        std::cerr << "SkillList of " << row << " has no skill " << skill << "\n";
        return 1;
    }

    CharacterState state;
    if (!load_character(argv[2], state, error)) {
        std::cerr << "load failed: " << error << "\n";
        return 1;
    }
    OriginalActorProperties resolved;
    if (!resolve_original_actor_properties(properties.characters, properties.classes, row, {level * 256, true}, resolved, error)) {
        std::cerr << "property resolve failed: " << error << "\n";
        return 1;
    }
    state.class_id = row;
    state.stats.level = static_cast<std::uint32_t>(level);
    state.stats.health = original_signed256(resolved.sheets.resolved[36]);
    state.stats.max_health = original_signed256(resolved.sheets.resolved[38]);
    state.stats.resource = original_signed256(resolved.sheets.resolved[41]);
    state.stats.max_resource = original_signed256(resolved.sheets.resolved[43]);
    // w2-paladin test seed only: optional absolute starting HP (clamped to max), for heal and damage before/after runs.
    if (argc == 8) state.stats.health = std::min(static_cast<float>(std::stod(argv[7])), state.stats.max_health);
    state.skills.clear();
    for (const auto table_id : source_list) {
        state.skills.push_back({skill_tables.skill_names()[std::size_t(table_id)], 0});
    }
    std::size_t slot_chosen = source_list.size();
    for (std::size_t i = 0; i < source_list.size(); ++i) {
        const auto id = source_list[i];
        if (id >= 0 && std::size_t(id) < skill_tables.skill_names().size() && skill_tables.skill_names()[std::size_t(id)] == slot_skill) {
            slot_chosen = i;
            break;
        }
    }
    if (slot_chosen == source_list.size()) { std::cerr << "slot skill " << slot_skill << " is not in the SkillList\n"; return 2; }
    state.skills[chosen].rank = 1;
    state.skill_slots.clear();
    state.skill_slots.push_back({0, 0, static_cast<std::uint32_t>(slot_chosen)});
    state.source_skill_slots_known = true;
    if (!save_character(argv[3], state, error)) {
        std::cerr << "save failed: " << error << "\n";
        return 1;
    }
    std::cout << "wrote " << argv[3] << " class=" << row << " skills=" << state.skills.size() << " slot0=" << skill
              << " level=" << level << " hp=" << state.stats.max_health << " mp=" << state.stats.max_resource << "\n";
    return 0;
}
