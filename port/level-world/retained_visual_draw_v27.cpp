#include "retained_visual_draw_v27.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
namespace dh2::world {
bool RetainedVisualDrawV27::bind(std::shared_ptr<RetainedGameObjectVisualV1> actual,std::string& e){
 if(visual_){e="Retained visual GPU geometry already bound";return false;}
 if(!actual||!actual->ready()||!actual->root_identity()){e="Required initialized actual retained visual for drawing";return false;}
 std::vector<RetainedVisualPartV27> candidate;
 const auto& bres=actual->bres();const auto& scene=actual->scene();
 for(std::size_t n=0;n<scene.instances.size();++n){
  if(!actual->mesh_attached_v76(static_cast<unsigned>(n)))continue;
  const auto& instance=scene.instances[n];assets::Mesh mesh{};
  if(dh2_mesh_open(&mesh,&bres,instance.geometry)!=assets::Error::ok||mesh.primitives!=instance.materials.size()||mesh.vertices>65536){e="Retained visual mesh/material/index domain rejected";return false;}
  for(std::uint32_t p=0;p<mesh.primitives;++p){
   assets::Primitive primitive{};
   if(dh2_mesh_primitive(&mesh,p,&primitive)!=assets::Error::ok||primitive.collada_type||primitive.index_count%3||primitive.index_count>std::uint32_t(std::numeric_limits<std::int32_t>::max())){e="Retained visual triangle primitive rejected";return false;}
   if(instance.materials[p]>=scene.materials.size()||scene.materials[instance.materials[p]].id!=primitive.material){e="Actual retained visual material binding differs";return false;}
   assets::Attribute positions{},uv{},colors{};
   if(dh2_mesh_attribute(&mesh,primitive.attributes[0],&positions)!=assets::Error::ok||positions.components<3||positions.components>4){e="Actual retained visual positions unavailable";return false;}
   const bool has_uv=dh2_mesh_attribute(&mesh,primitive.attributes[4],&uv)==assets::Error::ok&&uv.components>=2&&uv.components<=4;
   const bool has_color=dh2_mesh_attribute(&mesh,primitive.attributes[2],&colors)==assets::Error::ok&&colors.components<=4;
   RetainedVisualPartV27 part;part.instance=std::uint32_t(n);part.material=instance.materials[p];part.skinned=instance.controller>=0;
   std::shared_ptr<RetainedMeshNodeV91> native_mesh;
   if(!actual->borrow_mesh_source_v111(static_cast<unsigned>(n),native_mesh,e))return false;
   part.source_mesh_v111=native_mesh;part.source_primitive_v111=p;
   part.vertices.resize(mesh.vertices);part.indices.resize(primitive.index_count);
   for(std::uint32_t v=0;v<mesh.vertices;++v){
    float values[4]{};if(!dh2_attribute_read(&positions,v,values)){e="Actual retained visual position stream read failed";return false;}
    std::copy_n(values,3,part.vertices[v].position);
    if(has_uv){if(!dh2_attribute_read(&uv,v,values)){e="Actual retained visual UV stream read failed";return false;}std::copy_n(values,2,part.vertices[v].uv);}
    if(has_color){if(!dh2_attribute_read(&colors,v,values)){e="Actual retained visual color stream read failed";return false;}
     for(unsigned c=0;c<colors.components;++c)part.vertices[v].color[c]=values[c]/(colors.type==1?255.f:1.f);}
   }
   for(std::uint32_t i=0;i<primitive.index_count;++i){std::uint32_t index{};
    if(!dh2_index_read(&primitive,i,&index)||index>=mesh.vertices||index>65535){e="Actual retained visual index stream rejected";return false;}part.indices[i]=std::uint16_t(index);}
   candidate.push_back(std::move(part));
  }
 }
 visual_=std::move(actual);root_=visual_->root_identity();parts_=std::move(candidate);
 if(!refresh(e)){parts_.clear();visual_.reset();root_=0;return false;}
 e.clear();return true;
}
bool RetainedVisualDrawV27::refresh(std::string& e){
 if(!visual_||!visual_->ready()||visual_->root_identity()!=root_){e="Retained visual GPU receiver was released or replaced";return false;}
 const auto& scene=visual_->scene();const auto& flags=visual_->node_flags();
 for(auto& part:parts_){
  if(part.instance>=scene.instances.size()||part.material>=scene.materials.size()){e="Retained visual draw graph topology changed";return false;}
  const auto& instance=scene.instances[part.instance];
  if(instance.node_index>=flags.size()){e="Actual retained visual visibility node unavailable";return false;}
  part.visible=visual_->mesh_visible_v76(part.instance);
  part.matrix=instance.world;
  if(part.skinned){
   const RetainedGameObjectVisualV1::SkinnedMesh* skin{};
   for(const auto& candidate:visual_->skinned_meshes())if(candidate.instance==part.instance){if(skin){e="Duplicate SAME retained visual skin receiver";return false;}skin=&candidate;}
   if(!skin||skin->positions.size()!=part.vertices.size()){e="Required SAME retained visual skin/vertex backing";return false;}
   for(std::size_t i=0;i<part.vertices.size();++i)std::copy_n(skin->positions[i].begin(),3,part.vertices[i].position);
   part.matrix={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}; // positions already in source world space
  }
  for(float value:part.matrix)if(!std::isfinite(value)){e="Nonfinite actual retained visual draw matrix";return false;}
  for(const auto& vertex:part.vertices)for(float value:vertex.position)if(!std::isfinite(value)){e="Nonfinite actual retained visual draw position";return false;}
 }
 e.clear();return true;
}
}
