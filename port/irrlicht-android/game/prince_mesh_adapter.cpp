#include "scene_mesh_adapter.hpp"

#include <algorithm>
#include <cmath>
#include <new>
#include <utility>

namespace dh2::irrlicht_adapter {
namespace {

irr::u32 color_channel(float value) {
    if (!std::isfinite(value)) return 255;
    return static_cast<irr::u32>(std::lround(
        std::clamp(value, 0.0f, 1.0f) * 255.0f));
}

irr::video::SColor to_irr_color(const std::array<float, 4>& rgba) {
    return irr::video::SColor(color_channel(rgba[3]), color_channel(rgba[0]),
                              color_channel(rgba[1]), color_channel(rgba[2]));
}

void calculate_normals(irr::scene::SMeshBuffer& buffer) {
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

void apply_source_material(const PrinceMeshPart& source,
                           irr::scene::SMeshBuffer& buffer,
                           PrinceTextureResolver resolver,
                           void* user_data, MutablePrinceMesh& output) {
    auto& material = buffer.getMaterial();
    material.setFlag(irr::video::EMF_LIGHTING, false);
    material.setFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    material.DiffuseColor = to_irr_color(source.material_color);
    material.AmbientColor = to_irr_color(source.material_color);
    material.EmissiveColor = irr::video::SColor(0, 0, 0, 0);
    if (!source.diffuse_texture.empty()) {
        irr::video::ITexture* texture = resolver ? resolver(source, user_data) : nullptr;
        if (texture) {
            material.setTexture(0, texture);
            ++output.mapped_textures;
        } else {
            ++output.unmapped_diffuse_textures;
        }
    }
    if (!source.alpha_texture.empty()) ++output.alpha_map_materials;
    irr::core::matrix4 texture_matrix;
    texture_matrix.setM(source.texture_matrix.data());
    material.setTextureMatrix(0, texture_matrix);
}

irr::scene::SMeshBuffer* make_buffer(
    const PrinceMeshPart& source, PrinceTextureResolver resolver,
    void* user_data, MutablePrinceMesh& output) {
    if (source.vertices.empty() || source.vertices.size() > 65535 ||
        source.indices.empty() || source.indices.size() % 3) return nullptr;
    auto* buffer = new (std::nothrow) irr::scene::SMeshBuffer();
    if (!buffer) return nullptr;
    buffer->Vertices.reallocate(static_cast<irr::u32>(source.vertices.size()));
    for (const auto& vertex : source.vertices) {
        for (float value : vertex.position) {
            if (!std::isfinite(value)) { buffer->drop(); return nullptr; }
        }
        buffer->Vertices.push_back(irr::video::S3DVertex(
            vertex.position[0], vertex.position[1], vertex.position[2],
            0.0f, 0.0f, 0.0f, to_irr_color(vertex.color),
            vertex.uv[0], vertex.uv[1]));
    }
    buffer->Indices.reallocate(static_cast<irr::u32>(source.indices.size()));
    for (const auto index : source.indices) {
        if (index >= source.vertices.size()) { buffer->drop(); return nullptr; }
        buffer->Indices.push_back(index);
    }
    apply_source_material(source, *buffer, resolver, user_data, output);
    calculate_normals(*buffer);
    buffer->recalculateBoundingBox();
    buffer->setHardwareMappingHint(irr::scene::EHM_DYNAMIC,
                                   irr::scene::EBT_VERTEX);
    return buffer;
}

bool update_buffer(const PrinceMeshPart& source,
                   irr::scene::SMeshBuffer& buffer) {
    if (buffer.Vertices.size() != source.vertices.size()) return false;
    for (irr::u32 i = 0; i < buffer.Vertices.size(); ++i) {
        const auto& position = source.vertices[i].position;
        if (!std::isfinite(position[0]) || !std::isfinite(position[1]) ||
            !std::isfinite(position[2])) return false;
        buffer.Vertices[i].Pos.set(position[0], position[1], position[2]);
        buffer.Vertices[i].Normal.set(0.0f, 0.0f, 0.0f);
    }
    calculate_normals(buffer);
    buffer.recalculateBoundingBox();
    buffer.setDirty(irr::scene::EBT_VERTEX);
    return true;
}

} // namespace

MutablePrinceMesh::~MutablePrinceMesh() {
    if (mesh) mesh->drop();
}

MutablePrinceMesh::MutablePrinceMesh(MutablePrinceMesh&& other) noexcept
    : mesh(other.mesh), buffers(std::move(other.buffers)),
      mapped_textures(other.mapped_textures),
      unmapped_diffuse_textures(other.unmapped_diffuse_textures),
      alpha_map_materials(other.alpha_map_materials) {
    other.mesh = nullptr;
    other.mapped_textures = other.unmapped_diffuse_textures =
        other.alpha_map_materials = 0;
}

MutablePrinceMesh& MutablePrinceMesh::operator=(MutablePrinceMesh&& other) noexcept {
    if (this == &other) return *this;
    if (mesh) mesh->drop();
    mesh = other.mesh;
    buffers = std::move(other.buffers);
    mapped_textures = other.mapped_textures;
    unmapped_diffuse_textures = other.unmapped_diffuse_textures;
    alpha_map_materials = other.alpha_map_materials;
    other.mesh = nullptr;
    other.mapped_textures = other.unmapped_diffuse_textures =
        other.alpha_map_materials = 0;
    return *this;
}

bool build_mutable_prince_mesh(const PrinceActor& source,
                               MutablePrinceMesh* output,
                               std::string& error,
                               PrinceTextureResolver texture_resolver,
                               void* texture_user_data) {
    error.clear();
    if (!output || !source.ready() || source.parts().empty()) {
        error = "Prince source or mutable mesh output is invalid";
        return false;
    }
    MutablePrinceMesh candidate;
    candidate.mesh = new (std::nothrow) irr::scene::SMesh();
    if (!candidate.mesh) {
        error = "Irrlicht Prince mesh allocation failed";
        return false;
    }
    candidate.buffers.reserve(source.parts().size());
    for (const auto& part : source.parts()) {
        auto* buffer = make_buffer(part, texture_resolver, texture_user_data,
                                   candidate);
        if (!buffer) {
            error = "Irrlicht rejected a Prince source primitive or allocation";
            return false;
        }
        candidate.mesh->addMeshBuffer(buffer);
        candidate.buffers.push_back(buffer);
        buffer->drop();
    }
    candidate.mesh->recalculateBoundingBox();
    *output = std::move(candidate);
    return true;
}

bool update_mutable_prince_mesh(const PrinceActor& source,
                                MutablePrinceMesh& output,
                                std::string& error) {
    error.clear();
    if (!output.mesh || output.buffers.size() != source.parts().size()) {
        error = "Mutable Prince buffer count differs from the current source pose";
        return false;
    }
    for (std::size_t i = 0; i < source.parts().size(); ++i) {
        if (!output.buffers[i] ||
            !update_buffer(source.parts()[i], *output.buffers[i])) {
            error = "Mutable Prince vertex stream could not be updated";
            return false;
        }
    }
    output.mesh->recalculateBoundingBox();
    return true;
}

} // namespace dh2::irrlicht_adapter
