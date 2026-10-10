#pragma once

#include "session_world_item_consumer_v1.hpp"
#include "../../original_scene.hpp"
#include "../../original_character.hpp"
#include "../../renderer.hpp"
#include <functional>
#include <memory>

namespace dh::foundation {
class AssetCatalog;
}

namespace dh::foundation::interactions {

// A draw packet is a borrow of geometry decoded from the original
// itemdrops.bdae and one exact record in the caller's existing world store.
// The source lease pins both the BRES/Scene and any CharacterVisual pose data.
struct SourceWorldItemDropDrawV1 {
    loot::RuntimeWorldItemIdV1 identity{loot::invalid_runtime_world_item_v1};
    loot::RuntimeWorldItemRecordV1 source_record;
    std::int32_t item_id{-1};
    std::uint32_t quantity{};
    std::array<float, 3> source_position{};
    Mat4 world{};
    std::string visual_uri;
    bool source_pass_ready{};
    const Mesh* mesh{};
    std::shared_ptr<const void> source_retention;
};

struct SourceWorldItemDropUnresolvedV1 {
    loot::RuntimeWorldItemIdV1 identity{loot::invalid_runtime_world_item_v1};
    std::int32_t item_id{-1};
    std::string visual_uri;
    std::string reason;
};

struct SourceWorldItemDropRenderFrameV1 {
    std::vector<SourceWorldItemDropDrawV1> draws;
    std::vector<SourceWorldItemDropUnresolvedV1> unresolved;
    bool renderer_ready{true};
};

struct SourceWorldItemDropMaterialV1 {
    std::string resource_uri;
    std::string visual_uri;
    std::string item_identifier;
    std::uint32_t range_index{};
    const OriginalMaterial* authored_material{};
    const dh2::scene::Material* authored_scene_material{};
    const dh2::resources::BresView* source_image{};
};

struct SourceWorldItemDropRenderServicesV1 {
    // Resolve/upload the exact source material and texture for the current
    // renderer. This callback must preserve its authored pass and UV policy.
    std::function<bool(const SourceWorldItemDropMaterialV1&, Material&,
                       std::string&)> material;
    // Root submits to its current RenderQueue and retains this frame until all
    // borrowed mesh/range submissions have drained. No frame/camera is owned here.
    std::function<bool(std::shared_ptr<const SourceWorldItemDropRenderFrameV1>,
                       std::string&)> submit;
};

// Source-backed renderer projection over one existing RuntimeWorldItemAdapter.
// The initial source bank intentionally contains only the proven Potion0 static
// subtree and GoldStack01 skinned authored rest pose. It adds no object store,
// physics object, bob/spin, animation clock, or item selection policy.
class SourceWorldItemDropRenderV1 {
public:
    static bool load(AssetCatalog&, loot::RuntimeWorldItemAdapterV1&,
                     dh2::data::LootAudioVisualV8::Borrow,
                     SourceWorldItemDropRenderServicesV1,
                     std::unique_ptr<SourceWorldItemDropRenderV1>&,
                     std::string& error);

    bool prepare(std::shared_ptr<const SourceWorldItemDropRenderFrameV1>&,
                 std::string& error) const;
    bool submit(std::string& error) const;

private:
    struct SourceAssetsV1;
    SourceWorldItemDropRenderV1(loot::RuntimeWorldItemAdapterV1&,
        dh2::data::LootAudioVisualV8::Borrow,
        SourceWorldItemDropRenderServicesV1,
        std::shared_ptr<SourceAssetsV1>);

    loot::RuntimeWorldItemAdapterV1& items_;
    SessionWorldItemConsumerV1 consumer_;
    SourceWorldItemDropRenderServicesV1 services_;
    std::shared_ptr<SourceAssetsV1> assets_;
};

} // namespace dh::foundation::interactions
