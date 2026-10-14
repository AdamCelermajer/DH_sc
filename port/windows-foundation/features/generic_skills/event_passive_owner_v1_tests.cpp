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
constexpr unsigned kWalk = 46, kMaxHp = 38, kSnsLevel = 172, kTempProp1 = 190, kTempProp2 = 191;
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
            session.actor(1)->health = session.actor(1)->max_health - 3.0f * heal;
            const float before = session.actor(1)->health;
            // A kill by someone else does nothing; a non-kill hit does nothing.
            bus.dispatch({monster, player, 0u, true});
            bus.dispatch({player, monster, 0u, false});
            check(session.actor(1)->health == before, "Only the player's own kill heals");
            bus.dispatch({player, monster, 0u, true});
            check(std::fabs(session.actor(1)->health - (before + heal)) < 1e-3f,
                  "A player kill must heal TempProp1 at the rank (" + std::to_string(heal) + ")");
            // Clamp: a heal larger than the missing HP fills to max.
            session.actor(1)->health = session.actor(1)->max_health - 0.25f * heal;
            bus.dispatch({player, monster, 0u, true});
            check(session.actor(1)->health == session.actor(1)->max_health, "Carrion Eater must clamp HP to max");
            check(live(36) == std::int32_t(session.actor(1)->max_health * 256.0f) || std::abs(live(36) - std::int32_t(session.actor(1)->max_health * 256.0f)) < 256,
                  "The HP property must follow the actor HP");
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

        // ---- F. Blindness break (prince_assassin_blindness.luac OnTargetHit_Skill): the caster's own hit removes the buff ----
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
            bus.dispatch({monster, player, 0u});
            check(buffs.active(player, "Skill_Assassin_Blindness"), "A hit on the caster must not break Blindness");
            // The caster's own hit ends it: buff removed, selection restored by the owner.
            bus.dispatch({player, monster, 0u});
            check(!buffs.active(player, "Skill_Assassin_Blindness") && !session.world()->selection_suppressed(player),
                  "The caster's own hit must remove Blindness and restore its selection");
            // No live Blindness: a further own hit is a harmless no-op.
            bus.dispatch({player, monster, 0u});
            check(passives.last_error().empty(), "Blindness break with no live buff must be a no-op: " + passives.last_error());
        }

        std::cout << "PASS event_passive_owner_v1: live stat-passive sync from SkillTable ranks (apply, idempotent, rank 0, reload), "
                     "Killing Frenzy counter/timer/reset on bus hits, Carrion Eater kill heal with clamp, Blindness break on the caster's own hit\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL event_passive_owner_v1: " << ex.what() << '\n';
        return 1;
    }
}
