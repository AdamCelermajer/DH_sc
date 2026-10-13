// v21.8 skills-a: tests for the reusable timed buff owner (runtime_timed_buff_v1) against a real source Session.
// The Skill_Rogue_Quickness_buff numbers are read from the authored ClassTable, not restated here: the class row
// is `Speed_Modifier_Walk/Rotation/Attack += 5120 + 384 * SnS_Level` (raw fixed point /256, percent).
#include "runtime_timed_buff_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"

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
bool near(double a, double b) { return std::fabs(a - b) < 1e-4; }
constexpr unsigned kWalk = 46, kRotation = 47, kAttack = 48, kSnsLevel = 172, kMana = 41;
}

int main(int argc, char** argv) {
    try {
        // v21.8 slow decision (F_ApplyResult byte+25 bit0 -> PROPS_DebuffSlow): pure rules, no session needed.
        {
            dh2::data::PropertySheet attacker{};
            attacker[189] = 2000 << 8; // SnS_Slow_Duration (Water faery, skill mask 0x10000)
            attacker[146] = 500 << 8;  // default slow property
            const auto a = debuff_slow_spec_v1(0x100u, 0x10000u, attacker, false);
            check(a && a->class_name == "Debuff_Slow" && a->duration_ms == 2000.0 && a->strength == 0,
                  "Slow outcome with mask 0x10000 must use property 189 as a Debuff_Slow duration");
            const auto b = debuff_slow_spec_v1(0x100u, 0x0u, attacker, false);
            check(b && b->duration_ms == 500.0, "Slow outcome without mask 0x10000 must use property 146");
            check(!debuff_slow_spec_v1(0x80u, 0x10000u, attacker, false), "A push outcome must not slow");
            check(!debuff_slow_spec_v1(0x0u, 0x10000u, attacker, false), "No outcome must not slow");
            check(!debuff_slow_spec_v1(0x100u, 0x10000u, attacker, true), "A boss defender must not be slowed");
            attacker[189] = 0;
            check(!debuff_slow_spec_v1(0x100u, 0x10000u, attacker, false), "A zero slow duration must not slow");
        }
        check(argc == 2, "Supply repository root");
        const std::filesystem::path repo(argv[1]);
        AssetCatalog assets(repo / ".local-inputs/windows-source-clock-v19-preview-9/assets");
        std::string error;
        OriginalPropertyDatabase properties;
        check(load_original_property_tables(assets, "original-cache/data/pydata", properties, error), error);
        OriginalMeleeBindings bindings;
        check(bindings.load(AssetCatalog(repo / ".local-inputs/windows-melee-bindings"),
                            "original-melee-bindings.xml", error), error);
        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan plan;
        check(build_original_combat_visual_plan(assets, bindings, "KnightPlayerBase",
                  customization, "timed-buff-test", plan, error), error);
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
        profile.propertyOptions = {256, false}; // presentation-only profile: vitals are seeded below
        profile.sourceAnimationClips = plan.config.clips;
        config.profiles.emplace(config.playerProfileId, profile);
        CombatSession session;
        CharacterVisual player_visual;
        ActorPopulation population;
        check(session.initialize(assets, properties, bindings, config, player_visual,
                  population, {0, 0, 0}, customization, error), error);
        check(session.actor(1) && session.world()->combat_properties(1), "Timed buff Session lacks the player");
        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(properties.characters, rules, error), error);
        {
            // The presentation-only fixture profile has no vitals; give the player the sheet's own maximum HP/MP.
            auto seeded = *session.world()->combat_properties(1);
            seeded.sheets.resolved[36] = seeded.sheets.resolved[38];
            seeded.sheets.resolved[41] = seeded.sheets.resolved[43];
            check(session.world()->update_combat_properties(1, seeded, *session.world()->traits(1), error), error);
            session.actor(1)->max_health = float(seeded.sheets.resolved[38]) / 256.0f;
            session.actor(1)->health = session.actor(1)->max_health;
            check(session.actor(1)->alive(), "Timed buff test player must be alive");
        }

        const auto class_id = [&](const char* name) {
            const auto found = std::find(properties.classes.names.begin(), properties.classes.names.end(), name);
            check(found != properties.classes.names.end(), std::string("authored class absent: ") + name);
            return std::int32_t(found - properties.classes.names.begin());
        };
        // The authored class row, evaluated independently: walk/rotation/attack = 5120 + 384 * level.
        const auto buff_cells = [&](const char* name, std::uint32_t level) {
            dh2::data::PropertySheet sheet = rules.defaults, owner = session.world()->combat_properties(1)->sheets.resolved;
            sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
            check(dh2::data::apply_class(properties.classes, class_id(name), sheet, error, &owner), error);
            return sheet;
        };
        const auto quick1 = buff_cells("Skill_Rogue_Quickness_buff", 1);
        const auto quick3 = buff_cells("Skill_Rogue_Quickness_buff", 3);
        check(quick1[kWalk] == 5120 + 384 && quick1[kRotation] == 5120 + 384 && quick1[kAttack] == 5120 + 384 &&
              quick3[kWalk] == 5120 + 3 * 384, "Skill_Rogue_Quickness_buff class row no longer matches the recorded 5120 + 384 x level");

        std::vector<std::string> fx_played, fx_dropped;
        std::uintptr_t next_fx = 100;
        bool fx_fails = false;
        TimedBuffBindingsV1 buff_bindings;
        buff_bindings.classes = &properties.classes;
        buff_bindings.rules = &rules;
        buff_bindings.fx.play = [&](const std::string& set, ActorId, std::uintptr_t& identity, std::string& e) {
            if (fx_fails) { e = "fx unavailable"; return false; }
            fx_played.push_back(set);
            identity = next_fx++;
            return true;
        };
        buff_bindings.fx.drop = [&](std::uintptr_t identity, std::string&) {
            fx_dropped.push_back(std::to_string(identity));
            return true;
        };
        RuntimeTimedBuffOwnerV1 owner(buff_bindings);

        const auto live = [&]() -> const dh2::data::PropertySheet& { return session.world()->combat_properties(1)->sheets.resolved; };
        const auto base = live();
        const double base_walk = original_speed_modifier(base[kWalk]);
        check(near(base_walk, 1.0), "Fresh Knight is expected to start at walk rate 1.0");

        TimedBuffSpecV1 spec;
        spec.class_name = "Skill_Rogue_Quickness_buff";
        spec.strength = 1;
        spec.duration_ms = 1000.0;
        spec.fx_set_name = "spell_dh2_prince_rogue_quickness";
        spec.source = "Quickness";

        // 1. Apply: walk, rotation and attack speed rise by the class amount; the rest of the sheet is untouched.
        check(owner.apply_timed_buff(session, 1, spec, error), error);
        check(owner.active(1, spec.class_name) && owner.active_count(1) == 1 && near(owner.remaining_ms(1, spec.class_name), 1000.0),
              "Applied buff is not tracked");
        check(live()[kWalk] == base[kWalk] + quick1[kWalk] && live()[kRotation] == base[kRotation] + quick1[kRotation] &&
              live()[kAttack] == base[kAttack] + quick1[kAttack], "Buff did not add its class amount to the live speed cells");
        check(near(original_speed_modifier(live()[kWalk]), 1.215) && near(original_speed_modifier(live()[kAttack]), 1.215),
              "Quickness level 1 must give 21.5 percent walk and attack speed");
        for (unsigned p = 0; p < 224; ++p)
            if (p != kWalk && p != kRotation && p != kAttack)
                check(live()[p] == base[p], "Buff changed an unrelated property cell " + std::to_string(p));
        check(owner.take_sheet_dirty(1) && !owner.take_sheet_dirty(1), "Sheet dirty flag must fire once per change");
        check(fx_played.size() == 1 && fx_played[0] == spec.fx_set_name, "Buff FX was not requested exactly once");
        auto events = owner.take_events();
        check(events.size() == 1 && events[0].kind == TimedBuffEventV1::Kind::applied && events[0].changed_cells.size() == 3,
              "Apply must report one applied event with the three changed cells");

        // 2. Another system writes the same sheet (mana debit style copy-modify): the buff must survive it.
        {
            auto props = *session.world()->combat_properties(1);
            props.sheets.resolved[kMana] -= 256;
            check(session.world()->update_combat_properties(1, props, *session.world()->traits(1), error), error);
            check(owner.advance(session, 0.0, error), error);
            check(live()[kWalk] == base[kWalk] + quick1[kWalk] && live()[kMana] == base[kMana] - 256,
                  "A copy-modify sheet write lost the buff or was undone by it");
            check(owner.take_events().empty(), "No event is expected while the overlay is intact");
        }

        // 3. The sheet is rebuilt without the buff (equipment/stat rebuild): the next advance re-asserts it.
        {
            auto props = *session.world()->combat_properties(1);
            props.sheets.resolved[kWalk] = base[kWalk];
            props.sheets.resolved[kAttack] = base[kAttack];
            check(session.world()->update_combat_properties(1, props, *session.world()->traits(1), error), error);
            check(owner.advance(session, 0.0, error), error);
            check(live()[kWalk] == base[kWalk] + quick1[kWalk] && live()[kAttack] == base[kAttack] + quick1[kAttack],
                  "Overlay was not re-asserted after an external rebuild");
            events = owner.take_events();
            check(events.size() == 1 && events[0].kind == TimedBuffEventV1::Kind::reasserted && owner.take_sheet_dirty(1),
                  "Re-assertion must report one reasserted event and raise the dirty flag");
        }

        // 4. Time: 0.4 s in two steps keep the buff, the step crossing 1.0 s expires it, restores the cells, drops the FX.
        check(owner.advance(session, 0.4, error), error);
        check(owner.advance(session, 0.4, error), error);
        check(owner.active(1, spec.class_name) && near(owner.remaining_ms(1, spec.class_name), 200.0) && fx_dropped.empty(),
              "Buff aged incorrectly before its expiry");
        owner.take_sheet_dirty(1);
        owner.take_events();
        check(owner.advance(session, 0.3, error), error);
        check(!owner.active(1, spec.class_name) && owner.active_count(1) == 0, "Buff did not expire at its duration");
        check(live()[kWalk] == base[kWalk] && live()[kRotation] == base[kRotation] && live()[kAttack] == base[kAttack],
              "Expiry did not restore the speed cells");
        check(fx_dropped.size() == 1 && owner.take_sheet_dirty(1), "Expiry did not drop the FX and raise the dirty flag");
        events = owner.take_events();
        check(events.size() == 1 && events[0].kind == TimedBuffEventV1::Kind::expired, "Expiry must report one expired event");

        // 5. Re-cast replaces the same class (Lua RemoveBuff then CreateBuff) and a higher level is stronger.
        spec.strength = 1;
        check(owner.apply_timed_buff(session, 1, spec, error), error);
        spec.strength = 3;
        spec.duration_ms = 2000.0;
        check(owner.apply_timed_buff(session, 1, spec, error), error);
        check(owner.active_count(1) == 1 && near(owner.remaining_ms(1, spec.class_name), 2000.0) &&
              live()[kWalk] == base[kWalk] + quick3[kWalk], "Re-cast must replace the previous buff, not stack it");
        check(fx_played.size() == 3 && fx_dropped.size() == 2, "Replacement must drop the old FX and play a new one");

        // 6. A second buff class stacks on the same cells (type-4 accumulate); each expires on its own clock.
        TimedBuffSpecV1 shrine;
        shrine.class_name = "Shrine_Quickness_buff";
        shrine.strength = 1;
        shrine.duration_ms = 500.0;
        shrine.source = "Shrine";
        const auto shrine_cells = buff_cells("Shrine_Quickness_buff", 1);
        check(shrine_cells[kWalk] > 0, "Shrine_Quickness_buff is expected to raise walk speed");
        check(owner.apply_timed_buff(session, 1, shrine, error), error);
        check(owner.active_count(1) == 2 &&
              live()[kWalk] == base[kWalk] + quick3[kWalk] + shrine_cells[kWalk], "Two buff classes must accumulate");
        check(owner.advance(session, 0.6, error), error);
        check(owner.active_count(1) == 1 && live()[kWalk] == base[kWalk] + quick3[kWalk],
              "Only the shorter buff may expire; the other keeps its amount");

        // 7. Explicit removal (Lua RemoveBuff) restores the cells and is a no-op when nothing is active.
        check(owner.remove_timed_buff(session, 1, spec.class_name, error), error);
        check(owner.active_count(1) == 0 && live()[kWalk] == base[kWalk], "Removal did not restore the speed cells");
        check(owner.remove_timed_buff(session, 1, spec.class_name, error), error);

        // 8. Presentation failure never blocks the gameplay effect.
        fx_fails = true;
        spec.strength = 1;
        check(owner.apply_timed_buff(session, 1, spec, error), error);
        check(live()[kWalk] == base[kWalk] + quick1[kWalk], "FX failure must not block the buff");
        owner.take_events();
        check(owner.remove_timed_buff(session, 1, spec.class_name, error), error);
        fx_fails = false;

        // 9. Rejections leave the sheet alone.
        TimedBuffSpecV1 bad = spec;
        bad.class_name = "NoSuchClassRow";
        check(!owner.apply_timed_buff(session, 1, bad, error) && live()[kWalk] == base[kWalk], "Unknown class must be rejected");
        bad = spec;
        bad.duration_ms = 0.0;
        check(!owner.apply_timed_buff(session, 1, bad, error) && live()[kWalk] == base[kWalk], "Zero duration must be rejected without permanent");
        bad.duration_ms = -5.0;
        bad.permanent = true;
        check(!owner.apply_timed_buff(session, 1, bad, error) && live()[kWalk] == base[kWalk], "Permanent with a nonzero duration must be rejected");
        check(!owner.apply_timed_buff(session, 77, spec, error), "A foreign actor must be rejected");

        // 9b. A permanent buff (duration 0, Celest passive CreateBuff(id, 0, false, 1)): no expiry until RemoveBuff.
        bad = spec;
        bad.duration_ms = 0.0;
        bad.permanent = true;
        check(owner.apply_timed_buff(session, 1, bad, error), error);
        check(owner.advance(session, 600.0, error), error);
        check(owner.active(1, spec.class_name), "A duration 0 (permanent) buff must not expire");
        check(owner.remove_timed_buff(session, 1, spec.class_name, error), error);
        check(!owner.active(1, spec.class_name) && live()[kWalk] == base[kWalk], "Removing a permanent buff must restore the sheet");

        // 10. A dead actor drops its buffs (the owner released them); nothing is published.
        check(owner.apply_timed_buff(session, 1, spec, error), error);
        session.actor(1)->health = 0.0f;
        check(owner.advance(session, 0.016, error), error);
        check(owner.active_count(1) == 0, "A dead actor must lose its buffs");
        session.actor(1)->health = session.actor(1)->max_health;

        std::cout << "PASS runtime_timed_buff_v1: Quickness L1 walk/rotation/attack x" << original_speed_modifier(base[kWalk] + quick1[kWalk])
                  << ", copy-modify survival, re-assertion, expiry/restore, replace, stacking, removal, FX failure, rejections, death\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL runtime_timed_buff_v1: " << ex.what() << '\n';
        return 1;
    }
}
