#include "scene_mesh_adapter.hpp"

#include <cmath>
#include <new>

namespace dh2::irrlicht_adapter {
namespace {

using viewer::SceneDrawDescriptor;
using viewer::SceneMesh;

bool add_fits(std::uint32_t first, std::uint32_t count, std::uint32_t total) {
    return static_cast<std::uint64_t>(first) + count <= total;
}

bool finite_vertex(const float* vertex) {
    return std::isfinite(vertex[0]) && std::isfinite(vertex[1]) &&
           std::isfinite(vertex[2]) && std::isfinite(vertex[3]) &&
           std::isfinite(vertex[4]);
}

void set_status(MeshBuildStatus* status, MeshBuildStatus value) {
    if (status) *status = value;
}

bool validate_draw(const SceneMesh& source,
                   const SceneDrawDescriptor& draw,
                   MeshBuildStatus* error) {
    if (!draw.vertex_count || !draw.index_count || draw.index_count % 3 != 0) {
        set_status(error, MeshBuildStatus::invalid_scene);
        return false;
    }
    if (!add_fits(draw.first_vertex, draw.vertex_count, source.vertex_count) ||
        !add_fits(draw.first_index, draw.index_count, source.index_count) ||
        !add_fits(draw.first_texture, draw.texture_count,
                  source.texture_reference_count)) {
        set_status(error, MeshBuildStatus::range_error);
        return false;
    }

    for (std::uint32_t i = 0; i < draw.vertex_count; ++i) {
        const float* vertex = source.vertices +
            static_cast<std::size_t>(draw.first_vertex + i) * 5;
        if (!finite_vertex(vertex)) {
            set_status(error, MeshBuildStatus::invalid_scene);
            return false;
        }
    }
    const std::uint32_t vertex_end = draw.first_vertex + draw.vertex_count;
    for (std::uint32_t i = 0; i < draw.index_count; ++i) {
        const std::uint32_t index = source.indices[draw.first_index + i];
        if (index < draw.first_vertex || index >= vertex_end) {
            set_status(error, MeshBuildStatus::range_error);
            return false;
        }
    }
    return true;
}

void calculate_normals(irr::scene::SMeshBuffer& buffer) {
    // SceneMesh intentionally retains positions and UVs only. Reconstruct
    // smooth, area-independent normals for ordinary Irrlicht lighting rather
    // than inventing a source normal stream.
    const irr::u32 index_count = buffer.Indices.size();
    for (irr::u32 i = 0; i + 2 < index_count; i += 3) {
        const irr::u16 ia = buffer.Indices[i];
        const irr::u16 ib = buffer.Indices[i + 1];
        const irr::u16 ic = buffer.Indices[i + 2];
        const auto& a = buffer.Vertices[ia].Pos;
        const auto& b = buffer.Vertices[ib].Pos;
        const auto& c = buffer.Vertices[ic].Pos;

        const double abx = static_cast<double>(b.X) - a.X;
        const double aby = static_cast<double>(b.Y) - a.Y;
        const double abz = static_cast<double>(b.Z) - a.Z;
        const double acx = static_cast<double>(c.X) - a.X;
        const double acy = static_cast<double>(c.Y) - a.Y;
        const double acz = static_cast<double>(c.Z) - a.Z;
        double nx = aby * acz - abz * acy;
        double ny = abz * acx - abx * acz;
        double nz = abx * acy - aby * acx;
        const double length = std::sqrt(nx * nx + ny * ny + nz * nz);
        if (!(length > 0.0) || !std::isfinite(length)) continue;
        nx /= length;
        ny /= length;
        nz /= length;

        const irr::core::vector3df face(static_cast<irr::f32>(nx),
                                        static_cast<irr::f32>(ny),
                                        static_cast<irr::f32>(nz));
        buffer.Vertices[ia].Normal += face;
        buffer.Vertices[ib].Normal += face;
        buffer.Vertices[ic].Normal += face;
    }

    for (irr::u32 i = 0; i < buffer.Vertices.size(); ++i) {
        auto& normal = buffer.Vertices[i].Normal;
        const double length = std::sqrt(
            static_cast<double>(normal.X) * normal.X +
            static_cast<double>(normal.Y) * normal.Y +
            static_cast<double>(normal.Z) * normal.Z);
        if (length > 0.0 && std::isfinite(length)) {
            normal.X = static_cast<irr::f32>(normal.X / length);
            normal.Y = static_cast<irr::f32>(normal.Y / length);
            normal.Z = static_cast<irr::f32>(normal.Z / length);
        } else {
            normal.set(0.0f, 0.0f, 1.0f);
        }
    }
}

irr::scene::SMeshBuffer* make_buffer(
    const SceneMesh& source, const SceneDrawDescriptor& draw,
    TextureResolver texture_resolver, void* texture_user_data) {
    irr::scene::SMeshBuffer* buffer = new (std::nothrow) irr::scene::SMeshBuffer();
    if (!buffer) return nullptr;

    buffer->Vertices.reallocate(draw.vertex_count);
    for (std::uint32_t i = 0; i < draw.vertex_count; ++i) {
        const float* vertex = source.vertices +
            static_cast<std::size_t>(draw.first_vertex + i) * 5;
        buffer->Vertices.push_back(irr::video::S3DVertex(
            vertex[0], vertex[1], vertex[2],
            0.0f, 0.0f, 0.0f, irr::video::SColor(255, 255, 255, 255),
            vertex[3], vertex[4]));
    }

    buffer->Indices.reallocate(draw.index_count);
    for (std::uint32_t i = 0; i < draw.index_count; ++i) {
        const std::uint32_t source_index = source.indices[draw.first_index + i];
        buffer->Indices.push_back(static_cast<irr::u16>(source_index - draw.first_vertex));
    }

    calculate_normals(*buffer);
    buffer->recalculateBoundingBox();
    buffer->getMaterial().setFlag(irr::video::EMF_LIGHTING, false);
    buffer->getMaterial().setFlag(irr::video::EMF_BACK_FACE_CULLING, false);

    if (texture_resolver && draw.texture_count) {
        const auto* references = source.texture_references + draw.first_texture;
        irr::video::ITexture* texture = texture_resolver(
            draw, references, draw.texture_count, texture_user_data);
        if (texture) buffer->getMaterial().setTexture(0, texture);
    }
    return buffer;
}

} // namespace

namespace {

irr::scene::IMesh* build_scene_mesh(
    const SceneMesh& source, MeshBuildStatus* status,
    TextureResolver texture_resolver, void* texture_user_data,
    std::uint32_t* output_buffer_count,
    bool apply_source_room_filter,
    std::uint32_t room_root_node_record,
    SourceRoomRenderCounts* output_filtered_counts) {
    set_status(status, MeshBuildStatus::argument);
    if (output_buffer_count) *output_buffer_count = 0;
    if (output_filtered_counts) *output_filtered_counts = {};
    if (!source.vertices || !source.indices || !source.draws ||
        !source.vertex_count || !source.index_count || !source.draw_commands ||
        source.vertex_count > source.vertex_capacity ||
        source.index_count > source.index_capacity ||
        source.draw_commands > source.draw_capacity ||
        source.texture_reference_count > source.texture_reference_capacity ||
        (source.texture_reference_count && !source.texture_references))
        return nullptr;

    std::uint32_t visible_count = 0;
    SourceRoomRenderCounts filtered{};
    for (std::uint32_t i = 0; i < source.draw_commands; ++i) {
        const auto& draw = source.draws[i];
        if (!validate_draw(source, draw, status)) return nullptr;
        if (!draw.visible) continue;
        const auto role = apply_source_room_filter
            ? classify_source_room_draw(draw, room_root_node_record)
            : SourceRoomDrawRole::scenery;
        switch (role) {
        case SourceRoomDrawRole::room_root_bounds: ++filtered.room_root_bounds; break;
        case SourceRoomDrawRole::navigation_floor: ++filtered.navigation_floors; break;
        case SourceRoomDrawRole::exit_marker: ++filtered.exit_markers; break;
        case SourceRoomDrawRole::scenery: ++visible_count; break;
        }
    }
    if (output_filtered_counts) *output_filtered_counts = filtered;
    if (!visible_count) {
        set_status(status, MeshBuildStatus::no_visible_draws);
        return nullptr;
    }

    irr::scene::SMesh* mesh = new (std::nothrow) irr::scene::SMesh();
    if (!mesh) {
        set_status(status, MeshBuildStatus::allocation);
        return nullptr;
    }

    for (std::uint32_t i = 0; i < source.draw_commands; ++i) {
        const auto& draw = source.draws[i];
        if (!draw.visible || (apply_source_room_filter &&
                classify_source_room_draw(draw, room_root_node_record) !=
                    SourceRoomDrawRole::scenery)) continue;
        irr::scene::SMeshBuffer* buffer = make_buffer(
            source, draw, texture_resolver, texture_user_data);
        if (!buffer) {
            mesh->drop();
            set_status(status, MeshBuildStatus::allocation);
            return nullptr;
        }
        mesh->addMeshBuffer(buffer);
        buffer->drop();
    }

    mesh->recalculateBoundingBox();
    if (output_buffer_count) *output_buffer_count = visible_count;
    set_status(status, MeshBuildStatus::ok);
    return mesh;
}

} // namespace

irr::scene::IMesh* build_mesh(
    const SceneMesh& source, MeshBuildStatus* status,
    TextureResolver texture_resolver, void* texture_user_data,
    std::uint32_t* output_buffer_count) {
    return build_scene_mesh(source, status, texture_resolver, texture_user_data,
                            output_buffer_count, false, 0, nullptr);
}

irr::scene::IMesh* build_source_room_scenery_mesh(
    const SceneMesh& source, std::uint32_t room_root_node_record,
    MeshBuildStatus* status, TextureResolver texture_resolver,
    void* texture_user_data, std::uint32_t* output_buffer_count,
    SourceRoomRenderCounts* output_filtered_counts) {
    return build_scene_mesh(source, status, texture_resolver, texture_user_data,
                            output_buffer_count, true, room_root_node_record,
                            output_filtered_counts);
}

} // namespace dh2::irrlicht_adapter
