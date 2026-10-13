// v21.8 es-passive: tests for the OnTargetHit bus, the generic passive owner and buff capacity admission.
// Real source Session (same fixture as runtime_timed_buff_v1_tests). Class numbers are read from the authored ClassTable.
#include "combat_result_event_bus_v1.hpp"
#include "passive_skill_owner_v1.hpp"
#include "runtime_timed_buff_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}
constexpr unsigned kWalk = 46, kSnsLevel = 172;
constexpr unsigned kMaxHp = 38;
}

int main(int argc, char** argv) {
    try {
        // ---- Pure bus rules (no session) ----
        {
            CombatResultEventBusV1 bus;
            ActorId p = 1, m = 2;
            int hit_on_attacker = 0, hit_on_target = 0, miss_calls = 0;
            check(bus.subscribe(p, kCombatCallbackTargetHitV1, "frenzy", [&](const CombatResultEventV1& e, ActorId r) {
                      if (r == e.attacker) ++hit_on_attacker; else ++hit_on_target; }) != 0, "Hit listener must register");
            check(bus.subscribe(p, kCombatCallbackTargetMissedV1, "miss", [&](const CombatResultEventV1&, ActorId) { ++miss_calls; }) != 0,
                  "Miss listener must register");
            check(bus.subscribe(p, 0x4u, "bad", [](const CombatResultEventV1&, ActorId) {}) == 0, "Unknown callback flag must be refused");
            check(bus.flags(p) == (kCombatCallbackTargetHitV1 | kCombatCallbackTargetMissedV1), "Flag word must carry both callbacks");
            check(bus.flags(m) == 0, "A receiver without listeners has no flags");

            // Attacker p hits m: only p's hit listener runs (attacker receiver), not m's (none).
            check(bus.dispatch({p, m, 0u}) == 1 && hit_on_attacker == 1 && hit_on_target == 0, "Player attacker must run its hit listener once");
            // m hits p: p is the target receiver, so the hit listener runs with receiver == p (role decided by the listener).
            check(bus.dispatch({m, p, 0u}) == 1 && hit_on_target == 1, "Player as target must receive the hit on the target side");
            // A miss (result & 3 != 0) selects OnTargetMissed only.
            check(bus.dispatch({m, p, 1u}) == 1 && miss_calls == 1 && hit_on_target == 1, "A miss must run only the miss callback");
            // Two-sided: attacker and target both player-side listeners run attacker first.
            CombatResultEventBusV1 both;
            std::string order;
            both.subscribe(p, kCombatCallbackTargetHitV1, "a", [&](const CombatResultEventV1&, ActorId r) { order += r == p ? "P" : "?"; });
            both.subscribe(m, kCombatCallbackTargetHitV1, "b", [&](const CombatResultEventV1&, ActorId r) { order += r == m ? "M" : "?"; });
            both.dispatch({m, p, 0u});
            check(order == "MP", "Both receivers must run, attacker receiver (m) first then target receiver (p)");
            // Unsubscribe by key removes only that skill's listener on that receiver.
            check(bus.unsubscribe_key(p, "frenzy") == 1 && bus.flags(p) == kCombatCallbackTargetMissedV1, "unsubscribe_key must remove that skill only");
            check(bus.unsubscribe(9999) == false, "Unknown token must not unsubscribe");
        }

        check(argc == 2, "Supply repository root");
        const std::filesystem::path repo(argv[1]);
        AssetCatalog assets(repo / ".local-inputs/windows-source-clock-v19-preview-9/assets");
        std::string error;
        OriginalPropertyDatabase properties;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        OriginalMeleeBindings bindings;
        check(bindings.load(AssetCatalog(repo / ".local-inputs/windows-melee-bindings"), "original-melee-bindings.xml", error), error);
        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan plan;
        check(build_original_combat_visual_plan(assets, bindings, "KnightPlayerBase", customization, "passive-owner-test", plan, error), error);
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
        check(session.actor(1) && session.world()->combat_properties(1), "Passive test Session lacks the player");
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(properties.characters, rules, error), error);
        {
            auto seeded = *session.world()->combat_properties(1);
            seeded.sheets.resolved[36] = seeded.sheets.resolved[38];
            seeded.sheets.resolved[41] = seeded.sheets.resolved[43];
            check(session.world()->update_combat_properties(1, seeded, *session.world()->traits(1), error), error);
            session.actor(1)->max_health = float(seeded.sheets.resolved[38]) / 256.0f;
            session.actor(1)->health = session.actor(1)->max_health;
            check(session.actor(1)->alive(), "Passive test player must be alive");
        }

        // The authored row evaluated independently against the owner's view (same rule the buff owner uses).
        const auto buff_cells = [&](const char* name, std::uint32_t level) {
            const auto found = std::find(properties.classes.names.begin(), properties.classes.names.end(), std::string(name));
            check(found != properties.classes.names.end(), std::string("authored class absent: ") + name);
            dh2::data::PropertySheet sheet = rules.defaults, owner = session.world()->combat_properties(1)->sheets.resolved;
            sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
            check(dh2::data::apply_class(properties.classes, std::int32_t(found - properties.classes.names.begin()), sheet, error, &owner), error);
            return sheet;
        };
        const auto live = [&](unsigned cell) { return session.world()->combat_properties(1)->sheets.resolved[cell]; };

        TimedBuffBindingsV1 bind;
        bind.classes = &properties.classes;
        bind.rules = &rules;
        RuntimeTimedBuffOwnerV1 buffs(bind);
        RuntimePassiveSkillOwnerV1 passives(buffs);

        // ---- 1. Stat passive (Toughness: Max_HP through formula 6 on the buff sheet) via the generic passive owner ----
        // The contribution is evaluated before the apply (the owner's own view). Publishing re-synchronises the base, so the
        // base is re-read after removal rather than taken from the pre-apply value.
        {
            const PassiveStatRowV1 toughness{"Toughness", "Skill_Berserk_Toughness", "toughness"};
            const auto r1 = buff_cells("Skill_Berserk_Toughness", 1);
            check(passives.set_rank(session, 1, toughness, 1, error), error);
            check(passives.rank(1, "Toughness") == 1, "Rank memory must hold the applied rank");
            const auto after1 = live(kMaxHp);
            check(r1[kMaxHp] > 0, "Toughness L1 contribution to Max_HP must be positive");
            check(passives.set_rank(session, 1, toughness, 0, error) && buffs.active_count(1) == 0, "Rank 0 must remove the passive");
            const auto base = live(kMaxHp);
            check(after1 == base + r1[kMaxHp], "Toughness L1 must accumulate its class contribution onto the base Max_HP (type 4)");
            const auto r2 = buff_cells("Skill_Berserk_Toughness", 2);
            check(passives.set_rank(session, 1, toughness, 2, error), error);
            check(live(kMaxHp) == base + r2[kMaxHp] && r2[kMaxHp] > r1[kMaxHp],
                  "Rank change must replace the old instance: Max_HP = base + the rank-2 contribution (no stacking)");
            check(passives.set_rank(session, 1, toughness, 2, error) && live(kMaxHp) == base + r2[kMaxHp],
                  "Same rank must be a no-op");
            check(passives.set_rank(session, 1, toughness, 0, error) && live(kMaxHp) == base && buffs.active_count(1) == 0,
                  "Rank 0 must remove the passive and restore the base Max_HP");
        }
        // ---- 2. Capacity admission (IDA PROPS_AddBuff) on Skill_Berserk_KillingFrenzy speed cell 46 ----
        {
            const auto base = live(kWalk);
            const auto c1 = buff_cells("Skill_Berserk_KillingFrenzy", 1);
            const auto c2 = buff_cells("Skill_Berserk_KillingFrenzy", 2);
            const auto c3 = buff_cells("Skill_Berserk_KillingFrenzy", 3);
            const auto c5 = buff_cells("Skill_Berserk_KillingFrenzy", 5);
            auto spec = [&](std::uint32_t strength, std::uint32_t capacity) {
                TimedBuffSpecV1 s;
                s.class_name = "Skill_Berserk_KillingFrenzy";
                s.strength = strength;
                s.duration_ms = 5000.0;
                s.capacity = capacity;
                s.source = "frenzy";
                return s;
            };
            // Unlimited (capacity 0 = the Lua `true`, mapped to 128): instances accumulate.
            check(buffs.apply_timed_buff(session, 1, spec(1, 0), error), error);
            check(buffs.apply_timed_buff(session, 1, spec(2, 0), error), error);
            check(live(kWalk) == base + c1[kWalk] + c2[kWalk] && buffs.active_count(1) == 2,
                  "Unlimited capacity must accumulate every instance (type-4 cell)");
            check(buffs.apply_timed_buff(session, 1, spec(5, 0), error) && buffs.active_count(1) == 3, "Third unlimited instance");
            check(buffs.remove_timed_buff(session, 1, "Skill_Berserk_KillingFrenzy", error) && live(kWalk) == base && buffs.active_count(1) == 0,
                  "RemoveBuff must drop every instance of the class");

            // Capacity 2: a stronger instance evicts the weakest (strength 1), keeping 2 and 3.
            check(buffs.apply_timed_buff(session, 1, spec(1, 2), error), error);
            check(buffs.apply_timed_buff(session, 1, spec(2, 2), error), error);
            check(buffs.apply_timed_buff(session, 1, spec(3, 2), error), error);
            check(buffs.active_count(1) == 2 && live(kWalk) == base + c2[kWalk] + c3[kWalk],
                  "Capacity 2 must evict the weakest instance when a stronger one arrives");
            check(buffs.remove_timed_buff(session, 1, "Skill_Berserk_KillingFrenzy", error), error);

            // Capacity 1: a stronger existing instance blocks a weaker newcomer (rejected, nothing changes).
            check(buffs.apply_timed_buff(session, 1, spec(5, 1), error), error);
            const auto before_rejects = buffs.take_events().size();
            (void)before_rejects;
            check(buffs.apply_timed_buff(session, 1, spec(2, 1), error), error);
            check(buffs.active_count(1) == 1 && live(kWalk) == base + c5[kWalk], "A weaker newcomer must be rejected at capacity 1");
            const auto events = buffs.take_events();
            check(std::any_of(events.begin(), events.end(), [](const auto& e) { return e.kind == TimedBuffEventV1::Kind::rejected; }),
                  "The rejection must be published as an event");
            // Capacity 1: equal strength replaces (the Lua re-apply at the same rank).
            check(buffs.apply_timed_buff(session, 1, spec(5, 1), error) && buffs.active_count(1) == 1 && live(kWalk) == base + c5[kWalk],
                  "An equal-strength newcomer replaces the instance at capacity 1");
            check(buffs.remove_timed_buff(session, 1, "Skill_Berserk_KillingFrenzy", error) && live(kWalk) == base, "Cleanup");
        }

        // ---- 3. Event passive (KillingFrenzy-style) through the bus into the buff owner, on the real session ----
        {
            CombatResultEventBusV1 bus;
            const ActorId player = 1, monster = 2;
            int hits = 0;
            bus.subscribe(player, kCombatCallbackTargetHitV1, "Skill_Berserk_KillingFrenzy",
                          [&](const CombatResultEventV1& e, ActorId receiver) {
                              // The Lua checks attacker == self; the bus delivers both sides, so the listener must do the same.
                              if (receiver != player || e.attacker != player) return;
                              ++hits;
                              TimedBuffSpecV1 s;
                              s.class_name = "Skill_Berserk_KillingFrenzy";
                              s.strength = std::uint32_t(std::min(hits, 5));
                              s.duration_ms = 5000.0;
                              s.capacity = 0;
                              s.source = "killing frenzy";
                              std::string e2;
                              check(buffs.apply_timed_buff(session, player, s, e2), e2);
                          });
            const auto base = live(kWalk);
            bus.dispatch({monster, player, 0u}); // monster hits the player: attacker != player, no stack
            check(hits == 0 && live(kWalk) == base, "A hit the player takes must not feed the attacker-only passive");
            bus.dispatch({player, monster, 0u});
            bus.dispatch({player, monster, 0u});
            bus.dispatch({player, monster, 1u}); // a miss: OnTargetHit is not selected
            check(hits == 2, "Two player hits must run the hit listener twice and the miss must not");
            check(live(kWalk) == base + buff_cells("Skill_Berserk_KillingFrenzy", 1)[kWalk] + buff_cells("Skill_Berserk_KillingFrenzy", 2)[kWalk],
                  "Event passive must stack its class contribution per hit");
            bus.unsubscribe_key(player, "Skill_Berserk_KillingFrenzy");
            check(buffs.remove_timed_buff(session, player, "Skill_Berserk_KillingFrenzy", error) && live(kWalk) == base, "Cleanup event passive");
        }

        std::cout << "PASS passive_skill_owner_v1: OnTargetHit bus fan-out and roles, Toughness rank changes via the generic passive owner, "
                     "capacity admission (unlimited, eviction, rejection, replace), KillingFrenzy-style event stacking on the real session\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL passive_skill_owner_v1: " << ex.what() << '\n';
        return 1;
    }
}
