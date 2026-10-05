#include "particle_force_scene_v1.hpp"
#include <cstring>
#include <functional>
#include <map>
#include <stdexcept>
namespace dh2::animation {
const std::array<float,16>* ParticleGravitySceneV1::matrix()const noexcept{return scene&&node_index<scene->graph.size()?&scene->graph[node_index].world:nullptr;}
bool decode_particle_gravity_scene_v1(const resources::BresView& image,const scene::Scene& scene,std::vector<ParticleGravitySceneV1>& out,std::string& error){
 error.clear();try{
 auto span=[&](std::uint64_t p,std::uint64_t n){if(p>image.size||n>image.size-p)throw std::runtime_error("Particle force BRES range");return image.bytes+p;};
 auto word=[&](std::uint32_t p){std::uint32_t v;std::memcpy(&v,span(p,4),4);return v;};
 auto string=[&](std::uint32_t p){const char* b=reinterpret_cast<const char*>(span(p,1));auto end=static_cast<const char*>(std::memchr(b,0,image.size-p));if(!end)throw std::runtime_error("Particle force string");return std::string(b,end);};
 auto number=[&](std::uint32_t p){float v;std::memcpy(&v,span(p,4),4);return v;};
 const auto count=word(image.root_offset+0x88),table=word(image.root_offset+0x8c);
 if(count>4096)throw std::runtime_error("Particle force count");
 span(table,std::uint64_t(count)*16);
 std::map<std::string,std::uint32_t> forces;
 for(unsigned i=0;i<count;++i){auto p=table+16*i;if(word(p+8)!=0)throw std::runtime_error("Required non-gravity particle force");auto name=string(word(p));if(!forces.emplace(name,p).second)throw std::runtime_error("Duplicate source particle force name");span(word(p+12),12);}
 std::vector<ParticleGravitySceneV1> result;unsigned depth=0;
 std::function<void(std::uint32_t)> visit=[&](std::uint32_t node){if(++depth>64)throw std::runtime_error("Particle force node depth");span(node,80);const auto n=word(node+64),items=word(node+68);if(n>4096)throw std::runtime_error("Particle force instance count");span(items,std::uint64_t(n)*8);
 for(unsigned i=0;i<n;++i){auto at=items+8*i;if(word(at)!=12)continue;auto instance=word(at+4);span(instance,8);auto uri=string(word(instance+4));if(uri.empty()||uri[0]!='#')throw std::runtime_error("Particle force source URI");auto f=forces.find(uri.substr(1));if(f==forces.end())throw std::runtime_error("Particle force source named lookup");auto id=string(word(node));unsigned found=0,index=0;for(unsigned j=0;j<scene.graph.size();++j)if(scene.graph[j].id==id){++found;index=j;}if(found!=1)throw std::runtime_error("Particle force same scene node binding");auto p=word(f->second+12);std::uint32_t mode=word(p+8);result.push_back({f->first,id,index,number(p),number(p+4),mode,&scene});}
 auto children=word(node+56),a=word(node+60);if(children>4096)throw std::runtime_error("Particle force child count");span(a,std::uint64_t(children)*80);for(unsigned i=0;i<children;++i)visit(a+80*i);--depth;};
 const auto scenes=word(image.root_offset+152),sc=word(image.root_offset+156);if(scenes!=1)throw std::runtime_error("Particle force visual scene domain");span(sc,16);auto nodes=word(sc+8),a=word(sc+12);if(nodes>4096)throw std::runtime_error("Particle force root count");span(a,std::uint64_t(nodes)*80);for(unsigned i=0;i<nodes;++i)visit(a+80*i);
 if(result.size()!=count)throw std::runtime_error("Required declared particle force scene instance");
 out=std::move(result);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool particle_gravity_bindings_v1(const resources::BresView& image,std::uint32_t instance,const scene::Scene& scene,const std::vector<ParticleGravitySceneV1>& forces,std::vector<std::uint32_t>& out,std::string& error){
 error.clear();try{
 auto word=[&](std::uint64_t p){if(p>image.size||4>image.size-p)throw std::runtime_error("Particle force binding range");std::uint32_t v;std::memcpy(&v,image.bytes+p,4);return v;};
 auto count=word(std::uint64_t(instance)+0x18),p=word(std::uint64_t(instance)+0x1c);if(count>4096)throw std::runtime_error("Particle force binding count");std::vector<std::uint32_t> result;
 for(unsigned i=0;i<count;++i){auto text=word(std::uint64_t(p)+4*i);if(!text||text>=image.size)throw std::runtime_error("Particle force binding string");auto b=reinterpret_cast<const char*>(image.bytes+text);auto end=static_cast<const char*>(std::memchr(b,0,image.size-text));if(!end||b==end||*b!='#')throw std::runtime_error("Particle force binding URI");std::string name(b+1,end);bool found=false;for(unsigned j=0;j<forces.size();++j)if(forces[j].source_node_name==name){result.push_back(j);found=true;break;}if(!found)for(const auto& node:scene.graph)if(node.id==name)throw std::runtime_error("Required non-gravity force node receiver");}
 out=std::move(result);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
