#include "../original_character.hpp"
#include "../asset_catalog.hpp"
#include "../../engine-skinning/skinning.hpp"
#include <iostream>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include <cmath>
using namespace dh::foundation;
int main(int argc,char**argv){try{
    if(argc!=5)throw std::runtime_error("usage: actor_material_probe assets model template idle");
    AssetCatalog assets(argv[1]);CharacterVisualConfig config;config.model_path=argv[2];config.template_clip_path=argv[3];config.clips={{"preview",argv[4]}};config.allow_missing_animation_targets=true;
    CharacterVisual visual;std::string error;if(!visual.load(assets,config,error)||!visual.update(.1,error))throw std::runtime_error(error);
    auto bytes=assets.read(config.model_path);dh2::resources::BresView image{};if(dh2_bres_open(&image,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)throw std::runtime_error("BRES");
    dh2::scene::Scene scene;if(!dh2::scene::load(image,scene,error))throw std::runtime_error(error);
    std::cout<<"model="<<config.model_path<<" instances="<<scene.instances.size()<<" controllers="<<dh2_bres_library_count(&image,dh2::resources::Library::controller)<<"\n";
    for(std::size_t i=0;i<scene.materials.size();++i){const auto& material=scene.materials[i];std::cout<<"sourceMaterial="<<i<<" id="<<material.id<<" diffuse="<<material.diffuse<<"\n";}
    for(const auto& instance:scene.instances){std::cout<<"instance="<<instance.node<<" geometry="<<instance.geometry<<" controller="<<instance.controller<<" materials=";for(auto m:instance.materials)std::cout<<m<<",";std::cout<<"\n";}
    for(unsigned i=0;i<dh2_bres_library_count(&image,dh2::resources::Library::controller);++i){dh2::skinning::Skin skin;if(!dh2::skinning::load(image,i,scene,skin,error))throw std::runtime_error(error);std::cout<<"controller="<<skin.id<<" geometry="<<skin.geometry<<" joints="<<skin.nodes.size()<<"\n";}
    for(std::size_t i=0;i<visual.meshes().size();++i){const auto& mesh=visual.meshes()[i];const auto& material=visual.original_materials()[i];float lo[3]{INFINITY,INFINITY,INFINITY},hi[3]{-INFINITY,-INFINITY,-INFINITY};float ulo=INFINITY,uhi=-INFINITY,vlo=INFINITY,vhi=-INFINITY;
        for(const auto& vertex:mesh.vertices){float p[]{vertex.position.x,vertex.position.y,vertex.position.z};for(unsigned a=0;a<3;++a){lo[a]=std::min(lo[a],p[a]);hi[a]=std::max(hi[a],p[a]);}ulo=std::min(ulo,vertex.u);uhi=std::max(uhi,vertex.u);vlo=std::min(vlo,vertex.v);vhi=std::max(vhi,vertex.v);}
        std::cout<<"mesh="<<i<<" vertices="<<mesh.vertices.size()<<" triangles="<<mesh.indices.size()/3<<" material="<<material.id<<" diffuse="<<material.diffuse<<" alpha="<<material.alphaMap<<" technique="<<material.technique<<" effect="<<material.effectFile<<"\n bounds="<<lo[0]<<","<<lo[1]<<","<<lo[2]<<".."<<hi[0]<<","<<hi[1]<<","<<hi[2]<<" uv="<<ulo<<","<<vlo<<".."<<uhi<<","<<vhi<<"\n textureMatrix=";for(float value:material.textureMatrix)std::cout<<value<<",";std::cout<<"\n";
    }return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
