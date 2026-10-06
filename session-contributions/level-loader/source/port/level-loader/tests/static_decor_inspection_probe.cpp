#include "level_preparation_v1.hpp"
#include "static_decor_inspection_v1.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace dh2;
static void require(bool ok,const std::string& e){if(!ok)throw std::runtime_error(e);}
int main(int argc,char** argv){try{
    require(argc==5,"Expected cache, identity, definition, seed");std::string e;loader::StaticDecorInspectionV1 retained;
    {
        auto f=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);require(bool(*f),"Cache unavailable");
        assets::ZipBackingV1 b;b.owner=f;b.bytes=std::uint64_t(f->tellg());b.read=[f](std::uint64_t at,void* d,std::size_t n,std::string& e){f->clear();f->seekg(std::streamoff(at));f->read(static_cast<char*>(d),std::streamsize(n));if(!*f){e="Read failed";return false;}return true;};
        assets::ZipAssetPackV1 pack;require(pack.mount(b,"com.gameloft.android.GAND.GloftD2SS/files/",e),e);
        const std::string def=argv[3];const auto kind=def.find(".rule.xml")!=std::string::npos?loader::LevelSourceKindV1::procedural:loader::LevelSourceKindV1::fixed;
        loader::LevelPreparationV1 p(pack);require(p.begin({argv[2],def,kind,std::uint32_t(std::stoul(argv[4]))},e),e);
        while(p.step()==loader::LevelPreparationStepV1::pending){}require(p.stage()==loader::LevelPreparationStageV1::source_ready,p.error());
        require(loader::prepare_static_decor_inspection_v1(pack,p.latest_source().declarations(),retained,e),e);
        const auto prior=retained.source_owner;const auto count=retained.declarations.size();
        require(!loader::prepare_static_decor_inspection_v1(pack,{},retained,e),"Missing source accepted");
        require(retained.source_owner.map().sources().identity()==prior.map().sources().identity()&&retained.declarations.size()==count,"Failure replaced decor inspection");
    }
    unsigned meshes=0,vertices=0,skinned=0;
    require(!retained.map||(!retained.map.navigation_prepared()&&!retained.map.navigation().sewn),"Inspection unexpectedly prepared a navigation world");
    if(retained.map)for(const auto& i:retained.map.instances())if(i.kind==loader::MapGeometryKindV1::mesh){
        const auto& instance=retained.map.scene().instances.at(i.scene_instance);const auto& asset=retained.map.assets().at(i.asset);
        assets::Mesh mesh{};require(dh2_mesh_open(&mesh,&asset.view,instance.geometry)==assets::Error::ok,"Retained decor mesh unavailable");++meshes;vertices+=mesh.vertices;skinned+=instance.controller>=0;
        const auto& declaration=retained.declarations.at(i.module);
        require(retained.source_owner.element(declaration).attribute("gametype")&&*retained.source_owner.element(declaration).attribute("gametype")=="AnimatedDecor","Original decor source lost");
        for(unsigned k=0;k<3;++k)require(retained.map.modules().at(i.module).transform.position[k]==declaration.translated_position->at(k),"Decor module offset changed");
        for(auto v:instance.world)require(std::isfinite(v),"Decor transform not finite");
    }
    std::cout<<"{\"validation\":\"PASS\",\"decor_sources\":"<<retained.declarations.size()<<",\"mesh_instances\":"<<meshes<<",\"vertices\":"<<vertices<<",\"skinned_meshes\":"<<skinned<<",\"skipped_sources\":"<<retained.skipped.size()<<",\"source_pose_only\":true,\"runtime_objects_verified\":false}\n";
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
