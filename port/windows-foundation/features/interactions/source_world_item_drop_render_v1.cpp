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
constexpr const char* potion_visual = "dummy_itemdrop_Potion";
constexpr const char* gold_visual = "root_itemdrop_Gold_01";

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
        std::shared_ptr<const void> retention;
    };
    std::vector<std::uint8_t> bdae_bytes;
    dh2::resources::BresView bres{};
    dh2::scene::Scene scene;
    OriginalScene potion;
    CharacterVisual gold;
    std::map<std::string, std::vector<MeshSource>> visuals;
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
        const auto lookup_visual = [&](const char* expected, std::int32_t& index) {
            index = -1;
            for (std::size_t i = 0; i < rows.size(); ++i) {
                if (rows[i].visual != expected) continue;
                if (index >= 0) return false;
                index = static_cast<std::int32_t>(i);
            }
            return index >= 0;
        };
        std::int32_t potion_av = -1, gold_av = -1;
        if (!lookup_visual(potion_visual, potion_av) || !lookup_visual(gold_visual, gold_av))
            return fail(error, "Required exact Potion0/GoldStack01 AudioVisual visual rows");
        if (potion_av != 20 || gold_av != 13)
            return fail(error, "Itemdrop Visual rows differ from the source-backed ItemTable mapping");

        const auto potion_root = find_visual_root(source->scene, potion_visual);
        if (potion_root < 0 || !controllers_below(source->scene, potion_root).empty())
            return fail(error, "Potion0 source subtree is missing, ambiguous, or unexpectedly skinned");
        if (!decode_original_scene_module(source->bdae_bytes, potion_visual, identity(),
                                          source->potion, error)) return false;
        if (source->potion.mesh.ranges.empty() ||
            source->potion.mesh.ranges.size() != source->potion.materials.size())
            return fail(error, "Potion0 source subtree material/range mapping is incomplete");
        SourceAssetsV1::MeshSource potion_mesh;
        potion_mesh.mesh = &source->potion.mesh;
        potion_mesh.materials = &source->potion.materials;
        potion_mesh.retention = source;
        for (const auto& material : source->potion.materials) {
            const auto found = std::find_if(source->scene.materials.begin(), source->scene.materials.end(),
                [&](const auto& candidate) { return candidate.id == material.id; });
            if (found == source->scene.materials.end())
                return fail(error, "Potion0 material is absent from its source BRES scene");
            potion_mesh.scene_material_indices.push_back(
                static_cast<std::uint32_t>(std::distance(source->scene.materials.begin(), found)));
        }
        source->visuals[potion_visual].push_back(std::move(potion_mesh));

        const auto gold_root = find_visual_root(source->scene, gold_visual);
        if (gold_root < 0) return fail(error, "GoldStack01 source subtree is missing or ambiguous");
        const auto controller_ids = controllers_below(source->scene, gold_root);
        if (controller_ids.size() != 1)
            return fail(error, "GoldStack01 source visual does not select its proven single controller");
        dh2::skinning::Skin skin;
        if (!dh2::skinning::load(source->bres, controller_ids.front(), source->scene, skin, error))
            return false;
        CharacterVisualConfig config;
        config.model_path = itemdrop_uri;
        config.controller_ids.push_back(skin.id);
        config.expected_controller_count = 1;
        if (!source->gold.load(catalog, config, error)) return false;
        if (source->gold.meshes().empty() ||
            source->gold.meshes().size() != source->gold.original_materials().size())
            return fail(error, "GoldStack01 controller mesh/material mapping is incomplete");
        for (std::size_t i = 0; i < source->gold.meshes().size(); ++i) {
            const auto& mesh = source->gold.meshes()[i];
            const auto& material = source->gold.original_materials()[i];
            if (mesh.ranges.size() != 1 || mesh.ranges.front().indexCount == 0)
                return fail(error, "GoldStack01 source controller draw range is unsupported");
            const auto found = std::find_if(source->scene.materials.begin(), source->scene.materials.end(),
                [&](const auto& candidate) { return candidate.id == material.id; });
            if (found == source->scene.materials.end())
                return fail(error, "GoldStack01 material is absent from its source BRES scene");
            SourceAssetsV1::MeshSource gold_mesh;
            gold_mesh.mesh = &mesh;
            gold_mesh.materials = &source->gold.original_materials();
            gold_mesh.scene_material_indices.push_back(
                static_cast<std::uint32_t>(std::distance(source->scene.materials.begin(), found)));
            gold_mesh.retention = source;
            source->visuals[gold_visual].push_back(std::move(gold_mesh));
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
                frame->unresolved.push_back({item.identity, item.source_record.item_id,
                    item.source_visual_uri, "No source-backed geometry adapter is enrolled for this Visual URI"});
                continue;
            }
            loot::RuntimeWorldItemEntryV1 stored;
            if (!items_.inspect(item.identity, stored, error)) return false;
            if (!same_record(item.source_record, stored.source_outcome) ||
                stored.source_outcome.item_id != item.source_record.item_id ||
                stored.source_position != item.position || !stored.authored_item)
                return fail(error, "World-item render row no longer matches its exact same-store record/position");
            const auto world = translation({item.position[0], item.position[1], item.position[2]});
            for (const auto& source_mesh : found->second) {
                if (!source_mesh.mesh || !source_mesh.materials || !source_mesh.retention ||
                    source_mesh.mesh->ranges.size() != source_mesh.scene_material_indices.size() ||
                    source_mesh.mesh->ranges.size() != source_mesh.materials->size())
                    return fail(error, "Source itemdrop geometry/material lease is incomplete");
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
                    if (!services_.material(request, mesh.ranges[range_index].material, error)) return false;
                    const bool source_pass_ready =
                        mesh.ranges[range_index].material.sourcePass.has_value();
                    if (!source_pass_ready) frame->renderer_ready = false;
                    if (!request.authored_material->diffuse.empty() &&
                        !mesh.ranges[range_index].material.texture)
                        return fail(error, "Source itemdrop diffuse texture was not bound by root renderer");
                }
                SourceWorldItemDropDrawV1 draw;
                draw.identity = item.identity;
                draw.source_record = item.source_record;
                draw.item_id = item.source_record.item_id;
                draw.quantity = item.quantity;
                draw.source_position = item.position;
                draw.world = world;
                draw.visual_uri = item.source_visual_uri;
                draw.source_pass_ready = std::all_of(mesh.ranges.begin(), mesh.ranges.end(),
                    [](const auto& range) { return range.material.sourcePass.has_value(); });
                draw.mesh = nullptr;
                // Mesh is kept in a frame-owned copy so packets remain safe after the
                // next prepare call; the source lease also pins the exact BRES owner.
                auto mesh_pin = std::make_shared<Mesh>(std::move(mesh));
                draw.mesh = mesh_pin.get();
                draw.source_retention = std::make_shared<std::pair<std::shared_ptr<const void>,
                    std::shared_ptr<const Mesh>>>(source_mesh.retention, std::move(mesh_pin));
                frame->draws.push_back(std::move(draw));
            }
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

} // namespace dh::foundation::interactions
