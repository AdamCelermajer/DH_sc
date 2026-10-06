#include "render_eligibility_v40.hpp"
#include <scene.hpp>
#include <limits>
namespace dh2::loader {
bool source_node_registration_gate_v40(std::uint32_t flags,bool culled)noexcept{return (flags&1u)!=0&&!culled;}
bool source_rigid_mesh_render_gate_v40(std::uint8_t enabled)noexcept{return enabled!=0;}


SourceMeshPassProjectionV40 source_mesh_pass_projection_v40(std::uint32_t mode,
 std::uint32_t technique_flags,std::uint32_t node_flags)noexcept{
 SourceMeshPassProjectionV40 result;
 if(mode==4||mode==16){result.primary_pass=(technique_flags&0x10000u)?8u:4u;if(node_flags&0x800u)result.secondary_pass=7;}
 else if(mode==5)result.prepare_only=true;
 return result;
}
bool deliver_source_node_registration_v40(const SourceNodeRegistrationBorrowV40& node,
 const SourceNodeRegistrationServicesV40& services,SourceNodeRegistrationResultV40& out,std::string& e){
 if(!node.node_owner||!node.identity||!node.flags11c){e="Required SAME retained node flags/identity/lease";return false;}
 SourceNodeRegistrationResultV40 result;result.visible=(*node.flags11c&1u)!=0;
 if(result.visible){
  if(!services.provider_owner||!services.is_culled){e="Required actual scene-camera isCulled producer";return false;}
  if(!services.is_culled(node.identity,result.culled,e))return false;
  if(!result.culled){
   if(!services.on_register){e="Required actual native node onRegister/material pass producer";return false;}
   if(!services.on_register(node.identity,result.visit_children,e))return false;
   result.on_register_delivered=true;
  }
 }
 out=result;e.clear();return true;
}
bool find_source_visual_helper_v40(const scene::Scene& scene,std::uint32_t& out,std::string& e){
 for(std::size_t i=0;i<scene.graph.size();++i){const auto& node=scene.graph[i];
  if(node.parent< -1||node.parent>=static_cast<std::int32_t>(i)){e="Required actual source depth-first Scene graph";return false;}
 }
 std::uint32_t found=std::numeric_limits<std::uint32_t>::max();
 for(std::size_t i=0;i<scene.graph.size();++i)if(scene.graph[i].name.compare(0,8,"_colbox_")==0){found=static_cast<std::uint32_t>(i);break;}
 out=found;e.clear();return true;
}
}
