#include "source_equipment_material_binding.hpp"
#include "../../content_paths.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace dh::foundation {
namespace {
struct WeaponImageOwnerV1 {
    std::vector<std::uint8_t> bytes;
    dh2::scene::Scene materials;
};
bool same_material(const dh2::scene::Material& a,const dh2::scene::Material& b) {
    return a.id==b.id&&a.diffuse==b.diffuse&&a.alpha_map==b.alpha_map&&
        a.effect_file==b.effect_file&&a.effect_uri==b.effect_uri&&a.gles2_technique==b.gles2_technique&&
        std::memcmp(a.color,b.color,sizeof(a.color))==0&&
        std::memcmp(a.texture_matrix,b.texture_matrix,sizeof(a.texture_matrix))==0&&
        a.alpha_ref==b.alpha_ref&&a.additive==b.additive&&a.backface==b.backface;
}
bool same_material_table(const std::vector<dh2::scene::Material>& a,
                         const std::vector<dh2::scene::Material>& b) {
    if(a.size()!=b.size())return false;
    for(std::size_t i=0;i<a.size();++i)if(!same_material(a[i],b[i]))return false;
    return true;
}
}

SourceEquipmentOriginalBindingsV1::SourceEquipmentOriginalBindingsV1(const AssetCatalog& assets,
    effects::EffectTextureServices textures,
    std::function<bool(std::shared_ptr<const SourceEquipmentRenderFrameV1>,std::string&)> submit)
    :SourceEquipmentOriginalBindingsV1(assets,assets,std::move(textures),std::move(submit)) {}
SourceEquipmentOriginalBindingsV1::SourceEquipmentOriginalBindingsV1(const AssetCatalog& assets,
    const AssetCatalog& texture_assets,effects::EffectTextureServices textures,
    std::function<bool(std::shared_ptr<const SourceEquipmentRenderFrameV1>,std::string&)> submit)
    :assets_(assets),materials_(texture_assets,std::move(textures)),
     submit_(std::move(submit)) {}

bool SourceEquipmentOriginalBindingsV1::weapon_image(
    const dh2::skinning::VisualDrawViewV32& view,const std::string& uri,
    SourceEquipmentImageLeaseV1& output,std::string& error) {
    error.clear();
    if((view.weapon_slot!=1&&view.weapon_slot!=2)||uri.empty()) {
        error="Required actual source equipment weapon view/URI";return false;
    }
    try {
        auto owner=std::make_shared<WeaponImageOwnerV1>();
        owner->bytes=read_content(assets_,uri);
        dh2::resources::BresView image{};
        if(dh2_bres_open(&image,owner->bytes.data(),owner->bytes.size())!=dh2::resources::BresError::ok) {
            error="Actual source weapon AssetCatalog BRES is invalid";return false;
        }
        if(!dh2::scene::load(image,owner->materials,error))return false;
        if(!view.material_table||!same_material_table(*view.material_table,owner->materials.materials)) {
            error="Actual weapon material table does not correspond to its exact URI BRES";return false;
        }
        SourceEquipmentImageLeaseV1 lease;lease.retention=owner;lease.bytes=owner->bytes.data();
        lease.byte_count=owner->bytes.size();lease.image=image;lease.material_scene=&owner->materials;
        lease.resource_uri=uri;output=std::move(lease);error.clear();return true;
    } catch(const std::exception& e) { error=e.what();return false; }
}

bool SourceEquipmentOriginalBindingsV1::material(const SourceEquipmentDrawIdentityV1& identity,
    const dh2::resources::BresView& image,const dh2::scene::Material& source,
    Material& output,std::string& error) {
    if(source.id!=identity.material_id) {
        error="Source equipment material callback received another material identity";return false;
    }
    effects::EffectDrawSource effect;effect.kind=effects::EffectDrawKind::authored_mesh;
    effect.material=identity.material_index;effect.primitive=identity.primitive_index;
    effect.resource_uri=identity.resource_uri;effect.image=image;
    return materials_.bind(effect,source,output,error);
}

SourceEquipmentRenderServicesV1 SourceEquipmentOriginalBindingsV1::callbacks() {
    SourceEquipmentRenderServicesV1 result;
    result.weapon_image=[this](const auto& view,const auto& uri,auto& lease,auto& error) {
        return weapon_image(view,uri,lease,error);
    };
    result.material=[this](const auto& source,const auto& image,const auto& material,
                           auto& output,auto& error) {
        return this->material(source,image,material,output,error);
    };
    result.submit=submit_;
    return result;
}
} // namespace dh::foundation
