#pragma once
#include "source_equipment_render_bridge.hpp"
#include "../../features/effects/effects_material_binding.hpp"

namespace dh::foundation {

// Feature-owned adapters over the existing COMMON pass/texture binder and
// AssetCatalog. Captured callbacks borrow this owner, which must outlive the
// bridge and all queued frames; clear/destroy only after renderer drain.
class SourceEquipmentOriginalBindingsV1 {
public:
    SourceEquipmentOriginalBindingsV1(const AssetCatalog&,
        effects::EffectTextureServices,
        std::function<bool(std::shared_ptr<const SourceEquipmentRenderFrameV1>,std::string&)> submit);
    SourceEquipmentOriginalBindingsV1(const AssetCatalog& source_assets,const AssetCatalog& texture_assets,
        effects::EffectTextureServices,
        std::function<bool(std::shared_ptr<const SourceEquipmentRenderFrameV1>,std::string&)> submit);
    SourceEquipmentRenderServicesV1 callbacks();
    void clear_textures() { materials_.clear(); }
    std::size_t texture_count() const noexcept { return materials_.texture_count(); }
private:
    bool weapon_image(const dh2::skinning::VisualDrawViewV32&,const std::string&,
                      SourceEquipmentImageLeaseV1&,std::string&);
    bool material(const SourceEquipmentDrawIdentityV1&,const dh2::resources::BresView&,
                  const dh2::scene::Material&,Material&,std::string&);
    const AssetCatalog& assets_;
    effects::OriginalEffectMaterialBinding materials_;
    std::function<bool(std::shared_ptr<const SourceEquipmentRenderFrameV1>,std::string&)> submit_;
};

} // namespace dh::foundation
