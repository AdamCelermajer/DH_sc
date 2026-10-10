#include "class_preview_scene.hpp"
#include "scene_materials.hpp"
#include "../../../content_paths.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>
namespace dh::foundation::frontend {
bool ClassPreviewScene::load(const AssetCatalog& assets,std::string& error){try{
    auto bytes=read_content(assets,"models/class_selection.bdae");
    dh2::resources::BresView view{};
    if(dh2_bres_open(&view,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)throw std::runtime_error("Class scene BRES rejected");
    ClassPreviewScene next;
    if(!dh2::scene::load(view,next.scene_,error)||!decode_original_scene(bytes,next.backdrop_,error)||
       !apply_preview_scene_materials(bytes,next.backdrop_,error)||!read_class_select_light_inputs(next.scene_,next.lightInputs_,error))return false;
    auto node=[&](const char* id){for(unsigned i=0;i<next.scene_.graph.size();++i)if(next.scene_.graph[i].id==id)return i;throw std::runtime_error(std::string("Original class node absent: ")+id);};
    next.anchorNodes_={node("dummy_Warrior-node"),node("dummy_Rogue-node"),node("dummy_Mage-node")};next.cameraNode_=node("Camera01-node");
    // Show428f38 changes camera local position before constructing/binding
    // the animator. Animated channels then replace these source rest values.
    auto& camera=next.scene_.graph[next.cameraNode_];camera.translation[0]=0;camera.translation[1]=0;camera.translation[2]=-200;
    if(!dh2::scene::update_world(next.scene_,error)||!next.animation_.load(bytes.data(),bytes.size(),next.scene_,error))return false;
    if(next.animation_.skipped)throw std::runtime_error("Class scene animation channels unsupported");
    auto word=[](const std::uint8_t* p){std::uint32_t v;std::memcpy(&v,p,4);return v;};
    for(unsigned i=0;i<dh2_bres_library_count(&view,dh2::resources::Library::animation_clip);++i){
        const auto* p=dh2_bres_library_item(&view,dh2::resources::Library::animation_clip,i);
        if(!p)throw std::runtime_error("Class scene clip record absent");
        const auto name=word(p);if(name>=bytes.size()||!std::memchr(bytes.data()+name,0,bytes.size()-name))throw std::runtime_error("Class clip name rejected");
        next.clips_.push_back({reinterpret_cast<const char*>(bytes.data()+name),int(word(p+4)),int(word(p+8))});
    }
    // Show leaves previous selection=-1. First Update follows the native
    // destination-specific branches, including initial showcase transitions.
    next.loaded_=true;*this=std::move(next);error.clear();return true;
}catch(const std::exception& e){error=e.what();return false;}}
bool ClassPreviewScene::sample(int index,int dt,std::string& error){
    if(!loaded_||index<0||index>2||dt<0){error="Class scene update owner unavailable";return false;}
    if(index!=selected_||cursor_>=end_){
        const bool transition=index!=selected_;
        // Update428498 routes by destination. The source UI only admits
        // adjacent arrows, but these exact branches also define first update.
        const std::string name=transition?(index==0?"lol_2_to_1":index==2?"lol_2_to_3":selected_==0?"lol_1_to_2":"lol_3_to_2"):
            "lol_"+std::to_string(index+1)+"_idle";
        const auto clip=std::find_if(clips_.begin(),clips_.end(),[&](const Clip& c){return c.name==name;});
        if(clip==clips_.end()||clip->end<clip->start){error="Authored class scene clip absent: "+name;return false;}
        cursor_=clip->start;end_=clip->end;selected_=index;transition_=transition;clipName_=name;
    }
    if(!animation_.sample(scene_,cursor_,error))return false;
    if(!read_class_select_light_inputs(scene_,lightInputs_,error))return false;
    sampled_=cursor_;
    for(unsigned i=0;i<3;++i)anchors_[i]=scene_.graph.at(anchorNodes_[i]).world;
    const auto& m=scene_.graph.at(cameraNode_).world;
    camera_.eye={m[12],m[13],m[14]};camera_.target={m[12]-100*m[8],m[13]-100*m[9],m[14]-100*m[10]};camera_.up={0,0,1};
    // Collada source horizontal74.08049774169922/aspect1.5 to vertical.
    camera_.verticalFovDegrees=2*std::atan(std::tan(74.08049774169922f*.01745329238474369f*.5f)/1.5f)*(180.f/3.14159265358979323846f);
    // Base CCameraSceneNode ctor583734 writes+344=0x3faaaaab. Collada
    // perspective changes FOV only; Show and RenderClassSelectPane retain
    // this4/3 aspect even when the physical viewport has another ratio.
    const std::uint32_t aspectBits=0x3faaaaab;
    std::memcpy(&camera_.aspectRatio,&aspectBits,sizeof(aspectBits));
    camera_.nearPlane=50;camera_.farPlane=50000;
    cursor_=int(std::min<std::int64_t>(end_,std::int64_t(cursor_)+dt));error.clear();return true;
}
bool ClassPreviewScene::input_enabled()const noexcept{return loaded_&&selected_>=0&&(!transition_||cursor_>=end_);}
}
