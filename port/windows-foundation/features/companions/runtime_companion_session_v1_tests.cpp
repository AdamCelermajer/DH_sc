#include "runtime_companion_session_v1.hpp"
#include "runtime_companion_movement_v1.hpp"
#include "runtime_companion_follow_consumer_v1.hpp"
#include "../../original_actor_body_plan.hpp"
#include "../../source_module_floors.hpp"

#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::companions;

namespace {
void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

struct Snapshot {
    float hp, max_hp, mp, max_mp;
    Transform transform;
    ActorId target;
};

Snapshot capture(const ActorState& actor) {
    return {actor.health, actor.max_health, actor.resource, actor.max_resource,
            actor.transform, actor.target_id};
}

bool same(const ActorState& actor, const Snapshot& old) {
    return actor.health == old.hp && actor.max_health == old.max_hp &&
        actor.resource == old.mp && actor.max_resource == old.max_mp &&
        actor.transform.position == old.transform.position &&
        actor.transform.rotation == old.transform.rotation &&
        actor.transform.scale == old.transform.scale && actor.target_id == old.target;
}

struct CompanionSlots {
    std::array<float,3> destination{};
    std::uintptr_t attached = 0, visual = 0;
};

struct StopProbe { std::vector<std::uint32_t> calls; };

int stop_control(void* raw, const dh2::character::CharacterControlRequest32* request,
                 dh2::character::CharacterControlResponse16* response) {
    auto& probe = *static_cast<StopProbe*>(raw);
    if (!request || !response) return -1;
    response->word = 0;
    if (request->service == dh2::character::control_is_remotely_updated) {
        probe.calls.push_back(1);
        return 1;
    }
    if (request->service == dh2::character::control_stop_object) {
        probe.calls.push_back(2);
        return 1; // Explicit source GameObject.Stop service fixture.
    }
    if (request->service == dh2::character::control_character_event && request->argument == 0x3f) {
        probe.calls.push_back(3);
        return 1; // Explicit same-owner Character event0x3f fixture.
    }
    return -1;
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

        CombatSessionConfig config;
        config.diagnosticRngSeed = 0x713u;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        const auto item_bytes = assets.read(config.tableRoot + "/loot_table_pyarray.bin");
        const auto item_names = assets.read(config.tableRoot + "/loot_table_pyarraynames.bin");
        const auto item_fields = assets.read(config.tableRoot + "/loot_table_pystructnames.bin");
        dh2::data::ItemTable items;
        check(dh2::data::load_items({item_bytes.data(), item_bytes.size()},
              {item_names.data(), item_names.size()}, {item_fields.data(), item_fields.size()},
              items, error), error);
        check(items.identifiers.size() > 664, "Original Session player item row unavailable");
        config.mainItemId = items.identifiers[664];
        config.equippedItemIds = {config.mainItemId};

        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan player_plan;
        check(build_original_combat_visual_plan(assets, melee, config.playerProfileId,
              customization, "companion-session-player", player_plan, error), error);
        config.playerVisualConfig = player_plan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile player_policy;
        player_policy.action = {"AttackStatic", 0, {0, 1}};
        player_policy.initialIdle = {"Idle", 0, {0}};
        player_policy.damageMarkerNames = {"attack_mainhand"};
        player_policy.propertyOptions = {256, true};
        config.profiles.emplace(config.playerProfileId, player_policy);

        ActorPopulation population;
        const auto& source = source_companion_follow_records_v1();
        std::vector<RuntimeCompanionSessionActorV1> identities;
        for (std::size_t i = 0; i < source.size(); ++i) {
            const auto& row = source[i];
            OriginalCombatVisualPlan companion_plan;
            ActorCustomization companion_customization;
            companion_customization.allow_missing_animation_targets = true;
            check(build_original_combat_visual_plan(assets, melee, std::string(row.profile_id),
                  companion_customization, "source-companion-session", companion_plan, error), error);
            CombatSessionProfile animation_only;
            animation_only.initialIdle = {"Idle", 0, {0}};
            animation_only.animationOnly = true;
            animation_only.propertyOptions = {std::nullopt, false};
            animation_only.customization.allow_missing_animation_targets = true;
            config.profiles.emplace(std::string(row.profile_id), animation_only);

            PopulationActor placed;
            placed.profileId = std::string(row.profile_id);
            placed.definition.stableId = static_cast<std::uint64_t>(7 + i);
            placed.definition.sourceId = std::string(row.source_object_name);
            placed.definition.placement = {1,0,0,0, 0,1,0,0, 0,0,1,0,
                row.authored_position[0], row.authored_position[1], row.authored_position[2], 1};
            population.actors().push_back(std::move(placed));
            identities.push_back({std::string(row.source_object_name), std::string(row.profile_id),
                std::string(row.ai_script), row.ai_row, row.animation_table,
                static_cast<ActorId>(7 + i)});
        }

        CharacterVisual player_visual;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, player_visual, population,
              {0,0,0}, customization, error), error);

        // The movement probe borrows a real same-Session Faery ActorState and
        // binds its existing body/PF to one authored Swamp floor module. The
        // player is placed on that same test floor as the actual master ActorId.
        const auto floor_bytes = assets.read("original-cache/data/3d/modules/swamp/swamp.bdae");
        dh2::resources::BresView floor_view{};
        check(dh2_bres_open(&floor_view, floor_bytes.data(), floor_bytes.size()) ==
              dh2::resources::BresError::ok, "Actual Swamp floor BRES unavailable");
        dh2::scene::Scene floor_scene;
        check(dh2::scene::load(floor_view, floor_scene, error), error);
        unsigned floor_instance = UINT32_MAX;
        for (unsigned i = 0; i < floor_scene.instances.size(); ++i) {
            const auto node = floor_scene.instances[i].node_index;
            if (node < floor_scene.graph.size() &&
                floor_scene.graph[node].name.find("floor") != std::string::npos) {
                floor_instance = i;
                break;
            }
        }
        check(floor_instance != UINT32_MAX, "Actual Swamp floor instance absent");
        auto floor_world = std::make_shared<dh2::floors::World>();
        OriginalSourceFloorBinding floor_binding;
        floor_binding.instance = floor_instance;
        floor_binding.room = 0;
        floor_binding.mesh_local_quaternion = {0,0,0,1};
        floor_binding.mesh_local_scale = {1,1,1};
        check(append_original_module_floors(floor_view, floor_scene, {floor_binding},
              *floor_world, error), error);
        check(dh2::floors::build_graph(*floor_world, error) &&
              dh2::floors::post_load(*floor_world, error), error);
        std::array<float,3> floor_point{};
        bool found_floor = false;
        for (const auto& triangle : floor_world->records.front()->triangles) {
            for (unsigned k = 0; k < 3; ++k)
                floor_point[k] = (triangle.points[0][k] + triangle.points[1][k] +
                                  triangle.points[2][k]) / 3.0f;
            float height = floor_point[2];
            if (dh2::floors::height(*floor_world, floor_point.data(), height)) {
                floor_point[2] = height;
                found_floor = true;
                break;
            }
        }
        check(found_floor, "Actual Swamp floor has no walkable movement-test point");
        auto* movement_faery = session.actor(8);
        auto* movement_master = session.actor(session.player_id());
        check(movement_faery && movement_master, "Same-Session movement actors missing");
        movement_faery->transform.position = floor_point;
        movement_master->transform.position = {floor_point[0] + 5.0f,
            floor_point[1] + 5.0f, floor_point[2]};

        const auto* faery_properties = session.world()->combat_properties(8);
        check(faery_properties != nullptr, "Same-Session Faery property owner missing");
        OriginalCombatVisualPlan faery_visual_plan;
        ActorCustomization faery_customization;
        faery_customization.allow_missing_animation_targets = true;
        check(build_original_combat_visual_plan(assets, melee, "DefaultFairy",
              faery_customization, "companion-movement-faery", faery_visual_plan, error), error);
        OriginalActorBodyPlanInput body_input;
        body_input.properties = &faery_properties->sheets;
        body_input.ai = session.original_ai_tables();
        body_input.position = {floor_point[0], floor_point[1], floor_point[2]};
        body_input.source_name = "_prim_Faery";
        body_input.owner_identity = 8;
        body_input.visual = faery_visual_plan.config;
        body_input.visual.use_authored_modular_defaults = true;
        OriginalActorBodyPlan faery_body_plan;
        check(make_original_actor_body_plan(assets, body_input, faery_body_plan, error), error);

        auto native_world = std::make_shared<dh2::physical::NativeWorld>();
        const float world_bounds[]{-10000,-10000,10000,10000};
        native_world->load(world_bounds);
        auto session_lease = std::const_pointer_cast<void>(session.actor_binding_lease().lock());
        check(session_lease != nullptr, "Same-Session movement lease absent");
        CompanionSlots faery_slots;
        PlayableActorBodies bodies;
        OriginalActorPhysicalBindings physical;
        physical.actor_lease = session_lease;
        physical.world_lease = native_world;
        physical.data_lease = session_lease;
        physical.readonly_properties = &faery_properties->sheets;
        physical.destination1a8 = faery_slots.destination.data();
        physical.attached2e0 = &faery_slots.attached;
        physical.visual2d8 = &faery_slots.visual;
        physical.world = native_world.get();
        physical.ai = session.original_ai_tables();
        physical.static84 = [](std::uint8_t& out, std::string&) { out = 0; return true; };
        physical.is_player = [](std::int32_t, bool& out, std::string&) { out = false; return true; };
        physical.debug_switch = [](const char*, bool& out, std::string&) { out = false; return true; };
        physical.filter = [](void*, const auto&, const auto&, bool& allowed, std::string&) {
            allowed = true; return true;
        };
        physical.contact = [](dh2::physical::ContactEvent, void*, unsigned, std::string& e) {
            e = "Unexpected contact in companion path-only test"; return false;
        };
        auto& mutable_faery_properties = *const_cast<OriginalCombatProperties*>(faery_properties);
        check(bodies.bind(*movement_faery, mutable_faery_properties, faery_body_plan,
              std::move(physical), error), error);
        struct ObstacleStorage {
            std::array<dh2::navigation::ObstacleEntry, 64> entries{};
            std::array<unsigned, 64> keys{};
            dh2::navigation::ObstacleRegistry registry{entries.data(),0,64,keys.data(),0,64};
        };
        auto obstacles = std::make_shared<ObstacleStorage>();
        check(bodies.initialize_navigation(8, {floor_world, obstacles, floor_world.get(),
              &obstacles->registry}, error), error);
        for (const auto& identity : identities) {
            const auto* actor = session.actor(identity.actor_id);
            const auto* traits = session.world()->traits(identity.actor_id);
            check(actor && actor->definition_id == identity.source_object_name && traits &&
                  !traits->targetable && actor->attack_ids.empty() &&
                  session.owns_population_pose(identity.actor_id) &&
                  session.retained_actor_pose(identity.actor_id),
                  "Actual source companion did not enter this Session as an attackless pose owner");
        }
        const auto priest_before = capture(*session.actor(7));
        const auto faery_before = capture(*session.actor(8));

        RuntimeCompanionSessionV1 rejected_activation;
        check(!rejected_activation.bind(session, identities, false, error) &&
              error.find("RENE_FOLLOW") != std::string::npos,
              "Priest activation must require the exact source RENE_FOLLOW condition");
        RuntimeCompanionSessionV1 ungated_faery;
        const std::vector<RuntimeCompanionSessionActorV1> faery_only{identities.at(1)};
        check(ungated_faery.bind(session, faery_only, false, error), error);
        ActorId ungated_faery_id = invalid_actor_id;
        check(ungated_faery.resolve_actor("_prim_Faery", ungated_faery_id, error) &&
              ungated_faery_id == 8,
              "Faery must remain enrolled without the Priest-only RENE_FOLLOW condition");
        check(!ungated_faery.resolve_actor("_prim_NPC_PriestGood", ungated_faery_id, error),
              "Priest was admitted while its source activation condition was false");

        RuntimeCompanionSessionV1 companions;
        check(companions.bind(session, identities, true, error), error);
        ActorId priest_id = invalid_actor_id, faery_id = invalid_actor_id;
        check(companions.resolve_actor("_prim_NPC_PriestGood", priest_id, error), error);
        check(companions.resolve_actor("_prim_Faery", faery_id, error), error);
        check(priest_id == 7 && faery_id == 8,
              "Authored source object names did not resolve to the existing same-session ActorIds");

        RuntimeCompanionFollowFactsV1 facts;
        facts.has_master = true;
        facts.master_id = session.player_id();
        facts.state_known = true;
        facts.state_is_move = false;
        RuntimeCompanionFollowDecisionV1 decision;
        check(companions.plan_event("_prim_Faery", SourceFollowerEventV1::master_out_of_range,
              facts, decision, error), error);
        check(decision.handled && decision.source_policy_supported && decision.actor_id == faery_id &&
              decision.operation_count == 2 &&
              decision.operations[0] == SourceFollowerOperationV1::move_to_master &&
              decision.arguments[0] == session.player_id() &&
              decision.operations[1] == SourceFollowerOperationV1::clear_target,
              "Actual same-session master did not produce the exact follower MoveTo/ClearTarget plan");
        check(companions.plan_event("_prim_Faery", SourceFollowerEventV1::master_out_of_sight,
              facts, decision, error), error);
        check(decision.operation_count == 1 &&
              decision.operations[0] == SourceFollowerOperationV1::warp_behind_master &&
              decision.arguments[0] == session.player_id(),
              "Source out-of-sight callback did not retain its same-session master ActorId");
        check(companions.plan_event("_prim_Faery", SourceFollowerEventV1::master_in_melee_range,
              facts, decision, error), error);
        check(decision.operation_count == 1 && decision.operations[0] == SourceFollowerOperationV1::stop,
              "Source in-range callback did not preserve the authored Stop decision");

        facts.has_master = false;
        check(!companions.plan_event("_prim_Faery", SourceFollowerEventV1::master_out_of_sight,
              facts, decision, error), "Missing source master was accepted");
        facts.has_master = true;
        facts.master_id = 0xfedcba98u;
        check(!companions.plan_event("_prim_Faery", SourceFollowerEventV1::master_out_of_sight,
              facts, decision, error) && error.find("same CombatSession") != std::string::npos,
              "Stale master ActorId from another/no Session was accepted");
        check(same(*session.actor(7), priest_before) && same(*session.actor(8), faery_before),
              "Companion planning changed live source vitals, target, or transform");

        facts.master_id = session.player_id();
        facts.state_known = true;
        facts.state_is_move = false;
        check(companions.plan_event("_prim_Faery", SourceFollowerEventV1::master_out_of_range,
              facts, decision, error), error);
        RuntimeCompanionMovementV1 movement;
        check(movement.bind(session, bodies, error), error);
        RuntimeCompanionFollowConsumerV1 follow_consumer;
        check(follow_consumer.bind(session, companions, movement, error), error);
        std::vector<dh2::navigation::PathSegment> path_segments(
            std::size_t(floor_world->graph.node_count) + 1);
        std::vector<std::uint32_t> route_ids(path_segments.size());
        dh2::navigation::PathObject path{};
        path.segments = path_segments.data();
        path.capacity = static_cast<std::uint32_t>(path_segments.size());
        dh2::navigation::PathController path_controller{};
        OriginalActorPathBindings actor_path;
        actor_path.controller_lease = floor_world;
        actor_path.controller = &path_controller;
        actor_path.path = &path;
        dh2::navigation::RouteResult route{};
        route.search.path = route_ids.data();
        route.search.path_capacity = static_cast<std::uint32_t>(route_ids.size());
        features::SourcePathCommandBindings path_owner;
        path_owner.floor_lease = floor_world;
        path_owner.floors = floor_world.get();
        path_owner.controller = &actor_path;
        path_owner.result = &route;
        path_owner.source_search_limit = floor_world->graph.node_count + 1;
        std::uint32_t source_searches = 0, source_publications = 0;
        path_owner.search_enabled = [&](std::uint32_t& enabled, std::string&) {
            ++source_searches; enabled = 1; return true;
        };
        path_owner.publish_command = [&](const auto&, std::string&) {
            ++source_publications; return true;
        };
        movement_faery->target_id = session.player_id();
        dh2::character::ControllerCommandState32 gate{
            8, 8, 0, 0, 0, 0}; // Stable same-Session ActorId tokens plus fresh source gate words.
        RuntimeCompanionMovementResultV1 movement_result;
        RuntimeCompanionFollowDecisionV1 missing_master_decision;
        RuntimeCompanionMovementResultV1 missing_master_result;
        facts.has_master = false;
        check(!follow_consumer.execute_event("_prim_Faery",
              SourceFollowerEventV1::master_out_of_sight, facts, gate, false,
              nullptr, nullptr, nullptr, missing_master_decision,
              missing_master_result, error) &&
              error.find("no live Master ActorId") != std::string::npos,
              "Follow consumer accepted a reached callback without its source master");
        facts.has_master = true;
        facts.master_id = 0xfedcba98u;
        check(!follow_consumer.execute_event("_prim_Faery",
              SourceFollowerEventV1::master_out_of_sight, facts, gate, false,
              nullptr, nullptr, nullptr, missing_master_decision,
              missing_master_result, error) &&
              error.find("same CombatSession") != std::string::npos,
              "Follow consumer accepted a stale master ActorId");
        facts.master_id = session.player_id();
        RuntimeCompanionFollowDecisionV1 consumed_decision;
        check(follow_consumer.execute_event("_prim_Faery",
              SourceFollowerEventV1::master_out_of_range, facts, gate, false,
              &path_owner, nullptr, nullptr, consumed_decision,
              movement_result, error), error);
        check(movement_result.admission == OriginalCommandAdmission::admitted &&
              movement_result.command_dispatched && movement_result.path_published &&
              movement_result.path_found && movement_result.target_cleared &&
              consumed_decision.operations == decision.operations &&
              consumed_decision.arguments == decision.arguments &&
              source_searches == 1 && source_publications == 1 &&
              path.target[0] == movement_master->transform.position[0] &&
              path.target[1] == movement_master->transform.position[1] &&
              path.target[2] == movement_master->transform.position[2] &&
              movement_faery->transform.position == floor_point,
              "Source MoveTo did not build the same-session master path then execute ClearTarget without moving outside its owner");

        // Keep the same source master ActorId and reached out-of-range event,
        // but move the real master inside the existing Session. A fresh source
        // decision must publish a path to the updated Session transform.
        movement_faery->target_id = session.player_id();
        movement_master->transform.position[0] += 0.25f;
        movement_master->transform.position[1] += 0.125f;
        const auto updated_master_position = movement_master->transform.position;
        RuntimeCompanionMovementResultV1 moved_master_result;
        RuntimeCompanionFollowDecisionV1 moved_master_decision;
        check(follow_consumer.execute_event("_prim_Faery",
              SourceFollowerEventV1::master_out_of_range, facts, gate, false,
              &path_owner, nullptr, nullptr, moved_master_decision,
              moved_master_result, error), error);
        check(moved_master_result.path_published && moved_master_result.path_found &&
              moved_master_result.target_cleared &&
              moved_master_decision.arguments[0] == session.player_id() &&
              path.target[0] == updated_master_position[0] &&
              path.target[1] == updated_master_position[1] &&
              path.target[2] == updated_master_position[2] && source_searches == 2 &&
              source_publications == 2,
              "Fresh follower event did not resolve and path to the live moved master in the same Session");

        // The generic consumer intentionally leaves Rene's distinct script to
        // its existing source owner even when RENE_FOLLOW admitted its record.
        RuntimeCompanionMovementResultV1 rene_result;
        RuntimeCompanionFollowDecisionV1 rene_decision;
        check(follow_consumer.execute_event("_prim_NPC_PriestGood",
              SourceFollowerEventV1::master_out_of_range, facts, gate, false,
              nullptr, nullptr, nullptr, rene_decision, rene_result, error), error);
        check(!rene_decision.source_policy_supported && !rene_result.command_dispatched,
              "The generic follower consumer must not substitute Faery rules for Rene AI");

        // Preserve the original gate/remote ordering: if admission blocks,
        // or the Character reports remote ownership, PathTo is never reached
        // and therefore no PF/path binding is required for that event.
        movement_faery->target_id = session.player_id();
        auto blocked_gate = gate;
        blocked_gate.global_blocked = 1;
        check(movement.execute(decision, blocked_gate, false, nullptr, nullptr, nullptr,
              movement_result, error), error);
        check(movement_result.admission == OriginalCommandAdmission::blocked &&
              !movement_result.command_dispatched && !movement_result.path_published &&
              movement_result.target_cleared && movement_faery->target_id == invalid_actor_id,
              "Blocked source MoveTo incorrectly required/reached PathTo or skipped follower ClearTarget");

        movement_faery->target_id = session.player_id();
        check(movement.execute(decision, gate, true, nullptr, nullptr, nullptr,
              movement_result, error), error);
        check(movement_result.admission == OriginalCommandAdmission::admitted &&
              movement_result.command_dispatched && movement_result.remote_noop &&
              !movement_result.path_published && movement_result.target_cleared &&
              movement_faery->target_id == invalid_actor_id,
              "Remote-owned source MoveTo incorrectly required/reached PathTo or skipped ClearTarget");

        check(companions.plan_event("_prim_Faery", SourceFollowerEventV1::master_in_melee_range,
              facts, decision, error), error);
        check(follow_consumer.execute_event("_prim_Faery",
              SourceFollowerEventV1::master_in_melee_range, facts, blocked_gate, false,
              nullptr, nullptr, nullptr, consumed_decision, movement_result, error) &&
              movement_result.admission == OriginalCommandAdmission::blocked &&
              !movement_result.stop_dispatched,
              "Blocked source Stop incorrectly required its masked backend services");
        check(!follow_consumer.execute_event("_prim_Faery",
              SourceFollowerEventV1::master_in_melee_range, facts, gate, false,
              nullptr, nullptr, nullptr, consumed_decision, movement_result, error) &&
              error.find("reached same-owner service") != std::string::npos,
              "Admitted source Stop accepted missing complete Character services");
        StopProbe stop_probe;
        const dh2::character::CharacterControlServices16 stop_owner{&stop_probe, stop_control};
        check(follow_consumer.execute_event("_prim_Faery",
              SourceFollowerEventV1::master_in_melee_range, facts, gate, false,
              nullptr, &stop_owner, nullptr, consumed_decision,
              movement_result, error), error);
        check(movement_result.stop_dispatched &&
              stop_probe.calls == std::vector<std::uint32_t>({1,2,3}),
              "Source Stop did not preserve remote-query, complete GameObject.Stop, Character event0x3f order");

        check(companions.plan_event("_prim_Faery", SourceFollowerEventV1::master_out_of_sight,
              facts, decision, error), error);
        const auto before_missing_warp = movement_faery->transform.position;
        check(!movement.execute(decision, gate, false, nullptr, nullptr, nullptr,
              movement_result, error) && error.find("source-produced destination") != std::string::npos &&
              movement_faery->transform.position == before_missing_warp,
              "WarpBehind invented a destination or succeeded without source output");

        auto stale_master_decision = decision;
        stale_master_decision.arguments[0] = 0xfedcba98u;
        check(!movement.execute(stale_master_decision, gate, false, nullptr, nullptr, nullptr,
              movement_result, error) && error.find("live same-Session master") != std::string::npos,
              "Movement consumer accepted a stale master ActorId");

        session.detach_for_restore();
        check(!companions.resolve_actor("_prim_Faery", faery_id, error) &&
              error.find("stale or replaced CombatSession") != std::string::npos,
              "Stale Session owner survived detach/restore boundary");
        check(!movement.execute(decision, gate, false, nullptr, nullptr, nullptr,
              movement_result, error) && error.find("stale or replaced CombatSession") != std::string::npos,
              "Movement consumer survived Session lease teardown");
        check(!follow_consumer.execute_event("_prim_Faery",
              SourceFollowerEventV1::master_out_of_range, facts, gate, false,
              nullptr, nullptr, nullptr, consumed_decision, movement_result, error) &&
              error.find("stale or replaced CombatSession") != std::string::npos,
              "Follow consumer survived Session lease teardown");
        check(bodies.clear(error), error);
        std::cout << "runtime_companion_session_v1 PASS: asset-backed same-Session Faery follow consumer; moved live master re-resolved on fresh event; source gating and Rene separation; exact Stop order; stale/missing master and lease teardown rejected; WarpBehind requires source destination\n";
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    }
}
