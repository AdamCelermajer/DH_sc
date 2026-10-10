// Checked serialized-node/material traversal reused from scene-materials/scene.cpp.
// Selection is the separately recovered database getNode61c290/61c214 path.
#include "module_selected_scene_v2.hpp"
#include "../engine-math/math.hpp"
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <algorithm>
namespace dh2::world {
using scene::Scene;using scene::Material;using scene::Instance;using scene::Node;
namespace {
using resources::Library;
using Matrix=std::array<float,16>;
[[maybe_unused]] Matrix identity(){return {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};}
struct Reader {
    const resources::BresView& v;
    const std::uint8_t* at(std::uint64_t p,std::uint64_t n) const {
        if(!v.bytes || p>v.size || n>v.size-p) throw std::runtime_error("Scene field outside BRES");
        return v.bytes+p;
    }
    std::uint32_t w(std::uint64_t p) const {
        const auto* b=at(p,4);return b[0]|(std::uint32_t(b[1])<<8)|(std::uint32_t(b[2])<<16)|(std::uint32_t(b[3])<<24);
    }
    float f(std::uint64_t p) const {
        auto b=w(p);float x;std::memcpy(&x,&b,4);
        if(!std::isfinite(x))throw std::runtime_error("Nonfinite scene component");
        return x;
    }
    std::string text(std::uint32_t p,bool optional=false) const {
        if(!p){if(optional)return {};throw std::runtime_error("Missing scene string");}
        at(p,1);const auto n=std::min<std::size_t>(v.size-p,4096);
        const auto* end=static_cast<const std::uint8_t*>(std::memchr(v.bytes+p,0,n));
        if(!end)throw std::runtime_error("Unterminated scene string");
        return std::string(reinterpret_cast<const char*>(v.bytes+p),end-(v.bytes+p));
    }
    std::string field(std::uint64_t p,bool optional=false) const{return text(w(p),optional);}
    std::uint32_t item(Library lib,unsigned i) const {
        const auto* p=dh2_bres_library_item(&v,lib,i);
        if(!p)throw std::runtime_error("Missing library item");
        return std::uint32_t(p-v.bytes);
    }
    void array(std::uint64_t p,std::uint32_t n,unsigned stride)const {
        if(n>100000)throw std::runtime_error("Scene array exceeds limit");
        at(p,std::uint64_t(n)*stride);
    }
    unsigned find(Library lib,std::string uri)const {
        if(uri.empty()||uri[0]!='#')throw std::runtime_error("Expected local fragment reference");
        uri.erase(0,1);
        const unsigned count=dh2_bres_library_count(&v,lib);
        for(unsigned i=0;i<count;++i)if(field(item(lib,i)+(lib==Library::controller?4:0))==uri)return i;
        throw std::runtime_error("Unresolved local fragment: "+uri);
    }
    std::string image(unsigned param)const {
        if(w(param+12)!=1)throw std::runtime_error("Sampler arrays not supported");
        const auto counts=w(param+16),values=w(param+20);
        if(w(counts)!=1)throw std::runtime_error("Sampler value count differs");
        const auto index=w(w(values));
        if(index==0xffffffff)return {};
        const auto path=field(item(Library::image,index)+8);
        const auto pos=path.find_last_of("/\\");const auto name=path.substr(pos==std::string::npos?0:pos+1);
        if(name.empty()||name=="."||name=="..")throw std::runtime_error("Invalid image basename");
        return name;
    }
};
struct Loader {
    Reader r;Scene& s;std::vector<unsigned> path;std::vector<std::uint8_t>& node_visibility;std::vector<std::uint8_t>& instance_visibility;
    void materials(){
        const unsigned n=dh2_bres_library_count(&r.v,Library::material);
        if(n>4096)throw std::runtime_error("Too many materials");
        for(unsigned i=0;i<n;++i){
            auto p=r.item(Library::material,i);Material m;m.id=r.field(p);
            m.effect_file=r.field(p+8,true);m.effect_uri=r.field(p+12,true);
            auto count=r.w(p+16),base=r.w(p+20);r.array(base,count,24);
            for(unsigned j=0;j<count;++j){
                auto q=base+24*j;auto name=r.field(q);const auto type=r.w(q+8),data=r.w(q+20);
                if(name=="Multilight-fx-profile_GLES2/CurrentTechnique"){
                    if(type!=20||r.w(q+12)!=1||r.w(r.w(q+16))!=1)throw std::runtime_error("Actual Module GLES2 technique shape differs");r.at(data,8);m.gles2_technique=r.field(data+4);
                }else if(name=="Diffuse"||name=="diffuse-sampler"||name=="AlphaMap"){
                    if(type!=11)throw std::runtime_error("Unexpected image parameter type");
                    auto img=r.image(q);if(name=="AlphaMap")m.alpha_map=img;else m.diffuse=img;
                }else if(name=="__irrlicht_Diffuse_color"){
                    if(type!=7||r.w(q+12)!=1)throw std::runtime_error("Unexpected material color layout");
                    for(unsigned k=0;k<4;++k)m.color[k]=r.f(data+4*k);
                }else if(name=="texture-matrix"){
                    if(type!=10||r.w(q+12)!=1)throw std::runtime_error("Unexpected texture matrix layout");
                    for(unsigned k=0;k<16;++k)m.texture_matrix[k]=r.f(data+4*k);
                }else if(name=="Object_Alpha"||name=="alpha-ref"){
                    if(type!=4||r.w(q+12)!=1)throw std::runtime_error("Unexpected scalar parameter layout");
                    if(name=="Object_Alpha")m.color[3]*=r.f(data);else m.alpha_ref=r.f(data);
                }else if(name=="__irrlicht_Additive"||name=="__irrlicht_Backface_Culling"){
                    if(type!=0||r.w(q+12)!=1)throw std::runtime_error("Unexpected boolean parameter layout");
                    if(name=="__irrlicht_Additive")m.additive=r.w(data)!=0;else m.backface=r.w(data)!=0;
                }
            }
            s.materials.push_back(std::move(m));
        }
    }
    void node(unsigned p,const Matrix& parent,bool visible,std::int32_t parent_index=-1){
        if(path.size()>=64||s.nodes>=10000||std::find(path.begin(),path.end(),p)!=path.end())
            throw std::runtime_error("Cyclic or excessive scene graph");
        r.at(p,80);path.push_back(p);++s.nodes;
        float translation[3],scale[3],quaternion[4];
        for(unsigned i=0;i<3;++i){translation[i]=r.f(p+12+i*4);scale[i]=r.f(p+40+i*4);}
        for(unsigned i=0;i<4;++i)quaternion[i]=r.f(p+24+i*4);
        // ISceneNode::getRelativeTransformation calls getMatrix_transposed
        // then postScale; columns 0..2 receive scale, slots 12..14 translation.
        Matrix m;dh2_node_matrix(m.data(),translation,quaternion,scale);auto world=scene::multiply(parent,m);
        for(float x:world)if(!std::isfinite(x))throw std::runtime_error("Scene transform overflow");
        const auto node_index=std::uint32_t(s.graph.size());
        Node state;state.id=r.field(p);state.sid=r.field(p+8,true);state.name=r.field(p+4,true);state.parent=parent_index;state.world=world;
        // CSceneNode::getUserPropertyStr reads the first String at SNode+72.
        if(const auto properties=r.w(p+72))state.user_properties=r.field(properties,true);
        std::copy(translation,translation+3,state.translation);std::copy(scale,scale+3,state.scale);std::copy(quaternion,quaternion+4,state.quaternion);
        state.source_storage_v91().visibility={std::uint8_t(r.w(p+52)!=0),std::uint8_t(visible)};
        s.graph.push_back(std::move(state));
        visible=visible&&r.w(p+52)!=0;node_visibility.push_back(visible?1u:0u);
        auto count=r.w(p+64),base=r.w(p+68);r.array(base,count,8);
        for(unsigned i=0;i<count;++i){
            auto a=base+8*i;
            const auto tag=r.w(a);
            if(tag==4){const auto link=r.w(a+4);r.at(link,8);if(r.w(link))throw std::runtime_error("Actual external Module light database transport required");s.lights_v113.push_back({node_index,r.find(Library::light,r.field(link+4))});continue;}
            if(tag!=3&&tag!=2){++s.ignored_instances;continue;}
            auto g=r.w(a+4);r.at(g,24);
            if(r.w(g))throw std::runtime_error("External geometry not supported");
            std::int32_t controller=-1;std::string geometry_uri=r.field(g+4);
            if(tag==2){
                controller=r.find(Library::controller,geometry_uri);const auto record=r.item(Library::controller,controller);
                if(r.w(record)!=0)throw std::runtime_error("Unsupported controller type");
                const auto skin=r.w(record+8);r.at(skin,156);geometry_uri=r.field(skin+112);
            }
            unsigned geometry=r.find(Library::geometry,geometry_uri);
            Instance instance{r.field(p),node_index,geometry,world,{}};
            instance.controller=controller;
            const auto n=r.w(g+12),bindings=r.w(g+16);r.array(bindings,n,60);
            // The instance_material rows are the targets, while the runtime
            // binding key is each geometry primitive's authored material
            // symbol. Keep both domains together for the retained Module
            // mesh borrower/compiler, just as the general scene loader does.
            assets::Mesh mesh{};
            if(dh2_mesh_open(&mesh,&r.v,geometry)!=assets::Error::ok||mesh.primitives!=n)
                throw std::runtime_error("Actual Module material binding/primitive domain differs");
            for(unsigned j=0;j<n;++j){
                const auto b=bindings+60*j;
                if(r.w(b))throw std::runtime_error("External material binding not supported");
                assets::Primitive primitive{};
                if(dh2_mesh_primitive(&mesh,j,&primitive)!=assets::Error::ok||!primitive.material)
                    throw std::runtime_error("Actual Module source primitive material symbol missing");
                instance.materials.push_back(r.find(Library::material,r.field(b+4)));
                instance.material_symbols_v1.push_back({primitive.material});
            }
            s.instances.push_back(std::move(instance));instance_visibility.push_back(visible?1u:0u);
        }
        count=r.w(p+56);base=r.w(p+60);r.array(base,count,80);
        for(unsigned i=0;i<count;++i)node(base+80*i,world,visible,node_index);
        path.pop_back();
    }

    unsigned find_node(unsigned p,const std::string& id,unsigned depth=0){
        if(depth>=64)throw std::runtime_error("Excessive selected node search");
        r.at(p,80);if(r.field(p)==id)return p;
        auto n=r.w(p+56),children=r.w(p+60);r.array(children,n,80);
        for(unsigned i=0;i<n;++i)if(auto hit=find_node(children+80*i,id,depth+1))return hit;
        return 0;
    }
    bool selected(const std::string& id){
        auto root=r.v.root_offset;r.at(root,192);
        auto count=r.w(root+152),scenes=r.w(root+156);r.array(scenes,count,16);
        if(!count)return false;
        auto n=r.w(scenes+8),nodes=r.w(scenes+12);r.array(nodes,n,80);
        for(unsigned i=0;i<n;++i)if(auto hit=find_node(nodes+80*i,id)){
            materials();node(hit,identity(),true);return true;
        }
        return false;
    }
};
}
bool module_selected_scene_v2(const resources::BresView& v,const char* xref,ModuleSelectedSceneV2& out,bool& found,std::string& e){
 found=false;out={};e.clear();
 if(!xref||!*xref){e="Required nonempty authored Module xref";return false;}
 try{ModuleSelectedSceneV2 candidate;candidate.source_image_v93=v;Loader loader{{v},candidate.scene,{},candidate.node_visibility,candidate.instance_visibility};
     found=loader.selected(std::string(xref)+"-node");out=std::move(candidate);return true;}
 catch(const std::exception& error){e=error.what();return false;}
}
}
