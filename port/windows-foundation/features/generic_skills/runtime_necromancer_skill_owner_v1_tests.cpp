// v21.8 w2-necromancer: unit coverage for RuntimeNecromancerSkillOwnerV1 (FireSkullRain, Pestilence, Rise) and
// RuntimeNecromancerPassiveOwnerV1 (Wrath, Putrid) on a real CombatSession (MagePlayerBase caster, lizards in front).
//
// Covered:
//   - profile rows (registry lookup), DeathClaw knockback flag, Pestilence OnPre range from TempProp1;
//   - FireSkullRain: the 1000 ms loop rolls at 1000/2000/3000 ms and stops after the third roll (no fourth roll);
//   - Pestilence: an OnPre victim is rolled and infected; a dead victim explodes on the 500 ms death watch (the blast
//     rolls the other lizard within 200 of it); an untouched victim is removed after SnS_Dot_Duration + 1000 ms without
//     exploding;
//   - Rise: the projectile executor is required (explicit error); with no landing there is nothing to summon;
//   - Wrath: 3000 ms loop, buff when HP < 40 %, removed at >= 40 %, unchanged rank does not restart the loop;
//   - Putrid: defender-side OnTargetHit on the bus, proc chance from TempProp1 at rank, buff on the attacker.
// Not covered here (real runs only): projectile flight/landing of Rise, the DoT ticks (no native DoT tail in the port),
// the FX and sound output.
#include "runtime_necromancer_passive_owner_v1.hpp"
#include "runtime_necromancer_skill_owner_v1.hpp"
#include "runtime_skill_use_profile_registry_v1.hpp"
#include "../../combat_session.hpp"
#include "../../content_paths.hpp"
#include "../combat/runtime_player_combo_chain_v1.hpp"
#include "../../../script-runtime/script_constants.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }

// Records the TimedBuffSinkV1 calls of the passive owners.
class RecordingBuffSink final : public TimedBuffSinkV1 {
public:
    struct Applied { ActorId actor; std::string class_name; std::uint32_t strength; double duration_ms; };
    std::vector<Applied> applied;
    std::vector<std::pair<ActorId, std::string>> removed;
    bool remove_timed_buff(CombatSession&, ActorId actor, const std::string& class_name, std::string&) override {
        removed.push_back({actor, class_name});
        return true;
    }
    bool apply_timed_buff(CombatSession&, ActorId actor, const TimedBuffSpecV1& spec, std::string&) override {
        applied.push_back({actor, spec.class_name, spec.strength, spec.duration_ms});
        return true;
    }
    bool apply_timed_buff_to_targets(CombatSession&, ActorId, const std::vector<ActorId>&, const TimedBuffSpecV1&,
                                     const TargetBuffPolicyV1&, std::vector<TargetBuffOutcomeV1>&, std::string&) override {
        return true;
    }
};

std::size_t count_lines(const std::vector<std::string>& lines, const std::string& needle) {
    std::size_t n = 0;
    for (const auto& line : lines) if (line.find(needle) != std::string::npos) ++n;
    return n;
}
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Original shared asset root required");
        AssetCatalog assets(argv[1]);
        std::string error;
        OriginalPropertyDatabase db;
        OriginalMeleeBindings bindings;
        check(load_original_property_tables(assets, "original-cache/data/pydata", db, error) &&
              bindings.load(assets, "original-melee-bindings.xml", error), error);
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(db.characters, rules, error), error);
        const auto read = [&](const char* name) { return read_content(assets, std::string("data/pydata/") + name); };
        const auto bytes = [](const auto& buffer) { return dh2::data::Bytes{buffer.data(), buffer.size()}; };
        const auto clipNames = read("animations_dictionary_pyarraynames.bin"), clipValues = read("animations_dictionary_pyarray.bin");
        dh2::data::Dictionary clips;
        check(dh2::data::load_dictionary(bytes(clipNames), bytes(clipValues), clips, error), error);
        const auto records = read("animations_pyarray.bin"), names = read("animations_pyarraynames.bin"), fields = read("animations_pystructnames.bin");
        dh2::data::AnimationTables animations;
        check(dh2::data::load_animation_tables(bytes(records), bytes(names), bytes(fields), clips, animations, error), error);
        const auto itemRecords = read("loot_table_pyarray.bin"), itemNames = read("loot_table_pyarraynames.bin"), itemFields = read("loot_table_pystructnames.bin");
        dh2::data::ItemTable items;
        check(dh2::data::load_items(bytes(itemRecords), bytes(itemNames), bytes(itemFields), items, error), error);
        const auto constantsBytes = read_content(assets, "data/animations_pycst.bin");
        auto* constants = dh2_script_constants_create();
        check(constants, "Source constants unavailable");
        dh2_script_constants_reload reload{};
        check(dh2_script_constants_load(constants, constantsBytes.data(), static_cast<std::uint32_t>(constantsBytes.size()), &reload) == 0,
              "Source constants failed");
        equipment_menu::RuntimePlayerLocomotionConstantsV1 stance;
        check(equipment_menu::load_runtime_player_locomotion_constants_v1([&](const char* group, const char* key, std::int32_t& value, std::string& e) {
            if (dh2_script_constants_get(constants, group, key, &value)) { e = "Missing source constant"; return false; }
            return true;
        }, stance, error), error);
        dh2_script_constants_destroy(constants);
        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan base;
        check(build_original_combat_visual_plan(assets, bindings, "MagePlayerBase", customization, "necro-base", base, error), error);
        equipment_menu::RuntimePlayerLocomotionV1 equipped;
        const auto staff = dh2::data::item_id(items, "Staff01");
        check(staff >= 0, "Original Staff01 missing");
        check(equipment_menu::resolve_runtime_player_locomotion_v1(49, items, staff, -1, 0, stance, animations, clips, equipped, error), error);
        combat::RuntimePlayerComboChainPlanV1 attack;
        check(combat::plan_runtime_player_combo_chain_v1(assets, equipped, stance.stanced_list_mask, 3, true, animations, clips,
            "MagePlayerBase", base.config, "source-staff-bank", attack, error), error);

        // ---- A. profile rows and handles (no session) ----------------------------------------------------------------
        const auto* fire = find_skill_use_profile_registered_v1("FireSkullRain", "prince_necromancer_fireskullrain");
        const auto* pest = find_skill_use_profile_registered_v1("Pestilence", "prince_necromancer_pestilence");
        const auto* rise = find_skill_use_profile_registered_v1("Rise", "prince_necromancer_rise");
        const auto* claw = find_skill_use_profile_registered_v1("DeathClaw", "prince_necromancer_deathclaw");
        check(fire && pest && rise && claw, "Necromancer profile rows are missing");
        check(fire->external_owner && pest->external_owner && rise->external_owner, "Necromancer rows must run on the owner");
        check(!fire->sets_cooldown || fire->fixed_cooldown_ms == 0.0f, "FireSkullRain cooldown must be the class SnS_Cooldown");
        check(!pest->sets_cooldown && pest->pre_range_from_temp_prop1 && pest->pre_look_at, "Pestilence OnPre range/cooldown rows are wrong");
        check(claw->knockback_great_on_character_hit && !claw->external_owner, "DeathClaw must carry the great knockback flag on the generic loop");
        RuntimeNecromancerSkillOwnerV1 bare(RuntimeNecromancerSkillServicesV1{});
        check(bare.handles(*fire) && bare.handles(*pest) && bare.handles(*rise) && !bare.handles(*claw),
              "Necromancer owner handles() is wrong");
        std::cout << "profiles: FireSkullRain, Pestilence, Rise on the owner; DeathClaw generic with great knockback\n";

        // ---- B. real Session: caster + three lizards in front ----------------------------------------------------------
        CombatSessionConfig config;
        config.diagnosticRngSeed = 1234;
        config.playerId = 1;
        config.playerProfileId = "MagePlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = base.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        config.mainItemId = "Staff01";
        config.equippedItemIds = {"StartingSuitMage", "StartingBootsMage", "StartingGlovesMage", "Staff01"};
        CombatSessionProfile player;
        player.initialIdle = {"Idle", 0, {0}};
        player.propertyOptions = {256, true};
        player.reaction = CombatSessionChoice{"Injured", 0, {0}};
        player.death = CombatSessionChoice{"Died", 0, {0}};
        player.sequenceAction = attack.selection;
        player.retainedPhaseClock = true;
        player.sourceCombo = true;
        player.sourceAttackBank = attack.visual;
        player.sourceAttackPolicies = attack.sequence_policies;
        player.rangedProjectileMarkers = true;
        config.profiles.emplace(config.playerProfileId, player);
        CombatSessionProfile enemy;
        enemy.action = {"Attack", 0, {0, 1}};
        enemy.initialIdle = {"Idle", 0, {0}};
        enemy.propertyOptions = {20 * 256, true};
        enemy.damageMarkerNames = {"attack_mainhand"};
        enemy.customization = customization;
        config.profiles.emplace("Swamp_LizadMan_Type1", enemy);

        CombatSession session;
        CharacterVisual visual;
        ActorPopulation population;
        for (ActorId stable = 2; stable <= 4; ++stable) {
            PopulationActor placed;
            placed.profileId = "Swamp_LizadMan_Type1";
            placed.definition.stableId = stable;
            placed.definition.sourceId = "necro-lizard-" + std::to_string(stable);
            placed.definition.placement = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
            placed.transform = placed.definition.placement;
            population.actors().push_back(std::move(placed));
        }
        check(session.initialize(assets, db, bindings, config, visual, population, {0, 0, 0}, customization, error), error);
        const float heading = session.actor(1)->transform.rotation[2];
        const std::array<float, 3> look{std::sin(heading), -std::cos(heading), 0.0f};
        const std::array<float, 3> origin = session.actor(1)->transform.position;
        const auto in_front = [&](float distance) {
            return std::array<float, 3>{origin[0] + distance * look[0], origin[1] + distance * look[1], origin[2]};
        };
        // Lizard 2 at 150 (FireSkullRain range 280 and the Pestilence pre range), lizard 3 at 250 (within 200 of lizard 2
        // is 100 apart: the explosion reach), lizard 4 at 230 (beside lizard 2, for the removal case).
        session.actor(2)->transform.position = in_front(150.0f);
        session.actor(3)->transform.position = in_front(250.0f);
        session.actor(4)->transform.position = in_front(230.0f);

        std::vector<std::string> trace;
        RuntimeNecromancerSkillServicesV1 services;
        services.trace = [&](const std::string& line) { trace.push_back(line); };
        services.play_effect = [&](const std::string&, const std::array<float, 3>&, std::string&) { return true; };
        services.play_sound = [&](const std::string&, std::string&) { return true; };
        RuntimeNecromancerSkillOwnerV1 owner(std::move(services));

        std::vector<ActorId> order{1, 2, 3, 4};
        std::vector<SkillTargetSourceFactsV1> facts{{1, true, false, false, true, true}, {2, true, false, false, true, true},
                                                    {3, true, false, false, true, true}, {4, true, false, false, true, true}};
        dh2::data::PropertySheet sheet = session.world()->combat_properties(1)->sheets.resolved;
        const auto find = [&](const char* name) {
            const auto found = std::find(db.characters.fields.begin(), db.characters.fields.end(), name);
            check(found != db.characters.fields.end(), std::string("CharacterProperties field missing: ") + name);
            return static_cast<std::size_t>(found - db.characters.fields.begin());
        };
        sheet[find("SnS_MinDamage")] = 1 * 256;  // small rolls: the lizards must survive the FireSkullRain timeline
        sheet[find("SnS_MaxDamage")] = 1 * 256;
        sheet[find("SnS_Dot_Duration")] = 4000 * 256;  // Pestilence DoT duration (the class sheet supplies this value)
        RuntimeSkillExternalContextV1 context;
        context.caster = 1;
        context.generation = 1;
        context.row_mask = 0x20000u;
        context.category = 0;
        context.element = 0;
        context.formula_sheet = &sheet;
        context.population_order = &order;
        context.target_facts = &facts;
        context.non_character_attackable_objects_absent = true;
        context.characters = &db.characters;

        // ---- B1. FireSkullRain: 1000 ms loop, three rolls -------------------------------------------------------------
        context.profile = fire;
        context.skill_name = "FireSkullRain";
        std::string e;
        check(owner.use(session, context, e), "FireSkullRain Use failed: " + e);
        check(owner.active_fire_skull_rains() == 1, "FireSkullRain loop did not start");
        // All three lizards (150, 230, 250) are inside RANGE 280; the no-damage mask keeps them alive for the timeline.
        check(owner.advance(session, 999.0, e), e);
        check(count_lines(trace, "FireSkullRain roll") == 0, "FireSkullRain rolled before 1000 ms");
        check(owner.advance(session, 1.0, e), e);
        check(count_lines(trace, "FireSkullRain roll") == 3, "FireSkullRain tick 1 did not roll the three lizards");
        check(owner.advance(session, 1000.0, e), e);
        check(count_lines(trace, "FireSkullRain roll") == 6, "FireSkullRain tick 2 did not roll the three lizards");
        check(owner.advance(session, 1000.0, e), e);
        check(count_lines(trace, "FireSkullRain roll") == 9, "FireSkullRain tick 3 (third roll) missing");
        check(owner.active_fire_skull_rains() == 0, "FireSkullRain loop did not stop after the third roll");
        check(owner.advance(session, 4000.0, e), e);
        check(count_lines(trace, "FireSkullRain roll") == 9, "FireSkullRain rolled a fourth time");
        std::cout << "FireSkullRain: rolls at 1000/2000/3000 ms, 3 targets each, stopped after the third roll\n";

        // ---- B2. Pestilence: infect, death watch, explosion, removal --------------------------------------------------
        trace.clear();
        context.profile = pest;
        context.skill_name = "Pestilence";
        std::vector<ActorId> pre_list{2};
        context.ordered_character_targets = &pre_list;
        check(owner.use(session, context, e), "Pestilence Use failed: " + e);
        check(count_lines(trace, "Pestilence infected target=2") == 1 && owner.infected_victims() == 1,
              "Pestilence did not infect the OnPre target");
        check(owner.advance(session, 500.0, e), e);
        check(count_lines(trace, "explosion") == 0, "Pestilence exploded a living victim");
        // The victim dies (its health reaches zero in the Session): the next 500 ms death-watch poll explodes it.
        session.actor(2)->health = 0.0f;
        check(!session.actor(2)->alive(), "Victim did not die in the Session (test precondition)");
        check(owner.advance(session, 500.0, e), e);
        check(count_lines(trace, "victim dead=2 -> explosion") == 1, "Death watch did not detect the dead victim");
        // Lizards 3 (100 from the victim) and 4 (80 from the victim) are within 200 of it: both are rolled.
        check(count_lines(trace, "explosion victims=2") == 1, "Explosion did not find lizards 3 and 4 within 200 of the victim");
        check(count_lines(trace, "roll target=3") >= 1 && count_lines(trace, "roll target=4") >= 1,
              "Explosion did not roll lizards 3 and 4");
        check(owner.infected_victims() == 0, "Exploded victim was not removed");
        std::cout << "Pestilence: infected on OnPre, dead victim exploded on the death watch and rolled lizard 3\n";

        // Removal without death: lizard 4 is infected, then removed after SnS_Dot_Duration + 1000 ms, with no explosion.
        trace.clear();
        pre_list = {4};
        check(owner.use(session, context, e), "Pestilence Use (removal case) failed: " + e);
        check(owner.infected_victims() == 1, "Removal-case victim was not infected");
        check(owner.advance(session, 4999.0, e), e);
        check(owner.infected_victims() == 1, "Victim removed before SnS_Dot_Duration + 1000 ms");
        check(owner.advance(session, 2.0, e), e);
        check(owner.infected_victims() == 0 && count_lines(trace, "explosion") == 0,
              "Victim was not removed at SnS_Dot_Duration + 1000 ms without an explosion");
        std::cout << "Pestilence removal: lizard 4 leaves the list at 5000 ms with no explosion\n";

        // ---- B3. Rise: the executor is required; without a landing nothing is summoned -------------------------------
        context.profile = rise;
        context.skill_name = "Rise";
        std::string rise_error;
        check(!owner.pre(session, context, rise_error) && rise_error.find("shared projectile executor") != std::string::npos,
              "Rise without the executor did not report it explicitly: " + rise_error);
        check(!owner.rise_landing_known(1), "Rise reported a landing without a projectile");
        check(owner.use(session, context, e), "Rise Use without a landing must not fail: " + e);
        check(count_lines(trace, "nothing to summon") == 1, "Rise Use without a landing did not stay a no-op");
        std::cout << "Rise: executor required (explicit error); no landing -> no summon (summon DEFERRED)\n";

        // ---- C. passives on the same Session ------------------------------------------------------------------------
        CombatResultEventBusV1 bus;
        RecordingBuffSink buffs;
        std::vector<std::string> sounds;
        RuntimeNecromancerPassiveOwnerV1 passives(buffs, db.classes, rules, bus, 1,
            [&](const std::string& sound, std::string&) { sounds.push_back(sound); return true; });
        passives.bind(session);
        passives.set_rank(NecromancerPassiveKindV1::wrath, 3);
        check(passives.rank(NecromancerPassiveKindV1::wrath) == 3, "Wrath rank not stored");
        check(passives.advance(session, 2999.0, e), e);
        check(buffs.applied.empty(), "Wrath checked before its 3000 ms interval");
        // Full HP: no buff at the first check.
        check(passives.advance(session, 1.0, e), e);
        check(buffs.applied.empty(), "Wrath applied a buff at full HP");
        // Re-syncing the same rank must not restart the loop (the host syncs every frame): the check stays due at 6000 ms.
        check(passives.advance(session, 2000.0, e), e);
        passives.set_rank(NecromancerPassiveKindV1::wrath, 3);
        auto* me = session.actor(1);
        me->health = me->max_health * 0.3f;  // HP below 40 % before the 6000 ms check
        check(passives.advance(session, 1000.0, e), e);
        check(buffs.applied.size() == 1, "Unchanged Wrath rank restarted the loop (no check at 6000 ms)");
        // The buff is applied at 6000 ms with strength 3 and the activation sound.
        check(buffs.applied.size() == 1 && buffs.applied[0].class_name == "Skill_Necromancer_Wrath" &&
              buffs.applied[0].strength == 3 && buffs.applied[0].actor == 1, "Wrath buff not applied below 40 % HP");
        check(sounds.size() == 1 && sounds[0] == "sfx_passive_skill_activate_3", "Wrath activation sound not played");
        // HP back to 40 %: the buff is removed at the next check.
        me->health = me->max_health * 0.4f;
        check(passives.advance(session, 3000.0, e), e);
        check(buffs.removed.size() == 1 && buffs.removed[0].second == "Skill_Necromancer_Wrath", "Wrath buff not removed at 40 % HP");
        std::cout << "Wrath: 3000 ms loop; buff at 30 %, removed at 40 %; re-sync keeps the loop\n";

        // Putrid: a defender-side OnTargetHit on the player, proc chance TempProp1 (5 + 2 L percent) at rank L.
        passives.set_rank(NecromancerPassiveKindV1::putrid, 1);
        passives.seed_roll(7u);
        me->health = me->max_health;
        // Attacker in melee distance (inference: within the attacker's target radius).
        session.actor(3)->transform.position = in_front(20.0f);  // lizard 2 is dead from the Pestilence case
        const std::size_t before = buffs.applied.size();
        for (int i = 0; i < 400; ++i) {
            CombatResultEventV1 event;
            event.attacker = 3;
            event.target = 1;
            event.outcomes = 0x20u;
            bus.dispatch(event);
        }
        const auto putrid_applied = buffs.applied.size() - before;
        check(passives.putrid_procs() > 0 && putrid_applied == passives.putrid_applied() && putrid_applied > 0,
              "Putrid did not proc on defender-side hits");
        check(passives.putrid_procs() < 400, "Putrid procs on every hit");
        check(passives.putrid_dots() == putrid_applied, "Putrid buff did not queue its DoT in the DoT owner");
        for (std::size_t i = before; i < buffs.applied.size(); ++i)
            check(buffs.applied[i].class_name == "Skill_Necromancer_Putrid" && buffs.applied[i].actor == 3 &&
                  buffs.applied[i].duration_ms > 0.0, "Putrid buff is not on the attacker with the TempProp2 duration");
        // A hit the player deals (attacker == player) and a hit on another actor do not proc.
        const auto procs = passives.putrid_procs();
        CombatResultEventV1 own;
        own.attacker = 1;
        own.target = 2;
        bus.dispatch(own);
        check(passives.putrid_procs() == procs, "Putrid listened to an outgoing hit");
        std::cout << "Putrid: " << passives.putrid_procs() << " procs in 400 defender-side hits, buff on the attacker\n";

        // ---- B4. Rise summon (w4-rise): three slots, one-shot lifetime timers, oldest force-kill, dead reuse, cleanup ------
        {
            std::vector<ActorId> summoned, despawned, dead;
            ActorId next_actor = 100;
            RuntimeNecromancerSkillServicesV1 riser_services;
            riser_services.summon = [&](const std::string& character, const std::array<float, 3>&, ActorId& actor, std::string& err) {
                if (character != "RisenZombie") { err = "unexpected summon " + character; return false; }
                actor = next_actor++;
                summoned.push_back(actor);
                return true;
            };
            riser_services.despawn = [&](ActorId actor, std::string&) { despawned.push_back(actor); return true; };
            riser_services.is_dead = [&](ActorId actor) { return std::find(dead.begin(), dead.end(), actor) != dead.end(); };
            riser_services.trace = [](const std::string&) {};
            RuntimeNecromancerSkillOwnerV1 riser(riser_services);
            dh2::data::PropertySheet rise_sheet{};
            rise_sheet[176] = 9000 * 256;  // SnS_Duration = 9000 ms (x256)
            RuntimeSkillExternalContextV1 rc = context;
            rc.profile = rise;
            rc.skill_name = "Rise";
            rc.caster = 1;
            rc.formula_sheet = &rise_sheet;
            std::string re;
            riser.set_rise_landing_for_test(1, {10.0f, 20.0f, 30.0f});
            check(riser.use(session, rc, re) && riser.rise_live_summons() == 1, "Rise did not summon into slot 1: " + re);
            check(riser.use(session, rc, re) && riser.rise_live_summons() == 2, "Rise did not summon into slot 2: " + re);
            check(riser.use(session, rc, re) && riser.rise_live_summons() == 3, "Rise did not summon into slot 3: " + re);
            check(summoned == std::vector<ActorId>({100, 101, 102}), "Rise summon order differs");
            // checkslot() == -1 (three alive): ForceKillAndSpawnNew kills the oldest (slot 1, actor 100) and spawns into it.
            check(riser.use(session, rc, re), "Rise force-kill summon failed: " + re);
            check(despawned == std::vector<ActorId>({100}) && summoned.back() == 103 && riser.rise_live_summons() == 3,
                  "Rise ForceKillAndSpawnNew did not replace the oldest summon");
            // checkslot() frees a dead summon (no Kill call for a dead actor) and reuses its slot.
            dead.push_back(102);
            check(riser.use(session, rc, re) && summoned.back() == 104 && despawned.size() == 1,
                  "Rise did not reuse the slot of a dead summon: " + re);
            // Lifetime: every one-shot timer is SnS_Duration = 9000 ms; nothing expires before it, all expire at it.
            std::string te;
            check(riser.advance(session, 8999.0, te) && despawned.size() == 1, "Rise summon expired before SnS_Duration: " + te);
            check(riser.advance(session, 2.0, te) && despawned.size() == 4 && riser.rise_live_summons() == 0,
                  "Rise summons did not expire at SnS_Duration: " + te);
            // OnSkillCleanUp (KillAllSummoned): a cleanup kills every live summon and stops its timer.
            for (int i = 0; i < 2; ++i) check(riser.use(session, rc, re), "Rise summon before cleanup failed: " + re);
            riser.cleanup(session, 1);
            check(riser.rise_live_summons() == 0 && despawned.size() == 6, "Rise cleanup did not kill the live summons");
            check(riser.advance(session, 10000.0, te) && despawned.size() == 6, "Rise cleanup left a timer running");
            std::cout << "Rise: 3 slots, force-kill of the oldest, dead slot reused, 9000 ms expiry, cleanup kills live summons\n";
        }

        // v21.8 w4-rise: the Session player is invalid_actor_id; the bus delivers to it only once the identity is declared.
        {
            CombatResultEventBusV1 player_bus;
            player_bus.set_player_identity(invalid_actor_id);
            int delivered = 0;
            const auto token = player_bus.subscribe(invalid_actor_id, kCombatCallbackTargetHitV1, "probe",
                [&](const CombatResultEventV1&, ActorId) { ++delivered; });
            CombatResultEventV1 hit;
            hit.attacker = 7;
            hit.target = invalid_actor_id;
            player_bus.dispatch(hit);
            check(token != 0 && delivered == 1, "Bus did not deliver a monster hit to the declared player identity");
            CombatResultEventBusV1 undeclared_bus;
            check(undeclared_bus.subscribe(invalid_actor_id, kCombatCallbackTargetHitV1, "probe",
                [](const CombatResultEventV1&, ActorId) {}) == 0, "Bus accepted an undeclared invalid receiver");
        }

        std::cout << "necromancer_skill_owner_v1 passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "necromancer_skill_owner_v1 failed: " << ex.what() << '\n';
        return 1;
    }
}
