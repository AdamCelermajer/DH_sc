#include "scene_buffers.hpp"
#include "../scene-draw/draw.hpp"
#include "../skin-payloads/skin.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
using dh2::viewer::SceneMesh;
using dh2::viewer::SceneMeshError;
constexpr std::uint32_t max_vertices = 8192;
constexpr std::uint32_t max_indices = 24000;
constexpr std::uint32_t max_commands = 256;

struct Context {
    SceneMesh* output;
    const dh2::resources::BresView* image;
    SceneMeshError error;
    float minimum[3], maximum[3];
    const dh2::skin::Skin* skin;
    const dh2::math::Matrix4f* palette;
    const dh2::pose::Clip* clip;
    std::int32_t time;
    const dh2::layers::Layers* layers;
    const char* node_prefix;
};

void first_diffuse(const dh2::draw::Command* draw, Context& context) {
    if (draw->material_index < 0 || context.output->first_diffuse_texture[0]) return;
    dh2::materials::Material material{};
    if (dh2_material_record(&material, context.image, draw->material_index)
        != dh2::materials::Error::ok) return;
    for (std::uint32_t i = 0; i < material.parameter_count; ++i) {
        dh2::materials::Parameter parameter{};
        if (dh2_material_parameter(&parameter, &material, i)
            != dh2::materials::Error::ok || parameter.type_code != 11 ||
            !parameter.id || !std::strstr(parameter.id, "diffuse")) continue;
        dh2::materials::ImageRef image{};
        if (dh2_material_sampler_image(&image, &material, i)
            != dh2::materials::Error::ok || image.index < 0 || !image.source_path)
            continue;
        const char* slash = std::strrchr(image.source_path, '/');
        const char* backslash = std::strrchr(image.source_path, '\\');
        if (backslash && (!slash || backslash > slash)) slash = backslash;
        std::snprintf(context.output->first_diffuse_texture,
                      sizeof(context.output->first_diffuse_texture), "%s",
                      slash ? slash + 1 : image.source_path);
        return;
    }
}

bool append_draw(const dh2::draw::Command* draw, void* user) {
    auto& context = *static_cast<Context*>(user);
    if (context.node_prefix && (!draw->node_id ||
        std::strncmp(draw->node_id, context.node_prefix,
                     std::strlen(context.node_prefix)) != 0)) return true;
    auto& output = *context.output;
    dh2::assets::Mesh mesh{};
    dh2::assets::Primitive primitive{};
    dh2::assets::Attribute positions{}, uv{};
    if (dh2_mesh_open(&mesh, context.image, draw->geometry_index)
            != dh2::assets::Error::ok ||
        dh2_mesh_primitive(&mesh, draw->primitive_index, &primitive)
            != dh2::assets::Error::ok ||
        primitive.attributes[0] < 0 ||
        dh2_mesh_attribute(&mesh, primitive.attributes[0], &positions)
            != dh2::assets::Error::ok || positions.components < 3) {
        context.error = SceneMeshError::unsupported;
        return false;
    }
    const bool has_uv = primitive.attributes[4] >= 0;
    if (has_uv && (dh2_mesh_attribute(&mesh, primitive.attributes[4], &uv)
                   != dh2::assets::Error::ok || uv.components < 2)) {
        context.error = SceneMeshError::unsupported;
        return false;
    }
    if (mesh.vertices > max_vertices - output.vertex_count ||
        primitive.index_count > max_indices - output.index_count) {
        context.error = SceneMeshError::limit;
        return false;
    }
    const auto base = output.vertex_count;
    const auto* m = draw->world.m;
    for (std::uint32_t i = 0; i < mesh.vertices; ++i) {
        float position[16]{}, texcoord[16]{};
        if (!dh2_attribute_read(&positions, i, position) ||
            (has_uv && !dh2_attribute_read(&uv, i, texcoord))) {
            context.error = SceneMeshError::unsupported;
            return false;
        }
        if (context.skin) {
            dh2::math::Vector3f input{position[0], position[1], position[2]}, skinned{};
            if (dh2_skin_position(context.skin, i, context.palette, context.skin->joints,
                                   &input, &skinned) != dh2::skin::Error::ok) {
                context.error = SceneMeshError::unsupported; return false;
            }
            position[0] = skinned.x; position[1] = skinned.y; position[2] = skinned.z;
        }
        const float x = ((m[0] * position[0] + m[4] * position[1]) +
                         m[8] * position[2]) + m[12];
        const float y = ((m[1] * position[0] + m[5] * position[1]) +
                         m[9] * position[2]) + m[13];
        const float z = ((m[2] * position[0] + m[6] * position[1]) +
                         m[10] * position[2]) + m[14];
        if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z) ||
            !std::isfinite(texcoord[0]) || !std::isfinite(texcoord[1])) {
            context.error = SceneMeshError::unsupported;
            return false;
        }
        const auto offset = std::size_t(base + i) * 5;
        output.vertices[offset] = x;
        output.vertices[offset + 1] = y;
        output.vertices[offset + 2] = z;
        output.vertices[offset + 3] = texcoord[0];
        output.vertices[offset + 4] = texcoord[1];
        const float coordinates[3] = {x, y, z};
        for (int axis = 0; axis < 3; ++axis) {
            if (coordinates[axis] < context.minimum[axis])
                context.minimum[axis] = coordinates[axis];
            if (coordinates[axis] > context.maximum[axis])
                context.maximum[axis] = coordinates[axis];
        }
    }
    for (std::uint32_t i = 0; i < primitive.index_count; ++i) {
        std::uint32_t index = 0;
        if (!dh2_index_read(&primitive, i, &index) || index >= mesh.vertices) {
            context.error = SceneMeshError::unsupported;
            return false;
        }
        output.indices[output.index_count + i] =
            static_cast<std::uint16_t>(base + index);
    }
    output.vertex_count += mesh.vertices;
    output.index_count += primitive.index_count;
    ++output.draw_commands;
    first_diffuse(draw, context);
    return true;
}
bool append_first_skin(Context& context) {
    dh2::scene::Scene scene{};
    if (dh2_scene_open(&scene, context.image) != dh2::scene::Error::ok) return false;
    const auto count = dh2_bres_library_count(context.image, dh2::resources::Library::controller);
    for (std::uint32_t i = 0; i < count; ++i) {
        dh2::skin::Skin skin{};
        if (dh2_skin_open(&skin, context.image, i) != dh2::skin::Error::ok) continue;
        // Diagnostic: select the first resolvable controller, not every armour
        // alternative in a modular character file.
        for (std::uint32_t j = 0; j < scene.visuals; ++j) {
            dh2::scene::Visual visual{}; dh2::math::Matrix4f palette[256]{};
            if (dh2_scene_visual(&scene, j, &visual) != dh2::scene::Error::ok) continue;
            if (context.layers) {
                if (dh2_layers_skin_palette(context.layers, &skin, &visual, palette, 256)
                    != dh2::pose::Error::ok) continue;
            } else if (context.clip) {
                if (dh2_pose_skin_palette(context.clip, context.time, &skin, &visual, palette, 256)
                    != dh2::pose::Error::ok) continue;
            } else if (dh2_skin_scene_palette(&skin, &visual, palette, 256) != dh2::skin::Error::ok)
                continue;
            dh2::assets::Mesh mesh{};
            if (dh2_mesh_open(&mesh, context.image, skin.geometry_index) != dh2::assets::Error::ok)
                return false;
            context.skin = &skin; context.palette = palette;
            for (std::uint32_t k = 0; k < mesh.primitives; ++k) {
                dh2::assets::Primitive primitive{};
                if (dh2_mesh_primitive(&mesh, k, &primitive) != dh2::assets::Error::ok)
                    return false;
                if (primitive.collada_type != 0 || primitive.index_count % 3) continue;
                std::int32_t material_index = -1;
                const auto materials = dh2_bres_library_count(context.image, dh2::resources::Library::material);
                for (std::uint32_t m = 0; m < materials; ++m) {
                    dh2::materials::Material material{};
                    if (dh2_material_record(&material, context.image, m) == dh2::materials::Error::ok &&
                        std::strcmp(material.id, primitive.material) == 0) {
                        material_index = static_cast<std::int32_t>(m); break;
                    }
                }
                dh2::draw::Command command{};
                command.world.m[0] = command.world.m[5] = command.world.m[10] = command.world.m[15] = 1;
                command.geometry_index = skin.geometry_index;
                command.primitive_index = static_cast<std::int32_t>(k);
                command.material_index = material_index;
                if (!append_draw(&command, &context)) return false;
            }
            context.skin = nullptr; context.palette = nullptr;
            context.output->skin_joints = skin.joints;
            return context.output->draw_commands != 0;
        }
    }
    return false;
}
}

extern "C" void dh2_viewer_scene_mesh_free(SceneMesh* output) {
    if (!output) return;
    std::free(output->vertices);
    std::free(output->indices);
    *output = {};
}

static SceneMeshError mesh_at(
    SceneMesh* output, const dh2::resources::BresView* image,
    const dh2::pose::Clip* clip, std::int32_t milliseconds,
    const dh2::layers::Layers* layers, bool normalized = true,
    const char* node_prefix = nullptr) {
    if (!output) return SceneMeshError::argument;
    *output = {};
    if (!image || !image->bytes) return SceneMeshError::argument;
    output->vertices = static_cast<float*>(std::malloc(
        std::size_t(max_vertices) * 5 * sizeof(float)));
    output->indices = static_cast<std::uint16_t*>(std::malloc(
        std::size_t(max_indices) * sizeof(std::uint16_t)));
    if (!output->vertices || !output->indices) {
        dh2_viewer_scene_mesh_free(output);
        return SceneMeshError::allocation;
    }
    Context context{output, image, SceneMeshError::ok,
                    {INFINITY, INFINITY, INFINITY},
                    {-INFINITY, -INFINITY, -INFINITY}, nullptr, nullptr, clip, milliseconds, layers,
                    node_prefix};
    dh2::draw::Stats stats{};
    const auto walked = dh2_static_scene_draws(&stats, image, append_draw,
                                                &context, 20000, max_commands);
    if (context.error != SceneMeshError::ok || walked != dh2::draw::Error::ok) {
        const auto error = context.error != SceneMeshError::ok ? context.error
            : (walked == dh2::draw::Error::node_limit ||
               walked == dh2::draw::Error::draw_limit)
              ? SceneMeshError::limit : SceneMeshError::scene_walk;
        dh2_viewer_scene_mesh_free(output);
        return error;
    }
    if (!output->draw_commands && !node_prefix) append_first_skin(context);
    if (context.error != SceneMeshError::ok || !output->vertex_count || !output->index_count ||
        (!output->skin_joints && !node_prefix && output->draw_commands != stats.draw_commands)) {
        dh2_viewer_scene_mesh_free(output);
        return SceneMeshError::no_draw;
    }
    float span = 0.0f;
    for (int axis = 0; axis < 3; ++axis) {
        const auto extent = context.maximum[axis] - context.minimum[axis];
        if (extent > span) span = extent;
    }
    if (!std::isfinite(span) || !(span > 0)) {
        dh2_viewer_scene_mesh_free(output);
        return SceneMeshError::unsupported;
    }
    if (!normalized) return SceneMeshError::ok;
    float center[3]{};
    for (int axis = 0; axis < 3; ++axis)
        center[axis] = (context.minimum[axis] + context.maximum[axis]) * 0.5f;
    for (std::uint32_t i = 0; i < output->vertex_count; ++i) {
        for (int axis = 0; axis < 3; ++axis)
            output->vertices[5 * i + axis] =
                (output->vertices[5 * i + axis] - center[axis]) / span;
    }
    return SceneMeshError::ok;
}

extern "C" SceneMeshError dh2_viewer_scene_mesh_at(
    SceneMesh* output, const dh2::resources::BresView* image,
    const dh2::pose::Clip* clip, std::int32_t milliseconds) {
    return mesh_at(output, image, clip, milliseconds, nullptr);
}

extern "C" SceneMeshError dh2_viewer_scene_mesh_layers(
    SceneMesh* output, const dh2::resources::BresView* image,
    const dh2::layers::Layers* layers) {
    if (!layers) return SceneMeshError::argument;
    return mesh_at(output, image, nullptr, 0, layers);
}

extern "C" SceneMeshError dh2_viewer_scene_mesh(
    SceneMesh* output, const dh2::resources::BresView* image) {
    return dh2_viewer_scene_mesh_at(output, image, nullptr, 0);
}

extern "C" SceneMeshError dh2_world_scene_mesh(
    SceneMesh* output, const dh2::resources::BresView* image, const char* node_prefix) {
    return mesh_at(output, image, nullptr, 0, nullptr, false, node_prefix);
}

extern "C" SceneMeshError dh2_world_scene_mesh_at(
    SceneMesh* output, const dh2::resources::BresView* image,
    const dh2::pose::Clip* clip, std::int32_t milliseconds) {
    return mesh_at(output, image, clip, milliseconds, nullptr, false);
}
