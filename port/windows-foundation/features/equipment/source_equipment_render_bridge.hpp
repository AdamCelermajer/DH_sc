#pragma once
#include "../../../engine-skinning/visual_skin_owner_v6.hpp"
#include "../../renderer.hpp"
#include <functional>

namespace dh::foundation {
class AssetCatalog;
class CharacterVisual;

// Pins the exact body BRES image used to construct a source skin owner. The
// bridge checks both the image backing and the SAME live Scene identity.
struct SourceEquipmentImageLeaseV1 {
    std::shared_ptr<const void> retention;
    const std::uint8_t* bytes{};
    std::size_t byte_count{};
    dh2::resources::BresView image{};
    const dh2::scene::Scene* material_scene{};
    std::string resource_uri;
};

// Joint factory prevents pairing the renderer with a different body image or
// Scene: both the skin owner and image lease are produced from this body.
bool bind_source_equipment_render_skin(const AssetCatalog&, const CharacterVisual&,
    dh2::skinning::VisualAssetServicesV6,
    std::unique_ptr<dh2::skinning::VisualSkinOwnerV6>&,
    SourceEquipmentImageLeaseV1&, std::string& error);

struct SourceEquipmentDrawIdentityV1 {
    std::string resource_uri, geometry_id, material_id;
    std::int32_t category{-1}, module{-1}, weapon_slot{};
    std::uint32_t primitive_index{}, material_index{};
    std::uint64_t pose_revision{};
    bool skinned{}, positions_changed{}, source_color_missing{}, source_normal_missing{};
};
struct SourceEquipmentRenderPacketV1 {
    Mesh mesh;
    Mat4 world{};
    SourceEquipmentDrawIdentityV1 source;
    // Pins the real source image and the VisualSkinOwner draw topology.
    std::shared_ptr<const void> source_retention;
};
struct SourceEquipmentRenderFrameV1 { std::vector<SourceEquipmentRenderPacketV1> packets; };
struct SourceEquipmentRenderServicesV1 {
    // Weapon views use a different BRES/runtime retention than modular body
    // views. Resolve that exact URI/view; never reuse the body BRES image.
    std::function<bool(const dh2::skinning::VisualDrawViewV32&, const std::string&,
                       SourceEquipmentImageLeaseV1&, std::string&)> weapon_image;
    std::function<bool(const SourceEquipmentDrawIdentityV1&, const dh2::resources::BresView&,
                       const dh2::scene::Material&,
                       Material&, std::string&)> material;
    std::function<bool(std::shared_ptr<const SourceEquipmentRenderFrameV1>, std::string&)> submit;
};

// Reads current draw_views synchronously. Vertex positions are copied into the
// submitted frame because the owner invalidates them on its next draw_views;
// values, indices, UV/color/normal streams and world matrices are preserved.
class SourceEquipmentRenderBridgeV1 {
public:
    SourceEquipmentRenderBridgeV1(dh2::skinning::VisualSkinOwnerV6& owner,
        SourceEquipmentImageLeaseV1 image, SourceEquipmentRenderServicesV1 services)
        : owner_(owner), image_(std::move(image)), services_(std::move(services)) {}
    bool prepare(std::shared_ptr<const SourceEquipmentRenderFrameV1>&, std::string& error) const;
    bool submit(std::string& error) const;
private:
    dh2::skinning::VisualSkinOwnerV6& owner_;
    SourceEquipmentImageLeaseV1 image_;
    SourceEquipmentRenderServicesV1 services_;
};

} // namespace dh::foundation
