#include "runtime_player_locomotion_v1.hpp"
#include "runtime_player_locomotion_program_v1.hpp"

#include "../../content_paths.hpp"
#include "../../../game-data/class_preview_setup.hpp"
#include "../../../game-data/class_tables.hpp"
#include "../../../game-data/loot_tables_v2.hpp"
#include "../../../game-data/properties.hpp"
#include "../../../script-runtime/script_constants.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::equipment_menu;
using namespace dh2::data;

namespace {
void check(bool value, const std::string& error) {
    if (!value) throw std::runtime_error(error.empty() ? "Player locomotion source projection failed" : error);
}
std::vector<std::uint8_t> read(const AssetCatalog& assets, const std::string& name) {
    try { return read_content(assets, "data/pydata/" + name); }
    catch (const std::runtime_error&) { return read_content(assets, "data/" + name); }
}
Bytes view(const std::vector<std::uint8_t>& bytes) { return {bytes.data(), bytes.size()}; }
std::int32_t find_item_id(const ItemTable& table, const std::string& name) {
    const auto found = std::find(table.identifiers.begin(), table.identifiers.end(), name);
    check(found != table.identifiers.end(), "Actual ItemTable item absent: " + name);
    return static_cast<std::int32_t>(found - table.identifiers.begin());
}
std::int32_t item_with(const ItemTable& table, const std::function<bool(const Item&)>& predicate,
                       const char* why) {
    const auto found = std::find_if(table.rows.begin(), table.rows.end(), predicate);
    check(found != table.rows.end(), std::string("Actual source ItemTable row absent: ") + why);
    return static_cast<std::int32_t>(found - table.rows.begin());
}
const RuntimePlayerLocomotionStateV1& state(const RuntimePlayerLocomotionV1& bank,
                                            const char* name) {
    const auto found = std::find_if(bank.states.begin(), bank.states.end(),
        [&](const auto& row) { return row.name == name; });
    check(found != bank.states.end(), std::string("Projected locomotion state absent: ") + name);
    return *found;
}
const RuntimePlayerLocomotionSequenceV1& sequence(
        const RuntimePlayerLocomotionStateV1& state, std::int32_t id) {
    const auto found = std::find_if(state.reachable_sequences.begin(), state.reachable_sequences.end(),
        [&](const auto& row) { return row.sequence_id == id; });
    check(found != state.reachable_sequences.end(), "Projected reachable source sequence absent");
    return *found;
}
void verify_complete_source_phase_projection(const RuntimePlayerLocomotionV1& bank,
        const AnimationTables& animations, const Dictionary& dictionary) {
    for (const auto& state_row : bank.states) {
        check(state_row.selected_sequence == state_row.base_sequence +
              (state_row.stance_variants_enabled ? bank.stance : 0),
              "Source locomotion base-plus-stance rule differs");
        for (const auto& projected : state_row.reachable_sequences) {
            check(projected.sequence_id >= 0 &&
                  static_cast<std::size_t>(projected.sequence_id) < animations.sequences.size(),
                  "Projected source sequence identity outside AnimationTables");
            const auto& source = animations.sequences[static_cast<std::size_t>(projected.sequence_id)];
            check(projected.alias == animations.sequence_names[static_cast<std::size_t>(projected.sequence_id)] &&
                  projected.loop == source.loop && projected.type == source.type &&
                  projected.steps.size() == source.steps.size(),
                  "Source sequence alias, loop/type or full phase count changed");
            for (std::size_t i = 0; i < source.steps.size(); ++i) {
                const auto& original = source.steps[i];
                const auto& phase = projected.steps[i];
                check(phase.animation_id == original.anim && phase.blend_out == original.blend_out &&
                      phase.camera == original.cam && phase.effect == original.fx &&
                      phase.redirect == original.redir && phase.sound == original.sound &&
                      phase.speed == original.speed && phase.anchor_fx == original.anchor_fx &&
                      phase.camera_direction == original.cam_dir && phase.move_go == original.move_go &&
                      phase.swoosh == original.swoosh && phase.random_camera == original.random_cam,
                      "Source animation phase metadata changed");
                if (original.redir == 1) {
                    check(phase.redirected_sequence_alias ==
                          animations.sequence_names[static_cast<std::size_t>(original.anim)] &&
                          sequence(state_row, original.anim).sequence_id == original.anim,
                          "Source redirected sequence alias/child was not retained");
                } else {
                    check(phase.clip_alias == dictionary.names[static_cast<std::size_t>(original.anim)] &&
                          phase.clip_uri == dictionary.values[static_cast<std::size_t>(original.anim)],
                          "Source clip dictionary alias or URI changed");
                }
            }
        }
    }
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Caller-supplied original asset root required");
        AssetCatalog assets(argv[1]);
        std::string error;

        CharacterTable characters;
        const auto char_data = read(assets, "character_properties_pyarray.bin");
        const auto char_names = read(assets, "character_properties_pyarraynames.bin");
        const auto char_fields = read(assets, "character_properties_pystructnames.bin");
        check(load_characters(view(char_data), view(char_names), view(char_fields), characters, error), error);
        ClassTables classes;
        const auto class_data = read(assets, "character_classes_pyarray.bin");
        const auto class_names = read(assets, "character_classes_pyarraynames.bin");
        const auto class_fields = read(assets, "character_classes_pystructnames.bin");
        check(load_classes(view(class_data), view(class_names), view(class_fields), classes, error), error);
        PropertyRules rules;
        check(load_property_rules(characters, rules, error), error);
        LootTablesV2 loot;
        const auto loot_data = read(assets, "loot_table_pyarray.bin");
        const auto loot_names = read(assets, "loot_table_pyarraynames.bin");
        const auto loot_fields = read(assets, "loot_table_pystructnames.bin");
        check(loot.load(view(loot_data), view(loot_names), view(loot_fields), error), error);

        const auto animation_data = read(assets, "animations_pyarray.bin");
        const auto animation_names = read(assets, "animations_pyarraynames.bin");
        const auto animation_fields = read(assets, "animations_pystructnames.bin");
        const auto clip_values = read(assets, "animations_dictionary_pyarray.bin");
        const auto clip_names = read(assets, "animations_dictionary_pyarraynames.bin");
        Dictionary dictionary;
        check(load_dictionary(view(clip_names), view(clip_values), dictionary, error), error);
        AnimationTables animations;
        check(load_animation_tables(view(animation_data), view(animation_names),
              view(animation_fields), dictionary, animations, error), error);
        std::array<ClassPreviewDefinition, 3> classes_from_source;
        check(class_preview_definitions(characters, classes, rules, loot.borrow(), animations,
              dictionary, classes_from_source, error), error);
        check(classes_from_source[0].animation_table == 48 &&
              classes_from_source[1].animation_table == 50 &&
              classes_from_source[2].animation_table == 49,
              "Warrior/Rogue/Mage AnimTable IDs differ from actual Character properties");

        const auto constants_bytes = read(assets, "animations_pycst.bin");
        auto* pydata = dh2_script_constants_create();
        check(pydata != nullptr, "PyDataConstants allocation failed");
        dh2_script_constants_reload reload{};
        check(dh2_script_constants_load(pydata, constants_bytes.data(),
              static_cast<std::uint32_t>(constants_bytes.size()), &reload) == 0 &&
              reload.consumed == constants_bytes.size(), "Actual animations_pycst.bin load failed");
        RuntimePlayerLocomotionConstantLookupV1 lookup = [pydata](const char* group, const char* key,
                std::int32_t& value, std::string& message) {
            if (dh2_script_constants_get(pydata, group, key, &value) != 0) {
                message = std::string("actual PyDataConstants lookup failed: ") + group + "." + key;
                return false;
            }
            return true;
        };
        RuntimePlayerLocomotionConstantsV1 constants;
        check(load_runtime_player_locomotion_constants_v1(lookup, constants, error), error);
        check(constants.stanced_list_mask == 210 && constants.stance_count == 5 &&
              constants.idle_stanced_bit == 2 && constants.walk_stanced_bit == 16 &&
              constants.run_stanced_bit == 32,
              "Same-source Android locomotion stance constants differ");

        const auto& items = loot.borrow().items();
        const auto sword = find_item_id(items, "Longsword01");
        const auto dagger = find_item_id(items, "Dagger01");
        const auto staff = find_item_id(items, "Staff01");
        const auto bow = item_with(items, [](const Item& row) { return row.record.words[37] == 4; }, "bow damage category");
        const auto two_hander = item_with(items, [](const Item& row) {
            return row.record.words[26] == -4 && row.record.words[22] != 4 &&
                   row.record.words[22] != 5 && row.record.words[37] != 4 && row.record.words[37] != 5;
        }, "non-staff, non-bow two-handed weapon");
        const auto shield = item_with(items, [](const Item& row) { return row.record.words[22] == 6; }, "shield item type");

        auto resolve = [&](std::int32_t table, std::int32_t main, std::int32_t off,
                           std::int32_t flag = 0) {
            RuntimePlayerLocomotionV1 result;
            check(resolve_runtime_player_locomotion_v1(table, items, main, off, flag, constants,
                  animations, dictionary, result, error), error);
            check(result.states.size() == 3, "Source locomotion omitted Idle/Walk/Run");
            verify_complete_source_phase_projection(result, animations, dictionary);
            return result;
        };
        auto verify_program = [&](const RuntimePlayerLocomotionV1& source,
                                  const CharacterVisualConfig& base_visual,
                                  const char* label) {
            RuntimePlayerLocomotionProgramV1 program;
            check(build_runtime_player_locomotion_program_v1(assets, source, animations,
                  dictionary, base_visual, "source-locomotion-test", program, error), error);
            check(program.plan.stateNames.size() == 3 && program.plan.sequences.size() == 3,
                  "Locomotion program omitted an actual source state");
            for (const auto& projected_state : source.states) {
                const auto* sequence_plan = program.plan.sequence(projected_state.name, 0);
                check(sequence_plan && sequence_plan->id == projected_state.selected_sequence,
                      "Combat visual plan did not retain selected source sequence identity");
                const auto& root_source = animations.sequences[static_cast<std::size_t>(sequence_plan->id)];
                check(sequence_plan->loop == root_source.loop && sequence_plan->type == root_source.type &&
                      program.policies.at(sequence_plan->id).loop == root_source.loop &&
                      program.policies.at(sequence_plan->id).type == root_source.type &&
                      program.policies.at(sequence_plan->id).name ==
                          animations.sequence_names[static_cast<std::size_t>(sequence_plan->id)],
                      "Locomotion plan/policy changed source Loop, Type or sequence alias");
                check(!sequence_plan->phases.empty(), "Selected source locomotion sequence has no phase leaves");
                for (const auto& phase : sequence_plan->phases) {
                    check(!phase.sourcePath.empty() && phase.redirect == 0 && phase.has_visual() &&
                          phase.speed > 0 && program.plan.clipRates.at(phase.clipName) == phase.speed,
                          "Named locomotion leaf lost its source path, rate or clip binding");
                    check(assets.resolve(phase.resolvedPath).has_filename(),
                          "Named locomotion URI did not resolve inside the caller AssetCatalog");
                    const auto named = std::find_if(program.named_clips.begin(), program.named_clips.end(),
                        [&](const auto& clip) { return clip.named_alias == phase.clipName; });
                    check(named != program.named_clips.end() && named->source_uri == phase.sourceUri &&
                          named->resolved_path == phase.resolvedPath && named->speed == phase.speed &&
                          named->blend_out == phase.blendOut && named->move_go == phase.moveGO,
                          "Named locomotion clip receipt differs from its source phase");
                }
            }
            check(program.steps.size() >= program.named_clips.size(),
                  "Actual typed AnimationStep graph was not retained for source event consumers");
            std::cout << "PASS " << label << " source locomotion plan/policies/AssetCatalog clip bank\n";
            return program;
        };
        for (std::size_t i = 0; i < classes_from_source.size(); ++i) {
            const auto table = classes_from_source[i].animation_table;
            const auto bare = resolve(table, -1, -1);
            check(bare.stance == 0 && state(bare, "Idle").selected_sequence == 262 &&
                  state(bare, "Walk").selected_sequence == 280 &&
                  state(bare, "Run").selected_sequence == 271,
                  "Bare-handed stance clamp or Android state mask differs");
            const auto standard = resolve(table, sword, -1);
            check(standard.stance == 0, "Actual one-handed main weapon source stance differs");
            CharacterVisualConfig visual;
            const auto standard_program = verify_program(standard, visual, "ordinary weapon");
            check(standard_program.plan.sequence("Idle", 0)->id == state(standard, "Idle").selected_sequence,
                  "Ordinary weapon idle program used a different source root");
            const auto dual = resolve(table, sword, dagger);
            check(dual.stance == 2 && state(dual, "Idle").selected_sequence == 264 &&
                  state(dual, "Walk").selected_sequence == 282 &&
                  state(dual, "Run").selected_sequence == 271,
                  "Actual offhand weapon stance or state-specific stancing mask differs");
            const auto dual_program = verify_program(dual, visual, "dual wield");
            check(dual_program.plan.sequence("Idle", 0)->id == state(dual, "Idle").selected_sequence,
                  "Dual wield idle program did not select its actual stance sequence");
            const auto caster = resolve(table, staff, -1);
            check(caster.stance == 3 && state(caster, "Idle").selected_sequence == 265 &&
                  state(caster, "Walk").selected_sequence == 283 &&
                  state(caster, "Run").selected_sequence == 271,
                  "Actual ItemTable staff stance priority differs");
            const auto staff_program = verify_program(caster, visual, "staff");
            check(staff_program.plan.sequence("Idle", 0)->id == state(caster, "Idle").selected_sequence,
                  "Staff idle program did not select its actual stance sequence");
            const auto ranger = resolve(table, bow, -1);
            check(ranger.stance == 4 && state(ranger, "Idle").selected_sequence == 266 &&
                  state(ranger, "Walk").selected_sequence == 284 &&
                  state(ranger, "Run").selected_sequence == 271,
                  "Actual ItemTable bow stance priority differs");
            const auto two_handed = resolve(table, two_hander, -1, 0);
            check(two_handed.stance == 1, "Actual ItemTable two-hander stance differs");
            const auto shielded = resolve(table, sword, shield);
            check(shielded.stance == 0, "Original shield offhand was misclassified as dual wielding");
            check(state(dual, "Idle").reachable_sequences.front().sequence_id == 264,
                  "Full Idle source sequence graph was not rooted at the selected dual-wield sequence");
        }

        RuntimePlayerLocomotionV1 preserved;
        check(resolve_runtime_player_locomotion_v1(classes_from_source[0].animation_table, items,
              sword, -1, 0, constants, animations, dictionary, preserved, error), error);
        const auto old_stance = preserved.stance;
        const auto old_state_count = preserved.states.size();
        check(!resolve_runtime_player_locomotion_v1(classes_from_source[0].animation_table, items,
              static_cast<std::int32_t>(items.rows.size()), -1, 0, constants, animations,
              dictionary, preserved, error) && error.find("ItemTable") != std::string::npos &&
              preserved.stance == old_stance && preserved.states.size() == old_state_count,
              "Invalid source item id was not rejected atomically");
        auto incomplete_dictionary = dictionary;
        incomplete_dictionary.values.clear();
        check(!resolve_runtime_player_locomotion_v1(classes_from_source[0].animation_table, items,
              sword, -1, 0, constants, animations, incomplete_dictionary, preserved, error) &&
              preserved.stance == old_stance && preserved.states.size() == old_state_count,
              "Missing source clip dictionary leaf was not rejected atomically");
        RuntimePlayerLocomotionConstantsV1 unchanged{9, 9, 8, 4, 2};
        const auto before = unchanged;
        auto missing_constant = lookup;
        missing_constant = [lookup](const char* group, const char* key, std::int32_t& value,
                                    std::string& message) {
            if (std::string(group) == "AnimStancedAnim" && std::string(key) == "SL_RUN") {
                message = "missing actual SL_RUN provider";
                return false;
            }
            return lookup(group, key, value, message);
        };
        check(!load_runtime_player_locomotion_constants_v1(missing_constant, unchanged, error) &&
              error.find("SL_RUN") != std::string::npos && unchanged.stance_count == before.stance_count &&
              unchanged.run_stanced_bit == before.run_stanced_bit,
              "Missing source stance constant was not explicit and atomic");

        const auto valid_for_failure = resolve(classes_from_source[0].animation_table, sword, -1);
        RuntimePlayerLocomotionProgramV1 unchanged_program;
        unchanged_program.plan.roleId = "preserved-program";
        auto broken_bank = valid_for_failure;
        bool corrupted_direct_phase = false;
        for (auto& state_row : broken_bank.states) {
            for (auto& source_sequence : state_row.reachable_sequences) {
                for (auto& source_step : source_sequence.steps) {
                    if (source_step.redirect == 0 && source_step.animation_id >= 0) {
                        source_step.clip_uri = "missing/alias.bdae";
                        corrupted_direct_phase = true;
                        break;
                    }
                }
                if (corrupted_direct_phase) break;
            }
            if (corrupted_direct_phase) break;
        }
        check(corrupted_direct_phase, "Source locomotion graph has no direct clip phase to corrupt");
        check(!build_runtime_player_locomotion_program_v1(assets, broken_bank, animations, dictionary,
              CharacterVisualConfig{}, "source-locomotion-test", unchanged_program, error) &&
              unchanged_program.plan.roleId == "preserved-program",
              "Divergent source clip graph was accepted or changed output non-atomically");

        dh2_script_constants_destroy(pydata);
        std::cout << "PASS actual ItemTable GetAnimStance, typed AnimTable source programs, exact URI resolution, "
                     "named clip banks and atomic failures\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
