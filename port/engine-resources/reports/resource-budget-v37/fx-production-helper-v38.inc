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
