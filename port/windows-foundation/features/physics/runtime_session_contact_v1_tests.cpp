#include "runtime_session_contact_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../original_actor_body_plan.hpp"
#include "../../original_actor_collision_filter.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"
#include "../../original_actor_properties.hpp"
#include "../../playable_actor_bodies.hpp"

#include <array>
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::physics;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

struct NativeActorSlots {
    std::array<float, 3> destination{};
    std::uintptr_t attached{}, visual{};
};

struct SnapshotOwner {
    std::uint32_t frame = 1;
    std::uint32_t dt = 16;
    std::uint32_t movement = 4;
    std::uint32_t paused{};
    ActorId preferred = invalid_actor_id;
    std::array<std::uintptr_t, 4> methods{0x3dbf08u, 0x3dbfa0u, 0x3dbf40u, 0x3dbf68u};
};

} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply original shared asset root");
        AssetCatalog assets(argv[1]);
        std::string error;
        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
        check(melee.load(assets, "original-melee-bindings.xml", error), error);

        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan playerVisualPlan, enemyVisualPlan;
        check(build_original_combat_visual_plan(assets, melee, "KnightPlayerBase", customization,
              "session-contact-player", playerVisualPlan, error), error);
        check(build_original_combat_visual_plan(assets, melee, "Swamp_LizadMan_Type1", customization,
              "session-contact-enemy", enemyVisualPlan, error), error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 134;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = playerVisualPlan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile player;
        player.initialIdle = {"Idle", 0, {0}};
        player.damageMarkerNames = {"attack_mainhand"};
        OriginalAttackSelection playerAction;
        playerAction.state = "AttackStatic";
        player.sequenceAction = playerAction;
        player.retainedPhaseClock = true;
        player.sourceCombo = true;
        player.propertyOptions = {256, true};
        config.profiles.emplace("KnightPlayerBase", player);
        CombatSessionProfile enemy;
        enemy.action = {"Attack", 0, {0, 1}};
        enemy.initialIdle = {"Idle", 0, {0}};
        enemy.damageMarkerNames = {"attack_mainhand"};
        enemy.customization = customization;
        enemy.propertyOptions = {256, true};
        config.profiles.emplace("Swamp_LizadMan_Type1", enemy);
        ActorPopulation population;
        for (ActorId id : {ActorId{2}, ActorId{3}}) {
            PopulationActor placed;
            placed.profileId = "Swamp_LizadMan_Type1";
            placed.definition.stableId = id;
            placed.definition.sourceId = "session-contact-enemy-" + std::to_string(id);
            placed.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
            placed.transform = placed.definition.placement;
            population.actors().push_back(std::move(placed));
        }
        CharacterVisual visual;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, visual, population,
              {0,0,0}, customization, error), "Initial contact Session init: " + error);
        check(session.update(0, {}, {0,0,0}, 0, error), error);
        check(session.actor(1) && session.actor(2) && session.actor(3),
              "Contact fixture did not use three current CombatSession actors");

        // The live game FSM/AIS/controller and Application clock are explicit
        // caller services: their real native owner graph is not manufactured by
        // CombatSession. These test witnesses keep all combat effects on this
        // exact Session and exercise the recovered gates deterministically.
        std::map<ActorId, SnapshotOwner> snapshots;
        for (ActorId id : {ActorId{1}, ActorId{2}, ActorId{3}}) snapshots[id] = {};
        std::vector<std::tuple<ActorId, ActorId, std::uint32_t>> setTargetCalls;
        unsigned cancelSneakingCalls = 0;
        RuntimeSessionContactOwnerV1* contactOwnerBorrow = nullptr;
        bool reenteredCancel = false;
        bool enableReentrantCancel = false;
        RuntimeSessionContactServicesV1 contactServices;
        contactServices.snapshot = [&](ActorId id, RuntimeSessionContactSnapshotV1& out,
                                       std::string&) {
            const auto found = snapshots.find(id);
            if (found == snapshots.end()) return false;
            const auto token = static_cast<std::uintptr_t>(0x10000u + id * 0x100u);
            out.active_ais = token + 1;
            out.character = token + 2;
            out.char_ai = token + 3;
            out.controller = token + 4;
            out.controller_owner = id;
            out.preferred_target = found->second.preferred;
            out.movement_state = found->second.movement;
            out.paused = found->second.paused;
            out.application_frame = found->second.frame;
            out.dt_ms = found->second.dt;
            out.collision_methods = found->second.methods;
            return true;
        };
        contactServices.debug_load = [](std::string&) { return true; };
        contactServices.debug_switch = [](const char* key, bool& out, std::string& errorText) {
            if (std::string(key) != "isTracingPlayersCollision") {
                errorText = "Unexpected source collision debug key";
                return false;
            }
            out = false;
            return true;
        };
        contactServices.cancel_sneaking = [&](ActorId id, std::string&) {
            check(session.actor(id) != nullptr, "CancelSneaking lost its same Session owner");
            ++cancelSneakingCalls;
            if (enableReentrantCancel && !reenteredCancel) {
                reenteredCancel = true;
                std::string nestedError;
                check(contactOwnerBorrow && contactOwnerBorrow->physical_event(
                    id, RuntimeSessionContactEventV1::persist, 2, 1, nestedError), nestedError);
            }
            return true;
        };
        contactServices.set_target = [&](ActorId id, ActorId target, std::uint32_t flags,
                                         std::string& errorText) {
            if (flags != 0 || !session.actor(id) || !session.actor(target)) {
                errorText = "SetTarget fixture rejected source identity/flags";
                return false;
            }
            setTargetCalls.emplace_back(id, target, flags);
            session.actor(id)->target_id = target;
            return true;
        };
        RuntimeSessionContactOwnerV1 contactOwner(session, std::move(contactServices));
        contactOwnerBorrow = &contactOwner;
        check(contactOwner.bind_body(1, error), error);
        check(contactOwner.bind_body(2, error), error);

        // Actual native bodies use the real current Session property sheets,
        // resolved AI rows, authored model bounds and NativeWorld overlap. The
        // focused fixture leaves the collision-filter answer and PF update as
        // explicit services; it does not claim a production Level Step owner.
        auto nativeWorld = std::make_shared<dh2::physical::NativeWorld>();
        const float worldBounds[]{-100.f, -100.f, 100.f, 100.f};
        nativeWorld->load(worldBounds);
        PlayableActorBodies bodies;
        std::map<ActorId, NativeActorSlots> slots;
        std::map<void*, ActorId> physicalOwners;
        std::vector<std::tuple<ActorId, RuntimeSessionContactEventV1, ActorId, std::uint32_t>> delivered;

        for (ActorId id : {ActorId{1}, ActorId{2}}) {
            auto* actor = session.actor(id);
            actor->transform.position = {0.f, 0.f, 0.f};
            const auto* properties = session.world()->combat_properties(id);
            check(properties != nullptr, "Same Session actor lost original properties");
            OriginalActorBodyPlanInput input;
            input.properties = &properties->sheets;
            input.ai = session.original_ai_tables();
            input.position = {0,0,0};
            input.source_name = id == 1 ? "KnightPlayerBase" : "Swamp_LizadMan_Type1";
            input.owner_identity = id;
            input.visual = id == 1 ? playerVisualPlan.config : enemyVisualPlan.config;
            input.visual.use_authored_modular_defaults = true;
            OriginalActorBodyPlan bodyPlan;
            check(make_original_actor_body_plan(assets, input, bodyPlan, error), error);
            check(bodyPlan.physical_enabled, "Source actor body plan is not physically enabled");
            auto lease = std::const_pointer_cast<void>(session.actor_binding_lease().lock());
            check(lease != nullptr, "CombatSession binding lease is absent");
            OriginalActorPhysicalBindings physical;
            physical.actor_lease = lease;
            physical.world_lease = nativeWorld;
            physical.data_lease = lease;
            physical.readonly_properties = &properties->sheets;
            physical.destination1a8 = slots[id].destination.data();
            physical.attached2e0 = &slots[id].attached;
            physical.visual2d8 = &slots[id].visual;
            physical.world = nativeWorld.get();
            physical.ai = session.original_ai_tables();
            physical.static84 = [](std::uint8_t& out, std::string&) { out = 0; return true; };
            physical.is_player = [name=input.source_name](std::int32_t type, bool& out, std::string&) {
                out = original_actor_source_is_player(type, name);
                return true;
            };
            physical.debug_switch = [](const char*, bool& out, std::string&) { out = false; return true; };
            physical.update_pf = [](std::uintptr_t, const auto&, const auto&, std::string&) { return true; };
            physical.filter = [](void*, const auto&, const auto&, bool& allowed, std::string&) {
                allowed = true;
                return true;
            };
            physical.contact = [&, id](dh2::physical::ContactEvent rawEvent, void* peer,
                                       unsigned side, std::string& problem) {
                const auto found = physicalOwners.find(peer);
                if (found == physicalOwners.end()) {
                    problem = "NativeWorld peer was not resolved by the registered body map";
                    return false;
                }
                const auto event = rawEvent == dh2::physical::ContactEvent::add ? RuntimeSessionContactEventV1::begin :
                    rawEvent == dh2::physical::ContactEvent::persist ? RuntimeSessionContactEventV1::persist :
                    rawEvent == dh2::physical::ContactEvent::remove ? RuntimeSessionContactEventV1::end :
                    RuntimeSessionContactEventV1::result;
                delivered.emplace_back(id, event, found->second, side);
                return contactOwner.physical_event(id, event, found->second, side, problem);
            };
            check(bodies.bind(*actor, *properties, bodyPlan, std::move(physical), error), error);
            physicalOwners.emplace(const_cast<OriginalActorPhysical*>(bodies.physical(id)), id);
        }
        check(bodies.set_pinned(1, false, error) && bodies.set_pinned(2, false, error), error);
        check(bodies.physical(1)->native().body->GetMass() > 0.f,
              "Source Unpin did not restore physical mass for dynamic contact fixture");
        check(bodies.set_pinned(1, true, error), error);
        check(bodies.physical(1)->native().body->GetMass() == 0.f,
              "Source Pin did not set zero physical mass");
        check(bodies.set_pinned(1, false, error), error);

        // The actual authored AI rows resolve NPC→player as hostile. Begin is
        // expected to call the same-Session source SetTarget provider and to
        // increment the single world counter once per listening Actor/AIS.
        const auto* ownerFacts = session.world()->combat_properties(2);
        const auto* peerFacts = session.world()->combat_properties(1);
        check(dh2::data::ai_enemy(*session.original_ai_tables(), ownerFacts->sheets.resolved[0],
              peerFacts->sheets.resolved[0], session.world()->traits(2)->is_player,
              session.world()->traits(1)->is_player),
              "Fixture source AI faction rows do not classify the NPC/player pair as enemies");
        session.actor(2)->target_id = invalid_actor_id;
        snapshots[1].frame = snapshots[2].frame = 1;
        nativeWorld->update(16);
        check(std::any_of(delivered.begin(), delivered.end(), [](const auto& event) {
            return std::get<1>(event) == RuntimeSessionContactEventV1::begin;
        }), "Native two-body overlap did not deliver an actual Add/Begin event");
        check(contactOwner.world_collision_count() == 2,
              "AISDefault Begin did not balance one world count for each live body owner");
        check(!setTargetCalls.empty() && std::get<0>(setTargetCalls.front()) == 2 &&
              std::get<1>(setTargetCalls.front()) == 1 && session.actor(2)->target_id == 1,
              "Source Begin did not deliver its accepted AI_SetTarget through same ActorState");

        // Re-run persisted source gates directly on the same registered native
        // pair: one dt addition per actual Application frame, pause/state skips,
        // and current-target early return.
        auto* ownerFields = const_cast<RuntimeSessionContactAISFieldsV1*>(contactOwner.ais_fields(2));
        check(ownerFields != nullptr, "Per-AIS source counter owner missing");
        session.actor(2)->target_id = 3;
        snapshots[2].preferred = 3;
        snapshots[2].movement = 4;
        snapshots[2].paused = 0;
        snapshots[2].frame = 10;
        check(contactOwner.physical_event(2, RuntimeSessionContactEventV1::persist, 1, 1, error), error);
        check(ownerFields->collision_ms == 16 && ownerFields->last_collision_frame == 10,
              "Original moving Persist did not add source dt/stamp");
        check(contactOwner.physical_event(2, RuntimeSessionContactEventV1::persist, 1, 1, error), error);
        check(ownerFields->collision_ms == 16, "Same source Application frame accumulated twice");
        snapshots[2].frame = 11;
        snapshots[2].paused = 1;
        check(contactOwner.physical_event(2, RuntimeSessionContactEventV1::persist, 1, 1, error), error);
        check(ownerFields->collision_ms == 16, "Paused AIS collision tail accumulated source dt");
        snapshots[2].paused = 0;
        snapshots[2].movement = 3;
        snapshots[2].frame = 12;
        check(contactOwner.physical_event(2, RuntimeSessionContactEventV1::persist, 1, 1, error), error);
        check(ownerFields->collision_ms == 16, "Nonmoving FSM reached collision tail");
        snapshots[2].movement = 4;
        snapshots[2].frame = 13;
        check(contactOwner.physical_event(2, RuntimeSessionContactEventV1::persist, 1, 1, error), error);
        check(ownerFields->collision_ms == 32 && ownerFields->last_collision_frame == 13,
              "Moving unpaused later frame did not add exact source dt");
        session.actor(2)->target_id = 1;
        snapshots[2].frame = 14;
        check(contactOwner.physical_event(2, RuntimeSessionContactEventV1::persist, 1, 1, error), error);
        check(ownerFields->collision_ms == 32,
              "Collision with current source target did not return before counter tail");

        // Player→enemy uses the source player branch and reaches the explicit
        // CancelSneaking effect only after actual Session IsPlayer/AI-table checks.
        session.actor(1)->target_id = invalid_actor_id;
        snapshots[1].preferred = invalid_actor_id;
        snapshots[1].frame = 20;
        snapshots[1].movement = 4;
        snapshots[1].paused = 0;
        const auto cancelBefore = cancelSneakingCalls;
        enableReentrantCancel = true;
        const auto playerCollisionBefore = contactOwner.ais_fields(1)->collision_ms;
        check(contactOwner.physical_event(1, RuntimeSessionContactEventV1::persist, 2, 1, error), error);
        check(cancelSneakingCalls == cancelBefore + 2,
              "Source player/enemy Persist did not reach explicit CancelSneaking continuation");
        check(contactOwner.ais_fields(1)->collision_ms == playerCollisionBefore + 16,
              "Nested same-frame collision and resumed outer Persist did not preserve source one-frame accounting");

        // AISDefault OnUpdate consumes its canonical collision counter only
        // after the reached pause/timer/Stop continuation. Use the live same-
        // faction peer so collision does not retarget, and advance unique
        // source frames until the exact unsigned threshold is crossed.
        check(contactOwner.bind_body(3, error), error);
        session.actor(2)->target_id = invalid_actor_id;
        snapshots[2].preferred = invalid_actor_id;
        snapshots[2].movement = 4;
        snapshots[2].paused = 0;
        snapshots[2].dt = 16;
        snapshots[2].frame = 30;
        while (contactOwner.ais_fields(2)->collision_ms <= 199u) {
            ++snapshots[2].frame;
            check(contactOwner.physical_event(2, RuntimeSessionContactEventV1::persist,
                                              3, 1, error), error);
        }
        const auto overThreshold = contactOwner.ais_fields(2)->collision_ms;
        bool reachedPausePrefix = false, triggeredThreshold = false;
        check(!contactOwner.process_collision_pause_threshold(2,
              [&](ActorId id, std::string& problem) {
                  check(id == 2, "Pause continuation changed canonical actor identity");
                  reachedPausePrefix = true;
                  problem = "fixture stop provider failure after pause/timer prefix";
                  return false;
              }, triggeredThreshold, error) && reachedPausePrefix && !triggeredThreshold &&
              contactOwner.ais_fields(2)->collision_ms == overThreshold,
              "Failed pause/timer/Stop prefix incorrectly reset the source collision counter");
        check(contactOwner.process_collision_pause_threshold(2,
              [&](ActorId id, std::string&) { return id == 2; },
              triggeredThreshold, error), error);
        check(triggeredThreshold && contactOwner.ais_fields(2)->collision_ms == 0,
              "Successful pause/timer/Stop continuation did not reset canonical collision_ms at source tail");
        check(contactOwner.unbind_body(3, error), error);

        // Removing the bodies from overlap generates real End callbacks. Each
        // listening source AIS decrements once; no per-pair dedup is applied.
        check(bodies.set_position(2, {5000.f, 0.f, 0.f}, false, error), error);
        snapshots[1].frame = snapshots[2].frame = 21;
        nativeWorld->update(16);
        check(std::any_of(delivered.begin(), delivered.end(), [](const auto& event) {
            return std::get<1>(event) == RuntimeSessionContactEventV1::end;
        }), "Separating same-session body pair did not deliver native Remove/End");
        check(contactOwner.world_collision_count() == 0,
              "AISDefault End did not balance the world collision count");

        // Unknown active AIS override rejects; stale leases and unregistered
        // peer IDs cannot route through another actor or shadow target state.
        const auto sourceSnapshot = contactOwner.ais_fields(2);
        check(sourceSnapshot != nullptr, "AIS owner disappeared before guard checks");
        check(!contactOwner.physical_event(2, RuntimeSessionContactEventV1::persist, 99, 1, error),
              "Unregistered physical peer was accepted");
        check(!contactOwner.physical_event(2, RuntimeSessionContactEventV1::begin, 3, 1, error),
              "Registered Session actor without a physical body was accepted as peer");
        check(contactOwner.physical_event(1, RuntimeSessionContactEventV1::result, 0, 0, error), error);

        snapshots[2].methods[1] = 0xdeadbeefu;
        check(!contactOwner.physical_event(2, RuntimeSessionContactEventV1::persist, 1, 1, error) &&
              error.find("Unsupported source AIS OnCollisionPersist override") != std::string::npos,
              "Unknown active AIS collision override was silently treated as AISDefault");
        snapshots[2].methods[1] = 0x3dbfa0u;

        check(bodies.clear(error), error);
        nativeWorld->clear();
        check(contactOwner.unbind_body(1, error) && contactOwner.unbind_body(2, error), error);
        check(contactOwner.world_collision_count() == 0, "Contact owner leaked default world collision count");
        session.detach_for_restore();
        check(!contactOwner.physical_event(1, RuntimeSessionContactEventV1::result, 0, 0, error),
              "Stale detached CombatSession lease accepted a contact callback");

        // Keep both generations pinned after destroying a separate live
        // CombatSession. Contact owners must consult the independent lifetime
        // witness before touching their stored raw Session reference.
        auto doomedSession = std::make_unique<CombatSession>();
        CharacterVisual doomedVisual;
        check(doomedSession->initialize(assets, database, melee, config, doomedVisual,
              population, {0,0,0}, customization, error), "Destroyed-owner regression Session init: " + error);
        RuntimeSessionContactServicesV1 doomedServices;
        doomedServices.snapshot = [](ActorId id, RuntimeSessionContactSnapshotV1& out,
                                     std::string&) {
            const auto token = static_cast<std::uintptr_t>(0x20000u + id * 0x100u);
            out.active_ais = token + 1;
            out.character = token + 2;
            out.char_ai = token + 3;
            out.controller = token + 4;
            out.controller_owner = id;
            out.movement_state = 4;
            out.collision_methods = {0x3dbf08u,0x3dbfa0u,0x3dbf40u,0x3dbf68u};
            return true;
        };
        RuntimeSessionContactOwnerV1 doomedOwner(*doomedSession, std::move(doomedServices));
        check(doomedOwner.bind_body(1, error), error);
        auto pinnedActorBinding = doomedSession->actor_binding_lease().lock();
        auto pinnedLifetime = doomedSession->lifetime_lease().lock();
        check(pinnedActorBinding && pinnedLifetime && pinnedLifetime->alive(),
              "Lifetime regression did not pin both independent Session tokens");
        doomedSession.reset();
        check(!pinnedLifetime->alive() && !doomedOwner.owns_binding(),
              "Destroyed Session remained live through a pinned actor/lifetime token");
        check(!doomedOwner.physical_event(1, RuntimeSessionContactEventV1::result, 0, 0, error) &&
              error.find("stale/detached") != std::string::npos,
              "Contact owner dereferenced or accepted a destroyed CombatSession");

        std::cout << "PASS same-CombatSession two-body NativeWorld contact, source target service, inherited AIS counters/gates, and typed peer/lifetime guards\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return 1;
    }
}




