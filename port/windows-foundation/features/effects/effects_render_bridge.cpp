#include "effects_render_bridge.hpp"
#include "../../frame_perf.hpp" // B062 diagnostic probes
#include "../../../level-world/character_authored_particle_fx_v4.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>

namespace dh::foundation::effects {
namespace {
bool fail(std::string& error,const char* reason) { error=reason;return false; }
bool finite(float value) { return std::isfinite(value); }
}
bool EffectsRenderBridge::convert(const dh2::skinning::VisualDrawPartV6& part,
    std::uint32_t primitive_index,std::uint32_t material_index,EffectDrawSource source,
    const EffectRenderServices& services,EffectRenderPacket& output,std::string& error) {
    error.clear();
    if(!part.retention||!part.geometry||!part.material_table||!part.materials||
       primitive_index>=part.geometry->primitives.size()||material_index>=part.material_table->size()||
       part.positions.size()!=part.geometry->positions.size()||part.positions.empty())
        return fail(error,"Required SAME retained FX geometry/material/skin backing");
    if(primitive_index>=part.materials->size()||part.materials->at(primitive_index)!=material_index)
        return fail(error,"Required source FX primitive material binding");
    const auto& primitive=part.geometry->primitives[primitive_index];
    const auto& material=part.material_table->at(material_index);
    if(primitive.engine_type!=6||primitive.collada_type!=0||primitive.indices.empty()||
       primitive.indices.size()%3||primitive.material_symbol!=material.id)
        return fail(error,"Required supported original FX triangle-list primitive");
    auto attribute=[&](unsigned semantic)->const dh2::skinning::VisualAttributeV6* {
        auto index=primitive.attributes[semantic];
        return index>=0&&std::size_t(index)<part.geometry->attributes.size()?
            &part.geometry->attributes[index]:nullptr;
    };
    const auto* uv=attribute(4);const auto* color=attribute(2);const auto* normal=attribute(1);
    if(!uv||uv->components!=2||uv->values.size()!=part.positions.size()*2)
        return fail(error,"Required original baked FX UV stream");
    if(primitive.attributes[2]>=0&&(!color||color->components!=4||
       color->values.size()!=part.positions.size()*4||(color->type!=1&&color->type!=6)))
        return fail(error,"Unsupported original FX color stream");
    if(normal&&(normal->components!=3||normal->values.size()!=part.positions.size()*3))
        return fail(error,"Unsupported original FX normal stream");
    if(!services.material) return fail(error,"Required original FX material/texture renderer binding");
    EffectRenderPacket result;result.world=part.world;result.source=std::move(source);
    result.source.source_color_missing=!color;result.source.source_normal_missing=!normal;
    result.source_retention=part.retention;result.mesh.vertices.resize(part.positions.size());
    for(std::size_t index=0;index<part.positions.size();++index) {
        auto& v=result.mesh.vertices[index];const auto& p=part.positions[index];
        v.position={p[0],p[1],p[2]};v.u=uv->values[index*2];v.v=uv->values[index*2+1];
        // Missing source color is an attribute omission, not a generated tint.
        // Selected shader/constant-color behavior is supplied by material binding.
        if(color)for(unsigned k=0;k<4;++k)v.color[k]=color->values[index*4+k]/(color->type==1?255.f:1.f);
        if(normal)v.normal={normal->values[index*3],normal->values[index*3+1],normal->values[index*3+2]};
        if(!finite(v.position.x)||!finite(v.position.y)||!finite(v.position.z)||!finite(v.u)||!finite(v.v))
            return fail(error,"Nonfinite retained original FX vertex stream");
        for(auto c:v.color)if(!finite(c))return fail(error,"Nonfinite retained original FX color stream");
    }
    for(auto index:primitive.indices)if(index>=result.mesh.vertices.size())
        return fail(error,"Original FX index outside retained stream");
    for(auto value:result.world)if(!finite(value))return fail(error,"Nonfinite original FX world transform");
    result.mesh.indices=primitive.indices;DrawRange range;range.indexCount=primitive.indices.size();
    {DH_PROBE("fx.convert.material");
    if(!services.material(result.source,material,range.material,error))return false;}
    if(!range.material.sourcePass)return fail(error,"Required original FX render-pass state");
    if(!material.diffuse.empty()&&!range.material.texture)return fail(error,"Required decoded original FX texture");
    if(!normal&&range.material.lightingEnabled)return fail(error,"FX shader requires unavailable original normal stream");
    result.mesh.ranges.push_back(std::move(range));output=std::move(result);return true;
}
bool EffectsRenderBridge::prepare(std::shared_ptr<const EffectRenderFrame>& output,std::string& error) const {
    return prepare(output,{},error);
}
bool EffectsRenderBridge::prepare(std::shared_ptr<const EffectRenderFrame>& output,const std::vector<EmbeddedFxSourceV1>& embedded,std::string& error) const {
    std::vector<dh2::fx::CharacterFxMeshDrawSourceV4> meshes;
    std::vector<dh2::fx::CharacterParticleDrawSourceV3> particles;
    {DH_PROBE("fx.prepare.sources");
    if(!manager_.mesh_draw_sources_v4(meshes,error)||!manager_.particle_draw_sources_v3(particles,error))return false;}
    DH_PROBE("fx.prepare.convert");
    auto result=std::make_shared<EffectRenderFrame>();
    std::map<std::uintptr_t,std::string> uris;for(const auto& view:manager_.views())uris.emplace(view.identity,view.uri);
    for(const auto& source:meshes) {
        if(!source.image||!source.resource_bytes||!source.scene||!source.source_texture_matrix68)
            return fail(error,"Required actual mesh FX source provenance");
        EffectDrawSource info;info.kind=EffectDrawKind::authored_mesh;info.fx=source.fx_identity;
        info.node=source.node_identity;info.material=source.material;info.primitive=source.primitive;
        info.rendering_layer=source.rendering_layer;info.camera_offset_word=source.camera_offset_word;
        info.resource_uri=uris[info.fx];info.resource_bytes=source.resource_bytes;info.image=*source.image;
        info.scene=source.scene;info.source_texture_matrix=source.source_texture_matrix68;info.source_owner=source.source_owner204;
        EffectRenderPacket packet;
        if(!convert(source.part,source.primitive,source.material,std::move(info),services_,packet,error))return false;
        result->packets.push_back(std::move(packet));
    }
    for(const auto& source:particles) {
        if(!source.image||!source.resource_bytes||!source.scene||!source.source_texture_matrix68||
           !source.source_node_render_fields_ready||!source.part.geometry)
            return fail(error,"Required actual particle FX source provenance");
        for(std::uint32_t p=0;p<source.part.geometry->primitives.size();++p) {
            EffectDrawSource info;info.kind=EffectDrawKind::authored_particle;info.fx=source.fx_identity;
            info.node=source.emitter_node;info.particle=source.particle_identity;info.material=source.material;info.primitive=p;
            info.rendering_layer=source.rendering_layer;info.camera_offset_word=source.camera_offset_word;
            info.resource_uri=uris[info.fx];info.resource_bytes=source.resource_bytes;info.image=*source.image;
            info.scene=source.scene;info.source_texture_matrix=source.source_texture_matrix68;info.source_owner=source.source_owner204;
            EffectRenderPacket packet;
            if(!convert(source.part,p,source.material,std::move(info),services_,packet,error))return false;
            result->packets.push_back(std::move(packet));
        }
    }
    // P17 FIREWALL: resources owned by scene visuals (doors, decor, objects). Same provenance rules as the
    // manager's sources above; the resource pointer is the FX identity and the model path the texture base.
    EffectRenderServices cached=services_; // P17 FIREWALL: see embedded_material_cache_
    if(services_.material) {
        auto inner=services_.material;
        cached.material=[this,inner](const EffectDrawSource& source,const dh2::scene::Material& original,Material& output,std::string& error)->bool {
            const auto key=std::to_string(source.fx)+"|"+original.id;
            const auto hit=embedded_material_cache_.find(key);
            if(hit!=embedded_material_cache_.end()) {
                output=hit->second;
                std::copy_n(original.color,4,output.color.begin());
                return true;
            }
            if(!inner(source,original,output,error)) return false;
            embedded_material_cache_.emplace(key,output);
            return true;
        };
    }
    // B101/B105 diagnostic: DH_FX_HIDE=mesh|particles drops one embedded part kind (isolation runs only).
    static const std::string hide=std::getenv("DH_FX_HIDE")?std::getenv("DH_FX_HIDE"):"";
    for(const auto& item:embedded) {
        if(!item.resource)continue;
        const auto fx=reinterpret_cast<std::uintptr_t>(item.resource.get());
        std::vector<dh2::fx::CharacterParticleDrawSourceV3> embedded_particles;
        if(!item.resource->draw_sources_v3(embedded_particles,error))return false;
        if(hide=="particles")embedded_particles.clear();
        for(const auto& source:embedded_particles) {
            if(!source.image||!source.resource_bytes||!source.scene||!source.source_texture_matrix68||
               !source.source_node_render_fields_ready||!source.part.geometry)
                return fail(error,"Required actual embedded particle FX source provenance");
            for(std::uint32_t p=0;p<source.part.geometry->primitives.size();++p) {
                EffectDrawSource info;info.kind=EffectDrawKind::authored_particle;info.fx=fx;
                info.node=source.emitter_node;info.particle=source.particle_identity;info.material=source.material;info.primitive=p;
                info.rendering_layer=source.rendering_layer;info.camera_offset_word=source.camera_offset_word;
                info.resource_uri=item.uri;info.resource_bytes=source.resource_bytes;info.image=*source.image;
                info.scene=source.scene;info.source_texture_matrix=source.source_texture_matrix68;info.source_owner=source.source_owner204;
                EffectRenderPacket packet;
                if(!convert(source.part,p,source.material,std::move(info),cached,packet,error))return false;
                result->packets.push_back(std::move(packet));
            }
        }
        if(const auto* composite=dynamic_cast<const dh2::fx::CharacterAuthoredCompositeFxResourceV4*>(item.resource.get()); item.include_meshes&&composite) {
            std::vector<dh2::fx::CharacterFxMeshDrawSourceV4> embedded_meshes;
            if(!composite->mesh_draw_sources_v4(embedded_meshes,error))return false;
            if(hide=="mesh")embedded_meshes.clear();
            for(const auto& source:embedded_meshes) {
                if(!source.image||!source.resource_bytes||!source.scene||!source.source_texture_matrix68)
                    return fail(error,"Required actual embedded mesh FX source provenance");
                EffectDrawSource info;info.kind=EffectDrawKind::authored_mesh;info.fx=fx;
                info.node=source.node_identity;info.material=source.material;info.primitive=source.primitive;
                info.rendering_layer=source.rendering_layer;info.camera_offset_word=source.camera_offset_word;
                info.resource_uri=item.uri;info.resource_bytes=source.resource_bytes;info.image=*source.image;
                info.scene=source.scene;info.source_texture_matrix=source.source_texture_matrix68;info.source_owner=source.source_owner204;
                EffectRenderPacket packet;
                if(!convert(source.part,source.primitive,source.material,std::move(info),cached,packet,error))return false;
                result->packets.push_back(std::move(packet));
            }
        }
    }
    output=std::move(result);return true;
}
bool EffectsRenderBridge::submit(std::string& error) const {
    if(!services_.submit)return fail(error,"Required root FX renderer/source ordering callback");
    std::shared_ptr<const EffectRenderFrame> frame;if(!prepare(frame,error))return false;
    return services_.submit(std::move(frame),error);
}
DispatchResult RetainedEffectsAdapter::event(std::uintptr_t actor,const RetainedAnimationEvent& event,
    std::uint64_t ordinal,std::string& error) {
    error.clear();if(event.name.compare(0,3,"fx_")!=0)return DispatchResult::ignored;
    if(!actor||event.clip_id.empty()||!bindings_.attachment) {
        error="Required source retained FX actor/clip/attachment";return DispatchResult::required_failure;
    }
    if(!delivered_.insert({actor,event.clip_id,event.slot,event.generation,event.wall_timestamp_ms,
                          event.lag_ms,ordinal}).second)return DispatchResult::duplicate;
    ActorAttachment attachment;
    if(!bindings_.attachment(actor,attachment,error)||attachment.actor!=actor) {
        if(error.empty())error="Required SAME retained FX actor attachment";
        return DispatchResult::required_failure;
    }
    for(auto value:attachment.target_position)if(!finite(value)) {
        error="Invalid actual retained FX actor position";return DispatchResult::required_failure;
    }
    return manager_.animation_event(event.name.c_str(),attachment.target_position,error)?
        DispatchResult::delivered:DispatchResult::required_failure;
}
void RetainedEffectsAdapter::release_actor(std::uintptr_t actor,std::uintptr_t socket) {
    executor_.release_actor(actor,socket);
    for(auto it=delivered_.begin();it!=delivered_.end();)
        if(std::get<0>(*it)==actor)it=delivered_.erase(it);else ++it;
}
}
