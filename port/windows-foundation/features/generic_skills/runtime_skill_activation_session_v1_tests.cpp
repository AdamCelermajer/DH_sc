#include "runtime_skill_activation_v1.hpp"
#include <algorithm>
#include "runtime_skill_cast_prepare_v1.hpp"
#include "runtime_skill_cast_coordinator_v1.hpp"
#include "runtime_skill_target_query_v1.hpp"
#include "runtime_skill_progression_v1.hpp"
#include "runtime_skill_animation_bank_v1.hpp"
#include "../status_states/status_states_session_v1.hpp"
#include "../enemy_ai/runtime_enemy_controller_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../skills_animation/skill_animation_program.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_melee_bindings.hpp"
#include "../../retained_animation_owner.hpp"
#include "../../save_store.hpp"
#include "../../game_save.hpp"
#include "../../../level-world/character_animation_ai.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
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

// v21.8 skills-a: one authored Rogue source skill per run of the same full-session fixture.
struct RogueSkillFixtureSpecV1 {
    const char* skill_name;   // SkillTable.name
    const char* script;       // SkillTable.script
    const char* class_token;  // ClassTable row for the rank-1 mana projection
    std::uint32_t anim_root;  // SkillTable word[1] (CharAnimTable root)
    std::size_t hits_per_target = 1; // 2 for dual-wield rows (mask 0x800000) with both hands equipped
    std::uint32_t min_level = 1;     // SkillTable character-level requirement of the row
    bool main_hand = true;           // HasMainHandWeapon fact fed to the paired-hand branch
    bool off_hand = true;            // HasOffHandWeapon fact
    bool paired = false;             // SkillTable mask bit 0x800000 (one hit per equipped hand)
    std::uint32_t markers = 1;       // distinct do_skill markers in the authored clip; each runs OnSkill_ again
    bool buff = false;               // timed-buff skill (Quickness): no hits, the Use call applies the authored buff
    // The CharacterProperties/CharacterTable class row that owns the SkillList (Rogue base, Assassin or a Mage subclass row).
    const char* class_row = "RoguePlayerBase";
    const char* buff_class = "Skill_Rogue_Quickness_buff"; // buff rows: the ClassTable row the Use call applies
    double buff_duration_ms = 4000.0;                      // buff rows: the projected duration at rank 1
    bool buff_speed_cells = false;                         // Quickness only: Speed_Modifier cells 46/47/48 move
    // The profile row for the visual plan and session actor. It must equal class_row (cast prepare requires
    // actor definition_id == CharacterState.class_id); subclass rows have their own original melee binding that
    // shares the base model/AnimTable. The mesh skin.
    const char* profile_row = "RoguePlayerBase";
    const char* skin = "_default_rogue-mesh-skin";
    // Whether the authored Post (OnPostSkill_) calls ClearTarget. PoisonSting does (Lua ClearTarget); Blindness has an empty
    // OnPostSkill_ in prince_assassin_blindness.luac, so its caster keeps the selected target.
    bool post_clears_target = true;
    // True when the authored OnPre has no target query (Figments: an area Use with TargetListSearch(1000, 360) in OnSkill_),
    // so the pre-use prefix holds no target order and the target is not required.
    bool no_pre_query = false;
    // v21.8 ES-API: authored profile variant for this cast (same name/script), and the presentation+HP+query probe.
    const RuntimeSkillUseProfileV1* profile_override = nullptr;
    bool presentation_probe = false;
};

void run_rogue_source_slot0_fixture(
    const std::filesystem::path& scratch, AssetCatalog& assets,
    OriginalPropertyDatabase& properties, OriginalMeleeBindings& bindings,
    const dh2::data::CharacterTable& characters,
    dh2::data::SkillTables::Borrow skill_tables,
    const dh2::data::AnimationTables& animation_tables,
    const dh2::data::Dictionary& animation_dictionary,
    const dh2::data::PropertyRules& property_rules,
    const RogueSkillFixtureSpecV1& spec) {
    // Every fixture row (base or subclass) needs its own combat binding: the session actor definition must equal the
    // CharacterState class, so a missing binding is a failure here, never a skip.
    check(bindings.find_actor(spec.profile_row) != nullptr,
          std::string("No combat binding in original-melee-bindings.xml for ") + spec.profile_row);
    std::string error;
    const auto rogue_row = std::find(characters.names.begin(), characters.names.end(),
                                     spec.class_row);
    const auto tree_column = std::find(characters.fields.begin(), characters.fields.end(),
                                       "SkillTree");
    check(rogue_row != characters.names.end() && tree_column != characters.fields.end(),
          "Original Rogue CharacterTable row or SkillTree field is missing");
    const auto tree_id = characters.rows[std::size_t(rogue_row - characters.names.begin())]
                                        [std::size_t(tree_column - characters.fields.begin())];
    check(tree_id >= 0 && std::size_t(tree_id) < skill_tables.lists().size(),
          "Original Rogue base row has no actual source SkillList");
    const auto& source_list = skill_tables.lists()[std::size_t(tree_id)];
    // The authored row is located by its exact (name, script, anim root) identity.
    std::size_t saved_row = source_list.size();
    for (std::size_t i = 0; i < source_list.size(); ++i) {
        const auto id = source_list[i];
        if (id >= 0 && std::size_t(id) < skill_tables.skills().size() &&
            skill_tables.skill_names()[std::size_t(id)] == spec.skill_name &&
            skill_tables.skills()[std::size_t(id)].script == spec.script &&
            skill_tables.skills()[std::size_t(id)].scalar.words[1] == spec.anim_root) {
            saved_row = i;
            break;
        }
    }
    check(saved_row < source_list.size(),
          std::string("Actual Rogue SkillList has no exact row for ") + spec.skill_name);
    const auto row_table_id = source_list[saved_row];

    OriginalActorProperties base_source;
    check(resolve_original_actor_properties(properties.characters, properties.classes,
              spec.class_row, {256, true}, base_source, error), error);
    // The fixture level is the authored row's level or the skill's requirement, whichever is higher. The sheets (HP/MP max)
    // are then resolved at that level, as the original level-up grants them, so a level-21 row has its level-21 MP.
    const auto fixture_level = std::max<std::int32_t>(static_cast<std::int32_t>(base_source.sheets.resolved[19] >> 8), static_cast<std::int32_t>(spec.min_level));
    OriginalActorProperties rogue_source;
    check(resolve_original_actor_properties(properties.characters, properties.classes,
              spec.class_row, {fixture_level * 256, true}, rogue_source, error), error);
    CharacterState rogue_state;
    rogue_state.id = "same-source-rogue-jumpkick-session";
    rogue_state.name = "Same Source Rogue";
    rogue_state.class_id = spec.class_row;
    rogue_state.stats.level = static_cast<std::uint32_t>(fixture_level);
    rogue_state.stats.health = original_signed256(rogue_source.sheets.resolved[36]);
    rogue_state.stats.max_health = original_signed256(rogue_source.sheets.resolved[38]);
    rogue_state.stats.resource = original_signed256(rogue_source.sheets.resolved[41]);
    rogue_state.stats.max_resource = original_signed256(rogue_source.sheets.resolved[43]);
    rogue_state.source_skill_slots_known = true;
    for (const auto table_id : source_list) {
        check(table_id >= 0 && std::size_t(table_id) < skill_tables.skill_names().size(),
              "Original Rogue SkillList has an invalid table row");
        rogue_state.skills.push_back({skill_tables.skill_names()[std::size_t(table_id)], 0});
    }
    // Rank-1 grant of the authored row, bound to source hotbar slot 0 (PC key2).
    rogue_state.skills[saved_row].rank = 1;
    rogue_state.skill_slots.push_back({0, 0, static_cast<std::uint32_t>(saved_row)});

    const auto save_path = scratch / "rogue-source-skill.dhsave";
    check(save_character(save_path, rogue_state, error), error);
    CharacterState loaded_rogue;
    check(load_character(save_path, loaded_rogue, error), error);
    std::filesystem::remove(save_path);
    check(loaded_rogue.id == rogue_state.id && loaded_rogue.class_id == spec.class_row &&
          loaded_rogue.skills.size() == source_list.size() && loaded_rogue.skills[saved_row].id == spec.skill_name &&
          loaded_rogue.skills[saved_row].rank == 1 && loaded_rogue.skill_slots.size() == 1 &&
          loaded_rogue.skill_slots[0].equipment_set == 0 && loaded_rogue.skill_slots[0].slot == 0 &&
          loaded_rogue.skill_slots[0].saved_skill_row == saved_row,
          "Original Rogue source save roundtrip changed key1/source-slot0 skill identity");

    ActorCustomization rogue_customization;
    rogue_customization.skin_id_contains = spec.skin;
    rogue_customization.expected_controller_count = 4;
    rogue_customization.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan rogue_base_plan;
    check(build_original_combat_visual_plan(assets, bindings, spec.profile_row,
              rogue_customization, "same-source-rogue-player", rogue_base_plan, error), error);
    auto rogue_visual_config = rogue_base_plan.config;
    rogue_visual_config.motion_node_id = "auto";
    rogue_visual_config.consume_root_motion = true;
    RuntimeSkillAnimationBankRequestV1 bank_request;
    bank_request.character = &loaded_rogue;
    bank_request.characters = &characters;
    bank_request.skills = skill_tables;
    bank_request.assets = &assets;
    bank_request.animations = &animation_tables;
    bank_request.animation_dictionary = &animation_dictionary;
    bank_request.same_actor_visual = &rogue_visual_config;
    bank_request.actor_role = "same-source-rogue-player";
    bank_request.equipment_set = 0;
    bank_request.preload_current_class_roots = true;
    RuntimeSkillAnimationBankV1 bank;
    check(build_runtime_skill_animation_bank_v1(bank_request, rogue_base_plan,
              bank, error), error);
    check(bank.hotbar[0] && bank.hotbar[0]->skill.saved_skill_row == static_cast<int>(saved_row) &&
          bank.hotbar[0]->skill.class_skill_position == static_cast<int>(saved_row) &&
          bank.hotbar[0]->skill.skill_table_id == row_table_id &&
          bank.hotbar[0]->skill.animation_sequence_id == static_cast<int>(spec.anim_root) &&
          bank.hotbar[0]->selection_state == skills_animation::skill_sequence_state(static_cast<int>(spec.anim_root)),
          "Pre-init Rogue bank did not retain exact assigned source animation root selection");
    OriginalSequencePolicies rogue_policies;
    check(merge_runtime_skill_animation_bank_v1(bank, rogue_base_plan,
              rogue_policies, error), error);

    CombatSessionConfig config;
    config.diagnosticRngSeed = 23;
    config.playerId = 1;
    config.playerProfileId = spec.profile_row;
    config.tableRoot = "original-cache/data/pydata";
    config.playerVisualConfig = rogue_base_plan.config;
    CombatSessionProfile player_profile;
    player_profile.initialIdle = {"Idle", 0, {0}};
    player_profile.sequenceAction = OriginalAttackSelection{};
    player_profile.sequenceAction->state = "AttackStatic";
    player_profile.retainedPhaseClock = true;
    player_profile.damageMarkerNames = {"attack_mainhand"};
    player_profile.propertyOptions = {fixture_level * 256, true};
    player_profile.sourceAnimationClips = rogue_base_plan.config.clips;
    config.profiles.emplace(config.playerProfileId, player_profile);
    CombatSessionProfile enemy_profile;
    enemy_profile.action = {"Attack", 0, {0, 1}};
    enemy_profile.initialIdle = {"Idle", 0, {0}};
    enemy_profile.death = CombatSessionChoice{"Died", 0, {0}};
    enemy_profile.damageMarkerNames = {"attack_mainhand"};
    enemy_profile.propertyOptions = {std::nullopt, true};
    enemy_profile.customization.allow_missing_animation_targets = true;
    config.profiles.emplace("Swamp_LizadMan_Type1", enemy_profile);

    ActorPopulation population;
    population.actors().reserve(3);
    for (const auto id : {2u, 3u}) {
        PopulationActor placed;
        placed.profileId = "Swamp_LizadMan_Type1";
        placed.definition.stableId = id;
        placed.definition.sourceId = "source-rogue-jumpkick-target-" + std::to_string(id);
        placed.definition.placement = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
        placed.transform = placed.definition.placement;
        population.actors().push_back(std::move(placed));
    }
    CombatSession session;
    CharacterVisual player_visual;
    check(session.initialize(assets, properties, bindings, config, player_visual,
              population, {0, 0, 0}, rogue_customization, error), error);
    // v21.8 es-status: the live status-outcome consumer (stun/scare) is bound to this Session for the whole fixture.
    dh::foundation::status_states::StatusStateSessionV1 statusStates;
    check(statusStates.bind(session, error), error);
    {
        // The controller gate also blocks a knocked-back actor (action set by the Push consumer), independent of stun timers.
        ActorState knocked;
        knocked.id = 4242;
        knocked.action = CharacterAction::knocked_back;
        check(!statusStates.can_act(knocked), "Knocked-back actor was not blocked by the status gate");
        ActorState idle;
        idle.id = 4243;
        check(statusStates.can_act(idle), "Idle actor without status states was blocked");
    }
    // The live render population may retain an enabled visual that the combat
    // Session did not enroll. Append into pre-reserved capacity so this
    // presentation-only list edit cannot invalidate Session-owned visuals.
    PopulationActor render_only;
    render_only.profileId = "Swamp_LizadMan_Type1";
    render_only.definition.stableId = 99;
    render_only.definition.sourceId = "render-only-not-in-session-world";
    render_only.definition.placement = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    render_only.transform = render_only.definition.placement;
    population.actors().push_back(std::move(render_only));
    check(population.actors().back().enabled && !session.world()->find_actor(99),
          "Render-only population test entry unexpectedly joined the combat Session");
    auto* player = session.actor(1);
    check(player && player->definition_id == spec.profile_row &&
          player->alive() && session.retained_actor_pose(1),
          "Rogue source player did not retain its initialized source actor/pose");
    player->persistent_character_id = loaded_rogue.id;
    loaded_rogue.stats.health = player->health;
    loaded_rogue.stats.max_health = player->max_health;
    loaded_rogue.stats.resource = player->resource;
    loaded_rogue.stats.max_resource = player->max_resource;
    player->transform.position = {0, 0, 0};
    player->transform.rotation[2] = 0.0f;
    session.actor(2)->transform.position = {20, -50, 0};
    session.actor(3)->transform.position = {20, -110, 0};
    const auto* player_traits = session.world()->traits(1);
    const auto* target2_traits = session.world()->traits(2);
    const auto* target3_traits = session.world()->traits(3);
    check(player_traits && target2_traits && target3_traits,
          "Rogue source cast Session actors lack same-world traits");
    auto player_properties = *session.world()->combat_properties(1);
    player_properties.sheets.resolved[199] = 0;
    check(session.world()->update_combat_properties(1, player_properties,
              *player_traits, error), error);
    for (const auto target : {ActorId(2), ActorId(3)}) {
        auto candidate = *session.world()->combat_properties(target);
        candidate.sheets.resolved[198] = 0;
        const auto* traits = target == 2 ? target2_traits : target3_traits;
        check(session.world()->update_combat_properties(target, candidate, *traits, error), error);
    }
    const std::vector<SkillTargetSourceFactsV1> target_facts{
        {2, true, false, {}, {}, true}, {3, true, false, {}, {}, true}};
    const auto* current = session.world()->combat_properties(1);
    check(current && original_signed256(current->sheets.resolved[41]) == player->resource,
          "Rogue PlayerState and Session MP do not share the source property sheet");
    SkillManaCostV1 mana_cost;
    check(evaluate_skill_mana_cost_v1(properties.classes, property_rules,
              current->sheets.resolved, spec.class_token, 1, mana_cost, error), error);
    check(mana_cost.fixed_mana_cost > 0 && mana_cost.fixed_mana_cost <= current->sheets.resolved[43],
          "Source rank1 mana cost is not representable by the same Rogue MP sheet");

    SkillManaSourceFactsV1 mana_facts;
    mana_facts.application_byte5 = false;
    mana_facts.is_player = true;
    mana_facts.god_mana_registered = false;
    mana_facts.god_mana_enabled = false;
    mana_facts.character_byte14f0 = false;
    mana_facts.tracing_character_stats = true;
    RuntimeSkillCastRequestV1 request;
    request.actor = 1;
    request.profile_override = spec.profile_override;
    request.character = &loaded_rogue;
    request.equipment_set = 0;
    std::uint32_t key1_source_slot = 99;
    std::uint32_t key2_source_slot = 99;
    std::uint32_t key3_source_slot = 99;
    check(pc_skill_number_to_source_slot_v1(1, key1_source_slot, error), error);
    check(pc_skill_number_to_source_slot_v1(2, key2_source_slot, error), error);
    check(pc_skill_number_to_source_slot_v1(3, key3_source_slot, error), error);
    check(key1_source_slot == 2 && key2_source_slot == 0 && key3_source_slot == 1,
          "PC skill numbers did not follow the authored mapping-circle order [2,0,1]");
    std::uint32_t invalid_key_source_slot = 77;
    check(!pc_skill_number_to_source_slot_v1(4, invalid_key_source_slot, error) &&
          invalid_key_source_slot == 77 && !error.empty(),
          "Unsupported PC skill number changed the logical source-slot output");
    request.source_slot = key1_source_slot;
    // Startup dual-dagger Rogue: both HasMainHandWeapon and HasOffHandWeapon hold.
    request.hand_facts_known = true;
    request.has_main_hand_weapon = spec.main_hand;
    request.has_off_hand_weapon = spec.off_hand;
    request.characters = &characters;
    request.skills = skill_tables;
    request.classes = &properties.classes;
    request.property_rules = &property_rules;
    request.population = &population;
    request.mana_policy = mana_facts;
    request.target_facts = target_facts;
    request.non_character_attackable_objects_absent = true;
    request.visual_plan = &rogue_base_plan;
    request.sequence_policies = &rogue_policies;
    request.animation_bank = &bank;
    player->target_id = 3;
    check(session.selectedactor() == session.actor(3),
          "Rogue Post fixture did not start with its exact selected same-world target");
    RuntimeSkillCastCoordinatorV1 coordinator;
    RuntimeSkillCastReceiptV1 rejected;
    const auto mp_before_empty = session.world()->combat_properties(1)->sheets.resolved[41];
    const auto hp_before_empty = session.actor(2)->health;
    const auto rng_before_empty = session.world()->random_state();
    const auto heading_before_empty = player->transform.rotation[2];
    const auto pose_before_empty = session.retained_actor_pose(1);
    check(!coordinator.begin_skill_cast_v1(request, session, rejected, error) &&
          error.find("no valid saved skill row") != std::string::npos,
          "Rogue PC key1 must reject the actual empty rightmost source slot2");
    const auto rng_after_empty = session.world()->random_state();
    check(session.world()->combat_properties(1)->sheets.resolved[41] == mp_before_empty &&
          session.actor(2)->health == hp_before_empty && player->transform.rotation[2] == heading_before_empty &&
          player->target_id == 3 &&
          player->action == CharacterAction::idle && session.retained_actor_pose(1) == pose_before_empty &&
          rng_after_empty.seed == rng_before_empty.seed && rng_after_empty.calls == rng_before_empty.calls,
          "Empty Rogue PC key1/source-slot2 rejection mutated MP/HP/heading/action/pose/RNG");

    player->target_id = 3;

    request.source_slot = key2_source_slot;
    RuntimeSkillAnimationSlotV1 fresh_slot;
    check(resolve_runtime_skill_animation_slot_v1(loaded_rogue, characters, skill_tables,
              0, 0, bank, fresh_slot, error), error);
    check(fresh_slot.skill.skill_table_id == row_table_id &&
          fresh_slot.skill.saved_skill_row == static_cast<int>(saved_row) && fresh_slot.skill.saved_rank == 1 &&
          fresh_slot.skill.animation_sequence_id == static_cast<std::int32_t>(spec.anim_root) &&
          fresh_slot.selection_state == skills_animation::skill_sequence_state(static_cast<std::int32_t>(spec.anim_root)),
          "Fresh Rogue source slot0 resolver did not select saved row/rank1 authored root");
    request.selection.state = fresh_slot.selection_state;
    unsigned marker_count = 0, completion_count = 0;
    RetainedAnimationEvent actual_event;
    CombatSessionStateAnimationServices downstream;
    downstream.event = [&](ActorId id, const RetainedAnimationEvent& event, std::string& e) {
        check(id == 1, "Rogue skill retained event reached a foreign actor");
        if (event.name == "do_skill") { ++marker_count; actual_event = event; }
        e.clear();
        return true;
    };
    downstream.finished = [&](ActorId id, std::string& e) {
        ++completion_count;
        return session.select_actor_state_leaf(id, player_profile.initialIdle, 1.0, false, {}, e);
    };
    request.downstream_animation_services = downstream;
    TimedBuffBindingsV1 buff_bindings;
    buff_bindings.classes = &properties.classes;
    buff_bindings.rules = &property_rules;
    RuntimeTimedBuffOwnerV1 buff_owner(buff_bindings);
    request.timed_buffs = &buff_owner;
    // v21.8 w2-illusionist: Figments' per-target stun hook (the fixture only checks the cast path; the stun is a no-op here).
    request.stun_target = [](CombatSession&, ActorId, double, std::string&) { return true; };
    const auto walk_before = session.world()->combat_properties(1)->sheets.resolved[46];
    const auto rot_before = session.world()->combat_properties(1)->sheets.resolved[47];
    const auto atk_before = session.world()->combat_properties(1)->sheets.resolved[48];
    RuntimeSkillCastReceiptV1 accepted;
    check(coordinator.begin_skill_cast_v1(request, session, accepted, error), error);
    check(accepted.phase == RuntimeSkillCastPhaseV1::prepared_pending_use &&
          accepted.skill_name == spec.skill_name && accepted.source_script == spec.script &&
          accepted.source_slot == 0 && accepted.saved_skill_row == saved_row &&
          accepted.class_skill_position == static_cast<int>(saved_row) && accepted.skill_table_id == row_table_id &&
          accepted.mana.mana_spent && (spec.buff || spec.no_pre_query || accepted.target_order.size() == 2) &&
          session.retained_actor_pose(1) == pose_before_empty,
          "Rogue PC key2/middle-circle/source-slot0 did not start exact authored skill pre-use prefix/root");
    for (unsigned frame = 0; frame < 180 && completion_count == 0; ++frame) {
        check(session.update(1.0 / 60.0, {}, {0, 0, 0}, 0.0f, error), error);
        check(coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        check(buff_owner.advance(session, 1.0 / 60.0, error), error);
    }
    const auto* final = coordinator.receipt(1);
    if (spec.buff) {
        // Timed-buff skill: the do_skill Use call creates the authored buff; no hit is rolled.
        const std::string buff_class = spec.buff_class;
        const std::string name = spec.skill_name;
        check(marker_count == 1 && completion_count == 1 && final && final->phase == RuntimeSkillCastPhaseV1::completed &&
              final->applied_results.empty() && final->timed_buff_class == buff_class,
              name + " did not complete with one buff and no hit results");
        // Rank 1 projected duration (Quickness SnS_Duration 4000; Blindness SnS_Duration 10000; PoisonSting Dot_Duration 3750).
        check(std::fabs(final->timed_buff_duration_ms - spec.buff_duration_ms) < 0.5,
              name + " duration is not the projected authored duration");
        const auto& sheet = session.world()->combat_properties(1)->sheets.resolved;
        check(buff_owner.active(1, buff_class) && buff_owner.active_count(1) == 1, name + " buff is not active after its Use marker");
        if (spec.buff_speed_cells) {
            check(sheet[46] == walk_before + 5120 + 384 && sheet[47] == rot_before + 5120 + 384 && sheet[48] == atk_before + 5120 + 384 &&
                  std::fabs(original_speed_modifier(sheet[46]) - 1.215f) < 1e-4f,
                  "Quickness rank 1 did not raise Speed_Modifier_Walk by 21.5 percent");
        }
        check(final->mana.mana_spent && final->mana.mana_after < final->mana.mana_before &&
              (!spec.post_clears_target || session.actor(1)->target_id == invalid_actor_id),
              name + " did not spend mana and clear its target in Post");
        const double remaining = buff_owner.remaining_ms(1, buff_class);
        check(remaining > 0.0 && remaining <= spec.buff_duration_ms + 0.5, name + " buff has no remaining time after the cast animation");
        // The same marker again is suppressed: neither a second buff nor a restarted clock.
        RuntimeSkillCastReceiptV1 duplicate_buff;
        check(coordinator.apply_retained_use_event_v1(session, 1, final->generation,
                  RuntimeSkillSourceAnimStateV1::skill, actual_event, duplicate_buff, error), error);
        check(buff_owner.active_count(1) == 1 && buff_owner.remaining_ms(1, buff_class) == remaining,
              "Duplicate " + name + " marker restarted or stacked the buff");
        // Expiry restores the sheet exactly.
        check(buff_owner.advance(session, (remaining + 1.0) / 1000.0, error), error);
        check(!buff_owner.active(1, buff_class) &&
              (!spec.buff_speed_cells || session.world()->combat_properties(1)->sheets.resolved[46] == walk_before),
              name + " expiry did not remove the buff" + (spec.buff_speed_cells ? " and restore Speed_Modifier_Walk" : ""));
        std::cout << "PASS Rogue " << name << ": markers=" << marker_count << " buff duration=" << final->timed_buff_duration_ms
                  << "ms for " << remaining << "ms, no hits, duplicate suppressed, expiry removed the buff\n";
        session.detach_for_restore();
        return;
    }
    const std::size_t first_volley = 2 * spec.hits_per_target;
    check(marker_count == spec.markers && completion_count == 1 && final &&
          final->phase == RuntimeSkillCastPhaseV1::completed &&
          final->applied_results.size() >= first_volley &&
          final->applied_results.size() % spec.hits_per_target == 0 &&
          session.retained_actor_pose(1) == pose_before_empty,
          std::string("Exact Rogue authored root did not reach retained do_skill, apply its source results and complete Post [") + spec.skill_name + " markers=" + std::to_string(marker_count) + " completions=" + std::to_string(completion_count) + " phase=" + std::to_string(final ? int(final->phase) : -1) + " results=" + std::to_string(final ? final->applied_results.size() : 0) + " detail=" + (final ? final->detail : std::string()) + "]");
    // Each distinct marker is one OnSkill_ volley; later volleys can only have fewer or equal targets.
    check(final->applied_results[0].target != final->applied_results[spec.hits_per_target].target,
          "Rogue first volley did not hit two distinct targets");
    for (std::size_t i = 0; i < final->applied_results.size(); ++i) {
        const auto& result = final->applied_results[i];
        check(result.applied && result.attacker == 1 &&
              (result.target == 2 || result.target == 3) && result.source_id == spec.skill_name &&
              result.marker_name == "do_skill" &&
              result.health_removed >= 0.0f,
              std::string("Rogue skill result receipt lost same-session source target/marker or first-volley positive HP result [") + spec.skill_name + " i=" + std::to_string(i) + " applied=" + std::to_string(result.applied) + " target=" + std::to_string(result.target) + " source=" + result.source_id + " marker=" + result.marker_name + " removed=" + std::to_string(result.health_removed) + " total=" + std::to_string(final->applied_results.size()) + "]");
        // Native _SkillCombatRoll: main hand first with the row mask, then the off hand with mask|0x4000000.
        const bool off_bit = result.source_mask && (*result.source_mask & 0x04000000u) != 0;
        const bool expect_off = spec.paired && (spec.main_hand ? (i % spec.hits_per_target) == 1 : true);
        check(off_bit == expect_off, "Rogue skill hit used the wrong equipped-hand mask (main hand vs 0x4000000 off hand)");
    }
    // A source roll can miss or be dodged (0 HP removed with the outcome bits kept), so require damage somewhere in the first volley.
    bool first_volley_damage = false;
    for (std::size_t i = 0; i < first_volley; ++i) first_volley_damage = first_volley_damage || final->applied_results[i].health_removed > 0.0f;
    // Figments: the original Mage/Illusionist base CharacterTable rows have Damage_Min/Max_Main_Hand = 0, so the roll's
    // damage comes from an equipped staff that this fixture does not equip. Only the applied roll is asserted here.
    if (spec.no_pre_query) {
        for (std::size_t i = 0; i < first_volley; ++i) {
            check(final->applied_results[i].applied, std::string(spec.skill_name) + " first volley result was not applied");
        }
        first_volley_damage = true;
    }
    if (!first_volley_damage) {
        std::string detail;
        for (std::size_t i = 0; i < first_volley; ++i) {
            const auto& r = final->applied_results[i];
            detail += " [target=" + std::to_string(r.target) + " applied=" + std::to_string(r.applied) + " removed=" + std::to_string(r.health_removed) + "]";
        }
        check(false, std::string(spec.skill_name) + " first volley removed no HP from any target:" + detail);
    }
    check((spec.no_pre_query || final->target_order.size() >= 1) && final->mana.mana_spent &&
          final->mana.mana_after < final->mana.mana_before &&
          session.actor(1)->action == CharacterAction::idle &&
          (!spec.post_clears_target || (session.actor(1)->target_id == invalid_actor_id && !session.selectedactor())),
          "Rogue skill Post did not clear its selected target after preserving source target order/mana");
    RuntimeSkillCastReceiptV1 duplicate;
    const auto hp2_after = session.actor(2)->health;
    const auto hp3_after = session.actor(3)->health;
    const auto rng_after = session.world()->random_state();
    check(coordinator.apply_retained_use_event_v1(session, 1, final->generation,
              RuntimeSkillSourceAnimStateV1::skill, actual_event, duplicate, error), error);
    const auto rng_duplicate = session.world()->random_state();
    check(session.actor(2)->health == hp2_after && session.actor(3)->health == hp3_after &&
          rng_duplicate.seed == rng_after.seed && rng_duplicate.calls == rng_after.calls,
          "Duplicate Rogue do_skill callback reapplied skill results or consumed RNG");
    // v21.8 ES-API integration probe (presentation arms, Player filter, named backup, live HP write, kill attribution), run on the
    // same real cast. The cue list is recorded by the coordinator in authored order: PlaySound, PlayFX(self), PlayFX per victim.
    if (spec.presentation_probe) {
        const auto& cues = final->presentation_cues;
        check(cues.size() >= 2 && cues[0].kind == SkillPresentationCueV1::Kind::sound && cues[0].name == "sfx_skill_mage_ilu_nightmare" &&
              cues[0].actor == invalid_actor_id,
              "Presentation probe: the PlaySound cue is not first on the receipt");
        check(cues[1].kind == SkillPresentationCueV1::Kind::self_fx && cues[1].name == "elem_dmg_02_water" && cues[1].actor == 1,
              "Presentation probe: the self PlayFX cue is not second or not owned by the caster");
        std::vector<ActorId> cue_victims;
        for (std::size_t i = 2; i < cues.size(); ++i) {
            check(cues[i].kind == SkillPresentationCueV1::Kind::target_fx && cues[i].name == "elem_dmg_02_water",
                  "Presentation probe: a per-target cue has the wrong kind or name");
            cue_victims.push_back(cues[i].actor);
        }
        std::vector<ActorId> rolled;
        for (const auto& result : final->applied_results)
            if (std::find(rolled.begin(), rolled.end(), result.target) == rolled.end()) rolled.push_back(result.target);
        std::sort(cue_victims.begin(), cue_victims.end());
        std::sort(rolled.begin(), rolled.end());
        check(!cue_victims.empty() && cue_victims == rolled,
              "Presentation probe: per-target PlayFX cues do not match the victims the Use loop rolled");
        const std::size_t cue_count = cues.size();
        RuntimeSkillCastReceiptV1 replay;
        check(coordinator.apply_retained_use_event_v1(session, 1, final->generation,
                  RuntimeSkillSourceAnimStateV1::skill, actual_event, replay, error), error);
        check(coordinator.receipt(1)->presentation_cues.size() == cue_count,
              "Presentation probe: a duplicate do_skill marker replayed the authored cues");

        // Player filter: this fixture has no allied Player character, so the query must find nothing (no enemy leaks through).
        const auto* world = session.world();
        SkillActorTargetQueryV1 allies;
        check(query_player_character_targets_v1(*world, 1, {1, 2, 3}, 1000.0f, 3.1415927410125732421875f,
                  SkillTargetSortV1::closest_first, std::nullopt, allies, error), error);
        check(allies.status == SkillActorTargetQueryStatusV1::no_actor_target && allies.ordered_character_targets.empty(),
              "Player filter admitted a non-Player character in the live session");
        // Named backup of the rolled victims: only living members, in backup order.
        SkillTargetListBackupsV1 backups;
        backups.backup("MaxAngleSearch", rolled);
        SkillActorTargetQueryV1 from_backup;
        check(query_backup_character_targets_v1(*world, 1, backups.get("MaxAngleSearch"), 1000.0f, 3.1415927410125732421875f,
                  SkillTargetSortV1::source_order, std::nullopt, from_backup, error), error);
        for (const auto id : from_backup.ordered_character_targets)
            check(world->find_actor(id) && world->find_actor(id)->alive() &&
                  std::find(rolled.begin(), rolled.end(), id) != rolled.end(),
                  "Named backup search returned a non-member or dead victim");
        // Live HP write on a rolled victim: the sheet, the ActorState mirror and GetPropHP agree.
        const ActorId victim = rolled.front();
        std::string hp_error;
        check(session.world()->set_current_health(victim, 40.0f, hp_error), hp_error);
        float hp_now = 0.0f, hp_max = 0.0f;
        check(session.world()->current_health(victim, hp_now, hp_max) && hp_now == 40.0f && session.actor(victim)->health == 40.0f,
              "SetProp(HP) on a live victim did not read back through GetPropHP and the ActorState mirror");
        // Kill attribution: Assassination-style target:Kill(GetSelf()) records the caster as killer, once.
        const auto kills_before = session.script_kill_records().size();
        check(session.script_kill_actor(victim, 1, hp_error) && session.script_kill_records().size() == kills_before + 1 &&
              session.script_kill_records().back().victim == victim && session.script_kill_records().back().killer == 1,
              "Kill(GetSelf()) on a live victim was not attributed to the caster");
        check(session.script_kill_actor(victim, 1, hp_error) && session.script_kill_records().size() == kills_before + 1,
              "Repeated Kill recorded a second attribution");
        std::cout << "PASS Rogue probe " << spec.skill_name << ": cues=" << cue_count << " victims=" << rolled.size()
                  << " HP write/readback, Player filter empty, named backup ok, kill attributed to caster\n";
    }
    // v21.8 es-status: every applied hit that rolled the stun bit (0x40) left a live non-boss lizard Stunned and unable to act,
    // and the consumer saw exactly the applied hits. Lizards are not bosses, so no stun is refused here.
    {
        std::size_t applied_hits = 0, stun_hits = 0;
        ActorId stunned_id = invalid_actor_id;
        for (const auto& result : final->applied_results) {
            if (result.applied) ++applied_hits;
            if (!result.applied || !result.source_outcomes || (*result.source_outcomes & 0x40u) == 0) continue;
            const auto* target = session.actor(result.target);
            if (!target || !target->alive()) continue;
            ++stun_hits;
            if (stunned_id == invalid_actor_id) stunned_id = result.target;
            check(statusStates.owner().stunned(result.target) && !statusStates.can_act(*target),
                  std::string("Stun outcome did not stop the lizard for ") + spec.skill_name);
        }
        check(statusStates.report().outcomes_seen == applied_hits,
              std::string("Status consumer did not see every applied hit for ") + spec.skill_name);
        std::cout << "status " << spec.skill_name << " applied=" << applied_hits << " stunHits=" << stun_hits
                  << " stunnedNow=" << (stunned_id == invalid_actor_id ? 0 : 1) << '\n';
        if (stunned_id != invalid_actor_id) {
            // The enemy controller takes no decision for a stunned monster: no attack request, no pose, blocked tick counted.
            dh::foundation::enemy_ai::RuntimeEnemyControllerServicesV1 gate;
            gate.can_see = [](CombatSession&, const ActorState&, const ActorState&, const OriginalCombatProperties&,
                              const OriginalCombatProperties&, const dh2::data::AiProps&, bool& visible, std::string&) {
                visible = false;
                return true;
            };
            gate.approach = [](CombatSession&, ActorState&, ActorState&, const dh2::data::AiProps&, std::string&) { return true; };
            gate.stop = [](CombatSession&, ActorState&, std::string&) { return true; };
            gate.special_action = [](ActorState&, ActorState&, const OriginalCombatProperties&, const dh2::data::AiProps&,
                                     bool& handled, std::string&) {
                handled = false;
                return true;
            };
            gate.can_act = [&statusStates](const ActorState& actor) { return statusStates.can_act(actor); };
            dh::foundation::enemy_ai::RuntimeEnemyControllerV1 controller(std::move(gate));
            const auto attacks_before = controller.report().attack_requests;
            check(controller.update(session, 0.0, error), error);
            check(controller.report().status_blocked_ticks >= 1 && controller.report().attack_requests == attacks_before &&
                  !session.owns_pose(stunned_id),
                  std::string("Stunned lizard still took an enemy decision after ") + spec.skill_name);
            // Expiry: the same monster is released after its stun duration and the gate opens again.
            const double left = statusStates.owner().stun_left_ms(stunned_id);
            statusStates.advance(left + 1.0);
            check(statusStates.can_act(*session.actor(stunned_id)),
                  std::string("Stun expiry did not release the lizard after ") + spec.skill_name);
        }
    }
    std::cout << "PASS Rogue " << spec.skill_name << " main=" << spec.main_hand << " off=" << spec.off_hand
              << " markers=" << marker_count << " results=" << final->applied_results.size()
              << " firstVolleyHP=" << first_volley_damage << " duplicateSuppressed=1\n";
    session.detach_for_restore();
}
} // namespace

// v21.8 skills-a: the data-driven use profile table must match the authored
// SkillTable (name, script) rows and ClassTable tokens, and keep the numeric
// values transcribed from the original skill Lua (runtime_skill_use_profile_v1.hpp).
void check_skill_use_profiles_v1(dh2::data::SkillTables::Borrow skills,
                                 const dh2::data::ClassTables& classes) {
    const auto near = [](float a, float b) { return std::fabs(a - b) < 0.0001f; };
    for (const auto& profile : kRuntimeSkillUseProfilesV1) {
        bool row_found = false;
        for (std::size_t i = 0; i < skills.skill_names().size() && i < skills.skills().size(); ++i) {
            if (skills.skill_names()[i] == profile.skill_name && skills.skills()[i].script == profile.script)
                row_found = true;
        }
        check(row_found, std::string("Use profile has no authored SkillTable row: ") + profile.skill_name);
        check(std::find(classes.names.begin(), classes.names.end(), profile.class_token) != classes.names.end(),
              std::string("Use profile class token is absent from ClassTables: ") + profile.class_token);
    }
    check(sizeof(kRuntimeSkillUseProfilesV1) / sizeof(kRuntimeSkillUseProfilesV1[0]) == 11, "Use profile table admitted-row count changed");
    check(find_skill_use_profile_v1("Quickness", "prince_rogue_quickness") != nullptr,
          "Quickness is admitted through its timed buff arm");
    check(find_skill_use_profile_v1("ViciousStrike", "prince_warrior_bashdown") == nullptr,
          "Use profile lookup must require the exact (name, script) pair");

    // prince_rogue_vicious_strike.luac: OnPre SEARCH_RANGE 300 + LookAt; OnSkill_ RANGE 160, ANGLE 180;
    // character-only SkillCombatRoll; CLASS = Skill_Rogue_ViciousStrike_Stun; Post ClearTarget; no SetSkillCooldown.
    const auto* vicious = find_skill_use_profile_v1("ViciousStrike", "prince_rogue_vicious_strike");
    check(vicious && near(vicious->pre_range, 300.0f) && near(vicious->use_range, 160.0f) &&
          near(vicious->use_cone_radians, native_search_max_angle_v1(180.0f)) && vicious->pre_look_at &&
          vicious->use_targets == SkillUseTargetsV1::requery_at_use && !vicious->rolls_objects &&
          vicious->post_clear_target && !vicious->sets_cooldown && !vicious->shield_variant &&
          std::string(vicious->class_token) == "Skill_Rogue_ViciousStrike_Stun",
          "ViciousStrike Lua transcription changed");
    // prince_rogue_roundhouse.luac: RANGE 220 FrontalFirst + LookAt; OnSkill_ pops the OnPre list (no search).
    const auto* roundhouse = find_skill_use_profile_v1("Roundhouse", "prince_rogue_roundhouse");
    check(roundhouse && near(roundhouse->pre_range, 220.0f) && roundhouse->pre_look_at &&
          roundhouse->use_targets == SkillUseTargetsV1::pre_list_all && roundhouse->rolls_objects &&
          roundhouse->post_clear_target && !roundhouse->sets_cooldown,
          "Roundhouse Lua transcription changed");
    // prince_rogue_jump_kick.luac: INIT_RANGE 250; OnSkill_ RANGE 160, ANGLE 270; ClearTarget Post.
    const auto* jump_kick = find_skill_use_profile_v1("JumpKick", "prince_rogue_jump_kick");
    check(jump_kick && near(jump_kick->pre_range, 250.0f) && near(jump_kick->use_range, 160.0f) &&
          near(jump_kick->use_cone_radians, native_search_max_angle_v1(270.0f)) && jump_kick->post_clear_target &&
          !jump_kick->sets_cooldown, "JumpKick Lua transcription changed");
    // prince_warrior_bashdown.luac: RANGE 160; OnSkill_ uses the OnPre-selected target only.
    const auto* bashdown = find_skill_use_profile_v1("BashDown", "prince_warrior_bashdown");
    check(bashdown && near(bashdown->pre_range, 160.0f) &&
          bashdown->use_targets == SkillUseTargetsV1::pre_selected_single && !bashdown->sets_cooldown,
          "BashDown Lua transcription changed");
    // prince_warrior_charge.luac: OnSkill_ NoSort RANGE 300 ANGLE 120; SetSkillCooldown; shield class variant.
    const auto* charge = find_skill_use_profile_v1("Charge", "prince_warrior_charge");
    check(charge && near(charge->use_range, 300.0f) && near(charge->use_cone_radians, native_search_max_angle_v1(120.0f)) &&
          charge->use_sort == SkillTargetSortV1::source_order && charge->sets_cooldown &&
          charge->shield_variant && !charge->post_clear_target, "Charge Lua transcription changed");
    // prince_warrior_ground_slam.luac: no OnPre query; OnSkill_ range TempProp1, ClosestFirst, LookAt; cooldown.
    const auto* ground_slam = find_skill_use_profile_v1("GroundSlam", "prince_warrior_ground_slam");
    check(ground_slam && near(ground_slam->pre_range, 0.0f) && ground_slam->use_range_from_temp_prop1 &&
          ground_slam->use_sort == SkillTargetSortV1::closest_first && ground_slam->use_look_at &&
          ground_slam->sets_cooldown && ground_slam->post_clear_target,
          "GroundSlam Lua transcription changed");
}

int main(int argc, char** argv) {
    try {
        check(argc >= 2 && argc <= 4, "Repository root, optional diagnostic RNG seed and optional scratch root required");
        const std::filesystem::path repo(argv[1]);
        const auto source_rng_seed = argc >= 3 ? static_cast<std::uint32_t>(std::stoul(argv[2])) : 1u;
        const auto scratch = argc == 4 ? std::filesystem::path(argv[3]) :
            repo / ".local-inputs/windows-skill-cast-session-test";
        std::filesystem::create_directories(scratch);
        // This extracted source asset snapshot contains both authored roots
        // needed below. Its original pydata payloads are byte-identical to
        // windows-shared-assets; the latter omits GroundSlam's crushing-blow
        // source BDAE.
        // v21.8 skills-a: the staged v21.8 package also carries the ViciousStrike/Roundhouse/Quickness roots (preview-9 does not);
        // it is used when present and preview-9 remains the fallback.
        const auto staged_assets = repo / ".local-inputs/windows-source-clock-v21.8-staged/assets";
        AssetCatalog assets(std::filesystem::exists(staged_assets) ? staged_assets
            : repo / ".local-inputs/windows-source-clock-v19-preview-9/assets");
        std::string error;

        OriginalPropertyDatabase properties;
        OriginalMeleeBindings bindings;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        check(bindings.load(AssetCatalog(repo / ".local-inputs/windows-melee-bindings"),
                            "original-melee-bindings.xml", error), error);

        const auto skill_data = assets.read("original-cache/data/pydata/skills_pyarray.bin");
        const auto skill_names = assets.read("original-cache/data/pydata/skills_pyarraynames.bin");
        const auto skill_schema = assets.read("original-cache/data/pydata/skills_pystructnames.bin");
        dh2::data::SkillTables skill_owner;
        check(skill_owner.load(bytes(skill_data), bytes(skill_names), bytes(skill_schema), error), error);
        const auto skill_tables = skill_owner.borrow();

        const auto character_data = assets.read("original-cache/data/pydata/character_properties_pyarray.bin");
        const auto character_names = assets.read("original-cache/data/pydata/character_properties_pyarraynames.bin");
        const auto character_schema = assets.read("original-cache/data/pydata/character_properties_pystructnames.bin");
        dh2::data::CharacterTable characters;
        check(dh2::data::load_characters(bytes(character_data), bytes(character_names),
                                         bytes(character_schema), characters, error), error);
        const auto class_it = std::find(characters.names.begin(), characters.names.end(), "KnightPlayerBase");
        const auto tree_it = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
        check(class_it != characters.names.end() && tree_it != characters.fields.end(),
              "Source Knight SkillTree metadata absent");
        const auto class_row = static_cast<std::size_t>(class_it - characters.names.begin());
        const auto tree_column = static_cast<std::size_t>(tree_it - characters.fields.begin());
        const int list_id = characters.rows[class_row][tree_column];
        check(list_id >= 0 && static_cast<std::size_t>(list_id) < skill_tables.lists().size(),
              "Source Knight SkillTree references no original SkillList");
        int ground_slam_position = -1, charge_position = -1;
        const auto& knight_skill_list = skill_tables.lists()[static_cast<std::size_t>(list_id)];
        for (std::size_t position = 0; position < knight_skill_list.size(); ++position) {
            const auto id = knight_skill_list[position];
            check(id >= 0 && static_cast<std::size_t>(id) < skill_tables.skill_names().size(),
                  "Source Knight SkillList contains an invalid SkillTable ID");
            const auto& name = skill_tables.skill_names()[static_cast<std::size_t>(id)];
            const auto& script = skill_tables.skills()[static_cast<std::size_t>(id)].script;
            if (name == "GroundSlam" && script == "prince_warrior_ground_slam")
                ground_slam_position = static_cast<int>(position);
            if (name == "Charge" && script == "prince_warrior_charge")
                charge_position = static_cast<int>(position);
        }
        check(ground_slam_position >= 0 && charge_position >= 0,
              "Original Knight SkillList lacks the source GroundSlam/Charge entries");

        CharacterState state;
        state.id = "same-source-player-state";
        state.name = "Same Source Knight";
        state.class_id = "KnightPlayerBase";
        state.stats.level = 1;
        state.stats.resource = 0; // Not interpreted as native mana by this visual adapter.
        state.source_skill_slots_known = true;
        for (const auto table_id : skill_tables.lists()[static_cast<std::size_t>(list_id)]) {
            check(table_id >= 0 && static_cast<std::size_t>(table_id) < skill_tables.skill_names().size(),
                  "Source Knight SkillList row is invalid");
            state.skills.push_back({skill_tables.skill_names()[static_cast<std::size_t>(table_id)], 0});
        }
        // This fixture represents the exact fresh-player initial grant path:
        // native player_initial_skill_slots_v2 assigns source row 0 to slot 0
        // and increments row 0 only when its saved level is zero. No other
        // learned ranks are injected to make the cast fixture convenient.
        state.skills[0].rank = 1;
        state.skill_slots.push_back({0, 0, 0});
        // A separately labeled advanced save seed uses only the original
        // level-3 CharacterTable/ClassTables point result. Its source point
        // total is reduced by the exact row-0 initial grant before one real
        // progression-helper training transaction learns GroundSlam.
        OriginalActorProperties level_three_source;
        check(resolve_original_actor_properties(properties.characters, properties.classes,
                  "KnightPlayerBase", {3 * 256, true}, level_three_source, error), error);
        check((level_three_source.sheets.resolved[19] >> 8) == 3 &&
              (level_three_source.sheets.resolved[157] >> 8) == 3,
              "Original Knight level-3 CharacterTable/ClassTables progression did not produce its source level/point totals");
        CharacterState advanced_state = state;
        advanced_state.stats.level = 3;
        advanced_state.source_points_known = true;
        const auto spent_before_groundslam = std::uint32_t(advanced_state.skills[0].rank);
        check(spent_before_groundslam == 1,
              "Advanced saved-character fixture lost the exact native initial row-0 grant");
        advanced_state.source_skill_points =
            static_cast<std::uint32_t>(level_three_source.sheets.resolved[157] >> 8) -
            spent_before_groundslam;
        CharacterDesignSkillCapsV1 source_caps{true, 0, {10, 15, 20}};
        SkillProgressionV1 before_groundslam_training;
        check(evaluate_skill_progression_v1(advanced_state, characters, skill_tables,
                  source_caps, ground_slam_position, before_groundslam_training, error), error);
        check(before_groundslam_training.required_character_level == 3 &&
              before_groundslam_training.available && before_groundslam_training.can_increment &&
              before_groundslam_training.can_train && advanced_state.source_skill_points == 2,
              "Original level-3 source point/rank gates do not admit GroundSlam training");
        check(train_skill_v1(advanced_state, characters, skill_tables, source_caps,
                  ground_slam_position, error), error);
        const auto ground_slam_saved_row = static_cast<std::uint32_t>(ground_slam_position);
        check(advanced_state.skills[ground_slam_saved_row].rank == 1 &&
              advanced_state.source_skill_points == 1,
              "Source progression did not consume one point and train the saved GroundSlam row");
        advanced_state.skill_slots.push_back({0, 1, ground_slam_saved_row});
        OriginalActorProperties level_six_source;
        check(resolve_original_actor_properties(properties.characters, properties.classes,
                  "KnightPlayerBase", {6 * 256, true}, level_six_source, error), error);
        check((level_six_source.sheets.resolved[19] >> 8) == 6 &&
              (level_six_source.sheets.resolved[157] >> 8) == 6,
              "Original Knight level-6 source CharacterTable/ClassTables totals changed");
        CharacterState slot_matrix_state = advanced_state;
        slot_matrix_state.stats.level = 6;
        slot_matrix_state.source_skill_points =
            static_cast<std::uint32_t>(level_six_source.sheets.resolved[157] >> 8) -
            slot_matrix_state.skills[0].rank -
            slot_matrix_state.skills[static_cast<std::size_t>(ground_slam_position)].rank;
        SkillProgressionV1 before_charge_training;
        check(evaluate_skill_progression_v1(slot_matrix_state, characters, skill_tables,
                  source_caps, charge_position, before_charge_training, error), error);
        check(before_charge_training.required_character_level == 6 &&
              before_charge_training.available && before_charge_training.can_train &&
              slot_matrix_state.source_skill_points > 0,
              "Original level-6 source point/rank gates do not admit Charge training");
        check(train_skill_v1(slot_matrix_state, characters, skill_tables,
                  source_caps, charge_position, error), error);
        const auto charge_saved_row = static_cast<std::uint32_t>(charge_position);
        slot_matrix_state.skill_slots.push_back({0, 2, charge_saved_row});
        check(slot_matrix_state.skills[charge_saved_row].rank == 1 &&
              slot_matrix_state.source_skill_points == 3,
              "Original source progression did not train the saved Charge row for slot2");
        SkillVisualRequestV1 request;
        check(resolve_skill_visual_request_v1(state, characters, skill_tables, 0, request, error), error);
        check(request.skill_table_id == 7 && request.animation_sequence_id == 347 &&
              request.source_skill_name == "BashDown", "State did not resolve exact authored Headsplitter row");
        SkillVisualRequestV1 ground_slam_visual;
        check(resolve_skill_visual_request_v1(advanced_state, characters, skill_tables,
                  ground_slam_position, ground_slam_visual, error), error);
        check(ground_slam_visual.source_skill_name == "GroundSlam" &&
              ground_slam_visual.source_script == "prince_warrior_ground_slam",
              "Source GroundSlam class position did not resolve its authored SkillTable script");

        const auto anim_root = std::string("original-cache/data/pydata/");
        const auto animation_rows = assets.read(anim_root + "animations_pyarray.bin");
        const auto animation_names = assets.read(anim_root + "animations_pyarraynames.bin");
        const auto animation_schema = assets.read(anim_root + "animations_pystructnames.bin");
        const auto dictionary_values = assets.read(anim_root + "animations_dictionary_pyarray.bin");
        const auto dictionary_names = AssetCatalog(repo / ".local-inputs/actors").read(
            "animations_dictionary_pyarraynames.bin");
        dh2::data::Dictionary dictionary;
        dh2::data::AnimationTables animation_tables;
        check(dh2::data::load_dictionary(bytes(dictionary_names), bytes(dictionary_values), dictionary, error), error);
        check(dh2::data::load_animation_tables(bytes(animation_rows), bytes(animation_names),
                                               bytes(animation_schema), dictionary,
                                               animation_tables, error), error);
        dh2::data::PropertyRules property_rules;
        check(dh2::data::load_property_rules(properties.characters, property_rules, error), error);
        check_skill_use_profiles_v1(skill_tables, properties.classes);
        run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
            characters, skill_tables, animation_tables, dictionary, property_rules,
            {"JumpKick", "prince_rogue_jump_kick", "Skill_Rogue_JumpKick", 521, 1, 1, true, true, false, 1});
        // v21.8 ES-API integration: the admitted JumpKick row plus presentation arms (test variant only; the shipped table row is
        // unchanged). Sound: Figments' nightmare label; FX: the ColdRay elem_dmg_02_water set (both staged EffectsTables names).
        static RuntimeSkillUseProfileV1 jump_kick_presentation = *find_skill_use_profile_v1("JumpKick", "prince_rogue_jump_kick");
        jump_kick_presentation.use_sound_name = "sfx_skill_mage_ilu_nightmare";
        jump_kick_presentation.self_fx_name = "elem_dmg_02_water";
        jump_kick_presentation.target_fx_each_name = "elem_dmg_02_water";
        run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
            characters, skill_tables, animation_tables, dictionary, property_rules,
            {"JumpKick", "prince_rogue_jump_kick", "Skill_Rogue_JumpKick", 521, 1, 1, true, true, false, 1,
             false, "RoguePlayerBase", "Skill_Rogue_Quickness_buff", 4000.0, false, "RoguePlayerBase",
             "_default_rogue-mesh-skin", true, false, &jump_kick_presentation, true});
        // v21.8 skills-a: ViciousStrike (OnPre search + Use requery 160/180deg) and
        // Roundhouse (OnPre list reused at Use). Same full-session path as JumpKick.
        run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
            characters, skill_tables, animation_tables, dictionary, property_rules,
            {"ViciousStrike", "prince_rogue_vicious_strike", "Skill_Rogue_ViciousStrike_Stun", 528, 2, 3, true, true, true, 3});
        run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
            characters, skill_tables, animation_tables, dictionary, property_rules,
            {"Roundhouse", "prince_rogue_roundhouse", "Skill_Rogue_Roundhouse", 525, 2, 9, true, true, true, 1});
        // Quickness: timed buff (class Skill_Rogue_Quickness_buff, FX set spell_dh2_prince_rogue_quickness), level 6 row.
        run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
            characters, skill_tables, animation_tables, dictionary, property_rules,
            {"Quickness", "prince_rogue_quickness", "Skill_Rogue_Quickness", 524, 0, 6, true, true, false, 1, true,
             "RoguePlayerBase", "Skill_Rogue_Quickness_buff", 4000.0, true});
        // v21.8 skills-rogue: Assassin subclass tree (class row RoguePlayerBase_Assassin, base-class visual profile).
        // Blindness: SnS_Duration 8000+2000/level ms; PoisonSting: Dot_Duration 3000+750/level ms (cell 125). Rank 1 values.
        run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
            characters, skill_tables, animation_tables, dictionary, property_rules,
            {"Blindness", "prince_assassin_blindness", "Skill_Assassin_Blindness", 518, 0, 15, true, true, false, 1, true,
             "RoguePlayerBase_Assassin", "Skill_Assassin_Blindness", 10000.0, false, "RoguePlayerBase_Assassin", "_default_rogue-mesh-skin", false});
        run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
            characters, skill_tables, animation_tables, dictionary, property_rules,
            {"PoisonSting", "prince_assassin_poison_sting", "Skill_Assassin_PoisonSting", 520, 0, 18, true, true, false, 1, true,
             "RoguePlayerBase_Assassin", "Skill_Assassin_PoisonSting", 3750.0, false, "RoguePlayerBase_Assassin"});
        // v21.8 skills-mage-sub: Necromancer DeathClaw (SkillList of the MagePlayerBase_Necromancer row, root 415, level 21)
        // and Illusionist Figments (MagePlayerBase_Illusionist row, root 417, level 18). Same full-session path.
        run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
            characters, skill_tables, animation_tables, dictionary, property_rules,
            {"DeathClaw", "prince_necromancer_deathclaw", "Skill_Necromancer_DeathClaw", 415, 1, 21, true, true, false, 1,
             false, "MagePlayerBase_Necromancer", "", 0.0, false, "MagePlayerBase_Necromancer", "_default_mage-mesh-skin"});
        run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
            characters, skill_tables, animation_tables, dictionary, property_rules,
            {"Figments", "prince_illusionist_figments", "Skill_Illusionist_Figments", 417, 1, 18, true, true, false, 1,
             false, "MagePlayerBase_Illusionist", "", 0.0, false, "MagePlayerBase_Illusionist", "_default_mage-mesh-skin", false, true});
        // One-hand dual-wield paths: only the equipped hand hits (main: row mask; off: mask|0x4000000), once per target.
        for (const bool main_only : {true, false}) {
            run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
                characters, skill_tables, animation_tables, dictionary, property_rules,
                {"ViciousStrike", "prince_rogue_vicious_strike", "Skill_Rogue_ViciousStrike_Stun", 528, 1, 3, main_only, !main_only, true, 3});
            run_rogue_source_slot0_fixture(scratch, assets, properties, bindings,
                characters, skill_tables, animation_tables, dictionary, property_rules,
                {"Roundhouse", "prince_rogue_roundhouse", "Skill_Rogue_Roundhouse", 525, 1, 9, main_only, !main_only, true, 1});
        }
        std::cout << "PASS PC visual mapping regression: key1 rejects empty source2; "
                     "key2 selects middle-circle source0 JumpKick row0/root521.\n";

        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan base_plan;
        check(build_original_combat_visual_plan(assets, bindings, "KnightPlayerBase", customization,
                                                "same-generic-actor", base_plan, error), error);
        auto visual_config = base_plan.config;
        visual_config.motion_node_id = "auto";
        visual_config.consume_root_motion = true;
        RuntimeSkillAnimationBankRequestV1 animation_bank_request;
        animation_bank_request.character = &state;
        animation_bank_request.characters = &characters;
        animation_bank_request.skills = skill_tables;
        animation_bank_request.assets = &assets;
        animation_bank_request.animations = &animation_tables;
        animation_bank_request.animation_dictionary = &dictionary;
        animation_bank_request.same_actor_visual = &visual_config;
        animation_bank_request.actor_role = "same-generic-actor";
        animation_bank_request.equipment_set = 0;
        animation_bank_request.preload_current_class_roots = true;
        animation_bank_request.source_animation_table_id = dh2_character_animation_table_id(
            level_three_source.sheets.resolved[2],
            static_cast<std::int32_t>(animation_tables.characters.size()));
        RuntimeSkillAnimationBankV1 runtime_bank;
        check(build_runtime_skill_animation_bank_v1(animation_bank_request, base_plan,
                  runtime_bank, error), error);
        const auto preloaded_ground_slam = std::find_if(runtime_bank.class_skill_roots.begin(),
            runtime_bank.class_skill_roots.end(), [&](const auto& entry) {
                return entry.class_skill_position == ground_slam_position &&
                    entry.skill_table_id == ground_slam_visual.skill_table_id;
            });
        const auto preloaded_charge = std::find_if(runtime_bank.class_skill_roots.begin(),
            runtime_bank.class_skill_roots.end(), [&](const auto& entry) {
                return entry.class_skill_position == charge_position &&
                    entry.source_skill_name == "Charge";
            });
        check(runtime_bank.hotbar[0] && !runtime_bank.hotbar[1] && !runtime_bank.hotbar[2] &&
              runtime_bank.hotbar[0]->skill.skill_table_id == request.skill_table_id &&
              runtime_bank.hotbar[0]->selection_state ==
                  skills_animation::skill_sequence_state(request.animation_sequence_id) &&
              runtime_bank.class_roots_preloaded && preloaded_ground_slam != runtime_bank.class_skill_roots.end() &&
              preloaded_ground_slam->loaded && preloaded_ground_slam->animation_sequence_id ==
                  ground_slam_visual.animation_sequence_id && preloaded_charge != runtime_bank.class_skill_roots.end() &&
              preloaded_charge->loaded &&
              state.skills[static_cast<std::size_t>(ground_slam_position)].rank == 0 &&
              state.skills[static_cast<std::size_t>(charge_position)].rank == 0,
              "Pre-init class bank did not preload exact unlearned GroundSlam/Charge roots independent of rank/slot eligibility");
        const auto missing_asset_root = scratch / "missing-animation-bank";
        std::filesystem::remove_all(missing_asset_root);
        const auto assigned_bash_uri = std::string(
            "data/3D/characters/prince/animations/skill_dh2_prince_warrior_bash_down.bdae");
        const auto assigned_bash_bytes = assets.read(
            "original-cache/data/3d/characters/prince/animations/skill_dh2_prince_warrior_bash_down.bdae");
        const auto assigned_bash_destination = missing_asset_root /
            "original-cache/data/3d/characters/prince/animations/skill_dh2_prince_warrior_bash_down.bdae";
        std::filesystem::create_directories(assigned_bash_destination.parent_path());
        {
            std::ofstream output_file(assigned_bash_destination, std::ios::binary);
            check(static_cast<bool>(output_file), "Could not create isolated assigned-root asset fixture");
            output_file.write(reinterpret_cast<const char*>(assigned_bash_bytes.data()),
                              static_cast<std::streamsize>(assigned_bash_bytes.size()));
            check(static_cast<bool>(output_file), "Could not stage the exact assigned BashDown BDAE");
        }
        AssetCatalog missing_optional_assets(missing_asset_root);
        auto missing_optional_request = animation_bank_request;
        missing_optional_request.assets = &missing_optional_assets;
        missing_optional_request.source_animation_table_id.reset();
        RuntimeSkillAnimationBankV1 missing_optional_bank;
        check(build_runtime_skill_animation_bank_v1(missing_optional_request, base_plan,
                  missing_optional_bank, error), error);
        const auto missing_unlearned_root = std::find_if(
            missing_optional_bank.class_skill_roots.begin(),
            missing_optional_bank.class_skill_roots.end(), [&](const auto& entry) {
                return entry.class_skill_position == ground_slam_position &&
                    entry.skill_table_id == ground_slam_visual.skill_table_id;
            });
        check(missing_optional_bank.hotbar[0] &&
              missing_optional_bank.hotbar[0]->skill.source_skill_name == "BashDown" &&
              missing_unlearned_root != missing_optional_bank.class_skill_roots.end() &&
              !missing_unlearned_root->loaded && !missing_unlearned_root->diagnostic.empty() &&
              std::any_of(missing_optional_bank.programs.plan.sequences.begin(),
                  missing_optional_bank.programs.plan.sequences.end(), [](const auto& sequence) {
                      return sequence.id == 347;
                  }),
              "Missing unlearned animation assets downgraded a valid assigned root or lost their explicit diagnostic");
        std::filesystem::remove_all(missing_asset_root);
        animation_bank_request.character = &slot_matrix_state;
        animation_bank_request.preload_current_class_roots = false;
        RuntimeSkillAnimationBankV1 slot_matrix_bank;
        check(build_runtime_skill_animation_bank_v1(animation_bank_request, base_plan,
                  slot_matrix_bank, error), error);
        check(slot_matrix_bank.hotbar[0] && slot_matrix_bank.hotbar[1] &&
              slot_matrix_bank.hotbar[2] && slot_matrix_bank.hotbar[0]->skill.skill_table_id ==
                  request.skill_table_id && slot_matrix_bank.hotbar[1]->skill.skill_table_id ==
                  ground_slam_visual.skill_table_id &&
              slot_matrix_bank.hotbar[2]->skill.class_skill_position == charge_position &&
              slot_matrix_bank.hotbar[2]->skill.source_skill_name == "Charge" &&
              slot_matrix_bank.hotbar[0]->selection_state ==
                  skills_animation::skill_sequence_state(request.animation_sequence_id) &&
              slot_matrix_bank.hotbar[1]->selection_state ==
                  skills_animation::skill_sequence_state(ground_slam_visual.animation_sequence_id),
              "Source bank lost exact populated saved-hotbar row identity");
        CharacterState empty_middle_state = slot_matrix_state;
        empty_middle_state.skill_slots.erase(std::remove_if(
            empty_middle_state.skill_slots.begin(), empty_middle_state.skill_slots.end(),
            [](const SkillSlotBinding& binding) { return binding.equipment_set == 0 && binding.slot == 1; }),
            empty_middle_state.skill_slots.end());
        animation_bank_request.character = &empty_middle_state;
        RuntimeSkillAnimationBankV1 empty_middle_bank;
        check(build_runtime_skill_animation_bank_v1(animation_bank_request, base_plan,
                  empty_middle_bank, error), error);
        int empty_middle_position = -1;
        std::uint32_t empty_middle_row = UINT32_MAX;
        check(!resolve_assigned_skill_position_v1(empty_middle_state, skill_tables,
                  0, 1, empty_middle_position, empty_middle_row, error) &&
              error.find("no valid saved skill row") != std::string::npos,
              "Empty middle source hotbar slot shifted to a neighboring saved skill");
        int third_slot_position = -1;
        std::uint32_t third_slot_row = UINT32_MAX;
        check(resolve_assigned_skill_position_v1(empty_middle_state, skill_tables,
                  0, 2, third_slot_position, third_slot_row, error), error);
        check(empty_middle_bank.hotbar[0] && !empty_middle_bank.hotbar[1] &&
              empty_middle_bank.hotbar[2] && third_slot_position == charge_position &&
              third_slot_row == charge_saved_row &&
              empty_middle_bank.hotbar[2]->skill.source_skill_name == "Charge" &&
              empty_middle_bank.hotbar[2]->skill.animation_sequence_id ==
                  slot_matrix_bank.hotbar[2]->skill.animation_sequence_id,
              "Source slot2 did not retain its assigned Charge row/root when slot1 was empty");
        RuntimeSkillAnimationSlotV1 resolved_slot2;
        check(resolve_runtime_skill_animation_slot_v1(empty_middle_state, characters,
                  skill_tables, 0, 2, runtime_bank, resolved_slot2, error), error);
        check(resolved_slot2.skill.source_skill_name == "Charge" &&
              resolved_slot2.skill.saved_skill_row == static_cast<int>(charge_saved_row) &&
              resolved_slot2.selection_state == preloaded_charge->selection_state,
              "Fresh saved-slot resolver did not preserve slot2 Charge through the preloaded class root");
        animation_bank_request.character = &state;
        animation_bank_request.preload_current_class_roots = true;
        check(runtime_bank.active_faery_cast_sequence &&
              runtime_bank.active_faery_cast_selection_state ==
                  skills_animation::skill_sequence_state(*runtime_bank.active_faery_cast_sequence) &&
              runtime_bank.source_animation_table_id == animation_bank_request.source_animation_table_id &&
              std::all_of(runtime_bank.faery_cast_slots.begin(),
                  runtime_bank.faery_cast_slots.end(), [&](const auto& slot) {
                      return slot.source_animation_table_id == *animation_bank_request.source_animation_table_id &&
                          slot.faery_slot >= 0 && slot.faery_slot < 5 &&
                          slot.animation_sequence_id == animation_tables.characters[
                              static_cast<std::size_t>(*animation_bank_request.source_animation_table_id)]
                              .fields[31][static_cast<std::size_t>(slot.faery_slot)];
                  }) &&
              runtime_bank.programs.plan.sequences.front().id == 347 &&
              runtime_bank.programs.plan.sequences.front().type == 0 &&
              runtime_bank.programs.plan.sequences.front().loop == 0 &&
              std::any_of(runtime_bank.programs.plan.sequences.begin(), runtime_bank.programs.plan.sequences.end(),
                  [&](const auto& sequence) { return sequence.id == ground_slam_visual.animation_sequence_id; }) &&
              std::any_of(runtime_bank.programs.plan.sequences.begin(), runtime_bank.programs.plan.sequences.end(),
                  [&](const auto& sequence) { return sequence.id == preloaded_charge->animation_sequence_id; }),
              "Exact source BashDown, unlearned GroundSlam/Charge and authored Spells[0..4] roots/policies were not compiled");
        CharacterState faery_slot_zero_state = state;
        faery_slot_zero_state.source_faery_state_known = true;
        faery_slot_zero_state.faery_by_difficulty[0].current_faery = 0;
        RuntimeSkillFaeryAnimationSlotV1 resolved_faery_zero;
        check(resolve_runtime_faery_animation_slot_v1(faery_slot_zero_state, 0,
                  runtime_bank, resolved_faery_zero, error), error);
        check(resolved_faery_zero.faery_slot == 0 && resolved_faery_zero.loaded &&
              resolved_faery_zero.animation_sequence_id == runtime_bank.faery_cast_slots[0].animation_sequence_id &&
              resolved_faery_zero.selection_state == skills_animation::skill_sequence_state(
                  resolved_faery_zero.animation_sequence_id),
              "Fresh source current-Faery slot0 did not resolve its own preloaded Spells[0] Cast root");
        CharacterState faery_slot_four_state = state;
        faery_slot_four_state.source_faery_state_known = true;
        faery_slot_four_state.faery_by_difficulty[0].current_faery = 4;
        RuntimeSkillFaeryAnimationSlotV1 resolved_faery_four;
        check(resolve_runtime_faery_animation_slot_v1(faery_slot_four_state, 0,
                  runtime_bank, resolved_faery_four, error), error);
        check(resolved_faery_four.faery_slot == 4 && resolved_faery_four.loaded &&
              resolved_faery_four.animation_sequence_id == runtime_bank.faery_cast_slots[4].animation_sequence_id,
              "Fresh source current-Faery slot4 did not resolve its distinct authored Spells[4] Cast root");
        OriginalCombatVisualPlan composed_plan = base_plan;
        OriginalSequencePolicies composed_policies;
        check(merge_runtime_skill_animation_bank_v1(runtime_bank, composed_plan,
                  composed_policies, error), error);
        check(composed_plan.sequence(runtime_bank.hotbar[0]->selection_state, 0) &&
              composed_plan.sequence(preloaded_ground_slam->selection_state, 0) &&
              composed_plan.sequence(preloaded_charge->selection_state, 0) &&
              composed_plan.sequence(runtime_bank.active_faery_cast_selection_state, 0) &&
              composed_policies.count(request.animation_sequence_id) == 1 &&
              composed_policies.count(ground_slam_visual.animation_sequence_id) == 1,
              "Source roots were not merged into the actor plan before visual initialization");

        CombatSessionConfig config;
        config.diagnosticRngSeed = source_rng_seed;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = composed_plan.config;
        CombatSessionProfile profile;
        profile.initialIdle = {"Idle", 0, {0}};
        profile.sequenceAction = OriginalAttackSelection{};
        profile.sequenceAction->state = "AttackStatic";
        profile.retainedPhaseClock = true;
        profile.damageMarkerNames = {"attack_mainhand"};
        profile.propertyOptions = {256, true};
        profile.sourceAnimationClips = composed_plan.config.clips;
        config.profiles.emplace(config.playerProfileId, profile);
        CombatSessionProfile lizard;
        lizard.action = {"Attack", 0, {0, 1}};
        lizard.initialIdle = {"Idle", 0, {0}};
        lizard.death = CombatSessionChoice{"Died", 0, {0}};
        lizard.damageMarkerNames = {"attack_mainhand"};
        lizard.propertyOptions = {std::nullopt, true};
        lizard.customization.allow_missing_animation_targets = true;
        config.profiles.emplace("Swamp_LizadMan_Type1", lizard);

        CombatSession session;
        CharacterVisual player_visual;
        ActorPopulation population;
        for (const auto id : {2u, 3u}) {
            PopulationActor placed;
            placed.profileId = "Swamp_LizadMan_Type1";
            placed.definition.stableId = id;
            placed.definition.sourceId = "source-skill-target-fixture-" + std::to_string(id);
            placed.definition.placement = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
            placed.transform = placed.definition.placement;
            population.actors().push_back(std::move(placed));
        }
        check(session.initialize(assets, properties, bindings, config, player_visual, population,
                                 {0, 0, 0}, customization, error), error);
        const auto* initial_player_source = session.world()->combat_properties(1);
        check(initial_player_source, "Connected source player has no original combat-property sheet");
        check((initial_player_source->sheets.resolved[19] >> 8) == 1 &&
              (initial_player_source->sheets.resolved[157] >> 8) == 1,
              "Fresh source Knight seed no longer exposes level 1 and its original initial skill point");
        const auto* pose_owner = session.retained_actor_pose(1);
        check(pose_owner, "Asset-backed CombatSession has no retained player pose owner");


        // Same-session BashDown query: the farther target with the smaller
        // source facing angle wins under the recovered FrontalFirst heap.
        auto* caster_for_target = session.actor(1);
        auto* nearer_side = session.actor(2);
        auto* farther_front = session.actor(3);
        check(caster_for_target && nearer_side && farther_front,
              "Connected skill target Session actors are missing");
        caster_for_target->transform.position = {0, 0, 0};
        caster_for_target->transform.rotation[2] = 0.0f;
        nearer_side->transform.position = {20, -50, 0};
        farther_front->transform.position = {20, -110, 0};
        const auto* caster_traits = session.world()->traits(1);
        const auto* enemy2_traits = session.world()->traits(2);
        const auto* enemy3_traits = session.world()->traits(3);
        check(caster_traits && enemy2_traits && enemy3_traits,
              "Connected skill target source traits are missing");
        auto caster_target_properties = *session.world()->combat_properties(1);
        auto enemy2_properties = *session.world()->combat_properties(2);
        auto enemy3_properties = *session.world()->combat_properties(3);
        caster_target_properties.sheets.resolved[199] = 0;
        enemy2_properties.sheets.resolved[198] = 0;
        enemy3_properties.sheets.resolved[198] = 0;
        check(session.world()->update_combat_properties(1, caster_target_properties,
                                                        *caster_traits, error), error);
        check(session.world()->update_combat_properties(2, enemy2_properties,
                                                        *enemy2_traits, error), error);
        check(session.world()->update_combat_properties(3, enemy3_properties,
                                                        *enemy3_traits, error), error);
        check(session.world()->eligible_target(*caster_for_target, *nearer_side) &&
              session.world()->eligible_target(*caster_for_target, *farther_front),
              "Source target test enemies are not actual eligible same-world actors");
        const std::vector<ActorId> source_population_order{2, 3};
        std::vector<SkillTargetSourceFactsV1> source_target_facts{
            {2, true, false, {}, {}, true},
            {3, true, false, {}, {}, true}};
        SkillActorTargetQueryV1 target_query;
        check(query_bashdown_character_targets_v1(session, 1, source_population_order,
              source_target_facts, true, target_query, error), error);
        check(target_query.status == SkillActorTargetQueryStatusV1::selected_character &&
              target_query.selected == 3 && target_query.ordered_character_targets ==
                  std::vector<ActorId>{3, 2} && target_query.source_character_order_preserved,
              "Original FrontalFirst sort did not prefer the farther, more frontal same-world actor");
        SkillActorTargetQueryV1 source_use_query;
        check(query_source_character_targets_v1(session, 1, source_population_order,
                  source_target_facts, true, 300.0f,
                  3.1415927410125732421875f, SkillTargetSortV1::closest_first,
                  source_use_query, error), error);
        check(source_use_query.status == SkillActorTargetQueryStatusV1::selected_character &&
              source_use_query.ordered_character_targets == std::vector<ActorId>{2, 3},
              "GroundSlam source ClosestFirst target loop did not retain the actual closest-first order");
        check(query_source_character_targets_v1(session, 1, source_population_order,
                  source_target_facts, true, 300.0f,
                  native_search_max_angle_v1(120.0f), SkillTargetSortV1::source_order,
                  source_use_query, error), error);
        check(source_use_query.status == SkillActorTargetQueryStatusV1::selected_character &&
              source_use_query.ordered_character_targets == std::vector<ActorId>{2, 3} &&
              source_use_query.source_character_order_preserved,
              "Charge source NoSort/120-degree query changed the authored population order");

        // Charge has a distinct Use-time search. Preserve the exact Pre
        // selection/LookAt heading, then model the authored root-motion branch
        // passing that target before do_skill. The target remains in adjusted
        // range, but is now behind the same heading; the 120-degree Use cone
        // must reject it. A second same-world Character ahead remains eligible
        // in population order. This is source behavior, not a widened cone or
        // reuse of the Pre Lua-local list.
        caster_for_target->transform.position = {0, 0, 0};
        caster_for_target->transform.rotation[2] = 0.0f;
        nearer_side->transform.position = {0, -50, 0};
        farther_front->transform.position = {0, -1000, 0};
        SkillActorTargetQueryV1 charge_pre_query;
        check(query_source_character_targets_v1(session, 1, source_population_order,
                  source_target_facts, true, 300.0f,
                  3.1415927410125732421875f, SkillTargetSortV1::frontal_first,
                  charge_pre_query, error), error);
        check(charge_pre_query.status == SkillActorTargetQueryStatusV1::selected_character &&
              charge_pre_query.selected == 2 &&
              charge_pre_query.ordered_character_targets == std::vector<ActorId>{2},
              "Charge Pre did not select the sole in-range source actor before motion");
        check(look_at_bashdown_target_v1(session, charge_pre_query, error), error);
        check(charge_pre_query.look_at_heading_known &&
              std::fabs(caster_for_target->transform.rotation[2]) < 0.0001f,
              "Charge Pre LookAt did not retain the exact +Y source heading");

        caster_for_target->transform.position = {0, -100, 0};
        farther_front->transform.position = {0, -200, 0};
        SkillActorTargetQueryV1 charge_range_control;
        check(query_source_character_targets_v1(session, 1, source_population_order,
                  source_target_facts, true, 300.0f,
                  3.1415927410125732421875f, SkillTargetSortV1::source_order,
                  charge_range_control, error), error);
        check(charge_range_control.status == SkillActorTargetQueryStatusV1::selected_character &&
              charge_range_control.ordered_character_targets == std::vector<ActorId>{2, 3} &&
              charge_range_control.selected_distance < 300.0f,
              "Charge Use full-cone control did not retain the passed target within source-adjusted range");
        const std::vector<ActorId> passed_target_only{2};
        const std::vector<SkillTargetSourceFactsV1> passed_target_fact{source_target_facts[0]};
        SkillActorTargetQueryV1 charge_behind_only;
        check(query_source_character_targets_v1(session, 1, passed_target_only,
                  passed_target_fact, true, 300.0f,
                  native_search_max_angle_v1(120.0f), SkillTargetSortV1::source_order,
                  charge_behind_only, error), error);
        check(charge_behind_only.status == SkillActorTargetQueryStatusV1::no_actor_target &&
              charge_behind_only.ordered_character_targets.empty(),
              "Charge Use did not source-filter an in-range target behind its current heading");
        SkillActorTargetQueryV1 charge_use_after_motion;
        check(query_source_character_targets_v1(session, 1, source_population_order,
                  source_target_facts, true, 300.0f,
                  native_search_max_angle_v1(120.0f), SkillTargetSortV1::source_order,
                  charge_use_after_motion, error), error);
        check(charge_use_after_motion.status == SkillActorTargetQueryStatusV1::selected_character &&
              charge_use_after_motion.ordered_character_targets == std::vector<ActorId>{3} &&
              charge_use_after_motion.selected == 3 &&
              charge_use_after_motion.selected_angle < 0.0001f &&
              charge_use_after_motion.source_character_order_preserved,
              "Charge Use did not exclude the passed target while retaining the next in-front actor");
        caster_for_target->transform.position = {0, 0, 0};
        caster_for_target->transform.rotation[2] = 0.0f;
        nearer_side->transform.position = {20, -50, 0};
        farther_front->transform.position = {20, -110, 0};

        auto stale_order_facts = source_target_facts;
        stale_order_facts.push_back({99, true, false, {}, {}, true});
        const std::vector<ActorId> stale_order{99, 2, 3};
        check(!query_source_character_targets_v1(session, 1, stale_order,
                  stale_order_facts, true, 300.0f,
                  3.1415927410125732421875f, SkillTargetSortV1::source_order,
                  source_use_query, error) &&
              error.find("absent from this Session world") != std::string::npos,
              "Direct source query stopped rejecting an explicitly supplied stale actor order");
        const auto* source_ai3 = dh2::data::ai_props(
            session.world()->factions(), enemy3_properties.sheets.resolved[1]);
        check(source_ai3, "Original source AI row for candidate radius is missing");
        const float expected_distance = std::sqrt(20.0f * 20.0f + 110.0f * 110.0f) -
            source_ai3->interact_radius - session.world()->target_radius(*caster_for_target);
        check(std::fabs(target_query.selected_distance - expected_distance) < 0.0001f,
              "BashDown range did not use the original Character AI interaction radius");
        check(caster_for_target->target_id == invalid_actor_id,
              "BashDown Lua-local query incorrectly wrote the shared combat target field");
        const float original_heading = caster_for_target->transform.rotation[2];
        check(look_at_bashdown_target_v1(session, target_query, error), error);
        check(target_query.look_at_heading_known &&
              caster_for_target->transform.rotation[2] == target_query.look_at_heading &&
              caster_for_target->transform.rotation[2] != original_heading &&
              caster_for_target->target_id == invalid_actor_id,
              "Source LookAt(Point) did not update only the same actor facing field");

        auto missing_visible = source_target_facts;
        missing_visible[0].visible.reset();
        SkillActorTargetQueryV1 unavailable_target_query;
        check(!query_bashdown_character_targets_v1(session, 1, source_population_order,
              missing_visible, true, unavailable_target_query, error) &&
              error.find("visible") != std::string::npos &&
              unavailable_target_query.status == SkillActorTargetQueryStatusV1::unavailable,
              "Missing reached source visibility did not fail closed");
        auto invisible_side = source_target_facts;
        invisible_side[0].visible = false;
        check(query_bashdown_character_targets_v1(session, 1, source_population_order,
              invisible_side, true, unavailable_target_query, error), error);
        check(unavailable_target_query.status == SkillActorTargetQueryStatusV1::selected_character &&
              unavailable_target_query.selected == 3,
              "Source-invisible candidate reached later GameObject predicates or displaced the visible target");
        auto zoned_front = source_target_facts;
        zoned_front[1].zonable = true;
        zoned_front[1].zoned = true;
        zoned_front[1].in_zone = false;
        check(query_bashdown_character_targets_v1(session, 1, source_population_order,
              zoned_front, true, unavailable_target_query, error), error);
        check(unavailable_target_query.status == SkillActorTargetQueryStatusV1::selected_character &&
              unavailable_target_query.selected == 2,
              "Source zoned-out candidate was not rejected before interaction testing");
        auto not_zoned_front = source_target_facts;
        not_zoned_front[1].zonable = true;
        not_zoned_front[1].zoned = false;
        not_zoned_front[1].in_zone.reset();
        check(query_bashdown_character_targets_v1(session, 1, source_population_order,
              not_zoned_front, true, unavailable_target_query, error), error);
        check(unavailable_target_query.status == SkillActorTargetQueryStatusV1::selected_character &&
              unavailable_target_query.selected == 3,
              "Unreached source in-zone field was incorrectly required when the actor was not zoned");
        // Invalid Character AI IDs use the exact source ai_props fallback row8.
        auto invalid_ai_id = *session.world()->combat_properties(3);
        invalid_ai_id.sheets.resolved[1] = std::numeric_limits<std::int32_t>::max();
        check(session.world()->update_combat_properties(3, invalid_ai_id,
                                                        *enemy3_traits, error), error);
        check(query_bashdown_character_targets_v1(session, 1, source_population_order,
              source_target_facts, true, unavailable_target_query, error), error);
        const auto* fallback_ai8 = dh2::data::ai_props(session.world()->factions(), 8);
        check(fallback_ai8 && unavailable_target_query.status ==
                  SkillActorTargetQueryStatusV1::selected_character &&
              unavailable_target_query.selected == 3 &&
              std::fabs(unavailable_target_query.selected_distance -
                  (std::sqrt(20.0f * 20.0f + 110.0f * 110.0f) -
                   fallback_ai8->interact_radius - session.world()->target_radius(*caster_for_target))) < 0.0001f,
              "Invalid source Character AI ID did not use source fallback row8 interaction radius");
        check(session.world()->update_combat_properties(3, enemy3_properties,
                                                        *enemy3_traits, error), error);
        dh2::data::AiTables missing_fallback_ai;
        missing_fallback_ai.rows.resize(8);
        check(dh2::data::ai_props(missing_fallback_ai, -1) == nullptr,
              "Original AI lookup unexpectedly produced a radius without source fallback row8");
        nearer_side->transform.position = {800, 0, 0};
        farther_front->transform.position = {900, 0, 0};
        check(query_bashdown_character_targets_v1(session, 1, source_population_order,
              source_target_facts, true, unavailable_target_query, error), error);
        check(unavailable_target_query.status == SkillActorTargetQueryStatusV1::no_actor_target &&
              unavailable_target_query.selected == invalid_actor_id,
              "Source range rejection was not reported as a known actor no-target result");
        check(query_bashdown_character_targets_v1(session, 1, source_population_order,
              source_target_facts, std::nullopt, unavailable_target_query, error), error);
        check(unavailable_target_query.status == SkillActorTargetQueryStatusV1::non_character_target_unknown,
              "Actor-only no-target result incorrectly claimed non-character AttackableOnly absence");
        nearer_side->transform.position = {20, -50, 0};
        farther_front->transform.position = {20, -110, 0};
        caster_for_target->transform.position = {0, 0, 0};
        caster_for_target->transform.rotation[2] = 0.0f;
        caster_for_target->source_target_node180 = std::uintptr_t{0};
        caster_for_target->source_target_position184 = std::array<float, 3>{700, 700, 0};
        nearer_side->transform.position = {900, 0, 0};
        nearer_side->source_target_node180 = std::uintptr_t{0};
        nearer_side->source_target_position184 = std::array<float, 3>{0, 0, 0};
        const std::vector<ActorId> one_source_target{2};
        const std::vector<SkillTargetSourceFactsV1> one_source_fact{source_target_facts[0]};
        SkillActorTargetQueryV1 node_first_query;
        check(query_bashdown_character_targets_v1(session, 1, one_source_target,
                  one_source_fact, true, node_first_query, error), error);
        check(node_first_query.status == SkillActorTargetQueryStatusV1::no_actor_target &&
              node_first_query.selected == invalid_actor_id,
              "Null source node used its constructor-zeroed cache instead of transform+0x160");
        nearer_side->source_target_node180 = std::uintptr_t{0x1234};
        nearer_side->source_target_position184 = std::array<float, 3>{0, -50, 0};
        SkillActorTargetQueryV1 cached_node_query;
        check(query_source_character_targets_v1(session, 1, one_source_target,
                  one_source_fact, true, 160.0f,
                  3.1415927410125732421875f, SkillTargetSortV1::frontal_first,
                  cached_node_query, error), error);
        check(cached_node_query.status == SkillActorTargetQueryStatusV1::selected_character &&
              cached_node_query.selected == 2 &&
              cached_node_query.ordered_character_targets == std::vector<ActorId>{2},
              "Non-null source node did not select its authored cached target position+0x184");
        caster_for_target->transform.rotation[2] = 1.0f;
        check(look_at_bashdown_target_v1(session, cached_node_query, error), error);
        check(cached_node_query.look_at_heading_known &&
              std::fabs(caster_for_target->transform.rotation[2]) < 0.0001f,
              "LookAt target point did not use the cached point selected by its non-null source node");
        caster_for_target->source_target_position184.reset();
        nearer_side->source_target_node180 = std::uintptr_t{0};
        nearer_side->source_target_position184.reset();
        nearer_side->transform.position = {20, -50, 0};
        farther_front->transform.position = {20, -110, 0};
        caster_for_target->transform.rotation[2] = 0.0f;

        // Exercise source HasMana/UseMana on the same session-owned property
        // sheets. This validates a real preparation prefix only; target search
        // and SkillCombatRoll are not substituted by this fixture.
        auto* retained_actor = session.actor(1);
        const auto* retained_properties = session.world()->combat_properties(1);
        check(retained_actor && retained_properties, "Same-session player source property owner missing");
        retained_actor->persistent_character_id = state.id;
        state.stats.resource = retained_actor->resource;
        state.stats.max_resource = retained_actor->max_resource;
        dh2::data::PropertySheet max_cost_sheet = retained_properties->sheets.resolved;
        SkillManaCostV1 source_cost;
        check(evaluate_skill_mana_cost_v1(properties.classes, property_rules, max_cost_sheet,
                                          "Skill_Warrior_BashDown", state.skills[0].rank,
                                          source_cost, error), error);
        check(source_cost.fixed_mana_cost > 0 && source_cost.fixed_mana_cost <= max_cost_sheet[43],
              "Original BashDown cost does not fit the source MP maximum in this fixture");
        auto low_mp = *retained_properties;
        auto low_view = dh2::data::property_view(property_rules, low_mp.sheets);
        check(dh2_property_set(&low_view, 41, source_cost.fixed_mana_cost - 1) == 0,
              "Could not arrange an actual insufficient-MP source property state");
        const auto* live_traits = session.world()->traits(1);
        check(live_traits && session.world()->update_combat_properties(1, low_mp, *live_traits, error), error);
        retained_actor->resource = original_signed256(source_cost.fixed_mana_cost - 1);
        state.stats.resource = retained_actor->resource;
        SkillManaSourceFactsV1 mana_facts;
        mana_facts.application_byte5 = false;
        mana_facts.god_mana_registered = false;
        mana_facts.god_mana_enabled = false;
        mana_facts.character_byte14f0 = false;
        mana_facts.tracing_character_stats = true;
        int assigned_position = -1;
        std::uint32_t assigned_saved_row = UINT32_MAX;
        check(resolve_assigned_skill_position_v1(state, skill_tables, 0, 0,
                                                 assigned_position, assigned_saved_row, error), error);
        check(assigned_position == 0 && assigned_saved_row == 0,
              "NativeHUDSkill source slot0 did not preserve the original class-list position");
        check(state.skills.size() > 2 && state.skills[1].rank == 0 && state.skills[2].rank == 0,
              "Fresh-player source grant fixture unexpectedly learned nonstarter skill rows");
        state.skill_slots.push_back({0, 1, 1});
        int unlearned_position = -1;
        std::uint32_t unlearned_saved_row = UINT32_MAX;
        check(!resolve_assigned_skill_position_v1(state, skill_tables, 0, 1,
                  unlearned_position, unlearned_saved_row, error) &&
              error.find("not a learned") != std::string::npos,
              "A source-assigned rank-zero hotbar row was treated as a learned skill");
        state.skill_slots.pop_back();
        SkillManaPrepareResultV1 mana_result;
        check(prepare_skill_cast_mana_v1(state, session, 1, characters, skill_tables,
                                         properties.classes, property_rules, assigned_position,
                                         "Skill_Warrior_BashDown", mana_facts,
                                         mana_result, error), error);
        check(mana_result.status == SkillManaPrepareStatusV1::insufficient_mana &&
              mana_result.source_has_mana_check && !*mana_result.source_has_mana_check &&
              !mana_result.mana_spent && session.world()->combat_properties(1)->sheets.resolved[41] ==
                  source_cost.fixed_mana_cost - 1,
              "Source insufficient-mana gate did not reject without mutating the same property owner");

        auto exact_mp = *session.world()->combat_properties(1);
        auto exact_view = dh2::data::property_view(property_rules, exact_mp.sheets);
        check(dh2_property_set(&exact_view, 41, source_cost.fixed_mana_cost) == 0,
              "Could not arrange exact-cost source MP");
        check(session.world()->update_combat_properties(1, exact_mp, *live_traits, error), error);
        retained_actor->resource = original_signed256(source_cost.fixed_mana_cost);
        state.stats.resource = retained_actor->resource;
        check(prepare_skill_cast_mana_v1(state, session, 1, characters, skill_tables,
                                         properties.classes, property_rules, assigned_position,
                                         "Skill_Warrior_BashDown", mana_facts,
                                         mana_result, error), error);
        check(mana_result.status == SkillManaPrepareStatusV1::mana_committed &&
              mana_result.source_has_mana_check && *mana_result.source_has_mana_check &&
              mana_result.mana_spent && mana_result.mana_before == source_cost.fixed_mana_cost &&
              mana_result.mana_after == 0 &&
              session.world()->combat_properties(1)->sheets.resolved[41] == 0 &&
              retained_actor->resource == 0 && state.stats.resource == 0,
              "Source UseMana did not debit/publish the same player property and vital owners");

        // B083: real same-world object identities participate in source Search.
        WorldObject barrel;barrel.id=99;barrel.name="query-object";barrel.transform.position={0,-30,0};
        check(session.world()->bind_object(barrel,error),error);
        SkillObjectTargetArmV1 object_arm;object_arm.authored_order={99};object_arm.registry_order={99,2,3};
        SkillObjectTargetFactsV1 object_facts;object_facts.visible=true;object_facts.zonable=true;
        object_facts.zoned=false;object_facts.interactive=true;object_facts.interaction_type=8;object_facts.interaction_radius=10.f;
        object_arm.facts=[&](const WorldObject& object,ActorId caster,SkillObjectTargetFactsV1& out,std::string&){check(object.id==99&&caster==1,"Wrong live object borrower");out=object_facts;return true;};
        SkillActorTargetQueryV1 mixed;
        check(query_source_character_targets_v1(session,1,source_population_order,source_target_facts,{},160.f,3.14159274f,SkillTargetSortV1::frontal_first,mixed,error,&object_arm),error);
        check(mixed.status==SkillActorTargetQueryStatusV1::selected_character&&mixed.ordered_targets.back()==99,"Sorted query lost Character-before-object priority");
        nearer_side->transform.position={800,0,0};farther_front->transform.position={900,0,0};
        check(query_source_character_targets_v1(session,1,source_population_order,source_target_facts,{},160.f,3.14159274f,SkillTargetSortV1::frontal_first,mixed,error,&object_arm),error);
        check(mixed.status==SkillActorTargetQueryStatusV1::selected_object&&mixed.selected==99&&mixed.ordered_character_targets.empty(),"Actual attackable object not selected");
        object_facts.interactive=false;object_facts.interaction_type.reset();
        check(query_source_character_targets_v1(session,1,source_population_order,source_target_facts,{},160.f,3.14159274f,SkillTargetSortV1::frontal_first,mixed,error,&object_arm)&&mixed.status==SkillActorTargetQueryStatusV1::no_actor_target,error);
        object_facts.interactive=true;
        check(!query_source_character_targets_v1(session,1,source_population_order,source_target_facts,{},160.f,3.14159274f,SkillTargetSortV1::frontal_first,mixed,error,&object_arm)&&error.find("interaction type")!=std::string::npos,"Unknown reached source object type guessed");
        object_facts.interaction_type=0;
        check(query_source_character_targets_v1(session,1,source_population_order,source_target_facts,{},160.f,3.14159274f,SkillTargetSortV1::frontal_first,mixed,error,&object_arm)&&mixed.status==SkillActorTargetQueryStatusV1::no_actor_target,error);
        object_facts.interaction_type=8;object_facts.zoned=true;object_facts.in_zone=false;
        check(query_source_character_targets_v1(session,1,source_population_order,source_target_facts,{},160.f,3.14159274f,SkillTargetSortV1::frontal_first,mixed,error,&object_arm)&&mixed.status==SkillActorTargetQueryStatusV1::no_actor_target,error);
        object_facts.zoned=false;session.world()->find_object(99)->transform.position={1000,0,0};
        check(query_source_character_targets_v1(session,1,source_population_order,source_target_facts,{},160.f,3.14159274f,SkillTargetSortV1::frontal_first,mixed,error,&object_arm)&&mixed.status==SkillActorTargetQueryStatusV1::no_actor_target,error);
        object_arm.authored_order={99,99};
        check(!query_source_character_targets_v1(session,1,source_population_order,source_target_facts,{},160.f,3.14159274f,SkillTargetSortV1::frontal_first,mixed,error,&object_arm),"Duplicate object query accepted");
        object_arm.authored_order={99};object_facts.visible=false;object_facts.zonable.reset();
        check(query_source_character_targets_v1(session,1,source_population_order,source_target_facts,{},160.f,3.14159274f,SkillTargetSortV1::frontal_first,mixed,error,&object_arm)&&mixed.status==SkillActorTargetQueryStatusV1::no_actor_target,error);
        check(session.world()->remove_object(99),"Failed removing query object");
        nearer_side->transform.position={20,-50,0};farther_front->transform.position={20,-110,0};

        // The authored do_skill marker is the only source point that reaches
        // BashDown ApplyPropClass + SkillCombatRoll. Exercise the staged
        // coordinator against the original saved hotbar, same CharacterState,
        // live ActorPopulation order and this retained Session.
        const auto bashdown_id = skill_tables.skill_index("BashDown");
        check(bashdown_id == 7 && skill_tables.skill_field("ElementalType") == 3 &&
              skill_tables.skill_field("Flags") == 5,
              "Original SkillTable BashDown combat fields are unavailable");
        const auto& bashdown_row = skill_tables.skills()[static_cast<std::size_t>(bashdown_id)];
        const auto bashdown_element = static_cast<std::int32_t>(bashdown_row.scalar.words[5]);
        const auto bashdown_row_mask = bashdown_row.scalar.words[7];
        check(bashdown_element == -1 && bashdown_row_mask == 0x00335535u &&
              !(bashdown_row_mask & 0x00800000u),
              "BashDown source row no longer has its verified one-hand result request");
        const auto* cast_properties = session.world()->combat_properties(1);
        check(cast_properties && cast_properties->facts.main_damage_class == -1,
              "Empty current EquipmentSet did not resolve BashDown weapon category -1");
        auto full_mp = *cast_properties;
        auto full_mp_view = dh2::data::property_view(property_rules, full_mp.sheets);
        check(dh2_property_set(&full_mp_view, 41, full_mp.sheets.resolved[43]) == 0,
              "Could not restore the actual same-world MP owner for staged cast acceptance");
        check(session.world()->update_combat_properties(1, full_mp, *live_traits, error), error);
        retained_actor->resource = original_signed256(full_mp.sheets.resolved[41]);
        state.stats.resource = retained_actor->resource;
        const float target_health_before_bashdown = session.actor(target_query.selected)->health;

        unsigned marker_count = 0;
        unsigned completion_count = 0;
        RetainedAnimationEvent actual_source_event;
        CombatSessionStateAnimationServices callbacks;
        callbacks.event = [&](ActorId actor, const RetainedAnimationEvent& event,
                              std::string& callback_error) {
            check(actor == 1, "Skill marker reached a foreign session actor");
            if (event.name == "do_skill") {
                ++marker_count;
                actual_source_event = event;
            }
            callback_error.clear();
            return true;
        };
        callbacks.finished = [&](ActorId actor, std::string& failure) {
            check(actor == 1, "Skill completion reached a foreign session actor");
            ++completion_count;
            failure.clear();
            // Generic source completion resumes the retained Idle owner on
            // the Session's next update. The legacy state-leaf selector would
            // register a separate nonpersisted campaign lifecycle service.
            return true;
        };
        OriginalAttackSelection selection;
        selection.state = runtime_bank.hotbar[0]->selection_state;
        RuntimeSkillCastRequestV1 cast_request;
        cast_request.actor = 1;
        cast_request.character = &state;
        cast_request.equipment_set = 0;
        cast_request.source_slot = 0;
        cast_request.characters = &characters;
        cast_request.skills = skill_tables;
        cast_request.classes = &properties.classes;
        cast_request.property_rules = &property_rules;
        cast_request.population = &population;
        cast_request.mana_policy = mana_facts;
        cast_request.target_facts = source_target_facts;
        cast_request.non_character_attackable_objects_absent = true;
        cast_request.visual_plan = &composed_plan;
        cast_request.sequence_policies = &composed_policies;
        cast_request.selection = selection;
        cast_request.downstream_animation_services = callbacks;
        RuntimeSkillCastCoordinatorV1 cast_coordinator;
        RuntimeSkillCastReceiptV1 cast_receipt;
        GameSave b097_same_roll_checkpoint;
        check(capture_game_save("b097-same-roll", session.player_id(), state,
                  *session.world(), b097_same_roll_checkpoint, error), error);
        const auto mana_before_unlearned_rows = session.world()->combat_properties(1)->sheets.resolved[41];
        const auto rng_before_unlearned_rows = session.world()->random_state();
        for (const auto slot_position : {ground_slam_position, charge_position}) {
            const auto slot = slot_position == ground_slam_position ? 1u : 2u;
            state.skill_slots.push_back({0, slot, static_cast<std::uint32_t>(slot_position)});
            cast_request.source_slot = slot;
            RuntimeSkillCastReceiptV1 unlearned_receipt;
            check(!cast_coordinator.begin_skill_cast_v1(cast_request, session,
                      unlearned_receipt, error) && error.find("not a learned") != std::string::npos,
                  "Source rank-zero GroundSlam/Charge row passed the actual saved-rank cast gate");
            state.skill_slots.pop_back();
        }
        cast_request.source_slot = 0;
        const auto rng_after_unlearned_rows = session.world()->random_state();
        check(session.world()->combat_properties(1)->sheets.resolved[41] == mana_before_unlearned_rows &&
              rng_after_unlearned_rows.seed == rng_before_unlearned_rows.seed &&
              rng_after_unlearned_rows.calls == rng_before_unlearned_rows.calls &&
              retained_actor->action == CharacterAction::idle,
              "Unlearned source skill rejection changed MP, RNG or actor lifecycle");
        check(cast_coordinator.begin_skill_cast_v1(cast_request, session,
                                                   cast_receipt, error), error);
        check(cast_receipt.phase == RuntimeSkillCastPhaseV1::prepared_pending_use &&
              cast_receipt.skill_table_id == 7 && cast_receipt.class_skill_position == 0 &&
              cast_receipt.source_slot == 0 && cast_receipt.mana.mana_spent &&
              cast_receipt.target_order.size() == 2,
              "Accepted BashDown source Check/Pre did not retain exact slot, rank, target order and mana prefix");
        check(session.retained_actor_pose(1) == pose_owner,
              "Skill request replaced the existing session pose owner");
        const float hp_before_wrong_marker = session.actor(target_query.selected)->health;
        const auto rng_before_wrong_marker = session.world()->random_state();
        auto wrong_name = actual_source_event;
        wrong_name.name = "do_spell";
        RuntimeSkillCastReceiptV1 wrong_marker_receipt;
        check(!cast_coordinator.apply_retained_use_event_v1(session, 1,
                  cast_receipt.generation, RuntimeSkillSourceAnimStateV1::skill,
                  wrong_name, wrong_marker_receipt, error) &&
              error.find("state6/state7") != std::string::npos,
              "NativeHUDSkill accepted a state6 event with the Hotty do_spell name");
        auto wrong_state = actual_source_event;
        wrong_state.name = "do_skill";
        check(!cast_coordinator.apply_retained_use_event_v1(session, 1,
                  cast_receipt.generation, RuntimeSkillSourceAnimStateV1::cast,
                  wrong_state, wrong_marker_receipt, error) &&
              error.find("state6/state7") != std::string::npos,
              "NativeHUDSkill accepted its do_skill marker as a state7 Cast event");
        retained_actor->action = CharacterAction::hurt;
        check(!cast_coordinator.apply_retained_use_event_v1(session, 1,
                  cast_receipt.generation, RuntimeSkillSourceAnimStateV1::skill,
                  wrong_state, wrong_marker_receipt, error) &&
              error.find("interrupted cast owner") != std::string::npos,
              "NativeHUDSkill accepted a retained marker outside the same actor's Cast lifecycle");
        retained_actor->action = CharacterAction::casting;
        const auto rng_after_wrong_marker = session.world()->random_state();
        check(session.actor(target_query.selected)->health == hp_before_wrong_marker &&
              rng_after_wrong_marker.seed == rng_before_wrong_marker.seed &&
              rng_after_wrong_marker.calls == rng_before_wrong_marker.calls,
              "A wrong event/state/lifecycle rejection changed HP or combat RNG");
        for (unsigned frame = 0; frame < 180 && completion_count == 0; ++frame) {
            check(session.update(1.0 / 60.0, {}, {0, 0, 0}, 0.0f, error), error);
            check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        }
        check(marker_count == 1 && completion_count == 1 && session.retained_actor_pose(1) == pose_owner,
              "Exact source root did not deliver one do_skill marker and complete on its retained owner");
        const auto* final_cast = cast_coordinator.receipt(1);
        check(final_cast && final_cast->phase == RuntimeSkillCastPhaseV1::completed &&
              final_cast->applied_results.size() == 1,
              "BashDown source Use/Post did not complete one exact result receipt");
        const auto& bashdown_receipt = final_cast->applied_results.front();
        check(bashdown_receipt.applied && bashdown_receipt.attacker == 1 &&
              bashdown_receipt.target == target_query.selected &&
              bashdown_receipt.source_id == "BashDown" && bashdown_receipt.marker_name == "do_skill" &&
              bashdown_receipt.source_mask == (bashdown_row_mask | 0x08000000u) &&
              bashdown_receipt.health_removed > 0.0f &&
              session.actor(target_query.selected)->health ==
                  target_health_before_bashdown - bashdown_receipt.health_removed,
              "Authored BashDown marker did not apply its same-session source result to the retained target");
        const auto random_after_bashdown = session.world()->random_state();
        const float health_after_bashdown = session.actor(target_query.selected)->health;
        RuntimeSkillCastReceiptV1 duplicate_cast;
        check(cast_coordinator.apply_retained_use_event_v1(session, 1,
                  final_cast->generation, RuntimeSkillSourceAnimStateV1::skill,
                  actual_source_event, duplicate_cast, error), error);
        const auto random_after_duplicate = session.world()->random_state();
        check(session.actor(target_query.selected)->health == health_after_bashdown &&
              random_after_duplicate.seed == random_after_bashdown.seed &&
              random_after_duplicate.calls == random_after_bashdown.calls,
              "Duplicate BashDown marker occurrence recalculated or reapplied its source result");
        check(state.stats.resource < full_mp.sheets.resolved[41] && state.skills[0].rank == 1 &&
              session.actor(1)->action == CharacterAction::idle,
              "BashDown coordinator did not preserve rank, spend same-source mana or restore lifecycle action");

        // B083 cast consumer: nil and real object targets retain mana/animation,
        // object Use is once-only and never creates Character damage/RNG.
        const auto restore_b083_mp=[&]() {
            auto props=*session.world()->combat_properties(1);
            auto view=dh2::data::property_view(property_rules,props.sheets);
            check(dh2_property_set(&view,41,props.sheets.resolved[43])==0,"MP fixture failed");
            check(session.world()->update_combat_properties(1,props,*session.world()->traits(1),error),error);
            session.actor(1)->resource=original_signed256(props.sheets.resolved[41]);state.stats.resource=session.actor(1)->resource;
        };
        check(session.world()->bind_object(barrel,error),error);
        object_facts.visible=true;object_facts.zonable=true;object_facts.zoned=false;object_facts.interactive=true;object_facts.interaction_type=8;
        unsigned object_interactions=0;
        object_arm.interact=[&](CombatSession& current,ObjectId object,ActorId caster,std::string&){check(&current==&session&&object==99&&caster==1,"Wrong object Interact owners");++object_interactions;return true;};
        cast_request.object_targets=&object_arm;
        nearer_side->transform.position={800,0,0};farther_front->transform.position={900,0,0};
        session.world()->find_object(99)->transform.position={1000,0,0};
        restore_b083_mp();session.actor(1)->action=CharacterAction::hurt;
        auto injured_spell=cast_request;injured_spell.dispatch=RuntimeSkillCastDispatchV1::native_hud_spell;
        RuntimeSkillCastReceiptV1 refused_spell;const auto before_spell_mp=session.world()->combat_properties(1)->sheets.resolved[41];
        check(!cast_coordinator.begin_skill_cast_v1(injured_spell,session,refused_spell,error)&&error.find("event50006")!=std::string::npos&&session.world()->combat_properties(1)->sheets.resolved[41]==before_spell_mp,"Injured skill admission incorrectly admitted native Cast7 or spent spell mana");
        marker_count=0;completion_count=0;RuntimeSkillCastReceiptV1 nil_cast;
        check(cast_coordinator.begin_skill_cast_v1(cast_request,session,nil_cast,error),error);
        check(nil_cast.phase==RuntimeSkillCastPhaseV1::prepared_pending_use&&nil_cast.mana.mana_spent&&nil_cast.target_order.empty(),"Injured/nil cast refused or skipped MP");
        const auto nil_mp=session.world()->combat_properties(1)->sheets.resolved[41];
        RuntimeSkillCastReceiptV1 refused_duplicate;
        check(!cast_coordinator.begin_skill_cast_v1(cast_request,session,refused_duplicate,error)&&session.world()->combat_properties(1)->sheets.resolved[41]==nil_mp,"Duplicate nil cast debited MP");
        for(unsigned frame=0;frame<180&&completion_count==0;++frame){check(session.update(1./60.,{},{0,0,0},0.f,error),error);check(cast_coordinator.advance_after_session_update(session,1./60.,error),error);}
        check(marker_count==1&&completion_count==1&&session.actor(1)->action==CharacterAction::idle&&cast_coordinator.receipt(1)->applied_results.empty()&&object_interactions==0,"Nil cast failed marker/Post or restored orphan Injured");
        restore_b083_mp();session.world()->find_object(99)->transform.position={0,-30,0};
        marker_count=0;completion_count=0;RuntimeSkillCastReceiptV1 object_cast;
        const auto object_rng=session.world()->random_state();
        check(cast_coordinator.begin_skill_cast_v1(cast_request,session,object_cast,error),error);
        check(object_cast.phase==RuntimeSkillCastPhaseV1::prepared_pending_use&&object_cast.mana.mana_spent,"Object cast failed Pre");
        for(unsigned frame=0;frame<180&&completion_count==0;++frame){check(session.update(1./60.,{},{0,0,0},0.f,error),error);check(cast_coordinator.advance_after_session_update(session,1./60.,error),error);}
        check(object_interactions==1&&cast_coordinator.receipt(1)->applied_results.empty()&&session.world()->random_state().seed==object_rng.seed&&session.world()->random_state().calls==object_rng.calls,"Object SkillCombatRoll invented Character damage/RNG or duplicate Interact");
        RuntimeSkillCastReceiptV1 object_duplicate;
        check(cast_coordinator.apply_retained_use_event_v1(session,1,object_cast.generation,RuntimeSkillSourceAnimStateV1::skill,actual_source_event,object_duplicate,error)&&object_interactions==1,error);
        cast_request.object_targets=nullptr;check(session.world()->remove_object(99),"Object removal failed");
        nearer_side->transform.position={20,-50,0};farther_front->transform.position={20,-110,0};restore_b083_mp();

        // Normal progression/remapping occurs after the source bank and
        // CharacterVisual are initialized. Train the same live CharacterState
        // on that Session, assign its already-preloaded rank-zero root to slot1,
        // and resolve the current slot again rather than reusing bank.hotbar.
        state.stats.level = 3;
        state.source_points_known = true;
        state.source_skill_points = static_cast<std::uint32_t>(
            level_three_source.sheets.resolved[157] >> 8) - state.skills[0].rank;
        SkillProgressionV1 live_ground_slam_progression;
        check(evaluate_skill_progression_v1(state, characters, skill_tables,
                  source_caps, ground_slam_position, live_ground_slam_progression, error), error);
        check(live_ground_slam_progression.rank == 0 && live_ground_slam_progression.available &&
              live_ground_slam_progression.can_train && state.source_skill_points == 2,
              "Preinitialized same-session CharacterState cannot train the source GroundSlam row");
        check(train_skill_v1(state, characters, skill_tables, source_caps,
                  ground_slam_position, error), error);
        state.skill_slots.push_back({0, 1, ground_slam_saved_row});
        const auto advanced_save_path = scratch / "groundslam-advanced.dhsave";
        check(save_character(advanced_save_path, state, error), error);
        CharacterState loaded_advanced_state;
        check(load_character(advanced_save_path, loaded_advanced_state, error), error);
        check(loaded_advanced_state.id == state.id &&
              loaded_advanced_state.class_id == "KnightPlayerBase" &&
              loaded_advanced_state.stats.level == 3 &&
              loaded_advanced_state.source_skill_points == 1 &&
              loaded_advanced_state.skills[ground_slam_saved_row].id == "GroundSlam" &&
              loaded_advanced_state.skills[ground_slam_saved_row].rank == 1 &&
              loaded_advanced_state.skill_slots.back().slot == 1 &&
              loaded_advanced_state.skill_slots.back().saved_skill_row == ground_slam_saved_row,
              "Same-session SaveStore roundtrip lost the newly trained GroundSlam row/slot");
        std::filesystem::remove(advanced_save_path);

        auto advanced_sheets = level_three_source.sheets;
        const auto spent_source_points = static_cast<std::int32_t>(
            level_three_source.sheets.resolved[157] -
            static_cast<std::int32_t>(state.source_skill_points * 256u));
        check(spent_source_points == 2 * 256,
              "Same-session trained ranks do not account for exact source Skill_Points consumption");
        auto advanced_view = dh2::data::property_view(property_rules, advanced_sheets);
        check(dh2_property_add(&advanced_view, 157, -spent_source_points) == 0 &&
              (advanced_sheets.resolved[157] >> 8) == 1,
              "Could not publish same-session level-3 Skill_Points after training");
        auto advanced_combat_properties = *session.world()->combat_properties(1);
        advanced_combat_properties.sheets = advanced_sheets;
        check(session.world()->update_combat_properties(1, advanced_combat_properties,
                  *live_traits, error), error);
        retained_actor->health = original_signed256(advanced_sheets.resolved[36]);
        retained_actor->max_health = original_signed256(advanced_sheets.resolved[38]);
        retained_actor->resource = original_signed256(advanced_sheets.resolved[41]);
        retained_actor->max_resource = original_signed256(advanced_sheets.resolved[43]);
        state.stats.health = retained_actor->health;
        state.stats.max_health = retained_actor->max_health;
        state.stats.resource = retained_actor->resource;
        state.stats.max_resource = retained_actor->max_resource;
        cast_request.character = &state;
        cast_request.source_slot = 1;
        RuntimeSkillAnimationSlotV1 fresh_ground_slam_slot;
        check(resolve_runtime_skill_animation_slot_v1(state, characters,
                  skill_tables, 0, 1, runtime_bank, fresh_ground_slam_slot, error), error);
        check(fresh_ground_slam_slot.skill.source_skill_name == "GroundSlam" &&
              fresh_ground_slam_slot.selection_state == preloaded_ground_slam->selection_state,
              "Freshly resolved slot1 assignment did not map to its preloaded unlearned class root");
        cast_request.selection.state = fresh_ground_slam_slot.selection_state;
        marker_count = 0;
        completion_count = 0;
        actual_source_event = {};
        const auto ground_slam_target_health_before = session.actor(2)->health;
        const auto ground_slam_second_target_health_before = session.actor(3)->health;
        const auto ground_slam_rng_before = session.world()->random_state();
        RuntimeSkillCastReceiptV1 ground_slam_cast;
        check(cast_coordinator.begin_skill_cast_v1(cast_request, session,
                  ground_slam_cast, error), error);
        check(ground_slam_cast.phase == RuntimeSkillCastPhaseV1::prepared_pending_use &&
              ground_slam_cast.skill_name == "GroundSlam" &&
              ground_slam_cast.class_skill_position == ground_slam_position &&
              ground_slam_cast.source_slot == 1 && ground_slam_cast.mana.mana_spent,
              "Saved trained GroundSlam row did not reach the actual same-session source pre/cast prefix");
        for (unsigned frame = 0; frame < 180 && completion_count == 0; ++frame) {
            check(session.update(1.0 / 60.0, {}, {0, 0, 0}, 0.0f, error), error);
            check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        }
        const auto* final_ground_slam = cast_coordinator.receipt(1);
        check(marker_count == 1 && completion_count == 1 && final_ground_slam &&
              final_ground_slam->phase == RuntimeSkillCastPhaseV1::completed &&
              final_ground_slam->class_skill_position == ground_slam_position &&
              final_ground_slam->source_slot == 1 &&
              final_ground_slam->applied_results.size() == 2 &&
              final_ground_slam->applied_results[0].target == 2 &&
              final_ground_slam->applied_results[1].target == 3,
              "GroundSlam source animation did not deliver one do_skill Use and complete source Post");
        const auto ground_slam_source_mask =
            skill_tables.skills()[static_cast<std::size_t>(ground_slam_visual.skill_table_id)].scalar.words[7] |
            0x08000000u;
        for (std::size_t i = 0; i < final_ground_slam->applied_results.size(); ++i) {
            const auto& hit = final_ground_slam->applied_results[i];
            check(hit.applied && hit.attacker == 1 && hit.marker_name == "do_skill" &&
                  hit.source_id == "GroundSlam" && hit.source_mask == ground_slam_source_mask &&
                  hit.health_removed > 0.0f && hit.source_outcomes.has_value(),
                  "GroundSlam did not retain the source result/outcome for each ordered target");
        }
        check(final_ground_slam->mana.mana_spent &&
              session.actor(2)->health < ground_slam_target_health_before &&
              session.actor(3)->health < ground_slam_second_target_health_before &&
              std::fabs(ground_slam_target_health_before - session.actor(2)->health -
                  final_ground_slam->applied_results[0].health_removed) < 0.0001f &&
              std::fabs(ground_slam_second_target_health_before - session.actor(3)->health -
                  final_ground_slam->applied_results[1].health_removed) < 0.0001f &&
              session.world()->random_state().calls > ground_slam_rng_before.calls &&
              state.skills[ground_slam_saved_row].rank == 1 &&
              state.source_skill_points == 1 &&
              session.actor(1)->action == CharacterAction::idle,
              "Learned GroundSlam did not preserve its saved rank/points while spending mana and applying same-world results");
        const auto ground_slam_rng_after = session.world()->random_state();
        const auto ground_slam_hp_after_2 = session.actor(2)->health;
        const auto ground_slam_hp_after_3 = session.actor(3)->health;
        RuntimeSkillCastReceiptV1 duplicate_ground_slam;
        check(cast_coordinator.apply_retained_use_event_v1(session, 1,
                  final_ground_slam->generation, RuntimeSkillSourceAnimStateV1::skill,
                  actual_source_event, duplicate_ground_slam, error), error);
        check(session.actor(2)->health == ground_slam_hp_after_2 &&
              session.actor(3)->health == ground_slam_hp_after_3 &&
              session.world()->random_state().seed == ground_slam_rng_after.seed &&
              session.world()->random_state().calls == ground_slam_rng_after.calls,
              "Duplicate GroundSlam do_skill event reapplied source results or consumed RNG");

        // Continue normal training/remapping on the same preinitialized
        // CharacterState and Session. Level-six source points admit Charge;
        // its animation root was already loaded while the row was rank zero.
        state.stats.level = 6;
        state.source_skill_points = static_cast<std::uint32_t>(
            level_six_source.sheets.resolved[157] >> 8) - state.skills[0].rank -
            state.skills[ground_slam_saved_row].rank;
        SkillProgressionV1 live_charge_progression;
        check(evaluate_skill_progression_v1(state, characters, skill_tables,
                  source_caps, charge_position, live_charge_progression, error), error);
        check(live_charge_progression.rank == 0 && live_charge_progression.available &&
              live_charge_progression.can_train && state.source_skill_points == 4,
              "Preinitialized same-session CharacterState cannot train the source Charge row");
        check(train_skill_v1(state, characters, skill_tables,
                  source_caps, charge_position, error), error);
        state.skill_slots.push_back({0, 2, charge_saved_row});
        check(state.skills[charge_saved_row].rank == 1 && state.source_skill_points == 3,
              "Same-session Charge training did not preserve the exact source point/rank rules");
        const auto slot_matrix_save_path = scratch / "slot-matrix.dhsave";
        check(save_character(slot_matrix_save_path, state, error), error);
        CharacterState loaded_slot_matrix_state;
        check(load_character(slot_matrix_save_path, loaded_slot_matrix_state, error), error);
        check(loaded_slot_matrix_state.id == state.id &&
              loaded_slot_matrix_state.stats.level == 6 &&
              loaded_slot_matrix_state.skills[0].rank == 1 &&
              loaded_slot_matrix_state.skills[ground_slam_saved_row].rank == 1 &&
              loaded_slot_matrix_state.skills[charge_saved_row].rank == 1 &&
              loaded_slot_matrix_state.source_skill_points == 3 &&
              loaded_slot_matrix_state.skill_slots.size() == 3 &&
              loaded_slot_matrix_state.skill_slots[0].slot == 0 &&
              loaded_slot_matrix_state.skill_slots[1].slot == 1 &&
              loaded_slot_matrix_state.skill_slots[2].slot == 2 &&
              loaded_slot_matrix_state.skill_slots[2].saved_skill_row == charge_saved_row,
              "Same-session level-6 source SaveStore roundtrip lost training or slot assignments");
        std::filesystem::remove(slot_matrix_save_path);
        state.skill_slots.erase(std::remove_if(state.skill_slots.begin(), state.skill_slots.end(),
            [](const SkillSlotBinding& binding) {
                return binding.equipment_set == 0 && binding.slot == 1;
            }), state.skill_slots.end());
        check(state.skill_slots.size() == 2 && state.skill_slots[0].slot == 0 &&
              state.skill_slots[1].slot == 2,
              "Removing the trained middle assignment shifted the saved Charge slot");
        const auto slot_matrix_save_path_empty = scratch / "slot-matrix-empty.dhsave";
        check(save_character(slot_matrix_save_path_empty, state, error), error);
        CharacterState loaded_slot_matrix_state_empty;
        check(load_character(slot_matrix_save_path_empty, loaded_slot_matrix_state_empty, error), error);
        check(loaded_slot_matrix_state_empty.skill_slots.size() == 2 &&
              loaded_slot_matrix_state_empty.skill_slots[0].slot == 0 &&
              loaded_slot_matrix_state_empty.skill_slots[1].slot == 2 &&
              loaded_slot_matrix_state_empty.skill_slots[1].saved_skill_row == charge_saved_row,
              "Empty-middle SaveStore profile shifted slot2 or lost Charge row identity");
        std::filesystem::remove(slot_matrix_save_path_empty);

        RuntimeSkillCastRequestV1 slot_matrix_request = cast_request;
        slot_matrix_request.character = &state;
        slot_matrix_request.source_slot = 1;
        RuntimeSkillAnimationSlotV1 fresh_charge_slot;
        check(!resolve_runtime_skill_animation_slot_v1(state, characters,
                  skill_tables, 0, 1, runtime_bank, fresh_charge_slot, error) &&
              error.find("no valid saved skill row") != std::string::npos,
              "Fresh assignment resolver shifted empty source slot1 to a neighboring skill");
        check(resolve_runtime_skill_animation_slot_v1(state, characters,
                  skill_tables, 0, 2, runtime_bank, fresh_charge_slot, error), error);
        check(fresh_charge_slot.skill.source_skill_name == "Charge" &&
              fresh_charge_slot.skill.saved_skill_row == static_cast<int>(charge_saved_row) &&
              fresh_charge_slot.selection_state == preloaded_charge->selection_state,
              "Freshly resolved saved slot2 did not map to the preloaded Charge root");
        slot_matrix_request.selection.state = fresh_charge_slot.selection_state;
        const auto slot_one_hp_before = session.actor(2)->health;
        const auto slot_one_rng_before = session.world()->random_state();
        const auto slot_one_action_before = session.actor(1)->action;
        RuntimeSkillCastReceiptV1 empty_middle_cast;
        check(!cast_coordinator.begin_skill_cast_v1(slot_matrix_request, session,
                  empty_middle_cast, error) &&
              error.find("no valid saved skill row") != std::string::npos,
              "Same-session NativeHUDSkill slot1 did not reject the actual empty saved slot");
        check(session.actor(2)->health == slot_one_hp_before &&
              session.world()->random_state().seed == slot_one_rng_before.seed &&
              session.world()->random_state().calls == slot_one_rng_before.calls &&
              session.actor(1)->action == slot_one_action_before,
              "Rejected empty-middle-slot cast changed HP, RNG, or lifecycle action");

        // Use the source level-6 CharacterTable/ClassTables property output,
        // subtracting exactly the three saved ranks from Skill_Points. Keep
        // the resulting property owner, CharacterState and Session actor in
        // sync before the slot2 cast.
        auto level_six_sheets = level_six_source.sheets;
        const auto level_six_spent_points = static_cast<std::int32_t>(
            state.source_skill_points * 256u);
        auto level_six_view = dh2::data::property_view(property_rules, level_six_sheets);
        check(dh2_property_add(&level_six_view, 157, -level_six_spent_points) == 0 &&
              (level_six_sheets.resolved[157] >> 8) == 3,
              "Could not publish the source level-6 saved skill-point total");
        auto level_six_combat_properties = *session.world()->combat_properties(1);
        level_six_combat_properties.sheets = level_six_sheets;
        check(session.world()->update_combat_properties(1, level_six_combat_properties,
                  *live_traits, error), error);
        retained_actor->health = original_signed256(level_six_sheets.resolved[36]);
        retained_actor->max_health = original_signed256(level_six_sheets.resolved[38]);
        retained_actor->resource = original_signed256(level_six_sheets.resolved[41]);
        retained_actor->max_resource = original_signed256(level_six_sheets.resolved[43]);
        state.stats.health = retained_actor->health;
        state.stats.max_health = retained_actor->max_health;
        state.stats.resource = retained_actor->resource;
        state.stats.max_resource = retained_actor->max_resource;
        slot_matrix_request.character = &state;
        slot_matrix_request.source_slot = 2;
        slot_matrix_request.selection.state = fresh_charge_slot.selection_state;
        const auto charge_target_health_before = session.actor(2)->health;
        const auto charge_second_target_health_before = session.actor(3)->health;
        const auto charge_rng_before = session.world()->random_state();
        marker_count = 0;
        completion_count = 0;
        actual_source_event = {};
        RuntimeSkillCastReceiptV1 charge_cast;
        check(cast_coordinator.begin_skill_cast_v1(slot_matrix_request, session,
                  charge_cast, error), error);
        check(charge_cast.phase == RuntimeSkillCastPhaseV1::prepared_pending_use &&
              charge_cast.source_slot == 2 && charge_cast.skill_name == "Charge" &&
              charge_cast.class_skill_position == charge_position &&
              charge_cast.saved_skill_row == charge_saved_row && charge_cast.mana.mana_spent,
              "Saved slot2 did not select the exact learned Charge row/root in the same Session");
        for (unsigned frame = 0; frame < 180 && completion_count == 0; ++frame) {
            check(session.update(1.0 / 60.0, {}, {0, 0, 0}, 0.0f, error), error);
            check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
        }
        const auto* final_charge = cast_coordinator.receipt(1);
        check(marker_count == 1 && completion_count == 1 && final_charge &&
              final_charge->phase == RuntimeSkillCastPhaseV1::completed &&
              final_charge->source_slot == 2 && final_charge->skill_name == "Charge" &&
              !final_charge->applied_results.empty(),
              "Saved slot2 Charge did not reach its retained source Use/result/Post path");
        for (const auto& hit : final_charge->applied_results)
            check(hit.applied && hit.attacker == 1 && hit.marker_name == "do_skill" &&
                  hit.source_id == "Charge" && hit.health_removed > 0.0f,
                  "Charge retained Use result did not apply to the same-session target");
        check(session.actor(2)->health < charge_target_health_before &&
              session.actor(3)->health <= charge_second_target_health_before &&
              final_charge->mana.mana_spent &&
              session.world()->random_state().calls > charge_rng_before.calls &&
              state.skills[charge_saved_row].rank == 1 &&
              state.skill_slots.size() == 2 &&
              session.actor(1)->action == CharacterAction::idle,
              "Charge did not preserve its exact slot/rank while applying same-session mana/result lifecycle");
        const auto charge_rng_after = session.world()->random_state();
        const auto charge_hp_after_2 = session.actor(2)->health;
        const auto charge_hp_after_3 = session.actor(3)->health;
        RuntimeSkillCastReceiptV1 duplicate_charge;
        check(cast_coordinator.apply_retained_use_event_v1(session, 1,
                  final_charge->generation, RuntimeSkillSourceAnimStateV1::skill,
                  actual_source_event, duplicate_charge, error), error);
        check(session.actor(2)->health == charge_hp_after_2 &&
              session.actor(3)->health == charge_hp_after_3 &&
              session.world()->random_state().seed == charge_rng_after.seed &&
              session.world()->random_state().calls == charge_rng_after.calls,
              "Duplicate slot2 Charge marker reapplied results or consumed RNG");

        // Source skill cooldowns are transient owned timers, not SaveStore
        // fields. Wait for the completed Charge timer to become quiescent,
        // then interrupt a fresh occurrence before its do_skill Use marker.
        bool checkpoint_ready = cast_coordinator.checkpoint_v1(session, error);
        for (unsigned frame = 0; frame < 3600 && !checkpoint_ready; ++frame) {
            check(session.update(1.0 / 60.0, {}, {0, 0, 0}, 0.0f, error), error);
            check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
            checkpoint_ready = cast_coordinator.checkpoint_v1(session, error);
        }
        check(checkpoint_ready, error.empty()
            ? "Source Charge cooldown did not become checkpoint-quiescent after gameplay updates"
            : error);
        const auto mp_before_interrupted_cast = session.world()->combat_properties(1)->sheets.resolved[41];
        slot_matrix_request.source_slot = 2;
        slot_matrix_request.selection.state = fresh_charge_slot.selection_state;
        RuntimeSkillCastReceiptV1 interrupted_begin;
        check(cast_coordinator.begin_skill_cast_v1(slot_matrix_request, session,
                  interrupted_begin, error), error);
        check(interrupted_begin.phase == RuntimeSkillCastPhaseV1::prepared_pending_use &&
              interrupted_begin.skill_name == "Charge" && interrupted_begin.mana.mana_spent &&
              !interrupted_begin.applied_results.size(),
              "Interrupted Charge fixture did not stop after its real OnPre/UseMana prefix");
        check(!cast_coordinator.checkpoint_v1(session, error) &&
              error.find("active source") != std::string::npos,
              "A live source Skill6 sequence/cooldown was admitted to checkpoint");
        const auto hp_before_interruption = session.actor(2)->health;
        const auto rng_before_interruption = session.world()->random_state();
        const auto mp_after_interruption_prefix = session.world()->combat_properties(1)->sheets.resolved[41];
        check(mp_after_interruption_prefix < mp_before_interrupted_cast,
              "Interrupted cast fixture did not retain its source mana debit");
        bool did_depart = false;
        check(session.cancel_actor_source_sequence(1, session.actor_binding_lease(),
                  interrupted_begin.generation, 6, did_depart, error), error);
        check(did_depart, "Accepted same-state Skill6 restart did not retire its outgoing sequence");
        const auto* interrupted_receipt = cast_coordinator.receipt(1);
        check(interrupted_receipt && interrupted_receipt->generation == interrupted_begin.generation &&
              interrupted_receipt->phase == RuntimeSkillCastPhaseV1::interrupted &&
              interrupted_receipt->mana.mana_spent && interrupted_receipt->applied_results.empty(),
              "Accepted pre-Use departure did not preserve the reached prefix and retire its generation");
        RuntimeSkillCastReceiptV1 late_interrupted_marker;
        check(!cast_coordinator.apply_retained_use_event_v1(session, 1,
                  interrupted_begin.generation, RuntimeSkillSourceAnimStateV1::skill,
                  actual_source_event, late_interrupted_marker, error) &&
              error.find("retired interrupted generation") != std::string::npos,
              "Late do_skill marker was not rejected after the accepted state departure");
        bool duplicate_departure = true;
        check(session.cancel_actor_source_sequence(1, session.actor_binding_lease(),
                  interrupted_begin.generation, 6, duplicate_departure, error), error);
        const auto rng_after_interruption = session.world()->random_state();
        check(!duplicate_departure &&
              session.actor(2)->health == hp_before_interruption &&
              session.world()->combat_properties(1)->sheets.resolved[41] == mp_after_interruption_prefix &&
              rng_after_interruption.seed == rng_before_interruption.seed &&
              rng_after_interruption.calls == rng_before_interruption.calls,
              "Late marker or duplicate departure replayed damage/RNG or refunded the source debit");
        session.actor(1)->action = CharacterAction::idle;
        check(!cast_coordinator.checkpoint_v1(session, error) &&
              error.find("active source skill cooldown") != std::string::npos,
              "Accepted departure incorrectly erased the unpersisted source cooldown");
        check(!session.validate_lifecycle_checkpoint(error) && !error.empty(),
              "CombatSession checkpoint ignored the retained coordinator cooldown policy");
        checkpoint_ready = cast_coordinator.checkpoint_v1(session, error);
        for (unsigned frame = 0; frame < 3600 && !checkpoint_ready; ++frame) {
            check(session.update(1.0 / 60.0, {}, {0, 0, 0}, 0.0f, error), error);
            check(cast_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
            checkpoint_ready = cast_coordinator.checkpoint_v1(session, error);
        }
        check(checkpoint_ready && session.validate_lifecycle_checkpoint(error),
              error.empty() ? "Expired source timer did not permit a quiescent checkpoint" : error);
        GameSave quiescent_skill_checkpoint;
        check(capture_game_save("same-session-quiescent-skill", session.player_id(), state,
                  *session.world(), quiescent_skill_checkpoint, error), error);
        session.detach_for_restore();
        check(restore_game_save(quiescent_skill_checkpoint, "same-session-quiescent-skill",
                  *session.world(), state, error), error);
        check(session.rebind_after_restore(error), error);
        RuntimeSkillCastCoordinatorV1 restored_cast_coordinator;
        check(restored_cast_coordinator.checkpoint_v1(session, error), error);
        check(!cast_coordinator.checkpoint_v1(session, error) &&
              error.find("replaced Session") != std::string::npos,
              error.empty() ? "Quiescent restore silently reused a transient source clock from the prior Session lease" : error);

        // B097: restore the identical source sheets, target and combat RNG for
        // each rank, then run the production coordinator and actual authored
        // marker. This isolates rank from weapon, defense and random variance.
        std::array<float, 2> ranked_damage{};
        std::array<std::int32_t, 2> ranked_min{}, ranked_max{};
        cast_request.source_slot = 0;
        cast_request.selection.state = runtime_bank.hotbar[0]->selection_state;
        for (std::uint32_t rank : {1u, 2u}) {
            session.detach_for_restore();
            check(restore_game_save(b097_same_roll_checkpoint, "b097-same-roll",
                      *session.world(), state, error), error);
            state.skills[0].rank = rank;
            check(session.rebind_after_restore(error), error);
            const auto paired_rng_before = session.world()->random_state();
            check(paired_rng_before.seed == b097_same_roll_checkpoint.random.seed &&
                  paired_rng_before.calls == b097_same_roll_checkpoint.random.calls,
                  "B097 paired restore changed the initial source RNG");
            SkillTempPropertiesV1 ranked_info;
            check(evaluate_skill_temp_properties_v1(properties.classes, property_rules,
                      session.world()->combat_properties(1)->sheets.resolved,
                      "Skill_Warrior_BashDown", rank, ranked_info, error), error);
            ranked_min[rank - 1] = ranked_info.properties[174];
            ranked_max[rank - 1] = ranked_info.properties[175];
            check(ranked_info.properties[190] == 256 + static_cast<int>(12 * rank),
                  "B097 source BashDown damage factor did not include the current rank");
            unsigned ranked_markers = 0, ranked_completions = 0;
            RetainedAnimationEvent ranked_event;
            cast_request.downstream_animation_services.event =
                [&](ActorId actor, const RetainedAnimationEvent& event, std::string& failure) {
                    check(actor == 1, "B097 marker reached another actor");
                    if (event.name == "do_skill") { ++ranked_markers; ranked_event = event; }
                    failure.clear(); return true;
                };
            cast_request.downstream_animation_services.finished =
                [&](ActorId, std::string& failure) { ++ranked_completions; failure.clear(); return true; };
            RuntimeSkillCastCoordinatorV1 ranked_coordinator;
            RuntimeSkillCastReceiptV1 ranked_begin;
            check(ranked_coordinator.begin_skill_cast_v1(cast_request, session, ranked_begin, error), error);
            check(ranked_begin.phase == RuntimeSkillCastPhaseV1::prepared_pending_use &&
                  ranked_begin.mana.cost.fixed_mana_cost == static_cast<int>((6 + 3 * rank) * 256) &&
                  ranked_begin.mana.mana_before - ranked_begin.mana.mana_after ==
                      ranked_begin.mana.cost.fixed_mana_cost,
                  "B097 cast did not consume the exact current-rank source MP cost");
            for (unsigned frame = 0; frame < 180 && ranked_completions == 0; ++frame) {
                check(session.update(1.0 / 60.0, {}, {0, 0, 0}, 0.0f, error), error);
                check(ranked_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
            }
            const auto* ranked_final = ranked_coordinator.receipt(1);
            check(ranked_markers == 1 && ranked_completions == 1 && ranked_final &&
                  ranked_final->phase == RuntimeSkillCastPhaseV1::completed &&
                  ranked_final->applied_results.size() == 1,
                  "B097 paired cast did not reach the actual source marker/result exactly once");
            ranked_damage[rank - 1] = ranked_final->applied_results.front().health_removed;
            check(ranked_damage[rank - 1] > 0 &&
                  session.actor(target_query.selected)->health ==
                      target_health_before_bashdown - ranked_damage[rank - 1],
                  "B097 rank-dependent result did not reach the same target HP owner");
            const auto after_rng = session.world()->random_state();
            const auto after_mp = session.world()->combat_properties(1)->sheets.resolved[41];
            RuntimeSkillCastReceiptV1 ranked_duplicate;
            check(ranked_coordinator.apply_retained_use_event_v1(session, 1,
                      ranked_final->generation, RuntimeSkillSourceAnimStateV1::skill,
                      ranked_event, ranked_duplicate, error), error);
            check(session.world()->random_state().seed == after_rng.seed &&
                  session.world()->random_state().calls == after_rng.calls &&
                  session.world()->combat_properties(1)->sheets.resolved[41] == after_mp &&
                  session.actor(target_query.selected)->health ==
                      target_health_before_bashdown - ranked_damage[rank - 1],
                  "B097 duplicate marker spent MP or replayed damage/RNG");
            std::cout << "B097 rank" << rank << " MP=" << ranked_begin.mana.cost.fixed_mana_cost / 256
                      << " damage=" << ranked_damage[rank - 1] << " target=" << ranked_final->applied_results.front().target
                      << " outcomes=" << ranked_final->applied_results.front().source_outcomes.value_or(0)
                      << " RNGbefore=" << paired_rng_before.seed << ':' << paired_rng_before.calls
                      << " RNGafter=" << after_rng.seed << ':' << after_rng.calls
                      << " min/max=" << ranked_min[rank - 1] << '/' << ranked_max[rank - 1] << '\n';
            // The retained source sequence borrows this coordinator in its
            // checkpoint callback. Release that service while the owner lives.
            session.detach_for_restore();
        }
        // Original RNG uses seed % range, so widening a damage interval can
        // produce a smaller individual hit even from the same seed. Verify
        // growing source bounds and the reached result instead of guessing
        // that every sampled hit must grow monotonically.
        check(ranked_min[1] > ranked_min[0] && ranked_max[1] > ranked_max[0] &&
              ranked_damage[1] != ranked_damage[0],
              "B097 source damage bounds/current-rank hit did not change from rank1 to rank2");
        if (source_rng_seed == 1u) {
            // Independent source arithmetic for this unarmed/defense0 fixture:
            // base bounds512/1536 * (256+12*rank)/256 give536/1608
            // and560/1680. Random::GetRandom(0x33ff90), nine admitted
            // rolls from seed1, finishes at12133000. Its damage roll is
            // min + seed % (max-min):640 and600 fixed, respectively.
            check(ranked_min == std::array<std::int32_t, 2>{536, 560} &&
                  ranked_max == std::array<std::int32_t, 2>{1608, 1680} &&
                  ranked_damage == std::array<float, 2>{640.0f / 256.0f, 600.0f / 256.0f},
                  "B097 rank1/rank2 actual cast differs from independent source gold arithmetic");
        }

        // Rank2 is unaffordable at rank1's cost; Check must reject before
        // target/lifecycle/RNG/mana changes. Equality at rank2 cost succeeds.
        for (const bool sufficient : {false, true}) {
            session.detach_for_restore();
            check(restore_game_save(b097_same_roll_checkpoint, "b097-same-roll",
                      *session.world(), state, error), error);
            state.skills[0].rank = 2;
            auto threshold_properties = *session.world()->combat_properties(1);
            auto threshold_view = dh2::data::property_view(property_rules, threshold_properties.sheets);
            const int threshold_mp = (sufficient ? 12 : 9) * 256;
            check(dh2_property_set(&threshold_view, 41, threshold_mp) == 0,
                  "B097 could not arrange source mana threshold");
            check(session.world()->update_combat_properties(1, threshold_properties,
                      *session.world()->traits(1), error), error);
            session.actor(1)->resource = state.stats.resource = float(threshold_mp) / 256.0f;
            // An empty target search must still commit source mana.
            if (sufficient) {
                session.actor(2)->transform.position = {10000, 10000, 0};
                session.actor(3)->transform.position = {10000, 10000, 0};
            }
            check(session.rebind_after_restore(error), error);
            cast_request.downstream_animation_services = callbacks;
            const auto before_rng = session.world()->random_state();
            RuntimeSkillCastCoordinatorV1 threshold_coordinator;
            RuntimeSkillCastReceiptV1 threshold_begin;
            check(threshold_coordinator.begin_skill_cast_v1(cast_request, session, threshold_begin, error), error);
            if (!sufficient) {
                check(threshold_begin.phase == RuntimeSkillCastPhaseV1::rejected &&
                      !threshold_begin.mana.mana_spent &&
                      session.world()->combat_properties(1)->sheets.resolved[41] == threshold_mp &&
                      session.actor(1)->action == CharacterAction::idle &&
                      session.world()->random_state().seed == before_rng.seed &&
                      session.world()->random_state().calls == before_rng.calls,
                      "B097 rank2 insufficient-MP Check mutated the live cast owners");
            } else {
                check(threshold_begin.mana.mana_spent && threshold_begin.mana.mana_after == 0 &&
                      threshold_begin.target_order.empty(),
                      "B097 exact rank2 mana/empty search did not spend12 MP");
                for (unsigned frame = 0; frame < 180; ++frame) {
                    check(session.update(1.0 / 60.0, {}, {0, 0, 0}, 0.0f, error), error);
                    check(threshold_coordinator.advance_after_session_update(session, 1.0 / 60.0, error), error);
                }
                const auto* empty_final = threshold_coordinator.receipt(1);
                check(empty_final && empty_final->phase == RuntimeSkillCastPhaseV1::completed &&
                      empty_final->applied_results.empty(),
                      "B097 admitted empty-target cast created a fabricated hit");
            }
            session.detach_for_restore();
        }


        // B083 departure/Pre ordering. A synchronous source transition arm
        // borrows the same live Character408 and native LookAt kernel; the
        // full physical owner is independently exercised by its linked test.
        unsigned blur_calls=0,focus_calls=0,suffix_calls=0,pre_calls=0;
        auto order_request=slot_matrix_request;
        order_request.source_slot=0;order_request.animation_bank=&runtime_bank;
        order_request.selection=selection;
        order_request.downstream_animation_services={};
        order_request.downstream_animation_services.finished=[](ActorId,std::string& e){e.clear();return true;};
        order_request.downstream_animation_services.focused=[&](ActorId,std::string& e){++pre_calls;check(blur_calls==1&&focus_calls==1&&suffix_calls==0,"Pre did not follow one Blur/Focus prefix and precede animation suffix");e.clear();return true;};
        auto reset_order_fixture=[&](ActorId old_target,bool two_candidates) {
            // Each branch starts from a fresh source Session. A failed reached
            // prefix remains noncheckpointable; this fixture never restores it.
            session.detach_for_restore();
            check(session.initialize(assets,properties,bindings,config,player_visual,population,{0,0,0},customization,error),error);
            session.actor(1)->persistent_character_id=quiescent_skill_checkpoint.character.id;
            session.detach_for_restore();
            check(restore_game_save(quiescent_skill_checkpoint,"same-session-quiescent-skill",*session.world(),state,error),error);
            check(session.rebind_after_restore(error),error);
            blur_calls=focus_calls=suffix_calls=pre_calls=0;
            auto* owner=session.actor(1);owner->transform.position={0,0,0};owner->transform.rotation[2]=0;
            for(ActorId id:{2,3}){auto* a=session.actor(id);a->health=a->max_health;a->action=CharacterAction::idle;a->source_target_node180=0;a->source_target_position184.reset();}
            session.actor(2)->transform.position=two_candidates?std::array<float,3>{0,100,0}:std::array<float,3>{100,40,0};
            session.actor(3)->transform.position=two_candidates?std::array<float,3>{20,-110,0}:std::array<float,3>{0,1000,0};
            restore_b083_mp();
            check(session.bind_reconstructible_actor_transition_handler([&](const CombatSessionActorTransition& event,std::string& e){
                if(event.actor!=1){e.clear();return true;}
                if(event.stage==CombatSessionTransitionStage::blur){
                    ++blur_calls;
                    if(event.from_state==11&&session.actor(1)->target_id){
                        SkillActorTargetQueryV1 old;old.caster=1;old.selected=session.actor(1)->target_id;old.status=SkillActorTargetQueryStatusV1::selected_character;
                        if(!look_at_bashdown_target_v1(session,old,e))return false;
                    }
                }else if(event.stage==CombatSessionTransitionStage::focus_prefix){++focus_calls;session.actor(1)->source_flags520=0x6341u;}
                else ++suffix_calls;
                e.clear();return true;
            },[](std::string& e){e.clear();return true;},error),error);
            auto injury_props=*session.world()->combat_properties(1);injury_props.facts.original_state=11;
            check(session.world()->update_combat_properties(1,injury_props,*session.world()->traits(1),error),error);owner->action=CharacterAction::hurt;
            check(session.set_source_target(1,old_target,false,error),error);
            order_request.mana_policy=mana_facts;order_request.object_targets=nullptr;
        };
        RuntimeSkillCastReceiptV1 ordered_cast;
        reset_order_fixture(2,true);
        RuntimeSkillCastCoordinatorV1 frontal_coordinator;
        check(frontal_coordinator.begin_skill_cast_v1(order_request,session,ordered_cast,error),error);
        check(ordered_cast.target_order==std::vector<ActorId>{2,3}&&pre_calls==1&&suffix_calls==1&&ordered_cast.mana.mana_spent,
              "FrontalFirst searched before outgoing injury LookAt (old heading selects actor3)");
        const auto ordered_mp=session.world()->combat_properties(1)->sheets.resolved[41];
        RuntimeSkillCastReceiptV1 ordered_duplicate;
        check(!frontal_coordinator.begin_skill_cast_v1(order_request,session,ordered_duplicate,error)&&blur_calls==1&&pre_calls==1&&session.world()->combat_properties(1)->sheets.resolved[41]==ordered_mp,
              "Duplicate injured cast replayed Blur/Pre/debit");

        reset_order_fixture(3,false);
        RuntimeSkillCastCoordinatorV1 differing_coordinator;
        check(differing_coordinator.begin_skill_cast_v1(order_request,session,ordered_cast,error),error);
        const float different_heading=session.actor(1)->transform.rotation[2];
        check(ordered_cast.target_order==std::vector<ActorId>{2}&&different_heading>1.8f&&different_heading<2.1f&&pre_calls==1&&blur_calls==1,
              "Outgoing oldtarget408 overwrote the different skill target facing");

        reset_order_fixture(3,false);
        RuntimeSkillCastCoordinatorV1 frame_begin_coordinator;bool frame_cast_started=false;
        session.set_frame_begin_provider([&](CombatSession& current,double,std::string& e){
            if(frame_cast_started){e.clear();return true;}frame_cast_started=true;
            return frame_begin_coordinator.begin_skill_cast_v1(order_request,current,ordered_cast,e);
        });
        check(session.update(1./60.,{},{0,0,0},0.f,error),error);session.clear_frame_begin_provider();
        check(ordered_cast.target_order==std::vector<ActorId>{2}&&session.actor(1)->transform.rotation[2]>1.8f&&session.actor(1)->transform.rotation[2]<2.1f&&pre_calls==1&&blur_calls==1,
              "Stale beginning host facing erased source Pre LookAt dispatched inside the physical frame callback");

        reset_order_fixture(3,false);session.actor(2)->transform.position={1000,400,0};
        RuntimeSkillCastCoordinatorV1 nil_order_coordinator;
        check(nil_order_coordinator.begin_skill_cast_v1(order_request,session,ordered_cast,error),error);
        check(ordered_cast.target_order.empty()&&std::abs(std::abs(session.actor(1)->transform.rotation[2])-3.1415927f)<.0001f&&ordered_cast.mana.mana_spent,
              "Nil skill Pre discarded the reached outgoing injury LookAt or skipped mana");

        reset_order_fixture(3,false);
        auto low_props=*session.world()->combat_properties(1);auto low_mp_view=dh2::data::property_view(property_rules,low_props.sheets);
        check(dh2_property_set(&low_mp_view,41,source_cost.fixed_mana_cost-1)==0,"Low MP order fixture failed");
        check(session.world()->update_combat_properties(1,low_props,*session.world()->traits(1),error),error);
        session.actor(1)->resource=original_signed256(low_props.sheets.resolved[41]);state.stats.resource=session.actor(1)->resource;
        RuntimeSkillCastCoordinatorV1 low_order_coordinator;
        check(low_order_coordinator.begin_skill_cast_v1(order_request,session,ordered_cast,error),error);
        check(ordered_cast.phase==RuntimeSkillCastPhaseV1::rejected&&blur_calls==0&&focus_calls==0&&pre_calls==0&&session.original_actor_state(1)==11&&session.actor(1)->action==CharacterAction::hurt&&session.actor(1)->transform.rotation[2]==0,
              "HasMana refusal departed injury, queried target, or changed facing");

        reset_order_fixture(3,false);
        RuntimeSkillCastCoordinatorV1 bank_order_coordinator;auto invalid_order_request=order_request;invalid_order_request.animation_bank=nullptr;invalid_order_request.selection.state="missing-source-root";
        check(!bank_order_coordinator.begin_skill_cast_v1(invalid_order_request,session,ordered_cast,error)&&blur_calls==0&&session.original_actor_state(1)==11&&session.world()->combat_properties(1)->sheets.resolved[41]==session.world()->combat_properties(1)->sheets.resolved[43],
              "Invalid bank departed or debited before hierarchy admission");

        reset_order_fixture(3,false);check(session.world()->bind_object(barrel,error),error);
        SkillObjectTargetArmV1 unavailable_arm;unavailable_arm.authored_order={99};
        unavailable_arm.facts=[](const WorldObject&,ActorId,SkillObjectTargetFactsV1&,std::string& e){e="ordering fixture unavailable typed object facts";return false;};
        order_request.object_targets=&unavailable_arm;
        RuntimeSkillCastCoordinatorV1 query_failure_coordinator;
        check(!query_failure_coordinator.begin_skill_cast_v1(order_request,session,ordered_cast,error)&&ordered_cast.phase==RuntimeSkillCastPhaseV1::partial_failure&&!ordered_cast.mana.mana_spent&&blur_calls==1&&focus_calls==1&&suffix_calls==0&&pre_calls==0&&session.original_actor_state(1)==6,
              "Pre query failure rolled back reached departure or spent unreached mana");
        const auto query_failure_mp=session.world()->combat_properties(1)->sheets.resolved[41];
        check(!query_failure_coordinator.begin_skill_cast_v1(order_request,session,ordered_duplicate,error)&&blur_calls==1&&session.world()->combat_properties(1)->sheets.resolved[41]==query_failure_mp,
              "Partial Pre query failure replayed departure or debit");

        reset_order_fixture(3,false);order_request.mana_policy.tracing_character_stats.reset();
        RuntimeSkillCastCoordinatorV1 debit_failure_coordinator;
        check(!debit_failure_coordinator.begin_skill_cast_v1(order_request,session,ordered_cast,error)&&ordered_cast.phase==RuntimeSkillCastPhaseV1::partial_failure&&ordered_cast.mana.mana_spent&&blur_calls==1&&focus_calls==1&&suffix_calls==0&&session.original_actor_state(1)==6,
              "Postdebit provider failure refunded mana or rolled back departure");
        const auto partial_mp=session.world()->combat_properties(1)->sheets.resolved[41];
        check(!debit_failure_coordinator.begin_skill_cast_v1(order_request,session,ordered_duplicate,error)&&blur_calls==1&&session.world()->combat_properties(1)->sheets.resolved[41]==partial_mp,
              "Postdebit partial cast replayed departure or mana");
        std::cout<<"PASS B083 order: postBlur FrontalFirst; differing Character408/new target; nil facing; HasMana/bank refusal; pre/postdebit failures; duplicates\n";

        session.detach_for_restore();
        std::cout << "PASS B097 paired ranks1/2 actual do_skill damage, MP9/12, duplicate and mana/empty branches\n";
        std::cout << "PASS linked coordinator fixture: Warrior source slots0/1/2 BashDown/GroundSlam/Charge; "
                     "Rogue logical slot0→saved row0 JumpKick rank1/root521; logical slot1 empty "
                     "rejection without shifting; "
                     "level3/level6 source progression and SaveStore roundtrips; exact pre-init animation roots; "
                     "retained do_skill Use/Post, mana, same-world result/HP/RNG and duplicate suppression. "
                     "Hotty/other result tails remain separate.\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    }
}
