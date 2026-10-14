#include "runtime_session_projectile_render_v1.hpp"

#include "../../content_paths.hpp"
#include "runtime_session_projectile_v1.hpp"

#include <cmath>
#include <cstring>
#include <utility>

namespace dh::foundation::effects {
namespace {
constexpr const char* kFireBoltUri = "data/3D/projectiles/elemental_bolt_fire.bdae";
// v21.8 magewire: the skill projectile models (ProjectileTable rows SimplifiedChainLightning / Tornado).
constexpr const char* kChainLightningUri = "data/3D/projectiles/projectile_dh2_simplified_chain_lightning.bdae";
constexpr const char* kTornadoUri = "data/3D/projectiles/projectile_dh2_prince_mage_cyclone.bdae";
// v21.8 w2-archer: the Archer arrow models (ProjectileDict 9/10/12/13 = ProjectileTable rows ArrowDefault/BowProjectile, ArrowExplode, ArrowPinShot, ArrowPoison).
constexpr const char* kArrowDefaultUri = "data/3D/projectiles/projectile_arrow_default.bdae";
constexpr const char* kArrowExplodeUri = "data/3D/projectiles/projectile_arrow_explode.bdae";
constexpr const char* kArrowPinShotUri = "data/3D/projectiles/projectile_arrow_pin_shot.bdae";
constexpr const char* kArrowPoisonUri = "data/3D/projectiles/projectile_arrow_poison.bdae";

bool fail(std::string& error, const char* reason) {
    error = reason;
    return false;
}

bool finite(const std::array<float, 3>& value) {
    return std::isfinite(value[0]) && std::isfinite(value[1]) &&
           std::isfinite(value[2]);
}

Mat4 projectile_world(const std::array<float, 3>& position, float yaw) {
    const float c = std::cos(yaw), s = std::sin(yaw);
    Mat4 world{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    world[0] = c;
    world[1] = s;
    world[4] = -s;
    world[5] = c;
    world[12] = position[0];
    world[13] = position[1];
    world[14] = position[2];
    return world;
}

bool same_material(const OriginalMaterial& projected,
                    const dh2::scene::Material& source) {
    return projected.id == source.id && projected.diffuse == source.diffuse &&
        projected.alphaMap == source.alpha_map &&
        projected.effectFile == source.effect_file &&
        projected.technique == source.gles2_technique &&
        std::memcmp(projected.textureMatrix.data(), source.texture_matrix,
                    sizeof(source.texture_matrix)) == 0 &&
        projected.alphaReference == source.alpha_ref &&
        projected.additive == source.additive;
}
}

struct RuntimeSessionProjectileRenderV1::SourceV1 {
    std::vector<std::uint8_t> bytes;
    dh2::resources::BresView image{};
    dh2::scene::Scene scene;
    OriginalScene decoded;
    std::vector<std::uint32_t> source_material_indices;
};

RuntimeSessionProjectileRenderV1::RuntimeSessionProjectileRenderV1(
    RuntimeSessionProjectileRenderServicesV1 services,
    std::map<std::string, std::shared_ptr<SourceV1>> sources)
    : services_(std::move(services)), sources_(std::move(sources)) {}

bool RuntimeSessionProjectileRenderV1::load_source(
    AssetCatalog& assets, const char* uri, std::shared_ptr<SourceV1>& output,
    std::string& error) {
    output.reset();
    auto source = std::make_shared<SourceV1>();
    source->bytes = read_content(assets, uri);
    if (dh2_bres_open(&source->image, source->bytes.data(), source->bytes.size()) !=
        dh2::resources::BresError::ok)
        return fail(error, "Actual projectile BRES is invalid");
    if (!dh2::scene::load(source->image, source->scene, error)) return false;
    if (!decode_original_scene(source->bytes, source->decoded, error)) return false;
    if (source->decoded.mesh.vertices.empty() || source->decoded.mesh.indices.empty() ||
        source->decoded.mesh.ranges.size() != source->decoded.materials.size())
        return fail(error, "Actual projectile OriginalScene geometry/material rows are incomplete");
    source->source_material_indices.reserve(source->decoded.materials.size());
        for (const auto& projected : source->decoded.materials) {
            std::uint32_t match = 0;
            unsigned count = 0;
            for (std::size_t i = 0; i < source->scene.materials.size(); ++i) {
                if (source->scene.materials[i].id != projected.id) continue;
                match = static_cast<std::uint32_t>(i);
                ++count;
            }
            if (count != 1 || !same_material(projected, source->scene.materials[match]))
                return fail(error, "OriginalScene material no longer matches its exact BRES material identity");
            source->source_material_indices.push_back(match);
        }
    output = std::move(source);
    error.clear();
    return true;
}

bool RuntimeSessionProjectileRenderV1::load(
    AssetCatalog& assets, RuntimeSessionProjectileRenderServicesV1 services,
    std::unique_ptr<RuntimeSessionProjectileRenderV1>& output,
    std::string& error) {
    error.clear();
    output.reset();
    if (!services.material)
        return fail(error, "Projectile renderer requires the original material/pass binder");
    if (!services.submit)
        return fail(error, "Projectile renderer requires the current RenderQueue submit callback");
    try {
        std::map<std::string, std::shared_ptr<SourceV1>> sources;
        std::shared_ptr<SourceV1> fire;
        if (!load_source(assets, kFireBoltUri, fire, error)) return false;
        sources.emplace(kFireBoltUri, std::move(fire));
        // The skill projectiles are optional for the staff attack: when a package lacks their BDAE, their contact and draw
        // report the missing source explicitly (no AABB enrolled / unresolved model) instead of failing the whole renderer.
        for (const char* uri : {kChainLightningUri, kTornadoUri, kArrowDefaultUri, kArrowExplodeUri, kArrowPinShotUri, kArrowPoisonUri}) {
            std::shared_ptr<SourceV1> optional;
            std::string optional_error;
            try {
                if (load_source(assets, uri, optional, optional_error)) sources.emplace(uri, std::move(optional));
            } catch (const std::exception&) {
            }
        }
        // v21.8 w4-grapple: every ProjectileDict model of the original tables, not only the fixed list above. The executor
        // launches from these same tables, so any launched row (the Rogue GrapplingHook knife, a later skill) resolves its
        // model. Optional like the list: a model that fails to load is reported at its draw or contact, not here.
        {
            std::string tables_error;
            const auto tables = load_runtime_session_projectile_tables_v1(assets, tables_error);
            const auto* models = tables ? tables->group("ProjectileDict") : nullptr;
            if (models) {
                for (const auto& row : models->rows) {
                    for (const auto& field : row.fields) {
                        if (field.kind != dh2::android_ui::ProcessArrayValueV101::Kind::string || field.text.empty()) continue;
                        if (sources.count(field.text)) continue;
                        std::shared_ptr<SourceV1> dict_source;
                        std::string dict_error;
                        try {
                            if (load_source(assets, field.text.c_str(), dict_source, dict_error)) sources.emplace(field.text, std::move(dict_source));
                        } catch (const std::exception&) {
                        }
                    }
                }
            }
        }
        output.reset(new RuntimeSessionProjectileRenderV1(std::move(services), std::move(sources)));
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

bool RuntimeSessionProjectileRenderV1::prepare(
    const std::vector<RuntimeSessionProjectileVisualV1>& visuals,
    std::shared_ptr<const RuntimeSessionProjectileRenderFrameV1>& output,
    std::string& error) const {
    error.clear();
    auto frame = std::make_shared<RuntimeSessionProjectileRenderFrameV1>();
    try {
        for (const auto& visual : visuals) {
            if (visual.projectile_id == 0 || visual.owner == invalid_actor_id ||
                visual.source_row < 0 || visual.source_model < 0 ||
                !finite(visual.position) || !std::isfinite(visual.yaw_radians))
                return fail(error, "Projectile renderer received an invalid live visual packet");
            const auto found = sources_.find(visual.model_uri);
            if (found == sources_.end() || !found->second) {
                frame->renderer_ready = false;
                frame->unresolved.push_back({visual.projectile_id, visual.model_uri,
                    "No OriginalScene projectile adapter is enrolled for this source URI"});
                continue;
            }
            const auto& source = found->second;

            auto mesh = std::make_shared<Mesh>(source->decoded.mesh);
            bool pass_ready = true;
            for (std::size_t range_index = 0; range_index < mesh->ranges.size(); ++range_index) {
                const auto material_index = source->source_material_indices.at(range_index);
                const auto& source_material = source->scene.materials.at(material_index);
                RuntimeSessionProjectileMaterialRequestV1 request;
                request.resource_uri = visual.model_uri;
                request.range_index = static_cast<std::uint32_t>(range_index);
                request.authored_material = &source->decoded.materials.at(range_index);
                request.source_material = &source_material;
                request.source_image = &source->image;
                auto& material = mesh->ranges[range_index].material;
                if (!services_.material(request, material, error)) return false;
                if (!material.sourcePass) pass_ready = false;
                if ((!source_material.diffuse.empty() || !source_material.alpha_map.empty()) &&
                    material.texture == 0)
                    return fail(error, "Actual projectile source texture was not bound by the renderer callback");
            }
            frame->renderer_ready = frame->renderer_ready && pass_ready;
            RuntimeSessionProjectileDrawV1 draw;
            draw.projectile_id = visual.projectile_id;
            draw.owner = visual.owner;
            draw.source_row = visual.source_row;
            draw.source_model = visual.source_model;
            draw.position = visual.position;
            draw.yaw_radians = visual.yaw_radians;
            draw.world = projectile_world(visual.position, visual.yaw_radians);
            draw.mesh = mesh.get();
            draw.mesh_lease = mesh;
            draw.source_pass_ready = pass_ready;
            draw.source_retention = source;
            frame->draws.push_back(std::move(draw));
        }
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
    output = std::move(frame);
    error.clear();
    return true;
}

bool RuntimeSessionProjectileRenderV1::submit(
    const std::vector<RuntimeSessionProjectileVisualV1>& visuals,
    std::string& error) const {
    std::shared_ptr<const RuntimeSessionProjectileRenderFrameV1> frame;
    if (!prepare(visuals, frame, error)) return false;
    if (!frame->renderer_ready)
        return fail(error, "Projectile frame contains unresolved source model or material/pass data");
    return services_.submit(std::move(frame), error);
}

bool RuntimeSessionProjectileRenderV1::source_projectile_bounds_v1(
    const std::string& model_uri, std::array<float, 3>& minimum,
    std::array<float, 3>& maximum, std::string& error) const {
    const auto found = sources_.find(model_uri);
    if (found == sources_.end() || !found->second)
        return fail(error, "No authored projectile AABB is enrolled for this source URI");
    const auto& source = found->second;
    // v21.8 magefix2: the source PhysicalObject shape comes from the GameObject relative AABB, which VisualObject::CalcMeshBox
    // (0x47211c) takes from the `_colbox_` node of the model (VisualObject::VisualObject 0x472a0c finds it with
    // SearchByName("_colbox_")), not from the visible meshes. Every projectile BDAE authors one (the fire bolt's visible
    // glow quad is 324 units wide, its colbox 11.4).
    {
        const auto& graph = source->scene.graph;
        std::int32_t colbox = -1;
        for (std::size_t i = 0; i < graph.size(); ++i)
            if (graph[i].name.rfind("_colbox_", 0) == 0) { colbox = static_cast<std::int32_t>(i); break; }
        if (colbox >= 0) {
            for (const auto& instance : source->scene.instances) {
                const auto node = static_cast<std::int32_t>(instance.node_index);
                if (node < 0 || static_cast<std::size_t>(node) >= graph.size() ||
                    (node != colbox && graph[node].parent != colbox)) continue;
                dh2::assets::Mesh mesh{};
                if (dh2_mesh_open(&mesh, &source->image, instance.geometry) != dh2::assets::Error::ok) continue;
                // CalcMeshBox (0x47211c) multiplies the colbox mesh node's box by its PARENT's ISceneNode::getScale. In the
                // source loader (CColladaDatabase::constructNode 0x61b2f4) a node's geometry instance is an identity mesh
                // CHILD of the node that carries the authored TRS, so that parent is the authored node of the instance.
                // The box is then taken through the collada root's relative matrix; the root is the empty CRootSceneNode
                // wrapper, identity at construction (inference: nothing sets it before CalcMeshBox).
                const std::array<float, 3> scale{graph[node].scale[0], graph[node].scale[1], graph[node].scale[2]};
                std::array<float, 3> half{};
                bool valid = true;
                for (unsigned axis = 0; axis < 3; ++axis) {
                    const float low = mesh.minimum[axis] * scale[axis];
                    const float high = mesh.maximum[axis] * scale[axis];
                    half[axis] = std::fabs(high - low) * 0.5f;
                    valid = valid && std::isfinite(half[axis]);
                }
                if (!valid) continue;
                // CalcMeshBox centres the box on the origin; GameObject::SetRelativeAABB (0x38b110) widens an X or Y extent
                // narrower than 10 to 10.
                if (half[0] * 2.0f < 10.0f) half[0] = 5.0f;
                if (half[1] * 2.0f < 10.0f) half[1] = 5.0f;
                minimum = {-half[0], -half[1], -half[2]};
                maximum = {half[0], half[1], half[2]};
                error.clear();
                return true;
            }
        }
    }
    if (source->decoded.mesh.vertices.empty() ||
        !std::isfinite(source->decoded.minimum.x) ||
        !std::isfinite(source->decoded.minimum.y) ||
        !std::isfinite(source->decoded.maximum.x) ||
        !std::isfinite(source->decoded.maximum.y) ||
        source->decoded.minimum.x > source->decoded.maximum.x ||
        source->decoded.minimum.y > source->decoded.maximum.y)
        return fail(error, "Actual projectile BDAE absolute bounds are unavailable");
    minimum = {source->decoded.minimum.x, source->decoded.minimum.y,
               source->decoded.minimum.z};
    maximum = {source->decoded.maximum.x, source->decoded.maximum.y,
               source->decoded.maximum.z};
    error.clear();
    return true;
}

} // namespace dh::foundation::effects
