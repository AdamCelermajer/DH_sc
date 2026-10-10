// Included after V113 in the SAME renderer GPU domain. Source owns the node;
// this cache holds only weak node identity and admitted GL resource names.
bool prepare_source_skybox_v124(const SourceCampaignCandidateBorrowV55& actual,std::string& e)try{
 auto& gpu=source_geometry_gpu_v64;std::shared_ptr<dh2::camera::GameplaySkyboxPipelineV25> pipeline;
 if(!borrow_source_campaign_skybox_v124(actual.actual_world,pipeline,e))return false;
 auto node=pipeline?pipeline->current():nullptr;
 if(gpu.skybox_node_v124.lock()==node&&gpu.skybox_ready_v124){e.clear();return true;}
 if(node&&gpu.skybox_node_v124.lock()==node&&!gpu.skybox_ready_v124){e="Prior skybox GPU preparation failed; retained prefix requires release";return false;}
 for(auto& draw:gpu.skybox_draws_v124){release_native_batch_shader_v113(draw,false);release_buffer_v41(draw.vertices,false);release_buffer_v41(draw.indices,false);}
 gpu.skybox_draws_v124.clear();gpu.skybox_node_v124.reset();gpu.skybox_ready_v124=false;
 if(!node){gpu.skybox_ready_v124=true;actual.roots->force_register();e.clear();return true;}
 const auto resource=node->resource();if(!resource){e="Skybox actual resource retired before GPU preparation";return false;}
 // Retain each allocated prefix before any following callback can fail.
 gpu.skybox_node_v124=node;
 for(std::size_t i=0;i<resource->buffers.size();++i){
  dh2::world::NativeBatchPartV111 part;
  if(!dh2::camera::skybox_draw_source_v124(*pipeline,*resource,i,part,e))return false;
  gpu.skybox_draws_v124.emplace_back();auto& draw=gpu.skybox_draws_v124.back();
  draw.environment=true;draw.local_world_v124=true;draw.material=part.material;draw.source_pass_v112=part.pass_v112;
  draw.count=static_cast<GLsizei>(part.indices.size());
  buffer_storage_v41(draw.indices,GL_ELEMENT_ARRAY_BUFFER,buffer_bytes_v41(part.indices.size(),sizeof(std::uint16_t)),
   part.indices.data(),GL_STATIC_DRAW,dh2::resources::ResourceScopeV37::world);
  if(!prepare_native_batch_shader_v113(part,draw,e))return false;
 }
 gpu.skybox_ready_v124=true;actual.roots->force_register();e.clear();return true;
}catch(const std::exception& failure){e=failure.what();return false;}
bool submit_source_skybox_v124(std::size_t buffer,const Matrix& camera,std::string& e){
 auto& gpu=source_geometry_gpu_v64;auto node=gpu.skybox_node_v124.lock();
 SourceCampaignCandidateBorrowV55 actual;
 if(!node||buffer>=gpu.skybox_draws_v124.size()||!borrow_source_campaign_candidate_runtime_v61(actual,e)||
    !same_source_geometry_v64(actual)||!actual.camera_application){if(e.empty())e="Retired same-world skybox draw owner";return false;}
 std::shared_ptr<dh2::camera::GameplaySkyboxPipelineV25> pipeline;
 if(!borrow_source_campaign_skybox_v124(actual.actual_world,pipeline,e)||!pipeline||pipeline->current()!=node){
  if(e.empty())e="Skybox draw addressed a replaced source node";return false;}
 dh2::world::GameObjectSceneRootBorrowV1 root;
 if(!actual.roots->borrow_registered_root_v110(node->identity(),root,e))return false;
 if(!root.flags11c||!(*root.flags11c&1u)){e.clear();return true;}
 dh2::camera::CameraViewV11 view;if(!actual.camera_application->view(view,e))return false;
 auto& draw=gpu.skybox_draws_v124[buffer];dh2::camera::SkyboxRenderServicesV24 services;
 services.actual_driver_present=true;services.actual_active_camera_present=actual.roots->active_camera_v13().identity!=0;
 services.active_camera_position=[&](auto& out,auto& e){out=view.eye;e.clear();return true;};
 services.active_camera_near=[&](float& out,auto& e){out=view.near_plane;e.clear();return true;};
 services.set_world_transform=[&](const auto& matrix,auto& e){draw.placement=matrix;e.clear();return true;};
 services.set_material=[&](const auto& resource,const auto& source,auto& e){
  if(node->resource().get()!=&resource||&source!=&resource.buffers.at(buffer)||!draw.source_pass_v112||!draw.native_shader_v113){
   e="Skybox GPU material no longer matches source buffer";return false;}e.clear();return true;};
 services.set_shadow_cascade=[&](std::uint32_t cascade,auto& e){
  // This selected COMMON pass has no shadow-map sampler. Record the actual
  // node's cascade0 delivery in the driver submission, reject other cascades.
  if(cascade){e="Skybox requested unsupported nonzero shadow cascade";return false;}
  draw.shadow_cascade_v124=cascade;e.clear();return true;};
 services.draw_buffer=[&](const auto&,const auto&,auto& e){
  if(draw.shadow_cascade_v124){e="Skybox cascade state changed before submission";return false;}
  if(!submit_native_batch_shader_v113(draw,dh2::scene::multiply(camera,draw.placement),e))return false;
  ++dh2::perf::state.draws;return true;};
 return node->render(static_cast<std::uint32_t>(buffer+1),services,e);
}
