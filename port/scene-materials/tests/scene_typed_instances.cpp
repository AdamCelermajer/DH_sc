#include "../scene.hpp"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {
using Bytes=std::vector<std::uint8_t>;
std::uint32_t word(const Bytes& b,std::size_t p){
    assert(p<=b.size()&&b.size()-p>=4);
    return std::uint32_t(b[p])|(std::uint32_t(b[p+1])<<8)|(std::uint32_t(b[p+2])<<16)|(std::uint32_t(b[p+3])<<24);
}
std::string text(const Bytes& b,std::uint32_t p){
    assert(p<b.size());auto end=p;
    while(end<b.size()&&b[end])++end;
    assert(end<b.size());return {reinterpret_cast<const char*>(b.data()+p),end-p};
}
std::uint32_t find_node_instance(const Bytes& b,std::uint32_t node,std::uint32_t wanted,unsigned depth=0){
    assert(depth<64&&node<=b.size()&&b.size()-node>=80);
    const auto count=word(b,node+64),base=word(b,node+68);
    assert(count<=100000&&base<=b.size()&&std::uint64_t(count)*8<=b.size()-base);
    for(unsigned i=0;i<count;++i){const auto descriptor=base+i*8;if(word(b,descriptor)==wanted)return word(b,descriptor+4);}
    const auto children=word(b,node+56),child_base=word(b,node+60);
    assert(children<=100000&&child_base<=b.size()&&std::uint64_t(children)*80<=b.size()-child_base);
    for(unsigned i=0;i<children;++i)if(const auto hit=find_node_instance(b,child_base+i*80,wanted,depth+1))return hit;
    return 0;
}
std::uint32_t find_instance_uri(const Bytes& b,std::uint32_t wanted){
    const auto root=word(b,32),scene_count=word(b,root+152),scene_base=word(b,root+156);
    const auto instance_count=word(b,root+184),instances=word(b,root+188);
    assert(scene_count<=100000&&scene_base<=b.size()&&std::uint64_t(scene_count)*16<=b.size()-scene_base);
    assert(instance_count<=100000&&instances<=b.size()&&std::uint64_t(instance_count)*8<=b.size()-instances);
    for(unsigned i=0;i<instance_count;++i){
        const auto descriptor=instances+i*8;if(word(b,descriptor)!=6)continue;
        const auto link=word(b,descriptor+4);assert(link<=b.size()&&b.size()-link>=8);
        const auto visual_uri=text(b,word(b,link+4));assert(!visual_uri.empty()&&visual_uri[0]=='#');
        for(unsigned j=0;j<scene_count;++j){
            const auto row=scene_base+j*16;if(text(b,word(b,row))!=visual_uri.substr(1))continue;
            const auto node_count=word(b,row+8),nodes=word(b,row+12);
            assert(node_count<=100000&&nodes<=b.size()&&std::uint64_t(node_count)*80<=b.size()-nodes);
            for(unsigned k=0;k<node_count;++k)if(const auto instance=find_node_instance(b,nodes+k*80,wanted))return instance;
        }
    }
    return 0;
}
bool load(const Bytes& b,dh2::scene::Scene& out,std::string& error){
    dh2::resources::BresView view{};
    return dh2_bres_open(&view,b.data(),b.size())==dh2::resources::BresError::ok&&dh2::scene::load(view,out,error);
}
void expect_bad_ref(Bytes b,std::uint32_t tag){
    const auto record=find_instance_uri(b,tag);assert(record&&record<=b.size()&&b.size()-record>=8);
    const auto uri=word(b,record+4);assert(uri<b.size()&&b[uri]=='#'&&uri+1<b.size());
    b[uri+1]='!';
    dh2::scene::Scene scene;std::string error;
    assert(!load(b,scene,error)&&!error.empty());
    assert(scene.nodes==0&&scene.materials.empty()&&scene.instances.empty()&&scene.cameras_v1.empty()&&scene.lights_v113.empty());
}
}

int main(int argc,char** argv){
    if(argc!=2)return 2;
    std::ifstream file(argv[1],std::ios::binary);
    Bytes bytes((std::istreambuf_iterator<char>(file)),{});
    if(bytes.empty())return 3;
    dh2::scene::Scene scene;std::string error;
    if(!load(bytes,scene,error))return 4;
    // class_selection.bdae is the focused authored sample for both records.
    assert(scene.nodes>0&&scene.instances.size()==2);
    assert(scene.cameras_v1.size()==1);
    const auto& camera=scene.cameras_v1.front();
    assert(camera.node_index<scene.graph.size()&&camera.camera==0&&camera.id=="Camera01-camera");
    assert(camera.kind==0&&camera.horizontal_fov_or_mag>0&&camera.aspect==1.5f);
    assert(camera.znear==1.f&&camera.zfar==1000.f&&camera.target_uri.empty());
    assert(scene.lights_v113.size()==1);
    const auto& light=scene.lights_v113.front();
    assert(light.node_index<scene.graph.size()&&light.light==0&&light.id=="Omni01-light");
    assert(light.type==1&&light.parameter_count==3&&light.authored_color[0]==255&&light.authored_color[3]==255);
    assert(std::isfinite(light.intensity)&&light.intensity>0&&light.color[0]>0&&light.color[3]>0);
    for(unsigned i=0;i<light.parameter_count;++i)assert(std::isfinite(light.parameters[i]));
    expect_bad_ref(bytes,1);
    expect_bad_ref(bytes,4);
    return 0;
}
