// Source particle render pass for the retained live combat-FX graph. Geometry,
// material colours, source node fields, exact assets and queue distance all
// come from the same current native resource; nothing is rebuilt from icons.
#include "renderer_effect_draw_v4.inc"
#include "renderer_effect_program_connection_v4.inc"
#include "renderer_effect_material_directory_v4.inc"
struct EffectGpuTextureV4 {
 GLuint texture{};dh2::textures::BloodTextureImageOwnerV1 image;
 dh2::textures::TextureParameterOwnerV1& owner(){return image.parameters();}
};
struct EffectGpuResourceV4 {
  GLuint vertices{},indices{};
 bool first_draw_reported{};
 std::shared_ptr<const std::vector<std::uint8_t>> bytes;
 std::shared_ptr<EffectProgramConnectionV4> program;
 std::shared_ptr<dh2::scene::ShaderProgramRecordV4> program_record;
 dh2::scene::EffectRenderPassV4 source_pass;
 dh2::scene::EffectMaterialDirectoryV4 comparison;
 std::shared_ptr<EffectGpuTextureV4> texture;
};
std::map<std::uintptr_t,EffectGpuResourceV4> effect_gpu_resources_v4;
std::map<std::string,std::shared_ptr<EffectGpuTextureV4>> effect_gpu_textures_v4;
std::vector<dh2::fx::CharacterParticleDrawSourceV3> effect_draw_sources_v4;
dh2::textures::TextureDriverOptionsOwnerV1 effect_driver_options_v4;
dh2::textures::ShaderAdditionalConfigOwnerV1 effect_shader_config_v4;
std::vector<std::uint8_t> effect_shader_pack_v4;
dh2::scene::ShaderProgramCollectionV4 effect_program_collection_v4;
std::vector<std::shared_ptr<EffectProgramConnectionV4>> effect_program_owners_v4;
std::uint32_t effect_driver_unpack_alignment_v4{};
#include "renderer_effect_texture_v5.inc"
void clear_combat_fx_gpu(bool discard_context){
 effect_draw_sources_v4.clear();
 if(!discard_context){
  for(auto& pair:effect_gpu_resources_v4){auto& r=pair.second;
   if(r.vertices)glDeleteBuffers(1,&r.vertices);if(r.indices)glDeleteBuffers(1,&r.indices);
  }
 }
 if(!discard_context)for(auto& p:effect_program_owners_v4)p->release();
 std::string texture_error;
 for(auto& pair:effect_gpu_textures_v4)if(!release_effect_texture_v5(*pair.second,discard_context,false,texture_error))throw std::runtime_error(texture_error);
 effect_gpu_resources_v4.clear();effect_gpu_textures_v4.clear();effect_shader_pack_v4.clear();
 effect_program_collection_v4={};effect_program_owners_v4.clear();
 effect_shader_config_v4={};effect_driver_options_v4={};
}
std::shared_ptr<EffectGpuTextureV4> effect_texture_v4(const std::string&,
 dh2::android_ui::OriginalCacheAssetsV1&);
void sync_combat_fx_draw_sources_v3(std::vector<dh2::fx::CharacterParticleDrawSourceV3> parts,
 AAssetManager* assets,dh2::android_ui::OriginalCacheAssetsV1& cache){
 if(parts.empty()){
  effect_draw_sources_v4.clear();
  for(auto& pair:effect_gpu_resources_v4){auto& r=pair.second;glDeleteBuffers(1,&r.vertices);glDeleteBuffers(1,&r.indices);}
  effect_gpu_resources_v4.clear();return;
 }
 std::string error;
 if(effect_shader_pack_v4.empty()){
  effect_shader_pack_v4=read(assets,"shaders.pak","shaders");
  dh2::scene::ShaderSourcePack pack;
  if(!pack.load(effect_shader_pack_v4.data(),effect_shader_pack_v4.size(),error))throw std::runtime_error(error);
  dh2::textures::ShaderConfigFileServicesV1 files;
  files.read=[&](const char* name,bool& found,std::string& value,std::string&){
   const auto* actual=pack.find(name);found=actual!=nullptr;
   if(actual){if(actual->bytes.empty())value.clear();else value.assign(reinterpret_cast<const char*>(actual->bytes.data()),actual->bytes.size());}return true;
  };
  if(!effect_shader_config_v4.initialize(files,error))throw std::runtime_error(error);
 }
 for(const auto& s:parts){
  const auto& part=s.part;
  if(!s.resource_bytes||!s.image||!s.scene||!s.particle_identity||!s.source_node_render_fields_ready||
   !part.retention||!part.geometry||!part.material_table||!part.materials||
   part.geometry->primitives.size()!=1||part.materials->size()!=1||
   part.materials->front()!=s.material||part.positions.empty()||
   part.positions.size()!=part.geometry->positions.size()||part.positions.size()>65535)
   throw std::runtime_error("Required retained authored effect draw backing");
  const auto& material=part.material_table->at(s.material);
  const auto& primitive=part.geometry->primitives.front();
  if(primitive.engine_type!=3||primitive.collada_type||primitive.material_symbol!=material.id||
   primitive.indices.empty()||primitive.indices.size()%3||primitive.attributes[2]<0||primitive.attributes[4]<0)
   throw std::runtime_error("Required authored effect triangles/attributes");
  const auto& colors=part.geometry->attributes.at(primitive.attributes[2]);
  const auto& uv=part.geometry->attributes.at(primitive.attributes[4]);
  if(colors.type!=1||colors.components!=4||uv.components!=2||
   colors.values.size()!=part.positions.size()*4||uv.values.size()!=part.positions.size()*2)
   throw std::runtime_error("Required source effect vertex streams");
  std::vector<Vertex> vertices(part.positions.size());
  for(std::size_t i=0;i<vertices.size();++i){
   std::copy_n(part.positions[i].data(),3,vertices[i].p);
   std::copy_n(uv.values.data()+i*2,2,vertices[i].uv);
   for(unsigned j=0;j<4;++j)vertices[i].color[j]=colors.values[i*4+j]/255.f;
  }
  std::vector<std::uint16_t> indices;indices.reserve(primitive.indices.size());
  for(auto index:primitive.indices){if(index>=vertices.size())throw std::runtime_error("Source effect index range");indices.push_back(std::uint16_t(index));}
  auto found=effect_gpu_resources_v4.find(s.particle_identity);
  if(found!=effect_gpu_resources_v4.end()&&found->second.bytes!=s.resource_bytes){
   auto& old=found->second;if(old.vertices)glDeleteBuffers(1,&old.vertices);if(old.indices)glDeleteBuffers(1,&old.indices);
   effect_gpu_resources_v4.erase(found);found=effect_gpu_resources_v4.end();
  }
  if(found==effect_gpu_resources_v4.end()){
   auto& r=effect_gpu_resources_v4[s.particle_identity];r.bytes=s.resource_bytes;
   r.texture=effect_texture_v4(material.diffuse,cache);
   if(!dh2::scene::effect_render_pass_v4(*s.image,material.id.c_str(),"default",r.source_pass,error))throw std::runtime_error(error);
   struct Creation {const dh2::fx::CharacterParticleDrawSourceV3& source;const char* material;};
   Creation creation{s,material.id.c_str()};
   if(!effect_program_collection_v4.get_or_create(dh2::scene::effect_program_cache_name_v4(r.source_pass),&creation,
    [](void* raw,std::uint16_t,const std::string&,std::shared_ptr<void>& owner,
       std::vector<dh2::scene::ShaderUniformReflectionV4>& reflected,std::string& why){
     const auto& c=*static_cast<Creation*>(raw);auto p=std::make_shared<EffectProgramConnectionV4>();
     try{p->initialize(*c.source.image,c.material,effect_shader_pack_v4,effect_driver_options_v4.flags88(),effect_shader_config_v4.config());
      if(!reflect_effect_program_v4(p->native_program(),reflected,why)){p->release();return false;}
     }catch(...){p->release();throw;}
     owner=p;effect_program_owners_v4.push_back(p);return true;
    },r.program_record,error))throw std::runtime_error(error);
   r.program=std::static_pointer_cast<EffectProgramConnectionV4>(r.program_record->same_program);
   glGenBuffers(1,&r.vertices);glGenBuffers(1,&r.indices);check("Source effect buffer allocation");
   found=effect_gpu_resources_v4.find(s.particle_identity);
  }
  auto& r=found->second;
  glBindBuffer(GL_ARRAY_BUFFER,r.vertices);glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),vertices.data(),GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,r.indices);glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*2,indices.data(),GL_DYNAMIC_DRAW);
  check("Source effect retained geometry upload");
  if(!s.source_texture_matrix68)throw std::runtime_error("Required actual source effect Matrix4f68 owner");
  const float bias=r.texture->owner().lod_bias();
  EffectMaterialProfileContextV4 values;
  values.borrowed={material.color,s.source_texture_matrix68,reinterpret_cast<std::uintptr_t>(r.texture.get()),&bias};
  if(!r.comparison.refresh(r.program_record,r.source_pass,&values,EffectMaterialProfileContextV4::read,error))throw std::runtime_error(error);
 }
 effect_draw_sources_v4=std::move(parts);
 // Drop finished resource GPU buffers while their owning GL context is live.
 for(auto it=effect_gpu_resources_v4.begin();it!=effect_gpu_resources_v4.end();){
  const bool live=std::any_of(effect_draw_sources_v4.begin(),effect_draw_sources_v4.end(),[&](const auto& s){return s.particle_identity==it->first;});
  if(live){++it;continue;}auto& r=it->second;
  glDeleteBuffers(1,&r.vertices);glDeleteBuffers(1,&r.indices);it=effect_gpu_resources_v4.erase(it);
 }
}
void draw_combat_fx_v4(){
 if(effect_draw_sources_v4.empty())return;
 using namespace dh2::scene;
 std::vector<TransparentEntryV1> queue;
 TransparentQueueServicesV1 services;
 auto source=[&](std::uintptr_t id)->const dh2::fx::CharacterParticleDrawSourceV3*{
  const auto found=std::find_if(effect_draw_sources_v4.begin(),effect_draw_sources_v4.end(),[&](const auto& s){return s.particle_identity==id;});
  return found==effect_draw_sources_v4.end()?nullptr:&*found;
 };
 services.node_priority_d8=[&](auto id,std::int32_t& value,std::string&){const auto* s=source(id);if(!s)return false;value=s->rendering_layer;return true;};
 services.node_distance_bias_d0=[&](auto id,float& value,std::string&){const auto* s=source(id);if(!s)return false;std::memcpy(&value,&s->camera_offset_word,4);return true;};
 services.node_position_38=[&](auto id,std::array<float,3>& value,std::string&){const auto* s=source(id);if(!s)return false;value={s->emitter_world[12],s->emitter_world[13],s->emitter_world[14]};return true;};
 services.node_suborder_20=[&](auto id,auto part,std::int32_t& value,std::string&){if(!source(id)||part)return false;value=0;return true;};
 auto material_view=[&](std::uintptr_t identity)->const MaterialCompareViewV4*{
  for(const auto& s:effect_draw_sources_v4)if(identity==reinterpret_cast<std::uintptr_t>(&s.part.material_table->at(s.material))){
   const auto r=effect_gpu_resources_v4.find(s.particle_identity);
   return r==effect_gpu_resources_v4.end()?nullptr:r->second.comparison.view();
  }
  return nullptr;
 };
 services.material_equal_3537b0=[&](auto a,auto b,bool& value,std::string& error){
  const auto* left=material_view(a);const auto* right=material_view(b);
  if(!left||!right){error="Required current source effect material equality backing";return false;}
  return material_equal_v4(value,*left,*right,error)==0;
 };
 services.material_less_3537e4=[&](auto a,auto b,bool& value,std::string& error){
  const auto* left=material_view(a);const auto* right=material_view(b);
  if(!left||!right){error="Required current source effect material order backing";return false;}
  return material_less_v4(value,*left,*right,error)==0;
 };
 std::string error;
 for(const auto& s:effect_draw_sources_v4){
  TransparentEntryV1 entry;entry.node=s.particle_identity;entry.part=0;
  entry.material=reinterpret_cast<std::uintptr_t>(&s.part.material_table->at(s.material));
  entry.node_owner=s.part.retention;entry.material_owner=s.part.retention;
  if(!transparent_entry_v1(entry,submitted_eye,nullptr,0x7fffffff,services,error))throw std::runtime_error(error);
  queue.push_back(std::move(entry));
 }
 if(!transparent_sort_v2(queue,services,error))throw std::runtime_error(error);
 for(const auto& entry:queue){
  const auto* s=source(entry.node);if(!s)throw std::runtime_error("Source effect queue lost node");
   auto& r=effect_gpu_resources_v4.at(entry.node);const auto wvp=dh2::scene::multiply(submitted_camera,s->part.world);
  EffectDrawBorrowV4 draw;draw.vertices=r.vertices;draw.indices=r.indices;draw.texture=r.texture->texture;
  draw.count=static_cast<GLsizei>(s->part.geometry->primitives.front().indices.size());draw.stride=sizeof(Vertex);
  draw.position_offset=offsetof(Vertex,p);draw.color_offset=offsetof(Vertex,color);draw.uv_offset=offsetof(Vertex,uv);
  draw.world_view_projection=wvp.data();draw.diffuse_color=s->part.material_table->at(s->material).color;
  draw.source_texture_ready=r.texture->owner().ready();draw.sampler_bias=r.texture->owner().lod_bias();
   r.program->draw_pass(r.source_pass,{draw},false,true,effect_driver_options_v4.flags88(),effect_shader_config_v4.config());
   if(!r.first_draw_reported){
    check("Source combat effect first live draw");r.first_draw_reported=true;
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source effect live draw | node %016llx | material %s | vertices %zu | indices %d | same source program/reflection/material/texture/queue",static_cast<unsigned long long>(entry.node),s->part.material_table->at(s->material).id.c_str(),s->part.positions.size(),draw.count);
   }
 }
 check("Source combat effect transparent draw");
}
