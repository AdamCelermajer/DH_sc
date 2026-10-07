namespace {
struct LootGpuRecordV27 {
 dh2::world::RetainedVisualDrawV27 geometry;
 std::vector<Draw> draws;
};
std::map<std::uintptr_t,LootGpuRecordV27> loot_gpu_records_v27;
std::map<std::string,GLuint> loot_gpu_texture_cache_v27;
std::vector<GLuint> loot_gpu_textures_v27;
void release_loot_gpu_record_v27(LootGpuRecordV27& record,bool lost){
 if(lost){for(auto& draw:record.draws){release_buffer_v41(draw.vertices,true);release_buffer_v41(draw.indices,true);}record.draws.clear();}else{std::vector<GLuint> none;release(record.draws,none);}
}
void clear_retained_loot_gpu_v27(bool lost){
 for(auto& entry:loot_gpu_records_v27)release_loot_gpu_record_v27(entry.second,lost);
 loot_gpu_records_v27.clear();
 for(auto name:loot_gpu_textures_v27)release_model_texture_v40(name,lost);
 loot_gpu_textures_v27.clear();loot_gpu_texture_cache_v27.clear();
}
void refresh_loot_gpu_vertices_v27(LootGpuRecordV27& record){
 const auto& parts=record.geometry.parts();
 if(parts.size()!=record.draws.size())throw std::runtime_error("Loot GPU primitive count changed after source binding");
 for(std::size_t i=0;i<parts.size();++i){const auto& part=parts[i];auto& draw=record.draws[i];
  if(draw.cpu_geometry_v41.vertices.size()!=part.vertices.size())throw std::runtime_error("Loot GPU vertex count changed after source binding");
  for(std::size_t v=0;v<part.vertices.size();++v){
   std::copy_n(part.vertices[v].position,3,draw.cpu_geometry_v41.vertices[v].p);
   std::copy_n(part.vertices[v].uv,2,draw.cpu_geometry_v41.vertices[v].uv);
   std::copy_n(part.vertices[v].color,4,draw.cpu_geometry_v41.vertices[v].color);
  }
  draw.placement=part.matrix;
  buffer_subdata_v41(draw.vertices,GL_ARRAY_BUFFER,buffer_bytes_v41(draw.cpu_geometry_v41.vertices.size(),sizeof(Vertex)),draw.cpu_geometry_v41.vertices.data(),dh2::resources::ResourceScopeV37::loot);
 }
}
void build_loot_gpu_record_v27(LootGpuRecordV27& record,const std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>& visual){
 ModelPublicationScopeV50 publication_scope;
 std::string error;if(!record.geometry.bind(visual,error))throw std::runtime_error(error);
 const auto& parts=record.geometry.parts();record.draws.reserve(parts.size());
 try{
  for(const auto& part:parts){
   record.draws.emplace_back();auto& draw=record.draws.back();
   draw.material=visual->scene().materials.at(part.material);draw.count=GLsizei(part.indices.size());draw.environment=true;
   if(!dh2::resources::reserve_cpu_vector_v41(draw.cpu_geometry_v41.vertices,draw.cpu_geometry_v41.capacity,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::loot,part.vertices.size(),error))throw std::runtime_error(error);
   draw.cpu_geometry_v41.vertices.resize(part.vertices.size());draw.placement=part.matrix;
   draw.diffuse=upload(equipment_platform->manager,draw.material.diffuse,loot_gpu_texture_cache_v27,loot_gpu_textures_v27,&equipment_platform->cache,dh2::resources::ResourceScopeV37::loot);
   draw.alpha=upload(equipment_platform->manager,draw.material.alpha_map,loot_gpu_texture_cache_v27,loot_gpu_textures_v27,&equipment_platform->cache,dh2::resources::ResourceScopeV37::loot);
   buffer_storage_v41(draw.vertices,GL_ARRAY_BUFFER,buffer_bytes_v41(draw.cpu_geometry_v41.vertices.size(),sizeof(Vertex)),nullptr,GL_DYNAMIC_DRAW,dh2::resources::ResourceScopeV37::loot);
   buffer_storage_v41(draw.indices,GL_ELEMENT_ARRAY_BUFFER,buffer_bytes_v41(part.indices.size(),sizeof(std::uint16_t)),part.indices.data(),GL_STATIC_DRAW,dh2::resources::ResourceScopeV37::loot);
  }
  refresh_loot_gpu_vertices_v27(record);check("Actual retained loot GPU geometry upload");
 }catch(...){release_loot_gpu_record_v27(record,false);throw;}
}
}
bool sync_loot_visual_draws_v27(const std::vector<LootVisualDrawSourceV27>& actual,std::string& error){
 if(!world_mode||!live_script_world||!equipment_platform||equipment_platform->world!=live_script_world||!live_script_world->canonical_world||!live_script_world->canonical_world->scene_roots_v20){
  error="Required actual current World/cache/SceneManager for loot GPU submission";return false;
 }
 const auto roots=live_script_world->canonical_world->scene_roots_v20->roots();std::set<std::uintptr_t> alive;
 try{
  for(const auto& source:actual){
   if(!source.object||!source.visual||!source.visual->ready()||!source.visual->root_identity()||!alive.insert(source.object).second)
    throw std::runtime_error("Loot GPU source requires unique initialized actual Item/visual receiver");
   if(std::find(roots.begin(),roots.end(),source.visual->root_identity())==roots.end())throw std::runtime_error("Loot GPU root is not registered in SAME actual SceneManager");
   const auto owner=source.visual->root_game_object();if(owner&&owner!=source.object)throw std::runtime_error("Loot GPU root names a different source GameObject");
   auto found=loot_gpu_records_v27.find(source.object);
   if(found!=loot_gpu_records_v27.end()&&found->second.geometry.visual()!=source.visual){release_loot_gpu_record_v27(found->second,false);loot_gpu_records_v27.erase(found);found=loot_gpu_records_v27.end();}
   if(found==loot_gpu_records_v27.end()){
    auto result=loot_gpu_records_v27.try_emplace(source.object);auto& record=result.first->second;
    try{build_loot_gpu_record_v27(record,source.visual);}catch(...){loot_gpu_records_v27.erase(result.first);throw;}
    __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actual loot GPU root attached | Item %016llx | root %016llx | parts %zu | SAME retained visual/pose",
     static_cast<unsigned long long>(source.object),static_cast<unsigned long long>(source.visual->root_identity()),record.draws.size());
   }else{
    if(!found->second.geometry.refresh(error))throw std::runtime_error(error);
    refresh_loot_gpu_vertices_v27(found->second);
   }
  }
  for(auto i=loot_gpu_records_v27.begin();i!=loot_gpu_records_v27.end();){
   if(alive.count(i->first)){++i;continue;}
   release_loot_gpu_record_v27(i->second,false);i=loot_gpu_records_v27.erase(i);
  }
  check("Actual retained loot GPU pose update");error.clear();return true;
 }catch(const std::exception& ex){error=ex.what();return false;}
}
