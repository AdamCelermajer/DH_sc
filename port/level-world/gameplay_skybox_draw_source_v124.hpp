#pragma once
#include "gameplay_skybox_material_v25.hpp"
#include "native_batch_compiler_v111.hpp"
#include <algorithm>
namespace dh2::camera {
// Real BRES streams and the SAME V25 mutated material state. This adapter
// builds shader input, without creating a second node, camera or renderer.
inline bool skybox_draw_source_v124(const GameplaySkyboxPipelineV25& pipeline,
 const SkyboxResourceV24& resource,std::size_t buffer,world::NativeBatchPartV111& out,std::string& e){
 if(buffer>=resource.buffers.size()){e="Skybox draw buffer outside actual mesh";return false;}
 const auto& source=resource.buffers[buffer];
 if(source.material>=resource.scene.materials.size()||source.positions.empty()||
    source.positions.size()>65536||source.primitive.collada_type||source.indices.empty()||source.indices.size()%3){e="Skybox primitive outside native triangle domain";return false;}
 out.material=resource.scene.materials[source.material];
 std::shared_ptr<SkyboxMaterialV25> material;if(!pipeline.material(resource,source.material,material,e))return false;
 // CMaterial C1 selects row0. CurrentTechnique selects a named row only
 // when the authored material actually supplies it (same V112 rule).
 std::uint32_t selected=0;
 if(!out.material.gles2_technique.empty()){
  bool found=false;for(std::uint32_t i=0;i<material->techniques().size();++i)
   if(material->techniques()[i].name==out.material.gles2_technique){if(found){e="Duplicate authored skybox technique";return false;}found=true;selected=i;}
  if(!found){e="Authored skybox CurrentTechnique absent";return false;}
 }
 if(!material->state(selected,out.pass_v112,e))return false;
 if(out.pass_v112.stencil||out.pass_v112.sample_coverage||out.pass_v112.polygon_offset){e="Skybox auxiliary pass-state backend absent";return false;}
 auto values=std::make_shared<world::NativeBatchMaterialValuesV113>();
 if(!world::decode_batch_material_values_v113(resource.bres,out.material.id,*values,e))return false;
 if(!world::decode_batch_effect_values_v113(resource.bres,out.material.effect_uri,
     out.material.gles2_technique,*values,e))return false;
 out.material_values_v113=std::move(values);out.vertices.resize(source.positions.size());
 for(unsigned slot=0;slot<out.attributes.size();++slot){
  assets::Attribute attribute;
  if(dh2_mesh_attribute(&resource.mesh,source.primitive.attributes[slot],&attribute)!=assets::Error::ok)continue;
  if(attribute.vertices!=out.vertices.size()||!attribute.components||attribute.components>4){e="Skybox actual attribute width/count unsupported";return false;}
  auto& stream=out.attributes[slot];stream.source_type=attribute.type;stream.components=attribute.components;stream.values.resize(attribute.vertices);
  for(unsigned i=0;i<attribute.vertices;++i)if(!dh2_attribute_read(&attribute,i,stream.values[i].data())){e="Short skybox source attribute";return false;}
 }
 for(std::size_t i=0;i<out.vertices.size();++i){std::copy_n(source.positions[i].data(),3,out.vertices[i].position);
  if(i<source.uv.size())std::copy_n(source.uv[i].data(),2,out.vertices[i].uv);}
 out.indices.reserve(source.indices.size());for(auto index:source.indices){
  if(index>=out.vertices.size()){e="Skybox index outside actual vertex stream";return false;}out.indices.push_back(static_cast<std::uint16_t>(index));
 }
 e.clear();return true;
}
}
