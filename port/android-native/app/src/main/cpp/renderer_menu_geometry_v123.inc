// GPU transport only over the process preview's existing Character/Save/Gear.
// The retained scene owns animation, camera timing and object lifetimes.
SourceGeometryGpuV64 menu_preview_gpu_v123;
void retain_native_geometry_cleanup_owners_v125(std::set<std::uint32_t>& textures,std::set<std::uint32_t>& buffers){
 // These are allocation owners even before first render-list registration.
 // Retired/context-lost owners must finish their existing release path before
 // cleanup; snapshots never retain historical names or lookup-only caches.
 retain_native_geometry_resources_v125(source_geometry_gpu_v64,textures,buffers);
 retain_native_geometry_resources_v125(menu_preview_gpu_v123,textures,buffers);
}
void release_native_menu_geometry_v123(bool lost){release_geometry_gpu_v123(menu_preview_gpu_v123,lost);}
// Menu backdrop material transport. The limited model loader does not apply
// embedded ProfileCOMMON defaults or the original L1 light uniforms. Keep
// this correction on those backdrop batches; Character/Gear shaders, pose,
// animation and the World material path remain owned by their existing code.
// BEGIN menu-background-material-v124 host extraction
bool menu_background_material_color_v124(const dh2::resources::BresView& image,
 const dh2::scene::Material& material,std::array<float,4>& out,std::string& error){
 dh2::world::NativeBatchMaterialValuesV113 values;
 if(!dh2::world::decode_batch_material_values_v113(image,material.id,values,error))return false;
 if(material.effect_file.empty()){
  if(!dh2::world::decode_batch_effect_values_v113(image,material.effect_uri,"default",values,error))return false;
  const dh2::world::NativeBatchMaterialValueV113* diffuse{};
  for(const auto& entry:values.effect_defaults)if(entry.second.semantic==6){
   if(diffuse){error="Ambiguous authored menu diffuse-color semantic";return false;}
   const auto override=values.find(entry.first);diffuse=override==values.end()?&entry.second:&override->second;
  }
  if(!diffuse||(diffuse->source_type!=7&&diffuse->source_type!=16)||
     diffuse->element_counts!=std::vector<std::uint32_t>{1}||diffuse->bytes.size()!=sizeof(float)*4){
   error="Authored menu ProfileCOMMON diffuse-color producer absent";return false;
  }
  std::memcpy(out.data(),diffuse->bytes.data(),sizeof(float)*4);
 }else if(material.effect_file=="GL_Diffuse_L1_VC_iPhone.bdae"&&
          material.gles2_technique=="L1_Vc_Al_----_----_----_----"){
  const auto light=values.find("light0");
  if(light==values.end()||(light->second.source_type!=17&&light->second.source_type!=19)||
     light->second.light_urls!=std::vector<std::string>{"#"}){
   error="Authored menu L1 material needs its positive live light transport";return false;
  }
  // Source setMaterialParameter6324c4 leaves '#' NULL; commitLightParameter
  // 5b187c skips NULL. The fresh linked L1 program therefore has no ambient
  // or diffuse contribution. Its original GLES shader produces black foliage
  // while AlphaSampler BLUE still supplies opacity. This is the observed
  // source shadow branch, not a brightness multiplier or a fabricated light.
  out={0.f,0.f,0.f,1.f};
 }else {error="Unsupported authored menu backdrop material technique";return false;}
 for(float value:out)if(!std::isfinite(value)){error="Nonfinite authored menu material color";return false;}
 error.clear();return true;
}
// END menu-background-material-v124 host extraction
void prepare_menu_background_materials_v124(const std::shared_ptr<dh2::application::ApplicationServicesOwnerV5>& app){
 struct Receipt {
  std::chrono::steady_clock::time_point epoch;
  GLuint program{};
  std::vector<std::pair<GLuint,std::string>> batches;
 };
 static Receipt prepared;
 std::vector<std::pair<GLuint,std::string>> batches;
 for(const auto& batch:draws)batches.emplace_back(batch.vertices,batch.material.id);
 if(prepared.epoch==epoch&&prepared.program==program&&prepared.batches==batches)return;
 std::string error;const auto found=menu_renderer_domains_v121.find(app->identity());
 const auto domain=found==menu_renderer_domains_v121.end()?nullptr:found->second.lock();
 if(!domain||!domain->current(error))throw std::runtime_error(error.empty()?"Actual menu material source domain absent":error);
 const auto bytes=read(domain->assets,"main_menu_charactere_swamp.bdae","models");
 dh2::resources::BresView image{};
 if(dh2_bres_open(&image,bytes.data(),bytes.size())!=dh2::resources::BresError::ok)
  throw std::runtime_error("Authored menu material BRES rejected");
 // Resolve every original value before publishing any batch mutation.
 std::vector<std::array<float,4>> colors;colors.reserve(draws.size());
 for(const auto& batch:draws){
  if(!batch.skin.nodes.empty()||batch.native_shader_v113||batch.source_pass_v112)
   throw std::runtime_error("Menu backdrop material adapter addressed a different draw owner");
  std::array<float,4> color{};
  if(!menu_background_material_color_v124(image,batch.material,color,error))throw std::runtime_error(error);
  colors.push_back(color);
 }
 for(std::size_t i=0;i<draws.size();++i)std::copy(colors[i].begin(),colors[i].end(),draws[i].material.color);
 prepared={epoch,program,std::move(batches)};
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Original menu backdrop material defaults connected | batches %zu | ProfileCOMMON diffuse-color and NULL-light L1 shadows | Character/Gear unchanged",draws.size());
}
Matrix native_menu_projection_v123(){
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app;std::string error;
 if(!borrow_actual_application_services_v5(app,error))throw std::runtime_error(error);
 prepare_menu_background_materials_v124(app);
 dh2::camera::CameraViewV11 view;bool present{};
 if(!native_menu_preview_camera_view_v121(app,view,present,error))throw std::runtime_error(error);
 if(!present)throw std::runtime_error("Authored menu scene has no retained active preview camera");
 auto gpu_projection=view.projection;const dh2::camera::GpuProjectionFieldsV12 driver{0,0,0};
 dh2_camera_gpu_projection_v12(&gpu_projection,&driver);
 Matrix projection,view_matrix;std::copy_n(gpu_projection.values,16,projection.begin());
 std::copy_n(view.view.values,16,view_matrix.begin());
 submitted_projection_v113=projection;submitted_view=view_matrix;submitted_eye=view.eye;
 return dh2::scene::multiply(projection,view_matrix);
}
void draw_native_menu_character_v123(const Matrix& projection){
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app;std::string error;
 if(!borrow_actual_application_services_v5(app,error))throw std::runtime_error(error);
 const auto found=menu_renderer_domains_v121.find(app->identity());
 auto domain=found==menu_renderer_domains_v121.end()?nullptr:found->second.lock();
 if(!domain||!domain->current(error)||!app->source_objects_v121())
  throw std::runtime_error(error.empty()?"Required retained process menu render domain":error);
 auto& gpu=menu_preview_gpu_v123;
 if(gpu.world.lock()!=domain){
  release_native_menu_geometry_v123(false);gpu.world=domain;gpu.assets=domain->assets;
  gpu.original=std::make_unique<dh2::android_ui::OriginalCacheAssetsV1>(domain->assets);
 }
 const auto objects=app->source_objects_v121()->manager();std::set<std::uintptr_t> live;
 for(const auto identity:objects->characters()){
  std::shared_ptr<dh2::world::CanonicalCharacterCandidateRecordV60> record;bool matched{};
  if(!borrow_native_menu_preview_character_v122(domain,identity,record,matched,error))throw std::runtime_error(error);
  if(!matched)continue;
  if(!record||record->failed||!record->actor)throw std::runtime_error("Failed retained menu Character before GPU submission");
  // Requested front-end behavior: an Empty save slot has no displayed avatar.
  // The source lifecycle still owns its genuine fresh Character, but it must
  // not keep either that default body or a previous saved-slot GPU cache live.
  if(record->menu_preview_fresh_v122)continue;
  const auto visual=record->visual?record->visual->visual():nullptr;
  if(!record->actor->source_visual())continue;
  if(!visual||!visual->ready()||!visual->root_identity())throw std::runtime_error("Positive menu Character visual lacks retained scene root");
  live.insert(identity);auto& target=gpu.actors[identity];
  auto* gear=record->equipment?record->equipment.get():record->prepared_equipment_v60;
  const bool modular=gear!=nullptr;
  const std::vector<dh2::skinning::VisualDrawViewV32>* gear_parts_views{};
  if(target.record.lock()!=record||target.visual_identity!=record->actor->source_visual()||
     target.root_identity!=visual->root_identity()||target.modular!=modular){
   release_source_actor_gpu_v64(target,false);target.record=record;
   target.visual_identity=record->actor->source_visual();target.root_identity=visual->root_identity();target.modular=modular;
   if(!modular){target.retained=std::make_unique<dh2::world::RetainedVisualDrawV27>();
    if(!target.retained->bind(visual,error))throw std::runtime_error(error);}
  }
  if(modular){
   if(!gear->ready()||!gear->draw_views(gear_parts_views,error)||!gear_parts_views)
    throw std::runtime_error(error.empty()?"Required same restored menu Gear draw views":error);
   sync_visual_draws_v64(*gear_parts_views,gpu.assets,*gpu.original,target.batches,target.private_images,target.parts,target.mapping,
    dh2::resources::ResourceScopeV37::equipment,&gpu.texture_cache,&gpu.textures);
   target.visible.assign(target.batches.size(),(visual->scene_flags()&1u)?1:0);
  }else sync_source_retained_frames_v68(gpu,target,visual);
  const Draw* previous{};
  std::size_t submitted{};
  for(std::size_t i=0;i<target.batches.size();++i)if(target.visible[i]){
   submit_geometry_batch_v64(target.batches[i],dh2::scene::multiply(projection,target.batches[i].placement),previous);++submitted;
  }
  if(!target.menu_preview_trace_logged_v123){
   const auto class_id=record->save?record->save->class_id():-1;
   const auto* characters=record->design.characters();
   const char* class_name="<unresolved>";
   if(characters&&class_id>=0&&std::size_t(class_id)<characters->names.size())class_name=characters->names[std::size_t(class_id)].c_str();
   const auto* property_id=record->init_fields.properties_id13c8;
   const auto property=property_id?std::int32_t(*property_id):-1;
   const auto* model_name=record->actor->source_string(0x290);
   std::string gear_parts=modular?"[":"<absent>";
   if(modular&&gear_parts_views){
    const auto count=std::min<std::size_t>(gear_parts_views->size(),12);
    for(std::size_t i=0;i<count;++i){
     if(i)gear_parts+=',';
     gear_parts+=std::to_string((*gear_parts_views)[i].category)+":"+std::to_string((*gear_parts_views)[i].module)+":"+std::to_string((*gear_parts_views)[i].weapon_slot);
    }
    if(gear_parts_views->size()>count)gear_parts+="...";
    gear_parts+="]";
   }
   __android_log_print(ANDROID_LOG_INFO,"DH2Native",
    "Menu preview setup/draw | selected_slot %d | save_slot %d | fresh %d | PCLS %d:%s | Character13c8 %d | model_id %d | model %s | visual %s root %zu bres %zu | gear %s parts %zu modules %s | final_draw_owner %zu visual_owner %zu submitted %zu",
    record->menu_preview_selected_slot_v122,record->save?record->save->slot():-1,record->menu_preview_fresh_v122,
    class_id,class_name,property,record->properties?record->properties->resolved[3]:-1,
    model_name?model_name->c_str():"<unset>",visual->ready()?"ready":"not-ready",
    static_cast<std::size_t>(visual->root_identity()),visual->bres().size,modular&&gear->ready()?"ready":"absent",
    modular&&gear_parts_views?gear_parts_views->size():0,gear_parts.c_str(),static_cast<std::size_t>(identity),
    static_cast<std::size_t>(record->actor->source_visual()),submitted);
   target.menu_preview_trace_logged_v123=true;
  }
 }
 for(auto it=gpu.actors.begin();it!=gpu.actors.end();){
  if(live.count(it->first)){++it;continue;}release_source_actor_gpu_v64(it->second,false);it=gpu.actors.erase(it);
 }
}
