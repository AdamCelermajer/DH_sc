#include "particle_force_scene_v2.hpp"
#include <cstring>
#include <functional>
#include <map>
#include <stdexcept>
namespace dh2::animation {
const std::array<float,16>* ParticleForceSceneV2::matrix()const noexcept{return scene&&node_index<scene->graph.size()?&scene->graph[node_index].world:nullptr;}
namespace {
struct Reader {
 const resources::BresView& image;
 const std::uint8_t* span(std::uint64_t p,std::uint64_t n)const{if(p>image.size||n>image.size-p)throw std::runtime_error("Particle force BRES range");return image.bytes+p;}
 std::uint32_t word(std::uint64_t p)const{std::uint32_t v;std::memcpy(&v,span(p,4),4);return v;}
 std::string text(std::uint32_t p)const{const auto* b=reinterpret_cast<const char*>(span(p,1));const auto* end=static_cast<const char*>(std::memchr(b,0,image.size-p));if(!end)throw std::runtime_error("Particle force string");return {b,end};}
};
}
bool decode_particle_force_scene_v2(const resources::BresView& image,const scene::Scene& scene,std::vector<ParticleForceSceneV2>& out,std::string& error){
 error.clear();try{
 Reader r{image};const auto count=r.word(image.root_offset+0x88),table=r.word(image.root_offset+0x8c);
 if(count>4096)throw std::runtime_error("Particle force count");r.span(table,std::uint64_t(count)*16);
 std::map<std::string,std::uint32_t> declarations;
 for(unsigned i=0;i<count;++i){const auto at=table+16*i;declarations.try_emplace(r.text(r.word(at)),at);}
 std::vector<ParticleForceSceneV2> result;unsigned depth=0;
 std::function<void(std::uint32_t)> visit=[&](std::uint32_t node){
  if(++depth>64)throw std::runtime_error("Particle force depth");r.span(node,80);
  const auto count=r.word(node+64),items=r.word(node+68);if(count>4096)throw std::runtime_error("Particle force instance count");r.span(items,std::uint64_t(count)*8);
  for(unsigned i=0;i<count;++i){const auto item=items+8*i;if(r.word(item)!=12)continue;
   const auto instance=r.word(item+4);r.span(instance,8);auto uri=r.text(r.word(instance+4));
   if(uri.empty()||uri[0]!='#')throw std::runtime_error("Particle force URI");const auto named=declarations.find(uri.substr(1));
   if(named==declarations.end())throw std::runtime_error("Particle force named declaration");
   const auto source_id=r.text(r.word(node));unsigned found=0,index=0;
   for(unsigned j=0;j<scene.graph.size();++j)if(scene.graph[j].id==source_id){++found;index=j;}
   if(found!=1)throw std::runtime_error("Particle force SAME scene node");
   ParticleForceSceneV2 force;force.source_name=named->first;force.source_node_name=source_id;force.node_index=index;force.scene=&scene;
   force.type=r.word(named->second+8);const auto data=r.word(named->second+12);
   if(force.type==0)std::memcpy(&force.gravity,r.span(data,12),12);
   else if(force.type==2)std::memcpy(&force.deflector,r.span(data,28),28);
   else throw std::runtime_error("Required unsupported particle force receiver type "+std::to_string(force.type));
   result.push_back(std::move(force));
  }
  const auto count_children=r.word(node+56),children=r.word(node+60);if(count_children>4096)throw std::runtime_error("Particle force children");r.span(children,std::uint64_t(count_children)*80);
  for(unsigned i=0;i<count_children;++i)visit(children+80*i);--depth;
 };
 const auto scenes=r.word(image.root_offset+152),table_scenes=r.word(image.root_offset+156);if(scenes!=1)throw std::runtime_error("Particle force visual scene domain");r.span(table_scenes,16);
 const auto nodes=r.word(table_scenes+8),table_nodes=r.word(table_scenes+12);if(nodes>4096)throw std::runtime_error("Particle force roots");r.span(table_nodes,std::uint64_t(nodes)*80);
 for(unsigned i=0;i<nodes;++i)visit(table_nodes+80*i);
 out=std::move(result);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool particle_force_bindings_v2(const resources::BresView& image,std::uint32_t instance,const scene::Scene& scene,
 const std::vector<ParticleForceSceneV2>& forces,std::vector<std::uint32_t>& out,std::string& error){
 error.clear();try{
 Reader r{image};const auto count=r.word(std::uint64_t(instance)+0x18),table=r.word(std::uint64_t(instance)+0x1c);if(count>4096)throw std::runtime_error("Particle force binding count");r.span(table,std::uint64_t(count)*4);
 std::vector<std::uint32_t> result;
 for(unsigned i=0;i<count;++i){auto uri=r.text(r.word(table+4*i));if(uri.empty()||uri[0]!='#')throw std::runtime_error("Particle force binding URI");const auto name=uri.substr(1);
  bool found=false;for(unsigned j=0;j<forces.size();++j)if(forces[j].source_node_name==name){result.push_back(j);found=true;break;}
  if(!found)for(const auto& node:scene.graph)if(node.id==name)throw std::runtime_error("Required source bound force node receiver");
 }
 out=std::move(result);return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
}
