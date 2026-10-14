// v21.8 w2-archer: unit coverage for RuntimeArcherSkillOwnerV1 (ExplodingArrows, MultiHitArrow, PinShot, PoisonArrows, Volley).
//
// Original (prince_archer_*.luac, plain Lua; coordination/v21.8/sub-archer-phase1.md and w2-archer2-report.md): every skill
// spawns skill arrows with SpawnSkillProjectile (they fly along the owner's heading) and finishes in the `delayed` callback
// (SkillCombatRoll) or a StartSkillTimerCB timer. Covered on a real CombatSession (caster + four lizards) with the shared
// projectile executor driven through its contact()/update() entry points:
//   - the five profile rows (requires_bow, external owner, ranges) and handles();
//   - PoisonArrows: one ArrowPoison projectile, the contact rolls the lizard;
//   - ExplodingArrows: first hit, one 500 ms timer, then FX + sound + at most MAX_COUNT (3) victims around the target;
//   - MultiHitArrow: three Uses (one per do_skill marker), hit counter, great knockback only on hit 3 with one potential target;
//   - PinShot: first arrow at Use, second/third at 750/1500 ms (level gates 4 and 9), stun on the first and second victim only;
//   - Volley: rebound chance from TempProp1 through the shared RNG service, rebound arrow spawned ON the first victim toward a
//     different enemy, and the Lua target swap when it reports the first victim again.
// Not covered here (real runs only): projectile flight against the native physical world, FX/sound files, animation.
#include "runtime_archer_passive_owner_v1.hpp"
#include "runtime_archer_skill_owner_v1.hpp"
#include "runtime_skill_use_profile_registry_v1.hpp"
#include "../../combat_session.hpp"
#include "../../content_paths.hpp"
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

bool has_line(const std::vector<std::string>& trace, const std::string& needle) {
    return std::any_of(trace.begin(), trace.end(), [&](const std::string& line) { return line.find(needle) != std::string::npos; });
}
std::size_t count_lines(const std::vector<std::string>& trace, const std::string& needle) {
    return static_cast<std::size_t>(std::count_if(trace.begin(), trace.end(), [&](const std::string& line) { return line.find(needle) != std::string::npos; }));
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

        // ---- profile rows and handles() -------------------------------------------------------------------------------
        struct Row { const char* name; const char* script; float range; };
        const Row rows[] = {{"ExplodingArrows", "prince_archer_exploding_arrows", 1200.0f}, {"MultiHitArrow", "prince_archer_multihit", 600.0f},
                            {"PinShot", "prince_archer_pin_shot", 600.0f}, {"PoisonArrows", "prince_archer_poison_arrows", 600.0f},
                            {"Volley", "prince_archer_volley", 900.0f}};
        RuntimeArcherSkillOwnerV1 bare(RuntimeArcherSkillServicesV1{});
        for (const auto& row : rows) {
            const auto* profile = find_skill_use_profile_registered_v1(row.name, row.script);
            check(profile != nullptr, std::string("Archer profile row missing: ") + row.name);
            check(profile->requires_bow && profile->external_owner && !profile->rolls_combat && profile->post_clear_target && profile->pre_look_at,
                  std::string("Archer profile flags wrong: ") + row.name);
            check(profile->pre_range == row.range, std::string("Archer Pre range differs from the Lua RANGE: ") + row.name);
            check(profile->use_targets == SkillUseTargetsV1::pre_list_all, std::string("Archer OnSkill_ must pop the Pre list: ") + row.name);
            check(bare.handles(*profile), std::string("Owner does not handle ") + row.name);
        }
        check(find_skill_use_profile_registered_v1("PinShot", "prince_archer_pin_shot")->sets_cooldown &&
              find_skill_use_profile_registered_v1("ExplodingArrows", "prince_archer_exploding_arrows")->sets_cooldown &&
              !find_skill_use_profile_registered_v1("Volley", "prince_archer_volley")->sets_cooldown &&
              !find_skill_use_profile_registered_v1("MultiHitArrow", "prince_archer_multihit")->sets_cooldown,
              "Only ExplodingArrows and PinShot call SetSkillCooldown in OnPreSkill_");
        check(!bare.handles(*find_skill_use_profile_registered_v1("ViciousStrike", "prince_rogue_vicious_strike")),
              "The Archer owner must not take shared Rogue skills");
        // Lua ANGLE 220/270 are full angles: native half-angles 110/135 degrees.
        check(std::fabs(find_skill_use_profile_registered_v1("Volley", "prince_archer_volley")->pre_cone_radians - 135.0f * 0.017453f) < 1e-4f,
              "Volley ANGLE 270 is not the native half-angle");
        std::cout << "profiles: 5 Archer rows (HasBow, external owner, authored RANGE/ANGLE/cooldown), handles() ok\n";

        // ---- real session: caster + four lizards ---------------------------------------------------------------------------
        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan base;
        check(build_original_combat_visual_plan(assets, bindings, "MagePlayerBase", customization, "archer-owner-test", base, error), error);
        CombatSessionConfig config;
        config.diagnosticRngSeed = 77;
        config.playerId = 1;
        config.playerProfileId = "MagePlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = base.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        config.mainItemId = "Staff01";
        config.equippedItemIds = {"StartingSuitMage", "StartingBootsMage", "StartingGlovesMage", "Staff01"};
        CombatSessionProfile player;
        player.animationOnly = true;
        player.receiveDamage = true;
        player.initialIdle = {"Idle", 0, {0}};
        player.reaction = CombatSessionChoice{"Injured", 0, {0}};
        player.death = CombatSessionChoice{"Died", 0, {0}};
        player.reactionMinimalRandoms = false;
        player.propertyOptions = {256, true};
        config.profiles.emplace(config.playerProfileId, player);
        CombatSessionProfile enemy;
        OriginalAttackSelection enemyAttack;
        enemyAttack.state = "Attack";
        enemyAttack.group_path = {0};
        enemy.sequenceAction = enemyAttack;
        enemy.damageMarkerNames = {"attack_mainhand"};
        enemy.initialIdle = {"Idle", 0, {0}};
        enemy.motionRoot = "auto";
        enemy.customization = customization;
        enemy.propertyOptions = {2000 * 256, true};
        config.profiles.emplace("Swamp_LizadMan_Type1", enemy);
        ActorPopulation population;
        for (std::uint64_t id = 2; id <= 5; ++id) {
            PopulationActor placed;
            placed.profileId = "Swamp_LizadMan_Type1";
            placed.definition.stableId = id;
            placed.definition.sourceId = "archer-lizard-" + std::to_string(id);
            placed.definition.placement = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, -100, 0, 1};
            placed.transform = placed.definition.placement;
            population.actors().push_back(std::move(placed));
        }
        CharacterVisual visual;
        CombatSession session;
        check(session.initialize(assets, db, bindings, config, visual, population, {0, 0, 0}, customization, error), error);
        check(session.update(0, {}, Vec3{0, 0, 0}, 0, error), error);
        const auto origin = session.actor(1)->transform.position;
        const float heading = session.actor(1)->transform.rotation[2];
        const std::array<float, 3> look{std::sin(heading), -std::cos(heading), 0.0f};
        // Lizards in a tight cluster 400 units in front (A=2, B=3, C=4, D=5): all within 200 of A.
        const std::array<std::array<float, 3>, 4> offsets{{{0, 0, 0}, {60, 0, 0}, {-60, 0, 0}, {0, 80, 0}}};
        for (std::size_t i = 0; i < 4; ++i) {
            auto* lizard = session.actor(2 + i);
            check(lizard != nullptr, "Lizard actor is absent");
            lizard->transform.position = {origin[0] + 400.0f * look[0] + offsets[i][0], origin[1] + 400.0f * look[1] + offsets[i][1], origin[2]};
        }
        std::vector<ActorId> order{1, 2, 3, 4, 5};
        std::vector<SkillTargetSourceFactsV1> facts;
        for (const auto id : order) facts.push_back({id, true, false, false, true, true});

        auto tables = effects::load_runtime_session_projectile_tables_v1(assets, error);
        check(tables != nullptr, error);
        effects::RuntimeSessionProjectileServicesV1 projectileServices;
        projectileServices.source_is_enemy = [&](ActorId, ActorId, bool& hasAi, bool& enemyResult, std::string&) { hasAi = true; enemyResult = true; return true; };
        effects::RuntimeSessionProjectileV1 executor(session, std::move(projectileServices));
        check(executor.synchronize_session_binding(error), error);

        struct Recorder {
            std::vector<std::string> trace, effects, sounds;
            std::vector<std::pair<ActorId, double>> stuns;
            std::vector<std::pair<ActorId, bool>> knockbacks;
            std::vector<std::uint32_t> draws;
            std::uint32_t next_draw = 0;
        } rec;
        RuntimeArcherSkillServicesV1 services;
        services.projectiles = [&]() { return &executor; };
        services.projectile_tables = [&]() { return tables; };
        services.projectile_launched = [&](std::uint64_t, bool, std::string&) { return true; };
        services.play_effect = [&](const std::string& name, const std::array<float, 3>&, std::string&) { rec.effects.push_back(name); return true; };
        services.play_sound = [&](const std::string& name, std::string&) { rec.sounds.push_back(name); return true; };
        services.set_stun = [&](ActorId target, double ms, std::string&) { rec.stuns.push_back({target, ms}); return true; };
        services.knock_back = [&](CombatSession&, const RuntimeSkillKnockBackRequestV1& knock, std::string&) { rec.knockbacks.push_back({knock.target, knock.great}); return true; };
        services.random = [&](std::uint32_t bound, std::uint32_t& value, std::string&) { rec.draws.push_back(bound); value = rec.next_draw; return true; };
        services.trace = [&](const std::string& line) { rec.trace.push_back(line); };
        RuntimeArcherSkillOwnerV1 owner(std::move(services));

        const auto field = [&](const char* name) {
            const auto found = std::find(db.characters.fields.begin(), db.characters.fields.end(), name);
            check(found != db.characters.fields.end(), std::string("CharacterProperties field missing: ") + name);
            return static_cast<std::size_t>(found - db.characters.fields.begin());
        };
        dh2::data::PropertySheet sheet = session.world()->combat_properties(1)->sheets.resolved;
        sheet[field("SnS_MinDamage")] = 100 * 256;
        sheet[field("SnS_MaxDamage")] = 100 * 256;
        sheet[field("TempProp1")] = 200 * 256;     // ExplodingArrows RANGE_EXPL / Volley chances (tests set what they need)
        sheet[field("SnS_Duration")] = 2000 * 256; // PinShot stun duration (ms)

        std::uint64_t generation = 0;
        std::uint64_t frame = 1;
        const auto make_context = [&](const char* name, const char* script, std::uint32_t rank, const std::vector<ActorId>& list) {
            RuntimeSkillExternalContextV1 context;
            context.caster = 1;
            context.generation = ++generation;
            context.profile = find_skill_use_profile_registered_v1(name, script);
            context.skill_name = name;
            context.rank = rank;
            context.row_mask = 0x20000u;  // damage bit of the SkillCombatRoll mask
            context.category = 0;
            context.element = 0;
            context.formula_sheet = &sheet;
            context.characters = &db.characters;
            context.population_order = &order;
            context.target_facts = &facts;
            context.non_character_attackable_objects_absent = true;
            context.ordered_character_targets = &list;
            return context;
        };
        const auto step = [&](std::int32_t dt = 0) { check(executor.update(frame++, dt, false, error), error); };
        const auto live_ids = [&]() {
            std::vector<std::uint64_t> ids;
            for (const auto& visual_packet : executor.visuals()) ids.push_back(visual_packet.projectile_id);
            return ids;
        };
        const auto hit = [&](std::uint64_t id, ActorId peer) {
            bool accepted = false;
            check(executor.contact({id, peer, session.actor(peer)->transform.position, true}, accepted, error) && accepted, "projectile contact rejected: " + error);
            step();
        };
        const auto health = [&](ActorId id) { return session.actor(id)->health; };
        std::string e;

        // ---- PoisonArrows -----------------------------------------------------------------------------------------------
        {
            std::vector<ActorId> list{2, 3};
            auto context = make_context("PoisonArrows", "prince_archer_poison_arrows", 5, list);
            check(owner.pre(session, context, e), e);
            const float before = health(2);
            check(owner.use(session, context, e), "PoisonArrows Use failed: " + e);
            const auto ids = live_ids();
            check(ids.size() == 1, "PoisonArrows did not launch exactly one arrow");
            check(has_line(rec.trace, "row=ArrowPoison"), "PoisonArrows did not use the ArrowPoison ProjectileTable row");
            hit(ids[0], 2);
            check(health(2) < before, "PoisonArrows contact did not roll damage on the victim");
            check(live_ids().empty(), "A consumed arrow stayed alive");
            std::cout << "PoisonArrows: one ArrowPoison arrow, contact rolled " << before - health(2) << " damage\n";
        }

        // ---- ExplodingArrows --------------------------------------------------------------------------------------------
        {
            rec.trace.clear();
            std::vector<ActorId> list{3};
            auto context = make_context("ExplodingArrows", "prince_archer_exploding_arrows", 5, list);
            check(owner.pre(session, context, e), e);
            check(owner.use(session, context, e), "ExplodingArrows Use failed: " + e);
            check(has_line(rec.trace, "row=ArrowExplode"), "ExplodingArrows did not use the ArrowExplode row");
            const auto ids = live_ids();
            check(ids.size() == 1, "ExplodingArrows did not launch one arrow");
            const float before3 = health(3), before2 = health(2), before4 = health(4), before5 = health(5);
            hit(ids[0], 3);
            check(health(3) < before3 && has_line(rec.trace, "ExplodingArrows first hit"), "ExplodingArrows first hit did not roll");
            check(owner.active_timers() == 1, "ExplodingArrows did not start its 500 ms DoExplosion timer");
            check(rec.effects.empty() && rec.sounds.empty(), "Explosion presentation before the 500 ms timer");
            check(owner.advance(session, 499.0, e), e);
            check(rec.effects.empty(), "DoExplosion ran before 500 ms");
            check(owner.advance(session, 2.0, e), e);
            check(owner.active_timers() == 0, "DoExplosion one-shot was not spent");
            check(rec.effects == std::vector<std::string>{"spell_dh2_prince_rogue_burst_shot"} && rec.sounds == std::vector<std::string>{"sfx_impact_burst_shot"},
                  "DoExplosion did not play its FX and sound");
            // The victims inside RANGE_EXPL of the target: A(2), C(4), D(5) and the target itself (3); MAX_COUNT 3 of 4.
            check(count_lines(rec.trace, "ExplodingArrows explosion hit") == 3, "DoExplosion did not stop at MAX_COUNT 3 victims");
            const int damaged = int(health(2) < before2) + int(health(4) < before4) + int(health(5) < before5);
            check(damaged >= 2, "DoExplosion victims were not damaged");
            std::cout << "ExplodingArrows: first hit, 500 ms timer, FX+sound, 3 explosion victims\n";
        }

        // ---- MultiHitArrow ----------------------------------------------------------------------------------------------
        {
            rec.trace.clear();
            rec.knockbacks.clear();
            std::vector<ActorId> list{4};  // exactly one potential target: hit 3 is the great knockback
            auto context = make_context("MultiHitArrow", "prince_archer_multihit", 5, list);
            check(owner.pre(session, context, e), e);
            for (int marker = 0; marker < 3; ++marker) {
                check(owner.use(session, context, e), "MultiHitArrow Use failed: " + e);
                const auto ids = live_ids();
                check(ids.size() == 1, "MultiHitArrow marker did not launch exactly one arrow");
                hit(ids[0], 4);
                check(rec.knockbacks.size() == (marker == 2 ? 1u : 0u), "MultiHitArrow knockback count wrong at marker " + std::to_string(marker + 1));
            }
            check(rec.knockbacks[0] == std::make_pair(ActorId(4), true), "The third arrow with one potential target must be a GREAT knockback");
            check(count_lines(rec.trace, "MultiHitArrow arrow hit") == 3, "MultiHitArrow did not roll three arrows");
            // Two potential targets: hits 1 and 2 are standard knockbacks, hit 3 none.
            rec.knockbacks.clear();
            std::vector<ActorId> two{4, 5};
            auto context2 = make_context("MultiHitArrow", "prince_archer_multihit", 5, two);
            check(owner.pre(session, context2, e), e);
            for (int marker = 0; marker < 3; ++marker) {
                check(owner.use(session, context2, e), e);
                const auto ids = live_ids();
                check(ids.size() == 1, "MultiHitArrow (two targets) marker did not launch one arrow");
                hit(ids[0], marker == 0 ? 4 : 5);
            }
            check(rec.knockbacks.size() == 2 && !rec.knockbacks[0].second && !rec.knockbacks[1].second, "Two potential targets: hits 1-2 are standard knockbacks");
            std::cout << "MultiHitArrow: three arrows per Use, great KB on hit 3 (one target), standard KB on hits 1-2 (two targets)\n";
        }

        // ---- PinShot ----------------------------------------------------------------------------------------------------
        {
            rec.trace.clear();
            rec.stuns.clear();
            std::vector<ActorId> list{2, 3, 4};
            auto context = make_context("PinShot", "prince_archer_pin_shot", 10, list);  // level > 9: three arrows
            check(owner.pre(session, context, e), e);
            check(owner.use(session, context, e), "PinShot Use failed: " + e);
            check(live_ids().size() == 1 && owner.active_timers() == 2, "PinShot Use must fire one arrow now and arm two timers (level 10)");
            auto first = live_ids();
            hit(first[0], 2);
            check(rec.stuns.size() == 1 && rec.stuns[0].first == 2 && rec.stuns[0].second == 2000.0, "PinShot first victim was not stunned for SnS_Duration");
            check(owner.advance(session, 749.0, e) && live_ids().empty(), "PinShot second arrow launched before 750 ms");
            check(owner.advance(session, 2.0, e) && live_ids().size() == 1, "PinShot second arrow was not launched at 750 ms");
            const auto second = live_ids();
            hit(second[0], 3);
            check(rec.stuns.size() == 2 && rec.stuns[1].first == 3, "PinShot second victim was not stunned");
            check(owner.advance(session, 750.0, e) && live_ids().size() == 1 && owner.active_timers() == 0, "PinShot third arrow was not launched at 1500 ms");
            const auto third = live_ids();
            hit(third[0], 4);
            check(rec.stuns.size() == 2, "PinShot stunned a third victim: the Lua only tracks first and second targets");
            check(has_line(rec.trace, "PinShot skipped"), "PinShot third-target skip was not reported");
            // Level 3 (<= 4): a single arrow and no timers even with three targets.
            std::vector<ActorId> list2{2, 3, 4};
            auto low = make_context("PinShot", "prince_archer_pin_shot", 3, list2);
            check(owner.pre(session, low, e) && owner.use(session, low, e), e);
            check(live_ids().size() == 1 && owner.active_timers() == 0, "PinShot level 3 must not arm extra arrows");
            hit(live_ids()[0], 2);
            std::cout << "PinShot: level 10 = 3 arrows (now, 750, 1500 ms), stun 2000 ms on victims 1-2 only; level 3 = 1 arrow\n";
        }

        // ---- Volley -----------------------------------------------------------------------------------------------------
        {
            rec.trace.clear();
            sheet[field("TempProp1")] = 50 * 256;  // chances = 50
            std::vector<ActorId> list{2};
            auto context = make_context("Volley", "prince_archer_volley", 5, list);
            check(owner.pre(session, context, e), e);
            // Draw 90 (>= 50): no rebound.
            rec.next_draw = 90;
            check(owner.use(session, context, e), e);
            check(has_line(rec.trace, "row=ArrowDefault"), "Volley did not use ArrowDefault");
            hit(live_ids().at(0), 2);
            check(rec.draws == std::vector<std::uint32_t>{101} && has_line(rec.trace, "no rebound"), "Volley must draw GetRand(0,100) once and not rebound at 90 >= 50");
            check(live_ids().empty(), "Volley rebounded despite the failed chance");
            // Draw 10 (< 50): rebound to another enemy, spawned on the first victim.
            rec.trace.clear();
            rec.draws.clear();
            rec.next_draw = 10;
            auto context2 = make_context("Volley", "prince_archer_volley", 5, list);
            check(owner.pre(session, context2, e) && owner.use(session, context2, e), e);
            hit(live_ids().at(0), 2);
            check(has_line(rec.trace, "Volley rebound arrow launched"), "Volley did not launch its rebound arrow at draw 10 < 50");
            const auto rebound = live_ids();
            check(rebound.size() == 1, "Volley must hold exactly one rebound arrow");
            const float before = health(3) + health(4) + health(5);
            // The rebound arrow reports the first victim (it spawns inside it): the Lua swaps in newTarget.
            rec.next_draw = 99;  // no further rebound
            hit(rebound[0], 2);
            check(health(3) + health(4) + health(5) < before, "Volley rebound did not damage the new target instead of the first victim");
            check(has_line(rec.trace, "Volley rebound arrow hit"), "Volley rebound hit was not reported");
            std::cout << "Volley: GetRand(0,100) vs TempProp1, no rebound at 90, rebound arrow on the first victim -> new target at 10\n";
        }
        // ---- passives: Precision and SideStep on the shared event bus ------------------------------------------------------
        {
            struct FakeBuffs final : TimedBuffSinkV1 {
                std::vector<std::string> calls;
                bool remove_timed_buff(CombatSession&, ActorId, const std::string& cls, std::string&) override { calls.push_back("remove " + cls); return true; }
                bool apply_timed_buff(CombatSession&, ActorId, const TimedBuffSpecV1& spec, std::string&) override {
                    calls.push_back("apply " + spec.class_name + " rank=" + std::to_string(spec.strength) + (spec.permanent ? " permanent" : ""));
                    return true;
                }
            } buffs;
            CombatResultEventBusV1 bus;
            bool bow = true;
            std::vector<std::string> trace;
            std::uint32_t next_draw = 0;
            std::vector<ActorId> knocked;
            RuntimeArcherPassiveServicesV1 services;
            services.has_bow = [&]() { return bow; };
            services.knock_back = [&](CombatSession&, const RuntimeSkillKnockBackRequestV1& request, std::string&) {
                check(request.attacker == 1 && request.great && request.generation != 0, "SideStep knockback request is malformed");
                knocked.push_back(request.target);
                return true;
            };
            services.random = [&](std::uint32_t bound, std::uint32_t& value, std::string&) { check(bound == 101, "GetRand(0,100) is [0,101)"); value = next_draw; return true; };
            services.trace = [&](const std::string& line) { trace.push_back(line); };
            dh2::data::PropertyRules rules;
            check(dh2::data::load_property_rules(db.characters, rules, error), error);
            RuntimeArcherPassiveOwnerV1 passives(buffs, db.classes, rules, bus, 1, services);
            passives.bind(session);
            std::string e;
            check(archer_passive_kind_for_script_v1("prince_archer_precision") == ArcherPassiveKindV1::precision &&
                  archer_passive_kind_for_script_v1("prince_archer_sidestep") == ArcherPassiveKindV1::side_step &&
                  archer_passive_kind_for_script_v1("prince_archer_bowmastery") == ArcherPassiveKindV1::none,
                  "Archer passive script mapping");
            // Precision: OnSkillUpdate_ with a bow applies the permanent buff at the rank and starts the 1000 ms check.
            check(passives.set_rank(ArcherPassiveKindV1::precision, 3, e) && passives.precision_buff_active(), "Precision did not apply at rank 3 with a bow");
            check(buffs.calls.size() == 1 && buffs.calls[0] == "apply Skill_Archer_Precision rank=3 permanent", "Precision buff call is wrong: " + (buffs.calls.empty() ? std::string("none") : buffs.calls[0]));
            check(passives.set_rank(ArcherPassiveKindV1::precision, 3, e) && buffs.calls.size() == 1, "An unchanged rank must not rerun OnSkillUpdate_");
            // OnTargetHit(attacker == self) with the buff present and a bow: nothing to do.
            bus.dispatch({1, 2, 0, false});
            check(buffs.calls.size() == 1, "Precision rebuilt a buff that is already present");
            // The player drops the bow: the 1000 ms check removes the buff.
            bow = false;
            check(passives.advance(session, 999.0, e) && passives.precision_buff_active(), "Precision removed its buff before the 1000 ms check");
            check(passives.advance(session, 2.0, e) && !passives.precision_buff_active() && buffs.calls.back() == "remove Skill_Archer_Precision", "checkbuff did not remove the buff without a bow");
            check(passives.active_timers() == 0, "checkbuff must stop its own timer");
            // A landed hit without a bow does not create the buff; with a bow it does (the Lua OnTargetHit path).
            bus.dispatch({1, 2, 0, false});
            check(!passives.precision_buff_active(), "Precision applied without a bow");
            bow = true;
            bus.dispatch({1, 2, 0, false});
            check(passives.precision_buff_active() && buffs.calls.back() == "apply Skill_Archer_Precision rank=3 permanent", "Precision OnTargetHit did not create the buff with a bow");
            // A miss (outcomes & 3) selects the missed callback: no OnTargetHit work.
            const auto calls_before = buffs.calls.size();
            check(passives.set_rank(ArcherPassiveKindV1::precision, 0, e) && !passives.precision_buff_active() && buffs.calls.size() == calls_before + 1, "Rank 0 did not remove Precision");
            bus.dispatch({1, 2, 1, false});
            check(!passives.precision_buff_active(), "Precision listened on a miss");
            // A rank learned with no bow applies nothing; the bow showing up in the main hand re-runs OnSkillUpdate_.
            bow = false;
            check(passives.advance(session, 1.0, e), e);
            check(passives.set_rank(ArcherPassiveKindV1::precision, 2, e) && !passives.precision_buff_active(), "Precision applied at a rank learned without a bow");
            bow = true;
            check(passives.advance(session, 1.0, e) && passives.precision_buff_active() && buffs.calls.back() == "apply Skill_Archer_Precision rank=2 permanent",
                  "A bow appearing did not re-run OnSkillUpdate_ for Precision");
            check(passives.set_rank(ArcherPassiveKindV1::precision, 0, e) && !passives.precision_buff_active(), "Rank 0 did not remove Precision again");
            std::cout << "Precision: permanent buff at the rank with a bow, 1000 ms checkbuff removal without one, OnTargetHit re-creation, rank 0 removes\n";

            // SideStep: defender == player, attacker in melee reach, GetRand(0,100) < TempProp1 starts a 500 ms check timer.
            auto* lizard = session.actor(2);
            check(lizard != nullptr, "SideStep lizard absent");
            lizard->transform.position = {origin[0] + 20.0f, origin[1], origin[2]};
            check(passives.set_rank(ArcherPassiveKindV1::side_step, 5, e), e);
            next_draw = 100;  // the highest GetRand value never beats the chance
            bus.dispatch({2, 1, 0, false});
            check(passives.side_step_procs() == 0 && passives.active_timers() == 0, "SideStep proc'd at the highest roll");
            next_draw = 0;
            bus.dispatch({2, 1, 1, false});  // a miss: no OnTargetHit
            check(passives.side_step_procs() == 0, "SideStep listened on a miss");
            bus.dispatch({2, 1, 0, false});
            check(passives.side_step_procs() == 1 && passives.active_timers() == 1, "SideStep did not start its check timer at roll 0");
            check(passives.advance(session, 499.0, e) && passives.side_step_checks() == 0, "SideStep check ran before 500 ms");
            check(passives.advance(session, 2.0, e) && passives.side_step_checks() == 1 && knocked.size() == 1 && knocked[0] == 2, "SideStep check did not request the attacker's knockback");
            check(has_line(trace, "SideStep check attacker=2 knockback=requested"), "SideStep check is not traced");
            // A lizard that is not in Attack after the request ends the loop (StopTimerCB + attacker:Stop()).
            check(passives.active_timers() == 0, "SideStep timer must stop when the attacker is not in Attack");
            // Out of reach: no roll at all.
            lizard->transform.position = {origin[0] + 5000.0f, origin[1], origin[2]};
            const auto draws_before = passives.side_step_procs();
            bus.dispatch({2, 1, 0, false});
            check(passives.side_step_procs() == draws_before && passives.active_timers() == 0, "SideStep proc'd outside the melee reach");
            // Rank 0 (not learned): nothing.
            lizard->transform.position = {origin[0] + 20.0f, origin[1], origin[2]};
            check(passives.set_rank(ArcherPassiveKindV1::side_step, 0, e), e);
            bus.dispatch({2, 1, 0, false});
            check(passives.side_step_procs() == draws_before, "SideStep proc'd at rank 0");
            std::cout << "SideStep: melee-range hit on the player, GetRand vs TempProp1, 500 ms knockback check, loop ends when the attacker leaves Attack\n";
        }
        std::cout << "runtime_archer_skill_owner_v1 passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "runtime_archer_skill_owner_v1 failed: " << ex.what() << '\n';
        return 1;
    }
}
