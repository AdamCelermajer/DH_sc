#include "character_world_npc_bounds_v1.hpp"
#include <algorithm>
#include <limits>
#include <cmath>
namespace dh2::character {
bool character_npc_visual_bounds_v1(const resources::BresView& view,
 const scene::Scene& pose,const float* root,const float* position,
 std::int32_t collision_scale,std::uint8_t previous_flat,
 physical::CharacterOwnerBounds& out,std::string& error){
 error.clear();if(!root||!position||pose.graph.empty()){error="Required actual CPU pose/root/position bounds producers";return false;}
 for(unsigned i=0;i<16;++i)if(!std::isfinite(root[i])){error="Actual visual root matrix invalid";return false;}
 physical::DecorSceneMarker marker{};if(!physical::decor_scene_marker(view,pose,marker,error))return false;
 physical::DecorMeshBoxInput box{};std::copy_n(root,16,box.node_matrix);
 if(marker.found){std::copy_n(marker.bounds,6,box.bounds);std::copy_n(marker.parent_scale,3,box.parent_scale);}
 else{
  std::vector<physical::CharacterMeshEntry> entries;if(!physical::character_scene_entries(view,pose,entries,error))return false;
  bool skinned=false;for(const auto& e:entries)skinned|=e.skinned!=0;
  for(unsigned k=0;k<3;++k){box.bounds[k]=std::numeric_limits<float>::max();box.bounds[k+3]=-std::numeric_limits<float>::max();box.parent_scale[k]=1;}
  for(const auto& e:entries){if(bool(e.skinned)!=skinned)continue;for(unsigned k=0;k<3;++k){const float low=e.bounds[k]*e.parent_scale[k],high=e.bounds[k+3]*e.parent_scale[k];if(box.bounds[k]>low)box.bounds[k]=low;if(box.bounds[k+3]<high)box.bounds[k+3]=high;}}
  if(entries.empty())std::fill_n(box.bounds,6,0.f);
 }
 physical::CharacterOwnerBoundsInput input{};
 if(dh2_decor_marker_mesh_box(input.mesh_box,&box)){error="Actual source CalcMeshBox arithmetic failed";return false;}
 std::copy_n(position,3,input.position);input.collision_scale=collision_scale;input.already_scaled=marker.found;input.previous_flat=previous_flat;
 physical::CharacterOwnerBounds result{};if(dh2_character_owner_bounds(&result,&input)){error="Actual source Character owner bounds arithmetic failed";return false;}
 out=result;return true;
}
}
