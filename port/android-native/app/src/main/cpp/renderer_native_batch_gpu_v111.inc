//Included in public model_renderer scope after SourceGeometryGpuV64 helper
//definitions. These resources use SAME Draw/texture/buffer budget/context.
bool upload_source_batch_mesh_v111(const std::shared_ptr<void>& world,const std::shared_ptr<dh2::world::NativeBatchMeshV110>& mesh,AAssetManager* assets,std::string& e)try{
 SourceCampaignCandidateBorrowV55 actual;if(!assets||!mesh||!mesh->live()||!mesh->compiled_v111()||!borrow_source_campaign_candidate_runtime_v61(actual,e)||actual.actual_world!=world)return false;
 auto& gpu=source_geometry_gpu_v64;const auto context=dh2::android_resources::budget_lease_v39()->context_state_v41();
 if(!context.ready){e="Batch upload requires actual current GL context";return false;}
 if(!same_source_geometry_v64(actual)){
  if(!gpu.world.expired()&&gpu.world.lock()!=world){e="Batch GPU owner belongs to another campaign";return false;}
  release_source_geometry_gpu_v64(!context.ready||context.generation!=gpu.generation);
  gpu.assets=assets;gpu.original=std::make_unique<dh2::android_ui::OriginalCacheAssetsV1>(assets);gpu.world=world;gpu.level=actual.level;gpu.generation=context.generation;
 }else if(gpu.assets!=assets){e="Actual batch APK provider changed";return false;}
 auto found=gpu.compiled.find(mesh->identity());if(found!=gpu.compiled.end()){e="Actual compiled GPU prefix already exists; cannot replay upload";return false;}
 auto& target=gpu.compiled[mesh->identity()];target.source=mesh;
 auto data=mesh->compiled_v111();data->context_generation=context.generation;
 for(const auto& part:data->parts){
  if(part.native_retired_v113){target.batches.emplace_back();continue;}
  if(part.vertices.empty()||part.indices.empty()){e="Compiled native primitive has no source vertex/index stream";return false;}
  Draw batch;batch.material=part.material;batch.source_pass_v112=part.pass_v112;batch.environment=true;batch.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
  batch.source_mesh_v111=part.source_mesh_v111;batch.source_primitive_v111=part.source_primitive_v111;
  for(const auto& vertex:part.vertices)batch.clip_bounds.include(vertex.position);
  if(part.indices.size()>static_cast<std::size_t>(INT32_MAX)){e="Actual compiled primitive exceeds GLES signed count";return false;}batch.count=static_cast<GLsizei>(part.indices.size());
  //Retain the reached prefix before shader/stream/image admission can fail.
  target.batches.push_back(std::move(batch));auto& live=target.batches.back();
  buffer_storage_v41(live.indices,GL_ELEMENT_ARRAY_BUFFER,buffer_bytes_v41(part.indices.size(),sizeof(std::uint16_t)),part.indices.data(),GL_STATIC_DRAW,dh2::resources::ResourceScopeV37::world);
  if(!prepare_native_batch_shader_v113(part,live,e))return false;
 }
 target.pose_revision_v112=data->pose_revision_v112;target.uploaded=true;gpu.drawlist_registered_v69=false;if(actual.roots)actual.roots->force_register();e.clear();return true;
}catch(const std::exception& error){e=error.what();return false;}
bool release_source_batch_mesh_v111(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 if(!world||!id||!require_source_campaign_class_delivery_v106(world,e))return false;
 auto& gpu=source_geometry_gpu_v64;auto owner=gpu.world.lock();if(owner&&owner!=world){e="Batch GPU D1 addressed another campaign";return false;}
 const auto found=gpu.compiled.find(id);if(found==gpu.compiled.end()){e.clear();return true;} //Actual absent/previously context-lost allocation.
 const auto context=dh2::android_resources::budget_lease_v39()->context_state_v41();const bool lost=!context.ready||context.generation!=gpu.generation;
 for(auto& batch:found->second.batches){release_native_batch_shader_v113(batch,lost);release_buffer_v41(batch.vertices,lost);release_buffer_v41(batch.indices,lost);}
 gpu.registered_draws.erase(std::remove_if(gpu.registered_draws.begin(),gpu.registered_draws.end(),[&](const auto& value){return value.kind==SourceDrawSubmissionV69::compiled&&value.identity==id;}),gpu.registered_draws.end());gpu.compiled.erase(found);
 for(auto i=gpu.batch_programs_v113.begin();i!=gpu.batch_programs_v113.end();){if(i->second.use_count()!=1){++i;continue;}i->second->release(lost);i=gpu.batch_programs_v113.erase(i);}
 gpu.drawlist_registered_v69=false;e.clear();return true;
}
bool invalidate_source_batch_mesh_v111(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){return release_source_batch_mesh_v111(world,id,e);}
bool retire_source_campaign_batch_object_v113(const std::shared_ptr<void>& world,std::uintptr_t object,std::string& e){
 if(!world||!object||!require_source_campaign_class_delivery_v106(world,e))return false;
 SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_runtime_v61(actual,e)||actual.actual_world!=world)return false;
 //Clean may precede final map erase. Borrow the captured SAME segment owner;
 //no new object lookup, source visibility store or whole-pool frame pass.
 auto& gpu=source_geometry_gpu_v64;const auto context=dh2::android_resources::budget_lease_v39()->context_state_v41();const bool lost=!context.ready||context.generation!=gpu.generation;
 for(auto& entry:gpu.compiled){auto mesh=entry.second.source.lock();auto data=mesh?mesh->compiled_v111():nullptr;if(!mesh||!data){e="Retired actual compiled Clean owner";return false;}
  if(!data->retire_object_v113(*mesh,object,e))return false;
  for(std::size_t i=0;i<data->parts.size()&&i<entry.second.batches.size();++i)if(data->parts[i].native_retired_v113){auto& draw=entry.second.batches[i];release_native_batch_shader_v113(draw,lost);release_buffer_v41(draw.vertices,lost);release_buffer_v41(draw.indices,lost);}
 }
 gpu.drawlist_registered_v69=false;e.clear();return true;
}
bool finish_source_campaign_batch_object_retirement_v113(const std::shared_ptr<void>& world,std::uintptr_t object,std::string& e){
 if(!world||!object||!require_source_campaign_class_delivery_v106(world,e))return false;
 SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_runtime_v61(actual,e)||actual.actual_world!=world)return false;
 for(auto& entry:source_geometry_gpu_v64.compiled){auto mesh=entry.second.source.lock();auto data=mesh?mesh->compiled_v111():nullptr;if(!data){e="Actual compiled retirement completion owner expired";return false;}data->finish_object_retirement_v113(object);}
 e.clear();return true;
}
void restore_source_batch_uploads_v111(const SourceCampaignCandidateBorrowV55& actual){
 auto owner=actual.level?actual.level->batch_compiler_owner_slot_v96():nullptr;if(!owner||!owner->compiled0())return;
 const auto& root=owner->root34();if(!root.owner)return;
 //The source typed resource loan was produced by our constructor; validate
 //native identity/mesh130 before borrowing after a GL-context reset.
 auto node=root.native_v111;
 if(!node||node.get()!=root.owner.get()||node.owner_before(root.owner)||root.owner.owner_before(node)||node->identity()!=root.identity||!node->live()||!node->mesh()||node->mesh()->identity()!=root.mesh130)throw std::runtime_error("Replaced actual compiled root resource");
 auto mesh=node->mesh();auto existing=source_geometry_gpu_v64.compiled.find(mesh->identity());
 if(existing!=source_geometry_gpu_v64.compiled.end()){
  if(!existing->second.uploaded)throw std::runtime_error("Actual compiled GPU upload stopped within an admitted prefix; release required before replacement");
  return;
 }
 std::string e;
 if(!upload_source_batch_mesh_v111(actual.actual_world,mesh,source_geometry_gpu_v64.assets,e))throw std::runtime_error(e);
}
