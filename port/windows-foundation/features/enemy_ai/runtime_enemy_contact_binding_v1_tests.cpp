#include "runtime_enemy_contact_binding_v1.hpp"
#include "runtime_enemy_controller_v1.hpp"

#include "../../asset_catalog.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_melee_bindings.hpp"
#include "../../original_actor_properties.hpp"
#include "../../actor_state.hpp"
#include "../../../game-data/skill_tables.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;
using namespace dh::foundation::physics;

namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
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
        const auto skill_records = assets.read("original-cache/data/pydata/skills_pyarray.bin");
        const auto skill_names = assets.read("original-cache/data/pydata/skills_pyarraynames.bin");
        const auto skill_fields = assets.read("original-cache/data/pydata/skills_pystructnames.bin");
        dh2::data::SkillTables skill_tables;
        check(skill_tables.load({skill_records.data(), skill_records.size()},
                                {skill_names.data(), skill_names.size()},
                                {skill_fields.data(), skill_fields.size()}, error), error);
        check(std::none_of(skill_tables.borrow().skills().begin(),
                           skill_tables.borrow().skills().end(), [](const auto& row) {
                  return (row.scalar.words[7] & 0x02000000u) != 0;
              }),
              "Actual source Skill cache unexpectedly reaches CancelSneaking Active/Pre callbacks");

        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan playerVisual, enemyVisual;
        check(build_original_combat_visual_plan(assets, melee, "KnightPlayerBase", customization,
              "enemy-contact-player", playerVisual, error), error);
        check(build_original_combat_visual_plan(assets, melee, "Swamp_LizadMan_Type1", customization,
              "enemy-contact-lizard", enemyVisual, error), error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 17;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = playerVisual.config;
        config.playerVisualConfig.motion_node_id = "auto";
        CombatSessionProfile playerProfile;
        playerProfile.action = {"AttackStatic", 0, {0, 1}};
        playerProfile.initialIdle = {"Idle", 0, {0}};
        playerProfile.damageMarkerNames = {"attack_mainhand"};
        playerProfile.propertyOptions = {256, true};
        config.profiles.emplace("KnightPlayerBase", playerProfile);
        CombatSessionProfile enemyProfile;
        enemyProfile.action = {"Attack", 0, {0, 1}};
        enemyProfile.initialIdle = {"Idle", 0, {0}};
        enemyProfile.death = CombatSessionChoice{"Died", 0, {0}};
        enemyProfile.damageMarkerNames = {"attack_mainhand"};
        enemyProfile.propertyOptions = {256, true};
        enemyProfile.customization = customization;
        config.profiles.emplace("Swamp_LizadMan_Type1", enemyProfile);

        ActorPopulation population;
        PopulationActor lizard;
        lizard.profileId = "Swamp_LizadMan_Type1";
        lizard.definition.name = "_prim_Monster_contact_fixture";
        lizard.definition.stableId = 2;
        lizard.definition.sourceId = "enemy-contact-same-session-lizard";
        lizard.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        lizard.transform = lizard.definition.placement;
        population.actors().push_back(std::move(lizard));
        PopulationActor secondLizard;
        secondLizard.profileId = "Swamp_LizadMan_Type1";
        secondLizard.definition.name = "_prim_Monster_contact_fixture_peer";
        secondLizard.definition.stableId = 3;
        secondLizard.definition.sourceId = "enemy-contact-same-session-peer";
        secondLizard.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        secondLizard.transform = secondLizard.definition.placement;
        population.actors().push_back(std::move(secondLizard));

        CharacterVisual visual;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, visual, population,
                                 {0,0,0}, customization, error), error);
        check(session.update(0.0, {}, {0,0,0}, 0.0f, error), error);
        ActorState* enemy = session.actor(2);
        ActorState* sameFactionPeer = session.actor(3);
        ActorState* player = session.actor(1);
        check(enemy && sameFactionPeer && player && session.world()->find_actor(2) == enemy &&
              session.world()->find_actor(1) == player,
              "Contact fixture did not use the canonical same-Session records");
        enemy->transform.position = {0.f,0.f,0.f};
        sameFactionPeer->transform.position = {0.f,0.f,0.f};
        player->transform.position = {0.f,0.f,0.f};
        // CharacterAction::moving is the Session's current source-state
        // projection4; no lifecycle state or FSM is cloned for contact.
        enemy->action = CharacterAction::moving;
        check(session.update(0.0, {}, {0,0,0}, 0.0f, error), error);

        unsigned losCalls = 0, stopCalls = 0;
        bool trackedRayVisible = false;
        bool failStop = false;
        RuntimeEnemyControllerServicesV1 decisionServices;
        decisionServices.can_see = [&](CombatSession& current, const ActorState& source,
                                       const ActorState& target,
                                       const OriginalCombatProperties&,
                                       const OriginalCombatProperties&,
                                       const dh2::data::AiProps&, bool& visible,
                                       std::string&) {
            check(&current == &session, "Enemy controller left the contact Session");
            if (source.id == 2) {
                check(&source == session.actor(2) && &target == session.actor(target.id) &&
                      (source.target_id == invalid_actor_id || source.target_id == target.id),
                      "Enemy controller left the same-Session source/candidate actors");
                ++losCalls;
                visible = trackedRayVisible;
            } else {
                visible = false;
            }
            return true;
        };
        decisionServices.stop = [&](CombatSession& current, ActorState& source,
                                    std::string& problem) {
            check(&current == &session && session.actor(source.id) == &source,
                  "Collision pause Stop left the current same-Session ActorState");
            ++stopCalls;
            if (failStop) {
                problem = "source Stop owner failed after pause/timer31 prefix";
                return false;
            }
            return true;
        };
        RuntimeEnemyControllerV1 controller(std::move(decisionServices));
        check(controller.begin_contact_frame(session, 16, error), error);

        unsigned snapshotCalls = 0, debugLoads = 0, deleteBuffCalls = 0;
        RuntimeEnemyContactProjectionServicesV1 projection;
        projection.selected_ais_policy = [&](ActorId id, RuntimeEnemyContactAISPolicyV1& out,
                                             std::string&) {
            if (!session.actor(id)) return false;
            ++snapshotCalls;
            // Explicit source-vtable policy: this fixture exercises a selected
            // AISExternal/Player-compatible inherited AISDefault collision row.
            out = RuntimeEnemyContactAISPolicyV1::inherited_default;
            return true;
        };
        projection.debug_load = [&](std::string&) { ++debugLoads; return true; };
        projection.debug_switch = [](const char* key, bool& value, std::string& problem) {
            if (!key || std::string(key) != "isTracingPlayersCollision") {
                problem = "Unexpected source collision debug switch";
                return false;
            }
            value = false;
            return true;
        };
        projection.sneaking.skill_tables = skill_tables.borrow();
        projection.sneaking.delete_buff_record = [&](ActorId id, std::uint32_t key,
                                                      std::uintptr_t token,
                                                      std::string& problem) {
            if (id != 1 || key != 146 || token != 0xfeedu || !session.actor(id)) {
                problem = "DelBuff fixture rejected source actor/key/record";
                return false;
            }
            RuntimeEnemyContactControllerProjectionV1 before{};
            if (!controller.project_contact(session, id,
                    RuntimeEnemyContactAISPolicyV1::inherited_default, before, problem) ||
                before.interactive415 != 0) {
                if (problem.empty()) problem = "CancelSneaking changed byte415 before DelBuff completed";
                return false;
            }
            ++deleteBuffCalls;
            return true;
        };
        // The controller's canonical per-actor buff registry begins empty;
        // this record represents one real owner-published buff instance.
        check(controller.track_contact_buff(session, 1, 146, 0xfeedu, error), error);

        RuntimeEnemyContactBindingV1 binding(session, controller, std::move(projection));
        RuntimeSessionContactOwnerV1 contact(session, binding.services());
        check(contact.bind_body(1, error), error);
        check(contact.bind_body(2, error), error);
        check(contact.bind_body(3, error), error);
        check(enemy->target_id == invalid_actor_id && debugLoads == 0,
              "No contact must leave the source target/event path untouched");

        // AISDefault Begin calls the supplied SetTarget continuation only after
        // the real source IsCharacter/IsPlayer/IsEnemy kernel accepts contact.
        // The enemy composer publishes to the canonical Session target field.
        check(contact.physical_event(2, RuntimeSessionContactEventV1::begin, 1, 1, error), error);
        check(enemy->target_id == 1 && snapshotCalls >= 3 && debugLoads == 1,
              "Admitted same-session contact did not publish the canonical target");
        RuntimeEnemyContactControllerProjectionV1 liveController{};
        check(controller.project_contact(session, 2,
              RuntimeEnemyContactAISPolicyV1::inherited_default, liveController, error), error);
        check(liveController.controller_projection != &enemy->action &&
              liveController.preferred_target_known && liveController.preferred_target == 1 &&
              liveController.dt_ms == 16 && liveController.application_frame == session.update_serial(),
              "Contact did not feed actual same-controller target/frame/dt projection");
        const auto* enemyFacts = session.world()->combat_properties(2);
        const auto* peerFacts = session.world()->combat_properties(3);
        check(enemyFacts && peerFacts && !dh2::data::ai_enemy(*session.original_ai_tables(),
              enemyFacts->sheets.resolved[0], peerFacts->sheets.resolved[0], false, false),
              "Source fixture's two lizards are not same-faction non-enemies");

        check(controller.update(session, 0.0, error), error);
        check(losCalls == 0 && controller.report().attack_requests == 1 &&
              session.actor(2)->target_id == 1,
              "In-range tracked target was incorrectly dropped by a false navigation ray");

        // Source _UpdateTarget uses the strict authored view-radius predicate
        // for an existing target; it does not consult the navigation ray.
        // Out-of-radius and dead targets remain independent clear conditions.
        player->transform.position = {10000.f,0.f,0.f};
        check(controller.update(session, 0.0, error), error);
        check(session.actor(2)->target_id == invalid_actor_id &&
              controller.report().target_range_losses == 1 && losCalls == 0,
              "Tracked target outside authored AI view radius was not cleared by source range policy");
        player->transform.position = {0.f,0.f,0.f};
        check(controller.set_contact_target(session, 2, 1, 0, error), error);
        const auto playerAction = player->action;
        player->action = CharacterAction::dead;
        check(controller.update(session, 0.0, error), error);
        check(session.actor(2)->target_id == invalid_actor_id &&
              controller.report().target_deaths == 1 && losCalls == 0,
              "Dead tracked target was retained after the source dead-target gate");
        player->action = playerAction;
        const auto acquisitionRayCalls = losCalls;
        check(controller.update(session, 0.0, error), error);
        check(session.actor(2)->target_id == invalid_actor_id &&
              losCalls == acquisitionRayCalls + 1,
              "Untracked candidate bypassed the existing navigation-ray acquisition gate");
        trackedRayVisible = true;

        // The source player-contact branch executes the recovered
        // CancelSneaking kernel on actual resolved properties and actual
        // SkillTables. Force only the raw Special_Sneak source word positive
        // for this branch test; the real authored skill cache has no matching
        // cancel-skill flag, so Active/Pre remain correctly unreachable.
        auto* playerProps = const_cast<OriginalCombatProperties*>(
            session.world()->combat_properties(1));
        check(playerProps != nullptr, "Player source property projection missing");
        const auto savedSneak = playerProps->sheets.resolved[198];
        playerProps->sheets.resolved[198] = 1;
        InputActions playerMoving{};
        playerMoving.move2D.y = 1.0f;
        check(session.update(0.016, playerMoving, {0,0,0}, 0.0f, error),
              "Player movement update after enemy decision: " + error);
        check(controller.begin_contact_frame(session, 16, error), error);
        check(contact.physical_event(1, RuntimeSessionContactEventV1::begin, 2, 1, error), error);
        RuntimeEnemyContactControllerProjectionV1 playerContactProjection{};
        check(controller.project_contact(session, 1,
              RuntimeEnemyContactAISPolicyV1::inherited_default,
              playerContactProjection, error), error);
        check(deleteBuffCalls == 1 && playerContactProjection.interactive415 == 1 &&
              player->target_id == invalid_actor_id,
              "Source CancelSneaking did not deliver DelBuff146 before setting byte415");
        check(contact.physical_event(1, RuntimeSessionContactEventV1::persist, 2, 1, error), error);
        check(deleteBuffCalls == 1,
              "Empty same-ActorRuntime buff map repeated a removed DelBuff146 record");
        playerProps->sheets.resolved[198] = savedSneak;
        auto* enemyProps = const_cast<OriginalCombatProperties*>(
            session.world()->combat_properties(2));
        check(enemyProps != nullptr, "Enemy source property projection missing");
        const auto savedEnemySneak = enemyProps->sheets.resolved[198];
        enemyProps->sheets.resolved[198] = 1;
        RuntimeEnemyContactSneakingServicesV1 unavailablePositiveSkill;
        check(!controller.cancel_contact_sneaking(session, 2,
                  unavailablePositiveSkill, error) &&
              error.find("positive Special_Sneak") != std::string::npos,
              "Positive Special_Sneak accepted without its original SkillList/skill/script owners");
        enemyProps->sheets.resolved[198] = savedEnemySneak;

        // A repeat contact with the current target must not re-deliver SetTarget.
        check(controller.set_contact_target(session, 2, 1, 0, error), error);
        const auto snapshotsBeforeRepeat = snapshotCalls;
        check(contact.physical_event(2, RuntimeSessionContactEventV1::begin, 1, 1, error), error);
        check(session.actor(2)->target_id == 1 && snapshotCalls > snapshotsBeforeRepeat,
              "Repeated current-target contact did not remain a valid source callback");

        // Drive the controller and canonical contact owner through the actual
        // AISDefault >199 gate. Success commits the one Stop call and resets
        // the source counter; timer31 expires on one later Session frame with
        // its exact caller-provided 1000ms step. A separate Stop rejection
        // must retain its delivered pause/timer prefix and leave the counter.
        check(controller.set_contact_target(session, 2, invalid_actor_id, 0, error), error);
        enemy->action = CharacterAction::moving;
        auto accumulate_collision = [&](bool expectStopFailure) {
            unsigned frames = 0;
            while (contact.ais_fields(2)->collision_ms <= 199u) {
                check(++frames <= 32, "Source moving contact did not cross the >199 gate");
                check(session.update(0.016, {}, {0,0,0}, 0.0f, error), error);
                check(controller.begin_contact_frame(session, 16, error), error);
                check(contact.physical_event(2, RuntimeSessionContactEventV1::persist,
                                             3, 1, error), error);
            }
            failStop = expectStopFailure;
        };
        accumulate_collision(false);
        const auto collisionBeforePause = contact.ais_fields(2)->collision_ms;
        const auto stopsBeforePause = stopCalls;
        check(controller.process_contact_pause(session, contact, error), error);
        RuntimeEnemyContactControllerProjectionV1 pausedState{};
        check(controller.project_contact(session, 2,
              RuntimeEnemyContactAISPolicyV1::inherited_default, pausedState, error), error);
        check(stopCalls == stopsBeforePause + 1 && pausedState.paused == 1 &&
              pausedState.dt_ms == 16 && contact.ais_fields(2)->collision_ms == 0 &&
              collisionBeforePause > 199,
              "Controller threshold path did not execute pause/timer31/Stop then reset its canonical counter");
        check(session.update(1.0, {}, {0,0,0}, 0.0f, error), error);
        check(controller.begin_contact_frame(session, 1000, error), error);
        check(controller.project_contact(session, 2,
              RuntimeEnemyContactAISPolicyV1::inherited_default, pausedState, error), error);
        check(pausedState.paused == 0,
              "Source timer31 did not clear collision pause after one 1000ms Session frame");

        accumulate_collision(true);
        const auto collisionAfterFailedStop = contact.ais_fields(2)->collision_ms;
        const auto stopsBeforeFailedStop = stopCalls;
        check(!controller.process_contact_pause(session, contact, error) &&
              error.find("Stop owner failed") != std::string::npos,
              "Controller accepted a missing/failed source Stop continuation");
        check(controller.project_contact(session, 2,
              RuntimeEnemyContactAISPolicyV1::inherited_default, pausedState, error), error);
        check(stopCalls == stopsBeforeFailedStop + 1 && pausedState.paused == 1 &&
              contact.ais_fields(2)->collision_ms == collisionAfterFailedStop &&
              collisionAfterFailedStop > 199,
              "Failed Stop rolled back the reached pause/timer prefix or reset the source counter");
        failStop = false;
        check(session.update(1.0, {}, {0,0,0}, 0.0f, error), error);
        check(controller.begin_contact_frame(session, 1000, error), error);
        check(controller.project_contact(session, 2,
              RuntimeEnemyContactAISPolicyV1::inherited_default, pausedState, error), error);
        check(pausedState.paused == 0 &&
              contact.ais_fields(2)->collision_ms == collisionAfterFailedStop,
              "Timer31 expiry after failed Stop changed the unreached collision-counter reset prefix");

        // Current target is the source early return; non-enemy Persist then
        // reaches the exact owner pause/frame counter tail without SetTarget.
        enemy->target_id = invalid_actor_id;
        enemy->action = CharacterAction::moving;
        check(session.update(0.0, {}, {0,0,0}, 0.0f, error), error);
        unsigned paused = 1, frame = 2;
        RuntimeEnemyContactProjectionServicesV1 pausedProjection;
        pausedProjection.controller = [&](ActorId id, RuntimeEnemyContactControllerProjectionV1& out,
                                          std::string& problem) {
            const auto* actor = session.actor(id);
            if (!actor) { problem = "Actor missing from pause fixture"; return false; }
            out.controller_projection = &actor->action;
            out.preferred_target_known = true;
            out.preferred_target = actor->target_id;
            out.application_frame = frame;
            out.dt_ms = 16;
            out.paused = paused;
            out.ais_policy = RuntimeEnemyContactAISPolicyV1::inherited_default;
            return true;
        };
        pausedProjection.debug_load = [](std::string&) { return true; };
        pausedProjection.debug_switch = [](const char*, bool& value, std::string&) { value = false; return true; };
        pausedProjection.cancel_sneaking = [](ActorId, std::string&) { return true; };
        RuntimeEnemyContactBindingV1 pausedBinding(session, std::move(pausedProjection));
        RuntimeSessionContactOwnerV1 pausedContact(session, pausedBinding.services());
        check(pausedContact.bind_body(2, error) && pausedContact.bind_body(3, error), error);
        check(pausedContact.physical_event(2, RuntimeSessionContactEventV1::persist, 3, 1, error), error);
        check(enemy->target_id == invalid_actor_id &&
              pausedContact.ais_fields(2)->collision_ms == 0,
              "Paused source controller did not suppress the nonenemy collision counter tail");
        paused = 0;
        frame = 3;
        check(pausedContact.physical_event(2, RuntimeSessionContactEventV1::persist, 3, 1, error), error);
        check(enemy->target_id == invalid_actor_id &&
              pausedContact.ais_fields(2)->collision_ms == 16,
              "Unpaused source controller did not accumulate only on the later Application frame");

        // Missing policy and detached/stale bindings fail closed before any
        // source target effect can be delivered.
        RuntimeEnemyContactProjectionServicesV1 unknownPolicy;
        unknownPolicy.controller = [&](ActorId id, RuntimeEnemyContactControllerProjectionV1& out,
                                       std::string&) {
            const auto* actor = session.actor(id);
            if (!actor) return false;
            out.controller_projection = &actor->action;
            out.preferred_target_known = true;
            out.ais_policy = RuntimeEnemyContactAISPolicyV1::unknown;
            return true;
        };
        RuntimeEnemyContactBindingV1 badBinding(session, std::move(unknownPolicy));
        const auto badServices = badBinding.services();
        RuntimeSessionContactOwnerV1 badContact(session, badServices);
        check(!badContact.bind_body(2, error) &&
              error.find("unknown selected AIS collision policy") != std::string::npos,
              "Unknown selected AIS collision policy did not fail closed");

        RuntimeEnemyContactProjectionServicesV1 missingDebug;
        missingDebug.controller = [&](ActorId id, RuntimeEnemyContactControllerProjectionV1& out,
                                      std::string&) {
            const auto* actor = session.actor(id);
            if (!actor) return false;
            out.controller_projection = &actor->action;
            out.preferred_target_known = true;
            out.preferred_target = actor->target_id;
            out.ais_policy = RuntimeEnemyContactAISPolicyV1::inherited_default;
            return true;
        };
        RuntimeEnemyContactBindingV1 missingDebugBinding(session, std::move(missingDebug));
        RuntimeSessionContactOwnerV1 missingDebugContact(session, missingDebugBinding.services());
        enemy->target_id = invalid_actor_id;
        check(missingDebugContact.bind_body(2, error) && missingDebugContact.bind_body(1, error), error);
        check(!missingDebugContact.physical_event(2, RuntimeSessionContactEventV1::begin, 1, 1, error) &&
              error.find("Debug") != std::string::npos &&
              enemy->target_id == invalid_actor_id,
              "Missing source Debug service did not fail before target mutation");

        check(pausedContact.unbind_body(2, error) && pausedContact.unbind_body(3, error), error);
        check(contact.unbind_body(1, error) && contact.unbind_body(2, error) &&
              contact.unbind_body(3, error), error);
        session.detach_for_restore();
        check(!contact.physical_event(2, RuntimeSessionContactEventV1::result, 0, 0, error) &&
              error.find("stale/detached") != std::string::npos,
              "Stale same-Session contact lease was accepted");

        // Pin both the actor-generation and lifetime witnesses while the
        // Session is destroyed. The feature composer must reject through its
        // lifetime token before querying the raw Session/controller provider.
        auto doomedSession = std::make_unique<CombatSession>();
        CharacterVisual doomedVisual;
        check(doomedSession->initialize(assets, database, melee, config, doomedVisual,
              population, {0,0,0}, customization, error), error);
        unsigned destroyedControllerCalls = 0;
        RuntimeEnemyContactProjectionServicesV1 doomedProjection;
        doomedProjection.controller = [&](ActorId, RuntimeEnemyContactControllerProjectionV1&,
                                          std::string&) {
            ++destroyedControllerCalls;
            return true;
        };
        RuntimeEnemyContactBindingV1 doomedBinding(*doomedSession, std::move(doomedProjection));
        const auto doomedServices = doomedBinding.services();
        auto pinnedActorBinding = doomedSession->actor_binding_lease().lock();
        auto pinnedLifetime = doomedSession->lifetime_lease().lock();
        check(pinnedActorBinding && pinnedLifetime && pinnedLifetime->alive(),
              "Enemy contact lifetime regression did not pin both Session tokens");
        doomedSession.reset();
        RuntimeSessionContactSnapshotV1 deadSnapshot{};
        check(!pinnedLifetime->alive() &&
              !doomedServices.snapshot(1, deadSnapshot, error) &&
              error.find("destroyed CombatSession") != std::string::npos &&
              destroyedControllerCalls == 0,
              "Enemy composer touched or accepted a destroyed CombatSession");

        std::cout << "PASS same-Session contact target→enemy decision, no-contact silence, "
                     "source CancelSneaking kernel, collision pause/Stop/timer31 expiry and failure prefix, "
                     "missing-service/AIS rejection, and stale lease; "
                  << "LOS=" << losCalls << " debug=" << debugLoads
                  << " DelBuff146-callbacks=" << deleteBuffCalls << '\n';
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << "runtime enemy contact binding FAIL: " << failure.what() << '\n';
        return 1;
    }
}
