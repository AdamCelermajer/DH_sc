// B155: Mud Lord (swampking_core.luac) boss AI facts over the real authored scripts and pydata tables.
// argv[1] = package assets (original-cache tables, compiled level banks), argv[2] = Android data root holding the authored
// AI/skill scripts and the ProjectileTable (assets-extra/android).
#include "runtime_swampking_core_ai_v1.hpp"
#include "runtime_timer_trap_v1.hpp"
#include "../campaign_host/actor_clip_manifest.hpp"
#include "../../content_paths.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_campaign_runtime.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::bosses;
namespace plan = dh2::bosses::runtime_v1;

namespace {
int checks = 0;
void check(bool ok, const std::string& what) {
    ++checks;
    if (!ok) throw std::runtime_error(what);
}
std::string read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot read " + path);
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}
std::vector<std::uint8_t> read_bytes(const std::string& path) {
    const auto text = read_file(path);
    return {text.begin(), text.end()};
}

void classification(const std::string& package) {
    const AssetCatalog assets(package);
    std::string error;
    OriginalCampaignRuntime swamp, darkwood;
    check(swamp.load_compiled_level(assets, "data/scene/001_swamp.mlx", error), "swamp bank: " + error);
    const auto idled = campaign_host::collect_script_idled_actors(swamp);
    check(idled.count("_prim_Boss_SwampKing_01") == 1, "Swamp_Serpent_Intro puts the placed Swamp King in Idle by name");
    check(!idled.count("_prim_Monster_LizardFan01") && !idled.count("_prim_Monster_MisterSnack") && !idled.count("_prim_Monster_LizardFan03"),
          "the intro lizards (shown, animated, put in Limbus) are never idled: they stay props");
    check(!idled.count("All") && !idled.count("LocalPlayer"), "All/LocalPlayer are not actor names");
    const auto gameplay = campaign_host::collect_script_gameplay_actors(swamp);
    check(!gameplay.count("_prim_Boss_SwampKing_01"), "no script spawns the Swamp King (placed declaration)");
    check(darkwood.load_compiled_level(assets, "data/scene/003_darkwood.mlx", error), "darkwood bank: " + error);
    check(campaign_host::collect_script_idled_actors(darkwood).count("_prim_DarkQueen") == 1, "HG2_boss_intro idles the Dark Queen the same way");
}

void ai_script(const std::string& extra) {
    SwampkingCoreAiScriptV1 ai;
    std::string error;
    check(parse_swampking_core_ai_script_v1(read_file(extra + "/data/scripts/ai/swampking_core.luac"), ai, error), "swampking_core.luac: " + error);
    check(ai.attack_delay_ms == 1500 && ai.goto_dive_delay_ms == 15000 && ai.in_dive_pattern_delay_ms == 8500, "ATTACK/GOTODIVE/INDIVEPATTERN delays");
    check(ai.dive_retry_ms == 500 && ai.move_delay_ms == 500 && ai.emerge_delay_ms == 1000 && ai.retarget_delay_ms == 3000 &&
          ai.decision_delay_ms == 500 && ai.skill2_delay_ms == 500, "dive retry / move / emerge (DIVE_DELAY) / retarget / decision / spit delays");
    check(ai.phase2_pct == 75 && ai.weak_pct == 25 && ai.dives_before_reset == 2, "phase 2 at 75%, weak at 25%, two dives before reset");
    check(ai.close.phase1_first_slot == 1 && ai.close.phase1_normal_prob == 80 && ai.close.phase1_normal_slot == 1 && ai.close.phase1_other_slot == 0,
          "phase-1 melee: first DoSkill(1), then roll < 80 -> 1 else 0");
    check(ai.close.phase2_first_prob == 50 && ai.close.phase2_first_low_slot == 0 && ai.close.phase2_first_high_slot == 1 &&
          ai.close.phase2_prob_a == 40 && ai.close.phase2_prob_b == 50 && ai.close.phase2_a_slot == 0 && ai.close.phase2_b_slot == 1,
          "phase-2 melee: first roll < 50 -> 0 else 1; then < 40 -> 0, (40, 90) -> 1");
    check(ai.spit_skill == 2 && ai.emerge_skill == 3 && ai.intimidate_skill == 2, "spit DoSkill(2), emerge DoSkill(3), intimidate follow-up DoSkill(2)");
    check(ai.spawn_points.size() == 5 && ai.spawn_points.front() == "_prim_BossSpawnPoint_03" && ai.spawn_points.back() == "_prim_BossSpawnPoint_05",
          "five spawn points, phase-2/weak point first");
    check(ai.dive_anim == "swampking_dive" && ai.intimidate_anim == "swampking_intimidate" && ai.dive_sound == "sfx_serpent_submerge" &&
          ai.intimidate_sounds.size() == 3, "PlayAnim clips and sounds");
    check(ai.spotted_music == "SwampKingMusic" && ai.spotted_music_fade_ms == 500 && ai.died_music == "SwampHubAmbientMusic" &&
          ai.died_music_fade_ms == 1000 && ai.died_stop_music == "SwampKingMusic" && ai.died_stop_fade_ms == 2000, "music");
    SwampkingCoreAiScriptV1 rejected;
    check(!parse_swampking_core_ai_script_v1(read_file(extra + "/data/scripts/ai/witch.luac"), rejected, error), "witch.luac is not this shape");
    check(!parse_swampking_core_ai_script_v1(read_file(extra + "/data/scripts/ai/monster.luac"), rejected, error), "monster.luac is not this shape");
    // The decision functions with the parsed numbers equal the recovered plan.
    for (int roll = 1; roll <= 100; ++roll)
        for (const bool first : {false, true})
            for (const auto phase : {plan::Phase::phase1, plan::Phase::phase2}) {
                const auto a = plan::close_attack(phase, first, roll), b = plan::close_attack(phase, first, roll, ai.close);
                check(a.kind == b.kind && a.skill_slot == b.skill_slot && a.consumes_first_melee == b.consumes_first_melee, "parsed melee rules == plan");
            }
    check(plan::close_attack(plan::Phase::phase2, false, 40, ai.close).kind == plan::DecisionKind::no_skill_dispatched &&
          plan::close_attack(plan::Phase::phase2, false, 95, ai.close).kind == plan::DecisionKind::no_skill_dispatched,
          "phase-2 rolls 40 and 90..100 dispatch no skill (source gap kept)");
    check(plan::phase_after_update(plan::Phase::phase1, 75.0f, ai.phase2_pct) == plan::Phase::phase2 &&
          plan::phase_after_update(plan::Phase::phase1, 75.1f, ai.phase2_pct) == plan::Phase::phase1, "phase-2 boundary");
}

void skill_scripts(const std::string& extra) {
    std::string error;
    BossSkillScriptV1 a1, a2, a4, spit, poison;
    check(parse_boss_skill_script_v1(read_file(extra + "/data/scripts/skills/swamp_king_attack_1.luac"), a1, error), "attack_1: " + error);
    check(a1.shape == BossSkillShapeV1::single_target && a1.range == 300 && a1.class_token == "SwampKingSkill5PCT", "attack_1 single target 300");
    check(parse_boss_skill_script_v1(read_file(extra + "/data/scripts/skills/swamp_king_attack_2.luac"), a2, error), "attack_2: " + error);
    check(a2.shape == BossSkillShapeV1::single_target && a2.range == 200 && a2.class_token == "SwampKingSkill30PCT", "attack_2 single target 200");
    check(parse_boss_skill_script_v1(read_file(extra + "/data/scripts/skills/swamp_king_attack_4.luac"), a4, error), "attack_4: " + error);
    check(a4.shape == BossSkillShapeV1::target_list_aoe && a4.range == 400 && a4.class_token == "SwampKingSkillAOEPushback" &&
          a4.pre_sound == "sfx_serpent_submerge" && a4.skill_fx == "fx_swampking_blue_shockwave" && a4.skill_sound == "sfx_serpent_shockwave.wav",
          "attack_4 AOE pushback: list within 400, blue shockwave FX");
    check(parse_boss_skill_script_v1(read_file(extra + "/data/scripts/skills/swampking_atk_spit.luac"), spit, error), "spit: " + error);
    check(spit.shape == BossSkillShapeV1::projectile_aoe && spit.range_field == "RangeMaxDistance" && spit.projectile_field == "RangeProjectileID" &&
          spit.class_token == "SwampKingSkillPoisonAttack" && spit.aoe_class_token == "SwampKingSkillPoisonAttackAOE" && spit.aoe_range == 250 &&
          spit.timer_trap == "SwampkingSteamCloud" && spit.timer_trap_damager == "Level01Damage", "spit: projectile, steam-cloud trap, 250 AOE");
    // swamp_king_attack_3 (poison spit FX, target list) is in no SkillTree; it still parses as a target-list skill.
    check(parse_boss_skill_script_v1(read_file(extra + "/data/scripts/skills/swamp_king_attack_3.luac"), poison, error) &&
          poison.shape == BossSkillShapeV1::target_list_aoe, "attack_3 parses (unused by SwampKingSkills)");
    BossSkillScriptV1 bad;
    check(!parse_boss_skill_script_v1("local RANGE = 1.0;\nlocal function OnSkill_()\nend\n", bad, error), "a script without CLASS_ID is rejected");
    check(!parse_boss_skill_script_v1(read_file(extra + "/data/scripts/skills/witch_ranged_00.luac"), bad, error) || bad.shape != BossSkillShapeV1::single_target,
          "the witch projectile skill is not mistaken for a melee bite");
}

void definition(const std::string& package, const std::string& extra) {
    std::string error;
    const AssetCatalog package_assets(package), script_assets(extra);
    spawn::ProfileDerivationTablesV1 tables;
    check(spawn::load_profile_derivation_tables_v1(package_assets, "original-cache/data/pydata", tables, error), "derivation tables: " + error);
    OriginalPropertyDatabase properties;
    check(load_original_property_tables(package_assets, "original-cache/data/pydata", properties, error), "property tables: " + error);
    const auto skill_records = read_content(package_assets, "data/pydata/skills_pyarray.bin");
    const auto skill_names = read_content(package_assets, "data/pydata/skills_pyarraynames.bin");
    const auto skill_schema = read_content(package_assets, "data/pydata/skills_pystructnames.bin");
    dh2::data::SkillTables skills;
    check(skills.load({skill_records.data(), skill_records.size()}, {skill_names.data(), skill_names.size()},
                      {skill_schema.data(), skill_schema.size()}, error), "skill tables: " + error);
    const auto records = read_bytes(extra + "/data/pydata/projectiles_pyarray.bin");
    const auto names = read_bytes(extra + "/data/pydata/projectiles_pyarraynames.bin");
    const auto schema = read_bytes(extra + "/data/pydata/projectiles_pystructnames.bin");
    const auto dictionary = read_bytes(extra + "/data/pydata/projectiles_dictionary_pyarray.bin");
    std::vector<enemy_ai::SourceProjectileRowV1> rows;
    check(enemy_ai::load_source_projectile_table_v1({records.data(), records.size()}, {names.data(), names.size()},
                                                    {schema.data(), schema.size()}, {dictionary.data(), dictionary.size()}, rows, error),
          "ProjectileTable: " + error);
    SwampkingCoreSourcesV1 sources{&script_assets, &tables, skills.borrow(), &rows, &properties.classes};
    SwampkingCoreDefinitionV1 king;
    bool supported = false;
    std::string reason;
    check(derive_swampking_core_v1(sources, "SwampKing", supported, king, reason) && supported, "SwampKing is a swampking_core boss: " + reason);
    check(king.ai_script == "swampking_core" && king.range_min == 600 && king.range_max == 6000, "AI row script, CanRangeAttack 600..6000");
    check(king.skills.size() == 4, "SwampKingSkills has four slots");
    check(king.skills[0].skill_name == "SwampKingAttack1" && king.skills[0].skill_script == "swamp_king_attack_1" && king.skills[0].animation_sequence_id == 665,
          "DoSkill(0) = SwampKingAttack1 (swamp_king_attack_1, Anim 665)");
    check(king.skills[1].skill_name == "SwampKingAttack2" && king.skills[1].animation_sequence_id == 666, "DoSkill(1) = SwampKingAttack2 (Anim 666)");
    check(king.skills[2].skill_name == "SwampKingAttack3" && king.skills[2].skill_script == "swampking_atk_spit" && king.skills[2].animation_sequence_id == 667 &&
          king.skills[2].projectile && king.skills[2].projectile->name == "SwampkingSpit" && king.skills[2].range == 6000,
          "DoSkill(2) = SwampKingAttack3 with swampking_atk_spit (Anim 667), projectile row SwampkingSpit, RANGE = RangeMaxDistance");
    check(king.skills[3].skill_name == "SwampKingAttack4" && king.skills[3].animation_sequence_id == 668 &&
          king.skills[3].script.shape == BossSkillShapeV1::target_list_aoe, "DoSkill(3) = SwampKingAttack4 (emerge clip, AOE pushback)");
    check(king.skills[2].projectile->velocity == 10 && king.skills[2].projectile->timer_ms == 4000 && king.skills[2].projectile->impact_fx == 70 &&
          king.skills[2].projectile->model_uri == "data/3D/projectiles/elemental_bolt_water.bdae", "SwampkingSpit motion/impact/model");
    check(properties.classes.names[std::size_t(king.skills[0].class_id)] == "SwampKingSkill5PCT" &&
          properties.classes.names[std::size_t(king.skills[2].aoe_class_id)] == "SwampKingSkillPoisonAttackAOE", "class ids");
    check(king.dive_anim_id == 1320 && king.intimidate_anim_id == 1329 && king.dive_anim_path.find("swampking_dive.bdae") != std::string::npos,
          "PlayAnim AnimDict ids");
    // AOE pushback evidence (documented gap: the Session does not yet turn outcome bit 0x80 into SM_SetKnockBackState, F_ApplyResult
    // 0x3b10b4): SkillTable flags of SwampKingAttack4 add the "great knockback" bit 0x100000 over the bites, and its class row carries
    // the larger SnS_Push_Target_Chance (158 vs 108).
    check((king.skills[3].skill_flags & 0x100000u) && !(king.skills[0].skill_flags & 0x100000u) && !(king.skills[1].skill_flags & 0x100000u),
          "only the emerge AOE (SwampKingAttack4) carries the great-knockback skill flag");
    {
        OriginalActorProperties actor;
        OriginalActorPropertyOptions options;
        options.level_raw = 512;
        check(resolve_original_actor_properties(properties.characters, properties.classes, "SwampKing", options, actor, error), error);
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(properties.characters, rules, error), error);
        const auto push = [&](int slot) {
            dh2::data::PropertySheet sheet{};
            check(enemy_ai::ranged_skill_formula_sheet_v1(properties.characters, properties.classes, rules, actor.sheets.resolved,
                                                          king.skills[std::size_t(slot)].class_id, 0, sheet, error), error);
            return sheet[183] / 256; // SnS_Push_Target_Chance
        };
        check(push(0) == 108 && push(1) == 108 && push(3) == 158, "class SnS_Push_Target_Chance: bites 108, emerge AOE 158");
        check(actor.max_health == 900 && actor.level_raw == 512, "SwampKing at level 2 has 900 HP (dynamic level scales the class stats)");
    }
    SwampkingCoreDefinitionV1 other;
    check(derive_swampking_core_v1(sources, "Witch", supported, other, reason) && !supported, "the witch (witch.luac) is not this boss shape");
    check(derive_swampking_core_v1(sources, "Swamp_LizadMan_Type1", supported, other, reason) && !supported, "a melee lizard is not this boss shape");
    // Without the authored scripts (the v21.6/v21.7 package as shipped) the owner is unavailable, not a startup failure.
    check(derive_swampking_core_v1(SwampkingCoreSourcesV1{&package_assets, &tables, skills.borrow(), &rows, &properties.classes}, "SwampKing",
                                   supported, other, reason) && (!supported || reason.empty()), "missing scripts degrade to unavailable");

    OriginalMeleeActor actor;
    check(spawn::derive_melee_actor_v1(tables, "SwampKing", actor, error), "derived melee actor: " + error);
    check(append_swampking_core_states_v1(tables, king, actor, error), "append boss states: " + error);
    for (int slot = 0; slot < 4; ++slot) {
        const auto state = actor.states.find(swampking_core_skill_state_v1(slot));
        check(state != actor.states.end() && state->second.size() == 1 && state->second.front().id == 665 + slot,
              "SkillTree state carries its Anim root " + std::to_string(665 + slot));
    }
    check(actor.states.at("SkillTree2").front().steps.size() == 3 && actor.states.at("SkillTree3").front().steps.size() == 1,
          "spit root: pre/attack/post steps; emerge root: one step");
    check(!append_swampking_core_states_v1(tables, king, actor, error), "a second append is refused (duplicate state names)");
    CombatSessionProfile policy;
    check(enemy_ai::derive_ranged_skill_monster_policy_v1(tables, "SwampKing", policy, error) && policy.receiveDamage && policy.animationOnly,
          "incoming-only targetable receiver with AnimTable Idle/Injured/Died: " + error);
}
} // namespace


// TimerTrap (SummonTimerTrap 0x390c50 / TimerTrap::Create 0x39d724 / TimerTrap::SpecificUpdate 0x39d688) over the real tables.
void timer_traps(const std::string& extra) {
    const AssetCatalog assets(extra);
    const auto rec = read_content(assets, "data/game_objects_pyarray.bin"), nam = read_content(assets, "data/game_objects_pyarraynames.bin");
    std::vector<TimerTrapRowV1> traps;
    std::vector<GameObjectDamagerRowV1> damagers;
    std::string error;
    check(load_timer_trap_tables_v1({rec.data(), rec.size()}, {nam.data(), nam.size()}, traps, damagers, error), "game_objects tables: " + error);
    const auto trap = std::find_if(traps.begin(), traps.end(), [](const auto& r) { return r.name == "SwampkingSteamCloud"; });
    check(trap != traps.end() && trap->delay_ms == 1000 && trap->script == "swampking_trap" && trap->visual >= 0, "SwampkingSteamCloud row: Delay 1000, swampking_trap");
    const auto damager = std::find_if(damagers.begin(), damagers.end(), [](const auto& r) { return r.name == "Level01Damage"; });
    // F_GOAttack 0x3b061c reads: +4 immunity-timer slot (-1: none), +8 immunity ms, +12 max, +16 min; damage = rand(min..max) << 8.
    check(damager != damagers.end() && damager->min == 5 && damager->max == 10 && damager->damage == -1 && damager->immunity_delay == 0, "Level01Damage 5..10, no immunity slot");
    const auto script_text = read_file(extra + "/data/scripts/objects/swampking_trap.luac");
    TimerTrapScriptV1 script;
    check(parse_timer_trap_script_v1(script_text, script, error), "swampking_trap.luac: " + error);
    check(script.count_event == "inccounter" && script.counter_limit == 5 && script.damage_damager == "Level01Damage" && script.damage_element == "Water",
          "trap script: inccounter / COUNTER_DELAY 5 / Level01Damage / Water");
    TimerTrapModelV1 model;
    check(load_timer_trap_model_v1(assets, "data/3D/GameObjects/go_trap_swampking_steam_cloud.bdae", model, error), "trap model: " + error);
    check(model.activate_end > model.activate_start && model.remove_end > model.remove_start, "trap model clips");
    int counts = 0;
    for (const auto& event : model.events) counts += event.first == "inccounter" && event.second >= model.activate_start && event.second <= model.activate_end;
    check(counts == 1, "one inccounter marker inside the activate clip");

    TimerTrapDefinitionV1 def{*trap, script, model};
    RuntimeTimerTrapsV1 owner;
    owner.spawn(def, {10, 20, 30});
    check(owner.size() == 1, "spawned");
    int steps = 0;
    while (owner.size() && steps < 60000) { owner.update(10); ++steps; }
    // Activate every Delay (1 s): the counter reaches COUNTER_DELAY after 6 activations (inccounter 1..5 increment, the 6th removes).
    const auto log = owner.take_log();
    int activations = 0;
    for (const auto& line : log) if (line.find("Remove() after") != std::string::npos) activations = std::stoi(line.substr(line.find("after ") + 6));
    check(owner.size() == 0 && activations == 6, "the cloud activates 6 times then removes itself (video: ~7.6 s of mist)");
    check(steps * 10 >= 6000 && steps * 10 < 12000, "trap lifetime ~7-9 s");
    // B155 review: a definition without its count event inside the activate span would never expire, so it is refused.
    check(validate_timer_trap_definition_v1(def, error), "the authored steam cloud is a valid definition: " + error);
    auto bare = def;
    bare.model.events.clear();
    check(!validate_timer_trap_definition_v1(bare, error) && error.find("inside its activate span") != std::string::npos,
          "a model without the count event is refused");
    auto at_start = def;
    at_start.model.events.clear();
    for (const auto& event : def.model.events) at_start.model.events.push_back({event.first, def.model.activate_start});
    check(!validate_timer_trap_definition_v1(at_start, error), "an event at activate_start never counts (offset 0) and is refused");
    auto unnamed = def;
    unnamed.script.count_event.clear();
    check(!validate_timer_trap_definition_v1(unnamed, error), "a script with no count event is refused");
}
int main(int argc, char** argv) {
    try {
        check(argc == 3, "usage: runtime_swampking_core_ai_v1_tests <package assets> <android data root>");
        classification(argv[1]);
        ai_script(argv[2]);
        skill_scripts(argv[2]);
        definition(argv[1], argv[2]);
        timer_traps(argv[2]);
        std::cout << "runtime_swampking_core_ai_v1 tests passed (" << checks << " checks)\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "runtime_swampking_core_ai_v1 test failed: " << failure.what() << '\n';
        return 1;
    }
}
