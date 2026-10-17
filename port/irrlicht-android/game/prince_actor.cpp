#include "prince_actor.hpp"

#include "../../asset-payloads/payloads.hpp"
#include "../../engine-resources/resources.hpp"
#include "../../engine-skinning/skinning.hpp"
#include "../../level-world/visual_motion.hpp"
#include "../../scene-materials/scene.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <utility>

namespace dh2::irrlicht_game {
namespace {

constexpr char kWarriorSkinSuffix[] = "_default_warrior-mesh-skin";
constexpr char kPrinceRootId[] = "prince_modular-node";

bool finite3(const std::array<float, 3>& p) {
    return std::isfinite(p[0]) && std::isfinite(p[1]) && std::isfinite(p[2]);
}

} // namespace

struct PrinceActor::Impl {
    struct PartSource {
        std::uint32_t skin_index = 0;
        std::vector<std::array<float, 3>> positions;
        std::vector<std::uint16_t> indices;
    };

    scene::Scene source_scene;
    std::vector<skinning::Skin> skins;
    std::vector<PartSource> part_sources;
    std::vector<PrinceMeshPart> parts;
    visual::SceneBinding binding;
    std::uint32_t vertex_count = 0;
    std::uint32_t triangle_count = 0;
    bool loaded = false;

    bool read_primitives(const resources::BresView& view,
                         const skinning::Skin& skin,
                         std::uint32_t skin_index,
                         scene::Instance& instance,
                         std::string& error) {
        assets::Mesh mesh{};
        if (dh2_mesh_open(&mesh, &view, static_cast<std::int32_t>(skin.geometry)) !=
                assets::Error::ok ||
            !mesh.vertices || mesh.vertices > 65535 || !mesh.primitives) {
            error = "Prince controller geometry is invalid or exceeds Irrlicht's 16-bit vertex limit";
            return false;
        }

        for (std::uint32_t primitive_index = 0;
             primitive_index < mesh.primitives; ++primitive_index) {
            assets::Primitive primitive{};
            if (dh2_mesh_primitive(&mesh, static_cast<std::int32_t>(primitive_index),
                                   &primitive) != assets::Error::ok ||
                primitive.collada_type || !primitive.index_count ||
                primitive.index_count % 3 || primitive.index_count > 3000000) {
                error = "Prince primitive topology is unsupported";
                return false;
            }
            // Attribute numbers in the geometry stream are not semantic
            // identities. Each primitive maps semantic slots (position 0,
            // color 2, UV 4) to its own attribute IDs.
            const auto position_attribute = primitive.attributes[0];
            if (position_attribute < 0) {
                error = "Prince primitive has no semantic position attribute";
                return false;
            }
            assets::Attribute positions{};
            if (dh2_mesh_attribute(&mesh, position_attribute, &positions) !=
                    assets::Error::ok || positions.components < 3 ||
                positions.vertices != mesh.vertices ||
                mesh.vertices != skin.influences.size()) {
                error = "Prince primitive position stream differs from the skin weights";
                return false;
            }
            const auto uv_attribute = primitive.attributes[4];
            assets::Attribute uv{};
            bool have_uv = false;
            if (uv_attribute >= 0) {
                if (dh2_mesh_attribute(&mesh, uv_attribute, &uv) !=
                        assets::Error::ok || uv.components < 2 ||
                    uv.vertices != mesh.vertices) {
                    error = "Prince primitive semantic UV stream is invalid";
                    return false;
                }
                have_uv = true;
            }
            const auto color_attribute = primitive.attributes[2];
            assets::Attribute color{};
            bool have_color = false;
            if (color_attribute >= 0) {
                if (dh2_mesh_attribute(&mesh, color_attribute, &color) !=
                        assets::Error::ok || !color.components ||
                    color.vertices != mesh.vertices) {
                    error = "Prince primitive semantic color stream is invalid";
                    return false;
                }
                have_color = true;
            }

            std::vector<std::array<float, 3>> rest(mesh.vertices);
            std::vector<std::array<float, 2>> texture_coordinates(mesh.vertices);
            std::vector<std::array<float, 4>> colors(
                mesh.vertices, {1.0f, 1.0f, 1.0f, 1.0f});
            for (std::uint32_t vertex = 0; vertex < mesh.vertices; ++vertex) {
                float value[4]{};
                if (!dh2_attribute_read(&positions, vertex, value)) {
                    error = "Prince semantic position attribute could not be decoded";
                    return false;
                }
                rest[vertex] = {value[0], value[1], value[2]};
                if (!finite3(rest[vertex])) {
                    error = "Prince source position is nonfinite";
                    return false;
                }
                if (have_uv) {
                    if (!dh2_attribute_read(&uv, vertex, value) ||
                        !std::isfinite(value[0]) || !std::isfinite(value[1])) {
                        error = "Prince semantic UV attribute could not be decoded";
                        return false;
                    }
                    texture_coordinates[vertex] = {value[0], value[1]};
                }
                if (have_color) {
                    if (!dh2_attribute_read(&color, vertex, value)) {
                        error = "Prince semantic vertex-color attribute could not be decoded";
                        return false;
                    }
                    const float scale = color.type == 1 ? 1.0f / 255.0f : 1.0f;
                    for (std::uint32_t component = 0;
                         component < std::min<std::uint32_t>(4, color.components);
                         ++component) {
                        colors[vertex][component] = value[component] * scale;
                    }
                }
            }
            const auto material = std::find_if(
                source_scene.materials.begin(), source_scene.materials.end(),
                [&](const scene::Material& candidate) {
                    return candidate.id == (primitive.material ? primitive.material : "");
                });
            if (material == source_scene.materials.end()) {
                error = "Prince primitive references an unresolved source material";
                return false;
            }
            const auto material_index = static_cast<std::uint32_t>(
                material - source_scene.materials.begin());

            PrinceMeshPart output;
            output.skin_index = skin_index;
            output.geometry_index = skin.geometry;
            output.primitive_index = primitive_index;
            output.position_attribute = position_attribute;
            output.uv_attribute = uv_attribute;
            output.color_attribute = color_attribute;
            output.material_index = material_index;
            output.material_id = material->id;
            output.diffuse_texture = material->diffuse;
            output.alpha_texture = material->alpha_map;
            std::copy(material->color, material->color + 4,
                      output.material_color.begin());
            std::copy(material->texture_matrix, material->texture_matrix + 16,
                      output.texture_matrix.begin());
            output.vertices.resize(mesh.vertices);
            for (std::uint32_t vertex = 0; vertex < mesh.vertices; ++vertex) {
                output.vertices[vertex].rest_position = rest[vertex];
                output.vertices[vertex].position = rest[vertex];
                output.vertices[vertex].uv = texture_coordinates[vertex];
                output.vertices[vertex].color = colors[vertex];
            }
            output.indices.reserve(primitive.index_count);
            for (std::uint32_t index = 0; index < primitive.index_count; ++index) {
                std::uint32_t decoded = 0;
                if (!dh2_index_read(&primitive, index, &decoded) || decoded >= mesh.vertices) {
                    error = "Prince primitive index is outside its vertex buffer";
                    return false;
                }
                output.indices.push_back(static_cast<std::uint16_t>(decoded));
            }

            instance.materials.push_back(material_index);
            PartSource source_part;
            source_part.skin_index = skin_index;
            source_part.positions = rest;
            source_part.indices = output.indices;
            part_sources.push_back(std::move(source_part));
            parts.push_back(std::move(output));
            vertex_count += mesh.vertices;
            triangle_count += primitive.index_count / 3;
        }
        return true;
    }
};

PrinceActor::PrinceActor() : impl_(std::make_unique<Impl>()) {}
PrinceActor::~PrinceActor() = default;
PrinceActor::PrinceActor(PrinceActor&&) noexcept = default;
PrinceActor& PrinceActor::operator=(PrinceActor&&) noexcept = default;

bool PrinceActor::load(const std::uint8_t* model, std::size_t model_size,
                       std::string& error) {
    error.clear();
    if (!impl_ || !model || !model_size) {
        error = "Prince model source is required";
        return false;
    }

    Impl candidate;
    resources::BresView view{};
    if (dh2_bres_open(&view, model, model_size) != resources::BresError::ok ||
        !scene::load(view, candidate.source_scene, error)) {
        if (error.empty()) error = "Prince model BRES rejected";
        return false;
    }
    const auto root = std::find_if(candidate.source_scene.graph.begin(),
        candidate.source_scene.graph.end(), [](const scene::Node& node) {
            return node.id == kPrinceRootId;
        });
    if (root == candidate.source_scene.graph.end()) {
        error = "Prince modular rig root is absent";
        return false;
    }
    const auto root_node = static_cast<std::uint32_t>(
        root - candidate.source_scene.graph.begin());
    candidate.source_scene.instances.clear();
    const auto controllers = dh2_bres_library_count(
        &view, resources::Library::controller);
    for (std::uint32_t controller = 0; controller < controllers; ++controller) {
        skinning::Skin skin;
        if (!skinning::load(view, controller, candidate.source_scene, skin, error)) {
            error.clear();
            continue;
        }
        if (skin.id.find(kWarriorSkinSuffix) == std::string::npos) continue;
        if (std::any_of(candidate.skins.begin(), candidate.skins.end(),
                [&](const skinning::Skin& existing) { return existing.id == skin.id; })) {
            error = "Prince warrior controller identity is duplicated";
            return false;
        }
        for (const auto joint : skin.nodes) {
            if (joint >= candidate.source_scene.graph.size()) {
                error = "Prince warrior joint is outside the source scene graph";
                return false;
            }
        }
        const auto skin_index = static_cast<std::uint32_t>(candidate.skins.size());
        scene::Instance instance{
            root->id, root_node, skin.geometry, root->world, {}};
        instance.controller = static_cast<std::int32_t>(controller);
        if (!candidate.read_primitives(view, skin, skin_index, instance, error))
            return false;
        candidate.source_scene.instances.push_back(std::move(instance));
        candidate.skins.push_back(std::move(skin));
    }
    if (candidate.skins.size() != 4 || candidate.parts.empty()) {
        error = "Prince source does not provide exactly four default-warrior controllers";
        return false;
    }
    if (!candidate.binding.bind(candidate.source_scene, error)) {
        if (error.empty()) error = "Prince source visual binding rejected";
        return false;
    }
    candidate.loaded = true;
    *impl_ = std::move(candidate);
    return true;
}

dh2::scene::Scene& PrinceActor::scene() { return impl_->source_scene; }
const dh2::scene::Scene& PrinceActor::scene() const { return impl_->source_scene; }
dh2::visual::SceneBinding& PrinceActor::visual_binding() { return impl_->binding; }

bool PrinceActor::deform(std::string& error) {
    error.clear();
    if (!ready()) {
        error = "Prince source rig is not ready";
        return false;
    }
    auto& impl = *impl_;
    if (!impl.binding.update_world(impl.source_scene, error)) return false;
    for (std::uint32_t skin_index = 0; skin_index < impl.skins.size(); ++skin_index) {
        std::vector<skinning::Matrix> palette;
        if (!skinning::palette(impl.skins[skin_index], impl.source_scene,
                               palette, error)) return false;
        for (std::size_t part_index = 0;
             part_index < impl.parts.size(); ++part_index) {
            const auto& part = impl.parts[part_index];
            if (part.skin_index != skin_index) continue;
            const auto& rest = impl.part_sources[part_index].positions;
            std::vector<std::array<float, 3>> deformed;
            if (!skinning::positions(impl.skins[skin_index], palette, rest,
                                     deformed, error) ||
                deformed.size() != part.vertices.size()) {
                if (error.empty()) error = "Prince source deformation dimensions differ";
                return false;
            }
            for (std::size_t vertex = 0; vertex < deformed.size(); ++vertex) {
                if (!finite3(deformed[vertex])) {
                    error = "Prince animated position is nonfinite";
                    return false;
                }
                impl.parts[part_index].vertices[vertex].position = deformed[vertex];
            }
        }
    }
    return true;
}

const std::vector<PrinceMeshPart>& PrinceActor::parts() const {
    static const std::vector<PrinceMeshPart> empty;
    return impl_ ? impl_->parts : empty;
}
std::uint32_t PrinceActor::controller_count() const {
    return impl_ ? static_cast<std::uint32_t>(impl_->skins.size()) : 0;
}
std::uint32_t PrinceActor::joint_count() const {
    if (!impl_) return 0;
    std::uint32_t count = 0;
    for (const auto& skin : impl_->skins)
        count += static_cast<std::uint32_t>(skin.nodes.size());
    return count;
}
std::uint32_t PrinceActor::vertex_count() const {
    return impl_ ? impl_->vertex_count : 0;
}
std::uint32_t PrinceActor::triangle_count() const {
    return impl_ ? impl_->triangle_count : 0;
}
bool PrinceActor::ready() const { return impl_ && impl_->loaded; }

} // namespace dh2::irrlicht_game
