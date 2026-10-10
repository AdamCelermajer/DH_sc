#include "source_equipment_render_bridge.hpp"
#include "../../asset_catalog.hpp"
#include "source_equipment_appearance.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>

namespace dh::foundation {
namespace {
bool fail(std::string& error, const char* reason) { error=reason; return false; }
bool finite(float value) { return std::isfinite(value); }
struct DrawRetention {
    std::shared_ptr<const void> image;
    std::shared_ptr<const void> topology;
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
std::string table_difference(const std::vector<dh2::scene::Material>& a,
                             const std::vector<dh2::scene::Material>& b) {
    if(a.size()!=b.size())return " size";
    for(std::size_t i=0;i<a.size();++i) {
        if(a[i].id!=b[i].id)return " id@"+std::to_string(i);
        if(a[i].diffuse!=b[i].diffuse)return " diffuse@"+std::to_string(i);
        if(a[i].alpha_map!=b[i].alpha_map)return " alpha@"+std::to_string(i);
        if(a[i].effect_file!=b[i].effect_file||a[i].effect_uri!=b[i].effect_uri||a[i].gles2_technique!=b[i].gles2_technique)return " effect@"+std::to_string(i);
        if(std::memcmp(a[i].color,b[i].color,sizeof(a[i].color))!=0)return " color@"+std::to_string(i);
        if(std::memcmp(a[i].texture_matrix,b[i].texture_matrix,sizeof(a[i].texture_matrix))!=0)return " matrix@"+std::to_string(i);
        if(a[i].alpha_ref!=b[i].alpha_ref)return " alpha_ref@"+std::to_string(i);
        if(a[i].additive!=b[i].additive||a[i].backface!=b[i].backface)return " flags@"+std::to_string(i);
    }
    return " equal";
}
}

bool bind_source_equipment_render_skin(const AssetCatalog& assets, const CharacterVisual& body,
    dh2::skinning::VisualAssetServicesV6 asset_services,
    std::unique_ptr<dh2::skinning::VisualSkinOwnerV6>& output,
    SourceEquipmentImageLeaseV1& image_output, std::string& error) {
    error.clear();
    const auto* live_scene=body.retained_scene_borrow();
    const auto* config=body.configuration();
    if(!live_scene||!config)return fail(error,"Source equipment renderer requires the actual body Scene");
    try {
        dh2::skinning::VisualSkinResourcesV6 resources;
        const auto bytes=assets.read(config->model_path);
        if(!resources.load(bytes,error))return false;
        auto resource_pin=std::make_shared<const dh2::skinning::VisualSkinResourcesV6::Borrow>(resources.borrow());
        const auto& retained_bytes=resource_pin->bytes();
        dh2::resources::BresView image{};
        if(dh2_bres_open(&image,retained_bytes.data(),retained_bytes.size())!=dh2::resources::BresError::ok)
            return fail(error,"Source equipment renderer body BRES is invalid");
        auto owner=std::make_unique<dh2::skinning::VisualSkinOwnerV6>(*resource_pin,*live_scene,asset_services);
        if(!owner->initialize(error))return false;
        SourceEquipmentImageLeaseV1 lease;lease.retention=resource_pin;
        lease.bytes=retained_bytes.data();lease.byte_count=retained_bytes.size();lease.image=image;
        lease.material_scene=&resource_pin->factory_scene();lease.resource_uri=config->model_path;
        output=std::move(owner);image_output=std::move(lease);error.clear();return true;
    } catch(const std::exception& e) { error=e.what();return false; }
}

bool SourceEquipmentRenderBridgeV1::prepare(
    std::shared_ptr<const SourceEquipmentRenderFrameV1>& output,std::string& error) const {
    error.clear();
    if(!image_.retention||!image_.bytes||!image_.byte_count||!image_.image.bytes||!image_.material_scene||
       image_.image.bytes!=image_.bytes||image_.image.size!=image_.byte_count)
        return fail(error,"Required pinned source equipment body BRES/Scene lease");
    if(!services_.material)return fail(error,"Required source equipment COMMON material/pass binding");
    const std::vector<dh2::skinning::VisualDrawViewV32>* views=nullptr;
    if(!owner_.draw_views(views,error))return false;
    if(!views)return fail(error,"Required current source equipment draw views");
    auto frame=std::make_shared<SourceEquipmentRenderFrameV1>();
    try {
        for(const auto& view:*views) {
            if(!view.retention||!view.geometry||!view.material_table||!view.materials||
               !view.positions||view.positions->size()!=view.geometry->positions.size()||view.positions->empty()||
               view.materials->size()!=view.geometry->primitives.size())
                return fail(error,"Required retained source equipment geometry/material/pose view");
            for(const float value:view.world)if(!finite(value))
                return fail(error,"Nonfinite source equipment world matrix");
            for(std::uint32_t primitive_index=0;primitive_index<view.geometry->primitives.size();++primitive_index) {
                const auto& primitive=view.geometry->primitives[primitive_index];
                const auto material_index=view.materials->at(primitive_index);
                if(material_index>=view.material_table->size()||primitive.engine_type!=6||
                   primitive.collada_type!=0||primitive.indices.empty()||primitive.indices.size()%3)
                    return fail(error,"Unsupported source equipment triangle/material binding");
                const auto& material=view.material_table->at(material_index);
                if(primitive.material_symbol!=material.id)
                    return fail(error,"Source equipment primitive material identity mismatch");
                auto attribute=[&](unsigned semantic)->const dh2::skinning::VisualAttributeV6* {
                    const auto index=primitive.attributes[semantic];
                    return index>=0&&std::size_t(index)<view.geometry->attributes.size()?
                        &view.geometry->attributes[std::size_t(index)]:nullptr;
                };
                const auto* uv=attribute(4);const auto* color=attribute(2);const auto* normal=attribute(1);
                const auto count=view.positions->size();
                if(!uv||uv->components!=2||uv->values.size()!=count*2)
                    return fail(error,"Required source equipment UV stream");
                if(primitive.attributes[2]>=0&&(!color||color->components!=4||
                   color->values.size()!=count*4||(color->type!=1&&color->type!=6)))
                    return fail(error,"Unsupported source equipment color stream");
                if(normal&&(normal->components!=3||normal->values.size()!=count*3))
                    return fail(error,"Unsupported source equipment normal stream");
                SourceEquipmentRenderPacketV1 packet;packet.world=view.world;
                packet.source.resource_uri=image_.resource_uri;packet.source.geometry_id=view.geometry->id;
                packet.source.material_id=material.id;packet.source.category=view.category;
                packet.source.module=view.module;packet.source.weapon_slot=view.weapon_slot;
                packet.source.primitive_index=primitive_index;packet.source.material_index=material_index;
                packet.source.pose_revision=view.pose_revision;packet.source.skinned=view.skinned;
                packet.source.positions_changed=view.positions_changed;packet.source.source_color_missing=!color;
                packet.source.source_normal_missing=!normal;
                packet.mesh.vertices.resize(count);
                for(std::size_t i=0;i<count;++i) {
                    auto& vertex=packet.mesh.vertices[i];const auto& p=view.positions->at(i);
                    vertex.position={p[0],p[1],p[2]};vertex.u=uv->values[i*2];vertex.v=uv->values[i*2+1];
                    if(color)for(unsigned c=0;c<4;++c)vertex.color[c]=color->values[i*4+c]/(color->type==1?255.f:1.f);
                    if(normal)vertex.normal={normal->values[i*3],normal->values[i*3+1],normal->values[i*3+2]};
                    if(!finite(vertex.position.x)||!finite(vertex.position.y)||!finite(vertex.position.z)||
                       !finite(vertex.u)||!finite(vertex.v))return fail(error,"Nonfinite source equipment vertex stream");
                    for(const auto c:vertex.color)if(!finite(c))return fail(error,"Nonfinite source equipment color stream");
                }
                for(const auto index:primitive.indices)if(index>=count)
                    return fail(error,"Source equipment index outside vertex stream");
                packet.mesh.indices=primitive.indices;DrawRange range;range.indexCount=primitive.indices.size();
                SourceEquipmentImageLeaseV1 source_image=image_;
                if(view.weapon_slot) {
                    if(!services_.weapon_image)return fail(error,"Required exact source equipment weapon BRES lease callback");
                    const auto uri=owner_.weapon_uri(view.weapon_slot);
                    if(uri.empty()||!services_.weapon_image(view,uri,source_image,error))return false;
                    if(!source_image.retention||!source_image.bytes||!source_image.byte_count||
                       !source_image.image.bytes||source_image.image.bytes!=source_image.bytes||
                       source_image.image.size!=source_image.byte_count||!source_image.material_scene||source_image.resource_uri!=uri)
                        return fail(error,"Invalid exact source equipment weapon image lease");
                    dh2::resources::BresView verified{};
                    if(dh2_bres_open(&verified,source_image.bytes,source_image.byte_count)!=dh2::resources::BresError::ok)
                        return fail(error,"Invalid source equipment weapon BRES bytes");
                    source_image.image=verified;
                }
                if(!same_material_table(*view.material_table,source_image.material_scene->materials)) {
                    error="Source equipment material table does not match exact BRES: weapon="+
                        std::to_string(view.weapon_slot)+" category="+std::to_string(view.category)+
                        " module="+std::to_string(view.module)+" uri="+source_image.resource_uri+
                        table_difference(*view.material_table,source_image.material_scene->materials);
                    return false;
                }
                const auto& matched_material=source_image.material_scene->materials.at(material_index);
                if(!services_.material(packet.source,source_image.image,matched_material,range.material,error))return false;
                if(!range.material.sourcePass)return fail(error,"Required original source equipment render-pass state");
                if(!material.diffuse.empty()&&!range.material.texture)
                    return fail(error,"Required decoded original source equipment diffuse texture");
                if(!normal&&range.material.lightingEnabled)
                    return fail(error,"Source equipment shader requires unavailable normal stream");
                packet.mesh.ranges.push_back(std::move(range));
                packet.source_retention=std::make_shared<DrawRetention>(DrawRetention{
                    source_image.retention,view.retention});
                frame->packets.push_back(std::move(packet));
            }
        }
    } catch(const std::exception& e) { error=e.what();return false; }
    output=std::move(frame);error.clear();return true;
}

bool SourceEquipmentRenderBridgeV1::submit(std::string& error) const {
    if(!services_.submit)return fail(error,"Required root source equipment renderer ordering callback");
    std::shared_ptr<const SourceEquipmentRenderFrameV1> frame;
    if(!prepare(frame,error))return false;
    return services_.submit(std::move(frame),error);
}
} // namespace dh::foundation
