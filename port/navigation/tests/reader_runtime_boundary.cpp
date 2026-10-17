#include "../navigation.hpp"
#include "../../level-world/navigation.hpp"
#include "../../scene-payloads/scene.hpp"
#include "../../scene-materials/scene.hpp"
#include "../../world-data/world.hpp"
#include "../../level-world/world.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

static_assert(!std::is_same_v<dh2::scene_payload::Scene, dh2::scene::Scene>);
static_assert(!std::is_same_v<dh2::scene_payload::Node, dh2::scene::Node>);
static_assert(!std::is_same_v<dh2::scene_payload::Instance, dh2::scene::Instance>);
static_assert(!std::is_same_v<dh2::world::SourceLevel, dh2::world::Level>);
static_assert(!std::is_same_v<dh2::navigation::SurfaceTriangle,
                              dh2::navigation::Triangle>);
static_assert(sizeof(dh2::navigation::SurfaceTriangle) == 48);
static_assert(sizeof(dh2::navigation::Triangle) == 36);

namespace {
void require(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

std::vector<std::uint8_t> read(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(bool(input), "Missing boundary-test input: " + path);
    return {std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()};
}

std::uint32_t graph_floor(void*, std::uint32_t, const float*) { return 1; }
}

int main(int argc, char** argv) {
    if (argc != 2) return 2;
    try {
        const std::string cache = argv[1];
        const auto mlx = read(cache + "/data/scene/001_swamp.mlx");
        const auto catalogue = read(cache + "/data/3d/modules/swamp/swamp.bdae");
        dh2::world::SourceLevel imported{};
        dh2::world::Diagnostic world_diagnostic{};
        require(dh2_world_import_level(&imported, "SWAMP",
                    "data/scene/001_swamp.mlx", mlx.data(), mlx.size(),
                    &world_diagnostic) == dh2::world::Error::ok,
                world_diagnostic.message);
        require(imported.module_count == 9, "Source MLX module count changed");

        dh2::resources::BresView bres{};
        require(dh2_bres_open(&bres, catalogue.data(), catalogue.size()) ==
                    dh2::resources::BresError::ok, "Catalogue BRES rejected");
        dh2::scene_payload::Scene payload{};
        require(dh2_scene_open(&payload, &bres) == dh2::scene_payload::Error::ok,
                "Payload scene reader rejected the catalogue");
        dh2::scene::Scene live_scene;
        std::string error;
        require(dh2::scene::load(bres, live_scene, error), error);
        require(payload.visuals && !live_scene.graph.empty(),
                "One of the distinct scene implementations did not run");

        dh2::navigation::Navigation navigation{};
        dh2::navigation::Diagnostic navigation_diagnostic{};
        require(dh2_nav_build_swamp(&navigation, &imported, &payload,
                    &navigation_diagnostic) == dh2::navigation::Error::ok,
                navigation_diagnostic.message);
        require(navigation.triangle_count > 0, "Source navigation is empty");
        dh2::navigation::SurfaceTriangle copied{};
        require(dh2_nav_get_triangle(&navigation, 0, &copied) ==
                    dh2::navigation::Error::ok &&
                    !std::memcmp(&copied, navigation.triangles, sizeof(copied)),
                "Imported triangle getter did not copy geometry and source IDs");

        dh2::navigation::Node nodes[3]{};
        dh2::navigation::Edge edges[6]{};
        dh2::navigation::InvalidNode invalid[3]{};
        std::uint32_t validation[3]{};
        dh2::navigation::Graph graph{};
        graph.nodes = nodes;
        graph.edges = edges;
        graph.invalid = invalid;
        graph.validation = validation;
        graph.node_capacity = 3;
        graph.edge_capacity = 6;
        graph.invalid_capacity = 3;
        graph.validation_capacity = 3;
        graph.query = graph_floor;
        const dh2::navigation::Triangle graph_triangle{
            {{0, 0, 0}, {10, 0, 0}, {0, 10, 0}}};
        require(dh2_nav_begin_floor(&graph, 7) == 0 &&
                    dh2_nav_triangle(&graph, &graph_triangle, 0) == 0 &&
                    graph.node_count == 3 && graph.edge_count == 6,
                "Live graph triangle builder did not bind to its own symbol");

        // This call links the live world implementation while the imported
        // SourceLevel remains live in the same executable. A missing authored
        // descriptor is expected to reject without touching imported data.
        dh2::world::Level live_level;
        require(!dh2::world::load(bres, nullptr, 0, live_level, error) &&
                    !error.empty() && imported.module_count == 9,
                "Live world rejection affected the source MLX records");

        std::cout << "{\"pass\":true,\"single_native_image\":true,"
                     "\"distinct_scene_types\":3,\"source_modules\":"
                  << imported.module_count << ",\"payload_visuals\":"
                  << payload.visuals << ",\"live_scene_nodes\":"
                  << live_scene.graph.size() << ",\"imported_triangles\":"
                  << navigation.triangle_count << ",\"source_triangle_bytes\":"
                  << sizeof(copied) << ",\"live_triangle_bytes\":"
                  << sizeof(graph_triangle) << ",\"graph_nodes\":"
                  << graph.node_count << ",\"graph_edges\":"
                  << graph.edge_count << "}\n";
        dh2_nav_free(&navigation);
        dh2_world_free(&imported);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 3;
    }
}
