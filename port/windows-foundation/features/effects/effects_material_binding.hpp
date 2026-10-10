#pragma once
#include "effects_render_bridge.hpp"
#include "../../asset_catalog.hpp"
#include "../../texture_loader.hpp"
#include <map>
#include <string>
#include <utility>

namespace dh::foundation::effects {
struct EffectTextureServices {
    // Root invokes Renderer::createTexture with these decoded ORIGINAL pixels.
    std::function<bool(const TextureImage&,std::uint32_t&,std::string&)> upload;
    std::function<void(std::uint32_t)> release;
};
// Context-bound original texture uploads, separate from source FX resource pools.
// Clear only after all frame loans are drained, while the GL context is current.
class OriginalEffectMaterialBinding {
public:
    OriginalEffectMaterialBinding(const AssetCatalog& assets,EffectTextureServices services)
        : assets_(assets),services_(std::move(services)) {}
    ~OriginalEffectMaterialBinding();
    OriginalEffectMaterialBinding(const OriginalEffectMaterialBinding&)=delete;
    bool bind(const EffectDrawSource&,const dh2::scene::Material&,Material&,std::string&);
    void clear();
    std::size_t texture_count() const noexcept { return textures_.size(); }
private:
    const AssetCatalog& assets_;
    EffectTextureServices services_;
    std::map<std::filesystem::path,std::uint32_t> textures_;
    // B062: resolved texture path per (diffuse uri, owner resource uri); resolution probes the filesystem.
    std::map<std::pair<std::string,std::string>,std::filesystem::path> resolved_;
};
}
