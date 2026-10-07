#include "module_floor_append_v2.hpp"
#include "user_properties_v2.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace dh2::world {
namespace {void require(bool v,const char* e){if(!v)throw std::runtime_error(e);}}
// Mesh/selector/octree construction is the checked floors::append successor;
// the additional source property prefix is recovered from _LoadNavMesh520b40.
bool module_floor_load_record_pose_v2(const resources::BresView& view,const scene::Scene& scene,const scene::Instance& instance,unsigned room,const float* rotation,const float* scale,floors::Record& storage,std::string& error){
 error.clear();try{
  require(instance.node_index<scene.graph.size(),"Invalid floor node");const auto& node=scene.graph[instance.node_index];
  require(node.parent>=0&&unsigned(node.parent)<scene.graph.size(),"Floor has no room parent");
  auto* record=&storage;record->name=node.name;record->room=room;record->geometry=instance.geometry;record->flags={0,1};
  std::map<std::string,std::string> properties;
  require(user_properties_v2(node.user_properties.c_str(),properties,error),"Source floor UserProperties rejected");
  const auto found=properties.find("floortypes");
  if(found!=properties.end())require(dh2_floor_source_flags(&record->flags,found->second.c_str(),found->second.size())==0,"Source authored floor flags rejected");
  // constructNode creates a CSceneNode with the authored TRS, then a
  // BaseMeshSceneNode child initially with identity local TRS. Static baking
  // can subsequently rewrite its local quaternion; caller borrows that field.
  // _LoadNavMesh reads
  // properties/name from that CSceneNode, moves the child to its cached
  // absolute position, and copies the child's local rotation/scale.
  require(rotation&&scale,"Floor clone requires actual mesh local rotation/scale producer");
  const float position[]{instance.world[12],instance.world[13],instance.world[14]};
  require(dh2_floor_clone_matrix(&record->clone,position,rotation,scale)==0,"Floor clone transform rejected");
  assets::Mesh mesh{};require(dh2_mesh_open(&mesh,&view,instance.geometry)==assets::Error::ok,"Floor mesh rejected");require(mesh.primitives<=10000,"Too many floor mesh buffers");
  std::vector<floor_source::Part> parts;unsigned count=0;
  for(unsigned i=0;i<mesh.primitives;++i){assets::Primitive primitive{};require(dh2_mesh_primitive(&mesh,i,&primitive)==assets::Error::ok,"Floor primitive rejected");floor_source::Part part{};part.primitive_type=primitive.engine_type;
   if(part.primitive_type!=6){parts.push_back(part);continue;}
   require(dh2_mesh_attribute(&mesh,primitive.attributes[0],&part.position)==assets::Error::ok,"Floor position rejected");
   require(primitive.index_width==2,"Original floor selector requires 16-bit indices");part.indices=primitive.indices;part.draw_count=primitive.index_count;
   if(part.position.components>=2&&part.position.components<=4){require(part.draw_count%3==0&&part.draw_count/3<=100000-count,"Invalid floor triangle budget");count+=part.draw_count/3;}
   parts.push_back(part);
  }
  require(count>0,"Floor has no supported triangles");record->triangles.resize(count);unsigned written=0;
  require(dh2_floor_mesh_triangles(record->triangles.data(),count,&written,parts.data(),parts.size(),&record->clone,1)==0&&written==count,"Floor mesh extraction rejected");
  std::copy(mesh.minimum,mesh.minimum+3,record->local.minimum);std::copy(mesh.maximum,mesh.maximum+3,record->local.maximum);
  require(dh2_floor_transform_bounds(&record->world,&record->local,&record->clone)==0&&dh2_floor_source_bounds(&record->bounds,&record->world)==0,"Floor bounds rejected");
  record->octants.resize(count*8+1);record->indices.resize(count*8+1);record->scratch.resize(count);record->selected.resize(count);record->selected_ids.resize(count);
  record->tree={record->octants.data(),record->indices.data(),record->scratch.data(),record->triangles.data(),count,0,0,unsigned(record->octants.size()),unsigned(record->indices.size()),count,15,0,0};
  require(dh2_octree_build(&record->tree,record->triangles.data(),count,15)==0,"Floor octree storage exhausted");record->workspace={record->selected_ids.data(),record->selected.data(),count,0};
  return true;
 }catch(const std::exception& e){error=e.what();return false;}
}
bool module_floor_load_record_v2(const resources::BresView& view,const scene::Scene& scene,
 const scene::Instance& instance,unsigned room,floors::Record& storage,std::string& error){
 const float rotation[]{0,0,0,1},scale[]{1,1,1};
 return module_floor_load_record_pose_v2(view,scene,instance,room,rotation,scale,storage,error);
}
bool module_floor_append_v2(const resources::BresView& view,const scene::Scene& scene,
 const scene::Instance& instance,unsigned room,floors::World& out,std::string& error){
 if(out.records.size()>=512){error="Too many native floors";return false;}
 auto record=std::make_unique<floors::Record>();
 if(!module_floor_load_record_v2(view,scene,instance,room,*record,error))return false;
 out.records.push_back(std::move(record));return true;
}
bool module_floor_append_pose_v2(const resources::BresView& view,const scene::Scene& scene,
 const scene::Instance& instance,unsigned room,const float* rotation,const float* scale,
 floors::World& out,std::string& error){
 if(out.records.size()>=512){error="Too many native floors";return false;}
 auto record=std::make_unique<floors::Record>();
 if(!module_floor_load_record_pose_v2(view,scene,instance,room,rotation,scale,*record,error))return false;
 out.records.push_back(std::move(record));return true;
}
}
