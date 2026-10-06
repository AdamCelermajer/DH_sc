#include "module_static_scene_bounds_v3.hpp"
#include "floor_source.hpp"
#include <algorithm>
#include <functional>
namespace dh2::world {
namespace {
bool transformed(const std::array<float,6>& input,const std::array<float,16>& matrix,std::array<float,6>& output){
 octree::Box in{},out{};std::copy_n(input.data(),3,in.minimum);std::copy_n(input.data()+3,3,in.maximum);selector::Matrix m{};std::copy_n(matrix.data(),16,m.values);
 if(dh2_floor_transform_bounds(&out,&in,&m))return false;std::copy_n(out.minimum,3,output.data());std::copy_n(out.maximum,3,output.data()+3);return true;
}
void merge(std::array<float,6>& a,const std::array<float,6>& b){for(unsigned k=0;k<3;++k){a[k]=std::min(a[k],b[k]);a[k+3]=std::max(a[k+3],b[k+3]);}}
}
bool module_static_scene_bounds_v3(const resources::BresView& view,ModuleStaticSceneV2& owner,std::array<float,6>& output,std::string& error){
 error.clear();const auto& scene=owner.selected().scene;
 std::function<bool(int,std::array<float,6>&,bool&)> compute=[&](int parent,std::array<float,6>& stored,bool& any){
  any=false;
  auto child=[&](const std::array<float,6>& world){if(!any){stored=world;any=true;}else merge(stored,world);};
  if(parent>=0)for(unsigned i=0;i<scene.instances.size();++i){const auto& instance=scene.instances[i];if(static_cast<int>(instance.node_index)!=parent||!owner.mesh_attached(i))continue;
   if(instance.controller>=0){error="Required actual skinned Module bbox producer";return false;}assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&view,instance.geometry)!=assets::Error::ok){error="Actual Module bbox geometry rejected";return false;}
   std::array<float,6> box{};std::copy_n(mesh.minimum,3,box.data());std::copy_n(mesh.maximum,3,box.data()+3);
   std::array<float,6> world{};if(!transformed(box,instance.world,world)){error="Actual Module cached mesh bbox transform rejected";return false;}child(world);
  }
  for(unsigned i=0;i<scene.graph.size();++i)if(scene.graph[i].parent==parent&&owner.node_attached(i)){
   std::array<float,6> box{-1,-1,-1,1,1,1};bool recognized=false;
   if(!compute(static_cast<int>(i),box,recognized))return false;
   if(recognized)child(box);
  }
  // Original no recognized child branch preserves ctor or previous bbox.
  return true;
 };
 bool recognized=false;if(!compute(-1,owner.source_root_box(),recognized))return false;
 auto& box=owner.source_root_box();for(unsigned k=0;k<6;++k)box[k]-=owner.root_position()[k%3];
 return module_static_scene_query_bounds_v3(owner,output,error);
}
bool module_static_scene_query_bounds_v3(ModuleStaticSceneV2& owner,std::array<float,6>& output,std::string& error){
 error.clear();auto& flags=owner.source_root_flags();auto& cached=owner.source_root_absolute_box();
 if(flags&0x100u){if(!transformed(owner.source_root_box(),owner.root_cached(),cached)){error="Actual Module root transformed bbox rejected";return false;}flags&=~0x100u;}
 output=cached;return true;
}
}
