#include "../asset_catalog.hpp"
#include "../combat_session.hpp"
#include "../original_actor_body_plan.hpp"
#include "../original_actor_properties.hpp"
#include "../playable_actor_bodies.hpp"
#include "../original_melee_bindings.hpp"
#include "../source_module_floors.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <stdexcept>
#include <tuple>
#include <vector>

using namespace dh::foundation;

namespace {
void check(bool ok, const std::string& why) {
    if (!ok) throw std::runtime_error(why);
}

struct NativeSlots {
    std::array<float, 3> destination{};
    std::uintptr_t attached{}, visual{};
};

struct SourceObstacles {
    std::array<dh2::navigation::ObstacleEntry, 8> entries{};
    std::array<unsigned, 8> floors{};
    dh2::navigation::ObstacleRegistry registry{entries.data(), 0, 8,
                                                floors.data(), 0, 8};
};

struct FixtureActor {
    ActorId id{};
    std::string profile;
    std::string source_id;
    ActorCustomization customization;
    OriginalCombatVisualPlan visual;
};
} // namespace

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply the original shared asset root");
        AssetCatalog assets(argv[1]);
        std::string error;
        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
        check(melee.load(assets, "original-melee-bindings.xml", error), error);

        ActorCustomization customization;
        customization.allow_missing_animation_targets = true;
        std::vector<FixtureActor> fixture;
        for (const auto& item : std::array<std::pair<ActorId, const char*>, 2>{{
                 {1, "MagePlayerBase"}, {2, "Swamp_LizadMan_Type1"}}}) {
            FixtureActor actor;
            actor.id = item.first;
            actor.profile = item.second;
            actor.source_id = "physics-frame-" + std::to_string(actor.id);
            actor.customization = customization;
            check(build_original_combat_visual_plan(assets, melee, actor.profile,
                  customization, actor.source_id, actor.visual, error), error);
            actor.visual.config.motion_node_id = "auto";
            actor.visual.config.consume_root_motion = true;
            fixture.push_back(std::move(actor));
        }

        CombatSessionConfig config;
        config.diagnosticRngSeed = 917;
        config.playerId = fixture[0].id;
        config.playerProfileId = fixture[0].profile;
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = fixture[0].visual.config;
        CombatSessionProfile mage;
        mage.initialIdle = {"Idle", 0, {0}};
        mage.customization = customization;
        mage.propertyOptions = {256, false};
        mage.animationOnly = true;
        config.profiles.emplace(fixture[0].profile, mage);
        CombatSessionProfile lizard;
        lizard.action = {"Attack", 0, {0, 1}};
        lizard.initialIdle = {"Idle", 0, {0}};
        lizard.damageMarkerNames = {"attack_mainhand"};
        lizard.customization = customization;
        lizard.propertyOptions = {256, true};
        config.profiles.emplace(fixture[1].profile, lizard);

        ActorPopulation population;
        PopulationActor placed;
        placed.profileId = fixture[1].profile;
        placed.definition.stableId = fixture[1].id;
        placed.definition.sourceId = fixture[1].source_id;
        placed.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        placed.transform = placed.definition.placement;
        population.actors().push_back(std::move(placed));
        CharacterVisual playerVisual;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, playerVisual,
              population, {0,0,0}, customization, error), error);
        check(session.actor(1) && session.actor(2) && session.world(),
              "Expected Mage and Lizard actors in one current CombatSession");
        check(session.update(0, {}, {0,0,0}, 0, error), error);
        auto* mageActor = session.actor(1);
        auto* lizardActor = session.actor(2);
        mageActor->transform.position = {0.f, 0.f, 23.f};
        lizardActor->transform.position = {0.f, 0.f, 0.f};
        mageActor->health = mageActor->max_health = 91.f;
        mageActor->target_id = invalid_actor_id;
        const auto sourceResolvedBefore = session.world()->combat_properties(1)->sheets.resolved;

        std::map<ActorId, NativeSlots> slots;
        auto nativeWorld = std::make_shared<dh2::physical::NativeWorld>();
        const float bounds[]{-2000.f, -2000.f, 2000.f, 2000.f};
        nativeWorld->load(bounds);
        PlayableActorBodies bodies;
        std::vector<std::tuple<ActorId, dh2::physical::ContactEvent, ActorId, unsigned>> contacts;
        bool fail_next_contact = false;
        unsigned failed_callback_calls = 0;
        bool try_reentrant_release = false;
        bool reentrant_release_rejected = false;
        bool reentrant_stop_rejected = false;

        for (const auto& source : fixture) {
            auto* actor = session.actor(source.id);
            const auto* facts = session.world()->combat_properties(source.id);
            check(actor && facts, "Current Session actor/property owner was absent");
            OriginalActorBodyPlanInput input;
            input.properties = &facts->sheets;
            input.ai = session.original_ai_tables();
            input.position = {actor->transform.position[0], actor->transform.position[1],
                              actor->transform.position[2]};
            input.source_name = source.profile;
            input.owner_identity = source.id;
            input.visual = source.visual.config;
            input.visual.use_authored_modular_defaults = true;
            OriginalActorBodyPlan plan;
            check(make_original_actor_body_plan(assets, input, plan, error), error);
            check(plan.physical_enabled, "Actual Mage/Lizard source plan did not admit a body");

            auto lease = std::const_pointer_cast<void>(session.actor_binding_lease().lock());
            check(lease != nullptr, "Current Session body lease was absent");
            auto& state = slots[source.id];
            OriginalActorPhysicalBindings physical;
            physical.actor_lease = lease;
            physical.world_lease = nativeWorld;
            physical.data_lease = lease;
            physical.readonly_properties = &facts->sheets;
            physical.destination1a8 = state.destination.data();
            physical.attached2e0 = &state.attached;
            physical.visual2d8 = &state.visual;
            physical.world = nativeWorld.get();
            physical.ai = session.original_ai_tables();
            physical.static84 = [](std::uint8_t& out, std::string&) { out = 0; return true; };
            physical.is_player = [name=source.profile](std::int32_t type, bool& out, std::string&) {
                out = original_actor_source_is_player(type, name);
                return true;
            };
            physical.debug_switch = [](const char*, bool& out, std::string&) { out = false; return true; };
            physical.filter = [](void*, const auto&, const auto&, bool& allowed, std::string&) {
                allowed = true; return true;
            };
            physical.contact = [&, owner=source.id](dh2::physical::ContactEvent event, void* peer,
                                                     unsigned side, std::string& problem) {
                ActorId peerId = invalid_actor_id;
                if (!bodies.resolve_physical_actor(peer, peerId, problem)) return false;
                contacts.emplace_back(owner, event, peerId, side);
                if (try_reentrant_release) {
                    try_reentrant_release = false;
                    std::string releaseError;
                    reentrant_release_rejected = !bodies.release(owner, releaseError) &&
                        releaseError.find("during physical Step") != std::string::npos;
                    bool stopped=false;
                    reentrant_stop_rejected = !bodies.stop_physical(owner,
                        [&](ActorId id){return session.actor(id);},stopped,releaseError) &&
                        !stopped && releaseError.find("during physical Step") != std::string::npos;
                }
                if (fail_next_contact) {
                    fail_next_contact = false;
                    ++failed_callback_calls;
                    problem = "intentional reached contact failure";
                    return false;
                }
                return true;
            };
            check(bodies.bind(*actor, *facts, plan, std::move(physical), error), error);
        }
        check(bodies.size() == 2, "Both same-Session source bodies were not registered");
        check(bodies.set_pinned(1, false, error) && bodies.set_pinned(2, false, error), error);
        auto* mageBody = bodies.physical(1)->native().body;
        auto* lizardBody = bodies.physical(2)->native().body;
        check(mageBody && lizardBody, "Expected actual NativeWorld body receivers");
        check(mageBody->GetMass() > 0.f && lizardBody->GetMass() > 0.f,
              "Unpinned awake source bodies need positive mass");

        PlayableActorBodies::CurrentActorLookup lookup = [&](ActorId id) { return session.actor(id); };
        bool stepped = false;
        try_reentrant_release = true;
        check(bodies.step_world(*nativeWorld, 1, 16, lookup, stepped, error), error);
        check(stepped, "First explicit current frame did not perform one NativeWorld step");
        check(reentrant_release_rejected && reentrant_stop_rejected && bodies.size() == 2,
              "Reentrant body release during contact delivery did not fail safely");
        check(std::any_of(contacts.begin(), contacts.end(), [](const auto& contact) {
            return std::get<0>(contact) != std::get<2>(contact) &&
                   std::get<1>(contact) == dh2::physical::ContactEvent::add;
        }), "Overlapping same-session body geometry did not deliver a real Begin contact");

        // Source Pin controls mass; actual native sleeping is an independent flag.
        mageBody->PutToSleep();
        dh2::physical::NativeBodyObservation observed{};
        check(dh2_native_body_observe(&observed, &bodies.physical(1)->native()) == 0 &&
              observed.sleeping == 1 && observed.mass > 0.f && observed.pinned == 0,
              "Native sleeping was conflated with pinned/zero-mass policy");
        PlayableActorBodies::PhysicsPositionResult publication;
        const auto asleepPosition = mageActor->transform.position;
        check(bodies.reconcile_physics_position(1, lookup, false, publication, error), error);
        check(publication.body_present && publication.sleeping && !publication.xy_changed &&
              mageActor->transform.position == asleepPosition,
              "Sleeping source body imported position when source would skip it");
        check(bodies.set_pinned(1, true, error), error);
        check(dh2_native_body_observe(&observed, &bodies.physical(1)->native()) == 0 &&
              observed.mass == 0.f && observed.pinned == 1,
              "Source pin did not alter mass independently of sleeping");
        check(bodies.set_pinned(1, false, error), error);
        check(bodies.physical(1)->native().body->GetMass() > 0.f,
              "Source unpin did not restore positive mass");

        // Preflight mismatch paths must not consume the next frame.
        PlayableActorBodies::CurrentActorLookup mismatched = [&](ActorId id) -> ActorState* {
            return id == 1 ? nullptr : session.actor(id);
        };
        // The whole source Stop prefix belongs to the host. This pool performs
        // only its conditional physical part on the exact current actor/body.
        const auto stopActorPosition=mageActor->transform.position;
        const auto stopDestination=slots[1].destination;
        const auto stopHealth=mageActor->health;
        const auto stopTarget=mageActor->target_id;
        const auto stopAction=mageActor->action;
        const auto stopRng=session.world()->random_state();
        mageBody->SetLinearVelocity(b2Vec2(3.f,-4.f));
        mageBody->SetAngularVelocity(.75f);
        mageBody->SetXForm(b2Vec2(7.f,9.f),.35f);
        bool stopped=false;
        check(!bodies.stop_physical(1,mismatched,stopped,error)&&!stopped&&
              mageBody->GetLinearVelocity().x==3.f&&mageBody->GetAngularVelocity()==.75f,
              "Stale registry Stop changed the actual physical receiver");
        check(bodies.stop_physical(1,lookup,stopped,error)&&stopped,error);
        check(dh2_native_body_observe(&observed,&bodies.physical(1)->native())==0&&
              observed.sleeping==1&&observed.linear_velocity[0]==0.f&&
              observed.linear_velocity[1]==0.f&&observed.angular_velocity==0.f&&
              observed.pinned==0&&observed.mass>0.f&&observed.angle==.35f&&
              std::fabs(observed.position[0]-stopActorPosition[0]*.01f)<.001f&&
              std::fabs(observed.position[1]-stopActorPosition[1]*.01f)<.001f&&
              mageActor->transform.position==stopActorPosition&&
              slots[1].destination==stopDestination&&mageActor->health==stopHealth&&
              mageActor->target_id==stopTarget&&mageActor->action==stopAction&&
              session.world()->random_state().seed==stopRng.seed&&
              session.world()->random_state().calls==stopRng.calls,
              "Source physical Stop failed velocity/sleep/reseat or changed pin/gameplay state");
        // The following solver fixture uses raw Box2D setters (which do not
        // wake a sleeping body); explicitly restore its awake precondition.
        mageBody->WakeUp();
        check(!bodies.step_world(*nativeWorld, 2, 16, mismatched, stepped, error) && !stepped,
              "Stale current-actor registry was accepted");
        dh2::physical::NativeWorld foreignWorld;
        foreignWorld.load(bounds);
        check(!bodies.step_world(foreignWorld, 2, 16, lookup, stepped, error) && !stepped,
              "Foreign world was accepted for registered source bodies");
        // Move the second body away and let the first body's actual solver motion
        // cross the source >1-game-unit publication threshold.
        check(bodies.set_position(2, {500.f, 0.f, 0.f}, false, error), error);
        const auto beforePosition = mageActor->transform.position;
        const auto destinationBefore = slots[1].destination;
        const auto healthBefore = mageActor->health;
        const auto targetBefore = mageActor->target_id;
        const auto actionBefore = mageActor->action;
        const auto rngBefore = session.world()->random_state();
        mageBody->SetLinearVelocity(b2Vec2(1.f, 0.f));
        check(bodies.step_world(*nativeWorld, 2, 100, lookup, stepped, error), error);
        check(stepped, "Valid frame after rejected preflight did not step");
        const auto contactsAfterStep = contacts.size();
        dh2::physical::NativeBodyObservation afterStep{};
        check(dh2_native_body_observe(&afterStep, &bodies.physical(1)->native()) == 0,
              "Native post-step body observation failed");
        check(afterStep.position[0] * 100.f > beforePosition[0] + 1.f,
              "Native Step did not move the actual source body by >1 game unit");
        check(bodies.reconcile_physics_position(1, lookup, false, publication, error), error);
        check(publication.xy_changed && publication.body_reseated &&
              std::fabs(mageActor->transform.position[0] - afterStep.position[0] * 100.f) < .01f &&
              mageActor->transform.position[2] == beforePosition[2] &&
              slots[1].destination == destinationBefore,
              "Body movement was not published to the same actor while preserving Z/destination");
        OriginalTriggerActorBorrow actorBorrow;
        check(bodies.actor_borrow(1, actorBorrow, error), error);
        for (unsigned i = 0; i < 6; ++i)
            check(std::fabs(actorBorrow.absolute12c[i] -
                  (bodies.physical(1)->relative_bounds()[i] + mageActor->transform.position[i % 3])) < .001f,
                  "Same actor AABB was not translated with published physics position");
        const auto published = mageActor->transform.position;
        check(bodies.step_world(*nativeWorld, 2, 100, lookup, stepped, error) && !stepped,
              "Duplicate same-frame/same-dt call did not skip physical Step");
        dh2::physical::NativeBodyObservation duplicateObservation{};
        check(dh2_native_body_observe(&duplicateObservation, &bodies.physical(1)->native()) == 0,
              "Duplicate frame native observation failed");
        check(std::fabs(duplicateObservation.position[0] - afterStep.position[0]) < .0001f,
              "Duplicate frame changed native body position");
        check(mageActor->transform.position == published,
              "Duplicate frame changed same-actor position");
        check(contacts.size() == contactsAfterStep,
              "Duplicate frame delivered a new contact callback");
        check(!bodies.step_world(*nativeWorld, 1, 16, lookup, stepped, error) && !stepped,
              "Stale frame was accepted after a completed step");
        check(!bodies.step_world(*nativeWorld, 2, 101, lookup, stepped, error) && !stepped,
              "Same frame with inconsistent dt was accepted");

        // Reached callback failure is latched inside Step, surfaced only after
        // Box2D unlock, and the failed occurrence's prefix cannot be replayed.
        check(bodies.set_position(2, mageActor->transform.position, false, error), error);
        fail_next_contact = true;
        const auto failCountBefore = failed_callback_calls;
        check(!bodies.step_world(*nativeWorld, 3, 16, lookup, stepped, error) && stepped &&
              error.find("intentional reached contact failure") != std::string::npos,
              "NativeWorld did not surface the reached contact failure after Step");
        check(failed_callback_calls == failCountBefore + 1 && nativeWorld->cleanup_delivery_idle_v106(),
              "Failure escaped with backend delivery still active or callback not reached once");
        check(!bodies.step_world(*nativeWorld, 3, 16, lookup, stepped, error) && !stepped &&
              failed_callback_calls == failCountBefore + 1,
              "Same failed frame replayed a delivered callback prefix");

        // Publication is an actor/position operation only; gameplay authorities
        // and the source property sheets remain untouched by the physics bridge.
        const auto rngAfter = session.world()->random_state();
        check(mageActor->health == healthBefore && mageActor->target_id == targetBefore &&
              mageActor->action == actionBefore && rngAfter.seed == rngBefore.seed &&
              rngAfter.calls == rngBefore.calls,
              "Physics Step/publication changed HP, target, action, or shared RNG");
        const auto* mageFacts = session.world()->combat_properties(1);
        check(mageFacts && mageFacts->sheets.resolved == sourceResolvedBefore,
              "Physics publication changed the Mage's immutable source property sheet");

        // A malformed current-owner lookup cannot be used for reconciliation.
        check(!bodies.reconcile_physics_position(1, mismatched, false, publication, error),
              "Stale same-session owner accepted physics publication");

        // Missing source navigation must stop before changing the actor, body,
        // or caller output. This is a reached requirement, not a floor fallback.
        const auto actorBeforeMissingFloor = mageActor->transform.position;
        const auto outputBeforeMissingFloor = PlayableActorBodies::PhysicsPositionResult{
            true, true, true, true, true, true, {7.f, 8.f, 9.f}};
        publication = outputBeforeMissingFloor;
        mageBody->SetXForm(b2Vec2((actorBeforeMissingFloor[0] + 7.f) * .01f,
                                  actorBeforeMissingFloor[1] * .01f), mageBody->GetAngle());
        dh2::physical::NativeBodyObservation beforeMissingFloor{};
        check(dh2_native_body_observe(&beforeMissingFloor, &bodies.physical(1)->native()) == 0,
              "Missing-navigation precondition body observation failed");
        check(!bodies.reconcile_physics_position(1, lookup, true, publication, error) &&
              error.find("initialized floor/PF world") != std::string::npos,
              "Missing actual navigation/floor owner became a successful fallback");
        dh2::physical::NativeBodyObservation afterMissingFloor{};
        check(dh2_native_body_observe(&afterMissingFloor, &bodies.physical(1)->native()) == 0 &&
              mageActor->transform.position == actorBeforeMissingFloor &&
              afterMissingFloor.position[0] == beforeMissingFloor.position[0] &&
              afterMissingFloor.position[1] == beforeMissingFloor.position[1] &&
              publication.body_present == outputBeforeMissingFloor.body_present &&
              publication.position == outputBeforeMissingFloor.position,
              "Missing floor service changed actor/body/caller output before rejection");

        // Use the actual decoded swamp floor selected from its original BRES
        // scene and the existing floor/PF implementation; no synthetic floor
        // predicate is supplied to reconciliation.
        const auto floorBytes = assets.read("original-cache/data/3d/modules/swamp/swamp.bdae");
        dh2::resources::BresView floorView{};
        check(dh2_bres_open(&floorView, floorBytes.data(), floorBytes.size()) ==
              dh2::resources::BresError::ok, "Original swamp floor BRES is unavailable");
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
        check(floorInstance != UINT32_MAX, "Authored source swamp floor instance is missing");
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
                floorPoint[k] = (triangle.points[0][k] + triangle.points[1][k] +
                                 triangle.points[2][k]) / 3.f;
            float height = floorPoint[2];
            if (dh2::floors::height(*floorWorld, floorPoint.data(), height)) {
                floorPoint[2] = height;
                foundFloor = true;
                break;
            }
        }
        check(foundFloor, "Actual source floor has no selectable point");
        check(bodies.set_position(1, floorPoint, false, error), error);
        auto obstacles = std::make_shared<SourceObstacles>();
        check(bodies.initialize_navigation(1,
              {floorWorld, obstacles, floorWorld.get(), &obstacles->registry}, error), error);
        auto acceptedBody = floorPoint;
        acceptedBody[0] += 2.f;
        mageBody->SetXForm(b2Vec2(acceptedBody[0] * .01f, acceptedBody[1] * .01f),
                           mageBody->GetAngle());
        check(bodies.reconcile_physics_position(1, lookup, true, publication, error), error);
        check(publication.floor_checked && publication.floor_valid && publication.xy_changed &&
              publication.body_reseated &&
              std::fabs(mageActor->transform.position[0] - acceptedBody[0]) < .01f &&
              std::fabs(mageActor->transform.position[1] - acceptedBody[1]) < .01f &&
              mageActor->transform.position[2] == floorPoint[2],
              "Actual source floor did not accept and publish valid native XY while preserving Z");
        const auto acceptedPosition = mageActor->transform.position;

        check(bodies.set_position(1, floorPoint, false, error), error);
        const std::array<float, 3> rejectedOutside{floorPoint[0] + 50000.f,
                                                  floorPoint[1] + 50000.f,
                                                  floorPoint[2]};
        mageBody->SetXForm(b2Vec2(rejectedOutside[0] * .01f, rejectedOutside[1] * .01f),
                           mageBody->GetAngle());
        check(bodies.reconcile_physics_position(1, lookup, true, publication, error), error);
        check(publication.floor_checked && !publication.floor_valid && publication.xy_changed &&
              publication.body_reseated && mageActor->transform.position == publication.position &&
              mageActor->transform.position != rejectedOutside &&
              std::fabs(mageActor->transform.position[0] - acceptedPosition[0]) < .01f &&
              std::fabs(mageActor->transform.position[1] - acceptedPosition[1]) < .01f,
              "Rejected outside-floor point did not publish the source floor/PF clamp point");

        check(bodies.remove_physical(2,error),error);
        stopped=true;
        check(bodies.stop_physical(2,lookup,stopped,error)&&!stopped,
              "Actually absent physical receiver fabricated a Stop");
        check(bodies.clear(error), error);
        foreignWorld.clear();
        nativeWorld->clear();
        session.detach_for_restore();
        std::cout << "PASS same-session Mage/Lizard physics frame, real contacts, once-per-frame Step, "
                     "source sleeping/pin distinction, same-actor position publication, physical Stop and failure-prefix guard\n";
        return 0;
    } catch (const std::exception& failure) {
        std::cerr << failure.what() << '\n';
        return 1;
    }
}
