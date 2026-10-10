#include "native_batch_compiler_v111.hpp"
#include "retained_visual_child_v91.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <set>
namespace dh2::world {namespace {
bool needed(std::string& e,const char* what){if(e.empty())e=what;return false;}
bool finite(const float* values,std::size_t count){for(std::size_t i=0;i<count;++i)if(!std::isfinite(values[i]))return false;return true;}
std::int32_t source_f2iz(float f){
 //ARM __aeabi_f2iz: truncate finite values; its saturating exceptional branch
 //avoids host C++ float-to-integer UB. NaN becomes0; ±overflow saturates.
 if(std::isnan(f))return 0;if(f>=2147483648.f)return INT32_MAX;if(f<=-2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(f);
}
void bounds(NativeBatchPartV111& part){
 part.bounds={INFINITY,INFINITY,INFINITY,-INFINITY,-INFINITY,-INFINITY};
 for(const auto& vertex:part.vertices)for(unsigned a=0;a<3;++a){part.bounds[a]=std::min(part.bounds[a],vertex.position[a]);part.bounds[a+3]=std::max(part.bounds[a+3],vertex.position[a]);}
}
}
bool NativeBatchCompiledV111::quantize(bool position,bool normals,std::string& e){
 if(released_||quantized_||uploaded_)return needed(e,"Batch quantization outside original pre-upload lifetime");
 if(position)return needed(e,"Position quantization requires original selected position buffer protocol");
 for(auto& part:parts){
  auto& uv=part.attributes[4];
  if(uv.source_type==6&&!uv.values.empty()){
   if(uv.components!=2)return needed(e,"Original quantized UV buffer requires vector2d source");
   std::array<float,2> lo{{INFINITY,INFINITY}},hi{{-INFINITY,-INFINITY}};
   for(const auto& value:uv.values)for(unsigned a=0;a<2;++a){lo[a]=std::min(lo[a],value[a]);hi[a]=std::max(hi[a],value[a]);}
   for(unsigned a=0;a<2;++a){volatile float extent=hi[a]-lo[a];part.uv_scale[a]=extent/65535.f;volatile float sum=hi[a]+lo[a];part.uv_offset[a]=sum*0.5f;}
   if(!resources::reserve_cpu_vector_v41(part.quantized_uv,part.uv_charge,budget,resources::ResourceScopeV37::world,uv.values.size(),e))return false;
   part.quantized_uv.resize(uv.values.size());
   for(std::size_t i=0;i<uv.values.size();++i)for(unsigned a=0;a<2;++a){volatile float shifted=uv.values[i][a]-part.uv_offset[a];const auto bits=static_cast<std::uint16_t>(source_f2iz(shifted/part.uv_scale[a]));std::int16_t word;std::memcpy(&word,&bits,2);part.quantized_uv[i][a]=word;
    volatile float scaled=static_cast<float>(word)*part.uv_scale[a];part.vertices[i].uv[a]=scaled+part.uv_offset[a];}
  }
  //Other mapped texture-coordinate streams keep their own actual component
  //buffers and scale/offset; primary UV4 alone is routed by the current shader.
  for(unsigned slot=5;slot<18;++slot){auto& stream=part.attributes[slot];
   if(stream.source_type!=6||stream.values.empty()||stream.components!=2)continue;
   std::array<float,2> lo{{INFINITY,INFINITY}},hi{{-INFINITY,-INFINITY}};
   for(const auto& value:stream.values)for(unsigned a=0;a<2;++a){lo[a]=std::min(lo[a],value[a]);hi[a]=std::max(hi[a],value[a]);}
   for(unsigned a=0;a<2;++a){volatile float extent=hi[a]-lo[a];stream.quantized_scale[a]=extent/65535.f;volatile float sum=hi[a]+lo[a];stream.quantized_offset[a]=sum*0.5f;}
   if(!resources::reserve_cpu_vector_v41(stream.quantized_components,stream.quantized_charge,budget,resources::ResourceScopeV37::world,stream.values.size(),e))return false;
   stream.quantized_components.resize(stream.values.size());
   for(std::size_t i=0;i<stream.values.size();++i)for(unsigned a=0;a<2;++a){volatile float shifted=stream.values[i][a]-stream.quantized_offset[a];const auto bits=static_cast<std::uint16_t>(source_f2iz(shifted/stream.quantized_scale[a]));std::memcpy(&stream.quantized_components[i][a],&bits,2);}
  }
  auto& normal=part.attributes[1];
  if(normals&&normal.source_type==6&&!normal.values.empty()){
   if(normal.components!=3)return needed(e,"Original quantized normal buffer requires vector3d source");
   if(!resources::reserve_cpu_vector_v41(part.quantized_normal,part.normal_charge,budget,resources::ResourceScopeV37::world,normal.values.size(),e))return false;
   part.quantized_normal.resize(normal.values.size());
   for(std::size_t i=0;i<normal.values.size();++i)for(unsigned a=0;a<3;++a){volatile float scaled=normal.values[i][a]*32767.f;const auto bits=static_cast<std::uint16_t>(source_f2iz(scaled));std::memcpy(&part.quantized_normal[i][a],&bits,2);}
  }
 }
 quantized_=true;e.clear();return true;
}
bool NativeBatchCompiledV111::flush(const std::shared_ptr<NativeBatchMeshV110>& mesh,bool vertices,bool indices,bool textures,std::string& e){
 if(released_||!mesh||mesh->compiled_v111().get()!=this||!vertices||indices||textures||uploaded_||!gpu.owner||!gpu.upload)return needed(e,"Required SAME original FlushMeshBuffers(true,false,false) upload");
 if(!gpu.upload(mesh,e))return false;uploaded_=true;e.clear();return true;
}
bool NativeBatchCompiledV111::release(std::uintptr_t mesh,std::string& e){
 if(released_){e.clear();return true;}
 //GPU can exist after a failed upload prefix; always dispatch its actual
 //resource release, then free the SAME CPU capacities, indices and materials.
 if(gpu.owner&&(!gpu.release||!gpu.release(mesh,e)))return false;
 for(auto& part:parts)if(part.source_buffer_v112){auto& refs=part.source_buffer_v112->batch_backlinks_v112;auto found=refs.find(part.source_primitive_v111);if(found!=refs.end()&&found->second.batch18==mesh)refs.erase(found);}
 std::vector<NativeBatchPartV111>{}.swap(parts);animations_v112.clear();retiring_animations_v113.clear();visible={};gpu={};released_=true;e.clear();return true;
}
bool NativeBatchCompiledV111::update_segment_content_v112(std::size_t index,std::string& e){
 if(released_||index>=parts.size())return needed(e,"Actual updateSegmentContent receiver/range");auto& part=parts[index];
 if(!part.dynamic_v112){e.clear();return true;}
 if(part.native_retired_v113){e.clear();return true;}
 if(!part.source_buffer_v112||!part.source_fields_v112||part.source_ids_v112.size()!=part.vertices.size())return needed(e,"SAME retained dynamic buffer/vertex mapping");
 const auto backlink=part.source_buffer_v112->batch_backlinks_v112.find(part.source_primitive_v111);
 if(backlink==part.source_buffer_v112->batch_backlinks_v112.end()||backlink->second.batch18!=mesh_identity_v112||backlink->second.owner.lock().get()!=this)return needed(e,"Replaced actual CMeshBuffer30/backlink18 ownership");
 const auto& source=part.skinned_v112?part.source_buffer_v112->positions:part.source_buffer_v112->source_positions;
 const auto& matrix=part.source_fields_v112->world;
 for(std::size_t i=0;i<part.vertices.size();++i){const auto original=part.source_ids_v112[i];if(original>=source.size())return needed(e,"Changed actual dynamic source vertex domain");
  const auto& value=source[original];auto& vertex=part.vertices[i];
  for(unsigned axis=0;axis<3;++axis){const auto p=part.skinned_v112?value[axis]:matrix[axis]*value[0]+matrix[4+axis]*value[1]+matrix[8+axis]*value[2]+matrix[12+axis];if(!std::isfinite(p))return needed(e,"Nonfinite actual updated dynamic buffer");vertex.position[axis]=p;part.attributes[0].values[i][axis]=p;}
  auto& normal=part.attributes[1];if(!normal.values.empty()){
   if(normal.components!=3)return needed(e,"Actual dynamic normal stream width");
   if(part.skinned_v112){if(original>=part.source_buffer_v112->normals_v113.size())return needed(e,"Actual SAME software-skin normal producer");std::copy_n(part.source_buffer_v112->normals_v113[original].data(),3,normal.values[i].data());}
   else{if(i>=part.rest_normals_v113.size())return needed(e,"Actual dynamic source normal mapping");const auto& n=part.rest_normals_v113[i];for(unsigned a=0;a<3;++a)normal.values[i][a]=(matrix[a]*n[0]+matrix[4+a]*n[1])+matrix[8+a]*n[2];}
   if(!finite(normal.values[i].data(),3))return needed(e,"Nonfinite actual dynamic normal");
   if(!part.quantized_normal.empty())for(unsigned a=0;a<3;++a){const auto bits=static_cast<std::uint16_t>(source_f2iz(normal.values[i][a]*32767.f));std::memcpy(&part.quantized_normal[i][a],&bits,2);}
  }
 }
 bounds(part);e.clear();return true;
}
bool NativeBatchCompiledV111::animate_v112(std::uint32_t stamp,std::string& e){
 if(released_)return needed(e,"Retired compiled animator receiver");
 if(!validate_current_v112||!validate_current_v112(e))return false;
 for(const auto& entry:animations_v112)if(!entry.second.owner||!entry.second.phase||!entry.second.phase(stamp,e)||!validate_current_v112(e))return needed(e,"SAME transferred dynamic animation delivery");
 for(std::size_t i=0;i<parts.size();++i)if(!update_segment_content_v112(i,e)||!validate_current_v112(e))return false;
 if(!animations_v112.empty())++pose_revision_v112;e.clear();return true;
}
bool NativeBatchCompiledV111::retire_object_v113(NativeBatchMeshV110& mesh,std::uintptr_t object,std::string& e){
 if(released_||mesh.compiled_v111().get()!=this||mesh.identity()!=mesh_identity_v112||!object)return needed(e,"Actual compiled object retirement receiver");
 std::set<std::uintptr_t> animations;
 for(auto& part:parts){if(part.segment>=mesh.compiler_segments().size())return needed(e,"Actual compiled segment domain changed");if(mesh.compiler_segments()[part.segment].game_object2c!=object)continue;
  part.native_retired_v113=true;animations.insert(part.animation_identity_v113);
  if(part.source_buffer_v112){auto& refs=part.source_buffer_v112->batch_backlinks_v112;const auto found=refs.find(part.source_primitive_v111);if(found!=refs.end()&&found->second.batch18==mesh.identity()&&found->second.owner.lock().get()==this)refs.erase(found);}
  part.source_buffer_v112.reset();part.source_fields_v112.reset();
 }
 for(auto id:animations){if(!id)continue;bool still_used=false;for(const auto& part:parts)if(!part.native_retired_v113&&part.animation_identity_v113==id){still_used=true;break;}if(!still_used){auto found=animations_v112.find(id);if(found!=animations_v112.end()){retiring_animations_v113[object].push_back(std::move(found->second));animations_v112.erase(found);}}}
 e.clear();return true;
}
bool native_compile_scene_v111(const std::vector<loader::BatchNodeBorrowV96>& roots,const std::shared_ptr<NativeBatchMeshV110>& mesh,const NativeBatchCompileServicesV111& s,const loader::BatchLinkedCallbackV96& callback,std::string& e){
 if(!mesh||!mesh->live()||mesh->compiled_v111()||!s.owner||!s.budget||!s.current||!s.current(e)||!s.vertices22c||!s.indices230||!s.set_rendered||!s.get_rendered||!callback||!s.game_object_visible||!s.material_pass_v112)return needed(e,"Required actual Scene/CBatchDriver/limits/material/segment callback");
 const auto vertices=*s.vertices22c,indices=*s.indices230;
 if(vertices<=0||indices<3)return needed(e,"Original configured batch buffer cannot contain a triangle");
 const auto max_vertices=std::min<std::uint32_t>(static_cast<std::uint32_t>(vertices),65536u);
 const auto max_indices=static_cast<std::uint32_t>(indices)/3u*3u;
 auto data=std::make_shared<NativeBatchCompiledV111>();data->budget=s.budget;data->gpu=s.gpu;data->validate_current_v112=s.current;data->mesh_identity_v112=mesh->identity();
 if(!mesh->publish_compiled_v111(data,e))return false; //Retain failed compilation prefix.
 std::set<std::uintptr_t> active;
 std::function<bool(const loader::BatchNodeBorrowV96&)> visit;
 visit=[&](const loader::BatchNodeBorrowV96& node){
  if(!node.owner||!node.identity||!node.child_count||!node.child||!node.source_v111.flags11c||!s.current(e))return needed(e,"SAME selected Scene compile node/membership");
  if(!(*node.source_v111.flags11c&1u))return true; //Source hidden/nobatch nodes are not submitted.
  if(!active.insert(node.identity).second)return needed(e,"Cyclic actual compiled scene hierarchy");
  struct Guard{std::set<std::uintptr_t>& set;std::uintptr_t id;~Guard(){set.erase(id);}}guard{active,node.identity};
  const auto& source=node.source_v111;
  if(source.mesh_v111){
   auto native=source.mesh_v111;
   if(!native->fields)return needed(e,"Actual source mesh fields");
   const bool dynamic=source.dynamic_source_v111||native->fields->controller>=0;
   if(dynamic){
    if(!source.animation_identity_v112||!source.animation_v112||!native->mesh)return needed(e,"Actual source dynamic animator/buffer producer");
    if(!data->animations_v112.count(source.animation_identity_v112)){BatchAnimationBorrowV112 animation;
     if(!source.animation_v112(animation,e)||animation.identity!=source.animation_identity_v112||!animation.owner||!animation.phase)return needed(e,"SAME dynamic animation ownership handoff");
     data->animations_v112.emplace(animation.identity,std::move(animation));
    }
   }
   if(native->native_destroyed_v106||!native->source_resource_v93||!native->source_image_v93.bytes||!source.materials_v111)return needed(e,"Actual compile BRES/mesh/material owner");
   assets::Mesh input;if(dh2_mesh_open(&input,&native->source_image_v93,native->fields->geometry)!=assets::Error::ok)return needed(e,"Actual compile mesh stream");
   std::vector<scene::InstanceMaterialBindingV1> bindings;if(!source.materials_v111(bindings,e)||bindings.size()!=input.primitives)return needed(e,"Actual compile primitive material-binding count");
   if(!finite(native->fields->world.data(),16))return needed(e,"Nonfinite actual compile cached matrix");
   for(std::uint32_t p=0;p<input.primitives;++p){assets::Primitive primitive;scene::Material material;
    if(dh2_mesh_primitive(&input,p,&primitive)!=assets::Error::ok||primitive.collada_type||primitive.index_count%3||!primitive.material||!resolve_material_binding_v111(bindings,primitive.material,material,e))return needed(e,"Actual compile triangle/material binding");
    scene::EffectRenderPassV4 pass;if(!s.material_pass_v112(native->source_image_v93,material,pass,e))return false;
    auto values=std::make_shared<NativeBatchMaterialValuesV113>();if(!s.material_values_v113||!s.material_values_v113(native->source_image_v93,material,*values,e))return false;
    std::shared_ptr<NativeMaterialLightsV113> material_lights;if(!s.material_lights_v113||!s.material_lights_v113(native,p,values,material_lights,e)||!material_lights)return needed(e,"Actual material construction/light-reference owner absent");
    std::array<assets::Attribute,18> streams{};for(unsigned a=0;a<18;++a)if(primitive.attributes[a]>=0){
     if(dh2_mesh_attribute(&input,primitive.attributes[a],&streams[a])!=assets::Error::ok||!streams[a].components||streams[a].components>4)return needed(e,"Actual compile attribute descriptor/vector width");
    }
    if(!streams[0].data||streams[0].components!=3)return needed(e,"Actual compiled position stream requires recovered XYZ; homogeneous four-component transform producer absent");
    for(std::uint32_t cursor=0;cursor<primitive.index_count;){
     NativeBatchPartV111 part;part.material=material;part.source_node=node.identity;part.segment=mesh->compiler_segments().size();
     part.pass_v112=pass;
     part.material_values_v113=values;
     part.material_lights_v113=material_lights;
     part.source_mesh_v111=native;part.source_primitive_v111=p;
     part.dynamic_v112=dynamic;part.skinned_v112=native->fields->controller>=0;
     part.animation_identity_v113=dynamic?source.animation_identity_v112:0;
     if(dynamic){part.source_buffer_v112=native->mesh;part.source_fields_v112=native->fields;}
     const auto vertex_capacity=std::min<std::uint32_t>(input.vertices,max_vertices);
     const auto index_capacity=std::min<std::uint32_t>(primitive.index_count-cursor,max_indices);
     if(dynamic&&!resources::reserve_cpu_vector_v41(part.source_ids_v112,part.source_ids_charge_v112,s.budget,resources::ResourceScopeV37::world,vertex_capacity,e))return false;
     if(!resources::reserve_cpu_vector_v41(part.vertices,part.vertices_charge,s.budget,resources::ResourceScopeV37::world,vertex_capacity,e)||
        !resources::reserve_cpu_vector_v41(part.indices,part.indices_charge,s.budget,resources::ResourceScopeV37::world,index_capacity,e))return false;
     for(unsigned a=0;a<18;++a)if(streams[a].data&&!resources::reserve_cpu_vector_v41(part.attributes[a].values,part.attribute_charges[a],s.budget,resources::ResourceScopeV37::world,vertex_capacity,e))return false;
     if(dynamic&&streams[1].data&&!resources::reserve_cpu_vector_v41(part.rest_normals_v113,part.rest_normal_charge_v113,s.budget,resources::ResourceScopeV37::world,vertex_capacity,e))return false;
     std::map<std::uint32_t,std::uint16_t> remap;
     std::uint32_t last=cursor;
     while(last<primitive.index_count&&part.indices.size()+3<=max_indices){
      std::array<std::uint32_t,3> triangle;std::size_t additions{};std::set<std::uint32_t> new_ids;
      for(unsigned k=0;k<3;++k){if(!dh2_index_read(&primitive,last+k,&triangle[k])||triangle[k]>=input.vertices)return needed(e,"Actual compile source triangle index");if(!remap.count(triangle[k]))new_ids.insert(triangle[k]);}additions=new_ids.size();
      if(remap.size()+additions>max_vertices)break;
      for(auto original:triangle){auto found=remap.find(original);if(found==remap.end()){
       const auto slot=static_cast<std::uint16_t>(part.vertices.size());remap.emplace(original,slot);
       NativeBatchVertexV111 v;
       for(unsigned a=0;a<18;++a)if(streams[a].data){auto& attribute=part.attributes[a];attribute.source_type=streams[a].type;attribute.components=streams[a].components;
        std::array<float,4> value{};if(!dh2_attribute_read(&streams[a],original,value.data())||!finite(value.data(),attribute.components))return needed(e,"Nonfinite/short actual compile attribute");attribute.values.push_back(value);
        if(a==0){const auto& m=native->fields->world;for(unsigned axis=0;axis<3;++axis)v.position[axis]=m[axis]*value[0]+m[4+axis]*value[1]+m[8+axis]*value[2]+m[12+axis];}
        else if(a==1){if(attribute.components!=3)return needed(e,"Actual compiled normal vector width");const auto& m=native->fields->world;auto& transformed=attribute.values.back();for(unsigned axis=0;axis<3;++axis)transformed[axis]=(m[axis]*value[0]+m[4+axis]*value[1])+m[8+axis]*value[2];if(dynamic)part.rest_normals_v113.push_back({value[0],value[1],value[2]});}
        else if(a==4){if(attribute.components<2)return needed(e,"Actual compile UV vector width");std::copy_n(value.data(),2,v.uv);}
        else if(a==2){for(unsigned c=0;c<attribute.components;++c)v.color[c]=value[c]/(attribute.source_type==1?255.f:1.f);}
       }
       part.vertices.push_back(v);if(dynamic)part.source_ids_v112.push_back(original);found=remap.find(original);
      }
      part.indices.push_back(found->second);}
      last+=3;
     }
     if(last==cursor)return needed(e,"Actual batch limits cannot accept next source triangle");
     bounds(part);data->parts.push_back(std::move(part));mesh->compiler_segments().push_back({});
     if(!s.set_rendered(node.identity,e)||!callback(mesh->identity(),mesh->compiler_segments().size()-1,e)||!s.current(e))return false;
     cursor=last;
    }
   }
  }
  for(std::size_t i=0;;++i){std::size_t count;if(!node.child_count(count,e)||!s.current(e))return false;if(i>=count)break;loader::BatchNodeBorrowV96 child;if(!node.child(i,child,e)||!visit(child))return false;}
  return true;
 };
 for(const auto& root:roots)if(!visit(root))return false;
 //57bc78 swaps selected transparent pass buffers to the tail. Each native
 //backend part is one material/range buffer; static-vs-dynamic segment ranges
 //are therefore distinct buffers rather than merged posed snapshots.
 std::size_t solid=data->parts.size(),cursor=0;
 while(cursor<solid){std::uint32_t flags;std::memcpy(&flags,data->parts[cursor].pass_v112.pass.data()+4,4);
  if(!(flags&0x10000u)){++cursor;continue;}--solid;std::swap(data->parts[cursor],data->parts[solid]);
 }
 data->solid_batches_v112=static_cast<std::uint32_t>(solid);
 std::vector<NativeBatchSegmentV110> segments;segments.reserve(data->parts.size());
 for(auto& part:data->parts){segments.push_back(mesh->compiler_segments().at(part.segment));part.segment=segments.size()-1;}
 mesh->compiler_segments()=std::move(segments);
 for(auto& part:data->parts)if(part.dynamic_v112){part.source_buffer_v112->batch_backlinks_v112[part.source_primitive_v111]={mesh->identity(),part.segment,data};if(!data->update_segment_content_v112(part.segment,e))return false;}
 if(!data->parts.empty()){
  std::array<float,6> box{{INFINITY,INFINITY,INFINITY,-INFINITY,-INFINITY,-INFINITY}};
  for(const auto& part:data->parts)for(unsigned a=0;a<3;++a){box[a]=std::min(box[a],part.bounds[a]);box[a+3]=std::max(box[a+3],part.bounds[a+3]);}
  if(!mesh->source_update_bounds_v111(box,e))return false;
 }
 const std::weak_ptr<NativeBatchMeshV110> weak=mesh;auto visible=s.game_object_visible;
 data->visible=[weak,visible](std::size_t segment,bool& out,std::string& e){auto mesh=weak.lock();if(!mesh||!mesh->live()||segment>=mesh->compiler_segments().size())return needed(e,"Retired real compiled segment");const auto data=mesh->compiled_v111();if(data&&segment<data->parts.size()&&data->parts[segment].native_retired_v113){out=false;e.clear();return true;}const auto object=mesh->compiler_segments()[segment].game_object2c;if(!object)return needed(e,"Source compiled segment has NULL GameObject");return visible(object,out,e);};
 e.clear();return true;
}
}
