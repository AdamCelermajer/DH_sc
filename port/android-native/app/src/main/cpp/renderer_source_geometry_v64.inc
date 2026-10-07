// One GPU cache for actual ModuleDrawFrameV1 submissions. Matrices, materials,
// visibility and immutable BRES resources come from the retained loader graph.
struct SourceModuleMeshKeyV64 {
 std::shared_ptr<void> resource;
 std::uintptr_t root{};
 std::uint32_t instance{},geometry{};
};
struct SourceModuleMeshLessV64 {
 bool operator()(const SourceModuleMeshKeyV64& a,const SourceModuleMeshKeyV64& b)const{
  std::owner_less<std::shared_ptr<void>> less;
  if(less(a.resource,b.resource))return true;if(less(b.resource,a.resource))return false;
  if(a.root!=b.root)return a.root<b.root;
  if(a.instance!=b.instance)return a.instance<b.instance;
  return a.geometry<b.geometry;
 }
};
struct SourceModuleGpuV64 {std::vector<Draw> batches;};
struct SourceBatchGpuV111 {std::weak_ptr<dh2::world::NativeBatchMeshV110> source;std::vector<Draw> batches;bool uploaded{};std::uint64_t pose_revision_v112{};};
struct SourceActorGpuV64 {
 std::weak_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;
 std::uintptr_t visual_identity{},root_identity{};
 bool modular{},geometry_built{};
 std::unique_ptr<dh2::world::RetainedVisualDrawV27> retained;
 std::vector<Draw> batches;
 std::vector<GLuint> private_images;
 std::vector<dh2::skinning::VisualDrawViewV32> parts;
 std::vector<std::size_t> mapping;
 std::vector<std::uint8_t> visible;
};
// Native GPU transport of registered actual primitives. It holds lookup keys,
// never pointers into vectors that skin/equipment updates can replace.
struct SourceDrawSubmissionV69 {
 enum Kind {module,actor,object,compiled} kind{};
 SourceModuleMeshKeyV64 module_key;
 std::uintptr_t identity{};
 std::size_t primitive{};
 bool visible{true};
};
struct SourceGeometryGpuV64 {
 std::weak_ptr<void> world;
 std::weak_ptr<dh2::loader::CanonicalLevelContextV1> level;
 AAssetManager* assets{};
 std::unique_ptr<dh2::android_ui::OriginalCacheAssetsV1> original;
 std::map<SourceModuleMeshKeyV64,SourceModuleGpuV64,SourceModuleMeshLessV64> meshes;
 std::map<std::uintptr_t,SourceActorGpuV64> actors;
 std::map<std::uintptr_t,SourceActorGpuV64> objects;
 std::map<std::uintptr_t,SourceBatchGpuV111> compiled;
 std::map<std::string,std::shared_ptr<NativeBatchProgramV113>> batch_programs_v113;
 std::map<std::string,GLuint> texture_cache;
 std::vector<GLuint> textures;
 std::uint64_t generation{};
 std::vector<SourceDrawSubmissionV69> registered_draws;
 bool drawlist_registered_v69{};
 int viewport_width{},viewport_height{};
};
SourceGeometryGpuV64 source_geometry_gpu_v64;
void release_native_batch_shader_v113(Draw&,bool);
bool submit_native_batch_shader_v113(const Draw&,const Matrix&,std::string&);
bool update_native_batch_attributes_v113(const dh2::world::NativeBatchPartV111&,Draw&,std::string&);
void release_source_actor_gpu_v64(SourceActorGpuV64& actor,bool lost){
 for(auto& batch:actor.batches){release_buffer_v41(batch.vertices,lost);release_buffer_v41(batch.indices,lost);}
 for(auto image:actor.private_images)release_model_texture_v40(image,lost);
 actor={};
}
void release_source_geometry_gpu_v64(bool lost){
 auto& gpu=source_geometry_gpu_v64;
 for(auto& entry:gpu.meshes)for(auto& batch:entry.second.batches){
  release_buffer_v41(batch.vertices,lost);release_buffer_v41(batch.indices,lost);
 }
 for(auto& entry:gpu.actors)release_source_actor_gpu_v64(entry.second,lost);
 for(auto& entry:gpu.objects)release_source_actor_gpu_v64(entry.second,lost);
 for(auto& entry:gpu.compiled)for(auto& batch:entry.second.batches){release_native_batch_shader_v113(batch,lost);release_buffer_v41(batch.vertices,lost);release_buffer_v41(batch.indices,lost);}
 for(auto& entry:gpu.batch_programs_v113)entry.second->release(lost);
 for(auto texture:gpu.textures)release_model_texture_v40(texture,lost);
 gpu={};
}
bool same_source_geometry_v64(const SourceCampaignCandidateBorrowV55& actual){
 const auto& gpu=source_geometry_gpu_v64;
 const auto state=dh2::android_resources::budget_lease_v39()->context_state_v41();
 return gpu.assets&&gpu.original&&gpu.world.lock()==actual.actual_world&&
  gpu.level.lock()==actual.level&&state.ready&&state.generation==gpu.generation;
}
void build_source_module_mesh_v64(const dh2::loader::ModuleDrawFrameV1& frame,
 const dh2::loader::ModuleDrawMeshV1& source,SourceModuleGpuV64& out){
 using namespace dh2;std::string error;
 assets::Mesh mesh{};
 if(dh2_mesh_open(&mesh,&frame.bres,source.geometry)!=assets::Error::ok||
    mesh.primitives!=source.materials.size()||!mesh.vertices||mesh.vertices>65536)
  throw std::runtime_error("Actual Module geometry/material binding rejected");
 for(std::uint32_t j=0;j<mesh.primitives;++j){
  assets::Primitive primitive{};
  if(dh2_mesh_primitive(&mesh,j,&primitive)!=assets::Error::ok||primitive.collada_type||
     !primitive.index_count||primitive.index_count%3||primitive.index_count>3000000)
   throw std::runtime_error("Actual Module primitive topology rejected");
  assets::Attribute position_source{},uv_source{},color_source{};
  if(dh2_mesh_attribute(&mesh,primitive.attributes[0],&position_source)!=assets::Error::ok||
     position_source.components<3||position_source.components>4)
   throw std::runtime_error("Actual Module position stream unavailable");
  const bool uv=dh2_mesh_attribute(&mesh,primitive.attributes[4],&uv_source)==assets::Error::ok;
  const bool colors=dh2_mesh_attribute(&mesh,primitive.attributes[2],&color_source)==assets::Error::ok;
  if((uv&&(uv_source.components<2||uv_source.components>4))||
     (colors&&(!color_source.components||color_source.components>4)))
   throw std::runtime_error("Actual Module vertex stream width rejected");
  resources::CpuVectorCapacityV41 vertex_charge,index_charge;
  std::vector<Vertex> vertices;std::vector<std::uint16_t> indices;
  const auto budget=android_resources::budget_lease_v39();
  if(!resources::reserve_cpu_vector_v41(vertices,vertex_charge,budget,resources::ResourceScopeV37::world,mesh.vertices,error)||
     !resources::reserve_cpu_vector_v41(indices,index_charge,budget,resources::ResourceScopeV37::world,primitive.index_count,error))
   throw std::runtime_error(error);
  vertices.resize(mesh.vertices);indices.resize(primitive.index_count);
  Draw batch;batch.environment=true;batch.material=source.materials[j];batch.source_mesh_v111=source.source_mesh_v111;batch.source_primitive_v111=j;
  for(std::uint32_t i=0;i<mesh.vertices;++i){
   float value[4]{};
   if(!dh2_attribute_read(&position_source,i,value))throw std::runtime_error("Short actual Module position stream");
   for(unsigned axis=0;axis<3;++axis){
    if(!std::isfinite(value[axis]))throw std::runtime_error("Nonfinite actual Module position");
    vertices[i].p[axis]=value[axis];
   }
   batch.clip_bounds.include(vertices[i].p);
   if(uv){
    if(!dh2_attribute_read(&uv_source,i,value))throw std::runtime_error("Short actual Module UV stream");
    std::copy_n(value,2,vertices[i].uv);
   }
   if(colors){
    if(!dh2_attribute_read(&color_source,i,value))throw std::runtime_error("Short actual Module color stream");
    for(unsigned channel=0;channel<color_source.components;++channel)
     vertices[i].color[channel]=value[channel]/(color_source.type==1?255.f:1.f);
   }
  }
  for(std::uint32_t i=0;i<primitive.index_count;++i){
   std::uint32_t index{};
   if(!dh2_index_read(&primitive,i,&index)||index>=mesh.vertices)
    throw std::runtime_error("Actual Module triangle index outside vertex stream");
   indices[i]=static_cast<std::uint16_t>(index);
  }
  batch.count=static_cast<GLsizei>(indices.size());
  // Retain the reached GL prefix before any allocation can fail. The same
  // campaign/context teardown then releases every admitted name exactly once.
  out.batches.push_back(std::move(batch));auto& live=out.batches.back();
  buffer_storage_v41(live.vertices,GL_ARRAY_BUFFER,buffer_bytes_v41(vertices.size(),sizeof(Vertex)),vertices.data(),GL_STATIC_DRAW,resources::ResourceScopeV37::world);
  buffer_storage_v41(live.indices,GL_ELEMENT_ARRAY_BUFFER,buffer_bytes_v41(indices.size(),sizeof(std::uint16_t)),indices.data(),GL_STATIC_DRAW,resources::ResourceScopeV37::world);
 }
}
void prepare_source_module_frames_v64(const std::vector<dh2::loader::ModuleDrawFrameV1>& frames){
 auto& gpu=source_geometry_gpu_v64;
 for(auto it=gpu.meshes.begin();it!=gpu.meshes.end();){
  const auto& key=it->first;std::owner_less<std::shared_ptr<void>> less;
  const bool retained=std::any_of(frames.begin(),frames.end(),[&](const auto& f){
   return f.root_identity==key.root&&!less(f.resource,key.resource)&&!less(key.resource,f.resource);
  });
  if(retained){++it;continue;}
  std::vector<GLuint> no_textures;release(it->second.batches,no_textures);it=gpu.meshes.erase(it);
 }
 for(const auto& frame:frames){
  if(!frame.resource||!frame.root_identity)throw std::runtime_error("Actual Module draw resource/root unavailable");
  for(const auto& mesh:frame.meshes){
   SourceModuleMeshKeyV64 key{frame.resource,frame.root_identity,mesh.source_instance,mesh.geometry};
   auto found=gpu.meshes.find(key);
   if(found==gpu.meshes.end()){
    found=gpu.meshes.emplace(std::move(key),SourceModuleGpuV64{}).first;
    try{build_source_module_mesh_v64(frame,mesh,found->second);}
    catch(...){std::vector<GLuint> none;release(found->second.batches,none);gpu.meshes.erase(found);throw;}
   }
   if(found->second.batches.size()!=mesh.materials.size())throw std::runtime_error("Actual Module material cardinality changed");
   for(std::size_t i=0;i<mesh.materials.size();++i){auto& batch=found->second.batches[i];
    batch.material=mesh.materials[i];batch.placement=mesh.cached_world;
    batch.diffuse=upload(gpu.assets,batch.material.diffuse,gpu.texture_cache,gpu.textures,gpu.original.get());
    batch.alpha=upload(gpu.assets,batch.material.alpha_map,gpu.texture_cache,gpu.textures,gpu.original.get());
   }
  }
 }
}
void sync_source_retained_frames_v68(SourceActorGpuV64& target,
 const std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>& visual){
 auto& gpu=source_geometry_gpu_v64;std::string error;
  if(!target.retained->refresh(error))throw std::runtime_error(error);
  const auto& parts=target.retained->parts();
  const auto& materials=visual->scene().materials;
  const bool build=!target.geometry_built;
  if(!build&&target.batches.size()!=parts.size())throw std::runtime_error("Actual Character visual topology changed within a live root");
  if(build)target.batches.resize(parts.size());
  target.visible.resize(parts.size());
  for(std::size_t i=0;i<parts.size();++i){
   const auto& part=parts[i];auto& batch=target.batches[i];
   if(part.material>=materials.size())throw std::runtime_error("Actual Character material index outside retained visual");
   batch.material=materials[part.material];batch.placement=part.matrix;batch.environment=true;
   batch.source_mesh_v111=part.source_mesh_v111;batch.source_primitive_v111=part.source_primitive_v111;
   target.visible[i]=part.visible?1:0;
   if(build){
    if(part.vertices.empty()||part.indices.empty()||part.indices.size()>3000000)
     throw std::runtime_error("Actual Character primitive outside native GPU domain");
    if(!dh2::resources::reserve_cpu_vector_v41(batch.cpu_geometry_v41.vertices,batch.cpu_geometry_v41.capacity,
       dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,part.vertices.size(),error))
     throw std::runtime_error(error);
    batch.cpu_geometry_v41.vertices.resize(part.vertices.size());
    batch.count=static_cast<GLsizei>(part.indices.size());
   }
   if(batch.cpu_geometry_v41.vertices.size()!=part.vertices.size())throw std::runtime_error("Actual Character skin vertex count changed");
   bool changed=build;batch.clip_bounds={};
   for(std::size_t j=0;j<part.vertices.size();++j){
    auto& vertex=batch.cpu_geometry_v41.vertices[j];const auto& source=part.vertices[j];
    changed=changed||std::memcmp(vertex.p,source.position,sizeof(vertex.p))!=0;
    std::copy_n(source.position,3,vertex.p);batch.clip_bounds.include(vertex.p);
    if(build){std::copy_n(source.uv,2,vertex.uv);std::copy_n(source.color,4,vertex.color);}
   }
   batch.diffuse=upload(gpu.assets,batch.material.diffuse,gpu.texture_cache,gpu.textures,gpu.original.get());
   batch.alpha=upload(gpu.assets,batch.material.alpha_map,gpu.texture_cache,gpu.textures,gpu.original.get());
   if(build){
    buffer_storage_v41(batch.vertices,GL_ARRAY_BUFFER,buffer_bytes_v41(batch.cpu_geometry_v41.vertices.size(),sizeof(Vertex)),
     batch.cpu_geometry_v41.vertices.data(),part.skinned?GL_DYNAMIC_DRAW:GL_STATIC_DRAW,dh2::resources::ResourceScopeV37::world);
    buffer_storage_v41(batch.indices,GL_ELEMENT_ARRAY_BUFFER,buffer_bytes_v41(part.indices.size(),sizeof(std::uint16_t)),
     part.indices.data(),GL_STATIC_DRAW,dh2::resources::ResourceScopeV37::world);
   }else if(changed)buffer_subdata_v41(batch.vertices,GL_ARRAY_BUFFER,buffer_bytes_v41(batch.cpu_geometry_v41.vertices.size(),sizeof(Vertex)),
     batch.cpu_geometry_v41.vertices.data(),dh2::resources::ResourceScopeV37::world);
  }
  target.geometry_built=true;
}
void prepare_source_actor_frames_v64(const SourceCampaignCandidateBorrowV55& actual){
 auto& gpu=source_geometry_gpu_v64;std::string error;
 const auto manager=actual.application->source_player_manager_v59();
 dh2::player::PlayerInfoFieldsV1* local{};
 if(!manager||!manager->get_local_player(0,false,local,error)||!local||!local->character660)
  throw std::runtime_error(error.empty()?"Required actual selected PlayerManager Character660 before scene rendering":error);
 const auto player_identity=local->character660;
 const auto& characters=actual.objects->characters();
 if(std::find(characters.begin(),characters.end(),player_identity)==characters.end())
  throw std::runtime_error("Selected Character660 is absent from same canonical character directory");
 for(auto it=gpu.actors.begin();it!=gpu.actors.end();){
  if(std::find(characters.begin(),characters.end(),it->first)!=characters.end()){++it;continue;}
  release_source_actor_gpu_v64(it->second,false);it=gpu.actors.erase(it);
 }
 for(const auto identity:characters){
  SourceCampaignCharacterBorrowV62 borrowed;
  if(!borrow_source_campaign_character_v62(actual.actual_world,identity,borrowed,error))throw std::runtime_error(error);
  const auto& record=borrowed.character;
  if(!record||!record->actor||record->failed)
   throw std::runtime_error("Canonical Character initialization failed before render publication");
  const auto visual=record->visual?record->visual->visual():nullptr;
  const bool present=record->actor->source_visual()!=0;
  if(!present){
   // A genuinely absent source visual contributes no meshes. The selected
   // player must have its real loaded visual, equipment and profile.
   if(identity==player_identity)throw std::runtime_error("Selected player has no actual visual2d8");
   auto old=gpu.actors.find(identity);if(old!=gpu.actors.end()){release_source_actor_gpu_v64(old->second,false);gpu.actors.erase(old);}
   continue;
  }
  if(!visual||!visual->ready()||!visual->root_identity())
   throw std::runtime_error("Positive canonical Character visual lacks actual retained root");
  auto* gear=record->equipment?record->equipment.get():record->prepared_equipment_v60;
  const bool modular=gear!=nullptr;
  if(identity==player_identity&&(!modular||!gear->ready()||!record->save))
   throw std::runtime_error("Selected player needs same restored Save/Gear before GPU publication");
  auto& target=gpu.actors[identity];
  if(target.record.lock()!=record||target.visual_identity!=record->actor->source_visual()||
     target.root_identity!=visual->root_identity()||target.modular!=modular){
   release_source_actor_gpu_v64(target,false);
   target.record=record;target.visual_identity=record->actor->source_visual();
   target.root_identity=visual->root_identity();target.modular=modular;
   if(!modular){
    target.retained=std::make_unique<dh2::world::RetainedVisualDrawV27>();
    if(!target.retained->bind(visual,error))throw std::runtime_error(error);
   }
  }
  if(modular){
   const std::vector<dh2::skinning::VisualDrawViewV32>* parts{};
   if(!gear->ready()||!gear->draw_views(parts,error)||!parts)
    throw std::runtime_error(error.empty()?"Required same live Character Gear skin views":error);
   sync_visual_draws_v64(*parts,gpu.assets,*gpu.original,target.batches,target.private_images,target.parts,target.mapping,
    dh2::resources::ResourceScopeV37::equipment,&gpu.texture_cache,&gpu.textures);
   target.visible.assign(target.batches.size(),(visual->scene_flags()&1u)?1:0);
   continue;
  }
  sync_source_retained_frames_v68(target,visual);
 }
}
void submit_geometry_batch_v64(const Draw& b,const Matrix& transform,const Draw*& previous){
 if(dh2::render::outside_clip_v35(b.clip_bounds,transform)){++dh2::perf::state.culled;return;}
 if(b.native_shader_v113){std::string e;if(!submit_native_batch_shader_v113(b,transform,e))throw std::runtime_error(e);++dh2::perf::state.draws;previous=&b;return;}
 if(previous&&previous->native_shader_v113){glUseProgram(program);glEnableVertexAttribArray(position);glEnableVertexAttribArray(texcoord);glEnableVertexAttribArray(color);glUniform1i(diffuse_uniform_v35,0);glUniform1i(alpha_uniform_v35,1);glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL);glEnable(GL_BLEND);glBlendEquation(GL_FUNC_ADD);glCullFace(GL_BACK);glFrontFace(GL_CCW);previous=nullptr;}
 ++dh2::perf::state.draws;glUniformMatrix4fv(mvp,1,GL_FALSE,transform.data());
 const auto* old=previous;
 if(b.source_pass_v112){const auto& pass=*b.source_pass_v112;
  pass.blend?glEnable(GL_BLEND):glDisable(GL_BLEND);glBlendFunc(pass.blend_src,pass.blend_dst);glBlendEquation(pass.blend_equation);
  pass.depth?glEnable(GL_DEPTH_TEST):glDisable(GL_DEPTH_TEST);glDepthFunc(pass.depth_function);glDepthMask(pass.depth_write?GL_TRUE:GL_FALSE);
  pass.cull?glEnable(GL_CULL_FACE):glDisable(GL_CULL_FACE);glCullFace(pass.cull_face);glFrontFace(pass.front_face);
 }else{
  const bool restore=old&&old->source_pass_v112.has_value();
  if(restore){glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL);glEnable(GL_BLEND);glBlendEquation(GL_FUNC_ADD);glCullFace(GL_BACK);glFrontFace(GL_CCW);}
  if(!old||restore||old->material.backface!=b.material.backface)b.material.backface?glEnable(GL_CULL_FACE):glDisable(GL_CULL_FACE);
  if(!old||restore||old->material.additive!=b.material.additive){glBlendFunc(GL_SRC_ALPHA,b.material.additive?GL_ONE:GL_ONE_MINUS_SRC_ALPHA);glDepthMask(b.material.additive?GL_FALSE:GL_TRUE);}
 }
 if(!old||std::memcmp(old->material.texture_matrix,b.material.texture_matrix,sizeof(b.material.texture_matrix)))glUniformMatrix4fv(texture_matrix,1,GL_FALSE,b.material.texture_matrix);
 if(!old||std::memcmp(old->material.color,b.material.color,sizeof(b.material.color)))glUniform4fv(material_color,1,b.material.color);
 if(!old||old->material.alpha_map.empty()!=b.material.alpha_map.empty())glUniform1f(has_alpha,b.material.alpha_map.empty()?0:1);
 if(!old||old->material.alpha_ref!=b.material.alpha_ref)glUniform1f(alpha_ref,b.material.alpha_ref);
 const auto blue=[](const Draw& d){return d.material.effect_file=="GL_Diffuse_L1_VC_iPhone.bdae"&&d.material.gles2_technique=="L1_Vc_Al_----_----_----_----";};
 if(!old||blue(*old)!=blue(b))glUniform1f(original_alpha_blue,blue(b)?1:0);
 if(!old||old->diffuse!=b.diffuse){glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,b.diffuse);}
 if(!old||old->alpha!=b.alpha){glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,b.alpha);}
 glBindBuffer(GL_ARRAY_BUFFER,b.vertices);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,b.indices);
 glVertexAttribPointer(position,3,GL_FLOAT,GL_FALSE,b.vertex_stride_v111,nullptr);
 glVertexAttribPointer(texcoord,2,b.uv_type_v111,GL_FALSE,b.vertex_stride_v111,reinterpret_cast<void*>(b.uv_offset_v111));
 glVertexAttribPointer(color,4,GL_FLOAT,GL_FALSE,b.vertex_stride_v111,reinterpret_cast<void*>(b.color_offset_v111));
 glDrawElements(GL_TRIANGLES,b.count,GL_UNSIGNED_SHORT,nullptr);previous=&b;
}

void prepare_source_object_frames_v68(){
 std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>> visuals;std::string error;
 if(!capture_source_campaign_objects_v68(visuals,error))throw std::runtime_error(error);
 SourceCampaignCandidateBorrowV55 campaign;std::vector<std::shared_ptr<dh2::world::RetainedGameObjectVisualV1>> items;
 if(!borrow_source_campaign_candidate_v55(campaign,error)||!capture_source_campaign_item_visuals_v104(campaign.actual_world,items,error))throw std::runtime_error(error);
 visuals.insert(visuals.end(),items.begin(),items.end());
 auto& gpu=source_geometry_gpu_v64;std::set<std::uintptr_t> submitted;
 for(const auto& visual:visuals){
  if(!visual||!visual->ready()||!visual->root_identity())throw std::runtime_error("Actual object visual has no retained scene root");
  const auto id=reinterpret_cast<std::uintptr_t>(visual.get());if(!submitted.insert(id).second)continue;
  auto& target=gpu.objects[id];
  if(!target.retained||target.retained->visual()!=visual||target.root_identity!=visual->root_identity()){
   release_source_actor_gpu_v64(target,false);target.visual_identity=id;target.root_identity=visual->root_identity();
   target.retained=std::make_unique<dh2::world::RetainedVisualDrawV27>();
   if(!target.retained->bind(visual,error))throw std::runtime_error(error);
  }
  // Source actor/update owns animation and physics. Submission only reads
  // current skin/static vertices, visibility, materials and world transforms.
  sync_source_retained_frames_v68(target,visual);
 }
 for(auto at=gpu.objects.begin();at!=gpu.objects.end();){
  if(!submitted.count(at->first)){release_source_actor_gpu_v64(at->second,false);at=gpu.objects.erase(at);}
  else ++at;
 }
}

void clear_source_drawlist_transport_v69(){
 source_geometry_gpu_v64.registered_draws.clear();source_geometry_gpu_v64.drawlist_registered_v69=false;
}
bool source_primitive_compiled_v111(const Draw& original){
 auto source=original.source_mesh_v111.lock();if(!source)return false;
 //GPU membership is derived from the SAME admitted compiled resource, not
 //GameObject visibility or a guessed original BaseMeshSceneNode byte138.
 //Weak control-block identity prevents suppressing a later reused address.
 for(const auto& entry:source_geometry_gpu_v64.compiled){
  auto mesh=entry.second.source.lock();auto data=mesh?mesh->compiled_v111():nullptr;auto root=data?data->root.lock():nullptr;
  if(!mesh||!mesh->live()||!root||!root->live()||!entry.second.uploaded||entry.second.batches.size()!=data->parts.size())continue;
  for(const auto& part:data->parts){auto actual=part.source_mesh_v111.lock();
   if(actual==source&&!actual.owner_before(source)&&!source.owner_before(actual)&&part.source_primitive_v111==original.source_primitive_v111)return true;
  }
 }
 return false;
}
void rebuild_source_drawlist_transport_v69(){
 auto& gpu=source_geometry_gpu_v64;std::vector<SourceDrawSubmissionV69> submissions;std::string error_v111;
 for(const auto& module:gpu.meshes)for(std::size_t i=0;i<module.second.batches.size();++i)
  if(!source_primitive_compiled_v111(module.second.batches[i]))submissions.push_back({SourceDrawSubmissionV69::module,module.first,0,i});
 for(const auto& actor:gpu.actors)for(std::size_t i=0;i<actor.second.batches.size();++i)
  if(actor.second.visible.at(i)&&!source_primitive_compiled_v111(actor.second.batches[i]))submissions.push_back({SourceDrawSubmissionV69::actor,{},actor.first,i});
 for(const auto& object:gpu.objects)for(std::size_t i=0;i<object.second.batches.size();++i)
  if(object.second.visible.at(i)&&!source_primitive_compiled_v111(object.second.batches[i]))submissions.push_back({SourceDrawSubmissionV69::object,{},object.first,i});
 for(const auto& entry:gpu.compiled){auto mesh=entry.second.source.lock();auto data=mesh?mesh->compiled_v111():nullptr;auto root=data?data->root.lock():nullptr;
  if(!mesh||!mesh->live()||!root||!root->live()||!entry.second.uploaded||entry.second.batches.size()!=data->parts.size())throw std::runtime_error("Compiled root GPU resource expired or upload prefix incomplete before registration");
  if(!root->source_visible_v111())continue;
  //Register the actual batch node's material buffers once. Its source
  //segments can change GameObject visibility without replacing this draw key.
  for(std::size_t i=0;i<entry.second.batches.size();++i)submissions.push_back({SourceDrawSubmissionV69::compiled,{},entry.first,i});
 }
 gpu.registered_draws=std::move(submissions);gpu.drawlist_registered_v69=true;
}
void refresh_source_drawlist_transport_v69(){
 auto& gpu=source_geometry_gpu_v64;
 if(!gpu.drawlist_registered_v69)throw std::runtime_error("Cached source scene list has not been registered");
 for(auto& submission:gpu.registered_draws){
  if(submission.kind==SourceDrawSubmissionV69::module){
   const auto found=gpu.meshes.find(submission.module_key);
   if(found==gpu.meshes.end()||submission.primitive>=found->second.batches.size())
    throw std::runtime_error("Cached source module primitive was retired before registration");
   submission.visible=true; // Actual prepared Module capture contains admitted meshes.
  }else if(submission.kind==SourceDrawSubmissionV69::compiled){
   auto found=gpu.compiled.find(submission.identity);auto mesh=found==gpu.compiled.end()?nullptr:found->second.source.lock();auto data=mesh?mesh->compiled_v111():nullptr;auto root=data?data->root.lock():nullptr;std::string e;
   if(!mesh||!root||submission.primitive>=found->second.batches.size()||!data->visible||!data->visible(data->parts.at(submission.primitive).segment,submission.visible,e))throw std::runtime_error(e.empty()?"Retired compiled source draw":e);
   submission.visible=submission.visible&&root->source_visible_v111();
  }else{
   const auto& directory=submission.kind==SourceDrawSubmissionV69::actor?gpu.actors:gpu.objects;
   const auto found=directory.find(submission.identity);
   if(found==directory.end()||submission.primitive>=found->second.batches.size()||
      submission.primitive>=found->second.visible.size())
    throw std::runtime_error("Cached source object primitive was retired before registration");
   submission.visible=found->second.visible[submission.primitive]!=0;
  }
 }
}
bool source_inventory_geometry_v70(const dh2::visual::Root*& root,
 const std::vector<Draw>*& draws,std::shared_ptr<void>& owner,std::string& error){
 root=nullptr;draws=nullptr;owner.reset();SourceCampaignCandidateBorrowV55 actual;
 PlayerGameplayBinding player;SourceCampaignCharacterBorrowV62 selected;
 if(!borrow_source_campaign_candidate_v55(actual,error)||!same_source_geometry_v64(actual)||
    !borrow_source_campaign_player_gameplay_v67(actual.actual_world,player,error)||
    !borrow_source_campaign_character_v62(actual.actual_world,player.character,selected,error)){
  if(error.empty())error="Inventory requires current native campaign/player/GPU owners";return false;
 }
 const auto visual=selected.character->visual?selected.character->visual->visual():nullptr;
 if(!visual||!visual->ready()||!visual->root_identity()||selected.character->equipment.get()!=player.gear){
  error="Inventory requires selected Character's actual visual and saved Gear";return false;
 }
 // Opening the pane still runs the ONE source scene phase for this native
 // delivery. It samples the real animator and never steps AI/physics or loads
 // a preview character. World and avatar panes share that phase guard.
 if(!advance_source_scene_v69(error)||!visual->sync_position_v7(error))return false;
 const std::vector<dh2::skinning::VisualDrawViewV32>* parts{};
 if(!player.gear->draw_views(parts,error)||!parts)return false;
 auto& gpu=source_geometry_gpu_v64;auto& target=gpu.actors[player.character];
 if(target.record.lock()!=selected.character||target.visual_identity!=selected.character->actor->source_visual()||
    target.root_identity!=visual->root_identity()||!target.modular){
  release_source_actor_gpu_v64(target,false);target.record=selected.character;
  target.visual_identity=selected.character->actor->source_visual();target.root_identity=visual->root_identity();target.modular=true;
 }
 sync_visual_draws_v64(*parts,gpu.assets,*gpu.original,target.batches,target.private_images,target.parts,target.mapping,
  dh2::resources::ResourceScopeV37::equipment,&gpu.texture_cache,&gpu.textures);
 target.visible.assign(target.batches.size(),(visual->scene_flags()&1u)?1:0);
 owner=selected.character;root=&visual->binding().root;draws=&target.batches;error.clear();return true;
}
void submit_source_drawlist_transport_v69(const Matrix& camera,const Draw*& previous){
 auto& gpu=source_geometry_gpu_v64;
 //The registered source Scene phase already advanced the retained animator.
 //Copy only its current dynamic ranges into the SAME admitted GPU buffers.
 for(auto& entry:gpu.compiled){auto mesh=entry.second.source.lock();auto data=mesh?mesh->compiled_v111():nullptr;auto& target=entry.second;
  if(!data||!target.uploaded||target.batches.size()!=data->parts.size())throw std::runtime_error("Retired or incomplete compiled dynamic GPU receiver");
  if(target.pose_revision_v112==data->pose_revision_v112)continue;
  for(std::size_t i=0;i<data->parts.size();++i){const auto& part=data->parts[i];if(!part.dynamic_v112||part.native_retired_v113)continue;auto& draw=target.batches[i];draw.clip_bounds={};std::string e;
   if(draw.native_shader_v113){if(!update_native_batch_attributes_v113(part,draw,e))throw std::runtime_error(e);for(const auto& v:part.vertices)draw.clip_bounds.include(v.position);continue;}
   if(!part.quantized_uv.empty()){
    struct Packed {float position[3];std::int16_t uv[2];float color[4];};dh2::resources::CpuVectorCapacityV41 charge;std::vector<Packed> vertices;
    if(!dh2::resources::reserve_cpu_vector_v41(vertices,charge,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,part.vertices.size(),e))throw std::runtime_error(e);
    vertices.resize(part.vertices.size());
    for(std::size_t v=0;v<vertices.size();++v){std::copy_n(part.vertices[v].position,3,vertices[v].position);std::copy_n(part.quantized_uv.at(v).data(),2,vertices[v].uv);std::copy_n(part.vertices[v].color,4,vertices[v].color);draw.clip_bounds.include(vertices[v].position);}
    buffer_subdata_v41(draw.vertices,GL_ARRAY_BUFFER,buffer_bytes_v41(vertices.size(),sizeof(Packed)),vertices.data(),dh2::resources::ResourceScopeV37::world);
   }else{
    dh2::resources::CpuVectorCapacityV41 charge;std::vector<Vertex> vertices;
    if(!dh2::resources::reserve_cpu_vector_v41(vertices,charge,dh2::android_resources::budget_lease_v39(),dh2::resources::ResourceScopeV37::world,part.vertices.size(),e))throw std::runtime_error(e);vertices.resize(part.vertices.size());
    for(std::size_t v=0;v<vertices.size();++v){std::copy_n(part.vertices[v].position,3,vertices[v].p);std::copy_n(part.vertices[v].uv,2,vertices[v].uv);std::copy_n(part.vertices[v].color,4,vertices[v].color);draw.clip_bounds.include(vertices[v].p);}
    buffer_subdata_v41(draw.vertices,GL_ARRAY_BUFFER,buffer_bytes_v41(vertices.size(),sizeof(Vertex)),vertices.data(),dh2::resources::ResourceScopeV37::world);
   }
  }
  target.pose_revision_v112=data->pose_revision_v112;
 }
 if(!gpu.drawlist_registered_v69)throw std::runtime_error("Actual scene primitive list has not been registered");
 for(const auto& submission:gpu.registered_draws){
  if(!submission.visible&&submission.kind!=SourceDrawSubmissionV69::compiled)continue;
  const Draw* batch{};
  if(submission.kind==SourceDrawSubmissionV69::module){
   const auto found=gpu.meshes.find(submission.module_key);
   if(found==gpu.meshes.end()||submission.primitive>=found->second.batches.size())
    throw std::runtime_error("Registered actual module primitive retired before scene registration");
   batch=&found->second.batches[submission.primitive];
  }else if(submission.kind==SourceDrawSubmissionV69::compiled){
   auto found=gpu.compiled.find(submission.identity);auto mesh=found==gpu.compiled.end()?nullptr:found->second.source.lock();auto data=mesh?mesh->compiled_v111():nullptr;auto root=data?data->root.lock():nullptr;std::string error;bool visible;
   if(!mesh||!root||submission.primitive>=found->second.batches.size()||!data->visible||!data->visible(data->parts.at(submission.primitive).segment,visible,error))throw std::runtime_error(error.empty()?"Registered compiled resource retired":error);
   if(!visible||!root->source_visible_v111())continue;batch=&found->second.batches[submission.primitive];
  }else{
   const auto& directory=submission.kind==SourceDrawSubmissionV69::actor?gpu.actors:gpu.objects;
   const auto found=directory.find(submission.identity);
   if(found==directory.end()||submission.primitive>=found->second.batches.size())
    throw std::runtime_error("Registered actual object primitive retired before scene registration");
   // Read current source visibility/pose even when source registration is cached.
   if(!found->second.visible.at(submission.primitive))continue;
   batch=&found->second.batches[submission.primitive];
  }
  if(submission.kind!=SourceDrawSubmissionV69::compiled&&source_primitive_compiled_v111(*batch))continue;
  submit_geometry_batch_v64(*batch,dh2::scene::multiply(camera,batch->placement),previous);
 }
}
