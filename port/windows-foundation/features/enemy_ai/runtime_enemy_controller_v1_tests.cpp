#include "runtime_enemy_controller_v1.hpp"
#include "runtime_enemy_navigation_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../actor_profiles.hpp"
#include "../../original_actor_body_plan.hpp"
#include "../../original_actor_navigation.hpp"
#include "../../playable_actor_bodies.hpp"
#include "../../../level-world/native_body.hpp"
#include "../../source_navigation_world_storage.hpp"
#include "runtime_monster_level_policy_v1.hpp"
#include "../../../game-data/level_tables.hpp"
#include "../../../level-world/character_host_context.hpp"
#include "../../../level-world/physical_world.hpp"
#include "../../../scene-materials/scene.hpp"
#include "../../retained_pose_playback.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;

static void check(bool ok, const std::string& error) {
    if (!ok) throw std::runtime_error(error);
}

static std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream input(path,std::ios::binary);
    check(bool(input),"Original source LevelTable file missing: "+path.string());
    return {std::istreambuf_iterator<char>(input),std::istreambuf_iterator<char>()};
}

static void verify_route_release(AssetCatalog& assets, CombatSession& session,
                                 ActorId owner_id, ActorId target_id) {
    std::string error;
    auto* owner = session.actor(owner_id);
    auto* target = session.actor(target_id);
    auto* world = session.world();
    check(owner && target && world && world->combat_properties(owner_id),
          "Route-release fixture lacks the same-session actor/property owners");

    const auto floor_bytes = assets.read("original-cache/data/3d/modules/swamp/swamp.bdae");
    dh2::resources::BresView bres{};
    check(dh2_bres_open(&bres, floor_bytes.data(), floor_bytes.size()) ==
              dh2::resources::BresError::ok,
          "Route-release fixture could not open the authored Swamp floor BRES");
    dh2::scene::Scene floor_scene;
    check(dh2::scene::load(bres, floor_scene, error), error);
    unsigned floor_instance = UINT32_MAX;
    for (unsigned i = 0; i < floor_scene.instances.size(); ++i) {
        const auto& node = floor_scene.graph[floor_scene.instances[i].node_index];
        if (node.name.find("floor") != std::string::npos) { floor_instance = i; break; }
    }
    check(floor_instance != UINT32_MAX,
          "Route-release fixture lacks the authored Swamp floor helper");
    auto floors = std::make_shared<dh2::floors::World>();
    OriginalSourceFloorBinding floor_binding;
    floor_binding.instance = floor_instance;
    floor_binding.room = 0;
    floor_binding.mesh_local_quaternion = {0,0,0,1};
    floor_binding.mesh_local_scale = {1,1,1};
    check(append_original_module_floors(bres, floor_scene, {floor_binding}, *floors, error), error);
    check(dh2::floors::build_graph(*floors, error) && dh2::floors::post_load(*floors, error), error);
    check(floors->graph.edge_count != 0, "Route-release Swamp floor graph has no edges");
    const auto& edge = floors->graph.edges[0];
    const auto& start = floors->graph.nodes[edge.from-1];
    const auto& finish = floors->graph.nodes[edge.to-1];
    owner->transform.position = {start.position[0], start.position[1], start.position[2]};
    target->transform.position = {finish.position[0], finish.position[1], finish.position[2]};
    owner->target_id = target_id;

    auto* properties = world->combat_properties(owner_id);
    const auto* ai_tables = session.original_ai_tables();
    check(ai_tables != nullptr, "Route-release fixture lacks the current original AI table");
    OriginalActorBodyPlanInput plan_input;
    plan_input.visual.model_path = "original-cache/data/3d/characters/lizardman/lizardman.bdae";
    plan_input.visual.use_authored_modular_defaults = true;
    plan_input.properties = &properties->sheets;
    plan_input.ai = ai_tables;
    plan_input.position = {owner->transform.position[0], owner->transform.position[1],
                           owner->transform.position[2]};
    plan_input.source_name = "Swamp_LizadMan_Type1";
    plan_input.owner_identity = owner_id;
    OriginalActorBodyPlan plan;
    check(make_original_actor_body_plan(assets, plan_input, plan, error), error);

    auto body_world = std::make_shared<dh2::physical::NativeWorld>();
    const float body_bounds[]{-100000.f,-100000.f,100000.f,100000.f};
    body_world->load(body_bounds);
    const auto actor_token = session.actor_binding_lease().lock();
    const auto data_token = session.lifetime_lease().lock();
    check(actor_token && data_token, "Route-release fixture could not pin current Session owners");
    const auto actor_lease = std::const_pointer_cast<void>(actor_token);
    std::shared_ptr<const void> data_pin = data_token;
    const auto data_lease = std::const_pointer_cast<void>(data_pin);
    std::array<float,3> destination = owner->transform.position;
    std::uintptr_t attached = 0, visual_slot = 0;
    OriginalActorPhysicalBindings body_services;
    body_services.actor_lease = actor_lease;
    body_services.world_lease = body_world;
    body_services.data_lease = data_lease;
    body_services.identity = owner_id;
    body_services.position160 = owner->transform.position.data();
    body_services.destination1a8 = destination.data();
    body_services.attached2e0 = &attached;
    body_services.visual2d8 = &visual_slot;
    body_services.world = body_world.get();
    body_services.ai = ai_tables;
    body_services.readonly_properties = &properties->sheets;
    body_services.static84 = [](std::uint8_t& value, std::string&) { value = 0; return true; };
    body_services.is_player = [](std::int32_t type, bool& value, std::string&) {
        value = original_actor_source_is_player(type, "Swamp_LizadMan_Type1");
        return true;
    };
    body_services.debug_switch = [](const char* key, bool& value, std::string&) {
        value = key && std::string(key) == "MP_NoPhysics";
        return true;
    };
    body_services.filter = [](void*, const dh2::physical::Filter&,
                              const dh2::physical::Filter&, bool& allowed, std::string&) {
        allowed = false; return true;
    };
    body_services.contact = [](dh2::physical::ContactEvent, void*, unsigned, std::string&) {
        return true;
    };
    PlayableActorBodies bodies;
    check(bodies.bind(*owner, *properties, plan, std::move(body_services), error), error);
    auto navigation_world = std::make_shared<SourceNavigationWorldStorage>(floors, 2);
    check(bodies.initialize_navigation(owner_id,
          {floors, navigation_world, floors.get(), &navigation_world->registry}, error), error);

    CollisionScene current_scene;
    current_scene.floorInstances = 1; // Complete floor-only fixture; no colliders occlude this route.
    RuntimeEnemyNavigationConfigV1 config;
    config.navigation_world = navigation_world;
    config.bodies = &bodies;
    config.scene_collision = &current_scene;
    config.walk_alias = "route_walk";
    config.walk_choice = {"Idle",0,{0}};
    config.idle_alias = "route_idle";
    config.idle_choice = {"Idle",0,{0}};
    RuntimeEnemyNavigationV1 navigation(std::move(config));
    const auto* ai = dh2::data::ai_props(*ai_tables, properties->sheets.resolved[1]);
    check(ai != nullptr, "Route-release fixture source AI row is absent");
    check(!navigation.approach(session, *owner, *target, *ai, error) &&
          error.find("bound initialized source plan") != std::string::npos,
          "Route fixture should reach route construction before its missing locomotion bank fails closed");
    check(navigation.report().paths_built == 1 &&
          navigation.report().routes_with_waypoints_built == 1,
          "Route-release fixture did not build a nonempty route over the same actor PF");
    const bool owns_pose_before = session.owns_pose(owner_id);
    const auto transform = owner->transform;
    const auto action = owner->action;
    const auto target_before = owner->target_id;
    const auto idle_before = navigation.report().idle_selections;

    check(navigation.release_route(session, owner_id, error), error);
    check(navigation.report().routes_released == 1 &&
          navigation.report().idle_selections == idle_before &&
          owner->transform.position == transform.position &&
          owner->transform.rotation == transform.rotation && owner->action == action &&
          owner->target_id == target_before && session.owns_pose(owner_id) == owns_pose_before,
          "Route release selected locomotion or mutated the same actor/pose owner");
    check(!navigation.approach(session, *owner, *target, *ai, error) &&
          error.find("bound initialized source plan") != std::string::npos,
          "Released route rebuild should reach the same explicit locomotion-bank boundary");
    check(navigation.report().paths_built == 2 &&
          navigation.report().routes_with_waypoints_built == 2,
          "Approach reused a route cache after source route release");
    check(navigation.release_route(session, owner_id, error), error);
    check(navigation.report().routes_released == 2,
          "Idempotent route release failed to clear a rebuilt route");
    check(navigation.release_route(session, target_id, error), error);
    check(navigation.report().routes_released == 2,
          "Empty route release incorrectly counted a cached path");
    check(!navigation.release_route(session, invalid_actor_id, error) && !error.empty(),
          "Route release accepted an actor outside the same Session");
}

int main(int argc, char** argv) {
    try {
        check(argc == 3, "Supply original shared asset root and repository root");
        const std::filesystem::path root(argv[2]);
        AssetCatalog assets(argv[1]);
        std::string error;
        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        check(load_original_property_tables(assets, "original-cache/data/pydata", database, error), error);
        check(melee.load(assets, "original-melee-bindings.xml", error), error);

        CombatSessionConfig config;
        config.diagnosticRngSeed = 1234;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan plan;
        check(build_original_combat_visual_plan(assets, melee, config.playerProfileId,
              customization, "enemy-controller-test-player", plan, error), error);
        config.playerVisualConfig = plan.config;
        config.playerVisualConfig.clips.clear();
        const auto* idle = plan.phase("Idle", 0, {0});
        check(idle != nullptr, "Original Knight idle clip missing");
        config.playerVisualConfig.clips = {{"idle", idle->resolvedPath}};
        CombatSessionProfile knight;
        knight.action = {"AttackStatic", 0, {0, 1}};
        knight.initialIdle = {"Idle", 0, {0}};
        knight.damageMarkerNames = {"attack_mainhand"};
        knight.propertyOptions = {256, true};
        config.profiles.emplace(config.playerProfileId, knight);
        CombatSessionProfile lizard;
        lizard.action = {"Attack", 0, {0, 1}};
        lizard.initialIdle = {"Idle", 0, {0}};
        lizard.death = CombatSessionChoice{"Died", 0, {0}};
        lizard.damageMarkerNames = {"attack_mainhand"};
        lizard.propertyOptions = {std::nullopt, true};
        lizard.customization.allow_missing_animation_targets = true;
        config.profiles.emplace("Swamp_LizadMan_Type1", lizard);

        // Feed the actual source monster level policy into the generic actor
        // property owner before CombatSession resolves the same actor row.
        OriginalCombatProperties lizard_source, knight_source;
        check(build_original_combat_properties(database, "Swamp_LizadMan_Type1", {}, {}, {}, lizard_source, error), error);
        check(build_original_combat_properties(database, "KnightPlayerBase", {256, true}, {}, {}, knight_source, error), error);
        const auto character_property = [&](const char* name) {
            const auto found = std::find(database.characters.fields.begin(),database.characters.fields.end(),name);
            check(found!=database.characters.fields.end(),std::string("Source Character property missing: ")+name);
            return static_cast<std::size_t>(found-database.characters.fields.begin());
        };
        const auto level_records=read_file(root/"port/android-native/app/src/main/assets/data/levels_pyarray.bin");
        const auto level_names=read_file(root/"port/android-native/app/src/main/assets/data/levels_pyarraynames.bin");
        const auto level_schema=read_file(root/"port/android-native/app/src/main/assets/data/levels_pystructnames.bin");
        dh2::data::LevelTables level_tables;
        check(dh2::data::load_levels({level_records.data(),level_records.size()},
              {level_names.data(),level_names.size()},{level_schema.data(),level_schema.size()},
              level_tables,error),error);
        const auto swamp=std::find(level_tables.level_names.begin(),level_tables.level_names.end(),"SWAMP");
        check(swamp!=level_tables.level_names.end(),"Actual SWAMP LevelTable row missing");
        const auto level_index=static_cast<std::size_t>(swamp-level_tables.level_names.begin());
        dh2::character::LevelRangeRow24 range{};
        std::memcpy(&range,level_tables.levels[level_index].scalar.words+12,sizeof(range));
        RuntimeMonsterLevelInputsV1 level_input;
        level_input.source_reads_complete=true;
        const auto fixed_integer = [&](std::int32_t raw) {
            check(raw % 256 == 0, "Source fixture expected an exact integer fixed-point property");
            return original_signed256(raw);
        };
        level_input.monster_level_min=fixed_integer(lizard_source.sheets.resolved[character_property("LevelMin")]);
        level_input.monster_level_max=fixed_integer(lizard_source.sheets.resolved[character_property("LevelMax")]);
        level_input.monster_level_offset=fixed_integer(lizard_source.sheets.resolved[character_property("LevelOffset")]);
        level_input.host_player_level=fixed_integer(knight_source.sheets.resolved[character_property("Level")]);
        level_input.host_difficulty=0.0f;
        level_input.current_range_values_returned=true;
        level_input.current_range_min=static_cast<float>(range.minimum[0]);
        level_input.current_range_max=static_cast<float>(range.maximum[0]);
        RuntimeMonsterLevelDecisionV1 level_decision;
        check(resolve_runtime_monster_level_v1(level_input,level_decision,error),error);
        check(level_decision.calls_set_level && level_decision.uses_current_level_range &&
              level_decision.level_passed_to_to_fixed==1.0f,
              "Actual Swamp source monster level policy did not select the expected level");
        const float encoded_level=level_decision.level_passed_to_to_fixed*256.0f;
        check(std::isfinite(encoded_level) && encoded_level>=0.0f &&
              encoded_level<=static_cast<float>(INT32_MAX) && std::floor(encoded_level)==encoded_level,
              "Source integer level did not encode as a safe fixed-point word");
        config.profiles.at("Swamp_LizadMan_Type1").propertyOptions.level_raw=
            static_cast<std::int32_t>(encoded_level);
        ActorProfileLibrary source_profiles;
        check(source_profiles.load(assets, "actor-profiles-v2.xml", error), error);
        ActorPopulation population;
        const std::string actual_lizard_name = "_prim_Monster_3_05_313_356_14_33_13_179_13_38";
        ActorCustomization source_customization;
        source_customization.allow_missing_animation_targets = true;
        source_customization.use_authored_modular_defaults = true;
        check(population.load(assets, "original-cache/data/scene/001_swamp.mlx", source_profiles,
            [&](const ActorDefinition& definition) {
                return definition.name == actual_lizard_name
                    ? PopulationDecision::include : PopulationDecision::exclude;
            },
            [&](const ActorDefinition&, const ActorProfile&) { return source_customization; }, error), error);
        check(population.actors().size() == 1 &&
              population.actors()[0].definition.name == actual_lizard_name &&
              population.actors()[0].definition.properties.at("charpropsname") == "Swamp_LizadMan_Type1" &&
              population.actors()[0].definition.properties.at("ai_state") == "Limbus" &&
              population.actors()[0].definition.properties.at("condition_desc") == "Invalid" &&
              population.actors()[0].definition.properties.find("activate_cond") ==
                  population.actors()[0].definition.properties.end() &&
              population.actors()[0].definition.properties.find("spawn_prob") ==
                  population.actors()[0].definition.properties.end() &&
              population.actors()[0].definition.properties.find("auto_spawn") ==
                  population.actors()[0].definition.properties.end(),
              "Source Swamp lizard declaration/admission facts changed");
        const ActorId enemy_id = static_cast<ActorId>(population.actors()[0].definition.stableId);
        check(enemy_id != invalid_actor_id && enemy_id != 1,
              "Actual lizard source identity conflicts with the player owner");
        CharacterVisual player;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, player, population,
                                 {0, 0, 0}, customization, error), error);
        auto* world = session.world();
        check(world != nullptr && world->actors().size() == 2, "Session did not bind one shared live actor world");
        auto* owner = session.actor(enemy_id);
        auto* target = session.actor(1);
        check(owner && target && owner == world->find_actor(enemy_id) && target == world->find_actor(1),
              "CombatSession and source world do not share actor records");
        auto* owner_props = world->combat_properties(enemy_id);
        auto* target_props = world->combat_properties(1);
        check(owner_props && target_props, "Original combat property rows missing");
        check(owner_props->sheets.resolved[character_property("Level")] ==
                  static_cast<std::int32_t>(encoded_level),
              "Same-session actual lizard properties did not use the source Level policy result");
        check(search_sneak_gate(*owner_props, *target_props),
              "Actual Swamp lizard/player source sneak gate does not admit this test target");
        owner = session.actor(enemy_id);
        const auto* ai = dh2::data::ai_props(*session.original_ai_tables(),
                                             world->combat_properties(enemy_id)->sheets.resolved[1]);
        check(ai && ai->script == "monster", "Fixture is not an original monster AI row");
        const Vec3 source_origin{owner->transform.position[0],owner->transform.position[1],owner->transform.position[2]};
        check(std::abs(source_origin.x+6852.641f)<0.02f &&
              std::abs(source_origin.y-938.285f)<0.02f &&
              std::abs(source_origin.z-250.0f)<0.02f,
              "Population did not preserve the authored Swamp lizard world placement");
        target->transform.position = {source_origin.x+ai->view_radius_no_aggro,
                                      source_origin.y,source_origin.z};

        // The cooldown regression below runs the live controller and the same
        // Session actor through an actual NativeWorld Step. Keep the body
        // fixture to the lizard so there is no invented player collision or
        // spacing policy in this controller-focused repro.
        auto cooldown_world = std::make_shared<dh2::physical::NativeWorld>();
        const float cooldown_bounds[]{-1000.f,-1000.f,1000.f,1000.f};
        cooldown_world->load(cooldown_bounds);
        PlayableActorBodies cooldown_bodies;
        std::array<float,3> cooldown_destination = owner->transform.position;
        std::uintptr_t cooldown_attached = 0, cooldown_visual = 0;
        OriginalActorBodyPlanInput cooldown_plan_input;
        cooldown_plan_input.visual.model_path = "original-cache/data/3d/characters/lizardman/lizardman.bdae";
        cooldown_plan_input.visual.use_authored_modular_defaults = true;
        cooldown_plan_input.properties = &owner_props->sheets;
        cooldown_plan_input.ai = session.original_ai_tables();
        cooldown_plan_input.position = {owner->transform.position[0], owner->transform.position[1],
                                        owner->transform.position[2]};
        cooldown_plan_input.source_name = "Swamp_LizadMan_Type1";
        cooldown_plan_input.owner_identity = enemy_id;
        OriginalActorBodyPlan cooldown_plan;
        check(make_original_actor_body_plan(assets, cooldown_plan_input, cooldown_plan, error), error);
        auto cooldown_actor_lease = std::const_pointer_cast<void>(session.actor_binding_lease().lock());
        auto cooldown_data_token = session.lifetime_lease().lock();
        check(cooldown_actor_lease && cooldown_data_token,
              "Cooldown NativeWorld fixture could not pin current Session owners");
        std::shared_ptr<const void> cooldown_data_pin = cooldown_data_token;
        auto cooldown_data_lease = std::const_pointer_cast<void>(cooldown_data_pin);
        OriginalActorPhysicalBindings cooldown_physical;
        cooldown_physical.actor_lease = cooldown_actor_lease;
        cooldown_physical.world_lease = cooldown_world;
        cooldown_physical.data_lease = cooldown_data_lease;
        cooldown_physical.identity = enemy_id;
        cooldown_physical.position160 = owner->transform.position.data();
        cooldown_physical.destination1a8 = cooldown_destination.data();
        cooldown_physical.attached2e0 = &cooldown_attached;
        cooldown_physical.visual2d8 = &cooldown_visual;
        cooldown_physical.world = cooldown_world.get();
        cooldown_physical.ai = session.original_ai_tables();
        cooldown_physical.readonly_properties = &owner_props->sheets;
        cooldown_physical.static84 = [](std::uint8_t& value, std::string&) { value = 0; return true; };
        cooldown_physical.is_player = [](std::int32_t type, bool& value, std::string&) {
            value = original_actor_source_is_player(type, "Swamp_LizadMan_Type1"); return true;
        };
        cooldown_physical.debug_switch = [](const char*, bool& value, std::string&) { value = false; return true; };
        cooldown_physical.filter = [](void*, const dh2::physical::Filter&,
                                     const dh2::physical::Filter&, bool& allowed, std::string&) {
            allowed = false; return true;
        };
        cooldown_physical.contact = [](dh2::physical::ContactEvent, void*, unsigned, std::string&) {
            return true;
        };
        check(cooldown_bodies.bind(*owner, *owner_props, cooldown_plan,
                                   std::move(cooldown_physical), error), error);
        check(cooldown_bodies.set_pinned(enemy_id, false, error), error);
        check(cooldown_bodies.physical(enemy_id) &&
              cooldown_bodies.physical(enemy_id)->native().body,
              "Cooldown fixture lacks the lizard's real NativeWorld body");
        PlayableActorBodies::CurrentActorLookup cooldown_lookup =
            [&](ActorId id) { return session.actor(id); };
        dh2::physical::NativeBodyObservation cooldown_body_start{};
        check(dh2_native_body_observe(&cooldown_body_start,
              &cooldown_bodies.physical(enemy_id)->native()) == 0,
              "Cooldown fixture could not observe its initial native body");

        unsigned sight_calls = 0, approach_calls = 0, stop_calls = 0;
        bool visible_now = true;
        std::vector<std::string> events;
        RuntimeEnemyControllerServicesV1 services;
        services.can_see = [&](CombatSession& same, const ActorState& source, const ActorState& candidate,
                               const OriginalCombatProperties& source_props,
                               const OriginalCombatProperties& candidate_props,
                               const dh2::data::AiProps& source_ai, bool& visible, std::string&) {
            ++sight_calls;
            check(&same == &session && &source == session.actor(enemy_id) && &candidate == session.actor(1),
                  "Sight callback received a copied actor");
            check(&source_props == world->combat_properties(enemy_id) &&
                  &candidate_props == world->combat_properties(1) && &source_ai == ai,
                  "Sight callback lost canonical original properties");
            visible = visible_now;
            return true;
        };
        services.approach = [&](CombatSession& same, ActorState& source, ActorState& candidate,
                                const dh2::data::AiProps&, std::string&) {
            ++approach_calls;
            check(&same == &session && &source == session.actor(enemy_id) && &candidate == session.actor(1) &&
                  source.target_id == candidate.id, "Approach did not use the same canonical target");
            return true;
        };
        services.stop = [&](CombatSession& same, ActorState& source, std::string&) {
            ++stop_calls;
            events.push_back("stop");
            check(&same == &session && &source == session.actor(enemy_id) && source.target_id == 1,
                  "Stop was not issued on the same live actor before target clear");
            bool stopped = false;
            std::string stop_error;
            check(cooldown_bodies.stop_physical(enemy_id, cooldown_lookup, stopped, stop_error), stop_error);
            check(stopped, "In-melee source Stop did not reach the same lizard body");
            return true;
        };

        RuntimeEnemyControllerV1 controller(std::move(services));
        session.set_actor_decision_provider([&](CombatSession& same, double dt, std::string& e) {
            check(&same == &session, "Decision provider was routed to another session");
            return controller.update(same, dt, e);
        });
        InputActions input;
        check(session.update(0.0, input,
              {source_origin.x+ai->view_radius_no_aggro,source_origin.y,source_origin.z}, 0.0f, error), error);
        check(owner->target_id == invalid_actor_id && sight_calls == 0 &&
              controller.report().acquired_targets == 0,
              "Strict original view-radius boundary admitted a candidate");
        target->transform.position = {source_origin.x,source_origin.y,source_origin.z};
        check(session.update(0.0, input, source_origin, 0.0f, error), error);
        check(sight_calls == 1,
              "Default same-world scan did not consult LOS once for the in-radius candidate");
        check(owner->target_id == target->id && approach_calls == 0 && stop_calls == 1 &&
              events == std::vector<std::string>{"stop"},
              "In-melee acquisition failed to route source Stop before an attack");
        check(!session.owns_pose(enemy_id) && controller.report().acquired_targets == 1,
              "Target acquisition began an attack before the source melee callback boundary");
        check(ai->attack_delay == 800,
              "Actual Swamp monster row no longer supplies the expected 800 ms attack delay");
        check(session.update(0.0, input, source_origin, 0.0f, error), error);
        check(session.owns_pose(enemy_id) && controller.report().attack_requests == 1 &&
              stop_calls == 1 && approach_calls == 0,
              "Same-session in-melee attack did not follow Stop without a walk approach");
        const auto in_melee_attack_position = session.actor(enemy_id)->transform.position;
        std::uint64_t cooldown_native_frame = 0;
        std::uint32_t cooldown_elapsed_ms = 0;
        std::ofstream cooldown_log(root/".local-inputs/runtime-enemy-cooldown-nativeworld.jsonl",
                                   std::ios::binary | std::ios::trunc);
        check(bool(cooldown_log), "Could not open private cooldown body trace");
        bool attack_departed = false, cooldown_restarted = false;
        unsigned cooldown_frames_without_pose = 0;
        for (unsigned frame = 0; frame != 120; ++frame) {
            const bool had_attack_pose = session.owns_pose(enemy_id);
            bool stepped = false;
            check(cooldown_bodies.step_world(*cooldown_world, ++cooldown_native_frame, 50,
                                              cooldown_lookup, stepped, error), error);
            check(stepped, "Cooldown trace did not step the actual NativeWorld");
            // Original Level::Update steps PhysicalWorld before actor updates.
            check(session.update(0.05, input, source_origin, 0.0f, error), error);
            cooldown_elapsed_ms += 50;
            const bool has_attack_pose = session.owns_pose(enemy_id);
            const bool target_temporarily_cleared =
                had_attack_pose && !has_attack_pose && session.actor(enemy_id)->target_id == invalid_actor_id;
            if ((!target_temporarily_cleared && session.actor(enemy_id)->target_id != target->id) || approach_calls != 0 ||
                session.actor(enemy_id)->transform.position != in_melee_attack_position) {
                const auto actual_position = session.actor(enemy_id)->transform.position;
                std::cerr << "cooldown sample frame=" << frame
                          << " hadPose=" << had_attack_pose << " hasPose=" << has_attack_pose
                          << " target=" << session.actor(enemy_id)->target_id
                          << " playerHealth=" << session.actor(1)->health
                          << " approaches=" << approach_calls
                          << " stops=" << stop_calls
                          << " position=" << actual_position[0] << ',' << actual_position[1] << ',' << actual_position[2]
                          << " origin=" << in_melee_attack_position[0] << ',' << in_melee_attack_position[1] << ',' << in_melee_attack_position[2]
                          << '\n';
            }
            dh2::physical::NativeBodyObservation body{};
            check(dh2_native_body_observe(&body, &cooldown_bodies.physical(enemy_id)->native()) == 0,
                  "Cooldown trace could not observe stepped body");
            const auto actor_position = session.actor(enemy_id)->transform.position;
            cooldown_log << "{\"frame\":" << cooldown_native_frame
                         << ",\"elapsed_ms\":" << cooldown_elapsed_ms
                         << ",\"body_xy_m\":[" << body.position[0] << ',' << body.position[1] << "]"
                         << ",\"body_sleeping\":" << body.sleeping
                         << ",\"actor_xyz\":[" << actor_position[0] << ',' << actor_position[1] << ',' << actor_position[2] << "]"
                         << ",\"action\":" << static_cast<unsigned>(session.actor(enemy_id)->action)
                         << ",\"target\":" << session.actor(enemy_id)->target_id
                         << ",\"attack_pose\":" << (has_attack_pose ? "true" : "false")
                         << ",\"approach_calls\":" << approach_calls << "}\n";
            check(bool(cooldown_log), "Writing per-frame cooldown body trace failed");
            check((target_temporarily_cleared || session.actor(enemy_id)->target_id == target->id) && approach_calls == 0 &&
                  session.actor(enemy_id)->transform.position == in_melee_attack_position,
                  "Same-session source attack/cooldown selected approach or moved the stationary source attack");
            if (had_attack_pose && !has_attack_pose) attack_departed = true;
            else if (attack_departed && !has_attack_pose) ++cooldown_frames_without_pose;
            else if (attack_departed && has_attack_pose) cooldown_restarted = true;
            if (cooldown_restarted) break;
        }
        check(attack_departed && cooldown_frames_without_pose >= 14 &&
              cooldown_frames_without_pose <= 18 && cooldown_restarted &&
              session.actor(enemy_id)->target_id == target->id,
              "Actual attack completion did not preserve the source 800 ms departure cooldown before retry");
        dh2::physical::NativeBodyObservation cooldown_body_end{};
        check(dh2_native_body_observe(&cooldown_body_end,
              &cooldown_bodies.physical(enemy_id)->native()) == 0,
              "Cooldown trace could not observe final native body");
        const auto cooldown_actor_end = session.actor(enemy_id)->transform.position;
        check(std::abs(cooldown_body_end.position[0] - cooldown_body_start.position[0]) <= 0.01f &&
              std::abs(cooldown_body_end.position[1] - cooldown_body_start.position[1]) <= 0.01f &&
              cooldown_actor_end == in_melee_attack_position,
              "Stepped native lizard body or same-session ActorState drifted through the source cooldown");
        cooldown_log.flush();
        check(bool(cooldown_log), "Flushing per-frame cooldown body trace failed");
        const Vec3 outside_melee{source_origin.x + ai->melee_radius * 2.0f +
                                     world->target_radius(*target) + 1.0f,
                                 source_origin.y,source_origin.z};
        target->transform.position = {outside_melee.x,outside_melee.y,outside_melee.z};
        check(!world->original_melee_in_range(enemy_id, 1),
              "Fixture failed to leave melee range for the approach transition");
        check(session.update(0.0, input, outside_melee, 0.0f, error), error);
        check(approach_calls == 1 && stop_calls == 2,
              "Out-of-range target did not enter the shared approach state");
        target->transform.position = {source_origin.x,source_origin.y,source_origin.z};
        check(session.update(0.0, input, source_origin, 0.0f, error), error);
        check(stop_calls == 2 && session.owns_pose(enemy_id),
              "Melee reentry replaced an already-owned attack pose or repeated Stop");
        check(session.update(0.0, input, source_origin, 0.0f, error), error);
        check(stop_calls == 2,
              "Approach stop repeated during attack cooldown/pose ownership");
        check(controller.report().unsupported_special_actions >= 1,
              "Missing optional skill/buff behavior was not reported");

        visible_now = false;
        const auto sight_before_tracked_update = sight_calls;
        check(session.update(0.0, input, source_origin, 0.0f, error), error);
        check(stop_calls == 2 && session.actor(enemy_id)->target_id == 1 &&
              sight_calls == sight_before_tracked_update &&
              controller.report().target_sight_losses == 0,
              "Tracked target incorrectly used navigation LOS instead of source AI view-radius retention");
        check(events == std::vector<std::string>{"stop","stop"}, "Tracked target retention unexpectedly stopped locomotion");

        visible_now = true;
        const Vec3 outside_view{source_origin.x+ai->view_radius+1.0f,source_origin.y,source_origin.z};
        session.actor(enemy_id)->target_id = 1;
        target->transform.position = {outside_view.x,outside_view.y,outside_view.z};
        check(session.update(0.0, input, outside_view, 0.0f, error), error);
        check(stop_calls == 3 && session.actor(enemy_id)->target_id == invalid_actor_id &&
              controller.report().target_range_losses == 1,
              "Tracked target beyond authored ViewRadius was retained");

        session.actor(enemy_id)->target_id = 1;
        target->transform.position = {source_origin.x,source_origin.y,source_origin.z};
        apply_actor_damage(*target, target->health);
        check(session.update(0.0, input, source_origin, 0.0f, error), error);
        check(stop_calls == 4 && session.actor(enemy_id)->target_id == invalid_actor_id &&
              controller.report().target_deaths == 1,
              "Dead target did not stop and clear from the same live actor");
        check(events == std::vector<std::string>{"stop","stop","stop","stop"},
              "Source approach/range/death stop ordering changed");

        check(cooldown_bodies.clear(error), error);
        verify_route_release(assets, session, enemy_id, 1);

        std::cout << "runtime enemy controller PASS scanLOS=" << sight_calls
                  << " attackRequests=" << controller.report().attack_requests
                  << " stopEvents=" << stop_calls
                  << " targetDeaths=" << controller.report().target_deaths
                  << " routeRelease=1" << '\n';
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "runtime enemy controller FAIL: " << ex.what() << '\n';
        return 1;
    }
}
