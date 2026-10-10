#pragma once
#include "equipment_adapter.hpp"
#include "../../../engine-skinning/visual_skin_owner_v6.hpp"
namespace dh::foundation {
struct SourceEquipmentAppearanceStep {
    unsigned source_slot=0;
    std::string category,module,weapon_uri,anchor;
    std::int32_t category_id=-1,module_id=-1,weapon_mode=-1;
    bool equipped=false;
};
struct SourceEquipmentAppearancePlan {std::vector<SourceEquipmentAppearanceStep> steps;};
// Constructs one source rendering owner over the SAME retained CharacterVisual
// Scene. The root must consume its live draw_views before claiming body refresh.
// Source asset service and CharacterVisual must outlive the returned owner.
bool bind_source_equipment_skin(const AssetCatalog& body_assets,const CharacterVisual&,
    dh2::skinning::VisualAssetServicesV6,
    std::unique_ptr<dh2::skinning::VisualSkinOwnerV6>&,std::string&);
// Pure preparation against actual immutable ItemTable and actual skin module
// catalog. Source module naming, global lookup and placeholder fallback retained.
bool prepare_source_equipment_appearance(const CharacterState&,const dh2::data::ItemTable&,
    const std::vector<std::string>& slot_names,const dh2::skinning::VisualSkinOwnerV6&,
    SourceEquipmentAppearancePlan&,std::string&);
struct SourceAppearanceDebugServices {
    std::function<bool(std::string&)> load;
    std::function<bool(const char*,std::string&)> query;
};
// Applies recovered source order to the supplied retained owner. Source setters
// preserve delivered prefixes on service failure; this is not an atomic clone.
// Host prepares assets and its transaction before reaching this commit boundary.
bool apply_source_equipment_appearance(dh2::skinning::VisualSkinOwnerV6&,
    const SourceEquipmentAppearancePlan&,const SourceAppearanceDebugServices&,std::string&);
// Alternate renderer seam: stage external rigid attachments for the existing
// caller-owned EquipmentAttachmentSet using the SAME body pose/anchors. Host
// chooses this or native-owner weapon drawing, never both for the same weapon.
bool prepare_source_equipment_attachments(const AssetCatalog&,const CharacterVisual&,
    const SourceEquipmentAppearancePlan&,EquipmentAttachmentSet&,std::string&);
}
