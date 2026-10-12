#include "visual_mesh_box_fallback_v2.hpp"
#include <algorithm>
#include <limits>
namespace dh2::world {
bool visual_mesh_box_fallback_v2(const resources::BresView& b,const scene::Scene& s,const float* p,const float* q,const float* scale,float* out,std::string& e){
 if(!p||!q||!scale||!out){e="Required actual CalcMeshBox root transform";return false;}
 bool static_mesh=false;for(const auto& i:s.instances)if(i.controller<0)static_mesh=true;
 physical::DecorMeshBoxInput input{};bool any=false;
 std::fill_n(input.bounds,3,std::numeric_limits<float>::max());std::fill_n(input.bounds+3,3,-std::numeric_limits<float>::max());
 for(const auto& i:s.instances){if((i.controller<0)!=static_mesh)continue;if(i.node_index>=s.graph.size()){e="Actual CalcMeshBox node index unavailable";return false;}
  const auto& node=s.graph[i.node_index];const float* parent_scale=scale;
  if(node.parent>=0){if(static_cast<std::size_t>(node.parent)>=s.graph.size()){e="Actual CalcMeshBox parent unavailable";return false;}parent_scale=s.graph[static_cast<std::size_t>(node.parent)].scale;}
  assets::Mesh mesh{};if(dh2_mesh_open(&mesh,&b,static_cast<std::int32_t>(i.geometry))!=assets::Error::ok){e="Actual CalcMeshBox geometry unavailable";return false;}
  for(unsigned k=0;k<3;++k){const float minimum=mesh.minimum[k]*parent_scale[k],maximum=mesh.maximum[k]*parent_scale[k];if(minimum<input.bounds[k])input.bounds[k]=minimum;if(maximum>input.bounds[k+3])input.bounds[k+3]=maximum;}any=true;
 }
 if(!any){std::fill_n(out,6,0.f);return true;}
 std::fill_n(input.parent_scale,3,1.f);dh2_node_matrix(input.node_matrix,p,q,scale);
 if(dh2_decor_marker_mesh_box(out,&input)){e="Actual CalcMeshBox root transformation rejected";return false;}return true;
}
}
