#include "creation_preview.hpp"
#include "class_preview_scene.hpp"
#include "../../../asset_catalog.hpp"
#include <iostream>
#include <stdexcept>
int main(int argc,char**argv){if(argc!=2)return 2;try{
    dh::foundation::AssetCatalog assets(argv[1]);std::string error;
    dh::foundation::frontend::CreationPreview metadata;
    if(!metadata.load(assets,error))throw std::runtime_error(error);
    dh::foundation::OriginalScene menu;
    if(!dh::foundation::frontend::load_menu_preview_backdrop(assets,menu,error))throw std::runtime_error(error);
    for(unsigned i=0;i<menu.materials.size();++i){const auto&m=menu.materials[i];const auto&r=menu.mesh.ranges[i];
        std::cout<<"MENU_MATERIAL "<<m.id<<" effect "<<m.effectFile<<" technique "<<m.technique<<" texture "<<m.diffuse<<" alpha "<<m.alphaMap<<" alphaRef "<<m.alphaReference<<" color ";
        for(auto c:r.material.color)std::cout<<c<<' ';
        std::cout<<" genericLighting "<<r.material.lightingEnabled;
        if(r.material.sourcePass){const auto&p=*r.material.sourcePass;std::cout<<" pass blend "<<p.blend<<' '<<p.blendSource<<' '<<p.blendDestination<<" depth "<<p.depthTest<<' '<<p.depthWrite<<' '<<p.depthFunction<<" cull "<<p.cull<<' '<<p.cullFace<<' '<<p.frontFace;}
        std::cout<<'\n';
    }
    for(auto&actor:metadata.actors())for(const char* alias:{"MenuIdle","MenuOnSelect"}){
        std::int32_t start{},end{};if(!actor.body.animation_range(alias,start,end,error))throw std::runtime_error(error);
        std::cout<<"ACTOR_CLIP "<<actor.definition.character<<' '<<alias<<' '<<(std::string(alias)=="MenuIdle"?actor.definition.idle_clip:actor.definition.select_clip)<<" range "<<start<<' '<<end<<" duration "<<end-start<<'\n';
    }
    for(int index=0;index<3;++index){
        dh::foundation::frontend::CreationPreview actor;
        dh::foundation::frontend::ClassPreviewScene scene;
        if(!actor.load(assets,error)||!scene.load(assets,error)||!actor.select(index,error))throw std::runtime_error(error);
        int previous=0;
        for(int time:{0,250,650,1300,1600,2500,4000,5000,5366,6000}){
            const int dt=time-previous;
            if(!scene.sample(index,dt,error)||!actor.update(dt/1000.,error))throw std::runtime_error(error);
            // sample publishes the cursor BEFORE dt, so refresh at zero delta
            // to record the source scene pose at the requested elapsed time.
            if(!scene.sample(index,0,error))throw std::runtime_error(error);
            const auto&c=scene.camera();
            std::cout<<"CAPTURE class "<<index<<" elapsed "<<time<<" sceneClip "<<scene.selected_clip()<<" sourceMs "<<scene.sampled_milliseconds()<<" camera "<<c.eye.x<<' '<<c.eye.y<<' '<<c.eye.z<<" target "<<c.target.x<<' '<<c.target.y<<' '<<c.target.z<<" body "<<actor.actors()[index].body.animation_name()<<" bodyClock "<<actor.actors()[index].body.animation_elapsed_seconds()<<" idleBoundary "<<actor.idle_transition_status()<<'\n';
            previous=time;
        }
    }
    return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
