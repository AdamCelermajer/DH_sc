#include "../asset_catalog.hpp"
#include "../content_paths.hpp"
#include "../combat_session.hpp"
#include "../original_actor_body_plan.hpp"
#include "../original_actor_navigation.hpp"
#include "../original_actor_properties.hpp"
#include "../original_combat_visual_plan.hpp"
#include "../original_melee_bindings.hpp"
#include "../playable_actor_bodies.hpp"
#include "../source_module_floors.hpp"
#include "../game_save.hpp"
#include "../features/combat/runtime_player_combo_chain_v1.hpp"
#include "../features/combat/runtime_player_profile_attack_bank_v1.hpp"
#include "../features/equipment/runtime_player_locomotion_v1.hpp"
#include "../../script-runtime/script_constants.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <memory>
#include <map>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;
namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
}
struct NativeSlots {
    std::array<float, 3> destination{};
    std::uintptr_t attached{}, visual{};
};
struct SourceObstacles {
    std::array<dh2::navigation::ObstacleEntry, 8> entries{};
    std::array<unsigned, 8> floors{};
    dh2::navigation::ObstacleRegistry registry{entries.data(), 0, 8, floors.data(), 0, 8};
};
struct FixtureActor {
    ActorId id{};
    std::string profile;
    std::string sourceId;
    OriginalCombatVisualPlan plan;
};
Vec3 position_of(const ActorState& actor) {
    return {actor.transform.position[0], actor.transform.position[1], actor.transform.position[2]};
}
}

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
        std::vector<FixtureActor> fixture;
        for (const auto& entry : std::array<std::pair<ActorId, const char*>, 2>{{
                 {1, "RoguePlayerBase"}, {2, "Swamp_LizadMan_Type1"}}}) {
            FixtureActor actor;
            actor.id = entry.first;
            actor.profile = entry.second;
            actor.sourceId = "motion-phase-" + std::to_string(actor.id);
            check(build_original_combat_visual_plan(assets, melee, actor.profile, customization,
                  actor.sourceId, actor.plan, error), error);
            actor.plan.config.motion_node_id = "auto";
            actor.plan.config.consume_root_motion = true;
            fixture.push_back(std::move(actor));
        }
        const auto read = [&](const char* name) {
            return read_content(assets, std::string("data/pydata/") + name);
        };
        const auto bytes = [](const auto& buffer) {
            return dh2::data::Bytes{buffer.data(), buffer.size()};
        };
        const auto clipNames = read("animations_dictionary_pyarraynames.bin");
        const auto clipValues = read("animations_dictionary_pyarray.bin");
        dh2::data::Dictionary clips;
        check(dh2::data::load_dictionary(bytes(clipNames), bytes(clipValues), clips, error), error);
        const auto animationRecords = read("animations_pyarray.bin");
        const auto animationNames = read("animations_pyarraynames.bin");
        const auto animationFields = read("animations_pystructnames.bin");
        dh2::data::AnimationTables animations;
        check(dh2::data::load_animation_tables(bytes(animationRecords), bytes(animationNames),
              bytes(animationFields), clips, animations, error), error);
        const auto itemRecords = read("loot_table_pyarray.bin");
        const auto itemNames = read("loot_table_pyarraynames.bin");
        const auto itemFields = read("loot_table_pystructnames.bin");
        dh2::data::ItemTable items;
        check(dh2::data::load_items(bytes(itemRecords), bytes(itemNames), bytes(itemFields), items, error), error);
        const auto constantsBytes = read_content(assets, "data/animations_pycst.bin");
        auto* constants = dh2_script_constants_create();
        check(constants, "Source locomotion constants unavailable");
        dh2_script_constants_reload reload{};
        check(dh2_script_constants_load(constants, constantsBytes.data(),
              static_cast<std::uint32_t>(constantsBytes.size()), &reload) == 0,
              "Source locomotion constants failed");
        equipment_menu::RuntimePlayerLocomotionConstantsV1 stance;
        check(equipment_menu::load_runtime_player_locomotion_constants_v1(
            [&](const char* group, const char* key, std::int32_t& value, std::string& problem) {
                if (dh2_script_constants_get(constants, group, key, &value)) {
                    problem = "Missing source locomotion constant";
                    return false;
                }
                return true;
            }, stance, error), error);
        dh2_script_constants_destroy(constants);
        equipment_menu::RuntimePlayerLocomotionV1 equipped;
        const auto dagger = dh2::data::item_id(items, "Dagger01");
        check(dagger >= 0, "Original Dagger01 row missing");
        check(equipment_menu::resolve_runtime_player_locomotion_v1(50, items, dagger, dagger,
              0, stance, animations, clips, equipped, error), error);
        ActorProfileLibrary profiles;
        check(profiles.load(assets, "actor-profiles-v2.xml", error), error);
        const auto* rogueProfile = profiles.find("RoguePlayerBase");
        check(rogueProfile, "Original Rogue actor profile missing");
        combat::RuntimePlayerProfileAttackBankPlanV1 attackBank;
        check(combat::plan_runtime_player_profile_attack_bank_v1(assets, *rogueProfile, equipped,
              stance.stanced_list_mask, animations, clips, fixture[0].plan,
              "motion-phase-rogue-bank", attackBank, error), error);

        // Use the real Swamp floor mesh/PF pipeline so authored root admission
        // and body import share the same current actor and navigation owner.
        const auto floorBytes = assets.read("original-cache/data/3d/modules/swamp/swamp.bdae");
        dh2::resources::BresView floorView{};
        check(dh2_bres_open(&floorView, floorBytes.data(), floorBytes.size()) ==
              dh2::resources::BresError::ok, "Original Swamp floor BRES missing");
        dh2::scene::Scene floorScene;
        check(dh2::scene::load(floorView, floorScene, error), error);
        unsigned floorInstance = UINT32_MAX;
        for (unsigned i = 0; i < floorScene.instances.size(); ++i) {
            const auto node = floorScene.instances[i].node_index;
            if (node < floorScene.graph.size() &&
                floorScene.graph[node].name.find("floor") != std::string::npos) {
                floorInstance = i;
                break;
            }
        }
        check(floorInstance != UINT32_MAX, "Authored Swamp floor instance missing");
        auto floorWorld = std::make_shared<dh2::floors::World>();
        OriginalSourceFloorBinding floorBinding;
        floorBinding.instance = floorInstance;
        floorBinding.room = 0;
        floorBinding.mesh_local_quaternion = {0, 0, 0, 1};
        floorBinding.mesh_local_scale = {1, 1, 1};
        check(append_original_module_floors(floorView, floorScene, {floorBinding},
              *floorWorld, error), error);
        check(dh2::floors::build_graph(*floorWorld, error) &&
              dh2::floors::post_load(*floorWorld, error), error);
        std::array<float, 3> floorPoint{};
        bool foundFloor = false;
        for (const auto& triangle : floorWorld->records.front()->triangles) {
            for (unsigned k = 0; k < 3; ++k)
                floorPoint[k] = (triangle.points[0][k] + triangle.points[1][k] + triangle.points[2][k]) / 3.f;
            float height = floorPoint[2];
            if (dh2::floors::height(*floorWorld, floorPoint.data(), height)) {
                floorPoint[2] = height;
                foundFloor = true;
                break;
            }
        }
        check(foundFloor, "Original Swamp floor had no selectable point");

        CombatSessionConfig config;
        config.diagnosticRngSeed = 1234;
        config.playerId = 1;
        config.playerProfileId = fixture[0].profile;
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = fixture[0].plan.config;
        config.mainItemId = config.offItemId = "Dagger01";
        config.equippedItemIds = {"StartingSuitRogue", "StartingBootsRogue",
                                  "StartingGlovesRogue", "Dagger01", "Dagger01"};
        CombatSessionProfile player;
        player.initialIdle = {"Idle", 0, {0}};
        player.damageMarkerNames = {"attack_mainhand"};
        player.propertyOptions = {256, true};
        player.sequenceAction = attackBank.static_selection;
        player.retainedPhaseClock = true;
        player.sourceCombo = true;
        player.sourceAttackBank = attackBank.bank;
        player.sourceAttackPolicies = attackBank.sequence_policies;
        player.sourceAttackStateSelection = true;
        player.sourceMeleeHandMarkers = true;
        config.profiles.emplace(fixture[0].profile, player);
        CombatSessionProfile lizard;
        lizard.action = {"Attack", 0, {0, 1}};
        lizard.initialIdle = {"Idle", 0, {0}};
        lizard.damageMarkerNames = {"attack_mainhand"};
        // A source level-20 continuity defender preserves the full four-group
        // move bank; this is not an Act1 balance fixture.
        lizard.propertyOptions = {20 * 256, true};
        lizard.customization = customization;
        config.profiles.emplace(fixture[1].profile, lizard);
        ActorPopulation population;
        PopulationActor placed;
        placed.profileId = fixture[1].profile;
        placed.definition.stableId = 2;
        placed.definition.sourceId = fixture[1].sourceId;
        placed.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
        placed.transform = placed.definition.placement;
        population.actors().push_back(std::move(placed));
        CharacterVisual playerVisual;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, playerVisual, population,
              {floorPoint[0], floorPoint[1], floorPoint[2]}, customization, error), error);
        auto* playerActor = session.actor(1);
        auto* lizardActor = session.actor(2);
        check(playerActor && lizardActor && session.world(), "Same-Session source actors missing");
        lizardActor->transform.position = {floorPoint[0] + 100.f, floorPoint[1], floorPoint[2]};

        auto nativeWorld = std::make_shared<dh2::physical::NativeWorld>();
        const float bounds[]{-50000.f, -50000.f, 50000.f, 50000.f};
        nativeWorld->load(bounds);
        PlayableActorBodies bodies;
        std::map<ActorId, NativeSlots> slots;
        for (const auto& source : fixture) {
            auto* actor = session.actor(source.id);
            const auto* properties = session.world()->combat_properties(source.id);
            check(actor && properties, "Current Session actor/property sheet unavailable");
            OriginalActorBodyPlanInput bodyInput;
            bodyInput.properties = &properties->sheets;
            bodyInput.ai = session.original_ai_tables();
            bodyInput.position = {actor->transform.position[0], actor->transform.position[1], actor->transform.position[2]};
            bodyInput.source_name = source.profile;
            bodyInput.owner_identity = source.id;
            bodyInput.visual = source.plan.config;
            bodyInput.visual.use_authored_modular_defaults = true;
            OriginalActorBodyPlan bodyPlan;
            check(make_original_actor_body_plan(assets, bodyInput, bodyPlan, error), error);
            auto lease = std::const_pointer_cast<void>(session.actor_binding_lease().lock());
            check(bool(lease), "Current Session body lease unavailable");
            auto& slot = slots[source.id];
            OriginalActorPhysicalBindings physical;
            physical.actor_lease = lease;
            physical.world_lease = nativeWorld;
            physical.data_lease = lease;
            physical.readonly_properties = &properties->sheets;
            physical.position160 = actor->transform.position.data();
            physical.destination1a8 = slot.destination.data();
            physical.attached2e0 = &slot.attached;
            physical.visual2d8 = &slot.visual;
            physical.world = nativeWorld.get();
            physical.ai = session.original_ai_tables();
            physical.static84 = [](std::uint8_t& out, std::string&) { out = 0; return true; };
            physical.is_player = [name=source.profile](std::int32_t type, bool& out, std::string&) {
                out = original_actor_source_is_player(type, name); return true;
            };
            physical.debug_switch = [](const char*, bool& out, std::string&) { out = false; return true; };
            physical.filter = [](void*, const auto&, const auto&, bool& allowed, std::string&) { allowed = true; return true; };
            physical.contact = [](dh2::physical::ContactEvent, void*, unsigned, std::string&) { return true; };
            check(bodies.bind(*actor, *properties, bodyPlan, std::move(physical), error), error);
        }
        check(bodies.set_pinned(1, false, error) && bodies.set_pinned(2, false, error), error);
        PlayableActorBodies::CurrentActorLookup lookup = [&](ActorId id) { return session.actor(id); };
        auto obstaclesLease = std::make_shared<SourceObstacles>();
        check(bodies.initialize_navigation(1, {floorWorld, obstaclesLease, floorWorld.get(),
              &obstaclesLease->registry}, error), error);

        std::uint64_t lastSteppedFrame = 0;
        unsigned actualSteps = 0, phaseCalls = 0, emptyActorCalls = 0;
        unsigned importedPhysics = 0, appliedRoot = 0, nonzeroRootSamples = 0, disabledSamples = 0;
        unsigned sourceHitNotifications = 0;
        unsigned sourceHitResolutions = 0;
        bool hitWasAppliedBeforePhase = false;
        const auto rngCallsBeforeCombat = session.world()->random_state().calls;
        std::uint64_t hitNotificationFrame = 0;
        std::uint64_t hitResolutionFrame = 0;
        std::vector<std::pair<Vec3, bool>> playerSamples;
        double currentDt = 0.0;
        bool failOnLizard = false;
        bool injectReentryChecks = false;
        bool reentryChecksPassed = false;
        bool reachedFailure = false;
        std::vector<CombatSessionMotionSample> failedSamples;
        const auto lease = session.actor_binding_lease().lock();
        CombatSessionAnimationNotificationServices notifications;
        notifications.state_event = [&](ActorId id, std::uint32_t event,
                                        const RetainedAnimationEvent* marker, std::string&) {
            if (id == 1 && event == 0x28 && marker && marker->name == "attack_mainhand") {
                ++sourceHitNotifications;
                hitNotificationFrame = session.update_serial();
            }
            return true;
        };
        session.set_animation_notification_services(std::move(notifications));
        session.world()->set_resolution_observer([&](const PlayableCombatResolution& receipt, std::string&) {
            if (receipt.attacker == 1) {
                ++sourceHitResolutions;
                hitResolutionFrame = session.update_serial();
            }
            return true;
        });
        auto bindPhase = [&]() {
            return session.set_motion_phase_handler(
                [&](ActorState& actor, std::uint64_t frame, double dt,
                    const std::vector<CombatSessionMotionSample>& samples, std::string& problem) {
                    check(session.actor(actor.id) == &actor && session.actor_binding_lease().lock() == lease,
                          "Motion phase replaced same-Session actor/lease");
                    check(frame == session.update_serial(), "Motion phase frame serial is stale");
                    if (actor.id == 1) check(std::fabs(dt - currentDt) < 1e-12,
                                              "Motion phase lost actual frame dt");
                    ++phaseCalls;
                    if (actor.id == 1) {
                        if (sourceHitResolutions && hitResolutionFrame == frame) {
                            check(sourceHitResolutions > 0 &&
                                  session.world()->random_state().calls > rngCallsBeforeCombat,
                                  "Motion phase ran before the source hit/RNG prefix completed");
                            hitWasAppliedBeforePhase = true;
                        }
                        playerSamples.clear();
                        for (const auto& sample : samples) {
                            playerSamples.emplace_back(sample.authored_motion, sample.move_go);
                            if (!sample.move_go) ++disabledSamples;
                            if (sample.move_go && (sample.authored_motion.x != 0.f ||
                                sample.authored_motion.y != 0.f || sample.authored_motion.z != 0.f))
                                ++nonzeroRootSamples;
                        }
                        if (samples.empty()) ++emptyActorCalls;
                        if (failOnLizard && std::any_of(samples.begin(), samples.end(), [](const auto& sample) {
                                return sample.move_go && (sample.authored_motion.x != 0.f ||
                                    sample.authored_motion.y != 0.f || sample.authored_motion.z != 0.f);
                            })) failedSamples = samples;
                    } else if (samples.empty()) {
                        ++emptyActorCalls;
                    }
                    if (injectReentryChecks && actor.id == 1 && !samples.empty()) {
                        InputActions nested;
                        std::string nestedError;
                        const bool nestedUpdateRejected = !session.update(0, nested,
                            position_of(actor), actor.transform.rotation[2], nestedError);
                        const bool clearRejected = !session.clear_motion_phase_handler(nestedError);
                        const bool replacementRejected = !session.set_motion_phase_handler(
                            [](ActorState&, std::uint64_t, double,
                               const std::vector<CombatSessionMotionSample>&, std::string&) { return true; },
                            nestedError);
                        const bool checkpointRejected = !session.validate_lifecycle_checkpoint(nestedError);
                        bool replacementRejectedByInit = false;
                        auto invalidConfig = config;
                        CharacterVisual rejectedVisual;
                        ActorPopulation rejectedPopulation;
                        replacementRejectedByInit = !session.initialize(assets, database, melee,
                            invalidConfig, rejectedVisual, rejectedPopulation, {0,0,0}, customization,
                            nestedError);
                        bool detachRejected = false;
                        try { session.detach_for_restore(); }
                        catch (const std::logic_error&) { detachRejected = true; }
                        check(nestedUpdateRejected && clearRejected && replacementRejected &&
                              replacementRejectedByInit && checkpointRejected && detachRejected,
                              "Motion phase accepted reentry, mode change, Session replacement or detach");
                        injectReentryChecks = false;
                        reentryChecksPassed = true;
                    }
                    if (!lastSteppedFrame || frame != lastSteppedFrame) {
                        bool stepped = false;
                        const auto milliseconds = static_cast<std::uint32_t>(dt * 1000.0);
                        if (!bodies.step_world(*nativeWorld, frame, milliseconds, lookup, stepped, problem)) return false;
                        if (stepped) ++actualSteps;
                        lastSteppedFrame = frame;
                    }
                    PlayableActorBodies::PhysicsPositionResult physics;
                    if (!bodies.reconcile_physics_position(actor.id, lookup, actor.id == 1,
                                                           physics, problem)) return false;
                    if (actor.id == 1 && physics.xy_changed) {
                        ++importedPhysics; // Source accepts solver XY and skips this frame's authored root.
                    } else {
                        for (const auto& sample : samples) {
                            if (!sample.move_go) continue;
                            OriginalActorNavigationMoveResult movement;
                            const Vec3 current{actor.transform.position[0], actor.transform.position[1], actor.transform.position[2]};
                            if (!bodies.move_grounded(actor.id, current, sample.authored_motion, movement, problem)) return false;
                            actor.transform.position = {movement.position.x, movement.position.y, movement.position.z};
                            ++appliedRoot;
                        }
                    }
                    if (failOnLizard && actor.id == 2 && !failedSamples.empty()) {
                        failOnLizard = false;
                        reachedFailure = true;
                        problem = "intentional motion-phase failure after player prefix";
                        return false;
                    }
                    return true;
                }, error);
        };
        check(bindPhase(), error);
        const auto actorCount = session.world()->actors().size();
        check(session.update(0, {}, {floorPoint[0], floorPoint[1], floorPoint[2]}, 0, error), error);
        check(phaseCalls == actorCount && emptyActorCalls >= 1 && actualSteps == 1,
              "Zero-dt phase counts phase=" + std::to_string(phaseCalls) + " empty=" +
              std::to_string(emptyActorCalls) + " steps=" + std::to_string(actualSteps) +
              " actors=" + std::to_string(actorCount));

        // Handler changes, re-entry, replacement and detach are rejected during
        // the callback. This also verifies mode changes cannot drop a queued batch.
        InputActions selectTarget;
        selectTarget.targetSelect = true;
        check(session.update(0, selectTarget, position_of(*playerActor),
              playerActor->transform.rotation[2], error), error);
        check(playerActor->target_id == 2, "Nearest-first source target selection missed the fixture lizard");
        session.set_diagnostic_controller_admission_provider(
            [](ActorId id, DiagnosticControllerAdmissionFacts& facts, std::string&) {
                facts = {id, id, 0, 0, 0, 0}; return true;
            }, [](ActorId, bool& online, std::string&) { online = false; return true; });
        InputActions attack;
        attack.move2D.x = 1.f;
        currentDt = 1.0 / 60.0;
        check(session.update(currentDt, attack, position_of(*playerActor),
              playerActor->transform.rotation[2], error), error);
        attack.attack = true;
        currentDt = 0.0;
        check(session.update(0, attack, position_of(*playerActor),
              playerActor->transform.rotation[2], error), error);
        check(session.owns_pose(1), "Source combo did not acquire the retained pose; target=" +
              std::to_string(playerActor->target_id) + " inRange=" +
              std::to_string(session.world()->original_melee_in_range(1, 2)));
        attack.attack = false;
        bool sawSampleAndStep = false;
        bool sawFailureRecovery = false;
        injectReentryChecks = true;
        for (unsigned frame = 0; frame < 500 &&
             (!sourceHitResolutions || !sawSampleAndStep || !sawFailureRecovery); ++frame) {
            if (const auto* state = session.source_attack_state(1)) attack.attack = state->index < 3;
            currentDt = 1.0 / 60.0;
            const auto* body = bodies.physical(1);
            check(body && body->native().body, "Player source body missing");
            // Alternate a real solver displacement with a still body. The first
            // imports through the current floor/PF gate; the second admits real
            // retained root samples through that same source floor owner.
            if ((frame & 1u) == 0) body->native().body->SetLinearVelocity(b2Vec2(20.f, 0.f));
            else body->native().body->SetLinearVelocity(b2Vec2(0.f, 0.f));
            const auto callsBefore = phaseCalls;
            const bool updated = session.update(currentDt, attack, position_of(*playerActor),
                playerActor->transform.rotation[2], error);
            if (!updated) {
                check(reachedFailure && error.find("intentional motion-phase failure") != std::string::npos,
                      "Unexpected motion phase failure: " + error);
                check(!failedSamples.empty(), "Failure did not follow a real authored motion sample");
                currentDt = 0.0;
                const auto beforeRecoveryCalls = phaseCalls;
                check(session.update(0, attack, position_of(*playerActor),
                      playerActor->transform.rotation[2], error), error);
                const bool sameBatch = playerSamples.size() == failedSamples.size() &&
                    std::equal(playerSamples.begin(), playerSamples.end(), failedSamples.begin(),
                        [](const auto& actual, const auto& prior) {
                            return actual.second == prior.move_go &&
                                actual.first.x == prior.authored_motion.x &&
                                actual.first.y == prior.authored_motion.y &&
                                actual.first.z == prior.authored_motion.z;
                        });
                check(phaseCalls - beforeRecoveryCalls == actorCount && !sameBatch,
                      "Retired failed sample batch replayed on the next zero-dt frame: prior=" +
                      std::to_string(failedSamples.size()) + " current=" +
                      std::to_string(playerSamples.size()) + " priorDelta=" +
                      (failedSamples.empty() ? std::string("none") :
                       std::to_string(failedSamples.front().authored_motion.x) + "," +
                       std::to_string(failedSamples.front().authored_motion.y) + "," +
                       std::to_string(failedSamples.front().authored_motion.z)) + " currentDelta=" +
                      (playerSamples.empty() ? std::string("none") :
                       std::to_string(playerSamples.front().first.x) + "," +
                       std::to_string(playerSamples.front().first.y) + "," +
                       std::to_string(playerSamples.front().first.z)));
                sawFailureRecovery = true;
                continue;
            }
            check(phaseCalls - callsBefore == actorCount, "Frame did not deliver one phase per current actor");
            if (!playerSamples.empty() && appliedRoot && importedPhysics) sawSampleAndStep = true;
            if (sourceHitResolutions) failOnLizard = true;
        }
        check(sourceHitNotifications > 0 && sourceHitResolutions > 0 && hitWasAppliedBeforePhase,
              "Original marker/result was not applied before the position phase: notifications=" +
              std::to_string(sourceHitNotifications) + " resolutions=" +
              std::to_string(sourceHitResolutions) + " rng=" +
              std::to_string(session.world()->random_state().calls - rngCallsBeforeCombat) +
              " action=" + std::to_string(static_cast<int>(playerActor->action)));
        check(!playerSamples.empty() && nonzeroRootSamples > 0 && appliedRoot > 0 && importedPhysics > 0,
              "Actual source branch missing: samples=" + std::to_string(playerSamples.size()) +
              " nonzeroRoot=" + std::to_string(nonzeroRootSamples) +
              " appliedRoot=" + std::to_string(appliedRoot) +
              " importedPhysics=" + std::to_string(importedPhysics) +
              " disabled=" + std::to_string(disabledSamples));
        check(sawSampleAndStep, "Did not observe same-Session source samples with both real position branches");
        check(reentryChecksPassed && sawFailureRecovery,
              "Motion phase reentry guards or failed-prefix recovery were not exercised");

        // Finish the source action to produce a checkpointable restored pose.
        attack.attack = false;
        attack.move2D = {};
        for (unsigned frame = 0; frame < 600 && session.owns_pose(1); ++frame) {
            currentDt = 1.0 / 60.0;
            check(session.update(currentDt, attack, position_of(*playerActor),
                  playerActor->transform.rotation[2], error), error);
        }
        check(!session.owns_pose(1), "Source action did not complete for restore checks");
        session.clear_diagnostic_controller_admission_provider();
        session.set_animation_notification_services({});
        check(bodies.clear(error), error);
        const auto serialBeforeDetach = session.update_serial();
        auto character = make_default_character("motion-phase-checkpoint", "Knight", "KnightPlayerBase");
        playerActor->persistent_character_id = character.id;
        GameSave saved;
        check(capture_game_save("motion-phase", 1, character, *session.world(), saved, error), error);
        session.detach_for_restore();
        check(restore_game_save(saved, "motion-phase", *session.world(), character, error), error);
        check(session.rebind_after_restore(error), error);
        check(!session.update(0, {}, position_of(*session.actor(1)), 0, error) &&
              error.find("required fresh motion phase") != std::string::npos,
              "Restore allowed update without a fresh phase owner");
        check(session.update_serial() == serialBeforeDetach,
              "Missing restored phase owner advanced the Session frame");
        currentDt = 0.0;
        check(session.set_motion_phase_handler(
            [&](ActorState&, std::uint64_t frame, double dt,
                const std::vector<CombatSessionMotionSample>& samples, std::string&) {
                ++phaseCalls;
                check(frame == session.update_serial() && dt == 0.0 &&
                      std::all_of(samples.begin(), samples.end(), [](const auto& sample) {
                          return sample.authored_motion.x == 0.f && sample.authored_motion.y == 0.f &&
                                 sample.authored_motion.z == 0.f;
                      }), "Restored phase replayed displacement or lost current frame owner data");
                return true;
            }, error), error);
        check(session.update(0, {}, position_of(*session.actor(1)), 0, error), error);
        check(session.update_serial() == serialBeforeDetach + 1,
              "Fresh restored phase owner did not resume the same Session");

        // P16 LIFECYCLE (core change): a state selection between updates (a spawn's begin/select) queues root samples
        // outside an update. The per-frame rebinding must still succeed and the queued samples must reach the handler
        // bound at the next update; removal stays refused while they are queued.
        // Source sequence path (the lifecycle spawn's own call for state 1): a non-looping Injured sequence with an explicit group.
        OriginalAttackSelection reactSelection;reactSelection.state="Injured";reactSelection.variant=0;reactSelection.group_path={0};
        CombatSessionStateAnimationServices reactServices; // whole-sequence completion is not under test here
        reactServices.event = [](ActorId, const RetainedAnimationEvent&, std::string&) { return true; };
        reactServices.finished = [](ActorId, std::string&) { return true; };
        check(session.play_actor_state_sequence(1, reactSelection, reactServices, error),
              "Between-update Injured sequence of the player was refused: " + error);
        std::string pendingError;
        check(!session.clear_motion_phase_handler(pendingError) &&
              pendingError.find("without pending samples") != std::string::npos,
              "Queued between-update samples were not held before the handler was rebound");
        std::size_t queuedDelivered = 0, rebindCalls = 0;
        check(session.set_motion_phase_handler(
            [&](ActorState& actor, std::uint64_t frame, double, const std::vector<CombatSessionMotionSample>& samples,
                std::string&) {
                ++rebindCalls;
                check(frame == session.update_serial() && session.actor(actor.id) == &actor,
                      "Rebound handler received a stale frame or actor");
                if (actor.id == 1) queuedDelivered += samples.size();
                return true;
            }, error), "Per-frame rebinding was refused while between-update samples were queued: " + error);
        check(session.update(0, {}, position_of(*session.actor(1)), 0, error), error);
        check(rebindCalls > 0 && queuedDelivered > 0,
              "Between-update samples were not delivered to the rebound handler at the next update");
        check(session.clear_motion_phase_handler(error), error);
        std::cout << "PASS actual retained source attack marker before ordered motion samples; same-Session NativeWorld/PF floor import and root movement; zero-dt actor callbacks; restore requires fresh phase binding\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "CombatSession motion phase FAIL: " << failure.what() << '\n';
        return 1;
    }
}
