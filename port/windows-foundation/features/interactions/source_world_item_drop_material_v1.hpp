#pragma once

#include "source_world_item_drop_render_v1.hpp"
#include "../../texture_loader.hpp"
#include <map>

namespace dh::foundation::interactions {

struct SourceWorldItemDropTextureServicesV1 {
    std::function<bool(const TextureImage&, std::uint32_t&, std::string&)> upload;
    std::function<void(std::uint32_t)> release;
};

// Attempts the actual retained BRES COMMON/effect pass and decodes the exact
// diffuse/optional alpha textures. An unavailable effect leaves sourcePass
// unset so the frame can be inspected but never submitted. Texture lifetime is
// delegated to the caller's current renderer upload/release service.
class SourceWorldItemDropMaterialBindingsV1 {
public:
    SourceWorldItemDropMaterialBindingsV1(const AssetCatalog&,
                                          SourceWorldItemDropTextureServicesV1,
                                          const AssetCatalog* source_effect_assets = nullptr);
    ~SourceWorldItemDropMaterialBindingsV1();
    SourceWorldItemDropMaterialBindingsV1(const SourceWorldItemDropMaterialBindingsV1&) = delete;

    bool bind(const SourceWorldItemDropMaterialV1&, Material&, std::string& error);
    void clear_textures();
    std::size_t texture_count() const noexcept { return textures_.size(); }

private:
    struct ExternalEffectV1;
    const AssetCatalog& assets_;
    const AssetCatalog* source_effect_assets_{};
    SourceWorldItemDropTextureServicesV1 services_;
    std::map<std::string, std::uint32_t> textures_;
    std::map<std::string, std::shared_ptr<ExternalEffectV1>> external_effects_;
};

} // namespace dh::foundation::interactions
