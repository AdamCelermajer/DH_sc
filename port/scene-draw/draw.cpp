#include "draw.hpp"

#include <cstring>

namespace {
using dh2::draw::Command;
using dh2::draw::Error;
using dh2::draw::Stats;
using dh2::resources::Library;
using dh2::scene::Node;
using dh2::scene::Scene;

struct Context {
    Scene scene;
    Stats* stats;
    dh2::draw::Callback callback;
    void* user;
    std::uint32_t visual_index, max_draws;
    Error error;
};

bool visit_node(const Node* node, const dh2::math::Matrix4f* world,
                std::uint32_t, void* opaque) {
    auto& context = *static_cast<Context*>(opaque);
    auto& stats = *context.stats;
    ++stats.nodes;
    stats.instances += node->instances;
    for (std::uint32_t i = 0; i < node->instances; ++i) {
        dh2::scene::Instance instance{};
        if (dh2_scene_instance(node, static_cast<std::int32_t>(i), &instance)
            != dh2::scene::Error::ok) {
            context.error = Error::instance;
            return false;
        }
        if (instance.type != 3) continue;
        ++stats.geometry_instances;
        const auto geometry_index = dh2_scene_geometry_index(&context.scene, &instance);
        if (geometry_index < 0) {
            ++stats.skipped_unresolved_geometry;
            continue;
        }
        ++stats.resolved_geometry;
        dh2::assets::Mesh mesh{};
        const auto mesh_result = dh2_mesh_open(&mesh, &context.scene.image,
                                               geometry_index);
        if (mesh_result == dh2::assets::Error::geometry_type) {
            ++stats.skipped_unsupported_geometry;
            continue;
        }
        if (mesh_result != dh2::assets::Error::ok) {
            context.error = Error::mesh;
            return false;
        }
        for (std::uint32_t j = 0; j < mesh.primitives; ++j) {
            dh2::assets::Primitive primitive{};
            if (dh2_mesh_primitive(&mesh, static_cast<std::int32_t>(j), &primitive)
                != dh2::assets::Error::ok) {
                context.error = Error::primitive;
                return false;
            }
            if (primitive.collada_type != 0 || primitive.index_count == 0 ||
                primitive.index_count % 3 != 0 || mesh.vertices == 0) {
                ++stats.skipped_nontriangle_primitives;
                continue;
            }
            std::int32_t material_index = -1;
            const auto materials = dh2_bres_library_count(&context.scene.image,
                                                           Library::material);
            for (std::uint32_t k = 0; k < materials; ++k) {
                dh2::materials::Material material{};
                if (dh2_material_record(&material, &context.scene.image,
                                        static_cast<std::int32_t>(k))
                    != dh2::materials::Error::ok) {
                    context.error = Error::material;
                    return false;
                }
                if (std::strcmp(material.id, primitive.material) == 0) {
                    material_index = static_cast<std::int32_t>(k);
                    break;
                }
            }
            if (material_index < 0) ++stats.unresolved_materials;
            if (stats.draw_commands >= context.max_draws) {
                context.error = Error::draw_limit;
                return false;
            }
            Command command{*world, node->id, mesh.id, primitive.material,
                            context.visual_index, node->record, node->visible,
                            mesh.vertices, primitive.index_count,
                            primitive.index_width, geometry_index,
                            static_cast<std::int32_t>(j), material_index};
            ++stats.draw_commands;
            stats.triangles += primitive.index_count / 3;
            if (!context.callback(&command, context.user)) {
                context.error = Error::callback_stopped;
                return false;
            }
        }
    }
    return true;
}
}

extern "C" dh2::draw::Error dh2_static_scene_draws(
    Stats* stats, const dh2::resources::BresView* image,
    dh2::draw::Callback callback, void* user,
    std::uint32_t max_nodes, std::uint32_t max_draws) {
    if (!stats) return Error::argument;
    *stats = {};
    if (!image || !callback || !max_nodes || !max_draws) return Error::argument;
    Scene scene{};
    if (dh2_scene_open(&scene, image) != dh2::scene::Error::ok)
        return Error::scene;
    Context context{scene, stats, callback, user, 0, max_draws, Error::ok};
    stats->visual_references = scene.references;
    for (std::uint32_t i = 0; i < scene.references; ++i) {
        dh2::scene::Reference reference{};
        const auto result = dh2_scene_reference(&scene, static_cast<std::int32_t>(i),
                                                &reference);
        if (result == dh2::scene::Error::unsupported_reference) {
            ++stats->skipped_nonvisual_references;
            continue;
        }
        if (result != dh2::scene::Error::ok) return Error::scene;
        const auto visual_index = dh2_scene_visual_index(&scene, reference.url);
        if (visual_index < 0) {
            ++stats->skipped_unresolved_visuals;
            continue;
        }
        dh2::scene::Visual visual{};
        if (dh2_scene_visual(&scene, visual_index, &visual) != dh2::scene::Error::ok)
            return Error::visual;
        ++stats->visual_scenes;
        if (!visual.roots) continue;
        if (stats->nodes >= max_nodes) return Error::node_limit;
        context.visual_index = static_cast<std::uint32_t>(visual_index);
        context.error = Error::ok;
        const auto walked = dh2_scene_walk_visual(&visual, visit_node, &context,
                                                   max_nodes - stats->nodes);
        if (context.error != Error::ok) return context.error;
        if (walked == dh2::scene::Error::walk_limit) return Error::node_limit;
        if (walked != dh2::scene::Error::ok) return Error::scene;
    }
    return Error::ok;
}
