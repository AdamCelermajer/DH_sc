#include "navigation.hpp"
#include "../floor-types/floor_types.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace dh2::navigation;

namespace {
Error fail(Diagnostic* d, Error e, const char* message) {
    if (d) {
        *d = {};
        d->error = e;
        std::snprintf(d->message, sizeof d->message, "%s", message);
    }
    return e;
}
bool finite(float f) { return std::isfinite(f); }
bool contains(const char* text, const char* part) {
    return text && part && std::strstr(text, part) != nullptr;
}
bool copy_text(char (&to)[max_source_string], const char* from) {
    if (!from) return false;
    const auto length = std::strlen(from);
    if (length >= sizeof to) return false;
    std::memcpy(to, from, length + 1);
    return true;
}
bool copy_span(char* to, std::size_t capacity, dh2::floor_types::Span from) {
    if (!to || !capacity || (!from.data && from.size) || from.size >= capacity) return false;
    if (from.size) std::memcpy(to, from.data, from.size);
    to[from.size] = '\0';
    return true;
}
Error reserve_surfaces(Navigation* nav) {
    if (nav->surface_count == max_surfaces) return Error::limit;
    if (nav->surface_count < nav->surface_capacity) return Error::ok;
    auto capacity = nav->surface_capacity ? nav->surface_capacity * 2U : 8U;
    if (capacity > max_surfaces) capacity = max_surfaces;
    auto* grown = static_cast<Surface*>(std::realloc(nav->surfaces,
        std::size_t(capacity) * sizeof(Surface)));
    if (!grown) return Error::allocation;
    nav->surfaces = grown;
    nav->surface_capacity = capacity;
    return Error::ok;
}
Error reserve_triangles(Navigation* nav) {
    if (nav->triangle_count == max_triangles) return Error::limit;
    if (nav->triangle_count < nav->triangle_capacity) return Error::ok;
    auto capacity = nav->triangle_capacity ? nav->triangle_capacity * 2U : 128U;
    if (capacity > max_triangles) capacity = max_triangles;
    auto* grown = static_cast<SurfaceTriangle*>(std::realloc(nav->triangles,
        std::size_t(capacity) * sizeof(SurfaceTriangle)));
    if (!grown) return Error::allocation;
    nav->triangles = grown;
    nav->triangle_capacity = capacity;
    return Error::ok;
}
void transform_point(const dh2::math::Matrix4f& m, const float in[3], float out[3]) {
    for (unsigned row = 0; row < 3; ++row)
        out[row] = m.m[row] * in[0] + m.m[4 + row] * in[1]
            + m.m[8 + row] * in[2] + m.m[12 + row];
}

struct ModuleWalk {
    Navigation* nav;
    const dh2::world::SourceLevel* level;
    const dh2::world::Module* module;
    dh2::world::ModuleBinding binding;
    const dh2::scene_payload::Scene* scene;
    bool active, root_found, failed;
    std::uint32_t root_depth;
    Error error;
};

Error append_geometry(ModuleWalk& state, const dh2::scene_payload::Node& node,
    const dh2::math::Matrix4f& world, const dh2::scene_payload::Instance& instance) {
    const auto geometry_index = dh2_scene_geometry_index(state.scene, &instance);
    if (geometry_index < 0) return Error::scene;
    dh2::assets::Mesh mesh{};
    if (dh2_mesh_open(&mesh, &state.scene->image, geometry_index) != dh2::assets::Error::ok)
        return Error::unsupported_mesh;
    dh2::assets::Attribute position{};
    if (dh2_mesh_attribute(&mesh, 0, &position) != dh2::assets::Error::ok
        || position.type != 6 || position.components < 3 || position.vertices != mesh.vertices)
        return Error::unsupported_mesh;

    Surface surface{};
    surface.module_index = state.module->record.module_index;
    surface.module_source_record = state.module->record.source_record;
    surface.node_record = node.record;
    surface.geometry_index = static_cast<std::uint32_t>(geometry_index);
    surface.visible = node.visible;
    surface.first_triangle = state.nav->triangle_count;
    surface.vertex_count = mesh.vertices;
    surface.primitive_count = mesh.primitives;
    const char* user_properties = nullptr;
    std::size_t user_properties_size = 0;
    if (dh2_scene_user_data_string(&node, &user_properties, &user_properties_size)
        != dh2::scene_payload::Error::ok) return Error::scene;
    dh2::floor_types::Property floor_type_property{};
    if (user_properties && dh2::floor_types::find_property(user_properties,
            user_properties_size, {"floortypes", 10U}, &floor_type_property)
            != dh2::floor_types::Error::ok) return Error::malformed;
    surface.floor_type_tag_present = floor_type_property.found;
    if (floor_type_property.found
        && !copy_span(surface.floor_type_tag, sizeof(surface.floor_type_tag),
                      floor_type_property.value)) return Error::limit;
    const dh2::floor_types::Span node_name{
        node.name, node.name ? std::strlen(node.name) : 0U};
    surface.floor_type_flags = dh2::floor_types::floor_type_mask(
        floor_type_property.found, floor_type_property.value, node_name);
    surface.floor_type_flags_known = true;
    if (!copy_text(surface.module_name, state.module->record.name)
        || !copy_text(surface.source_node_id, node.id)
        || !copy_text(surface.source_node_name, node.name)
        || !copy_text(surface.source_geometry_id, mesh.id)
        || !copy_text(surface.source_geometry_name, mesh.name)) return Error::limit;

    for (std::uint32_t primitive_index = 0; primitive_index < mesh.primitives; ++primitive_index) {
        dh2::assets::Primitive primitive{};
        if (dh2_mesh_primitive(&mesh, static_cast<std::int32_t>(primitive_index), &primitive)
            != dh2::assets::Error::ok) return Error::mesh;
        if (primitive.collada_type != 0 || primitive.index_count % 3 != 0)
            return Error::unsupported_mesh;
        for (std::uint32_t index_offset = 0; index_offset < primitive.index_count; index_offset += 3) {
            if (surface.triangle_count == max_triangles
                || state.nav->triangle_count == max_triangles) return Error::limit;
            SurfaceTriangle triangle{};
            triangle.surface_index = state.nav->surface_count;
            triangle.primitive_index = primitive_index;
            triangle.source_triangle_index = index_offset / 3;
            float* points[] = {triangle.a, triangle.b, triangle.c};
            for (unsigned corner = 0; corner < 3; ++corner) {
                std::uint32_t vertex = 0;
                if (!dh2_index_read(&primitive, index_offset + corner, &vertex)
                    || vertex >= mesh.vertices) return Error::mesh;
                float raw[4]{};
                if (!dh2_attribute_read(&position, vertex, raw)) return Error::mesh;
                transform_point(world, raw, points[corner]);
                if (!finite(points[corner][0]) || !finite(points[corner][1])
                    || !finite(points[corner][2])) return Error::malformed;
            }
            const auto reserve = reserve_triangles(state.nav);
            if (reserve != Error::ok) return reserve;
            state.nav->triangles[state.nav->triangle_count++] = triangle;
            ++surface.triangle_count;
        }
    }
    if (!surface.triangle_count) return Error::ok;
    const auto reserve = reserve_surfaces(state.nav);
    if (reserve != Error::ok) return reserve;
    state.nav->surfaces[state.nav->surface_count++] = surface;
    return Error::ok;
}

bool visit_node(const dh2::scene_payload::Node* node, const dh2::math::Matrix4f* matrix,
                std::uint32_t depth, void* opaque) {
    auto& state = *static_cast<ModuleWalk*>(opaque);
    if (state.active && depth <= state.root_depth) state.active = false;
    if (node->record == state.binding.node_record) {
        if (state.root_found) {
            state.failed = true; state.error = Error::scene; return false;
        }
        state.root_found = state.active = true;
        state.root_depth = depth;
    }
    if (!state.active || !contains(node->name, "floor")) return true;

    dh2::math::Matrix4f placed{};
    dh2::world::Diagnostic world_diagnostic{};
    if (dh2_world_place_matrix(&placed, &state.binding, matrix, &world_diagnostic)
        != dh2::world::Error::ok) {
        state.failed = true; state.error = Error::unsupported_transform; return false;
    }
    for (std::uint32_t i = 0; i < node->instances; ++i) {
        dh2::scene_payload::Instance instance{};
        if (dh2_scene_instance(node, static_cast<std::int32_t>(i), &instance)
            != dh2::scene_payload::Error::ok) {
            state.failed = true; state.error = Error::scene; return false;
        }
        // PFWorld searches mesh scene nodes. Other node instance kinds are
        // intentionally left to their recovered constructors.
        if (instance.type != 3) continue;
        const auto result = append_geometry(state, *node, placed, instance);
        if (result != Error::ok) {
            state.failed = true; state.error = result; return false;
        }
    }
    return true;
}

bool source_is_swamp(const dh2::world::SourceLevel* level) {
    if (!level || !level->name || std::strcmp(level->name, "SWAMP")
        || level->module_count != 9 || !level->modules) return false;
    for (std::uint32_t i = 0; i < level->module_count; ++i) {
        char normalized[1024]{};
        dh2::world::Diagnostic d{};
        if (!level->modules[i].cache_dae
            || dh2_world_cache_path(normalized, sizeof normalized,
                level->modules[i].cache_dae, &d) != dh2::world::Error::ok
            || std::strcmp(normalized, "data/3d/modules/swamp/swamp.bdae")) return false;
    }
    return true;
}

Error query_height_impl(const Navigation* nav, float x, float y, float reference_z,
    float max_vertical_distance, float edge_tolerance, bool actor_query,
    std::uint32_t object_path_mask, FloorHit* output, bool* found) {
    if (found) *found = false;
    if (output) *output = {};
    if (!nav || !output || !found || !nav->triangles || !nav->surfaces
        || !nav->triangle_count || !nav->surface_count
        || nav->triangle_count > max_triangles || nav->surface_count > max_surfaces
        || nav->triangle_count > nav->triangle_capacity
        || nav->surface_count > nav->surface_capacity
        || !finite(x) || !finite(y) || !finite(reference_z)
        || !finite(max_vertical_distance) || max_vertical_distance < 0
        || max_vertical_distance > 1000000.0f || !finite(edge_tolerance)
        || edge_tolerance < 0 || edge_tolerance > 0.25f) return Error::argument;

    constexpr std::uint32_t category_mask =
        dh2::floor_types::kFloorTypeVoid | dh2::floor_types::kFloorTypeWall;
    float best_distance = 0;
    FloorHit best{};
    for (std::uint32_t i = 0; i < nav->triangle_count; ++i) {
        const auto& t = nav->triangles[i];
        if (t.surface_index >= nav->surface_count) return Error::malformed;
        const auto& surface = nav->surfaces[t.surface_index];
        if (actor_query) {
            if (!surface.floor_type_flags_known
                || (surface.floor_type_flags & category_mask) != 0U
                || !dh2::floor_types::can_path_on(surface.floor_type_flags,
                                                   object_path_mask)) continue;
        }
        const float denominator = (t.b[1] - t.c[1]) * (t.a[0] - t.c[0])
            + (t.c[0] - t.b[0]) * (t.a[1] - t.c[1]);
        if (!finite(denominator)) return Error::malformed;
        if (std::fabs(denominator) <= 1.0e-8f) continue;
        const float wa = ((t.b[1] - t.c[1]) * (x - t.c[0])
            + (t.c[0] - t.b[0]) * (y - t.c[1])) / denominator;
        const float wb = ((t.c[1] - t.a[1]) * (x - t.c[0])
            + (t.a[0] - t.c[0]) * (y - t.c[1])) / denominator;
        const float wc = 1.0f - wa - wb;
        if (!finite(wa) || !finite(wb) || !finite(wc)) return Error::malformed;
        if (wa < -edge_tolerance || wb < -edge_tolerance || wc < -edge_tolerance
            || wa > 1.0f + edge_tolerance || wb > 1.0f + edge_tolerance
            || wc > 1.0f + edge_tolerance) continue;
        const float height = wa * t.a[2] + wb * t.b[2] + wc * t.c[2];
        if (!finite(height)) return Error::malformed;
        const float distance = std::fabs(height - reference_z);
        if (distance > max_vertical_distance || (*found && distance >= best_distance)) continue;
        best.surface_index = t.surface_index;
        best.primitive_index = t.primitive_index;
        best.source_triangle_index = t.source_triangle_index;
        best.height = height;
        best.vertical_distance = distance;
        best.barycentric[0] = wa; best.barycentric[1] = wb; best.barycentric[2] = wc;
        best.floor_type_flags_known = surface.floor_type_flags_known;
        best.floor_type_flags = surface.floor_type_flags;
        best.floor_type_tag_present = surface.floor_type_tag_present;
        std::memcpy(best.floor_type_tag, surface.floor_type_tag,
                    sizeof(best.floor_type_tag));
        best_distance = distance;
        *found = true;
    }
    if (*found) *output = best;
    return Error::ok;
}

float dot3(const float a[3], const float b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
void subtract3(const float a[3], const float b[3], float out[3]) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}
void cross3(const float a[3], const float b[3], float out[3]) {
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

bool point_in_triangle_3d(const SurfaceTriangle& triangle, const float point[3]) {
    const float* vertices[] = {triangle.a, triangle.b, triangle.c};
    for (unsigned edge_index = 0; edge_index < 3; ++edge_index) {
        const float* edge_start = vertices[edge_index];
        const float* edge_end = vertices[(edge_index + 1U) % 3U];
        const float* opposite = vertices[(edge_index + 2U) % 3U];
        float edge[3], to_point[3], to_opposite[3], side_point[3], side_opposite[3];
        subtract3(edge_end, edge_start, edge);
        subtract3(point, edge_start, to_point);
        subtract3(opposite, edge_start, to_opposite);
        cross3(edge, to_point, side_point);
        cross3(edge, to_opposite, side_opposite);
        const float same_side = dot3(side_point, side_opposite);
        if (!finite(same_side) || same_side < 0.0f) return false;
    }
    return true;
}

bool intersect_segment_triangle(const float start[3], const float direction[3],
    const SurfaceTriangle& triangle, float* fraction, float position[3]) {
    float ab[3], ac[3], normal[3];
    subtract3(triangle.b, triangle.a, ab);
    subtract3(triangle.c, triangle.a, ac);
    cross3(ab, ac, normal);
    const float normal_length_squared = dot3(normal, normal);
    if (!finite(normal_length_squared)) return false;
    if (normal_length_squared <= 0.0f) return false;
    const float normal_length = std::sqrt(normal_length_squared);
    if (!finite(normal_length) || normal_length <= 0.0f) return false;
    normal[0] /= normal_length;
    normal[1] /= normal_length;
    normal[2] /= normal_length;

    const float denominator = dot3(normal, direction);
    if (!finite(denominator) || std::fabs(denominator) <= 1.0e-6f) return false;
    float start_to_plane[3];
    subtract3(triangle.a, start, start_to_plane);
    const float t = dot3(normal, start_to_plane) / denominator;
    if (!finite(t) || t < 0.0f || t > 1.0f) return false;
    float point[3];
    for (unsigned i = 0; i < 3; ++i) {
        point[i] = start[i] + direction[i] * t;
        if (!finite(point[i])) return false;
    }
    if (!point_in_triangle_3d(triangle, point)) return false;
    *fraction = t;
    position[0] = point[0]; position[1] = point[1]; position[2] = point[2];
    return true;
}
}

extern "C" Error dh2_nav_build_swamp(Navigation* output,
    const dh2::world::SourceLevel* level, const dh2::scene_payload::Scene* scene, Diagnostic* d) {
    if (!output || !scene) return fail(d, Error::argument, "Missing navigation build input");
    if (output->surfaces || output->triangles || output->surface_count
        || output->triangle_count || output->surface_capacity || output->triangle_capacity)
        return fail(d, Error::argument, "Navigation output must be initialized and empty");
    if (!source_is_swamp(level))
        return fail(d, Error::level, "Expected SWAMP with nine modules using swamp.bdae");

    Navigation candidate{};
    for (std::uint32_t i = 0; i < level->module_count; ++i) {
        ModuleWalk state{};
        state.nav = &candidate;
        state.level = level;
        state.module = &level->modules[i];
        state.scene = scene;
        state.error = Error::ok;
        dh2::world::Diagnostic world_diagnostic{};
        if (dh2_world_bind_module(&state.binding, state.module, scene, &world_diagnostic)
            != dh2::world::Error::ok) {
            dh2_nav_free(&candidate);
            return fail(d, Error::scene, "Failed to bind SWAMP module root");
        }
        dh2::scene_payload::Visual visual{};
        if (state.binding.visual_index >= scene->visuals
            || dh2_scene_visual(scene, static_cast<std::int32_t>(state.binding.visual_index), &visual)
                != dh2::scene_payload::Error::ok) {
            dh2_nav_free(&candidate);
            return fail(d, Error::scene, "Invalid module visual scene");
        }
        const auto walked = dh2_scene_walk_visual(&visual, visit_node, &state, 65536);
        if (state.failed || walked != dh2::scene_payload::Error::ok || !state.root_found) {
            const auto error = state.failed ? state.error : Error::scene;
            dh2_nav_free(&candidate);
            return fail(d, error, state.failed ? "Failed while collecting floor geometry"
                : "Module floor subtree walk did not complete");
        }
    }
    if (!candidate.surface_count || !candidate.triangle_count) {
        dh2_nav_free(&candidate);
        return fail(d, Error::malformed, "SWAMP catalogue contains no selected floor triangles");
    }
    *output = candidate;
    if (d) *d = {};
    return Error::ok;
}

extern "C" void dh2_nav_free(Navigation* nav) {
    if (!nav) return;
    std::free(nav->surfaces);
    std::free(nav->triangles);
    *nav = {};
}

extern "C" Error dh2_nav_surface(const Navigation* nav, std::uint32_t index,
                                  Surface* output) {
    if (!nav || !output || !nav->surfaces || nav->surface_count > nav->surface_capacity
        || nav->surface_count > max_surfaces || index >= nav->surface_count)
        return Error::argument;
    *output = nav->surfaces[index];
    return Error::ok;
}

extern "C" Error dh2_nav_get_triangle(const Navigation* nav, std::uint32_t index,
                                   SurfaceTriangle* output) {
    if (!nav || !output || !nav->triangles || nav->triangle_count > nav->triangle_capacity
        || nav->triangle_count > max_triangles || index >= nav->triangle_count)
        return Error::argument;
    *output = nav->triangles[index];
    return Error::ok;
}

extern "C" Error dh2_nav_query_height(const Navigation* nav,
    float x, float y, float reference_z, float max_vertical_distance,
    float edge_tolerance, FloorHit* output, bool* found) {
    return query_height_impl(nav, x, y, reference_z, max_vertical_distance,
                             edge_tolerance, false, 0U, output, found);
}

extern "C" Error dh2_nav_query_actor_floor(const Navigation* nav,
    float x, float y, float reference_z, float max_vertical_distance,
    float edge_tolerance, std::uint32_t object_path_mask,
    FloorHit* output, bool* found) {
    return query_height_impl(nav, x, y, reference_z, max_vertical_distance,
                             edge_tolerance, true, object_path_mask, output, found);
}

extern "C" Error dh2_nav_query_segment(const Navigation* nav,
    const float start[3], const float end[3], bool include_all,
    SegmentHit* output, bool* found) {
    if (found) *found = false;
    if (output) *output = {};
    if (!nav || !start || !end || !output || !found || !nav->triangles
        || !nav->surfaces || !nav->triangle_count || !nav->surface_count
        || nav->triangle_count > max_triangles || nav->surface_count > max_surfaces
        || nav->triangle_count > nav->triangle_capacity
        || nav->surface_count > nav->surface_capacity) return Error::argument;
    float direction[3];
    for (unsigned i = 0; i < 3; ++i) {
        if (!finite(start[i]) || !finite(end[i])
            || std::fabs(start[i]) > 10000000.0f
            || std::fabs(end[i]) > 10000000.0f) return Error::argument;
        direction[i] = end[i] - start[i];
        if (!finite(direction[i])) return Error::argument;
    }
    const float segment_length_squared = dot3(direction, direction);
    if (!finite(segment_length_squared) || segment_length_squared > 1.0e12f)
        return Error::argument;

    constexpr std::uint32_t category_mask =
        dh2::floor_types::kFloorTypeVoid | dh2::floor_types::kFloorTypeWall;
    for (std::uint32_t surface_index = 0; surface_index < nav->surface_count;
         ++surface_index) {
        const auto& surface = nav->surfaces[surface_index];
        if (surface.first_triangle > nav->triangle_count
            || surface.triangle_count > nav->triangle_count - surface.first_triangle)
            return Error::malformed;
        if (!include_all && (!surface.floor_type_flags_known
            || (surface.floor_type_flags & category_mask) != 0U)) continue;

        bool surface_found = false;
        SegmentHit surface_hit{};
        for (std::uint32_t i = 0; i < surface.triangle_count; ++i) {
            const auto triangle_index = surface.first_triangle + i;
            const auto& triangle = nav->triangles[triangle_index];
            if (triangle.surface_index != surface_index) return Error::malformed;
            const float* vertices[] = {triangle.a, triangle.b, triangle.c};
            for (const auto* vertex : vertices) {
                for (unsigned axis = 0; axis < 3; ++axis) {
                    if (!finite(vertex[axis]) || std::fabs(vertex[axis]) > 10000000.0f)
                        return Error::malformed;
                }
            }
            float t = 0.0f, position[3]{};
            if (!intersect_segment_triangle(start, direction, triangle, &t, position)
                || (surface_found && t >= surface_hit.fraction)) continue;
            surface_hit.surface_index = surface_index;
            surface_hit.primitive_index = triangle.primitive_index;
            surface_hit.source_triangle_index = triangle.source_triangle_index;
            surface_hit.fraction = t;
            std::memcpy(surface_hit.position, position, sizeof(position));
            surface_hit.floor_type_flags_known = surface.floor_type_flags_known;
            surface_hit.floor_type_flags = surface.floor_type_flags;
            surface_hit.floor_type_tag_present = surface.floor_type_tag_present;
            std::memcpy(surface_hit.floor_type_tag, surface.floor_type_tag,
                        sizeof(surface_hit.floor_type_tag));
            surface_found = true;
        }
        if (surface_found) {
            *output = surface_hit;
            *found = true;
            return Error::ok;
        }
    }
    return Error::ok;
}
