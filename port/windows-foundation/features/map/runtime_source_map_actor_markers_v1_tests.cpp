#include "runtime_source_map_actor_markers_v1.hpp"
#include "runtime_source_map_menu_provider_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_combat_visual_plan.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;

namespace dh::foundation::map_ui {
bool source_map_project_runtime_frame_v1(
    const SourceMapCameraFrameV1&,
    const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>&,
    const Point3V1&, const std::array<float, 4>&,
    std::array<float, 2>&, std::string& error) {
    error = "projection is outside this menu-routing test";
    return false;
}
bool source_map_camera_frame_current_v1(const SourceMapCameraFrameV1&,
                                        std::string& error) {
    error.clear();
    return true;
}
}

namespace dh::foundation::frontend::art {
const ScreenArt& original_art(Screen) noexcept {
    static const ScreenArt empty;
    return empty;
}
}

namespace {
void check(bool ok, const std::string& message) {
    if (!ok) throw std::runtime_error(message);
}
bool same_owner(const std::shared_ptr<const void>& a,
                const std::shared_ptr<void>& b) {
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}
frontend::art::HitRegion map_triangle(const char* path, float x, float y) {
    frontend::art::HitRegion region;
    region.button_path = path;
    region.triangles = {{x, y, 0, 0}, {x + 10, y, 0, 0}, {x, y + 10, 0, 0}};
    return region;
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply repository root");
        const std::filesystem::path root(argv[1]);
        AssetCatalog assets(root / ".local-inputs/windows-shared-assets");
        AssetCatalog metadata(root / ".local-inputs/windows-melee-bindings");
        OriginalPropertyDatabase database;
        OriginalMeleeBindings melee;
        std::string error;
        check(load_original_property_tables(assets, "original-cache/data/pydata",
                                            database, error), error);
        check(melee.load(metadata, "original-melee-bindings.xml", error), error);

        ActorCustomization customization;
        customization.skin_id_contains = "_default_warrior-mesh-skin";
        customization.expected_controller_count = 4;
        customization.allow_missing_animation_targets = true;
        OriginalCombatVisualPlan plan;
        check(build_original_combat_visual_plan(assets, melee, "KnightPlayerBase",
              customization, "map-session-marker-test", plan, error), error);
        check(plan.phase("Idle", 0, {0}) != nullptr,
              "Original Knight idle phase is required by the same-session fixture");

        CombatSessionConfig config;
        config.diagnosticRngSeed = 17;
        config.playerId = 1;
        config.playerProfileId = "KnightPlayerBase";
        config.tableRoot = "original-cache/data/pydata";
        config.playerVisualConfig = plan.config;
        config.playerVisualConfig.motion_node_id = "auto";
        config.playerVisualConfig.consume_root_motion = true;
        CombatSessionProfile player;
        player.initialIdle = {"Idle", 0, {0}};
        player.animationOnly = true;
        player.customization = customization;
        player.motionRoot = "auto";
        config.profiles.emplace(config.playerProfileId, player);

        ActorPopulation population;
        CharacterVisual visual;
        CombatSession session;
        check(session.initialize(assets, database, melee, config, visual, population,
                                 {0, 0, 0}, customization, error), error);
        const ActorId player_id = session.player_id();
        const auto* world = session.world();
        check(player_id == 1 && world && session.actor(player_id) &&
              world->find_actor(player_id) == session.actor(player_id) &&
              world->traits(player_id) && world->traits(player_id)->is_player &&
              session.retained_actor_pose(player_id) && session.retained_actor_visual_borrow(player_id),
              "Fixture did not publish one actual retained local player in its current Session world: player=" +
              std::to_string(player_id) + " world=" + std::to_string(world != nullptr) +
              " actor=" + std::to_string(session.actor(player_id) != nullptr) +
              " world_actor=" + std::to_string(world && world->find_actor(player_id) == session.actor(player_id)) +
              " traits=" + std::to_string(world && world->traits(player_id) != nullptr) +
              " is_player=" + std::to_string(world && world->traits(player_id) && world->traits(player_id)->is_player));

        map_page::SourceMarkerV1 marker;
        check(source_map_current_player_marker_v1(session, marker, error), error);
        const auto lease = session.actor_binding_lease().lock();
        check(marker.family == 3 && marker.source_id == player_id && marker.owner &&
              same_owner(lease, marker.owner),
              "Map family3 marker did not retain this exact Session actor binding");
        const auto* actor = session.actor(player_id);
        check(actor && marker.world_position.x == actor->transform.position[0] &&
              marker.world_position.y == actor->transform.position[1] &&
              marker.world_position.z == actor->transform.position[2] &&
              std::isfinite(marker.world_position.x) && std::isfinite(marker.world_position.y) &&
              std::isfinite(marker.world_position.z),
              "Map family3 marker did not use the same live ActorState transform");

        std::vector<map_page::SourceMarkerV1> markers;
        markers.push_back(marker);
        const auto before = markers.size();
        check(!map_page::source_map_append_current_player_marker_v1(session, markers, error) &&
              !error.empty() && markers.size() == before,
              "Map collector accepted duplicate same-session family3 marker or mutated output");
        markers.clear();
        check(map_page::source_map_append_current_player_marker_v1(session, markers, error), error);
        check(markers.size() == 1 && markers[0].family == 3 &&
              markers[0].source_id == player_id && same_owner(lease, markers[0].owner),
              "Map collector did not append the actual current local player exactly once");

        CombatSession unbound;
        const auto prior = markers;
        check(!map_page::source_map_append_current_player_marker_v1(unbound, markers, error) &&
              !error.empty() && markers.size() == prior.size() &&
              markers[0].source_id == prior[0].source_id &&
              same_owner(std::shared_ptr<const void>(lease), markers[0].owner),
              "Unbound/replaced session changed the existing source marker collection");

        // Exercise the exact authored pushed-menu symbol and the callable
        // routing to the existing page and same current CombatSession.
        auto owner_world = std::make_shared<int>(1);
        auto owner_level = std::make_shared<int>(2);
        auto owner_rooms = std::make_shared<int>(3);
        auto owner_camera = std::make_shared<int>(4);
        auto owner_player = std::make_shared<int>(5);
        auto owner_character = std::make_shared<int>(6);
        auto owner_save = std::make_shared<int>(7);
        auto owner_quests = std::make_shared<int>(8);
        auto owner_events = std::make_shared<int>(9);
        unsigned map_shown = 0, map_hidden = 0, map_legend = 0, map_reset = 0;
        bool received_same_owner = true;
        map_page::ServicesV1 page_services;
        page_services.source_page.borrow = [&](map_ui::SourceMapBorrowV1& out,
                                               std::string&) {
            out = {owner_world, owner_level, owner_rooms, owner_camera,
                   owner_player, owner_character, owner_save, owner_quests,
                   owner_events, 7, 0};
            return true;
        };
        page_services.source_page.project = [](const map_ui::SourceMapBorrowV1&,
                                                character_menu::Frame&, std::string&) {
            return true;
        };
        auto verify_owner = [&](const map_ui::SourceMapBorrowV1& owner) {
            const bool same = owner.world.get() == owner_world.get() &&
                owner.level.get() == owner_level.get() &&
                owner.room_zones.get() == owner_rooms.get() &&
                owner.camera.get() == owner_camera.get() &&
                owner.local_player.get() == owner_player.get() &&
                owner.character.get() == owner_character.get() &&
                owner.save.get() == owner_save.get() &&
                owner.quests.get() == owner_quests.get() &&
                owner.events.get() == owner_events.get() && owner.level_generation == 7;
            received_same_owner &= same;
            return same;
        };
        page_services.source_page.show = [&](const map_ui::SourceMapBorrowV1& owner,
                                              std::string&) {
            ++map_shown; return verify_owner(owner);
        };
        page_services.source_page.hide = [&](const map_ui::SourceMapBorrowV1& owner,
                                              std::string&) {
            ++map_hidden; return verify_owner(owner);
        };
        page_services.source_page.legend = [&](const map_ui::SourceMapBorrowV1& owner,
                                                bool visible, std::string&) {
            ++map_legend; return visible && verify_owner(owner);
        };
        page_services.source_page.reset_zoom = [&](const map_ui::SourceMapBorrowV1& owner,
                                                    std::string&) {
            ++map_reset; return verify_owner(owner);
        };
        page_services.source_page.authored_resource = [](std::string& hash,
                                                          std::uint32_t& sprite,
                                                          std::string&) {
            hash = "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0";
            sprite = 655;
            return true;
        };
        page_services.collect_markers = [](const map_ui::SourceMapBorrowV1&,
                                            std::vector<map_page::SourceMarkerV1>& out,
                                            std::string&) { out.clear(); return true; };
        map_page::RuntimeSourceMapPageV1 map_page_runtime(std::move(page_services));
        map_page::RuntimeSourceMapMenuProviderV1 provider(map_page_runtime);
        map_page::SourceMapPushedMenuCallbacksV1 route;
        bool route_registered = false;
        map_page::RegisterSourcePushedMenuV1 registrar =
            [&](const char* symbol, const map_page::SourceMapPushedMenuCallbacksV1& callbacks,
                std::string&) {
                if (route_registered || std::string(symbol) != "menu_MapSheet") return false;
                route_registered = true;
                route = callbacks;
                return true;
            };
        check(provider.register_with(registrar, error) && provider.registered() &&
              route_registered && route.open && route.close && route.release &&
              route.append_current_player_marker,
              "MapSheet adapter did not register callable handlers at the exact authored symbol");
        check(!provider.register_with(registrar, error) && !error.empty(),
              "MapSheet adapter accepted a duplicate registration");
        check(route.open(error) && route.visible() && map_shown == 1,
              "menu_MapSheet open did not route through the existing same-owner page");

        frontend::art::ScreenArt synthetic_controls;
        synthetic_controls.render_regions.push_back(
            {"menu_MapSheet.RenderMap", {100, 300, 30, 190}});
        synthetic_controls.hit_regions.push_back(map_triangle("menu_MapSheet.btn_Legend", 10, 10));
        synthetic_controls.hit_regions.push_back(map_triangle("menu_MapSheet.btn_ResetZoom", 30, 10));
        map_page::AuthoredMapArtV1 map_art{
            &synthetic_controls,
            "43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0",
            655};
        map_page::PageInputV1 map_action = map_page::PageInputV1::none;
        check(route.release(map_art, {12, 12}, 480, 320, map_action, error) &&
              map_action == map_page::PageInputV1::legend && map_legend == 1,
              "menu_MapSheet Legend control did not reach the original page action owner");
        check(route.release(map_art, {32, 12}, 480, 320, map_action, error) &&
              map_action == map_page::PageInputV1::reset_zoom && map_reset == 1,
              "menu_MapSheet ResetZoom control did not reach the original page action owner");
        check(route.release(map_art, {100, 200}, 480, 320, map_action, error) &&
              map_action == map_page::PageInputV1::none,
              "MapSheet adapter invented a marker-click action outside source controls");

        std::vector<map_page::SourceMarkerV1> routed_markers;
        check(route.append_current_player_marker(session, routed_markers, error), error);
        check(routed_markers.size() == 1 && routed_markers.front().family == 3 &&
              routed_markers.front().source_id == player_id &&
              same_owner(lease, routed_markers.front().owner),
              "MapSheet callable did not append the current same-Session family3 marker");
        const auto routed_before = routed_markers;
        check(!route.append_current_player_marker(session, routed_markers, error) &&
              !error.empty() && routed_markers.size() == routed_before.size(),
              "MapSheet callable did not fail closed on duplicate family3 marker");
        check(!route.append_current_player_marker(unbound, routed_markers, error) &&
              !error.empty() && routed_markers.size() == routed_before.size(),
              "MapSheet callable did not fail closed for unbound current Session");
        check(route.close(error) && !route.visible() && map_hidden == 1 && received_same_owner,
              "menu_MapSheet close did not release the same page owner");
        std::cout << "PASS current Session family3 marker uses same live player/world/lease; duplicate and unbound collectors fail closed\n";
        std::cout << "PASS menu_MapSheet exact-symbol registration, same-owner controls, and current-Session marker routing\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
}
