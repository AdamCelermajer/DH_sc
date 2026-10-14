// v21.8 es-passive: live passive seams on a real source Session and the authored SkillTable.
//  - sync_stat_passive_ranks_v1: saved skill rows (CharacterState::skills, ids = SkillTable names) -> stat passive ranks.
//  - RuntimeEventPassiveOwnerV1: Killing Frenzy counter/timer/reset and Carrion Eater kill heal, fed through the bus.
#include "event_passive_owner_v1.hpp"
#include "passive_skill_owner_v1.hpp"
#include "combat_result_event_bus_v1.hpp"
#include "runtime_timed_buff_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
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
constexpr unsigned kWalk = 46, kMaxHp = 38, kSnsLevel = 172, kTempProp1 = 190;
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply repository root");
        const std::filesystem::path repo(argv[1]);
        AssetCatalog assets(repo / ".local-inputs/windows-source-clock-v19-preview-9/assets");
        std::string error;
        OriginalPropertyDatabase properties;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        OriginalMeleeBindings bindings;
        check(bindings.load(AssetCatalog(repo / ".local-inputs/windows-melee-bindings"), "original-melee-bindings.xml", error), error);
        const auto skill_data = assets.read("original-cache/data/pydata/skills_pyarray.bin");
        const auto skill_names = assets.read("original-cache/data/pydata/skills_pyarraynames.bin");
        const auto skill_schema = assets.read("original-cache/data/pydata/skills_pystructnames.bin");
        dh2::data::SkillTables skill_owner;
        check(skill_owner.load(bytes(skill_data), bytes(skill_names), bytes(skill_schema), error), error);
        const auto tables = skill_owner.borrow();

        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan plan;
        check(build_original_combat_visual_plan(assets, bindings, "KnightPlayerBase", customization, "event-passive-test", plan, error), error);
        CombatSessionConfig config;
        config.diagnosticRngSeed = 31;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = plan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile profile;
        profile.initialIdle = {"Idle", 0, {0}};
        profile.animationOnly = true;
        profile.propertyOptions = {256, false};
        profile.sourceAnimationClips = plan.config.clips;
        config.profiles.emplace(config.playerProfileId, profile);
        CombatSession session;
        CharacterVisual player_visual;
        ActorPopulation population;
        check(session.initialize(assets, properties, bindings, config, player_visual, population, {0, 0, 0}, customization, error), error);
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(properties.characters, rules, error), error);
        {
            auto seeded = *session.world()->combat_properties(1);
            seeded.sheets.resolved[36] = seeded.sheets.resolved[38];
            seeded.sheets.resolved[41] = seeded.sheets.resolved[43];
            check(session.world()->update_combat_properties(1, seeded, *session.world()->traits(1), error), error);
            session.actor(1)->max_health = float(seeded.sheets.resolved[38]) / 256.0f;
            session.actor(1)->health = session.actor(1)->max_health;
        }
        const auto live = [&](unsigned cell) { return session.world()->combat_properties(1)->sheets.resolved[cell]; };
        // The authored row evaluated independently against the owner's view, the same rule the buff owner uses.
        const auto class_cells = [&](const char* name, std::uint32_t level) {
            const auto found = std::find(properties.classes.names.begin(), properties.classes.names.end(), std::string(name));
            check(found != properties.classes.names.end(), std::string("authored class absent: ") + name);
            dh2::data::PropertySheet sheet = rules.defaults, owner = session.world()->combat_properties(1)->sheets.resolved;
            sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
            check(dh2::data::apply_class(properties.classes, std::int32_t(found - properties.classes.names.begin()), sheet, error, &owner), error);
            return sheet;
        };

        TimedBuffBindingsV1 bind;
        bind.classes = &properties.classes;
        bind.rules = &rules;
        RuntimeTimedBuffOwnerV1 buffs(bind);

        // ---- A. Live sync of stat passives from the saved skill rows ----
        {
            RuntimePassiveSkillOwnerV1 passives(buffs);
            CharacterState state = make_default_character();
            state.skills = {{"Toughness", 2}, {"NotASkillAtAll", 4}};
            check(tables.skill_index("Toughness") >= 0, "SkillTable must know the Toughness dictionary name");
            // The contribution is evaluated before the first apply; the base is re-read after a removal because publishing
            // re-synchronises the base sheet.
            const auto t2 = class_cells("Skill_Berserk_Toughness", 2);
            check(sync_stat_passive_ranks_v1(passives, session, 1, state, tables, error), error);
            const auto after2 = live(kMaxHp);
            check(passives.rank(1, "Toughness") == 2, "Rank memory must record the synced rank");
            // Same rows again: no change (idempotent).
            check(sync_stat_passive_ranks_v1(passives, session, 1, state, tables, error) && live(kMaxHp) == after2,
                  "A second sync with the same ranks must not stack");
            // Save load / skills page drops the rank to 0: the passive is removed.
            state.skills[0].rank = 0;
            check(sync_stat_passive_ranks_v1(passives, session, 1, state, tables, error) && buffs.active_count(1) == 0,
                  "Rank 0 in the saved rows must remove the stat passive");
            const auto base = live(kMaxHp);
            check(after2 == base + t2[kMaxHp] && t2[kMaxHp] > 0,
                  "Live sync at Toughness rank 2 must apply its class contribution to Max_HP");
            const auto t1 = class_cells("Skill_Berserk_Toughness", 1);
            // Rank 1 after a load: applied once more.
            state.skills[0].rank = 1;
            check(sync_stat_passive_ranks_v1(passives, session, 1, state, tables, error) && live(kMaxHp) == base + t1[kMaxHp],
                  "Rank 1 must apply after the rank changes back");
            state.skills[0].rank = 0;
            check(sync_stat_passive_ranks_v1(passives, session, 1, state, tables, error) && live(kMaxHp) == base,
                  "Cleanup must restore the base Max_HP");
        }

        // ---- B. Killing Frenzy: counter 1..5, reset after 1500 ms, rebuild at 5 with a 5000 ms timer ----
        {
            CombatResultEventBusV1 bus;
            RuntimeEventPassiveOwnerV1 passives(buffs, properties.classes, rules, bus, 1);
            passives.set_rank(EventPassiveKindV1::killing_frenzy, 1);
            passives.bind(session);
            const ActorId player = 1, monster = 2;
            const auto base = live(kWalk);
            const auto frenzy = [&](std::uint32_t strength) { return class_cells("Skill_Berserk_KillingFrenzy", strength)[kWalk]; };
            // A monster hitting the player does not feed the attacker-side passive.
            bus.dispatch({monster, player, 0u});
            check(passives.frenzy_counter() == 0 && !passives.frenzy_buff_active() && live(kWalk) == base,
                  "A hit the player takes must not start Killing Frenzy");
            // A miss is not OnTargetHit.
            bus.dispatch({player, monster, 1u});
            check(passives.frenzy_counter() == 0, "A miss must not start Killing Frenzy");
            for (std::uint32_t hit = 1; hit <= 5; ++hit) {
                bus.dispatch({player, monster, 0u});
                check(passives.frenzy_counter() == hit && passives.frenzy_buff_active(), "Hit " + std::to_string(hit) + " must raise the counter");
                check(live(kWalk) == base + frenzy(hit), "Counter " + std::to_string(hit) + " must carry the counter x rank strength");
            }
            // At the maximum a further hit rebuilds the 5 x rank buff and restarts the 5000 ms timer.
            bus.dispatch({player, monster, 0u});
            check(passives.frenzy_counter() == 5 && live(kWalk) == base + frenzy(5), "Counter must stay at 5 and rebuild at 5 x rank");
            std::string advance_error;
            check(passives.advance(session, 4.0, advance_error) && passives.frenzy_buff_active(), "The 5000 ms timer must not expire after 4 s");
            check(passives.advance(session, 1.5, advance_error) && !passives.frenzy_buff_active() && live(kWalk) == base,
                  "The 5000 ms timer must remove the buff after 5.5 s");
            check(passives.frenzy_counter() == 5, "The reset keeps the counter (Lua resetBuff)");
            // Next hit with no buff restarts at 1.
            bus.dispatch({player, monster, 0u});
            check(passives.frenzy_counter() == 1 && live(kWalk) == base + frenzy(1), "After a reset the next hit restarts at 1");
            check(passives.advance(session, 1.6, advance_error) && live(kWalk) == base && !passives.frenzy_buff_active(),
                  "The 1500 ms reset must remove the buff");
            // Rank 0 disables the listener's effect.
            passives.set_rank(EventPassiveKindV1::killing_frenzy, 0);
            bus.dispatch({player, monster, 0u});
            check(passives.frenzy_counter() == 1 && !passives.frenzy_buff_active(), "Rank 0 must not start Killing Frenzy");
        }

        // ---- C. Carrion Eater: heal TempProp1 on a kill by the player, clamped to max HP ----
        {
            CombatResultEventBusV1 bus;
            RuntimeEventPassiveOwnerV1 passives(buffs, properties.classes, rules, bus, 1);
            passives.set_rank(EventPassiveKindV1::carrion_eater, 2);
            passives.bind(session);
            const ActorId player = 1, monster = 2;
            const float heal = float(class_cells("Skill_Necromancer_CarrionEater", 2)[kTempProp1]) / 256.0f;
            check(heal > 0.0f, "CarrionEater TempProp1 must be positive at rank 2");
            { std::string hp_error; check(session.world()->set_current_health(1, session.actor(1)->max_health - 3.0f * heal, hp_error), hp_error); }
            const float before = session.actor(1)->health;
            // A kill by someone else does nothing; a non-kill hit does nothing.
            bus.dispatch({monster, player, 0u, true});
            bus.dispatch({player, monster, 0u, false});
            check(session.actor(1)->health == before, "Only the player's own kill heals");
            bus.dispatch({player, monster, 0u, true});
            check(std::fabs(session.actor(1)->health - (before + heal)) < 1e-3f,
                  "A player kill must heal TempProp1 at the rank (" + std::to_string(heal) + ")");
            check(live(36) == std::int32_t(std::lround(session.actor(1)->health * 256.0f)),
                  "Heal must go through the saved HP cell (the death-reward vital sync compares HP*256 with the property)");
            {
                const auto heals = passives.take_carrion_heals();
                check(heals.size() == 1 && heals[0].rank == 2 && std::fabs(heals[0].heal - heal) < 1e-3f &&
                          std::fabs(heals[0].before - before) < 1e-3f && std::fabs(heals[0].after - (before + heal)) < 1e-3f,
                      "Exactly the player's own kill is recorded as a heal event (before/heal/after)");
                check(passives.take_carrion_heals().empty(), "take_carrion_heals drains the list");
            }
            // Live damage reaches ActorState first (the property sheet lags until the next vital sync): the heal is
            // computed from the actor HP, not from the stale property cell (real-run bug: hp 104.9 -> 166.8).
            session.actor(1)->health -= 4.0f;
            const float lagged = session.actor(1)->health;
            bus.dispatch({player, monster, 0u, true});
            check(std::fabs(session.actor(1)->health - (lagged + heal)) < 1e-3f, "Heal must start from the live actor HP");
            // Clamp: a heal larger than the missing HP fills to max.
            { std::string hp_error; check(session.world()->set_current_health(1, session.actor(1)->max_health - 0.25f * heal, hp_error), hp_error); }
            bus.dispatch({player, monster, 0u, true});
            check(session.actor(1)->health == session.actor(1)->max_health, "Carrion Eater must clamp HP to max");
            check(live(36) == std::int32_t(session.actor(1)->max_health * 256.0f) || std::abs(live(36) - std::int32_t(session.actor(1)->max_health * 256.0f)) < 256,
                  "The HP property must follow the actor HP");
        }

        // ---- E. Holy Weapon (w2-paladin): listener gating. Only a hit RECEIVED by the player with the row learned runs it.
        // The positive drain/kill needs a bound attacker sheet: this fixture has only the player, so the drain write is
        // refused and reported (the drain and the kill are verified in the real hidden-window run, not here).
        {
            CombatResultEventBusV1 bus;
            RuntimeEventPassiveOwnerV1 passives(buffs, properties.classes, rules, bus, 1);
            passives.set_rank(EventPassiveKindV1::holy_weapon, 2);
            passives.bind(session);
            const ActorId player = 1, monster = 2;
            const float before = session.actor(1)->health;
            // The player's own hit on a monster does not run the Holy Weapon listener.
            bus.dispatch({player, monster, 0u, false});
            check(passives.last_error().empty(), "Holy Weapon must ignore hits the player deals: " + passives.last_error());
            check(session.actor(1)->health == before, "Holy Weapon must not change the player HP on its own hits");
            // A monster hit on the player runs it; the unbound attacker is refused with a named error.
            bus.dispatch({monster, player, 0u, false});
            check(passives.last_error().find("Holy Weapon") != std::string::npos,
                  "Holy Weapon must run for a hit received and refuse an unbound attacker by name (error was: '" + passives.last_error() + "')");
            check(session.actor(1)->health == before, "Holy Weapon must not change the player HP");
            // Rank 0 learned nothing: no listener action, no error.
            passives.set_rank(EventPassiveKindV1::holy_weapon, 0);
            bus.dispatch({monster, player, 0u, false});
            check(passives.rank(EventPassiveKindV1::holy_weapon) == 0, "rank 0 clears the Holy Weapon listener");
            check(event_passive_kind_for_script_v1("prince_paladin_holy_weapon") == EventPassiveKindV1::holy_weapon,
                  "prince_paladin_holy_weapon must map to the Holy Weapon kind");
        }

        // ---- D. Attacker leech helper (raw x256 HP/MP, clamped to max, non-positive ignored) ----
        {
            ActorState attacker;
            attacker.health = 5.0f;
            attacker.max_health = 10.0f;
            attacker.resource = 1.0f;
            attacker.max_resource = 4.0f;
            apply_attacker_leech_v1(attacker, 3 * 256, 10 * 256);
            check(attacker.health == 8.0f && attacker.resource == 4.0f, "Leech must add HP and clamp MP to max");
            apply_attacker_leech_v1(attacker, -256, 0);
            check(attacker.health == 8.0f && attacker.resource == 4.0f, "Non-positive leech must change nothing");
            apply_attacker_leech_v1(attacker, 128, 0);
            check(attacker.health == 8.5f, "A half-point leech must add exactly half a point");
            attacker.health = 0.0f;
            apply_attacker_leech_v1(attacker, 256, 256);
            check(attacker.health == 0.0f, "A dead attacker must not regain HP from leech");
        }

        // ---- E. v21.8 w2-illusionist: Petrify, Disciples (death branch), Mirror (defender-side stun) ----
        {
            constexpr unsigned kDamageMinMainHand = 79, kTempProp2 = 191;
            const auto bind_monster = [&](ActorId id, std::int32_t damage_min_raw) {
                auto props = *session.world()->combat_properties(1);
                props.sheets.resolved[0] = 2;
                props.sheets.saved[0] = 2;
                props.sheets.resolved[kDamageMinMainHand] = damage_min_raw;
                auto traits = *session.world()->traits(1);
                traits.is_player = false;
                traits.targetable = true;
                ActorState state;
                state.id = id;
                state.definition_id = "w2_illusionist_test_monster";
                state.class_id = "w2_illusionist_test";
                state.faction_id = 2;
                state.health = state.max_health = 100.0f;
                check(session.world()->bind_actor(state, props, traits, error), "w2 monster bind failed: " + error);
            };
            bind_monster(2, 5 * 256);   // Damage_Min_Main_Hand 5: at or below the monster threshold (10)
            // Petrify's own rows lower Damage_Min_Main_Hand per stack, so the rating must stay above 10 through the cap test.
            bind_monster(3, 200 * 256); // 200: above the threshold for all eight applications
            bind_monster(4, 5 * 256);

            // Petrify: CreateBuff(CLASS, 0, 5, L) permanent per monster; a weak target gets nothing; capacity 5 holds.
            // Each owner is tested on its own bus so no other listener changes the buffs under test.
            CombatResultEventBusV1 bus_petrify;
            RuntimeEventPassiveOwnerV1 passives(buffs, properties.classes, rules, bus_petrify, 1);
            passives.bind(session);
            passives.set_rank(EventPassiveKindV1::petrify, 3);
            bus_petrify.dispatch({1, 2, 0u});
            check(buffs.active_count(2) == 0, "Petrify must not act on a target at or below the damage threshold");
            bus_petrify.dispatch({1, 3, 0u});
            check(buffs.active(3, "Skill_Illusionist_Petrify"), "Petrify must debuff a target above the damage threshold");
            // The authored Petrify row writes Damage_Min_Main_Hand absolutely (-1 - 2*L, here -7): after the first stack the
            // target is below the Lua threshold (rating > 10), so no further stack is added. Capacity 5 is therefore not
            // reached through this path; the test pins the gate, not a stack count.
            for (int i = 0; i < 7; ++i) bus_petrify.dispatch({1, 3, 0u});
            check(buffs.active_count(3) == 1, "Petrify must not restack once the rating is below the threshold");
            check(session.world()->combat_properties(3)->sheets.resolved[kDamageMinMainHand] == -7 * 256,
                  "Petrify row must set Damage_Min_Main_Hand to -1 - 2*L (= -7 at rank 3)");
            check(passives.last_error().empty(), "Petrify must not raise a listener error: " + passives.last_error());

            // Disciples: a dead defender is skipped (Lua defender HP > 0). The roll is drawn only for living defenders.
            CombatResultEventBusV1 bus_disciples;
            RuntimeEventPassiveOwnerV1 disciples(buffs, properties.classes, rules, bus_disciples, 1);
            disciples.bind(session);
            disciples.set_rank(EventPassiveKindV1::disciples, 3);
            bus_disciples.dispatch({1, 4, 0u, true});
            check(!buffs.active(4, "Skill_Illusionist_Disciples"), "Disciples must not flip a defender that died from the hit");

            // Mirror: only when the player is the defender. The stun goes to the melee-range attacker with TempProp1 chance.
            CombatResultEventBusV1 bus_mirror;
            std::vector<std::pair<ActorId, double>> stuns;
            RuntimeEventPassiveOwnerV1 mirror(buffs, properties.classes, rules, bus_mirror, 1);
            mirror.bind(session);
            mirror.set_rank(EventPassiveKindV1::mirror, 3);
            mirror.set_stun_target([&](CombatSession&, ActorId target, double duration_ms, std::string&) {
                stuns.push_back({target, duration_ms});
                return true;
            });
            bus_mirror.dispatch({1, 2, 0u}); // the player attacks: not a Mirror event
            check(stuns.empty(), "Mirror must not stun when the player is the attacker");
            for (int i = 0; i < 400; ++i) bus_mirror.dispatch({2, 1, 0u});
            const auto mirror_ms = double(class_cells("Skill_Illusionist_Mirror", 3)[kTempProp2]) / 256.0;
            check(!stuns.empty(), "Mirror must stun a melee-range attacker on some of 400 proc rolls (fixed seed)");
            for (const auto& [target, ms] : stuns) {
                check(target == 2 && std::fabs(ms - mirror_ms) < 1e-6, "Mirror stun must target the attacker for TempProp2 ms");
            }
            check(mirror.last_error().empty(), "Mirror must not raise a listener error: " + mirror.last_error());
        }

        // ---- E2. Killing Streak (v21.8 berserker): a player kill adds the body (capacity 128) and the dummy (capacity 1) for
        //         SnS_Cooldown ms; the second kill stacks the body and replaces the dummy ----
        {
            CombatResultEventBusV1 bus;
            RuntimeEventPassiveOwnerV1 passives(buffs, properties.classes, rules, bus, 1);
            passives.set_rank(EventPassiveKindV1::killing_streak, 2);
            passives.bind(session);
            const ActorId player = 1, monster = 2;
            const auto row = class_cells("Skill_Berserk_KillingStreak", 2);
            const double bufftime_ms = double(row[177]) / 256.0;
            check(std::fabs(bufftime_ms - 5000.0) < 1e-6, "Killing Streak bufftime at rank 2 must be 4000 + 500 x 2 ms");
            const auto before = buffs.active_count(1);
            bus.dispatch({player, monster, 0u, false});
            check(buffs.active_count(1) == before, "A non-killing hit must not start Killing Streak");
            bus.dispatch({monster, player, 0u, true});
            check(buffs.active_count(1) == before, "Only the player's own kill starts Killing Streak");
            bus.dispatch({player, monster, 0u, true});
            check(buffs.active_count(1) == before + 2, "A player kill must add the body and the dummy buff");
            bus.dispatch({player, monster, 0u, true});
            check(buffs.active_count(1) == before + 3, "A second kill stacks the body (capacity 128) and replaces the dummy (capacity 1)");
            std::string streak_error;
            check(buffs.advance(session, 4.9, streak_error) && buffs.active_count(1) == before + 3, "Killing Streak must last its bufftime");
            check(buffs.advance(session, 0.2, streak_error) && buffs.active_count(1) == before, "Killing Streak must expire after SnS_Cooldown");
            check(passives.last_error().empty(), "Killing Streak listener error: " + passives.last_error());
        }

        // ---- F. Ravager (v21.8 berserker): Damage_Bonus_TwoHand from rank 1; the two-hand-in-one-hand buff (cell 203) only from rank 7 ----
        {
            RuntimePassiveSkillOwnerV1 passives(buffs);
            CharacterState state = make_default_character();
            state.skills = {{"Ravager", 6}};
            check(tables.skill_index("Ravager") >= 0, "SkillTable must know the Ravager dictionary name");
            const auto base91 = live(91), base203 = live(203);
            const auto r6 = class_cells("Skill_Berserk_Ravager", 6);
            const auto flag7 = class_cells("Skill_Berserk_Ravager_buff", 7);
            check(r6[91] > 0 && flag7[203] > 0, "Ravager rows must contribute at rank 6 and the flag at rank 7");
            check(sync_stat_passive_ranks_v1(passives, session, 1, state, tables, error), error);
            check(live(91) == base91 + r6[91], "Ravager rank 6 must add its two-hand damage bonus");
            check(live(203) == base203, "Ravager rank 6 must not set the two-hand-in-one-hand flag (rank > 6 only)");
            state.skills[0].rank = 7;
            check(sync_stat_passive_ranks_v1(passives, session, 1, state, tables, error), error);
            check(live(203) == base203 + flag7[203], "Ravager rank 7 must set the two-hand-in-one-hand flag");
            state.skills[0].rank = 6;
            check(sync_stat_passive_ranks_v1(passives, session, 1, state, tables, error) && live(203) == base203,
                  "Ravager dropping to rank 6 must clear the flag");
            state.skills[0].rank = 0;
            check(sync_stat_passive_ranks_v1(passives, session, 1, state, tables, error) && live(91) == base91,
                  "Ravager rank 0 must restore the two-hand damage cell");
        }

        // ---- G. Survival (v21.8 berserker): HP% < 35 on a hit the player takes arms the buff and an 8 s flag; the player's own
        //         attacks do not; HP% >= 35 removes the buff ----
        {
            CombatResultEventBusV1 bus;
            RuntimeEventPassiveOwnerV1 passives(buffs, properties.classes, rules, bus, 1);
            passives.set_rank(EventPassiveKindV1::survival, 3);
            passives.bind(session);
            const ActorId player = 1, monster = 2;
            float hp = 0.0f, maxhp = 0.0f;
            check(session.world()->current_health(player, hp, maxhp) && maxhp > 0.0f, "Survival test needs the player's HP");
            std::string hp_error;
            const auto set_hp_percent = [&](float percent) {
                check(session.world()->set_current_health(player, maxhp * percent / 100.0f, hp_error), hp_error);
            };
            const auto before = buffs.active_count(1);
            set_hp_percent(100.0f);
            bus.dispatch({monster, player, 0u, false});
            check(buffs.active_count(1) == before && !passives.survival_flag(), "Full HP on a hit taken must not arm Survival");
            set_hp_percent(30.0f);
            bus.dispatch({monster, player, 0u, false});
            check(buffs.active_count(1) == before + 1 && passives.survival_buff_active() && passives.survival_flag(),
                  "HP below 35% on a hit taken must apply the Survival buff and set the flag");
            bus.dispatch({monster, player, 0u, false});
            check(buffs.active_count(1) == before + 1, "A second low hit while the flag is set must not stack Survival");
            std::string survival_error;
            check(passives.advance(session, 8.1, survival_error) && !passives.survival_flag(), "The Survival flag must clear after 8 s");
            check(buffs.advance(session, 8.1, survival_error) && buffs.active_count(1) == before,
                  "The Survival buff must expire with its 8000 ms duration");
            bus.dispatch({player, monster, 0u, false});
            check(buffs.active_count(1) == before, "The player's own attack at low HP must not arm Survival");
            bus.dispatch({monster, player, 0u, false});
            check(buffs.active_count(1) == before + 1, "A low hit after the flag cleared must re-arm Survival");
            set_hp_percent(100.0f);
            bus.dispatch({monster, player, 0u, false});
            check(buffs.active_count(1) == before && !passives.survival_buff_active(),
                  "HP at 35% or more must remove the Survival buff");
            check(passives.last_error().empty(), "Survival listener error: " + passives.last_error());
        }

        // ---- H. v21.8 w2-assassin: Blindness break (prince_assassin_blindness.luac OnTargetHit_Skill): the caster's own hit removes the buff ----
        {
            CombatResultEventBusV1 bus;
            RuntimeEventPassiveOwnerV1 passives(buffs, properties.classes, rules, bus, 1);
            passives.set_rank(EventPassiveKindV1::blindness_break, 15);
            passives.bind(session);
            const ActorId player = 1, monster = 2;
            TimedBuffSpecV1 blind;
            blind.class_name = "Skill_Assassin_Blindness";
            blind.strength = 15;
            blind.duration_ms = 10000.0;
            blind.capacity = 1;
            blind.suppress_selection = true;
            blind.source = "blindness test";
            std::string blind_error;
            check(buffs.apply_timed_buff(session, player, blind, blind_error), blind_error);
            check(buffs.active(player, "Skill_Assassin_Blindness") && session.world()->selection_suppressed(player),
                  "Blindness must suppress the caster's selection while it runs");
            // A hit the caster receives does not end Blindness (attacker != self).
            bus.dispatch({monster, player, 0u, false});
            check(buffs.active(player, "Skill_Assassin_Blindness"), "A hit on the caster must not break Blindness");
            // The caster's own hit ends it: buff removed, selection restored by the owner.
            bus.dispatch({player, monster, 0u, false});
            check(!buffs.active(player, "Skill_Assassin_Blindness") && !session.world()->selection_suppressed(player),
                  "The caster's own hit must remove Blindness and restore its selection");
            // No live Blindness: a further own hit is a harmless no-op.
            bus.dispatch({player, monster, 0u, false});
            check(passives.last_error().empty(), "Blindness break with no live buff must be a no-op: " + passives.last_error());
        }

        // ---- I. v21.8 w3-ally: Conversion. A converted monster gets a 2000 ms untargetable window (selection suppression) and
        // the PlaySound label; one window at a time. The hit is rolled (GetRand(0,100) < TempProp1), so the test dispatches
        // player hits until one converts (bounded), then checks the window clock.
        {
            CombatResultEventBusV1 bus;
            RuntimeEventPassiveOwnerV1 passives(buffs, properties.classes, rules, bus, 1);
            passives.set_rank(EventPassiveKindV1::conversion, 40);
            passives.bind(session);
            const ActorId player = 1, monster = 2;
            bool converted = false;
            for (int attempt = 0; attempt < 400 && !converted; ++attempt) {
                bus.dispatch({player, monster, 0u, false});
                converted = passives.conversion_window_open();
            }
            check(converted, "Conversion must convert the monster within 400 player hits at rank 40");
            check(session.world()->selection_suppressed(monster), "Converted monster must be untargetable during the window");
            {
                const auto sounds = passives.take_sound_cues();
                check(sounds.size() == 1 && sounds[0] == "sfx_passive_skill_activate_1", "Conversion must raise its PlaySound label once");
            }
            check(passives.advance(session, 1.9, error), error);
            check(session.world()->selection_suppressed(monster) && passives.conversion_window_open(),
                  "Window must stay open for 2000 ms");
            check(passives.advance(session, 0.2, error), error);
            check(!session.world()->selection_suppressed(monster) && !passives.conversion_window_open(),
                  "Window must end at 2000 ms and restore the monster's selection");
        }

        std::cout << "PASS event_passive_owner_v1: live stat-passive sync from SkillTable ranks (apply, idempotent, rank 0, reload), "
                     "Killing Frenzy counter/timer/reset on bus hits, Carrion Eater kill heal with clamp, Killing Streak stacking and "
                     "expiry, Ravager rank-7 flag, Survival HP-threshold arm/flag/removal\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL event_passive_owner_v1: " << ex.what() << '\n';
        return 1;
    }
}
