#include "scene_buffers.hpp"
#include "../scene-draw/draw.hpp"

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
    float min_x, max_x, min_z, max_z;
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
        const float x = ((m[0] * position[0] + m[4] * position[1]) +
                         m[8] * position[2]) + m[12];
        const float z = ((m[2] * position[0] + m[6] * position[1]) +
                         m[10] * position[2]) + m[14];
        if (!std::isfinite(x) || !std::isfinite(z) ||
            !std::isfinite(texcoord[0]) || !std::isfinite(texcoord[1])) {
            context.error = SceneMeshError::unsupported;
            return false;
        }
        const auto offset = std::size_t(base + i) * 4;
        output.vertices[offset] = x;
        output.vertices[offset + 1] = z;
        output.vertices[offset + 2] = texcoord[0];
        output.vertices[offset + 3] = texcoord[1];
        if (x < context.min_x) context.min_x = x;
        if (x > context.max_x) context.max_x = x;
        if (z < context.min_z) context.min_z = z;
        if (z > context.max_z) context.max_z = z;
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
}

extern "C" void dh2_viewer_scene_mesh_free(SceneMesh* output) {
    if (!output) return;
    std::free(output->vertices);
    std::free(output->indices);
    *output = {};
}

extern "C" SceneMeshError dh2_viewer_scene_mesh(
    SceneMesh* output, const dh2::resources::BresView* image) {
    if (!output) return SceneMeshError::argument;
    *output = {};
    if (!image || !image->bytes) return SceneMeshError::argument;
    output->vertices = static_cast<float*>(std::malloc(
        std::size_t(max_vertices) * 4 * sizeof(float)));
    output->indices = static_cast<std::uint16_t*>(std::malloc(
        std::size_t(max_indices) * sizeof(std::uint16_t)));
    if (!output->vertices || !output->indices) {
        dh2_viewer_scene_mesh_free(output);
        return SceneMeshError::allocation;
    }
    Context context{output, image, SceneMeshError::ok,
                    INFINITY, -INFINITY, INFINITY, -INFINITY};
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
    if (!output->vertex_count || !output->index_count ||
        output->draw_commands != stats.draw_commands) {
        dh2_viewer_scene_mesh_free(output);
        return SceneMeshError::no_draw;
    }
    const auto dx = context.max_x - context.min_x;
    const auto dz = context.max_z - context.min_z;
    const auto span = dx > dz ? dx : dz;
    if (!std::isfinite(span) || !(span > 0)) {
        dh2_viewer_scene_mesh_free(output);
        return SceneMeshError::unsupported;
    }
    const auto cx = (context.min_x + context.max_x) * 0.5f;
    const auto cz = (context.min_z + context.max_z) * 0.5f;
    for (std::uint32_t i = 0; i < output->vertex_count; ++i) {
        output->vertices[4 * i] = (output->vertices[4 * i] - cx) * 1.5f / span;
        output->vertices[4 * i + 1] = (output->vertices[4 * i + 1] - cz) * 1.5f / span;
    }
    return SceneMeshError::ok;
}
