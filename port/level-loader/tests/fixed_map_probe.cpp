#include "fixed_map_v1.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
using namespace dh2;
static void require(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
static std::string quote(const std::string& s) {
    std::string out="\"";
    for(unsigned char c:s){if(c=='"'||c=='\\')out+='\\';if(c>=32)out+=static_cast<char>(c);else throw std::runtime_error("JSON text outside probe domain");}
    return out+'"';
}
static const char* kind(loader::MapGeometryKindV1 value) {
    switch(value) {
        case loader::MapGeometryKindV1::mesh:return "mesh";
        case loader::MapGeometryKindV1::floor:return "floor";
        case loader::MapGeometryKindV1::exit:return "exit";
        case loader::MapGeometryKindV1::minimap:return "minimap";
        case loader::MapGeometryKindV1::module_root:return "module_root";
        default:return "unclassified";
    }
}
int main(int argc,char** argv) {
    if(argc!=4)return 2;
    try {
        loader::FixedMapV1::Borrow retained;
        std::string error;
        {
            auto file=std::make_shared<std::ifstream>(argv[1],std::ios::binary|std::ios::ate);
            require(bool(*file),"Cache unavailable");const auto length=file->tellg();require(length>=0,"Cache length unavailable");
            assets::ZipBackingV1 backing;backing.owner=file;backing.bytes=static_cast<std::uint64_t>(length);
            backing.read=[file](std::uint64_t at,void* dst,std::size_t count,std::string& err) {
                file->clear();file->seekg(static_cast<std::streamoff>(at));file->read(static_cast<char*>(dst),static_cast<std::streamsize>(count));
                if(!*file){err="Cache read failed";return false;}return true;
            };
            assets::ZipAssetPackV1 pack;
            require(pack.mount(std::move(backing),"com.gameloft.android.GAND.GloftD2SS/files/",error),error);
            loader::FixedSourcesV1 sources;require(sources.prepare(pack,argv[2],argv[3],error),error);
            loader::FixedMapV1 map;require(map.prepare(pack,sources.borrow(),error),error);retained=map.borrow();
            require(!map.prepare(pack,{},error),"Empty source map succeeded");
            require(map.borrow().sources().identity()==argv[2],"Failure replaced existing map");
            // Subsequent preparations release independent candidates while an
            // older borrow retains its BRES, XML and native floor graph owners.
            for(unsigned i=0;i<2;++i) {
                require(sources.prepare(pack,"separate-visit",argv[3],error),error);
                require(map.prepare(pack,sources.borrow(),error),error);
                require(map.borrow().sources().identity()=="separate-visit","New identity missing");
                require(retained.sources().identity()==argv[2],"Older map identity changed");
            }
        }
        require(bool(retained),"Retained map lost");
        std::map<std::string,unsigned> counts;
        std::uint64_t vertices=0,indices=0,primitives=0;
        std::array<float,3> low,high;low.fill(std::numeric_limits<float>::infinity());high.fill(-std::numeric_limits<float>::infinity());
        for(const auto& i:retained.instances()) {
            ++counts[kind(i.kind)];const auto& asset=retained.assets().at(i.asset);
            require(asset.view.bytes==asset.owner->data(),"BRES borrow detached from owner");
            const auto& instance=retained.scene().instances.at(i.scene_instance);
            require(instance.node_index<retained.scene().graph.size(),"Placed node missing");
            for(auto material:instance.materials)require(material<retained.scene().materials.size(),"Placed material missing");
            assets::Mesh mesh{};require(dh2_mesh_open(&mesh,&asset.view,instance.geometry)==assets::Error::ok,"Retained mesh unavailable");
            for(unsigned p=0;p<mesh.primitives;++p) {
                assets::Primitive primitive{};require(dh2_mesh_primitive(&mesh,p,&primitive)==assets::Error::ok,"Retained primitive unavailable");
                for(unsigned j=0;j<primitive.index_count;++j){unsigned index=0;require(dh2_index_read(&primitive,j,&index)&&index<mesh.vertices,"Retained index unavailable");}
                primitives++;indices+=primitive.index_count;
                if(i.kind!=loader::MapGeometryKindV1::mesh)continue;
                assets::Attribute positions{};require(dh2_mesh_attribute(&mesh,primitive.attributes[0],&positions)==assets::Error::ok,"Retained positions unavailable");
                require(positions.components>=3&&positions.components<=4,"Unsupported map position layout");
                vertices+=positions.vertices;
                for(unsigned v=0;v<positions.vertices;++v) {
                    float point[4]{};require(dh2_attribute_read(&positions,v,point),"Retained vertex unavailable");
                    for(unsigned k=0;k<3;++k) {
                        const auto& w=instance.world;
                        const float x=w[12+k]+w[k]*point[0]+w[4+k]*point[1]+w[8+k]*point[2];
                        require(std::isfinite(x),"Placed vertex not finite");low[k]=std::min(low[k],x);high[k]=std::max(high[k],x);
                    }
                }
            }
        }
        require(counts["mesh"]>0,"No named module render meshes");
        const auto& navigation=retained.navigation();require(navigation.sewn&&navigation.graph.node_count>0,"Retained navigation unavailable");
        std::map<unsigned,unsigned> floor_flag_counts;
        for(const auto& metadata:retained.floor_metadata()) {
            const auto& floor=*navigation.records.at(metadata.floor_record);
            require(floor.flags.floor==metadata.flags.floor&&floor.flags.object==metadata.flags.object,"Resolved floor flags lost");
            ++floor_flag_counts[floor.flags.floor];
        }
        std::uint64_t triangles=0;for(const auto& floor:navigation.records)triangles+=floor->triangles.size();
        std::cout<<std::setprecision(9)<<"{\"validation\":\"PASS\",\"identity\":"<<quote(retained.sources().identity())
            <<",\"asset_count\":"<<retained.assets().size()<<",\"module_count\":"<<retained.modules().size()
            <<",\"nodes\":"<<retained.scene().graph.size()<<",\"materials\":"<<retained.scene().materials.size()<<",\"geometry_kinds\":{";
        bool comma=false;for(const auto& row:counts){if(comma)std::cout<<',';comma=true;std::cout<<quote(row.first)<<':'<<row.second;}
        std::cout<<"},\"mesh_vertex_visits\":"<<vertices<<",\"all_indices\":"<<indices<<",\"all_primitives\":"<<primitives
            <<",\"navigation\":{\"floors\":"<<navigation.records.size()<<",\"triangles\":"<<triangles<<",\"nodes\":"<<navigation.graph.node_count
            <<",\"edges\":"<<navigation.graph.edge_count<<",\"sewn\":true},\"mesh_bounds\":[[";
        for(unsigned j=0;j<3;++j){if(j)std::cout<<',';std::cout<<low[j];}std::cout<<"],[";
        for(unsigned j=0;j<3;++j){if(j)std::cout<<',';std::cout<<high[j];}std::cout<<"]],\"modules\":[";comma=false;
        for(const auto& m:retained.modules()) {
            if(comma)std::cout<<',';
            comma=true;
            std::cout<<"{\"name\":"<<quote(m.authored_name)<<",\"xrefobject\":"<<quote(m.xrefobject)<<",\"asset\":"<<m.asset<<",\"position\":[";
            for(unsigned j=0;j<3;++j){if(j)std::cout<<',';std::cout<<m.transform.position[j];}std::cout<<"]}";
        }
        std::cout<<"],\"modules_without_floors\":[";comma=false;
        for(unsigned j=0;j<retained.modules().size();++j) {
            const bool has_floor=std::any_of(navigation.records.begin(),navigation.records.end(),[j](const auto& floor){return floor->room==j;});
            if(!has_floor){if(comma)std::cout<<',';comma=true;std::cout<<quote(retained.modules()[j].authored_name);}
        }
        std::cout<<"],\"floor_flag_counts\":{";comma=false;
        for(const auto& row:floor_flag_counts){if(comma)std::cout<<',';comma=true;std::cout<<quote(std::to_string(row.first))<<':'<<row.second;}
        std::cout<<"},\"unclassified_geometry\":[";comma=false;
        for(const auto& i:retained.instances())if(i.kind==loader::MapGeometryKindV1::unclassified) {
            if(comma)std::cout<<',';
            comma=true;
            const auto& instance=retained.scene().instances.at(i.scene_instance);
            std::cout<<quote(retained.scene().graph.at(instance.node_index).name);
        }
        std::cout<<"],\"floor_metadata\":[";comma=false;
        for(const auto& metadata:retained.floor_metadata()) {
            if(comma)std::cout<<',';
            comma=true;
            const auto& instance=retained.scene().instances.at(metadata.scene_instance);
            const auto& node=retained.scene().graph.at(instance.node_index);
            std::cout<<"{\"node\":"<<quote(node.name)<<",\"raw\":";
            // JSON control characters must be escaped in original properties.
            std::string encoded="\"";constexpr char hex[]="0123456789abcdef";
            for(unsigned char c:node.user_properties) {
                if(c=='"'||c=='\\'){encoded+='\\';encoded+=static_cast<char>(c);}
                else if(c<32){encoded+="\\u00";encoded+=hex[c>>4];encoded+=hex[c&15];}
                else encoded+=static_cast<char>(c);
            }
            std::cout<<encoded<<"\",\"floor_flags\":"<<metadata.flags.floor<<",\"object_flags\":"<<metadata.flags.object<<'}';
        }
        std::cout<<"],\"ownership_checks\":true,\"map_render_verified\":false,\"gameplay_verified\":false}\n";
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
