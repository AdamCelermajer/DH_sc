#include "runtime_player_profile_attack_bank_v1.hpp"

#include "../equipment/runtime_player_locomotion_v1.hpp"
#include "../../actor_profiles.hpp"
#include "../../content_paths.hpp"
#include "../../../script-runtime/script_constants.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <set>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::combat;
using namespace dh::foundation::equipment_menu;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read(const AssetCatalog& assets, const std::string& uri) {
    return read_content(assets, uri);
}

dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value) {
    return {value.data(), value.size()};
}

const OriginalCombatSequencePlan& sequence(const OriginalCombatVisualPlan& plan,
                                           const std::string& state) {
    const auto* value = plan.sequence(state, 0);
    check(value != nullptr, "Attack bank is missing source state " + state);
    return *value;
}

void check_phase_asset(const AssetCatalog& assets, const OriginalCombatPhase& phase) {
    check(!phase.sourceUri.empty() && !phase.resolvedPath.empty() && phase.has_visual(),
          "Source attack leaf did not retain its AnimDict resource");
    check(std::filesystem::exists(assets.root() / phase.resolvedPath),
          "Source attack leaf resolved to a missing profile asset: " + phase.sourceUri);
    check(std::isfinite(phase.speed) && phase.speed > 0 && phase.blendOut == 100 &&
          phase.moveGO == 1, "Source attack leaf lost authored rate/blend/MoveGO");
}

} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply unified original asset root");
        AssetCatalog assets(argv[1]);
        std::string error;

        const auto dictionary_names = read(assets, "data/pydata/animations_dictionary_pyarraynames.bin");
        const auto dictionary_values = read(assets, "data/pydata/animations_dictionary_pyarray.bin");
        dh2::data::Dictionary clips;
        check(dh2::data::load_dictionary(bytes(dictionary_names), bytes(dictionary_values), clips, error), error);
        const auto sequence_records = read(assets, "data/pydata/animations_pyarray.bin");
        const auto sequence_names = read(assets, "data/pydata/animations_pyarraynames.bin");
        const auto sequence_fields = read(assets, "data/pydata/animations_pystructnames.bin");
        dh2::data::AnimationTables animations;
        check(dh2::data::load_animation_tables(bytes(sequence_records), bytes(sequence_names),
              bytes(sequence_fields), clips, animations, error), error);
        const auto item_records = read(assets, "data/pydata/loot_table_pyarray.bin");
        const auto item_names = read(assets, "data/pydata/loot_table_pyarraynames.bin");
        const auto item_fields = read(assets, "data/pydata/loot_table_pystructnames.bin");
        dh2::data::ItemTable items;
        check(dh2::data::load_items(bytes(item_records), bytes(item_names),
              bytes(item_fields), items, error), error);

        ActorProfileLibrary profiles;
        check(profiles.load(assets, "actor-profiles-v2.xml", error), error);
        auto* constants = dh2_script_constants_create();
        check(constants != nullptr, "Original animation constants allocation failed");
        const auto constants_bytes = read(assets, "data/animations_pycst.bin");
        dh2_script_constants_reload reload{};
        check(dh2_script_constants_load(constants, constants_bytes.data(),
              static_cast<std::uint32_t>(constants_bytes.size()), &reload) == 0,
              "Original source animation constants failed to load");
        RuntimePlayerLocomotionConstantsV1 stance_constants;
        const bool constants_loaded = load_runtime_player_locomotion_constants_v1(
            [&](const char* group, const char* key, std::int32_t& value, std::string& message) {
                if (dh2_script_constants_get(constants, group, key, &value) != 0) {
                    message = std::string("Missing source stance constant ") + group + "." + key;
                    return false;
                }
                return true;
            }, stance_constants, error);
        dh2_script_constants_destroy(constants);
        check(constants_loaded, error);
        check((stance_constants.stanced_list_mask & 0xc0) == 0xc0,
              "Recovered source mask does not enable attack/static weapon stance offsets");

        const auto item_id = [&](const char* name) {
            const auto id = dh2::data::item_id(items, name);
            check(id >= 0, std::string("Missing authored ItemTable row ") + name);
            return id;
        };

        const struct Case {
            const char* profile;
            const char* skin;
            const char* main;
            const char* off;
            std::int32_t character_flag;
            std::int32_t expected_table;
            std::int32_t expected_stance;
            std::int32_t expected_moving;
            std::int32_t expected_static;
            const char* moving_name;
            const char* static_name;
            const char* group_name;
            std::size_t phases_per_state;
        } cases[] = {
            {"RoguePlayerBase", "_default_rogue-mesh-skin", "Dagger01", "Dagger01", 4,
             50, 2, 250, 245, "Human_Attack_02_Dual", "Human_AttackStatic_02_Dual",
             "DualAttack", 12},
            {"MagePlayerBase", "_default_mage-mesh-skin", "Staff01", nullptr, 0,
             49, 3, 251, 246, "Human_Attack_03_Staff", "Human_AttackStatic_03_Staff",
             "StaffAttack", 1}
        };

        for (const auto& test : cases) {
            const auto* profile = profiles.find(test.profile);
            check(profile != nullptr, std::string("Missing actor profile ") + test.profile);
            check(std::stoi(profile->animation_table) == test.expected_table,
                  "Class actor profile table ID differs from the original source row");

            RuntimePlayerLocomotionV1 equipped;
            check(resolve_runtime_player_locomotion_v1(test.expected_table, items,
                  item_id(test.main), test.off ? item_id(test.off) : -1,
                  test.character_flag, stance_constants, animations, clips,
                  equipped, error), error);
            check(equipped.stance == test.expected_stance,
                  std::string(test.profile) + " equipment did not resolve through source stance rules");

            ActorCustomization customization;
            customization.skin_id_contains = test.skin;
            customization.expected_controller_count = 4;
            customization.allow_missing_animation_targets = true;
            CharacterVisualConfig same_profile_config;
            check(make_visual_config(assets, *profile, customization,
                  same_profile_config, error), error);
            OriginalCombatVisualPlan same_profile_visual;
            same_profile_visual.profileId = profile->id;
            same_profile_visual.roleId = std::string("profile-attack-bank-") + profile->id;
            same_profile_visual.config = same_profile_config;

            RuntimePlayerProfileAttackBankPlanV1 plan;
            check(plan_runtime_player_profile_attack_bank_v1(assets, *profile, equipped,
                  stance_constants.stanced_list_mask, animations, clips,
                  same_profile_visual, same_profile_visual.roleId, plan, error), error);
            check(plan.source_profile_id == profile->id &&
                  plan.animation_table == test.expected_table &&
                  plan.stance == test.expected_stance &&
                  plan.moving_sequence_id == test.expected_moving &&
                  plan.static_sequence_id == test.expected_static &&
                  plan.moving_stance_applied && plan.static_stance_applied,
                  std::string(test.profile) + " bank did not preserve exact source table/stance roots");
            check(plan.moving_selection.state == "Attack" &&
                  plan.static_selection.state == "AttackStatic" &&
                  plan.moving_selection.variant == 0 && plan.static_selection.variant == 0 &&
                  plan.moving_selection.group_path.empty() && plan.static_selection.group_path.empty(),
                  "Attack bank invented request state, variant or group selection");
            check(plan.bank.profileId == profile->id &&
                  plan.bank.config.model_path == same_profile_config.model_path &&
                  plan.bank.config.skin_id_contains == test.skin,
                  "Attack bank replaced the same-profile model or class skin policy");
            const auto& moving = sequence(plan.bank, "Attack");
            const auto& stationary = sequence(plan.bank, "AttackStatic");
            check(moving.id == test.expected_moving && stationary.id == test.expected_static &&
                  moving.name == test.moving_name && stationary.name == test.static_name &&
                  moving.type == 1 && stationary.type == 1 && moving.loop == 0 && stationary.loop == 0,
                  "Attack bank root name, Type or Loop differs from source AnimTable");
            const auto check_policy = [&](std::int32_t id, const std::string& name) {
                const auto found = plan.sequence_policies.find(id);
                check(found != plan.sequence_policies.end() && found->second.id == id &&
                      found->second.name == name && found->second.type == 1 &&
                      found->second.loop == 0,
                      "Attack bank redirect policy differs from the source AnimTable");
            };
            check_policy(test.expected_moving, test.moving_name);
            check_policy(test.expected_static, test.static_name);
            check(moving.phases.size() == test.phases_per_state &&
                  stationary.phases.size() == test.phases_per_state,
                  "Attack bank did not preserve the authored ordered root groups/leaves");
            check(!plan.requires_original_rng_start,
                  "Type1 attack bank incorrectly requests a Type2 start draw");
            for (const auto& phase : moving.phases) check_phase_asset(assets, phase);
            for (const auto& phase : stationary.phases) check_phase_asset(assets, phase);

            if (std::string(test.profile) == "RoguePlayerBase") {
                for (int group = 1; group <= 4; ++group) {
                    check_policy(166 + group, "DualAttack0" + std::to_string(group));
                    check_policy(170 + group, "DualAttackStatic0" + std::to_string(group));
                }
                check(moving.phases.front().sourcePath == std::vector<std::size_t>{0, 0} &&
                      moving.phases.back().sourcePath == std::vector<std::size_t>{3, 2} &&
                      moving.phases.front().sourceUri.find("prince_dual_pre_combo_01_moving.bdae") != std::string::npos &&
                      moving.phases.back().sourceUri.find("prince_dual_post_combo_04_moving.bdae") != std::string::npos,
                      "Rogue dual-wield moving sequence lost its four authored ordered groups");
                check(stationary.phases.front().sourceUri.find("prince_dual_pre_combo_01.bdae") != std::string::npos &&
                      stationary.phases.back().sourceUri.find("prince_dual_combo_04_to_idle.bdae") != std::string::npos,
                      "Rogue dual-wield static sequence differs from authored source clips");
            } else {
                check_policy(651, "StaffAttack01");
                check_policy(654, "StaffAttackStatic01");
                check(moving.phases.front().sourcePath == std::vector<std::size_t>{0, 0} &&
                      stationary.phases.front().sourcePath == std::vector<std::size_t>{0, 0} &&
                      moving.phases.front().sourceUri.find("skill_dh2_prince_mage_wand_attack.bdae") != std::string::npos &&
                      stationary.phases.front().sourceUri == moving.phases.front().sourceUri,
                      "Mage Staff attack did not use the source-authored shared wand clip");
            }

            // Session's pre-load union operation must be able to merge this
            // profile bank without replacing any existing clip or alias.
            CharacterVisualConfig union_config = same_profile_config;
            std::set<std::string> aliases;
            for (const auto& clip : union_config.clips) aliases.insert(clip.first);
            for (const auto& clip : plan.bank.config.clips) {
                const auto existing = std::find_if(union_config.clips.begin(), union_config.clips.end(),
                    [&](const auto& item) { return item.first == clip.first; });
                if (existing != union_config.clips.end())
                    check(existing->second == clip.second,
                          "Pre-load bank union changes an existing same-profile clip alias");
                else {
                    check(aliases.insert(clip.first).second,
                          "Pre-load bank union creates a duplicate named clip");
                    union_config.clips.push_back(clip);
                }
            }
            check(union_config.model_path == same_profile_config.model_path &&
                  union_config.skin_id_contains == same_profile_config.skin_id_contains &&
                  union_config.clips.size() > same_profile_config.clips.size(),
                  "Attack bank union did not add clips to the same class visual configuration");

            // A true same-profile guard prevents accidentally passing the
            // Warrior/Knight plan as a fallback for Rogue or Mage.
            if (std::string(test.profile) == "RoguePlayerBase") {
                RuntimePlayerProfileAttackBankPlanV1 unchanged;
                unchanged.source_profile_id = "preserve-on-error";
                auto knight_visual = same_profile_visual;
                knight_visual.profileId = "KnightPlayerBase";
                check(!plan_runtime_player_profile_attack_bank_v1(assets, *profile, equipped,
                      stance_constants.stanced_list_mask, animations, clips,
                      knight_visual, same_profile_visual.roleId, unchanged, error) &&
                      unchanged.source_profile_id == "preserve-on-error",
                      "Knight visual fallback was accepted or failure replaced output");
                auto mismatched_equipment = equipped;
                ++mismatched_equipment.animation_table;
                check(!plan_runtime_player_profile_attack_bank_v1(assets, *profile,
                      mismatched_equipment, stance_constants.stanced_list_mask, animations,
                      clips, same_profile_visual, same_profile_visual.roleId, unchanged, error),
                      "Equipment stance from a different animation table was accepted");
            }
        }

        std::cout << "PASS source Rogue dual roots 250/245 with 12 ordered leaves each; Mage Staff roots 251/246 with exact wand clip; same-profile pre-load bank union and fallback rejection\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "FAIL: " << failure.what() << '\n';
        return 1;
    }
}
