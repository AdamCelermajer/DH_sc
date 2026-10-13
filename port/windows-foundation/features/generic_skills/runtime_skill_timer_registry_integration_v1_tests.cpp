// v21.8 es-runtime: integration coverage for the general skill runtime (ES-RUNTIME integration test).
//
// A FireSkullRain-style skill (prince_necromancer_fireskullrain.luac: OnSkill_ starts StartSkillTimerCB(1000, true,
// DamageTick); DamageTick searches the Enemy list within RANGE 280 and SkillCombatRolls every target; an empty list
// stops the timer; after MAX_TICKS (2) further ticks the timer is stopped, so 3 rolling ticks) runs through the owner
// registry with a NON-Mage owner (FireSkullRainTestOwnerV1 below). The registry also holds the real Mage owner, which must
// keep its own skills. Real CombatSession with two lizards in front of the caster (same assets as mage_skill_owner_v1).
//
// Covered: dispatch by handles() to the non-Mage owner; the loop starts only at Use, ticks at 1000/2000/3000 ms, stops
// after the third tick (no fourth tick at 4000-8000 ms); the registry's advance() drives the owner's clock; reset()
// cancels a running loop; an empty target list stops the loop at its first tick.
// Not covered (real runs only): the skill animation, FX and sound of FireSkullRain.
#include "runtime_mage_skill_owner_v1.hpp"
#include "runtime_skill_external_owner_registry_v1.hpp"
#include "runtime_skill_external_owner_v1.hpp"
#include "runtime_skill_target_query_v1.hpp"
#include "runtime_skill_timer_service_v1.hpp"
#include "runtime_skill_use_profile_registry_v1.hpp"
#include "../../combat_session.hpp"
#include "../../content_paths.hpp"
#include "../combat/runtime_player_combo_chain_v1.hpp"
#include "../../../script-runtime/script_constants.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool value, const std::string& message) { if (!value) throw std::runtime_error(message); }

constexpr float kPi = 3.1415927410125732421875f;
constexpr double kTickMs = 1000.0;  // TICK_DELAY
constexpr unsigned kMaxTicks = 2;   // MAX_TICKS
constexpr float kRange = 280.0f;    // RANGE

// Non-Mage external owner: the authored FireSkullRain body on the registry seam.
class FireSkullRainTestOwnerV1 final : public RuntimeSkillExternalOwnerV1 {
public:
    bool handles(const RuntimeSkillUseProfileV1& profile) const override {
        return std::string(profile.skill_name) == "FireSkullRain";
    }
    bool pre(CombatSession&, const RuntimeSkillExternalContextV1&, std::string&) override { return true; }
    // OnSkill_: if timer_id == nil then timer_id = StartSkillTimerCB(TICK_DELAY, true, DamageTick).
    bool use(CombatSession&, const RuntimeSkillExternalContextV1& context, std::string& error) override {
        if (timer_ != 0) return true;
        if (!context.formula_sheet || !context.population_order || !context.target_facts)
            return fail(error, "FireSkullRain Use needs its formula sheet and population facts");
        caster_ = context.caster;
        generation_ = context.generation;
        row_mask_ = context.row_mask;
        element_ = context.element;
        category_ = context.category;
        sheet_ = *context.formula_sheet;
        order_ = *context.population_order;
        facts_ = *context.target_facts;
        objects_absent_ = context.non_character_attackable_objects_absent;
        ticks_ = 0;
        timer_ = timers_.start(kTickMs, true, [this](RuntimeSkillTimerIdV1, std::string& timer_error) {
            if (!session_) return fail(timer_error, "FireSkullRain tick fired outside the owner clock");
            return tick(*session_, timer_error);
        });
        return timer_ != 0 || fail(error, "FireSkullRain timer could not be started");
    }
    // The host's clock. The registry forwards advance() here.
    bool advance(CombatSession& session, double dt_ms, std::string& error) override {
        error.clear();
        session_ = &session;
        std::string timer_error;
        const bool ok = timers_.advance(dt_ms, timer_error);
        session_ = nullptr;
        if (!ok) {
            error = timer_error;
            return false;
        }
        return true;
    }
    void reset() override {
        timers_.clear();
        timer_ = 0;
        ticks_ = 0;
    }

    std::size_t active_timers() const { return timers_.size(); }
    unsigned rolls = 0;

private:
    static bool fail(std::string& error, const std::string& message) {
        error = message;
        return false;
    }
    // DamageTick.
    bool tick(CombatSession& session, std::string& error) {
        SkillActorTargetQueryV1 found;
        if (!query_source_character_targets_v1(session, caster_, order_, facts_, objects_absent_, kRange, 2.0f * kPi,
                SkillTargetSortV1::source_order, found, error)) return false;
        if (found.ordered_character_targets.empty()) {
            stop();  // IsTargetListEmpty -> StopTimerCB, tick_count = 0
            return true;
        }
        const auto* world = session.world();
        const auto* attacker = session.actor(caster_);
        if (!world || !attacker) return fail(error, "FireSkullRain caster or world is absent");
        for (const auto target : found.ordered_character_targets) {
            const auto* victim = session.actor(target);
            if (!victim || target == caster_ || !world->eligible_target(*attacker, *victim)) continue;
            CombatSessionSourceHit hit;
            hit.attacker = caster_;
            hit.target = target;
            hit.binding_lease = session.actor_binding_lease();
            hit.generation = generation_;
            hit.event_index = next_event_++;
            hit.mask = row_mask_ | 0x08000000u;
            hit.source_id = "FireSkullRain";
            hit.marker_name = "do_skill";
            hit.category = category_;
            hit.element = element_;
            hit.direct_amount = 0;
            hit.attacker_formula_sheet = &sheet_;
            DamageEvent result;
            if (!session.apply_source_result(hit, result, error)) return false;
            ++rolls;
        }
        if (ticks_ >= kMaxTicks) {  // if tick_count >= MAX_TICKS -> StopTimerCB
            stop();
            return true;
        }
        ++ticks_;
        return true;
    }
    void stop() {
        timers_.stop(timer_);
        timer_ = 0;
        ticks_ = 0;
    }

    RuntimeSkillTimerServiceV1 timers_;
    CombatSession* session_ = nullptr;
    RuntimeSkillTimerIdV1 timer_ = 0;
    unsigned ticks_ = 0;
    std::uint64_t next_event_ = 0;
    ActorId caster_ = invalid_actor_id;
    std::uint64_t generation_ = 0;
    std::uint32_t row_mask_ = 0;
    std::int32_t element_ = -1, category_ = -1;
    dh2::data::PropertySheet sheet_{};
    std::vector<ActorId> order_;
    std::vector<SkillTargetSourceFactsV1> facts_;
    std::optional<bool> objects_absent_;
};
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
        check(build_original_combat_visual_plan(assets, bindings, "MagePlayerBase", customization, "ranged-base", base, error), error);
        equipment_menu::RuntimePlayerLocomotionV1 equipped;
        const auto staff = dh2::data::item_id(items, "Staff01");
        check(staff >= 0, "Original Staff01 missing");
        check(equipment_menu::resolve_runtime_player_locomotion_v1(49, items, staff, -1, 0, stance, animations, clips, equipped, error), error);
        combat::RuntimePlayerComboChainPlanV1 attack;
        check(combat::plan_runtime_player_combo_chain_v1(assets, equipped, stance.stanced_list_mask, 3, true, animations, clips,
            "MagePlayerBase", base.config, "source-staff-bank", attack, error), error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 4321;
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
        for (const ActorId stable : {ActorId{2}, ActorId{3}}) {
            PopulationActor placed;
            placed.profileId = "Swamp_LizadMan_Type1";
            placed.definition.stableId = stable;
            placed.definition.sourceId = "fireskullrain-lizard-" + std::to_string(stable);
            placed.definition.placement = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
            placed.transform = placed.definition.placement;
            population.actors().push_back(std::move(placed));
        }
        check(session.initialize(assets, db, bindings, config, visual, population, {0, 0, 0}, customization, error), error);
        const float heading = session.actor(1)->transform.rotation[2];
        const std::array<float, 3> look{std::sin(heading), -std::cos(heading), 0.0f};
        const std::array<float, 3> origin = session.actor(1)->transform.position;
        // Both lizards are inside RANGE 280 in front of the caster; they are made tough enough to survive three ticks.
        for (const ActorId id : {ActorId{2}, ActorId{3}}) {
            auto* lizard = session.actor(id);
            check(lizard != nullptr, "Lizard actor is absent from the session");
            const float distance = id == 2 ? 100.0f : 160.0f;
            lizard->transform.position = {origin[0] + distance * look[0], origin[1] + distance * look[1], origin[2]};
            lizard->health = 1000.0f;
            lizard->max_health = 1000.0f;
        }

        // The registry holds the real Mage owner and the non-Mage FireSkullRain owner.
        RuntimeMageSkillOwnerV1 mage(RuntimeMageSkillServicesV1{});
        FireSkullRainTestOwnerV1 fire_owner;
        RuntimeSkillExternalOwnerRegistryV1 registry;
        check(registry.add(&mage) && registry.add(&fire_owner), "Registry was not built");
        const auto* fire_profile_base = find_skill_use_profile_registered_v1("ColdRay", "prince_mage_coldray");
        const auto* chain = find_skill_use_profile_registered_v1("ChainLightning", "prince_mage_chain_lightning");
        check(fire_profile_base && chain, "Profile rows are missing");
        // The FireSkullRain row is not in the shared table yet (phase 2b). A copy of an external-owner row stands in for it.
        RuntimeSkillUseProfileV1 fire_profile = *fire_profile_base;
        fire_profile.skill_name = "FireSkullRain";
        fire_profile.script = "prince_necromancer_fireskullrain";
        check(registry.owner_for(fire_profile) == &fire_owner, "FireSkullRain did not dispatch to the non-Mage owner");
        check(registry.owner_for(*chain) == &mage, "ChainLightning did not stay on the Mage owner");
        std::cout << "dispatch: FireSkullRain -> non-Mage owner, ChainLightning -> Mage owner\n";

        std::vector<ActorId> order{1, 2, 3};
        std::vector<SkillTargetSourceFactsV1> facts{{1, true, false, false, true, true}, {2, true, false, false, true, true},
                                                    {3, true, false, false, true, true}};
        dh2::data::PropertySheet sheet = session.world()->combat_properties(1)->sheets.resolved;
        const auto column = [&](const char* name) {
            const auto found = std::find(db.characters.fields.begin(), db.characters.fields.end(), name);
            check(found != db.characters.fields.end(), std::string("CharacterProperties field missing: ") + name);
            return static_cast<std::size_t>(found - db.characters.fields.begin());
        };
        sheet[column("SnS_MinDamage")] = 5 * 256;
        sheet[column("SnS_MaxDamage")] = 5 * 256;
        RuntimeSkillExternalContextV1 context;
        context.caster = 1;
        context.generation = 1;
        context.profile = &fire_profile;
        context.skill_name = "FireSkullRain";
        context.row_mask = 0x20000u;
        context.category = 0;
        context.element = 0;
        context.formula_sheet = &sheet;
        context.population_order = &order;
        context.target_facts = &facts;
        context.non_character_attackable_objects_absent = true;

        const auto health = [&](ActorId id) { return session.actor(id)->health; };
        const float start2 = health(2), start3 = health(3);
        const auto step = [&](double dt_ms, const char* label) {
            std::string e;
            check(registry.advance(session, dt_ms, e), std::string(label) + " advance failed: " + e);
        };

        // Use starts the loop; nothing rolls before the first 1000 ms.
        check(registry.use(session, context, error), "FireSkullRain Use failed: " + error);
        check(fire_owner.active_timers() == 1 && fire_owner.rolls == 0, "Use did not start exactly one tick loop");
        step(999.0, "t=999");
        check(fire_owner.rolls == 0, "A tick ran before its 1000 ms interval");
        step(1.0, "t=1000");
        check(fire_owner.rolls == 2, "Tick 1 did not roll both lizards");
        step(1000.0, "t=2000");
        check(fire_owner.rolls == 4, "Tick 2 did not roll both lizards");
        step(1000.0, "t=3000");
        check(fire_owner.rolls == 6, "Tick 3 did not roll both lizards");
        check(fire_owner.active_timers() == 0, "The loop did not stop after its third tick");
        step(5000.0, "t=8000");
        check(fire_owner.rolls == 6, "The loop ticked after MAX_TICKS");
        check(health(2) < start2 && health(3) < start3, "The rolls did not damage the lizards");
        std::cout << "loop: rolls at 1000/2000/3000 ms (2 lizards each), stopped; rolls=" << fire_owner.rolls
                  << " lizard2=" << start2 << "->" << health(2) << " lizard3=" << start3 << "->" << health(3) << "\n";

        // reset() cancels a running loop through the registry.
        check(registry.use(session, context, error), "Second FireSkullRain Use failed: " + error);
        step(500.0, "second cast t=500");
        registry.reset();
        check(fire_owner.active_timers() == 0, "Registry reset did not cancel the running loop");
        const unsigned rolls_after_reset = fire_owner.rolls;
        step(5000.0, "second cast after reset");
        check(fire_owner.rolls == rolls_after_reset, "A cancelled loop still ticked");
        std::cout << "reset: a running loop is cancelled by the registry reset (no tick after it)\n";

        // An empty target list stops the loop at its first tick (IsTargetListEmpty branch).
        for (const ActorId id : {ActorId{2}, ActorId{3}}) {
            auto* lizard = session.actor(id);
            lizard->transform.position = {origin[0] + 5000.0f, origin[1] + 5000.0f, origin[2]};
        }
        check(registry.use(session, context, error), "Third FireSkullRain Use failed: " + error);
        step(1000.0, "empty list t=1000");
        check(fire_owner.active_timers() == 0 && fire_owner.rolls == rolls_after_reset,
              "An empty target list did not stop the loop at its first tick");
        std::cout << "empty list: the loop stops at its first tick without a roll\n";

        std::cout << "skill_timer_registry_integration_v1 passed\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "skill_timer_registry_integration_v1 failed: " << ex.what() << '\n';
        return 1;
    }
}
