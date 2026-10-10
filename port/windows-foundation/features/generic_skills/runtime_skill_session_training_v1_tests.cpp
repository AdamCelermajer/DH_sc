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
#include <charconv>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2 || argc == 3 || argc == 4,
              "Supply repository root, optional fresh diagnostic profile path, and optional source level");
        const std::filesystem::path repo(argv[1]);
        int diagnostic_level = 3;
        if (argc == 4) {
            const std::string_view input(argv[3]);
            const auto parsed = std::from_chars(input.data(), input.data() + input.size(), diagnostic_level);
            check(parsed.ec == std::errc{} && parsed.ptr == input.data() + input.size() &&
                  (diagnostic_level == 3 || diagnostic_level == 6),
                  "Optional diagnostic source level must be 3 or 6");
        }
        AssetCatalog assets(repo / ".local-inputs/windows-source-clock-v19-preview-9/assets");
        std::string error;
        OriginalPropertyDatabase properties;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        OriginalMeleeBindings bindings;
        check(bindings.load(AssetCatalog(repo / ".local-inputs/windows-melee-bindings"),
                            "original-melee-bindings.xml", error), error);

        const auto skill_data = assets.read("original-cache/data/pydata/skills_pyarray.bin");
        const auto skill_names = assets.read("original-cache/data/pydata/skills_pyarraynames.bin");
        const auto skill_schema = assets.read("original-cache/data/pydata/skills_pystructnames.bin");
        dh2::data::SkillTables skill_owner;
        check(skill_owner.load(bytes(skill_data), bytes(skill_names), bytes(skill_schema), error), error);
        const auto skills = skill_owner.borrow();
        const auto character_data = assets.read("original-cache/data/pydata/character_properties_pyarray.bin");
        const auto character_names = assets.read("original-cache/data/pydata/character_properties_pyarraynames.bin");
        const auto character_schema = assets.read("original-cache/data/pydata/character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        check(dh2::data::load_characters(bytes(character_data), bytes(character_names),
                                         bytes(character_schema), characters, error), error);

        const auto class_name = std::find(characters.names.begin(), characters.names.end(), "KnightPlayerBase");
        const auto tree_field = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
        check(class_name != characters.names.end() && tree_field != characters.fields.end(),
              "Source Knight class/SkillTree metadata missing");
        const auto class_row = static_cast<std::size_t>(class_name - characters.names.begin());
        const auto tree_column = static_cast<std::size_t>(tree_field - characters.fields.begin());
        const auto list_id = characters.rows[class_row][tree_column];
        check(list_id >= 0 && static_cast<std::size_t>(list_id) < skills.lists().size(),
              "Source Knight SkillTree does not resolve");
        const auto& list = skills.lists()[static_cast<std::size_t>(list_id)];
        const auto ground_name = std::find_if(list.begin(), list.end(), [&](int row) {
            return row >= 0 && static_cast<std::size_t>(row) < skills.skill_names().size() &&
                   skills.skill_names()[static_cast<std::size_t>(row)] == "GroundSlam";
        });
        check(ground_name != list.end(), "Source Knight SkillList lacks GroundSlam");
        const auto position = static_cast<int>(ground_name - list.begin());
        const auto design_bytes = assets.read("original-cache/data/pydata/design_pycst.bin");
        std::unique_ptr<dh2_script_constants, decltype(&dh2_script_constants_destroy)> design(
            dh2_script_constants_create(), &dh2_script_constants_destroy);
        check(static_cast<bool>(design), "Could not allocate source CharacterDesign constants");
        dh2_script_constants_reload design_receipt{};
        check(design_bytes.size() <= UINT32_MAX &&
              dh2_script_constants_load(design.get(), design_bytes.data(),
                  static_cast<std::uint32_t>(design_bytes.size()), &design_receipt) == 0 &&
              design_receipt.consumed == design_bytes.size(),
              "Original design_pycst corpus did not load completely");
        CharacterDesignSkillCapsV1 caps;
        caps.known = true;
        caps.unlocked_difficulty = 0;
        constexpr const char* cap_names[] = {
            "MaxSkillLevelBNormal", "MaxSkillLevelCHard", "MaxSkillLevelDVeryHard"};
        for (std::size_t i = 0; i < caps.max_skill_level.size(); ++i) {
            std::int32_t value{};
            check(dh2_script_constants_get(design.get(), "CharacterDesign", cap_names[i], &value) == 0 &&
                  value > 0, "Original CharacterDesign skill cap missing");
            caps.max_skill_level[i] = static_cast<std::uint32_t>(value);
        }

        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan plan;
        check(build_original_combat_visual_plan(assets, bindings, "KnightPlayerBase",
                  customization, "same-session-training-test", plan, error), error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 23;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = plan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile profile;
        profile.initialIdle = {"Idle", 0, {0}};
        profile.animationOnly = true;
        profile.propertyOptions = {diagnostic_level * 256, false};
        profile.sourceAnimationClips = plan.config.clips;
        config.profiles.emplace(config.playerProfileId, profile);
        CombatSession session;
        CharacterVisual player_visual;
        ActorPopulation population;
        check(session.initialize(assets, properties, bindings, config, player_visual,
                  population, {0, 0, 0}, customization, error), error);
        check(session.actor(1) && session.world()->combat_properties(1),
              "Source Knight training Session lacks player actor/property owner");
        auto combat = *session.world()->combat_properties(1);
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(properties.characters, rules, error), error);
        const auto initial_level_raw = combat.sheets.resolved[19];
        const auto initial_points_raw = combat.sheets.resolved[157];
        check(initial_level_raw >= 0 && (initial_level_raw & 255) == 0 &&
              initial_points_raw > 0 && (initial_points_raw & 255) == 0,
              "Configured source Knight Session has no integral level/Skill_Points value");

        CharacterState state;
        state.id = "session-training-loaded-profile";
        state.name = "Knight";
        state.class_id = "KnightPlayerBase";
        state.stats.level = static_cast<std::uint32_t>(initial_level_raw >> 8);
        state.source_points_known = true;
        state.source_skill_slots_known = true;
        state.source_skill_points = static_cast<std::uint32_t>(initial_points_raw >> 8);
        for (const int table_id : list) {
            check(table_id >= 0 && static_cast<std::size_t>(table_id) < skills.skill_names().size(),
                  "Source Knight SkillList contains an invalid saved row");
        state.skills.push_back({skills.skill_names()[static_cast<std::size_t>(table_id)], 0});
        }
        const auto starter = std::find_if(state.skills.begin(), state.skills.end(),
            [](const SkillProgress& rank) { return rank.id == "BashDown"; });
        check(starter != state.skills.end(), "Source Knight starter row missing");
        starter->rank = 1;
        state.skill_slots.push_back({0, 0, static_cast<std::uint32_t>(starter - state.skills.begin())});
        check(state.source_skill_points > 0,
              "Source Knight does not have a point for its initial rank-0 starter grant");
        --state.source_skill_points;
        session.actor(1)->persistent_character_id = state.id;

        // Match the original source initial-grant side effect: consume one
        // point with IncSkill's property_add operation and keep that same sheet
        // under the Session World owner.
        auto view = dh2::data::property_view(rules, combat.sheets);
        check(dh2_property_add(&view, 157, -256) == 0 &&
              combat.sheets.resolved[157] ==
                  static_cast<std::int32_t>(state.source_skill_points * 256u),
              "Source initial-skill point debit did not match the saved points");
        const auto* traits = session.world()->traits(1);
        check(traits && session.world()->update_combat_properties(1, combat, *traits, error), error);
        check(session.world()->combat_properties(1)->sheets.resolved[157] ==
                  static_cast<std::int32_t>(state.source_skill_points * 256u),
              "Session did not retain exact source initial-grant point debit before training: " +
              std::to_string(session.world()->combat_properties(1)->sheets.resolved[157]));
        if (argc >= 3) {
            const std::filesystem::path diagnostic_profile(argv[2]);
            check(!std::filesystem::exists(diagnostic_profile),
                  "Refusing to overwrite an existing profile/save file");
            if (diagnostic_profile.has_parent_path())
                std::filesystem::create_directories(diagnostic_profile.parent_path());
            OriginalActorProperties prepared_level;
            check(resolve_original_actor_properties(properties.characters, properties.classes,
                      state.class_id, {initial_level_raw, true}, prepared_level, error), error);
            auto diagnostic_state = state;
            diagnostic_state.stats.health = prepared_level.health;
            diagnostic_state.stats.max_health = prepared_level.max_health;
            diagnostic_state.stats.resource = prepared_level.resource;
            diagnostic_state.stats.max_resource = prepared_level.max_resource;
            check(diagnostic_state.stats.health >= 0 &&
                  diagnostic_state.stats.health <= diagnostic_state.stats.max_health &&
                  diagnostic_state.stats.resource >= 0 &&
                  diagnostic_state.stats.resource <= diagnostic_state.stats.max_resource,
                  "Source prepared vitals are outside their authored maxima");
            check(save_character(diagnostic_profile, diagnostic_state, error), error);
            CharacterState diagnostic_roundtrip;
            check(load_character(diagnostic_profile, diagnostic_roundtrip, error), error);
            check(diagnostic_roundtrip.stats.health == prepared_level.health &&
                  diagnostic_roundtrip.stats.max_health == prepared_level.max_health &&
                  diagnostic_roundtrip.stats.resource == prepared_level.resource &&
                  diagnostic_roundtrip.stats.max_resource == prepared_level.max_resource &&
                  diagnostic_roundtrip.source_skill_points == diagnostic_state.source_skill_points &&
                  diagnostic_roundtrip.skills.size() == diagnostic_state.skills.size() &&
                  diagnostic_roundtrip.skill_slots.size() == 1 &&
                  diagnostic_roundtrip.skill_slots[0].equipment_set == 0 &&
                  diagnostic_roundtrip.skill_slots[0].slot == 0 &&
                  diagnostic_roundtrip.skill_slots[0].saved_skill_row == 0 &&
                  diagnostic_roundtrip.skills[static_cast<std::size_t>(position)].rank == 0 &&
                  std::equal(diagnostic_roundtrip.skills.begin(), diagnostic_roundtrip.skills.end(),
                      diagnostic_state.skills.begin(), [](const auto& left, const auto& right) {
                          return left.id == right.id && left.rank == right.rank;
                      }),
                  "Diagnostic profile did not retain source-refilled vitals and skill progress");
            std::cout << "diagnostic source-refilled level=" << state.stats.level
                      << " HP=" << diagnostic_state.stats.health << '/'
                      << diagnostic_state.stats.max_health << " MP="
                      << diagnostic_state.stats.resource << '/'
                      << diagnostic_state.stats.max_resource << " SkillPoints="
                      << diagnostic_state.source_skill_points << " BashDown="
                      << diagnostic_state.skills[static_cast<std::size_t>(starter - state.skills.begin())].rank
                      << " GroundSlam=" << diagnostic_state.skills[static_cast<std::size_t>(position)].rank
                      << '\n';
        }

        const auto nonce = std::chrono::steady_clock::now().time_since_epoch().count();
        const auto save_root = std::filesystem::temp_directory_path() /
            ("dh2-session-skill-training-test-" + std::to_string(nonce));
        std::filesystem::create_directories(save_root);
        const auto save_path = save_root / "profile.dhsave";
        check(save_character(save_path, state, error), error);
        CharacterState loaded;
        check(load_character(save_path, loaded, error), error);
        std::filesystem::remove(save_path);
        check(loaded.source_skill_points + 1u == static_cast<std::uint32_t>(initial_points_raw >> 8) &&
              loaded.skills[static_cast<std::size_t>(position)].rank == 0,
              "SaveStore profile load changed source training facts");
        state = std::move(loaded);
        const auto points_before_training = state.source_skill_points;

        const auto sheet_before_probe = session.world()->combat_properties(1)->sheets.resolved;
        const auto rng_before_probe = session.world()->random_state();
        bool accepted = false;
        check(probe_skill_training_in_session_v1(state, session, properties, skills, caps,
                  position, accepted, error), error);
        check(accepted && state.source_skill_points == points_before_training && state.skills[position].rank == 0 &&
              session.world()->combat_properties(1)->sheets.resolved == sheet_before_probe &&
              session.world()->random_state().seed == rng_before_probe.seed &&
              session.world()->random_state().calls == rng_before_probe.calls,
              "Source IncSkill probe mutated same-state/session owners or RNG");

        const auto before_bad_state = state;
        const auto before_bad_sheet = session.world()->combat_properties(1)->sheets.resolved;
        const auto before_bad_rng = session.world()->random_state();
        SessionSkillTrainingCommitV1 rejected_commit;
        check(!train_skill_in_session_v1(state, session, properties, skills, caps,
                  static_cast<int>(list.size()), rejected_commit, error) &&
              state.source_skill_points == before_bad_state.source_skill_points &&
              state.skills[position].rank == before_bad_state.skills[position].rank &&
              session.world()->combat_properties(1)->sheets.resolved == before_bad_sheet &&
              session.world()->random_state().seed == before_bad_rng.seed &&
              session.world()->random_state().calls == before_bad_rng.calls,
              "Out-of-list training rejection changed same-state/session owners or RNG");

        const auto before_zero_points = *session.world()->combat_properties(1);
        auto zero_points = before_zero_points;
        auto zero_view = dh2::data::property_view(rules, zero_points.sheets);
        check(dh2_property_add(&zero_view, 157,
                  -static_cast<std::int32_t>(points_before_training * 256u)) == 0 &&
              zero_points.sheets.resolved[157] == 0,
              "Could not arrange source zero-point rejection case");
        check(session.world()->update_combat_properties(1, zero_points, *traits, error), error);
        auto no_points_state = state;
        no_points_state.source_skill_points = 0;
        const auto no_points_sheet = session.world()->combat_properties(1)->sheets.resolved;
        const auto no_points_rng = session.world()->random_state();
        check(!train_skill_in_session_v1(no_points_state, session, properties, skills, caps,
                  position, rejected_commit, error) && no_points_state.source_skill_points == 0 &&
              no_points_state.skills[position].rank == state.skills[position].rank &&
              session.world()->combat_properties(1)->sheets.resolved == no_points_sheet &&
              session.world()->random_state().seed == no_points_rng.seed &&
              session.world()->random_state().calls == no_points_rng.calls,
              "Zero-point training rejection changed same-state/session owners or RNG");
        check(session.world()->update_combat_properties(1, before_zero_points, *traits, error), error);

        SessionSkillTrainingCommitV1 committed;
        check(train_skill_in_session_v1(state, session, properties, skills, caps,
                  position, committed, error), error);
        const auto* after = session.world()->combat_properties(1);
        check(after && committed.saved_skill_row == position && committed.previous_rank == 0 &&
              committed.current_rank == 1 && committed.remaining_points == points_before_training - 1u &&
              state.skills[position].rank == 1 &&
              state.source_skill_points == committed.remaining_points &&
              after->sheets.resolved[157] ==
                  static_cast<std::int32_t>(committed.remaining_points * 256u),
              "Source IncSkill did not commit one saved rank and one matching World point debit");

        auto recalculated = *after;
        check(dh2::data::recalc_properties_with_class(properties.classes, rules,
                  recalculated.sheets, error), error);
        check(recalculated.sheets.resolved[157] ==
                  static_cast<std::int32_t>(committed.remaining_points * 256u),
              "Source class/gear recalculation restored spent Skill_Points");
        check(session.world()->update_combat_properties(1, recalculated, *traits, error), error);

        check(save_character(save_path, state, error), error);
        CharacterState restored_profile;
        check(load_character(save_path, restored_profile, error), error);
        std::filesystem::remove(save_path);
        check(restored_profile.source_skill_points == committed.remaining_points &&
              restored_profile.skills[position].rank == 1,
              "Post-training SaveStore roundtrip split source rank and point debit");

        GameSave saved;
        check(capture_game_save("training-commit", 1, state, *session.world(), saved, error), error);
        session.detach_for_restore();
        check(restore_game_save(saved, "training-commit", *session.world(), state, error), error);
        check(session.rebind_after_restore(error), error);
        const auto* restored = session.world()->combat_properties(1);
        check(restored && state.source_skill_points == committed.remaining_points &&
              state.skills[position].rank == 1 &&
              restored->sheets.resolved[157] ==
                  static_cast<std::int32_t>(committed.remaining_points * 256u),
              "GameSave restore split same-Session trained rank and point ownership");
        session.detach_for_restore();
        std::filesystem::remove(save_root);
        std::cout << "PASS same-Session source IncSkill probe/commit, SaveStore and GameSave point-rank consistency\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
