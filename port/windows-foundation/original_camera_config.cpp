#include "original_camera_config.hpp"
#include "asset_catalog.hpp"
#include "content_paths.hpp"
#include "../level-loader/vendor/tinyxml/tinyxml.h"
#include "../level-world/gameplay_camera_scene_v3.hpp"
#include "../level-world/gameplay_camera_damping_v1.hpp"
#include "../engine-animation/animation.hpp"
#include "../game-data/animation_tables.hpp"
#include "../game-data/design_settings.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace dh::foundation {
namespace {
float fromBits(std::uint32_t bits){float v;std::memcpy(&v,&bits,4);return v;}
void require(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
dh2::data::Bytes bytes(const std::vector<std::uint8_t>& v){return {v.data(),v.size()};}
bool finite(CameraVec3 v){return std::isfinite(v.x)&&std::isfinite(v.y)&&std::isfinite(v.z);}
// P16 LEVELS: required=false returns nullptr for a level without LevelConfig (debug maps); callers use the InitPost defaults.
TiXmlElement* levelConfig(TiXmlDocument& doc,const std::vector<std::uint8_t>& data,bool required=true){
    if(data.empty()||data.size()>8*1024*1024)throw std::runtime_error("Selected level XML outside bounds");
    std::string xml(data.begin(),data.end());
    if(xml.find('\0')!=std::string::npos)throw std::runtime_error("NUL in selected level XML");
    doc.Parse(xml.c_str());if(doc.Error())throw std::runtime_error(doc.ErrorDesc());
    auto* root=doc.RootElement();if(!root)throw std::runtime_error("Selected level has no root");
    TiXmlElement* result=nullptr;
    auto inspect=[&](TiXmlElement* n){const char* type=n->Attribute("gametype");if(type&&std::string(type)=="LevelConfig"){
        if(result)throw std::runtime_error("Multiple LevelConfig declarations");result=n;}};
    inspect(root);for(auto* n=root->FirstChildElement();n;n=n->NextSiblingElement())inspect(n);
    if(!result&&required)throw std::runtime_error("Selected level has no LevelConfig");return result;
}
void overrideConfig(const TiXmlElement& node,OriginalCameraConfig& out){
    if(const auto* s=node.Attribute("camera_file"))out.file=*s?s:"data/3D/camera/CameraTests.bdae";
    if(const auto* s=node.Attribute("camera_name"))out.node=*s?s:"PlayerCamera_Default";
    if(const auto* s=node.Attribute("camera_animset"))out.animationSet=s;
    for(auto pair:{std::pair<const char*,float*>{"camera_znear",&out.nearPlane},{"camera_zfar",&out.farPlane}}){
        if(node.Attribute(pair.first)){
            int v;
            if(node.QueryIntAttribute(pair.first,&v)!=TIXML_SUCCESS)
                throw std::runtime_error(std::string("Invalid original camera integer ")+pair.first);
            *pair.second=float(v);
        }
    }
    if(out.animationSet.empty()||out.nearPlane<=0||out.farPlane<=out.nearPlane)
        throw std::runtime_error("Invalid original camera configuration");
}
}
bool decode_original_camera_config(const std::vector<std::uint8_t>& xml,OriginalCameraConfig& out,std::string& error){
    try{TiXmlDocument doc;auto* n=levelConfig(doc,xml);const auto* tpl=n->Attribute("template");
        if(tpl&&*tpl)throw std::runtime_error("LevelConfig template requires asset-aware resolution");
        OriginalCameraConfig result;overrideConfig(*n,result);out=std::move(result);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
struct OriginalGameplayCamera::Impl {
    OriginalCameraConfig config;
    dh2::camera::GameplayCameraSceneV3 scene;
    dh2::animation::Player templatePlayer,idlePlayer;
    dh2::animation::TransformSet transforms;
    std::vector<std::array<float,4>> values;
    std::vector<std::int32_t> cursors;
    std::vector<bool> initialized;
    std::uint32_t selected=0;
    std::size_t idleIndex=1;
    std::uint64_t clockMs=0;
    std::int32_t idleStart=0,idleEnd=0;
    float distance=0,zoomMin=0,zoomMax=0;
    float velocity[3]{};
    CameraPose pose;
    bool ready=false;
    bool sample(std::string& error){
        const auto time=static_cast<std::int32_t>(std::min<std::uint64_t>(clockMs+std::uint64_t(idleStart),std::uint64_t(idleEnd)));
        for(std::size_t i=0;i<transforms.targets().size();++i){
            const auto& t=transforms.targets()[i];if(t.node==UINT32_MAX)continue;
            if(t.node>=scene.graph().graph.size()||(t.type!=1&&t.type!=5&&t.type!=10)){
                error="Unsupported original camera node channel";return false;}
            const auto* binding=transforms.clip_target(idleIndex,i);
            if(!initialized[i]&&(!binding||(binding->mode==1&&!binding->has_default))){
                error="Original camera channel lacks initial value";return false;}
            if(!transforms.sample(idleIndex,i,time,values[i].data(),t.components,&cursors[i],error))return false;
            initialized[i]=true;auto& node=scene.graph().graph[t.node];
            auto* target=t.type==1?node.translation:t.type==5?node.quaternion:node.scale;
            std::copy_n(values[i].data(),t.components,target);
        }
        // CameraLevel::Load sets requested zoom=0. Idle is finite with shake
        // enabled; applied zoom stays zero until completion. Thereafter source
        // HandleZoom clamps to the actual DesignSettings bounds.
        const float zoom=clockMs<std::uint64_t(idleEnd-idleStart)?0.0f:std::clamp(0.0f,zoomMin,zoomMax);
        const float child[3]{0,0,-zoom*distance};
        return scene.set_camera_instance_position(selected,child,error)&&scene.update_selected_absolute_v67(selected,error);
    }
    bool refresh(std::string& error){
        float eye[3],target[3];if(!scene.eye_and_target(selected,eye,target,error))return false;
        pose={{eye[0],eye[1],eye[2]},{target[0],target[1],target[2]},{0,0,1},fromBits(0x3edbf877)*180.0f/3.14159265358979323846f};
        if(!finite(pose.position)||!finite(pose.target)){error="Nonfinite original camera view";return false;}
        return true;
    }
};
OriginalGameplayCamera::OriginalGameplayCamera():impl_(std::make_unique<Impl>()){}
OriginalGameplayCamera::~OriginalGameplayCamera()=default;
OriginalGameplayCamera::OriginalGameplayCamera(OriginalGameplayCamera&&) noexcept=default;
OriginalGameplayCamera& OriginalGameplayCamera::operator=(OriginalGameplayCamera&&) noexcept=default;
bool OriginalGameplayCamera::load(const AssetCatalog& assets,const std::string& levelUri,const std::filesystem::path& fallbackRoot,std::string& error){
    try{
        auto next=std::make_unique<Impl>();
        TiXmlDocument doc;auto* config=levelConfig(doc,read_content(assets,levelUri),false);
        if(!config){static bool noticed=false;if(!noticed){noticed=true;std::cerr<<"Level notice: no LevelConfig; original InitPost camera defaults used (unverified for this level)" << std::endl;}}
        const auto* tpl=config?config->Attribute("template"):nullptr;
        if(tpl&&*tpl){TiXmlDocument templateDoc;auto data=read_content(assets,tpl,levelUri);
            auto* templateConfig=levelConfig(templateDoc,data);const auto* nested=templateConfig->Attribute("template");
            if(nested&&*nested)throw std::runtime_error("Nested LevelConfig templates require further resolution");
            overrideConfig(*templateConfig,next->config);}
        if(config)overrideConfig(*config,next->config);
        std::unique_ptr<AssetCatalog> fallback;if(!fallbackRoot.empty())fallback=std::make_unique<AssetCatalog>(fallbackRoot);
        const auto read=[&](const std::string& uri){try{return read_content(assets,uri);}catch(const std::exception&){
            if(!fallback)throw;return read_content(*fallback,uri);}};
        const auto dictNames=read("data/animations_dictionary_pyarraynames.bin"),dictValues=read("data/animations_dictionary_pyarray.bin");
        dh2::data::Dictionary dict;require(dh2::data::load_dictionary(bytes(dictNames),bytes(dictValues),dict,error),error);
        const auto records=read("data/animations_pyarray.bin"),names=read("data/animations_pyarraynames.bin"),fields=read("data/animations_pystructnames.bin");
        dh2::data::AnimationTables tables;require(dh2::data::load_animation_tables(bytes(records),bytes(names),bytes(fields),dict,tables,error),error);
        const auto row=std::find(tables.camera_names.begin(),tables.camera_names.end(),next->config.animationSet);
        if(row==tables.camera_names.end())throw std::runtime_error("Selected CamAnimSet absent: "+next->config.animationSet);
        const auto& anims=tables.cameras.at(std::size_t(row-tables.camera_names.begin()));
        require(next->scene.load(read(next->config.file),error),error);
        require(next->scene.select(next->config.node,next->selected,error),error);
        const auto& selected=next->scene.cameras().at(next->selected);
        const auto& graph=next->scene.graph().graph;
        const auto& parent=graph.at(selected.node);const auto& target=graph.at(selected.target_node);
        float square=0;for(unsigned i=0;i<3;++i){const auto d=target.translation[i]-parent.translation[i];square+=d*d;}
        next->distance=std::sqrt(square);
        auto animation=[&](int id,dh2::animation::Player& player){if(id<0||std::size_t(id)>=dict.values.size())throw std::runtime_error("Required original camera template/idle resource");
            auto data=read(dict.values[id]);require(player.load(data.data(),data.size(),next->scene.graph(),error,dh2::animation::MissingTargets::ignore),error);};
        animation(anims.template_id,next->templatePlayer);animation(anims.idle,next->idlePlayer);
        require(next->transforms.compile_dynamic({{anims.template_id,&next->templatePlayer},{anims.idle,&next->idlePlayer}},next->scene.graph(),error,&next->templatePlayer),error);
        next->idleIndex=std::size_t(next->transforms.find_clip(anims.idle));
        const auto* clip=next->transforms.clip(next->idleIndex);if(!clip||clip->start<0||clip->end<clip->start)throw std::runtime_error("Invalid original camera idle clip");
        next->idleStart=clip->start;next->idleEnd=clip->end;
        next->values.resize(next->transforms.targets().size());next->cursors.resize(next->values.size());next->initialized.resize(next->values.size());
        const auto designRecords=read("data/design_pyarray.bin"),designNames=read("data/design_pyarraynames.bin"),designFields=read("data/design_pystructnames.bin");
        dh2::data::DesignSettingsOwner design;require(design.load(bytes(designRecords),bytes(designNames),bytes(designFields),error),error);
        auto borrow=design.borrow();if(borrow.rows().empty())throw std::runtime_error("Original DesignSettings has no rows");
        next->zoomMin=fromBits(borrow.rows()[0].words[0xac/4]);next->zoomMax=fromBits(borrow.rows()[0].words[0xa8/4]);
        if(!std::isfinite(next->zoomMin)||!std::isfinite(next->zoomMax)||next->zoomMin>next->zoomMax)throw std::runtime_error("Invalid original camera zoom limits");
        require(next->sample(error)&&next->refresh(error),error);next->ready=true;impl_=std::move(next);error.clear();return true;
    }catch(const std::exception& ex){error=ex.what();return false;}
}
bool OriginalGameplayCamera::loaded() const noexcept{return impl_&&impl_->ready;}
bool OriginalGameplayCamera::reset(CameraVec3 anchor,std::string& error){
    if(!loaded()||!finite(anchor)){error="Original camera reset requires loaded camera and finite anchor";return false;}
    impl_->clockMs=0;std::fill_n(impl_->velocity,3,0);std::fill(impl_->cursors.begin(),impl_->cursors.end(),0);
    const float p[3]{anchor.x,anchor.y,anchor.z};
    return impl_->scene.set_root_position(p,error)&&impl_->sample(error)&&impl_->refresh(error);
}
bool OriginalGameplayCamera::update(CameraVec3 anchor,std::uint32_t dt,std::string& error){
    return update_anchor(anchor,dt,true,error);
}
bool OriginalGameplayCamera::update_anchor(CameraVec3 anchor,std::uint32_t dt,bool applyDamping,std::string& error){
    if(!loaded()||!finite(anchor)){error="Original camera update requires loaded camera and finite anchor";return false;}
    float root[3];if(!impl_->scene.root_position(root,error))return false;
    float desired[3]{anchor.x,anchor.y,anchor.z};const float ratio=fromBits(0x3f333333);
    if(applyDamping)dh2_gameplay_camera_damping_v1(impl_->velocity,desired,root,root,&ratio,dt);
    impl_->clockMs+=dt;
    return impl_->scene.set_root_position(desired,error)&&impl_->sample(error)&&impl_->refresh(error);
}
const CameraPose& OriginalGameplayCamera::pose() const noexcept{return impl_->pose;}
const OriginalCameraConfig& OriginalGameplayCamera::config() const noexcept{return impl_->config;}
CameraMovementBasis OriginalGameplayCamera::movementBasis() const{return cameraMovementBasis(impl_->pose,{0,0,1});}
float OriginalGameplayCamera::authoredDistance() const noexcept{return impl_->distance;}
float OriginalGameplayCamera::sourceAspect() const noexcept{return fromBits(0x3fd578e9);}
} // namespace dh::foundation
