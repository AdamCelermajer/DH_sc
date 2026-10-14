// v21.8 w4-grapple: unit coverage for GrapplingHook (RuntimeRogueSkillOwnerV1) and the general forced move (ForcedHeadToV1).
//
// Original (prince_assassin_grapling_hook.luac, plain Lua; coordination/v21.8/w4-grapple-report.md): OnSkill_ plays the knife
// sound and SpawnSkillProjectile(GrapplingHook) on the caster heading; the delayed body on a hit runs SkillCombatRoll, then
// SetStunState(duration, true) unless boss/miniboss, then HeadTo(caster position) while the target is Stunned.
// Covered on a real CombatSession (caster + lizards) with the shared projectile executor driven through contact()/update():
//   - the profile row (external owner, no roll in the coordinator, Pre query, Post ClearTarget, no cooldown);
//   - ForcedHeadToV1 on its own: steps by speed*dt in XY, keeps Z, arrival, refusal, cancel, gone, move error;
//   - hit -> roll (damage, victim hurt), stun with the projected SnS_Duration, pull toward the caster while Stunned,
//     pull steps through the move service and ends at arrival; a refused (not stunned) target is not pulled;
//   - boss/miniboss: the roll happens, the stun and the pull do not.
// Not covered here (real run only): the flight of the knife against the native world, the FX, the sounds, the animation.
#include "forced_head_to_v1.hpp"
#include "runtime_rogue_skill_owner_v1.hpp"
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

// ---- ForcedHeadToV1 alone (no session) ---------------------------------------------------------------------------------
void test_forced_head_to() {
    std::map<ActorId, std::array<float, 3>> positions{{7, {0.0f, 0.0f, 5.0f}}};
    bool may_continue = true;
    bool fail_move = false;
    ForcedHeadToServicesV1 services;
    services.position_of = [&](ActorId id, std::array<float, 3>& p) {
        const auto found = positions.find(id);
        if (found == positions.end()) return false;
        p = found->second;
        return true;
    };
    services.move = [&](ActorId id, const std::array<float, 3>& p, std::string& error) {
        if (fail_move) { error = "move refused"; return false; }
        positions[id] = p;
        return true;
    };
    services.may_continue = [&](ActorId) { return may_continue; };
    ForcedHeadToV1 pull(std::move(services));
    std::string error;

    // Steps: 250 units/s over 1000 ms along +X, Z kept; arrival radius 100 ends the command.
    pull.start(7, {1000.0f, 0.0f, 900.0f}, 250.0f, 100.0f);
    check(pull.active(7) && pull.size() == 1, "HeadTo command not registered");
    check(pull.advance(1000.0, error) && error.empty(), "HeadTo advance failed: " + error);
    check(std::fabs(positions[7][0] - 250.0f) < 1e-3f && std::fabs(positions[7][1]) < 1e-3f, "HeadTo step is not speed*dt along the goal");
    check(positions[7][2] == 5.0f, "HeadTo changed the Z of the actor (Z must stay the actor's own)");
    for (int i = 0; i < 10 && pull.active(7); ++i) check(pull.advance(1000.0, error), error);
    check(!pull.active(7) && pull.last_end(7) == "arrived", "HeadTo did not end on arrival: " + pull.last_end(7));
    check(positions[7][0] >= 900.0f - 1e-3f && positions[7][0] <= 1000.0f, "HeadTo stopped outside the arrival radius");

    // Refused: the continue predicate ends the command without a move.
    positions[7] = {0.0f, 0.0f, 5.0f};
    pull.start(7, {1000.0f, 0.0f, 0.0f}, 250.0f, 100.0f);
    may_continue = false;
    check(pull.advance(500.0, error) && !pull.active(7) && pull.last_end(7) == "refused", "HeadTo ignored the continue predicate");
    check(positions[7][0] == 0.0f, "HeadTo moved an actor it refused");
    may_continue = true;

    // Cancel and gone.
    pull.start(7, {1000.0f, 0.0f, 0.0f}, 250.0f, 100.0f);
    pull.cancel(7);
    check(!pull.active(7) && pull.last_end(7) == "cancelled", "HeadTo cancel");
    pull.start(7, {1000.0f, 0.0f, 0.0f}, 250.0f, 100.0f);
    positions.erase(7);
    check(pull.advance(100.0, error) && !pull.active(7) && pull.last_end(7) == "gone", "HeadTo did not end for an absent actor");

    // Move error: the command stops and the error is returned.
    positions[7] = {0.0f, 0.0f, 5.0f};
    fail_move = true;
    pull.start(7, {1000.0f, 0.0f, 0.0f}, 250.0f, 100.0f);
    check(!pull.advance(100.0, error) && error == "move refused" && !pull.active(7), "HeadTo did not report the move error");
    fail_move = false;
    std::cout << "forced HeadTo: step, Z kept, arrival, refusal, cancel, gone, move error\n";
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

        test_forced_head_to();

        // ---- profile row ----------------------------------------------------------------------------------------------
        const auto* profile = find_skill_use_profile_registered_v1("GrapplingHook", "prince_assassin_grapling_hook");
        check(profile != nullptr, "GrapplingHook profile row missing");
        check(profile->external_owner && !profile->rolls_combat && profile->post_clear_target && profile->pre_look_at,
              "GrapplingHook flags differ from the Lua (external owner, no coordinator roll, LookAt, Post ClearTarget)");
        check(profile->pre_range == 1000.0f, "GrapplingHook Pre range differs from the Lua RANGE 1000");
        check(!profile->sets_cooldown && !profile->requires_bow && profile->enemy_stun == nullptr && profile->enemy_buff == nullptr,
              "GrapplingHook has a cooldown or an arm the Lua does not have");
        check(std::fabs(profile->pre_cone_radians - 110.0f * 0.017453292f) < 1e-3f, "GrapplingHook ANGLE 220 is not the native half-angle 110");
        check(RuntimeRogueSkillOwnerV1(RuntimeRogueSkillServicesV1{}).handles(*profile), "Rogue owner does not handle GrapplingHook");
        std::cout << "profile: GrapplingHook row (RANGE 1000, half-angle 110, external owner, no cooldown, Post ClearTarget)\n";

        // ---- real session: caster (1) + lizards -----------------------------------------------------------------------------
        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan base;
        check(build_original_combat_visual_plan(assets, bindings, "MagePlayerBase", customization, "rogue-owner-test", base, error), error);
        CombatSessionConfig config;
        config.diagnosticRngSeed = 91;
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
        for (std::uint64_t id = 2; id <= 3; ++id) {
            PopulationActor placed;
            placed.profileId = "Swamp_LizadMan_Type1";
            placed.definition.stableId = id;
            placed.definition.sourceId = "rogue-lizard-" + std::to_string(id);
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
        for (std::size_t i = 0; i < 2; ++i) {
            auto* lizard = session.actor(2 + i);
            check(lizard != nullptr, "Lizard actor is absent");
            lizard->transform.position = {origin[0] + 300.0f * look[0] + 40.0f * static_cast<float>(i), origin[1] + 300.0f * look[1], origin[2]};
        }
        std::vector<ActorId> order{1, 2, 3};
        std::vector<SkillTargetSourceFactsV1> facts;
        for (const auto id : order) facts.push_back({id, true, false, false, true, true});

        auto tables = effects::load_runtime_session_projectile_tables_v1(assets, error);
        check(tables != nullptr, error);
        effects::RuntimeSessionProjectileServicesV1 projectileServices;
        projectileServices.source_is_enemy = [&](ActorId, ActorId, bool& hasAi, bool& enemyResult, std::string&) { hasAi = true; enemyResult = true; return true; };
        effects::RuntimeSessionProjectileV1 executor(session, std::move(projectileServices));
        check(executor.synchronize_session_binding(error), error);

        struct Recorder {
            std::vector<std::string> trace, sounds;
            std::vector<std::pair<ActorId, double>> stuns;
            std::map<ActorId, bool> stunned;     // the ES-STATUS state the test drives
            bool stun_takes = true;              // false: the stun request is made but the target is not stunned
            std::map<ActorId, bool> boss;        // IsBoss / IsMiniBoss
        } rec;
        RuntimeRogueSkillServicesV1 services;
        services.projectiles = [&]() { return &executor; };
        services.projectile_tables = [&]() { return tables; };
        services.projectile_launched = [&](std::uint64_t, bool, std::string&) { return true; };
        services.play_sound = [&](const std::string& name, std::string&) { rec.sounds.push_back(name); return true; };
        services.set_stun = [&](ActorId target, double ms, bool& applied, std::string&) {
            applied = true;
            rec.stuns.push_back({target, ms});
            if (rec.stun_takes) rec.stunned[target] = true;
            return true;
        };
        services.is_stunned = [&](ActorId target) { return rec.stunned[target]; };
        services.is_boss_or_miniboss = [&](ActorId target) { return rec.boss[target]; };
        services.move_actor = [&](ActorId actor, const std::array<float, 3>& position, std::string&) {
            session.actor(actor)->transform.position = position;
            return true;
        };
        services.trace = [&](const std::string& line) { rec.trace.push_back(line); };
        RuntimeRogueSkillOwnerV1 owner(std::move(services));

        const auto field = [&](const char* name) {
            const auto found = std::find(db.characters.fields.begin(), db.characters.fields.end(), name);
            check(found != db.characters.fields.end(), std::string("CharacterProperties field missing: ") + name);
            return static_cast<std::size_t>(found - db.characters.fields.begin());
        };
        dh2::data::PropertySheet sheet = session.world()->combat_properties(1)->sheets.resolved;
        sheet[field("SnS_MinDamage")] = 100 * 256;
        sheet[field("SnS_MaxDamage")] = 100 * 256;
        sheet[field("SnS_Duration")] = 2000 * 256;  // GrapplingHook stun duration (ms) at the projected rank

        RuntimeSkillExternalContextV1 context;
        context.caster = 1;
        context.generation = 1;
        context.profile = profile;
        context.skill_name = "GrapplingHook";
        context.rank = 12;
        context.row_mask = 0x20000u;
        context.category = 0;
        context.element = 0;
        context.formula_sheet = &sheet;
        context.characters = &db.characters;
        context.population_order = &order;
        context.target_facts = &facts;
        context.non_character_attackable_objects_absent = true;
        std::vector<ActorId> list{2, 3};
        context.ordered_character_targets = &list;

        std::uint64_t frame = 1;
        const auto step = [&](std::int32_t dt = 0) { check(executor.update(frame++, dt, false, error), error); };
        const auto hit = [&](std::uint64_t id, ActorId peer) {
            bool accepted = false;
            check(executor.contact({id, peer, session.actor(peer)->transform.position, true}, accepted, error) && accepted, "knife contact rejected: " + error);
            step();
        };
        const auto dist_to_caster = [&](ActorId id) {
            const auto p = session.actor(id)->transform.position;
            const auto c = session.actor(1)->transform.position;
            return std::hypot(p[0] - c[0], p[1] - c[1]);
        };
        std::string e;

        // ---- Use: sound + one knife; hit on lizard 2: roll, stun 2000 ms, pull toward the caster ---------------------------
        check(owner.use(session, context, e), "GrapplingHook Use failed: " + e);
        check(rec.sounds.size() == 1 && rec.sounds[0] == "sfx_skill_rogue_ass_throwing_knife", "GrapplingHook PlaySound label differs from the Lua");
        const auto knives = executor.visuals();
        check(knives.size() == 1, "GrapplingHook Use must launch exactly one knife");
        const auto knife = knives[0].projectile_id;
        const float health_before = session.actor(2)->health;
        const float start_distance = dist_to_caster(2);
        hit(knife, 2);
        check(session.actor(2)->health < health_before, "GrapplingHook roll did not damage the hit lizard");
        check(rec.stuns.size() == 1 && rec.stuns[0].first == 2 && std::fabs(rec.stuns[0].second - 2000.0) < 1e-6,
              "GrapplingHook stun is not SetStunState(SnS_Duration, true) on the hit target");
        check(owner.active_pulls() == 1, "GrapplingHook did not start the pull on a stunned target");
        check(has_line(rec.trace, "pull start victim=2"), "GrapplingHook pull start is not traced");
        check(owner.advance(session, 1000.0, e), e);
        const float moved = start_distance - dist_to_caster(2);
        check(std::fabs(moved - 250.0f) < 1.0f, "Pull step is not speed*dt: moved " + std::to_string(moved));
        check(owner.advance(session, 60000.0, e), e);
        check(owner.active_pulls() == 0, "Pull did not end at arrival");
        check(dist_to_caster(2) <= session.world()->melee_reach(1) + 1.0f, "Pulled lizard did not arrive within the caster's melee reach");
        std::cout << "GrapplingHook hit: roll damage, stun 2000 ms, pull 250/s, arrival within melee reach\n";

        // ---- a target that is no longer Stunned at the hit (or refused) is not pulled ----------------------------------------
        // The stun request is made but the target does not take it (the ES-STATUS refusal path): no pull.
        rec.stun_takes = false;
        check(owner.use(session, context, e), "second GrapplingHook Use failed: " + e);
        const auto knife2 = executor.visuals().back().projectile_id;
        session.actor(3)->transform.position = {origin[0] + 350.0f * look[0], origin[1] + 350.0f * look[1], origin[2]};
        const auto before3 = session.actor(3)->transform.position;
        hit(knife2, 3);
        check(rec.stuns.size() == 2, "second hit did not request the stun");
        check(owner.active_pulls() == 0, "GrapplingHook pulled a target that is not Stunned");
        check(session.actor(3)->transform.position == before3, "non-stunned target moved");
        rec.stun_takes = true;
        std::cout << "GrapplingHook second hit: stun requested, target not Stunned, no pull\n";

        // ---- boss / miniboss: roll only, no stun, no pull ----------------------------------------------------------------------
        rec.boss[2] = true;
        const auto stuns_before = rec.stuns.size();
        check(owner.use(session, context, e), "third GrapplingHook Use failed: " + e);
        const auto knife3 = executor.visuals().back().projectile_id;
        const float boss_health = session.actor(2)->health;
        const auto boss_pos = session.actor(2)->transform.position;
        hit(knife3, 2);
        check(has_line(rec.trace, "GrapplingHook hit victim=2 damage="), "boss target must still take the GrapplingHook roll (the roll is traced)");
        check(session.actor(2)->health <= boss_health, "boss roll changed the health upward");
        check(rec.stuns.size() == stuns_before, "boss target must not be stunned by GrapplingHook");
        check(owner.active_pulls() == 0, "boss target must not be pulled by GrapplingHook");
        check(session.actor(2)->transform.position == boss_pos, "boss target moved");
        rec.boss[2] = false;
        std::cout << "GrapplingHook boss: roll only, no stun, no pull\n";

        std::cout << "runtime_rogue_skill_owner_v1 passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "runtime_rogue_skill_owner_v1 failed: " << ex.what() << '\n';
        return 1;
    }
}
