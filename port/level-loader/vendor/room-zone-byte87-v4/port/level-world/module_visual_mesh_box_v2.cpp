#include "module_visual_mesh_box_v2.hpp"
#include <algorithm>
#include <cfloat>
#include <cmath>
namespace dh2::world {
bool module_visual_mesh_box_v2(const std::vector<ModuleVisualMeshV2>& meshes,
 const std::array<float,16>& root,std::array<float,6>& out,std::string& error){
 error.clear();
 if(meshes.empty()){out.fill(0);return true;}
 const bool animated=std::any_of(meshes.begin(),meshes.end(),[](const auto& m){return m.animated;});
 physical::DecorMeshBoxInput input{};
 for(unsigned k=0;k<3;++k){input.bounds[k]=FLT_MAX;input.bounds[k+3]=-FLT_MAX;input.parent_scale[k]=1;}
 for(const auto& mesh:meshes){
  if(mesh.animated!=animated)continue;
  for(unsigned k=0;k<3;++k){
   if(!std::isfinite(mesh.parent_scale[k])||!std::isfinite(mesh.local_box[k])||!std::isfinite(mesh.local_box[k+3])){
    error="Module CalcMeshBox requires finite actual mesh bounds and parent scale";return false;
   }
   const float low=mesh.local_box[k]*mesh.parent_scale[k];
   const float high=mesh.local_box[k+3]*mesh.parent_scale[k];
   // Original does not reorder after negative parent scale.
   if(low<input.bounds[k])input.bounds[k]=low;
   if(high>input.bounds[k+3])input.bounds[k+3]=high;
  }
 }
 for(unsigned k=0;k<16;++k){
  if(!std::isfinite(root[k])){error="Module CalcMeshBox requires actual finite root relative matrix";return false;}
  input.node_matrix[k]=root[k];
 }
 return dh2_decor_marker_mesh_box(out.data(),&input)==0;
}
}
