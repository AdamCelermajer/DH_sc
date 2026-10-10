#include "runtime_skill_animation_bank_v1.hpp"
#include "runtime_skill_progression_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"
#include "../../../game-data/animation_tables.hpp"
#include "../../../game-data/skill_tables.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
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

std::int32_t signed_word(std::uint32_t raw) {
    std::int32_t result{};
    std::memcpy(&result, &raw, sizeof(result));
    return result;
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Repository root required");
        const std::filesystem::path repo(argv[1]);
        const auto asset_root = repo / ".local-inputs/windows-source-clock-v19-preview-9/assets";
        AssetCatalog assets(asset_root);
        std::string error;

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
        const auto class_it = std::find(characters.names.begin(), characters.names.end(),
                                        "KnightPlayerBase");
        const auto tree_it = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
        check(class_it != characters.names.end() && tree_it != characters.fields.end(),
              "Original Knight SkillTree metadata is missing");
        const auto class_row = static_cast<std::size_t>(class_it - characters.names.begin());
        const auto tree_column = static_cast<std::size_t>(tree_it - characters.fields.begin());
        const auto list_id = characters.rows[class_row][tree_column];
        check(list_id >= 0 && static_cast<std::size_t>(list_id) < skills.lists().size(),
              "Original Knight SkillTree does not identify a source SkillList");
        const auto& list = skills.lists()[static_cast<std::size_t>(list_id)];
        check(!list.empty() && list[0] >= 0 &&
              static_cast<std::size_t>(list[0]) < skills.skills().size() &&
              static_cast<std::size_t>(list[0]) < skills.skill_names().size(),
              "Original Knight source row0 is invalid");
        const auto root_id = signed_word(skills.skills()[static_cast<std::size_t>(list[0])].scalar.words[1]);
        check(root_id >= 0, "Original Knight source row0 has no authored animation root");

        const auto animation_data = assets.read("original-cache/data/pydata/animations_pyarray.bin");
        const auto animation_names = assets.read("original-cache/data/pydata/animations_pyarraynames.bin");
        const auto animation_schema = assets.read("original-cache/data/pydata/animations_pystructnames.bin");
        const auto dictionary_values = assets.read(
            "original-cache/data/pydata/animations_dictionary_pyarray.bin");
        AssetCatalog dictionary_assets(repo / ".local-inputs/actors");
        const auto dictionary_names = dictionary_assets.read("animations_dictionary_pyarraynames.bin");
        dh2::data::Dictionary dictionary;
        check(dh2::data::load_dictionary(bytes(dictionary_names), bytes(dictionary_values),
                  dictionary, error), error);
        dh2::data::AnimationTables animations;
        check(dh2::data::load_animation_tables(bytes(animation_data), bytes(animation_names),
                  bytes(animation_schema), dictionary, animations, error), error);

        OriginalMeleeBindings melee;
        check(melee.load(AssetCatalog(repo / ".local-inputs/windows-melee-bindings"),
                  "original-melee-bindings.xml", error), error);
        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan base;
        check(build_original_combat_visual_plan(assets, melee, "KnightPlayerBase",
                  customization, "rank-zero-preload-test", base, error), error);
        auto same_visual = base.config;
        same_visual.motion_node_id = "auto";
        same_visual.consume_root_motion = true;

        CharacterState state;
        state.id = "unlearned-current-class-root-preload";
        state.class_id = "KnightPlayerBase";
        state.stats.level = 1;
        state.source_skill_slots_known = true;
        for (const auto table_id : list) {
            check(table_id >= 0 && static_cast<std::size_t>(table_id) < skills.skill_names().size(),
                  "Original Knight SkillList contains an invalid SkillTable row");
            state.skills.push_back({skills.skill_names()[static_cast<std::size_t>(table_id)], 0});
        }
        state.skill_slots.push_back({0, 0, 0});

        RuntimeSkillAnimationBankRequestV1 request;
        request.character = &state;
        request.characters = &characters;
        request.skills = skills;
        request.assets = &assets;
        request.animations = &animations;
        request.animation_dictionary = &dictionary;
        request.same_actor_visual = &same_visual;
        request.actor_role = "rank-zero-preload-test";
        request.equipment_set = 0;
        request.preload_current_class_roots = true;

        RuntimeSkillAnimationBankV1 bank;
        check(build_runtime_skill_animation_bank_v1(request, base, bank, error), error);
        const auto class_root = std::find_if(bank.class_skill_roots.begin(), bank.class_skill_roots.end(),
            [&](const RuntimeSkillAnimationClassRootV1& root) {
                return root.active_skill_list_id == list_id && root.class_skill_position == 0 &&
                    root.skill_table_id == list[0];
            });
        check(bank.class_roots_preloaded && bank.active_skill_list_id == list_id &&
              !bank.hotbar[0] && class_root != bank.class_skill_roots.end() &&
              class_root->loaded && class_root->animation_sequence_id == root_id &&
              class_root->source_skill_name == skills.skill_names()[static_cast<std::size_t>(list[0])] &&
              state.skills[0].rank == 0 && state.skill_slots[0].saved_skill_row == 0,
              "Rank-zero saved slot did not preload its exact source class root without becoming an active hotbar entry");

        RuntimeSkillAnimationSlotV1 resolved;
        check(!resolve_runtime_skill_animation_slot_v1(state, characters, skills, 0, 0,
                  bank, resolved, error) && state.skills[0].rank == 0,
              "Rank-zero current row became cast-resolvable before the source grant");

        CharacterDesignSkillCapsV1 source_caps{true, 0, {10, 15, 20}};
        state.stats.level = 10;
        state.source_points_known = true;
        state.source_skill_points = 1;
        check(train_skill_v1(state, characters, skills, source_caps, 0, error), error);
        check(state.skills[0].rank == 1 && state.source_skill_points == 0,
              "Source progression did not apply the first learned rank through its source gates");
        check(resolve_runtime_skill_animation_slot_v1(state, characters, skills, 0, 0,
                  bank, resolved, error), error);
        check(resolved.skill.saved_skill_row == 0 && resolved.skill.saved_rank == 1 &&
              resolved.skill.animation_sequence_id == root_id &&
              resolved.selection_state == class_root->selection_state,
              "Post-grant fresh slot resolution did not select the already preloaded source root");

        auto no_preload = request;
        no_preload.preload_current_class_roots = false;
        RuntimeSkillAnimationBankV1 rejected_bank;
        rejected_bank.character_state_id = "unchanged-on-failure";
        CharacterState still_unlearned = state;
        still_unlearned.skills[0].rank = 0;
        still_unlearned.source_skill_points = 1;
        no_preload.character = &still_unlearned;
        check(!build_runtime_skill_animation_bank_v1(no_preload, base, rejected_bank, error) &&
              rejected_bank.character_state_id == "unchanged-on-failure" &&
              still_unlearned.skills[0].rank == 0,
              "No-preload rank-zero hotbar cast was admitted or failed bank output was partially committed");

        CharacterState mismatched = still_unlearned;
        mismatched.skills[0].id = "not-an-authored-source-row";
        request.character = &mismatched;
        RuntimeSkillAnimationBankV1 rejected_mismatch;
        check(!build_runtime_skill_animation_bank_v1(request, base, rejected_mismatch, error) &&
              rejected_mismatch.class_id.empty() && mismatched.skills[0].rank == 0,
              "Mismatched rank-zero saved row was accepted as a current-class root");

        std::cout << "runtime_skill_animation_bank_rank0_v1_tests PASS: source rank0 slot omitted from active hotbar; exact SkillList root preloaded; source training then resolves row0/rank1; no-preload and mismatched rows reject\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "runtime_skill_animation_bank_rank0_v1_tests: " << error.what() << '\n';
        return 1;
    }
}
