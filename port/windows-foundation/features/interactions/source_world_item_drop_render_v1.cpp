#include "source_world_item_drop_render_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../content_paths.hpp"
#include "../../../engine-skinning/skinning.hpp"
#include <algorithm>
#include <map>
#include <set>

namespace dh::foundation::interactions {
namespace {
bool fail(std::string& error, const char* reason) { error = reason; return false; }

constexpr const char* itemdrop_uri = "data/3D/GameObjects/itemdrops.bdae";

std::int32_t find_visual_root(const dh2::scene::Scene& scene,
                              const std::string& authored_name) {
    std::int32_t result = -1;
    for (std::size_t i = 0; i < scene.graph.size(); ++i) {
        const auto& node = scene.graph[i];
        if (node.name == authored_name || node.id == authored_name || node.sid == authored_name) {
            if (result >= 0) return -2;
            result = static_cast<std::int32_t>(i);
        }
    }
    return result;
}

std::vector<std::uint32_t> controllers_below(const dh2::scene::Scene& scene,
                                             std::int32_t root) {
    std::set<std::uint32_t> result;
    for (const auto& instance : scene.instances) {
        auto node = static_cast<std::int32_t>(instance.node_index);
        while (node >= 0 && node != root)
            node = scene.graph.at(static_cast<std::size_t>(node)).parent;
        if (node == root && instance.controller >= 0)
            result.insert(static_cast<std::uint32_t>(instance.controller));
    }
    return {result.begin(), result.end()};
}

bool same_record(const loot::RuntimeWorldItemRecordV1& a,
                 const loot::RuntimeWorldItemRecordV1& b) noexcept {
    return a.source_actor == b.source_actor && a.killer_actor == b.killer_actor &&
           a.loot_table == b.loot_table && a.item_id == b.item_id &&
           a.quantity == b.quantity && a.authored_item == b.authored_item &&
           a.authored_entry == b.authored_entry &&
           a.resolved_gold_value == b.resolved_gold_value;
}
}

struct SourceWorldItemDropRenderV1::SourceAssetsV1 {
    struct MeshSource {
        const Mesh* mesh{};
        std::vector<std::uint32_t> scene_material_indices;
        const std::vector<OriginalMaterial>* materials{};
        std::shared_ptr<std::vector<OriginalMaterial>> owned_materials;
        std::shared_ptr<const void> retention;
        mutable std::shared_ptr<const Mesh> bound;   // bound mesh copy (textures/passes), reused each frame
        mutable bool bound_pass_ready{false};
    };
    std::vector<std::uint8_t> bdae_bytes;
    dh2::resources::BresView bres{};
    dh2::scene::Scene scene;
    std::vector<std::unique_ptr<OriginalScene>> statics;
    std::vector<std::unique_ptr<CharacterVisual>> skinned;
    std::map<std::string, std::vector<MeshSource>> visuals;
    std::map<std::string, std::string> unresolved_visuals;
};

SourceWorldItemDropRenderV1::SourceWorldItemDropRenderV1(
    loot::RuntimeWorldItemAdapterV1& items,
    dh2::data::LootAudioVisualV8::Borrow audiovisual,
    SourceWorldItemDropRenderServicesV1 services,
    std::shared_ptr<SourceAssetsV1> assets)
    : items_(items), consumer_(items, std::move(audiovisual)),
      services_(std::move(services)), assets_(std::move(assets)) {}

bool SourceWorldItemDropRenderV1::load(
    AssetCatalog& catalog, loot::RuntimeWorldItemAdapterV1& items,
    dh2::data::LootAudioVisualV8::Borrow audiovisual,
    SourceWorldItemDropRenderServicesV1 services,
    std::unique_ptr<SourceWorldItemDropRenderV1>& output,
    std::string& error) {
    error.clear();
    output.reset();
    if (!audiovisual) return fail(error, "Required original ItemAudioVisualTable borrow");
    if (!services.material) return fail(error, "Required original itemdrop source material binding");
    if (!services.submit) return fail(error, "Required root itemdrop RenderQueue submission callback");
    try {
        auto source = std::make_shared<SourceAssetsV1>();
        source->bdae_bytes = read_content(catalog, itemdrop_uri);
        if (dh2_bres_open(&source->bres, source->bdae_bytes.data(), source->bdae_bytes.size()) !=
            dh2::resources::BresError::ok)
            return fail(error, "Original itemdrops.bdae BRES could not be opened");
        if (!dh2::scene::load(source->bres, source->scene, error)) return false;

        const auto rows = audiovisual.rows();
        // P14: every ItemAudioVisualTable Visual string is resolved on its own.
        // A visual that cannot be proven (missing node, ambiguous skin, bad
        // material mapping) stays UNRESOLVED with a reason; it is never drawn
        // with another item's model.
        std::set<std::string> wanted;
        for (const auto& row : rows) if (!row.visual.empty()) wanted.insert(row.visual);

        for (const auto& visual_name : wanted) {
            const auto root = find_visual_root(source->scene, visual_name);
            if (root < 0) {
                source->unresolved_visuals[visual_name] = root == -2 ?
                    "ambiguous source node" : "source node absent from itemdrops.bdae";
                continue;
            }
            const auto controller_ids = controllers_below(source->scene, root);
            std::string local_error;
            if (controller_ids.empty()) {
                auto scene = std::make_unique<OriginalScene>();
                if (!decode_original_scene_module(source->bdae_bytes, visual_name, identity(),
                                                  *scene, local_error) ||
                    scene->mesh.ranges.empty() || scene->mesh.ranges.size() != scene->materials.size()) {
                    source->unresolved_visuals[visual_name] =
                        "static subtree decode failed: " + local_error;
                    continue;
                }
                SourceAssetsV1::MeshSource mesh_source;
                mesh_source.mesh = &scene->mesh;
                mesh_source.materials = &scene->materials;
                mesh_source.retention = source;
                bool ok = true;
                for (const auto& material : scene->materials) {
                    const auto found = std::find_if(source->scene.materials.begin(), source->scene.materials.end(),
                        [&](const auto& candidate) { return candidate.id == material.id; });
                    if (found == source->scene.materials.end()) { ok = false; break; }
                    mesh_source.scene_material_indices.push_back(
                        static_cast<std::uint32_t>(std::distance(source->scene.materials.begin(), found)));
                }
                if (!ok) {
                    source->unresolved_visuals[visual_name] = "material absent from source BRES scene";
                    continue;
                }
                source->statics.push_back(std::move(scene));
                source->visuals[visual_name].push_back(std::move(mesh_source));
                continue;
            }
            if (controller_ids.size() != 1) {
                source->unresolved_visuals[visual_name] = "source visual has " +
                    std::to_string(controller_ids.size()) + " skin controllers (single controller required)";
                continue;
            }
            dh2::skinning::Skin skin;
            if (!dh2::skinning::load(source->bres, controller_ids.front(), source->scene, skin, local_error)) {
                source->unresolved_visuals[visual_name] = "skin load failed: " + local_error;
                continue;
            }
            auto visual = std::make_unique<CharacterVisual>();
            CharacterVisualConfig config;
            config.model_path = itemdrop_uri;
            config.controller_ids.push_back(skin.id);
            config.expected_controller_count = 1;
            if (!visual->load(catalog, config, local_error) || visual->meshes().empty() ||
                visual->meshes().size() != visual->original_materials().size()) {
                source->unresolved_visuals[visual_name] = "skinned rest pose load failed: " + local_error;
                continue;
            }
            std::vector<SourceAssetsV1::MeshSource> parts;
            bool ok = true;
            for (std::size_t i = 0; i < visual->meshes().size() && ok; ++i) {
                const auto& mesh = visual->meshes()[i];
                const auto& material = visual->original_materials()[i];
                if (mesh.ranges.size() != 1 || mesh.ranges.front().indexCount == 0) { ok = false; break; }
                const auto found = std::find_if(source->scene.materials.begin(), source->scene.materials.end(),
                    [&](const auto& candidate) { return candidate.id == material.id; });
                if (found == source->scene.materials.end()) { ok = false; break; }
                SourceAssetsV1::MeshSource part;
                part.mesh = &mesh;
                part.owned_materials = std::make_shared<std::vector<OriginalMaterial>>(1, material);
                part.materials = part.owned_materials.get();
                part.scene_material_indices.push_back(
                    static_cast<std::uint32_t>(std::distance(source->scene.materials.begin(), found)));
                part.retention = source;
                parts.push_back(std::move(part));
            }
            if (!ok) {
                source->unresolved_visuals[visual_name] = "skinned draw range/material mapping unsupported";
                continue;
            }
            source->skinned.push_back(std::move(visual));
            source->visuals[visual_name] = std::move(parts);
        }
        output.reset(new SourceWorldItemDropRenderV1(items, std::move(audiovisual),
                                                     std::move(services), std::move(source)));
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
}

bool SourceWorldItemDropRenderV1::prepare(
    std::shared_ptr<const SourceWorldItemDropRenderFrameV1>& output,
    std::string& error) const {
    error.clear();
    std::vector<SessionWorldItemPresentationV1> items;
    if (!consumer_.enumerate(items, error)) return false;
    auto frame = std::make_shared<SourceWorldItemDropRenderFrameV1>();
    try {
        for (const auto& item : items) {
            const auto found = assets_->visuals.find(item.source_visual_uri);
            if (!item.has_source_visual || found == assets_->visuals.end()) {
                const auto reason = assets_->unresolved_visuals.find(item.source_visual_uri);
                frame->unresolved.push_back({item.identity, item.source_record.item_id,
                    item.source_visual_uri, reason != assets_->unresolved_visuals.end() ? reason->second :
                    std::string("No source-backed geometry adapter is enrolled for this Visual URI")});
                continue;
            }
            loot::RuntimeWorldItemEntryV1 stored;
            if (!items_.inspect(item.identity, stored, error)) return false;
            if (!same_record(item.source_record, stored.source_outcome) ||
                stored.source_outcome.item_id != item.source_record.item_id ||
                stored.source_position != item.position || !stored.authored_item)
                return fail(error, "World-item render row no longer matches its exact same-store record/position");
            const auto world = translation({item.position[0], item.position[1], item.position[2]});
            std::vector<SourceWorldItemDropDrawV1> item_draws;
            std::string skip_reason;
            for (const auto& source_mesh : found->second) {
                if (!source_mesh.mesh || !source_mesh.materials || !source_mesh.retention ||
                    source_mesh.mesh->ranges.size() != source_mesh.scene_material_indices.size() ||
                    source_mesh.mesh->ranges.size() != source_mesh.materials->size())
                    return fail(error, "Source itemdrop geometry/material lease is incomplete");
                // Materials/textures are bound once per source mesh and reused.
                if (!source_mesh.bound) {
                    Mesh mesh = *source_mesh.mesh;
                    for (std::size_t range_index = 0; range_index < mesh.ranges.size(); ++range_index) {
                        const auto scene_index = source_mesh.scene_material_indices[range_index];
                        if (scene_index >= assets_->scene.materials.size())
                            return fail(error, "Source itemdrop material index is outside its retained scene");
                        SourceWorldItemDropMaterialV1 request;
                        request.resource_uri = item.source_model_resource_uri;
                        request.visual_uri = item.source_visual_uri;
                        request.item_identifier = item.item_identifier;
                        request.range_index = static_cast<std::uint32_t>(range_index);
                        request.authored_material = &source_mesh.materials->at(range_index);
                        request.authored_scene_material = &assets_->scene.materials[scene_index];
                        request.source_image = &assets_->bres;
                        std::string material_error;
                        if (!services_.material(request, mesh.ranges[range_index].material, material_error)) {
                            skip_reason = "material binding failed: " + material_error;
                            break;
                        }
                        if (!request.authored_material->diffuse.empty() &&
                            !mesh.ranges[range_index].material.texture) {
                            skip_reason = "diffuse texture was not bound by the root renderer";
                            break;
                        }
                    }
                    if (!skip_reason.empty()) break;
                    source_mesh.bound_pass_ready = std::all_of(mesh.ranges.begin(), mesh.ranges.end(),
                        [](const auto& range) { return range.material.sourcePass.has_value(); });
                    source_mesh.bound = std::make_shared<const Mesh>(std::move(mesh));
                }
                if (!source_mesh.bound_pass_ready) frame->renderer_ready = false;
                SourceWorldItemDropDrawV1 draw;
                draw.identity = item.identity;
                draw.source_record = item.source_record;
                draw.item_id = item.source_record.item_id;
                draw.quantity = item.quantity;
                draw.source_position = item.position;
                draw.world = world;
                draw.visual_uri = item.source_visual_uri;
                draw.source_pass_ready = source_mesh.bound_pass_ready;
                draw.mesh = source_mesh.bound.get();
                draw.source_retention = std::make_shared<std::pair<std::shared_ptr<const void>,
                    std::shared_ptr<const Mesh>>>(source_mesh.retention, source_mesh.bound);
                item_draws.push_back(std::move(draw));
            }
            if (!skip_reason.empty()) {
                frame->unresolved.push_back({item.identity, item.source_record.item_id,
                                             item.source_visual_uri, skip_reason});
                continue;
            }
            for (auto& draw : item_draws) frame->draws.push_back(std::move(draw));
        }
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    }
    output = std::move(frame);
    error.clear();
    return true;
}

bool SourceWorldItemDropRenderV1::submit(std::string& error) const {
    std::shared_ptr<const SourceWorldItemDropRenderFrameV1> frame;
    if (!prepare(frame, error)) return false;
    if (!frame->renderer_ready)
        return fail(error, "Source itemdrop frame has unresolved authored effect/pass data; RenderQueue submission is fail-closed");
    return services_.submit(std::move(frame), error);
}


std::vector<std::string> SourceWorldItemDropRenderV1::resolved_visuals() const {
    std::vector<std::string> names;
    for (const auto& pair : assets_->visuals) names.push_back(pair.first);
    return names;
}

const std::map<std::string, std::string>& SourceWorldItemDropRenderV1::unresolved_visuals() const {
    return assets_->unresolved_visuals;
}

} // namespace dh::foundation::interactions
