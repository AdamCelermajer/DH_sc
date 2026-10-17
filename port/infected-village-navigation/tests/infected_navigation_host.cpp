#include "../../android-app/infected_village_scene.hpp"
#include "../../floor-types/floor_types.hpp"
#include "../../navigation/navigation.hpp"
#include "../../scene-payloads/scene.hpp"
#include "../../world-data/world_scene.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

namespace {
using dh2::navigation::Navigation;
using dh2::navigation::Surface;
using dh2::navigation::SurfaceTriangle;

void require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "Infected Village navigation check failed: %s\n", message);
    std::exit(1);
}

std::vector<std::uint8_t> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    require(static_cast<bool>(input), path.c_str());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

struct NodeLookup {
    std::uint32_t record;
    dh2::scene_payload::Node result{};
    bool found = false;
};

bool find_node(const dh2::scene_payload::Node* node, const dh2::math::Matrix4f*,
               std::uint32_t, void* opaque) {
    auto& lookup = *static_cast<NodeLookup*>(opaque);
    if (node->record == lookup.record) {
        lookup.result = *node;
        lookup.found = true;
    }
    return true;
}

using QuantizedPoint = std::array<std::int64_t, 3>;
using EdgeKey = std::array<std::int64_t, 6>;

QuantizedPoint quantize(const float* point) {
    return {std::llround(point[0] * 100.0), std::llround(point[1] * 100.0),
            std::llround(point[2] * 100.0)};
}

EdgeKey edge_key(const float* a, const float* b) {
    auto qa = quantize(a);
    auto qb = quantize(b);
    if (qb < qa) std::swap(qa, qb);
    return {qa[0], qa[1], qa[2], qb[0], qb[1], qb[2]};
}

std::map<EdgeKey, float> module_edges(const Navigation& nav, std::uint32_t module) {
    std::map<EdgeKey, float> result;
    for (std::uint32_t i = 0; i < nav.triangle_count; ++i) {
        const auto& triangle = nav.triangles[i];
        if (nav.surfaces[triangle.surface_index].module_index != module) continue;
        const float* vertices[] = {triangle.a, triangle.b, triangle.c};
        for (unsigned edge = 0; edge < 3; ++edge) {
            const auto* a = vertices[edge];
            const auto* b = vertices[(edge + 1U) % 3U];
            const auto key = edge_key(a, b);
            const float dx = b[0] - a[0];
            const float dy = b[1] - a[1];
            const float dz = b[2] - a[2];
            result.emplace(key, std::sqrt(dx * dx + dy * dy + dz * dz));
        }
    }
    return result;
}

void append_floor(const dh2::viewer::SceneMesh& mesh, std::uint32_t module_index,
                  const char* wanted_node, const dh2::scene_payload::Scene& scene,
                  const dh2::scene_payload::Visual& visual,
                  std::vector<Surface>& surfaces, std::vector<SurfaceTriangle>& triangles) {
    const dh2::viewer::SceneDrawDescriptor* draw = nullptr;
    for (std::uint32_t i = 0; i < mesh.draw_commands; ++i) {
        if (std::string(mesh.draws[i].node_id) == wanted_node) {
            require(!draw, "floor node emitted more than one draw; inspect primitive selection");
            draw = &mesh.draws[i];
        }
    }
    require(draw != nullptr, "source floor node is absent from its module draw set");
    require(draw->visible != 0 && draw->index_count > 0 && draw->index_count % 3 == 0,
            "source floor draw is not visible triangle geometry");
    require(draw->first_index <= mesh.index_count &&
            draw->index_count <= mesh.index_count - draw->first_index,
            "floor index range exceeds imported mesh");

    NodeLookup lookup{draw->node_record};
    require(dh2_scene_walk_visual(&visual, find_node, &lookup, 65536) ==
                dh2::scene_payload::Error::ok && lookup.found,
            "floor draw node record could not be resolved in source BDAE");
    require(std::string(lookup.result.id) == wanted_node,
            "floor draw record maps to a different source node ID");
    const char* user_properties = nullptr;
    std::size_t user_properties_bytes = 0;
    require(dh2_scene_user_data_string(&lookup.result, &user_properties,
                &user_properties_bytes) == dh2::scene_payload::Error::ok,
            "floor UserProperties could not be read");
    dh2::floor_types::Property property{};
    if (user_properties)
        require(dh2::floor_types::find_property(user_properties, user_properties_bytes,
                    {"floortypes", 10}, &property) == dh2::floor_types::Error::ok,
                "floor floortypes property could not be parsed");
    const dh2::floor_types::Span name{lookup.result.name,
        std::char_traits<char>::length(lookup.result.name)};

    Surface surface{};
    surface.module_index = module_index;
    surface.node_record = draw->node_record;
    surface.geometry_index = static_cast<std::uint32_t>(draw->geometry_index);
    surface.visible = draw->visible;
    surface.first_triangle = static_cast<std::uint32_t>(triangles.size());
    surface.vertex_count = draw->vertex_count;
    surface.primitive_count = 1;
    surface.floor_type_tag_present = property.found;
    if (property.found) {
        require(property.value.size < sizeof(surface.floor_type_tag),
                "floor type tag exceeds checked surface metadata");
        if (property.value.size)
            std::copy(property.value.data, property.value.data + property.value.size,
                      surface.floor_type_tag);
        surface.floor_type_tag[property.value.size] = '\0';
    }
    surface.floor_type_flags = dh2::floor_types::floor_type_mask(
        property.found, property.value, name);
    surface.floor_type_flags_known = true;
    std::snprintf(surface.module_name, sizeof(surface.module_name), "infected_module_%02u",
                  module_index + 1U);
    std::snprintf(surface.source_node_id, sizeof(surface.source_node_id), "%s",
                  lookup.result.id);
    std::snprintf(surface.source_node_name, sizeof(surface.source_node_name), "%s",
                  lookup.result.name);
    std::snprintf(surface.source_geometry_id, sizeof(surface.source_geometry_id), "%s",
                  draw->geometry_id);

    for (std::uint32_t offset = 0; offset < draw->index_count; offset += 3U) {
        SurfaceTriangle triangle{};
        triangle.surface_index = static_cast<std::uint32_t>(surfaces.size());
        triangle.primitive_index = static_cast<std::uint32_t>(draw->primitive_index);
        triangle.source_triangle_index = offset / 3U;
        float* out[] = {triangle.a, triangle.b, triangle.c};
        for (unsigned corner = 0; corner < 3; ++corner) {
            const std::uint32_t vertex = mesh.indices[draw->first_index + offset + corner];
            require(vertex >= draw->first_vertex &&
                    vertex < draw->first_vertex + draw->vertex_count &&
                    vertex < mesh.vertex_count,
                    "floor triangle points outside its original draw vertices");
            std::copy_n(mesh.vertices + std::size_t(vertex) * 5U, 3, out[corner]);
        }
        triangles.push_back(triangle);
        ++surface.triangle_count;
    }
    require(surface.triangle_count > 0, "floor draw produced no triangles");
    surfaces.push_back(surface);
    (void)scene;
}

} // namespace

int main(int argc, char** argv) {
    require(argc == 2, "usage: infected_navigation_host <cache-root>");
    const std::string cache = argv[1];
    auto mlx = read_file(cache + "/data/scene/005_infectedvillage.mlx");
    auto catalogue = read_file(cache + "/data/3d/modules/infectedvillage/infectedvillage.bdae");
    auto mgp0 = read_file(cache + "/data/3d/modules/infectedvillage/mgp/infected01.mgp");
    auto mvp0 = read_file(cache + "/data/3d/modules/infectedvillage/mvp/infected01.mvp");
    auto mgp1 = read_file(cache + "/data/3d/modules/infectedvillage/mgp/infected02.mgp");
    auto mvp1 = read_file(cache + "/data/3d/modules/infectedvillage/mvp/infected02.mvp");

    dh2::infectedpreview::SourceFiles files{
        mlx.data(), mlx.size(), catalogue.data(), catalogue.size(),
        {mgp0.data(), mgp1.data()}, {mgp0.size(), mgp1.size()},
        {mvp0.data(), mvp1.data()}, {mvp0.size(), mvp1.size()}};
    dh2::infectedpreview::Preview preview{};
    dh2::infectedpreview::Diagnostic diagnostic{};
    require(dh2::infectedpreview::load(&preview, &files, &diagnostic) ==
                dh2::infectedpreview::Error::ok,
            diagnostic.message);

    const dh2::world::Object* entry = nullptr;
    for (std::uint32_t i = 0; i < preview.level.entity_count; ++i) {
        const auto& object = preview.level.entities[i];
        if (object.module_index == 0 && object.gametype &&
            std::string(object.gametype) == "SpawnPoint" &&
            dh2_world_field(&object, "entrypointID") &&
            std::string(dh2_world_field(&object, "entrypointID")) == "0") {
            require(entry == nullptr, "entrypoint zero is ambiguous in module 0");
            entry = &object;
        }
    }
    require(entry && entry->name && std::string(entry->name) == "_prim_EntryPoint",
            "module zero does not contain the checked source entrypoint");
    require(entry->source_record == 1 && entry->local.position[0] == 900.8499756f &&
            entry->local.position[1] == -4148.52002f &&
            entry->local.position[2] == 1348.88000f &&
            std::string(entry->source_path) ==
                "data/3d/modules/infectedvillage/mgp/infected01.mgp",
            "entrypoint source record or local transform changed");
    const float expected_origins[2][3] = {{-3448.5f, 3000.0f, 0.0f},
                                          {2551.5f, 3000.0f, 0.0f}};
    for (std::uint32_t module = 0; module < 2; ++module)
        for (unsigned axis = 0; axis < 3; ++axis)
            require(std::fabs(preview.level.modules[module].record.local.position[axis] -
                              expected_origins[module][axis]) < 0.001f,
                    "source module placement changed");

    dh2::resources::BresView bres{};
    require(dh2_bres_open(&bres, catalogue.data(), catalogue.size()) ==
                dh2::resources::BresError::ok,
            "Infected Village BDAE could not be opened for navigation-source metadata");
    dh2::scene_payload::Scene scene{};
    require(dh2_scene_open(&scene, &bres) == dh2::scene_payload::Error::ok,
            "Infected Village source scene could not be opened");

    std::vector<Surface> surfaces;
    std::vector<SurfaceTriangle> triangles;
    const char* floor_nodes[] = {"_floor_infectedvillage_01-node_PIVOT",
                                 "_floor_infectedvillage_02-node_PIVOT"};
    for (std::uint32_t module = 0; module < 2; ++module) {
        const auto& record = preview.level.modules[module];
        dh2::world::Diagnostic world_diagnostic{};
        dh2::world::ModuleBinding binding{};
        require(dh2_world_bind_module(&binding, &record, &scene,
                    &world_diagnostic) == dh2::world::Error::ok,
                "MLX module did not bind to its exact BDAE root");
        dh2::scene_payload::Visual visual{};
        require(dh2_scene_visual(&scene, static_cast<std::int32_t>(binding.visual_index),
                    &visual) == dh2::scene_payload::Error::ok,
                "selected module visual scene could not be opened");
        std::uint32_t floor_draws = 0;
        for (std::uint32_t draw = 0; draw < preview.modules[module].draw_commands; ++draw)
            if (std::string(preview.modules[module].draws[draw].node_id).find("floor")
                    != std::string::npos) ++floor_draws;
        require(floor_draws == 1,
                "source module contains a different number of floor-named mesh draws");
        append_floor(preview.modules[module], module, floor_nodes[module], scene,
                     visual, surfaces, triangles);
    }

    Navigation nav{};
    nav.surfaces = surfaces.data();
    nav.surface_count = nav.surface_capacity = static_cast<std::uint32_t>(surfaces.size());
    nav.triangles = triangles.data();
    nav.triangle_count = nav.triangle_capacity = static_cast<std::uint32_t>(triangles.size());

    dh2::navigation::FloorHit entry_hit{};
    bool entry_found = false;
    const auto entry_query = dh2_nav_query_height(&nav,
        entry->world_position[0], entry->world_position[1], entry->world_position[2],
        100.0f, 1.0e-6f, &entry_hit, &entry_found);
    require(entry_query == dh2::navigation::Error::ok && entry_found,
            "source entrypoint has no queried floor within native validation band");
    require(entry_hit.surface_index < surfaces.size() &&
            surfaces[entry_hit.surface_index].module_index == 0 &&
            std::string(surfaces[entry_hit.surface_index].source_node_id) == floor_nodes[0],
            "entrypoint floor sample did not resolve to module 0's original floor node");

    const auto module0_edges = module_edges(nav, 0);
    const auto module1_edges = module_edges(nav, 1);
    std::size_t shared_edges = 0;
    float longest_shared_edge = 0.0f;
    for (const auto& edge : module0_edges) {
        const auto found = module1_edges.find(edge.first);
        if (found == module1_edges.end()) continue;
        ++shared_edges;
        longest_shared_edge = std::max(longest_shared_edge,
                                       std::min(edge.second, found->second));
    }

    std::printf("pass=true static_floor_queries_only=true modules=2 floors=%u triangles=%u "
                "entry=(%.3f,%.3f,%.3f) floor_z=%.3f vertical_delta=%.3f "
                "entry_floor=%s floor_tags_present=%u shared_module_edges=%zu "
                "longest_shared_edge=%.3f actor_collision_or_walkability=not_tested\n",
        nav.surface_count, nav.triangle_count,
        entry->world_position[0], entry->world_position[1], entry->world_position[2],
        entry_hit.height, entry_hit.vertical_distance,
        surfaces[entry_hit.surface_index].source_node_id,
        static_cast<unsigned>(surfaces[0].floor_type_tag_present) +
            static_cast<unsigned>(surfaces[1].floor_type_tag_present),
        shared_edges, longest_shared_edge);
    dh2::infectedpreview::free(&preview);
    return 0;
}
