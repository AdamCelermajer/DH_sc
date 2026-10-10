#include "runtime_player_combo_chain_v1.hpp"

#include "../equipment/runtime_player_locomotion_v1.hpp"
#include "../../combat_session.hpp"
#include "../../camera.hpp"
#include "../../content_paths.hpp"
#include "../../../script-runtime/script_constants.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <set>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::combat;
using namespace dh::foundation::equipment_menu;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read(const AssetCatalog& assets, const std::string& name) {
    return read_content(assets, name);
}

dh2::data::Bytes bytes(const std::vector<std::uint8_t>& data) {
    return {data.data(), data.size()};
}

bool project_visible(const CameraPose& camera, Vec3 point, float aspect) {
    const auto view = cameraViewMatrix(camera);
    const auto projection = cameraProjectionMatrix(camera.verticalFovDegrees, aspect, .5f, 100000.f);
    const float world[]{point.x, point.y, point.z, 1};
    float eye[4]{}, clip[4]{};
    for (unsigned row = 0; row < 4; ++row)
        for (unsigned col = 0; col < 4; ++col)
            eye[row] += view[col * 4 + row] * world[col];
    for (unsigned row = 0; row < 4; ++row)
        for (unsigned col = 0; col < 4; ++col)
            clip[row] += projection[col * 4 + row] * eye[col];
    if (!std::isfinite(clip[3]) || clip[3] <= 0) return false;
    const float x = clip[0] / clip[3], y = clip[1] / clip[3], z = clip[2] / clip[3];
    return x >= -1 && x <= 1 && y >= -1 && y <= 1 && z >= -1 && z <= 1;
}

} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply unified original asset root");
        AssetCatalog assets(argv[1]);
        std::string error;

        const auto clip_names = read(assets, "data/pydata/animations_dictionary_pyarraynames.bin");
        const auto clip_values = read(assets, "data/pydata/animations_dictionary_pyarray.bin");
        dh2::data::Dictionary clips;
        check(dh2::data::load_dictionary(bytes(clip_names), bytes(clip_values), clips, error), error);
        const auto animation_records = read(assets, "data/pydata/animations_pyarray.bin");
        const auto animation_names = read(assets, "data/pydata/animations_pyarraynames.bin");
        const auto animation_fields = read(assets, "data/pydata/animations_pystructnames.bin");
        dh2::data::AnimationTables animations;
        check(dh2::data::load_animation_tables(bytes(animation_records), bytes(animation_names),
              bytes(animation_fields), clips, animations, error), error);
        const auto item_records = read(assets, "data/pydata/loot_table_pyarray.bin");
        const auto item_names = read(assets, "data/pydata/loot_table_pyarraynames.bin");
        const auto item_fields = read(assets, "data/pydata/loot_table_pystructnames.bin");
        dh2::data::ItemTable items;
        check(dh2::data::load_items(bytes(item_records), bytes(item_names), bytes(item_fields), items, error), error);

        const auto constants_bytes = read(assets, "data/animations_pycst.bin");
        auto* constants = dh2_script_constants_create();
        check(constants != nullptr, "Original source constants allocation failed");
        dh2_script_constants_reload reload{};
        check(dh2_script_constants_load(constants, constants_bytes.data(),
              static_cast<std::uint32_t>(constants_bytes.size()), &reload) == 0,
              "Original source animation constants load failed");
        RuntimePlayerLocomotionConstantsV1 stance_constants;
        check(load_runtime_player_locomotion_constants_v1(
            [&](const char* group, const char* key, std::int32_t& value, std::string& message) {
                if (dh2_script_constants_get(constants, group, key, &value) != 0) {
                    message = std::string("Missing source stance constant ") + group + "." + key;
                    return false;
                }
                return true;
            }, stance_constants, error), error);
        dh2_script_constants_destroy(constants);

        OriginalPropertyDatabase properties;
        OriginalMeleeBindings bindings;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        check(bindings.load(assets, "original-melee-bindings.xml", error), error);
        ActorProfileLibrary profiles;
        check(profiles.load(assets, "actor-profiles-v2.xml", error), error);
        const auto* profile = profiles.find("KnightPlayerBase");
        check(profile != nullptr, "Original Knight player visual profile is absent");
        const auto animation_table = std::stoi(profile->animation_table);

        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan base_visual;
        check(build_original_combat_visual_plan(assets, bindings, "KnightPlayerBase",
              customization, "combo-chain-test", base_visual, error), error);

        const auto item_id = [&](const char* name) {
            const auto id = dh2::data::item_id(items, name);
            check(id >= 0, std::string("Missing source item row ") + name);
            return id;
        };
        std::int32_t bow_id = -1;
        for (std::size_t i = 0; i < items.rows.size(); ++i)
            if (items.rows[i].record.words[37] == 4) {
                bow_id = static_cast<std::int32_t>(i);
                break;
            }
        check(bow_id >= 0, "Original ItemTable has no bow-category weapon row");
        const std::pair<std::int32_t, std::int32_t> weapons[]{
            {item_id("Longsword01"), -1},
            {item_id("Longsword01"), item_id("Dagger01")},
            {item_id("Staff01"), -1},
            {bow_id, -1}
        };
        const std::int32_t expected_stances[]{0, 2, 3, 4};
        std::vector<RuntimePlayerComboChainPlanV1> static_plans;
        std::vector<RuntimePlayerComboChainPlanV1> moving_plans;
        RuntimePlayerLocomotionV1 ordinary_longsword;
        for (std::size_t i = 0; i < std::size(weapons); ++i) {
            RuntimePlayerLocomotionV1 equipped;
            check(resolve_runtime_player_locomotion_v1(animation_table, items,
                  weapons[i].first, weapons[i].second, 0, stance_constants,
                  animations, clips, equipped, error), error);
            check(equipped.stance == expected_stances[i],
                  "Actual ItemTable weapon rows resolved the wrong original stance");
            if (i == 0) ordinary_longsword = equipped;

            RuntimePlayerComboChainPlanV1 stationary, moving;
            check(plan_runtime_player_combo_chain_v1(assets, equipped,
                  stance_constants.stanced_list_mask, 3, true, animations, clips,
                  "KnightPlayerBase", base_visual.config, "player-combo-static-" + std::to_string(i),
                  stationary, error), error);
            check(plan_runtime_player_combo_chain_v1(assets, equipped,
                  stance_constants.stanced_list_mask, 4, false, animations, clips,
                  "KnightPlayerBase", base_visual.config, "player-combo-moving-" + std::to_string(i),
                  moving, error), error);
            check(stationary.state == "AttackStatic" && stationary.command_delivered &&
                  stationary.selection.state == "AttackStatic" &&
                  stationary.selection.variant == 0 && stationary.selection.group_path.empty(),
                  "Stationary player request did not preserve state/root command semantics");
            check(moving.state == "Attack" && !moving.command_delivered &&
                  moving.selection.state == "Attack" && moving.selection.variant == 0,
                  "Moving player request did not preserve source predecessor selection");
            const auto* static_base = dh2::data::animation_state(animations, animation_table, "AttackStatic");
            const auto* moving_base = dh2::data::animation_state(animations, animation_table, "Attack");
            check(static_base && moving_base, "Player source attack fields are absent");
            const auto static_base_id = static_cast<std::int32_t>(static_base - animations.sequences.data());
            const auto moving_base_id = static_cast<std::int32_t>(moving_base - animations.sequences.data());
            const auto expected_static_id = static_base_id +
                ((stance_constants.stanced_list_mask & 0x80) ? equipped.stance : 0);
            const auto expected_moving_id = moving_base_id +
                ((stance_constants.stanced_list_mask & 0x40) ? equipped.stance : 0);
            check(stationary.base_sequence_id == static_base_id &&
                  stationary.selected_sequence_id == expected_static_id,
                  "Static attack did not apply the original 0x80 stance-list gate");
            check(moving.base_sequence_id == moving_base_id &&
                  moving.selected_sequence_id == expected_moving_id,
                  "Moving attack did not apply the original 0x40 stance-list gate");
            check(stationary.visual.sequence("AttackStatic", 0)->id == expected_static_id &&
                  moving.visual.sequence("Attack", 0)->id == expected_moving_id,
                  "Generated source visuals do not use the selected AnimTable root");
            const auto contains_type2 = [](const RuntimePlayerComboChainPlanV1& plan) {
                return std::any_of(plan.sequence_policies.begin(), plan.sequence_policies.end(),
                    [](const auto& policy) { return policy.second.type == 2; });
            };
            check(stationary.requires_original_rng_start == contains_type2(stationary) &&
                  moving.requires_original_rng_start == contains_type2(moving),
                  "Type2 source start requirements were not preserved for the caller's RNG owner");
            static_plans.push_back(std::move(stationary));
            moving_plans.push_back(std::move(moving));
        }

        // The unmodified one-hand attack profile is the control: the generated
        // real AnimTable graph must keep its authored phase paths/rates/MoveGO.
        const auto* original_static = base_visual.sequence("AttackStatic", 0);
        const auto* generated_static = static_plans.front().visual.sequence("AttackStatic", 0);
        check(original_static && generated_static &&
              original_static->id == generated_static->id &&
              original_static->phases.size() == generated_static->phases.size(),
              "Generated ordinary attack root differs from the authored combat visual profile");
        for (std::size_t i = 0; i < original_static->phases.size(); ++i) {
            const auto& source = original_static->phases[i];
            const auto& generated = generated_static->phases[i];
            check(source.sourceIndices == generated.sourceIndices &&
                  source.sourceUri == generated.sourceUri && source.speed == generated.speed &&
                  source.blendOut == generated.blendOut && source.moveGO == generated.moveGO,
                  "Generated ordinary attack changed authored phase path/rate/blend/MoveGO");
        }

        CharacterVisual attack_visual;
        check(attack_visual.load(assets, static_plans.front().visual.config, error), error);
        auto visual_binding = combat_visual_binding(attack_visual);
        OriginalAttackSequenceServices sequence_services;
        sequence_services.visual = visual_binding;
        sequence_services.restart_clip = [&](const std::string& clip, std::string& message) {
            return attack_visual.restart(clip, false, message);
        };
        OriginalAttackSequence sequence;
        check(sequence.prepare(static_plans.front().visual,
              static_plans.front().sequence_policies, static_plans.front().selection,
              sequence_services, "player-combo-authored-sequence", error), error);
        check(sequence.phases().size() == original_static->phases.size() &&
              !sequence.scheduled_markers().empty(),
              "Actual source attack markers or authored phase graph were not scheduled");
        for (std::size_t i = 0; i < sequence.phases().size(); ++i) {
            check(sequence.phases()[i].rate == original_static->phases[i].speed &&
                  sequence.phases()[i].source.moveGO == original_static->phases[i].moveGO,
                  "Source attack playback changed authored rate or movement policy");
        }

        // Feed the exact stance-zero request state to one real CombatSession.
        // The actor selection is asserted separately from camera projection.
        CombatSessionConfig config;
        config.diagnosticRngSeed = 1234;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = base_visual.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile player;
        player.initialIdle = {"Idle", 0, {0}};
        player.damageMarkerNames = {"attack_mainhand"};
        player.propertyOptions = {256, true};
        player.sequenceAction = static_plans.front().selection;
        player.retainedPhaseClock = true;
        player.sourceCombo = true;
        config.profiles.emplace("KnightPlayerBase", player);
        CombatSessionProfile target;
        target.action = {"Attack", 0, {0, 1}};
        target.initialIdle = {"Idle", 0, {0}};
        target.damageMarkerNames = {"attack_mainhand"};
        target.propertyOptions = {std::nullopt, true};
        target.customization.allow_missing_animation_targets = true;
        config.profiles.emplace("Swamp_LizadMan_Type1", target);
        ActorPopulation population;
        PopulationActor placed;
        placed.profileId = "Swamp_LizadMan_Type1";
        placed.definition.stableId = 2;
        placed.definition.sourceId = "actual-player-combo-chain-test";
        placed.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
        placed.transform = placed.definition.placement;
        population.actors().push_back(std::move(placed));
        CharacterVisual session_visual;
        CombatSession session;
        check(session.initialize(assets, properties, bindings, config, session_visual,
              population, {0,0,0}, customization, error), error);
        InputActions input;
        input.targetSelect = true;
        check(session.update(0, input, {0,0,0}, 0, error), error);
        check(session.actor(1)->target_id == 2 && session.selectedactor() &&
              session.selectedactor()->id == 2,
              "Session target_id and selectedactor diverged before the combo request");

        CameraPose camera;
        camera.position = {0,0,10};
        camera.target = {0,0,0};
        camera.up = {0,1,0};
        check(!project_visible(camera, {0,-100,0}, 16.f/9.f),
              "Target fixture unexpectedly projects into the HUD viewport");

        input.targetSelect = false;
        input.attack = static_plans.front().command_delivered;
        check(session.update(0, input, {0,0,0}, 0, error), error);
        check(session.owns_pose(1), "Delivered player combo command did not start source sequence");
        std::set<std::uint32_t> root_groups;
        std::uint64_t generation = 0;
        for (const auto& boundary : session.combo_boundaries())
            if (boundary.beginning && boundary.depth == 0) {
                root_groups.insert(boundary.step);
                generation = boundary.generation;
            }
        for (unsigned frame = 0; frame < 1000 && session.owns_pose(1); ++frame) {
            RuntimePlayerComboChainPlanV1 repeated_request;
            const auto* attack_state = session.source_attack_state(1);
            check(attack_state != nullptr, "Same-session source combo owner is unavailable");
            const bool hold_request = attack_state->index < 2;
            check(plan_runtime_player_combo_chain_v1(assets, ordinary_longsword,
                  stance_constants.stanced_list_mask, 3, hold_request, animations, clips,
                  "KnightPlayerBase", base_visual.config,
                  "player-combo-held", repeated_request, error), error);
            input.attack = repeated_request.command_delivered;
            check(session.update(.016, input, {0,0,0},
                  session.actor(1)->transform.rotation[2], error), error);
            for (const auto& boundary : session.combo_boundaries()) {
                if (boundary.beginning && boundary.depth == 0) root_groups.insert(boundary.step);
                if (generation) check(boundary.generation == generation,
                    "Repeated attack request allocated a new combo action generation");
            }
            check(session.actor(1)->target_id == 2 && session.selectedactor() &&
                  session.selectedactor()->id == 2,
                  "Repeated attack request changed the authoritative selected target");
        }
        check(!session.owns_pose(1), "Released source chain did not finish after requests stopped");
        check(root_groups == std::set<std::uint32_t>{0,1,2},
              "Same session did not advance the three authored ordered combo roots");
        check(session.actor(1)->target_id == 2 && session.selectedactor() &&
              session.selectedactor()->id == 2,
              "Source combo completion lost a live selected target");
        check(!project_visible(camera, {session.selectedactor()->transform.position[0],
                                         session.selectedactor()->transform.position[1],
                                         session.selectedactor()->transform.position[2]}, 16.f/9.f),
              "HUD projection fixture no longer separates visibility from target selection");

        std::cout << "PASS actual ItemTable stances 0/2/3 select source Attack/AttackStatic roots "
                  << static_plans[0].selected_sequence_id << ','
                  << static_plans[1].selected_sequence_id << ','
                  << static_plans[2].selected_sequence_id << ','
                  << static_plans[3].selected_sequence_id
                  << "; authored rates/MoveGO/markers retained; same-session groups 0/1/2 and "
                     "target_id/selectedactor retained while target is offscreen\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return 1;
    }
}
