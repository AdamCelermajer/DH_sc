#include "procedural_map_sources_v1.hpp"
#include "fixed_declarations_v1.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace dh2;
static void require(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
static std::string quote(const std::string& s){
    std::string out="\"";constexpr char hex[]="0123456789abcdef";
    for(unsigned char c:s){if(c=='\"'||c=='\\'){out+='\\';out+=char(c);}
        else if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}else out+=char(c);}
    return out+'\"';
}
int main(int argc,char** argv){
    try{
        require(argc==5,"Expected cache, identity, definition, seed");std::string error;
        const auto seed=std::stoul(argv[4]);require(seed<=UINT32_MAX,"Seed exceeds uint32 domain");
        loader::ProceduralMapSourcesV1 retained_sources;loader::FixedMapV1::Borrow retained;
        loader::FixedDeclarationsV1::Borrow declarations;
        {
            auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
            require(bool(*file),"Cache unavailable");const auto length=file->tellg();require(length>=0,"Cache length unavailable");
            assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=std::uint64_t(length);
            backing.read=[file](std::uint64_t pos,void* dst,std::size_t n,std::string& e){
                file->clear();file->seekg(std::streamoff(pos));file->read(static_cast<char*>(dst),std::streamsize(n));
                if(!*file){e="Cache read failed";return false;}return true;};
            assets::ZipAssetPackV1 pack;
            require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
            loader::ProceduralSourcesV1 sources;require(sources.prepare(pack,argv[2],argv[3],error),error);
            loader::ProceduralBlocksV1 blocks;require(blocks.prepare(sources.borrow(),error),error);
            loader::ProceduralConnectionsV1 connections;require(connections.prepare(blocks.borrow(),error),error);
            loader::ProceduralListsV1 lists;require(lists.prepare(connections.borrow(),error),error);
            loader::ProceduralRulesV1 rules;require(rules.prepare(lists.borrow(),error),error);
            loader::ProceduralLayoutResultV1 layout;
            require(loader::generate_procedural_layout_v1(rules.borrow(),std::uint32_t(seed),layout,error),error);
            if(!layout.generated){std::cout<<"{\"validation\":\"NO_LAYOUT\",\"generated\":false}\n";return 0;}
            loader::ProceduralModulePlanV1 modules;require(loader::prepare_procedural_modules_v1(pack,layout,modules,error),error);
            require(loader::prepare_procedural_map_sources_v1(pack,modules,retained_sources,error),error);
            const auto previous=retained_sources.sources;
            require(!loader::prepare_procedural_map_sources_v1(pack,{},retained_sources,error),"Missing source accepted");
            require(retained_sources.sources.identity()==previous.identity()&&retained_sources.sources.documents()[0].source()==previous.documents()[0].source(),"Failed map source replaced retained input");
            loader::FixedMapV1 map;require(map.prepare(pack,retained_sources.sources,error),error);retained=map.borrow();
            require(!map.prepare(pack,{},error),"Missing map source accepted");
            require(map.borrow().sources().identity()==argv[2],"Failed map replaced identity");
            loader::FixedDeclarationsV1 objects;require(objects.prepare(retained,error),error);declarations=objects.borrow();
            // Prepare and release a second owned candidate while retaining the first.
            loader::ProceduralMapSourcesV1 again;
            require(loader::prepare_procedural_map_sources_v1(pack,std::move(modules),again,error),error);
            require(map.prepare(pack,again.sources,error),error);
            require(map.borrow().modules().size()==retained.modules().size(),"Repeated assembly changed room count");
        }
        require(retained.sources().identity()==argv[2]&&retained_sources.modules->layout.source_owner,
                "Map or original source ownership lost after teardown");
        std::uint64_t indices=0,vertices=0,triangles=0,meshes=0;
        std::map<std::string,unsigned> kinds,types;
        for(const auto& instance:retained.instances()){
            const auto& asset=retained.assets().at(instance.asset);require(asset.view.bytes==asset.owner->data(),"BRES detached");
            const auto& placed=retained.scene().instances.at(instance.scene_instance);
            for(const auto value:placed.world)require(std::isfinite(value),"World transform not finite");
            for(const auto material:placed.materials)require(material<retained.scene().materials.size(),"Material missing");
            assets::Mesh mesh{};require(dh2_mesh_open(&mesh,&asset.view,placed.geometry)==assets::Error::ok,"Retained mesh unavailable");
            for(unsigned p=0;p<mesh.primitives;++p){assets::Primitive primitive{};
                require(dh2_mesh_primitive(&mesh,p,&primitive)==assets::Error::ok,"Primitive unavailable");
                for(unsigned j=0;j<primitive.index_count;++j){unsigned index;
                    require(dh2_index_read(&primitive,j,&index)&&index<mesh.vertices,"Mesh index unavailable");}
                indices+=primitive.index_count;
            }
            vertices+=mesh.vertices;
            const char* kind=instance.kind==loader::MapGeometryKindV1::mesh?"mesh":
                instance.kind==loader::MapGeometryKindV1::floor?"floor":
                instance.kind==loader::MapGeometryKindV1::exit?"exit":
                instance.kind==loader::MapGeometryKindV1::minimap?"minimap":
                instance.kind==loader::MapGeometryKindV1::module_root?"module_root":"unclassified";
            ++kinds[kind];meshes+=instance.kind==loader::MapGeometryKindV1::mesh;
        }
        require(meshes>0,"No room render meshes");
        for(const auto& floor:retained.navigation().records)triangles+=floor->triangles.size();
        require(retained.navigation().sewn,"Navigation not sewn");
        for(const auto& declaration:declarations.declarations()){
            const auto& element=declarations.element(declaration);const auto* type=element.attribute("gametype");
            ++types[type?*type:"<absent>"];
            require(!declarations.document(declaration).source().empty(),"Selected declaration source lost");
        }
        std::cout<<"{\"validation\":\"PASS\",\"identity\":"<<quote(retained.sources().identity())
            <<",\"seed\":"<<seed<<",\"generated\":true,\"module_count\":"<<retained.modules().size()
            <<",\"source_documents\":"<<retained.sources().documents().size()<<",\"assets\":"<<retained.assets().size()
            <<",\"vertices\":"<<vertices<<",\"indices\":"<<indices<<",\"navigation\":{\"floors\":"<<retained.navigation().records.size()
            <<",\"triangles\":"<<triangles<<",\"nodes\":"<<retained.navigation().graph.node_count
            <<",\"edges\":"<<retained.navigation().graph.edge_count<<"},\"geometry_kinds\":{";bool comma=false;
        for(const auto& row:kinds){if(comma)std::cout<<',';comma=true;std::cout<<quote(row.first)<<':'<<row.second;}
        std::cout<<"},\"declaration_types\":{";comma=false;
        for(const auto& row:types){if(comma)std::cout<<',';comma=true;std::cout<<quote(row.first)<<':'<<row.second;}
        std::cout<<"},\"modules\":[";comma=false;
        for(const auto& module:retained.modules()){
            if(comma)std::cout<<',';
            comma=true;
            const auto& element=retained.sources().documents().at(module.document).elements().at(module.element);
            std::cout<<"{\"name\":"<<quote(module.authored_name)<<",\"position\":"<<quote(*element.attribute("position"))
                <<",\"xrefobject\":"<<quote(module.xrefobject)<<",\"dae\":"<<quote(*element.attribute("dae"))<<'}';
        }
        std::cout<<"],\"ownership_checks\":true,\"repeated_preparation_checked\":true,\"map_render_verified\":false,\"gameplay_verified\":false}\n";
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
