// Source particle render pass for the retained live combat-FX graph. Geometry,
// material colours, source node fields, exact assets and queue distance all
// come from the same current native resource; nothing is rebuilt from icons.
#include "renderer_authored_effect_draw_v5.inc"
#include "renderer_authored_effect_program_connection_v5.inc"
#include "renderer_effect_material_directory_v4.inc"
#include "renderer_authored_effect_sources_v5.inc"
struct EffectGpuTextureV4 {
 std::shared_ptr<dh2::resources::ContextResourceBudgetV37> resource_budget_v40;
 dh2::resources::CpuAdmissionV40 decoded_cpu_v40;
 dh2::resources::ResourceTokenV37 texture_budget_v40;
 std::uint64_t texture_generation_v40{};
 GLuint texture{};dh2::textures::GeneralFxTextureImageOwnerV2 image;
 dh2::textures::TextureParameterOwnerV1& owner(){return image.parameters();}
};
struct EffectGpuResourceV4 {
  GLuint vertices{},indices{};
  dh2::resources::ResourceTokenV37 vertex_budget_v38,index_budget_v38;
  std::uint64_t budget_generation_v38{};
 std::size_t vertex_capacity{},index_capacity{};
 std::uint64_t last_seen{};
 std::shared_ptr<const void> source_retention;
 dh2::fx::AuthoredFxGeometryCacheV34 geometry_cache;
 bool first_draw_reported{};
 std::shared_ptr<const std::vector<std::uint8_t>> bytes;
 std::shared_ptr<EffectProgramConnectionV5> program;
 std::shared_ptr<dh2::scene::ShaderProgramRecordV4> program_record;
 dh2::scene::EffectRenderPassV4 source_pass;
 dh2::scene::EffectMaterialDirectoryV4 comparison;
 std::shared_ptr<EffectGpuTextureV4> texture;
};
std::map<std::uintptr_t,EffectGpuResourceV4> effect_gpu_resources_v4;
std::map<std::string,std::shared_ptr<EffectGpuTextureV4>> effect_gpu_textures_v4;
std::vector<AuthoredEffectDrawSourceV5> effect_draw_sources_v4;
EffectSubmissionCountersV34 effect_submission_counters_v34;
std::unordered_map<std::uintptr_t,std::size_t> effect_source_indices_v34;
std::unordered_map<std::uintptr_t,std::uintptr_t> effect_material_indices_v34;
std::vector<dh2::scene::TransparentEntryV1> effect_queue_v34;
static void release_effect_buffers_v38(EffectGpuResourceV4& r,bool discard_context){
 const auto context=dh2::android_resources::budget_v38().snapshot();
 if(!discard_context&&context.context_ready&&r.budget_generation_v38==context.context_generation){
  if(r.vertices)glDeleteBuffers(1,&r.vertices);
  if(r.indices)glDeleteBuffers(1,&r.indices);
 }
 dh2::android_resources::release_v38(r.vertex_budget_v38);dh2::android_resources::release_v38(r.index_budget_v38);
 r.vertices=r.indices=0;r.vertex_capacity=r.index_capacity=0;r.budget_generation_v38=0;
}
static void allocate_effect_buffer_names_v38(EffectGpuResourceV4& r,std::string& error){
 using namespace dh2::resources;ResourceReservationV37 vertices,indices;
 auto& budget=dh2::android_resources::budget_v38();
 if(!budget.reserve_create({ResourceKindV37::vertex_buffer,ResourceScopeV37::fx,0,0},vertices,error)||
    !budget.reserve_create({ResourceKindV37::index_buffer,ResourceScopeV37::fx,0,0},indices,error))throw std::runtime_error("FX buffer admission: "+error);
  try{
   r.budget_generation_v38=budget.snapshot().context_generation;
   glGenBuffers(1,&r.vertices);glGenBuffers(1,&r.indices);check("Source effect buffer allocation");
   if(!r.vertices||!r.indices)throw std::runtime_error("Source effect buffer name unavailable");
  if(!vertices.commit(r.vertex_budget_v38,error)||!indices.commit(r.index_budget_v38,error))throw std::runtime_error("FX buffer admission commit: "+error);
 }catch(...){release_effect_buffers_v38(r,false);throw;}
}
static void upload_effect_storage_v38(EffectGpuResourceV4& resource,GLenum target,std::size_t bytes,const void* data,
 dh2::resources::ResourceKindV37 kind,dh2::resources::ResourceTokenV37& token,std::string& error){
 using namespace dh2::resources;ResourceReservationV37 candidate;
 // glBufferData may coexist with queued old storage even for the same GL name.
 // Admit the conservative old+candidate peak before requesting new driver bytes.
 if(!dh2::android_resources::budget_v38().reserve_replace(token,{kind,ResourceScopeV37::fx,bytes,0},ReplacementModeV37::same_object_coexisting_storage,candidate,error)){
  // Admission failure never touched GL. Retain old names/storage/charges,
  // but invalidate the CPU dirty receipt so a later retry resubmits geometry.
  resource.geometry_cache={};throw std::runtime_error("FX storage admission: "+error);
 }
 try{
  glBufferData(target,bytes,data,GL_DYNAMIC_DRAW);check("Source effect admitted storage upload");
  if(!candidate.commit(token,error))throw std::runtime_error("FX storage admission commit: "+error);
 }catch(...){
  // BufferData may already have replaced storage when the error is observed.
  // Discard both names and their exact charges before reporting the failure;
  // a later retry must rebuild geometry rather than reuse stale dirty flags.
  candidate.abort();release_effect_buffers_v38(resource,false);
  resource.geometry_cache={};throw;
 }
}
// Bounded modern GPU cache. Active source entries are never evicted; idle
// buffers expire after600 submissions or overflow256 retained entries.
static void trim_effect_gpu_cache_v34(){
 for(auto it=effect_gpu_resources_v4.begin();it!=effect_gpu_resources_v4.end();){
  if(effect_source_indices_v34.count(it->first)||effect_submission_counters_v34.syncs-it->second.last_seen<600){++it;continue;}
   auto& r=it->second;release_effect_buffers_v38(r,false);it=effect_gpu_resources_v4.erase(it);++effect_submission_counters_v34.evictions;
 }
 while(effect_gpu_resources_v4.size()>std::max<std::size_t>(256,effect_source_indices_v34.size())){
  auto oldest=effect_gpu_resources_v4.end();
  for(auto it=effect_gpu_resources_v4.begin();it!=effect_gpu_resources_v4.end();++it)
   if(!effect_source_indices_v34.count(it->first)&&(oldest==effect_gpu_resources_v4.end()||it->second.last_seen<oldest->second.last_seen))oldest=it;
  if(oldest==effect_gpu_resources_v4.end())break;
  release_effect_buffers_v38(oldest->second,false);effect_gpu_resources_v4.erase(oldest);++effect_submission_counters_v34.evictions;
 }
 effect_submission_counters_v34.cache_entries=static_cast<std::uint32_t>(effect_gpu_resources_v4.size());
}
dh2::textures::TextureDriverOptionsOwnerV1 effect_driver_options_v4;
dh2::textures::ShaderAdditionalConfigOwnerV1 effect_shader_config_v4;
std::vector<std::uint8_t> effect_shader_pack_v4;
dh2::scene::ShaderProgramCollectionV4 effect_program_collection_v4;
std::vector<std::shared_ptr<EffectProgramConnectionV5>> effect_program_owners_v4;
std::uint32_t effect_driver_unpack_alignment_v4{};
#include "renderer_effect_texture_v5.inc"
void clear_combat_fx_gpu(bool discard_context){
 effect_draw_sources_v4.clear();
 effect_queue_v34.clear();effect_source_indices_v34.clear();effect_material_indices_v34.clear();
  for(auto& pair:effect_gpu_resources_v4)release_effect_buffers_v38(pair.second,discard_context);
 if(!discard_context)for(auto& p:effect_program_owners_v4)p->release();
 std::string texture_error;
 for(auto& pair:effect_gpu_textures_v4)if(!release_effect_texture_v5(*pair.second,discard_context,false,texture_error))throw std::runtime_error(texture_error);
 effect_gpu_resources_v4.clear();effect_gpu_textures_v4.clear();effect_shader_pack_v4.clear();
 effect_program_collection_v4={};effect_program_owners_v4.clear();
 effect_shader_config_v4={};effect_driver_options_v4={};
 effect_submission_counters_v34={};
}
std::shared_ptr<EffectGpuTextureV4> effect_texture_v4(const std::string&,
 dh2::android_ui::OriginalCacheAssetsV1&);
void sync_authored_combat_fx_draw_sources_v5(std::vector<AuthoredEffectDrawSourceV5> parts,
 AAssetManager* assets,dh2::android_ui::OriginalCacheAssetsV1& cache){
 ++effect_submission_counters_v34.syncs;effect_submission_counters_v34.sources+=parts.size();
 effect_submission_counters_v34.visible_entries=static_cast<std::uint32_t>(parts.size());
 effect_source_indices_v34.clear();effect_material_indices_v34.clear();
 effect_source_indices_v34.reserve(parts.size());effect_material_indices_v34.reserve(parts.size());
 for(std::size_t i=0;i<parts.size();++i){const auto& s=parts[i];effect_source_indices_v34.emplace(s.render_identity,i);
  if(s.part.material_table&&s.material<s.part.material_table->size())effect_material_indices_v34.emplace(reinterpret_cast<std::uintptr_t>(&s.part.material_table->at(s.material)),s.render_identity);
 }
 if(parts.empty()){
  effect_draw_sources_v4.clear();
  effect_queue_v34.clear();trim_effect_gpu_cache_v34();return;
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
  if(!s.resource_bytes||!s.image||!s.scene||!s.render_identity||!s.source_node_render_fields_ready||
   !part.retention||!part.geometry||!part.material_table||!part.materials||
   part.geometry->primitives.size()!=1||part.materials->size()!=1||
   part.materials->front()!=s.material||part.positions.empty()||
   part.positions.size()!=part.geometry->positions.size()||part.positions.size()>65535)
   throw std::runtime_error("Required retained authored effect draw backing");
  const auto& material=part.material_table->at(s.material);
  auto found=effect_gpu_resources_v4.find(s.render_identity);
  if(found!=effect_gpu_resources_v4.end()&&found->second.bytes!=s.resource_bytes){
   auto& old=found->second;release_effect_buffers_v38(old,false);
   effect_gpu_resources_v4.erase(found);found=effect_gpu_resources_v4.end();
  }
  if(found==effect_gpu_resources_v4.end()){
   ++effect_submission_counters_v34.cold_resources;
    auto& r=effect_gpu_resources_v4[s.render_identity];r.bytes=s.resource_bytes;
    try{
   r.texture=effect_texture_v4(material.diffuse,cache);
   if(!dh2::scene::effect_render_pass_v4(*s.image,material.id.c_str(),"default",r.source_pass,error))throw std::runtime_error(error);
   struct Creation {const AuthoredEffectDrawSourceV5& source;const char* material;};
   Creation creation{s,material.id.c_str()};
   if(!effect_program_collection_v4.get_or_create(dh2::scene::effect_program_cache_name_v4(r.source_pass),&creation,
    [](void* raw,std::uint16_t,const std::string&,std::shared_ptr<void>& owner,
       std::vector<dh2::scene::ShaderUniformReflectionV4>& reflected,std::string& why){
     const auto& c=*static_cast<Creation*>(raw);auto p=std::make_shared<EffectProgramConnectionV5>();
     try{p->initialize(*c.source.image,c.material,effect_shader_pack_v4,effect_driver_options_v4.flags88(),effect_shader_config_v4.config());
      if(!reflect_effect_program_v4(p->native_program(),reflected,why)){p->release();return false;}
     }catch(...){p->release();throw;}
     owner=p;effect_program_owners_v4.push_back(p);return true;
    },r.program_record,error))throw std::runtime_error(error);
   r.program=std::static_pointer_cast<EffectProgramConnectionV5>(r.program_record->same_program);
    allocate_effect_buffer_names_v38(r,error);
    }catch(...){
     release_effect_buffers_v38(r,false);effect_gpu_resources_v4.erase(s.render_identity);throw;
    }
   found=effect_gpu_resources_v4.find(s.render_identity);
  }else ++effect_submission_counters_v34.warm_hits;
   auto& r=found->second;
   if(!r.vertices||!r.indices)allocate_effect_buffer_names_v38(r,error);
  r.last_seen=effect_submission_counters_v34.syncs;r.source_retention=part.retention;
  dh2::fx::AuthoredFxGeometryChangeV34 changed;
  if(!r.geometry_cache.update(part,s.material,changed,error))throw std::runtime_error(error);
  const auto& packet=r.geometry_cache.packet();const auto& vertices=packet.vertices;const auto& indices=packet.indices;
  ++effect_submission_counters_v34.packet_updates;if(changed.indices)++effect_submission_counters_v34.topology_rebuilds;
  const auto vertex_bytes=vertices.size()*sizeof(Vertex),index_bytes=indices.size()*sizeof(std::uint16_t);
  if(changed.vertices||!r.vertex_capacity){glBindBuffer(GL_ARRAY_BUFFER,r.vertices);
    if(vertex_bytes>r.vertex_capacity){upload_effect_storage_v38(r,GL_ARRAY_BUFFER,vertex_bytes,vertices.data(),dh2::resources::ResourceKindV37::vertex_buffer,r.vertex_budget_v38,error);r.vertex_capacity=vertex_bytes;++effect_submission_counters_v34.storage_allocations;}
   else glBufferSubData(GL_ARRAY_BUFFER,0,vertex_bytes,vertices.data());
   ++effect_submission_counters_v34.vertex_uploads;effect_submission_counters_v34.vertex_bytes+=vertex_bytes;
  }else ++effect_submission_counters_v34.vertex_skips;
  if(changed.indices||!r.index_capacity){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,r.indices);
    if(index_bytes>r.index_capacity){upload_effect_storage_v38(r,GL_ELEMENT_ARRAY_BUFFER,index_bytes,indices.data(),dh2::resources::ResourceKindV37::index_buffer,r.index_budget_v38,error);r.index_capacity=index_bytes;++effect_submission_counters_v34.storage_allocations;}
   else glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,index_bytes,indices.data());
   ++effect_submission_counters_v34.index_uploads;effect_submission_counters_v34.index_bytes+=index_bytes;
  }else ++effect_submission_counters_v34.index_skips;
   try{check("Source effect retained geometry upload");}
   catch(...){release_effect_buffers_v38(r,false);r.geometry_cache={};throw;}
  if(!s.source_texture_matrix68)throw std::runtime_error("Required actual source effect Matrix4f68 owner");
  const float bias=r.texture->owner().lod_bias();
  EffectMaterialProfileContextV4 values;
  values.borrowed={material.color,s.source_texture_matrix68,reinterpret_cast<std::uintptr_t>(r.texture.get()),&bias};
  if(!r.comparison.refresh(r.program_record,r.source_pass,&values,EffectMaterialProfileContextV4::read,error))throw std::runtime_error(error);
 }
 effect_draw_sources_v4=std::move(parts);
 trim_effect_gpu_cache_v34();
}
void draw_combat_fx_v4(){
 if(effect_draw_sources_v4.empty())return;
 using namespace dh2::scene;
 auto& queue=effect_queue_v34;queue.clear();queue.reserve(effect_draw_sources_v4.size());
 TransparentQueueServicesV1 services;
 auto source=[&](std::uintptr_t id)->const AuthoredEffectDrawSourceV5*{
  const auto found=effect_source_indices_v34.find(id);return found==effect_source_indices_v34.end()?nullptr:&effect_draw_sources_v4[found->second];
 };
 services.node_priority_d8=[&](auto id,std::int32_t& value,std::string&){const auto* s=source(id);if(!s)return false;value=s->rendering_layer;return true;};
 services.node_distance_bias_d0=[&](auto id,float& value,std::string&){const auto* s=source(id);if(!s)return false;std::memcpy(&value,&s->camera_offset_word,4);return true;};
 services.node_position_38=[&](auto id,std::array<float,3>& value,std::string&){const auto* s=source(id);if(!s)return false;value={s->emitter_world[12],s->emitter_world[13],s->emitter_world[14]};return true;};
 services.node_suborder_20=[&](auto id,auto part,std::int32_t& value,std::string&){const auto* s=source(id);if(!s||part!=s->source_part)return false;value=s->particle?0:std::int32_t(s->part.geometry->positions.size());return true;};
 auto material_view=[&](std::uintptr_t identity)->const MaterialCompareViewV4*{
  const auto found=effect_material_indices_v34.find(identity);if(found==effect_material_indices_v34.end())return nullptr;
  const auto r=effect_gpu_resources_v4.find(found->second);return r==effect_gpu_resources_v4.end()?nullptr:r->second.comparison.view();
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
  TransparentEntryV1 entry;entry.node=s.render_identity;entry.part=s.source_part;
  entry.material=reinterpret_cast<std::uintptr_t>(&s.part.material_table->at(s.material));
  entry.node_owner=s.part.retention;entry.material_owner=s.part.retention;
  if(!transparent_entry_v1(entry,submitted_eye,nullptr,0x7fffffff,services,error))throw std::runtime_error(error);
  std::uint32_t flags=0;std::memcpy(&flags,effect_gpu_resources_v4.at(s.render_identity).source_pass.pass.data()+4,4);
  if(!(flags&0x10000)){if(!s.particle)throw std::runtime_error("Required original opaque authored mesh queue");}
  queue.push_back(std::move(entry));
 }
 if(!transparent_sort_v2(queue,services,error))throw std::runtime_error(error);
 EffectDrawProgramV5::BatchScope batch_scope;++effect_submission_counters_v34.state_snapshots;
 for(const auto& entry:queue){
  const auto* s=source(entry.node);if(!s)throw std::runtime_error("Source effect queue lost node");
   auto& r=effect_gpu_resources_v4.at(entry.node);const auto wvp=dh2::scene::multiply(submitted_camera,s->part.world);
  EffectDrawBorrowV5 draw;draw.vertices=r.vertices;draw.indices=r.indices;draw.texture=r.texture->texture;
  draw.count=static_cast<GLsizei>(s->part.geometry->primitives.front().indices.size());draw.stride=sizeof(Vertex);
  draw.position_offset=offsetof(Vertex,p);draw.color_offset=offsetof(Vertex,color);draw.uv_offset=offsetof(Vertex,uv);
  draw.world_view_projection=wvp.data();draw.diffuse_color=s->part.material_table->at(s->material).color;
  draw.source_color_missing=s->part.geometry->primitives.front().attributes[2]<0;
  draw.source_texture_ready=r.texture->owner().ready();draw.sampler_bias=r.texture->owner().lod_bias();
   r.program->draw_one_in_batch(batch_scope,r.source_pass,draw,false,true,effect_driver_options_v4.flags88(),effect_shader_config_v4.config());
   ++effect_submission_counters_v34.draws;
   if(!r.first_draw_reported){
    check("Source combat effect first live draw");r.first_draw_reported=true;
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Source effect live draw | node %016llx | material %s | vertices %zu | indices %d | same source program/reflection/material/texture/queue",static_cast<unsigned long long>(entry.node),s->part.material_table->at(s->material).id.c_str(),s->part.positions.size(),draw.count);
   }
 }
 check("Source combat effect transparent draw");
}

// Compatibility transport for the existing all-particle frame; the mixed
// frame must convert BOTH actual source lists and submit in one call above.
[[maybe_unused]] void sync_combat_fx_draw_sources_v3(std::vector<dh2::fx::CharacterParticleDrawSourceV3> parts,AAssetManager* assets,dh2::android_ui::OriginalCacheAssetsV1& cache){
 std::vector<AuthoredEffectDrawSourceV5> actual;for(auto& s:parts)actual.push_back(authored_effect_source_v5(std::move(s)));
 sync_authored_combat_fx_draw_sources_v5(std::move(actual),assets,cache);
}
