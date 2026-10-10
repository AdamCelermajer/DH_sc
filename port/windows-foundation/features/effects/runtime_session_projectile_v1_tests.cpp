#include "runtime_session_projectile_v1.hpp"
#include "runtime_session_projectile_frame_v1.hpp"
#include "runtime_session_projectile_contacts_v1.hpp"
#include "../../../game-data/items.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../original_actor_body_plan.hpp"
#include "../../original_actor_physical.hpp"
#include "../../playable_actor_bodies.hpp"
#include "../../animation_markers.hpp"
#include "../../../level-world/physical_world.hpp"
#include "../../../level-world/native_body.hpp"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::effects;

namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
}

std::shared_ptr<dh2::android_ui::SourceProcessArraysV101> load_arrays(
    const AssetCatalog& assets) {
    auto tables = std::make_shared<dh2::android_ui::SourceProcessArraysV101>();
    bool complete = false;
    std::string error;
    while (!complete) {
        const auto reader = [&](const std::string& uri,
                                std::vector<std::uint8_t>& bytes,
                                std::string& read_error) {
            if (uri.rfind("data/", 0) != 0) {
                read_error = "unexpected source array URI: " + uri;
                return false;
            }
            try {
                bytes = assets.read(std::filesystem::path("original-cache") / uri);
                return true;
            } catch (const std::exception& e) {
                read_error = e.what();
                return false;
            }
        };
        check(tables->load_stage(reader, complete, error), error);
    }
    return tables;
}

void run(const std::filesystem::path& root) {
    AssetCatalog assets(root);
    std::string error;
    OriginalPropertyDatabase db;
    OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets, "original-cache/data/pydata", db, error), error);
    check(bindings.load(assets, "original-melee-bindings.xml", error), error);
    const auto wand_animation = assets.read(
        "data/3D/characters/prince/animations/skill_dh2_prince_mage_wand_attack.bdae");
    AnimationMarkers wand_markers;
    check(wand_markers.load(wand_animation.data(), wand_animation.size(), 0,
                            10000000, error), error);
    std::string actual_attack_marker;
    std::int32_t actual_attack_marker_time = -1;
    for (const auto& marker : wand_markers.markers()) {
        if (marker.name == "do_skill" || marker.name == "attack_ranged" ||
            marker.name == "attack_mainhand") {
            actual_attack_marker = marker.name;
            actual_attack_marker_time = marker.time_ms;
        }
    }
    check(actual_attack_marker == "do_skill" && actual_attack_marker_time >= 0,
          "Actual Mage wand source clip has no source state-5 attack marker");
    ActorCustomization customization;
    customization.allow_missing_animation_targets = true;
    OriginalCombatVisualPlan visual_plan;
    check(build_original_combat_visual_plan(assets, bindings, "MagePlayerBase",
        customization, "session-projectile-test", visual_plan, error), error);

    CombatSessionConfig config;
    config.diagnosticRngSeed = 23;
    config.playerId = 1;
    config.playerProfileId = "MagePlayerBase";
    config.tableRoot = "original-cache/data/pydata";
    config.playerVisualConfig = visual_plan.config;
    config.playerVisualConfig.motion_node_id = "auto";
    config.playerVisualConfig.consume_root_motion = true;
    config.mainItemId = "Staff01";
    config.equippedItemIds = {"StartingSuitMage", "StartingBootsMage",
                              "StartingGlovesMage", "Staff01"};
    CombatSessionProfile player;
    player.animationOnly = true;
    player.receiveDamage = true;
    player.initialIdle = {"Idle", 0, {0}};
    player.reaction = CombatSessionChoice{"Injured", 0, {0}};
    player.death = CombatSessionChoice{"Died", 0, {0}};
    player.reactionMinimalRandoms = false;
    player.propertyOptions = {256, true};
    config.profiles.emplace("MagePlayerBase", player);
    CombatSessionProfile enemy;
    OriginalAttackSelection enemy_attack;
    enemy_attack.state = "Attack";
    enemy_attack.group_path = {0};
    enemy.sequenceAction = enemy_attack;
    enemy.damageMarkerNames = {"attack_mainhand"};
    enemy.initialIdle = {"Idle", 0, {0}};
    enemy.motionRoot = "auto";
    enemy.customization = customization;
    enemy.propertyOptions = {256, true};
    enemy.customization = customization;
    config.profiles.emplace("Swamp_LizadMan_Type1", enemy);
    ActorPopulation population;
    PopulationActor placed;
    placed.profileId = "Swamp_LizadMan_Type1";
    placed.definition.stableId = 2;
    placed.definition.sourceId = "session-projectile-target";
    placed.definition.placement = {1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    placed.transform = placed.definition.placement;
    population.actors().push_back(std::move(placed));
    CharacterVisual visual;
    auto session = std::make_unique<CombatSession>();
    check(session->initialize(assets, db, bindings, config, visual, population,
                             {0,0,0}, customization, error), error);
    check(session->update(0, {}, Vec3{0,0,0}, 0, error), error);

    auto tables = load_arrays(assets);
    const auto item_bytes = assets.read("original-cache/data/pydata/loot_table_pyarray.bin");
    const auto name_bytes = assets.read("original-cache/data/pydata/loot_table_pyarraynames.bin");
    const auto field_bytes = assets.read("original-cache/data/pydata/loot_table_pystructnames.bin");
    const auto bytes = [](const auto& v) { return dh2::data::Bytes{v.data(), v.size()}; };
    dh2::data::ItemTable item_table;
    check(dh2::data::load_items(bytes(item_bytes), bytes(name_bytes), bytes(field_bytes),
                                item_table, error), error);
    const auto staff_id = dh2::data::item_id(item_table, "Staff01");
    check(staff_id >= 0, "Actual Staff01 item row missing");
    std::vector<dh2::character::CombatItemRecord164> item_rows(item_table.rows.size());
    for (std::size_t i = 0; i < item_table.rows.size(); ++i)
        std::copy_n(item_table.rows[i].record.words, 41, item_rows[i].words);
    const dh2::character::CombatItemInstance4 staff{staff_id};
    const dh2::character::CombatItemInstance4* staff_ref = &staff;
    const dh2::character::CombatEquipSet8 equipment_set{&staff_ref};
    const dh2::character::CombatInventory16 inventory{&equipment_set, 1, 0};
    dh2::character::CombatProperties896 properties{};
    std::copy(session->world()->combat_properties(1)->sheets.resolved.begin(),
              session->world()->combat_properties(1)->sheets.resolved.end(),
              properties.words);

    unsigned enemy_queries = 0, owner_hit_calls = 0, semantic_calls = 0;
    bool owner_hit_provider_ready = false;
    std::vector<std::string> hit_order;
    RuntimeSessionProjectileServicesV1 services;
    services.source_is_enemy = [&](ActorId owner, ActorId peer, bool& has_ai,
                                   bool& enemy_result, std::string&) {
        ++enemy_queries;
        check(owner == 1 && peer == 2, "Enemy query received wrong Session actor IDs");
        has_ai = true;
        enemy_result = true;
        return true;
    };
    services.source_owner_on_projectile_hit =
        [&](const RuntimeSessionProjectileImpactCandidateV1& c,
            std::string&) {
            ++owner_hit_calls;
            hit_order.push_back("owner-on-projectile-hit");
            check(c.owner == 1 && c.target == 2 && c.source_row == 20 &&
                  c.source_marker == "do_skill" && c.projectile_id != 0,
                  "Source owner callback lost the actual projectile/peer occurrence");
            if (!owner_hit_provider_ready) return false;
            return true;
        };
    services.source_hit_semantics = [&](const RuntimeSessionProjectileImpactCandidateV1& c,
                                        CombatSessionSourceHit& hit,
                                        std::string& semantic_error) {
        ++semantic_calls;
        hit_order.push_back("f-melee-attack");
        check(c.owner == 1 && c.target == 2 && c.source_row == 20 &&
              c.source_marker == "do_skill" && c.source_critical,
              "Deferred source hit lost launch identity/row critical flag");
        return runtime_session_projectile_melee_hit_v1(
            tables, c, &inventory, item_rows.data(),
            static_cast<std::uint32_t>(item_rows.size()), hit, semantic_error);
    };
    RuntimeSessionProjectileV1 runtime(*session, std::move(services));
    check(runtime.synchronize_session_binding(error), error);
    RuntimeSessionProjectileLaunchV1 request;
    request.owner = 1;
    request.occurrence = 7;
    request.event = {actual_attack_marker, "skill_dh2_prince_mage_wand_attack", 0,
                     static_cast<std::uint32_t>(actual_attack_marker_time), 41, 0};
    request.source_event = {5, 0, 0, 1, 20};
    request.current_properties = &properties;
    request.current_inventory = &inventory;
    request.item_rows = item_rows.data();
    request.item_count = static_cast<std::uint32_t>(item_rows.size());
    request.pose = {1, {10.0f, 20.0f, 30.0f}, {3.0f, 4.0f, 12.0f}};
    bool source_can_range = false;
    RuntimeSourceRangeSelectionV1 source_selection;
    check(runtime_source_range_selection_v1(tables, properties, &inventory,
        item_rows.data(), static_cast<std::uint32_t>(item_rows.size()),
        source_can_range, source_selection, error), error);
    check(source_can_range && source_selection.selected_item == staff_id &&
          source_selection.projectile.row == 20 &&
          source_selection.projectile.velocity == 20.0f &&
          source_selection.projectile.timer_ms == 1000 &&
          source_selection.projectile.max_distance == -1.0f,
          "Live Mage Staff01 did not select exact source row20 motion/timer values");
    // This actual source row disables range expiry with -1; the timer branch
    // is the reachable range/lifetime gate for this staff fixture.
    bool accepted = false;
    std::uint64_t id = 0;
    check(runtime.launch_state5(tables, request, accepted, id, error), error);
    check(accepted && id != 0 && runtime.active_count() == 1,
          "Actual admitted Staff01 state-5 event did not create a record");
    const auto visual_packet = runtime.visuals();
    check(visual_packet.size() == 1 && visual_packet[0].source_row == 20 &&
          visual_packet[0].source_model == 1 &&
          visual_packet[0].model_uri == "data/3D/projectiles/elemental_bolt_fire.bdae" &&
          visual_packet[0].position == request.pose.origin,
          "Actual FireWandProjectile row/model visual packet missing");
    const auto original_lease = session->actor_binding_lease();
    std::uint64_t duplicate_id = 0;
    check(runtime.launch_state5(tables, request, accepted, duplicate_id, error), error);
    check(!accepted && duplicate_id == id && runtime.active_count() == 1,
          "Repeated retained marker spawned another projectile");

    // The source is state-5-only: a state-6 do_skill is not admitted by this
    // feature, even though source skill callbacks have their own caller.
    auto wrong_state = request;
    wrong_state.occurrence++;
    wrong_state.event.generation++;
    wrong_state.source_event.state = 6;
    check(runtime.launch_state5(tables, wrong_state, accepted, duplicate_id, error), error);
    check(!accepted && runtime.active_count() == 1,
          "State-6 do_skill incorrectly launched a state-5 projectile");

    const auto before = runtime.visuals().front().position;
    check(runtime.update(1, 100, true, error), error);
    check(runtime.visuals().front().position == before && runtime.active_count() == 1,
          "Paused source frame moved or expired projectile");

    // Match the recovered Level frame: PhysicalWorld::update runs first, then
    // ObjectManager/Projectile::Update writes velocity for the next Step.
    dh2::physical::NativeWorld source_world;
    const float world_bounds[]{-10.0f, -10.0f, 10.0f, 10.0f};
    source_world.load(world_bounds);
    b2BodyDef body_definition;
    body_definition.position.Set(request.pose.origin[0] * 0.01f,
                                 request.pose.origin[1] * 0.01f);
    auto* source_body = source_world.create(&body_definition);
    check(source_body != nullptr, "Native source-unit body creation failed");
    b2CircleDef circle;
    circle.radius = 0.05f;
    circle.density = 1.0f;
    source_body->CreateShape(&circle);
    source_body->SetMassFromShapes();
    dh2::physical::NativeBody native_body{source_body, 0.05f, 0};
    const float zero_velocity[]{0.0f, 0.0f};
    check(dh2_native_body_set_linear(&native_body, zero_velocity) == 0,
          "Native source-unit initial velocity failed");
    source_world.update(16);
    check(runtime.update(2, 16, false, error), error);
    float observed[4]{};
    check(dh2_native_body_query(observed, &native_body) == 0,
          "Native source-unit body query failed");
    check(runtime.visuals().front().position == before &&
          std::abs(observed[0] - before[0]) < 0.001f &&
          std::abs(observed[1] - before[1]) < 0.001f,
          "Projectile moved before its first post-Step velocity write");
    const float source_velocity[]{source_selection.projectile.velocity *
                                      runtime.visuals().front().forward[0],
                                  source_selection.projectile.velocity *
                                      runtime.visuals().front().forward[1]};
    check(dh2_native_body_set_linear(&native_body, source_velocity) == 0,
          "Native source-unit velocity write failed");
    source_world.update(16);
    check(runtime.update(3, 16, false, error), error);
    const auto after = runtime.visuals().front().position;
    check(dh2_native_body_query(observed, &native_body) == 0,
          "Native source-unit body query after Step failed");
    const auto normalized_forward = runtime.visuals().front().forward;
    check(std::abs((after[0] - before[0]) -
              32.0f * normalized_forward[0]) < 0.001f &&
          std::abs((after[1] - before[1]) -
              32.0f * normalized_forward[1]) < 0.001f &&
          after[2] == request.pose.origin[2] &&
          std::abs(observed[0] - after[0]) < 0.001f &&
          std::abs(observed[1] - after[1]) < 0.001f && runtime.active_count() == 1,
          "Projectile XY displacement differs from real NativeBody units/cadence");

    bool contact_accepted = false;
    check(runtime.contact({id, 2, after, true},
                          contact_accepted, error), error);
    check(contact_accepted && enemy_queries == 1 && semantic_calls == 0 &&
          runtime.take_results().empty(),
          "Contact applied a result before the following source update");
    const auto* attacker_combat = session->world()->combat_properties(1);
    const auto* defender_combat = session->world()->combat_properties(2);
    check(attacker_combat && defender_combat,
          "Actual same-Session combat sheets unavailable for differential");
    const auto combat_view = [](const OriginalCombatProperties& p) {
        const auto& f = p.facts;
        return dh2::data::CombatantView{p.sheets.resolved.data(),
            f.main_damage_class, f.off_damage_class, unsigned(f.two_hander),
            unsigned(f.dual_wield), unsigned(f.shield), f.original_state,
            f.combo_hits};
    };
    const auto direct_attacker = combat_view(*attacker_combat);
    const auto direct_defender = combat_view(*defender_combat);
    auto direct_rng = session->world()->random_state();
    dh2::data::CombatResult direct_result;
    check(dh2_combat_melee(&direct_result, &direct_attacker, &direct_defender,
          &direct_rng, 0, unsigned(source_selection.projectile.magic)) == 0,
          "Original source F_MeleeAttack helper rejected actual Mage rows");
    check(direct_result.mask == 0x0005554au &&
          direct_result.weapon_category == item_rows[staff_id].words[37] &&
          direct_result.element == -1 && direct_result.amount >= 0,
          "Direct source F_MeleeAttack request differs from decoded row/equipment");
    const auto rng_before_missing_owner = session->world()->random_state();
    check(!runtime.update(4, 0, false, error) &&
          error.find("OnProjectileHit") != std::string::npos && semantic_calls == 0 &&
          session->world()->pending_resolutions().empty() &&
          session->world()->random_state().seed == rng_before_missing_owner.seed &&
          session->world()->random_state().calls == rng_before_missing_owner.calls,
          "Missing required source owner callback crossed the F_MeleeAttack prefix");
    owner_hit_provider_ready = true;
    owner_hit_calls = 0;
    hit_order.clear();
    check(runtime.update(5, 0, false, error), error);
    const auto receipt = runtime.take_results();
    const auto impacts = runtime.take_impacts();
    const auto& same_session_calculations = session->world()->pending_resolutions();
    check(same_session_calculations.size() == 1,
          "Projectile did not enter the actual same-Session result owner exactly once");
    const auto& session_result = same_session_calculations.back().melee.original;
    check(owner_hit_calls == 1 && semantic_calls == 1 &&
          hit_order == std::vector<std::string>({"owner-on-projectile-hit",
                                                 "f-melee-attack"}) &&
          receipt.size() == 1 && receipt[0].applied &&
          receipt[0].target == 2 && receipt[0].source_mask &&
          *receipt[0].source_mask == direct_result.mask &&
          receipt[0].source_outcomes &&
          *receipt[0].source_outcomes == direct_result.outcomes &&
          session_result.amount == direct_result.amount &&
          session_result.dot_element == direct_result.dot_element &&
          session_result.dot_duration == direct_result.dot_duration &&
          session_result.dot_amount == direct_result.dot_amount &&
          session_result.hp_leech == direct_result.hp_leech &&
          session_result.mp_leech == direct_result.mp_leech &&
          session_result.outcomes == direct_result.outcomes &&
          session_result.mask == direct_result.mask &&
          session_result.weapon_category == direct_result.weapon_category &&
          session_result.element == direct_result.element &&
          std::abs(receipt[0].requested_damage -
                   static_cast<float>(direct_result.amount) / 256.0f) < 0.001f &&
          impacts.size() == 1 && impacts[0].result &&
          impacts[0].result->applied &&
          impacts[0].source_calculation &&
          impacts[0].source_calculation->original.amount == direct_result.amount &&
          impacts[0].source_calculation->original.outcomes == direct_result.outcomes &&
          impacts[0].source_calculation->original.mask == direct_result.mask &&
          impacts[0].source_calculation->original.weapon_category ==
              direct_result.weapon_category &&
          impacts[0].source_calculation->original.element == direct_result.element &&
          !impacts[0].source_apply_result_flag &&
          session->world()->random_state().seed == direct_rng.seed &&
          session->world()->random_state().calls == direct_rng.calls &&
          runtime.active_count() == ((*receipt[0].source_outcomes & 3u) ? 1u : 0u),
          "Deferred projectile result differs from direct original F_MeleeAttack/session RNG");

    // A fresh launch expires only after source timer dt is applied, then emits
    // the expiry packet without fabricating a target/result.
    request.occurrence = 8;
    request.event.generation = 42;
    check(runtime.launch_state5(tables, request, accepted, id, error), error);
    check(accepted, "Fresh timer-expiry occurrence was not launched");
    float expected_x = request.pose.origin[0];
    float expected_y = request.pose.origin[1];
    float expected_speed = source_selection.projectile.velocity;
    float prior_physics_velocity = 0.0f;
    for (std::uint64_t tick = 0; tick < 63; ++tick) {
        expected_x += prior_physics_velocity * normalized_forward[0] *
                      100.0f * 0.016f;
        expected_y += prior_physics_velocity * normalized_forward[1] *
                      100.0f * 0.016f;
        prior_physics_velocity = expected_speed;
        expected_speed -= source_selection.projectile.velocity_damping * 0.016f;
        check(runtime.update(6 + tick, 16, false, error), error);
    }
    const auto expiry = runtime.take_impacts();
    check(runtime.active_count() == 0 && expiry.size() == 1 && expiry[0].kind == 1 &&
          expiry[0].target == invalid_actor_id && !expiry[0].result,
          "Staff01 row20 timer expiry did not remain a non-hit impact");
    check(std::abs(expiry[0].position[0] - expected_x) < 0.01f &&
          std::abs(expiry[0].position[1] - expected_y) < 0.01f &&
          expiry[0].position[2] == request.pose.origin[2],
          "Source-cadence movement/damping did not precede row20 timer expiry");

    bool fail_material = false;
    unsigned render_submissions = 0;
    std::shared_ptr<const RuntimeSessionProjectileRenderFrameV1> submitted_render_frame;
    RuntimeSessionProjectileRenderServicesV1 render_services;
    render_services.material = [&](const RuntimeSessionProjectileMaterialRequestV1& material,
                                   Material& output, std::string& material_error) {
        if (fail_material) {
            material_error = "test source material/pass refusal";
            return false;
        }
        check(material.resource_uri ==
                  "data/3D/projectiles/elemental_bolt_fire.bdae" &&
              material.authored_material && material.source_material &&
              material.source_image,
              "Frame consumer renderer lost actual projectile material identity");
        output.texture = 1; // Adapter fixture; no live GPU/upload claim.
        output.sourcePass = SourceMaterialPass{};
        material_error.clear();
        return true;
    };
    render_services.submit = [&](std::shared_ptr<const RuntimeSessionProjectileRenderFrameV1> frame,
                                 std::string& submit_error) {
        submitted_render_frame = std::move(frame);
        ++render_submissions;
        submit_error.clear();
        return true;
    };
    std::unique_ptr<RuntimeSessionProjectileRenderV1> renderer;
    check(RuntimeSessionProjectileRenderV1::load(assets, std::move(render_services),
          renderer, error), error);

    // Binding renewal invalidates every outstanding record and old marker key.
    request.occurrence = 9;
    request.event.generation = 43;
    check(runtime.launch_state5(tables, request, accepted, id, error) && accepted,
          error);
    session->detach_for_restore();
    RuntimeSessionProjectileFrameOutputV1 frame_output;
    check(original_lease.expired() &&
          !runtime_session_projectile_frame_v1(runtime, *renderer, 68, 16, false,
                                               frame_output, error) &&
          render_submissions == 0 && runtime.active_count() == 0,
          "Detached Session retained a stale projectile or lease");
    check(session->rebind_after_restore(error), error);
    check(runtime.synchronize_session_binding(error), error);
    check(runtime.contact({id, 2, {10.0f, 20.0f, 0.0f}, true},
                          contact_accepted, error) && !contact_accepted,
          "Old projectile identity survived a Session rebind");
    request.occurrence = 10;
    request.event.generation = 44;
    check(runtime.launch_state5(tables, request, accepted, duplicate_id, error) &&
          accepted && duplicate_id != id,
          "Current Session binding could not accept a fresh source occurrence");

    const auto frame_start_position = runtime.visuals().front().position;
    check(runtime_session_projectile_frame_v1(runtime, *renderer, 68, 16, true,
                                              frame_output, error), error);
    check(frame_output.updated && frame_output.submitted &&
          frame_output.impacts.empty() && frame_output.results.empty() &&
          frame_output.visuals.size() == 1 &&
          frame_output.visuals.front().position == frame_start_position &&
          render_submissions == 1 && submitted_render_frame &&
          submitted_render_frame->draws.size() == 1 &&
          submitted_render_frame->draws.front().projectile_id == duplicate_id &&
          submitted_render_frame->draws.front().mesh_lease,
          "Paused frame inferred impact or failed to retain the current draw packet");

    fail_material = true;
    check(!runtime_session_projectile_frame_v1(runtime, *renderer, 69, 0, false,
                                               frame_output, error) &&
          frame_output.updated && !frame_output.submitted &&
          frame_output.visuals.size() == 1 && frame_output.impacts.empty() &&
          frame_output.results.empty() && render_submissions == 1,
          "Material failure published a packet or discarded its frame result values");
    fail_material = false;
    RuntimeSessionProjectileVisualV1 unsupported_visual;
    unsupported_visual.projectile_id = 999;
    unsupported_visual.owner = 1;
    unsupported_visual.source_row = 20;
    unsupported_visual.source_model = 999;
    unsupported_visual.model_uri = "data/3D/projectiles/unclassified-source.bdae";
    check(!renderer->submit({unsupported_visual}, error) &&
          !error.empty() && render_submissions == 1,
          "Unknown source projectile URI crossed the existing renderer submit gate");

    check(runtime.contact({duplicate_id, 2, frame_start_position, true},
                          contact_accepted, error) && contact_accepted,
          "Frame consumer fixture could not queue an actual typed contact");
    check(runtime_session_projectile_frame_v1(runtime, *renderer, 70, 0, false,
                                              frame_output, error), error);
    check(frame_output.updated && frame_output.submitted &&
          frame_output.results.size() == 1 && frame_output.impacts.size() == 1 &&
          frame_output.impacts.front().result &&
          frame_output.impacts.front().result->applied &&
          render_submissions == 2,
          "Frame consumer did not drain the reached contact result before render submission");
    check(runtime_session_projectile_frame_v1(runtime, *renderer, 71, 0, false,
                                              frame_output, error), error);
    check(frame_output.updated && frame_output.submitted &&
          frame_output.results.empty() && frame_output.impacts.empty() &&
          render_submissions == 3,
          "Frame consumer replayed an already consumed result/impact");

    // A real projectile sensor in the same NativeWorld as the registered
    // Session actor. The exact source BDAE AABB and native filter are checked
    // before the actual Box2D overlap is delivered through the actor resolver.
    auto* live_target = session->actor(2);
    const auto* live_target_properties = session->world()->combat_properties(2);
    check(live_target && live_target_properties,
          "Same-Session projectile overlap target is unavailable");
    auto ai_tables = std::make_shared<dh2::data::AiTables>();
    check(load_original_ai_tables(assets, "original-cache/data/pydata/",
                                  *ai_tables, error), error);
    auto physical_properties = std::make_shared<OriginalCombatProperties>(
        *live_target_properties);
    OriginalActorBodyPlanInput body_input;
    body_input.properties = &physical_properties->sheets;
    body_input.ai = ai_tables.get();
    body_input.owner_identity = 2;
    body_input.position = {live_target->transform.position[0],
                           live_target->transform.position[1],
                           live_target->transform.position[2]};
    body_input.source_name = "Swamp_LizadMan_Type1";
    body_input.visual.model_path =
        "original-cache/data/3d/characters/lizardman/lizardman.bdae";
    OriginalActorBodyPlan target_plan;
    check(make_original_actor_body_plan(assets, body_input, target_plan, error), error);
    auto contact_world = std::make_shared<dh2::physical::NativeWorld>();
    const float contact_world_bounds[]{-1000, -1000, 1000, 1000};
    contact_world->load(contact_world_bounds);
    PlayableActorBodies actor_bodies;
    std::array<float, 3> target_destination = live_target->transform.position;
    std::uintptr_t target_attached = 0, target_visual = 0;
    OriginalActorPhysicalBindings physical_bindings;
    physical_bindings.actor_lease = std::make_shared<int>(1);
    physical_bindings.world_lease = contact_world;
    physical_bindings.data_lease = physical_properties;
    physical_bindings.identity = 2;
    physical_bindings.position160 = live_target->transform.position.data();
    physical_bindings.destination1a8 = target_destination.data();
    physical_bindings.attached2e0 = &target_attached;
    physical_bindings.visual2d8 = &target_visual;
    physical_bindings.world = contact_world.get();
    physical_bindings.ai = ai_tables.get();
    physical_bindings.readonly_properties = &physical_properties->sheets;
    physical_bindings.static84 = [](std::uint8_t& out, std::string&) {
        out = 0; return true;
    };
    physical_bindings.is_player = [](std::int32_t type, bool& out, std::string&) {
        out = original_actor_source_is_player(type, "Swamp_LizadMan_Type1");
        return true;
    };
    physical_bindings.debug_switch = [](const char*, bool& out, std::string&) {
        out = false; return true;
    };
    physical_bindings.filter = [](void*, const auto&, const auto&, bool& out,
                                  std::string&) { out = true; return true; };
    physical_bindings.contact = [](auto, void*, unsigned, std::string&) {
        return true;
    };
    check(actor_bodies.bind(*live_target, *physical_properties, target_plan,
                            physical_bindings, error), error);

    request.occurrence = 12;
    request.event.generation = 46;
    request.pose.origin = live_target->transform.position;
    check(runtime.launch_state5(tables, request, accepted, id, error) && accepted,
          error);
    const auto current_projectiles = runtime.visuals();
    const auto body_visual = std::find_if(current_projectiles.begin(),
        current_projectiles.end(), [&](const auto& value) {
            return value.projectile_id == id;
        });
    check(body_visual != current_projectiles.end(),
          "New source projectile visual is absent before contact registration");
    RuntimeSessionProjectileBodyPlanV1 projectile_plan, no_collision_plan;
    check(runtime_session_projectile_body_plan_v1(*renderer, *body_visual, false,
                                                   projectile_plan, error), error);
    check(runtime_session_projectile_body_plan_v1(*renderer, *body_visual, true,
                                                   no_collision_plan, error), error);
    check(projectile_plan.radius ==
              0.005f * std::max(projectile_plan.source_bounds_max[0] -
                                    projectile_plan.source_bounds_min[0],
                                projectile_plan.source_bounds_max[1] -
                                    projectile_plan.source_bounds_min[1]) &&
          projectile_plan.physics_position[0] == body_visual->position[0] * 0.01f &&
          projectile_plan.physics_position[1] == body_visual->position[1] * 0.01f &&
          projectile_plan.category_bits == 32 && projectile_plan.mask_bits == 0x51f &&
          projectile_plan.group_index == 0 && no_collision_plan.group_index == -666,
          "Projectile body plan differs from authored BDAE AABB/source filters");
    unsigned no_collision_queries = 0;
    unsigned peer_resolution_calls = 0;
    RuntimeSessionProjectileContactServicesV1 contact_services;
    contact_services.resolve_peer_actor = [&](void* context, ActorId& actor,
        bool& recognized, std::string& resolve_error) {
        ++peer_resolution_calls;
        recognized = false;
        if (!actor_bodies.resolve_physical_actor(context, actor, resolve_error)) {
            std::cerr << "peer context not found: " << context << " expected "
                      << actor_bodies.physical(2) << " err=" << resolve_error << "\n";
            resolve_error.clear();
            return true;
        }
        recognized = true;
        return true;
    };
    contact_services.peer_visible = [&](ActorId actor, bool& visible,
                                        std::string&) {
        visible = actor == 2 && session->actor(actor) != nullptr;
        return true;
    };
    contact_services.source_no_collisions = [&](bool& disabled, std::string&) {
        ++no_collision_queries; disabled = false; return true;
    };
    RuntimeSessionProjectileContactBodiesV1 projectile_bodies(
        *session, runtime, *contact_world, std::move(contact_services));
    std::uintptr_t source_identity = 0;
    check(projectile_bodies.register_projectile(id, *renderer, source_identity,
                                                 error), error);
    std::uint64_t identity_id = 0;
    check(source_identity != 0 &&
          projectile_bodies.resolve_source_identity(source_identity, identity_id,
                                                     error) && identity_id == id &&
          no_collision_queries == 1,
          "Registered projectile source identity lost its exact runtime ID");
    b2Body* projectile_body = nullptr;
    for (auto* body = contact_world->backend()->GetBodyList(); body;
         body = body->GetNext()) {
        auto* shape = body->GetShapeList();
        if (shape && shape->GetFilterData().categoryBits == 32 &&
            shape->GetFilterData().maskBits == 0x51f) {
            projectile_body = body;
            check(shape->IsSensor() && body->IsBullet() &&
                  shape->GetFilterData().groupIndex == projectile_plan.group_index &&
                  std::abs(static_cast<b2CircleShape*>(shape)->GetRadius() -
                           projectile_plan.radius) < 1e-6f,
                  "Native projectile body lost its source sensor/radius/bullet fields");
            break;
        }
    }
    check(projectile_body != nullptr,
          "Source projectile sensor was not registered in the Session NativeWorld");
    bool stepped = false;
    const auto actor_lookup = [&](ActorId actor) { return session->actor(actor); };
    check(actor_bodies.step_world(*contact_world, 1, 16, actor_lookup,
                                  stepped, error), error);
    check(stepped, "Same-session NativeWorld did not execute its source frame Step");
    check(projectile_bodies.after_world_step(1, error), error);
    check(peer_resolution_calls == 1,
          "Actual overlapping projectile did not resolve exactly one registered Session body");
    check(runtime.update(1000, 0, false, error), error);
    check(projectile_bodies.after_runtime_update(error), error);
    const auto native_contact_receipt = runtime.take_results();
    check(native_contact_receipt.size() == 1 &&
          native_contact_receipt.front().target == 2,
          "Actual NativeWorld overlap did not reach the same Session result owner");
    const auto post_contact_visuals = runtime.visuals();
    const bool projectile_still_live = std::any_of(post_contact_visuals.begin(),
        post_contact_visuals.end(), [&](const auto& value) {
            return value.projectile_id == id;
        });
    if (projectile_still_live) {
        check(projectile_bodies.size() == 1,
              "Continuing source projectile lost its native sensor binding");
        check(projectile_bodies.unregister_projectile(id, error), error);
    }
    check(projectile_bodies.size() == 0 &&
          !projectile_bodies.resolve_source_identity(source_identity,
                                                      identity_id, error),
          "Projectile sensor removal retained a stale source identity");
    check(actor_bodies.clear(error), error);

    request.occurrence = 13;
    request.event.generation = 47;
    check(runtime.launch_state5(tables, request, accepted, id, error) && accepted,
          "Fresh projectile was not launched for actor-binding renewal test");
    const auto renewal_visuals = runtime.visuals();
    const auto renewal_visual = std::find_if(renewal_visuals.begin(),
        renewal_visuals.end(), [&](const auto& value) {
            return value.projectile_id == id;
        });
    check(renewal_visual != renewal_visuals.end(),
          "Binding-renewal projectile visual is absent");
    std::uintptr_t stale_identity = 0;
    check(projectile_bodies.register_projectile(id, *renderer, stale_identity,
                                                 error), error);
    check(projectile_bodies.size() == 1 && stale_identity != 0,
          "Binding-renewal projectile did not own a native sensor");
    check(runtime.update(1001, 0, false, error), error);
    check(projectile_bodies.after_runtime_update(error), error);
    contact_world->update(16);
    check(projectile_bodies.after_world_step(2, error), error);
    const auto stepped_visuals = runtime.visuals();
    const auto stepped_visual = std::find_if(stepped_visuals.begin(),
        stepped_visuals.end(), [&](const auto& value) {
            return value.projectile_id == id;
        });
    check(stepped_visual != stepped_visuals.end() &&
          std::abs((stepped_visual->position[0] - request.pose.origin[0]) -
                   20.0f * 100.0f * 0.016f * stepped_visual->forward[0]) < 0.01f &&
          std::abs((stepped_visual->position[1] - request.pose.origin[1]) -
                   20.0f * 100.0f * 0.016f * stepped_visual->forward[1]) < 0.01f,
          "Native projectile velocity did not preserve source Box2D units/cadence");
    check(runtime.update(1002, 1000, false, error), error);
    check(projectile_bodies.after_runtime_update(error), error);
    check(projectile_bodies.size() == 0 &&
          !projectile_bodies.resolve_source_identity(stale_identity,
                                                      identity_id, error),
          "Source timer expiry left a native projectile body or identity registered");

    request.occurrence = 14;
    request.event.generation = 48;
    check(runtime.launch_state5(tables, request, accepted, id, error) && accepted,
          "Fresh projectile was not launched for Session-binding renewal test");
    const auto detach_visuals = runtime.visuals();
    const auto detach_visual = std::find_if(detach_visuals.begin(),
        detach_visuals.end(), [&](const auto& value) {
            return value.projectile_id == id;
        });
    check(detach_visual != detach_visuals.end(),
          "Session-binding projectile visual is absent");
    std::uintptr_t detach_identity = 0;
    check(projectile_bodies.register_projectile(id, *renderer, detach_identity,
                                                 error), error);
    check(projectile_bodies.size() == 1 && detach_identity != 0,
          "Session-binding projectile did not own a native sensor");
    session->detach_for_restore();
    check(!projectile_bodies.resolve_source_identity(detach_identity,
                                                      identity_id, error),
          "Old actor-binding projectile identity resolved after detach");
    check(!projectile_bodies.after_runtime_update(error) &&
          projectile_bodies.size() == 0 && runtime.active_count() == 0,
          "Binding renewal retained a projectile sensor or queued record");
    check(session->rebind_after_restore(error), error);
    check(runtime.synchronize_session_binding(error), error);

    request.occurrence = 15;
    request.event.generation = 49;
    check(runtime.launch_state5(tables, request, accepted, id, error) && accepted,
          "Fresh no-contact projectile was not launched for destroyed-Session guard");

    // Session lifetime and actor-binding freshness are independent witnesses.
    // Retain both old tokens while destroying the actual Session; even then,
    // no public projectile operation may dereference its borrowed raw pointer
    // or publish stale projectiles as live.
    const auto retained_lifetime = session->lifetime_lease().lock();
    const auto retained_binding = session->actor_binding_lease().lock();
    check(retained_lifetime && retained_lifetime->alive() && retained_binding,
          "Could not retain the explicit Session lifetime/binding witnesses");
    check(!runtime.visuals().empty() && runtime.active_count() == 1,
          "Pre-destruction projectile was not observable");
    session.reset();
    check(!retained_lifetime->alive() && retained_binding,
          "Session destruction did not invalidate lifetime independently of binding");
    check(runtime.visuals().empty() && runtime.active_count() == 0,
          "Destroyed Session still published a projectile as live");
    check(!runtime.synchronize_session_binding(error) &&
          error.find("destroyed") != std::string::npos,
          "Destroyed Session binding synchronization was accepted");
    const auto submissions_before_death = render_submissions;
    check(!runtime_session_projectile_frame_v1(runtime, *renderer, 72, 16, false,
                                               frame_output, error) &&
          !frame_output.updated && !frame_output.submitted &&
          frame_output.visuals.empty() && render_submissions == submissions_before_death,
          "Destroyed Session frame was submitted to the renderer");
    check(!runtime.launch_state5(tables, request, accepted, duplicate_id, error) &&
          !accepted && duplicate_id == 0,
          "Destroyed Session accepted a projectile launch");
    check(!runtime.update(1000, 16, false, error),
          "Destroyed Session accepted a projectile update");
    bool dead_contact_accepted = false;
    check(!runtime.contact({id, 2, {10.0f, 20.0f, 0.0f}, true},
                           dead_contact_accepted, error) &&
          !dead_contact_accepted,
          "Destroyed Session accepted a projectile contact");
    check(runtime.take_impacts().empty() && runtime.take_results().empty(),
          "Destroyed Session exposed stale projectile impacts/results");
    std::cout << "PASS same-Session Staff01 row20 state5 launch, retained occurrence, "
                 "paused/dt/timer, direct result/RNG parity, frame consumer/render ordering, "
                 "stale/destroyed Session guards\n";
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Original shared asset root required");
        run(argv[1]);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
