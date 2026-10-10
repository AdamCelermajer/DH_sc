#include "runtime_skill_session_training_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../game_save.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"
#include "../../save_store.hpp"
#include "../../../script-runtime/script_constants.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}

struct ClassFixture {
    const char* id;
    const char* skin;
};

void run_class(const ClassFixture& fixture, AssetCatalog& assets, OriginalPropertyDatabase& properties,
               OriginalMeleeBindings& bindings, const dh2::data::CharacterTable& characters,
               dh2::data::SkillTables::Borrow skills, const dh2::data::PropertyRules& rules,
               const CharacterDesignSkillCapsV1& caps) {
    std::string error;
    const auto class_name = std::find(characters.names.begin(), characters.names.end(), fixture.id);
    const auto tree_field = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
    check(class_name != characters.names.end() && tree_field != characters.fields.end(),
          std::string("Missing source CharacterTable row/SkillTree: ") + fixture.id);
    const auto class_row = static_cast<std::size_t>(class_name - characters.names.begin());
    const auto tree_column = static_cast<std::size_t>(tree_field - characters.fields.begin());
    const auto list_id = characters.rows[class_row][tree_column];
    check(list_id >= 0 && static_cast<std::size_t>(list_id) < skills.lists().size(),
          std::string("Source SkillTree does not resolve: ") + fixture.id);
    const auto& list = skills.lists()[static_cast<std::size_t>(list_id)];
    check(list.size() > 1, std::string("Source SkillList has no trainable follow-up: ") + fixture.id);

    ActorCustomization customization;
    customization.skin_id_contains = fixture.skin;
    customization.expected_controller_count = 4;
    customization.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan plan;
    check(build_original_combat_visual_plan(assets, bindings, fixture.id, customization,
              std::string("three-class-training-") + fixture.id, plan, error), error);

    CombatSessionConfig config;
    config.diagnosticRngSeed = static_cast<std::uint32_t>(0x51C000u + class_row);
    config.playerId = 1;
    config.playerProfileId = fixture.id;
    config.tableRoot = "original-cache/data/pydata";
    config.playerVisualConfig = plan.config;
    config.playerVisualConfig.motion_node_id = "auto";
    config.playerVisualConfig.consume_root_motion = true;
    CombatSessionProfile session_profile;
    session_profile.initialIdle = {"Idle", 0, {0}};
    session_profile.animationOnly = true;
    session_profile.propertyOptions = {3 * 256, false};
    session_profile.sourceAnimationClips = plan.config.clips;
    config.profiles.emplace(config.playerProfileId, std::move(session_profile));

    CombatSession session;
    CharacterVisual visual;
    ActorPopulation population;
    check(session.initialize(assets, properties, bindings, config, visual, population,
              {0, 0, 0}, customization, error), error);
    check(session.actor(1) && session.world() && session.world()->combat_properties(1),
          std::string("Source Session has no same-class player: ") + fixture.id);
    auto combat = *session.world()->combat_properties(1);
    const auto raw_level = combat.sheets.resolved[19];
    const auto raw_points = combat.sheets.resolved[157];
    check(raw_level >= 0 && (raw_level & 255) == 0 && raw_points > 0 && (raw_points & 255) == 0,
          std::string("Source class level/property157 is not integral/positive: ") + fixture.id);

    CharacterState state;
    state.id = std::string("same-session-training-") + fixture.id;
    state.name = fixture.id;
    state.class_id = fixture.id;
    state.stats.level = static_cast<std::uint32_t>(raw_level >> 8);
    state.source_points_known = true;
    state.source_skill_slots_known = true;
    state.source_skill_points = static_cast<std::uint32_t>(raw_points >> 8);
    for (const int table_id : list) {
        check(table_id >= 0 && static_cast<std::size_t>(table_id) < skills.skill_names().size(),
              std::string("Invalid source SkillTable ID: ") + fixture.id);
        state.skills.push_back({skills.skill_names()[static_cast<std::size_t>(table_id)], 0});
    }
    check(!state.skills.empty() && state.source_skill_points > 0,
          std::string("Source starter grant inputs unavailable: ") + fixture.id);

    // Native player_initial_skill_slots_v2 assigns source list position 0 to
    // hotbar slot 0. If that saved row is rank zero, it consumes one point.
    state.skills[0].rank = 1;
    state.skill_slots.push_back({0, 0, 0});
    --state.source_skill_points;
    session.actor(1)->persistent_character_id = state.id;
    auto property_view = dh2::data::property_view(rules, combat.sheets);
    check(dh2_property_add(&property_view, 157, -256) == 0 &&
          combat.sheets.resolved[157] ==
              static_cast<std::int32_t>(state.source_skill_points * 256u),
          std::string("Source starter grant did not debit property157: ") + fixture.id);
    const auto* traits = session.world()->traits(1);
    check(traits && session.world()->update_combat_properties(1, combat, *traits, error), error);

    // This exercise uses a real SaveStore load before the trained mutation.
    const auto nonce = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto save_root = std::filesystem::temp_directory_path() /
        (std::string("dh2-training-class-") + fixture.id + "-" + std::to_string(nonce));
    std::filesystem::create_directories(save_root);
    const auto profile_path = save_root / "before.dhsave";
    check(save_character(profile_path, state, error), error);
    CharacterState loaded;
    check(load_character(profile_path, loaded, error), error);
    check(loaded.class_id == fixture.id && loaded.stats.level == state.stats.level &&
          loaded.source_skill_points == state.source_skill_points &&
          loaded.skills.size() == list.size() && loaded.skills[0].rank == 1 &&
          loaded.skill_slots.size() == 1 && loaded.skill_slots[0].saved_skill_row == 0,
          std::string("SaveStore changed source starter/rank facts: ") + fixture.id);
    state = std::move(loaded);

    int position = -1;
    for (std::size_t i = 1; i < list.size(); ++i) {
        SkillProgressionV1 candidate;
        if (!evaluate_skill_progression_v1(state, characters, skills, caps,
                                            static_cast<int>(i), candidate, error)) continue;
        if (candidate.can_train) {
            position = static_cast<int>(i);
            break;
        }
    }
    check(position >= 1, std::string("No source level/rank/point-admitted row at this Session level: ") + fixture.id);
    const auto& row = skills.skills()[static_cast<std::size_t>(list[static_cast<std::size_t>(position)])];
    check(row.scalar.words[8] <= state.stats.level,
          std::string("Selected test row exceeds same-class source level: ") + fixture.id);

    SessionSkillTrainingCommitV1 committed;
    check(train_skill_in_session_v1(state, session, properties, skills, caps,
              position, committed, error), error);
    const auto* trained = session.world()->combat_properties(1);
    check(trained && committed.class_skill_position == position &&
          committed.saved_skill_row == position && committed.previous_rank == 0 &&
          committed.current_rank == 1 &&
          committed.remaining_points + 1u == static_cast<std::uint32_t>(raw_points >> 8) - 1u &&
          state.skills[static_cast<std::size_t>(position)].rank == 1 &&
          state.source_skill_points == committed.remaining_points &&
          trained->sheets.resolved[157] ==
              static_cast<std::int32_t>(state.source_skill_points * 256u),
          std::string("Same-Session commit split class rank/points/property157: ") + fixture.id);

    const auto after_path = save_root / "after.dhsave";
    check(save_character(after_path, state, error), error);
    CharacterState after_profile;
    check(load_character(after_path, after_profile, error), error);
    check(after_profile.skills[static_cast<std::size_t>(position)].rank == 1 &&
          after_profile.source_skill_points == committed.remaining_points,
          std::string("Post-training SaveStore split rank and points: ") + fixture.id);

    GameSave snapshot;
    check(capture_game_save(state.id, 1, state, *session.world(), snapshot, error), error);
    session.detach_for_restore();
    check(restore_game_save(snapshot, state.id, *session.world(), state, error), error);
    check(session.rebind_after_restore(error), error);
    const auto* restored = session.world()->combat_properties(1);
    check(restored && state.skills[static_cast<std::size_t>(position)].rank == 1 &&
          state.source_skill_points == committed.remaining_points &&
          restored->sheets.resolved[157] ==
              static_cast<std::int32_t>(committed.remaining_points * 256u),
          std::string("GameSave restore split rank/points/property157: ") + fixture.id);
    session.detach_for_restore();
    std::filesystem::remove_all(save_root);
    std::cout << fixture.id << " level=" << state.stats.level
              << " initial-property157=" << (raw_points >> 8)
              << " trained-position=" << position
              << " skill=" << state.skills[static_cast<std::size_t>(position)].id
              << " points=" << committed.remaining_points << " PASS\n";
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply repository root");
        const std::filesystem::path repo(argv[1]);
        AssetCatalog assets(repo / ".local-inputs/windows-source-clock-v19-preview-9/assets");
        std::string error;
        OriginalPropertyDatabase properties;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        OriginalMeleeBindings bindings;
        check(bindings.load(AssetCatalog(repo / ".local-inputs/windows-melee-bindings"),
                  "original-melee-bindings.xml", error), error);

        const auto skill_records = assets.read("original-cache/data/pydata/skills_pyarray.bin");
        const auto skill_names = assets.read("original-cache/data/pydata/skills_pyarraynames.bin");
        const auto skill_schema = assets.read("original-cache/data/pydata/skills_pystructnames.bin");
        dh2::data::SkillTables skill_owner;
        check(skill_owner.load(bytes(skill_records), bytes(skill_names), bytes(skill_schema), error), error);
        const auto skills = skill_owner.borrow();
        const auto character_records = assets.read("original-cache/data/pydata/character_properties_pyarray.bin");
        const auto character_names = assets.read("original-cache/data/pydata/character_properties_pyarraynames.bin");
        const auto character_schema = assets.read("original-cache/data/pydata/character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        check(dh2::data::load_characters(bytes(character_records), bytes(character_names),
                  bytes(character_schema), characters, error), error);
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(properties.characters, rules, error), error);

        const auto design_bytes = assets.read("original-cache/data/pydata/design_pycst.bin");
        std::unique_ptr<dh2_script_constants, decltype(&dh2_script_constants_destroy)> design(
            dh2_script_constants_create(), &dh2_script_constants_destroy);
        check(static_cast<bool>(design), "Could not allocate source CharacterDesign constants");
        dh2_script_constants_reload receipt{};
        check(design_bytes.size() <= UINT32_MAX &&
              dh2_script_constants_load(design.get(), design_bytes.data(),
                  static_cast<std::uint32_t>(design_bytes.size()), &receipt) == 0 &&
              receipt.consumed == design_bytes.size(), "Original design_pycst was not fully loaded");
        CharacterDesignSkillCapsV1 caps;
        caps.known = true;
        caps.unlocked_difficulty = 0;
        constexpr const char* names[] = {
            "MaxSkillLevelBNormal", "MaxSkillLevelCHard", "MaxSkillLevelDVeryHard"};
        for (std::size_t i = 0; i < caps.max_skill_level.size(); ++i) {
            std::int32_t value{};
            check(dh2_script_constants_get(design.get(), "CharacterDesign", names[i], &value) == 0 &&
                  value > 0, "Original CharacterDesign skill cap missing");
            caps.max_skill_level[i] = static_cast<std::uint32_t>(value);
        }

        const ClassFixture fixtures[] = {
            {"KnightPlayerBase", "_default_warrior-mesh-skin"},
            {"RoguePlayerBase", "_default_rogue-mesh-skin"},
            {"MagePlayerBase", "_default_mage-mesh-skin"},
        };
        for (const auto& fixture : fixtures)
            run_class(fixture, assets, properties, bindings, characters,
                      skills, rules, caps);
        std::cout << "PASS same-Session source training transaction for all three base classes\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
