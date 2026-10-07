// Include INSIDE model_renderer namespace in source_campaign_runtime_v61.cpp.
// Requires player_light_tweaker_owner_v90.hpp and current Stage34/Config/Scene.
// Register on the existing App at World enrollment BEFORE loader snapshots.
static bool install_campaign_player_light_tweakers_v90(
 const std::shared_ptr<SourceWorldBorrowV61>& world,std::string& e){
 using dh2::application::PlayerLightTweakerOwnerV90;
 if(!world||!world->application||!world->read||!world->files_owner||!world->canonical_world||!world->canonical_world->scene_roots_v20){e="Required actual App/files/scene before original tweaker PostInit";return false;}
 const auto names=world->canonical_world->scene_roots_v20->source_light_names_v89();
 if(!names){e="Required actual original LightSet names owner";return false;}
 // Original PostInit32f90c allocates -> C1 -> publishes58+4*i -> assignsf8.
 // Do not create another owner on return visit. Failed assignment retains
 // the already published C1 prefix and retries the same receiver.
 for(std::size_t i=0;i<4;++i){
  auto owner=world->application->source_tweaker58_v90(i);
  if(!owner){owner=std::make_shared<PlayerLightTweakerOwnerV90>();
   if(!owner->construct(world->read,e)||!world->application->publish_source_tweaker58_v90(i,owner,e))return false;
  }
  if(!owner->constructed()||!owner->assign_light_type_after_publication(names->source_names()[i],e))return false;
 }
 auto bag=world->stage34_native_v80?world->stage34_native_v80:std::make_shared<dh2::loader::Stage34ServicesV1>();
 if(!bag->owner)bag->owner=world->files_owner;
 if(!bag->fog_params58)bag->fog_params58=[weak=std::weak_ptr<SourceWorldBorrowV61>(world)](
  const std::shared_ptr<void>& app,dh2::loader::Stage34FogParamsV1& out,std::string& e){
  auto w=weak.lock();
  if(!w||!w->application||app.get()!=w->application.get()||app.owner_before(w->application)||w->application.owner_before(app)){e="Foreign App58 tweaker request";return false;}
  auto owner=w->application->source_tweaker58_v90();if(!owner||!owner->constructed()){e="Required SAME actual App58 PlayerLightTweaker";return false;}
  out={owner,owner->identity(),owner->fog_color_f4(),owner->fog_start110(),owner->fog_end114(),owner->fog_direction11c(),{}};
  out.source_fog_written_v90=[weak,typed=std::weak_ptr<PlayerLightTweakerOwnerV90>(owner)](std::string& e){
   auto w=weak.lock();auto value=typed.lock();
   if(!w||!value||value!=w->application->source_tweaker58_v90()||!value->constructed()){e="App58 changed during authentic startup field stores";return false;}
   value->source_fog_written();e.clear();return true;
  };e.clear();return true;
 };
 world->stage34_native_v80=std::move(bag);e.clear();return true;
}
// Public native setValue composition. This lends primitives only; it creates
// no attribute receiver, registration map, Scene, Config or rendering owner.
// Caller retains the actual attributes receipt and passes this bag directly
// to the SAME application's owner->set_value(attrs,index,bag,error).
bool bind_campaign_player_light_tweak_services_v90(
 const std::shared_ptr<SourceWorldBorrowV61>& source_world,
 dh2::application::PlayerLightTweakServicesV90& out,std::string& e){
 using namespace dh2;using namespace dh2::loader;
 if(!source_world||!source_world->application||!source_world->files_owner){e="Required actual current tweaker source authority";return false;}
 const std::weak_ptr<SourceWorldBorrowV61> weak=source_world;
 const auto scope=[weak](auto& w,std::string& e){
  w=weak.lock();if(!w||!w->application||!w->canonical_world){e="Actual App/Scene tweaker scope retired";return false;}
  std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app;
  if(!borrow_actual_application_services_v5(app,e)||app!=w->application){if(e.empty())e="Tweaker source scope belongs to another actual App";return false;}return true;
 };
 const auto scene=[scope](std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1>& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(w,e))return false;
  out=w->canonical_world->scene_roots_v20;if(!out){e="Required same actual App10.scene1c";return false;}return true;
 };
 const auto config=[scope](const std::shared_ptr<CanonicalLevelContextV1>& level,std::shared_ptr<dh2::world::CanonicalLevelConfigV1>& out,std::string& e){
  std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(w,e)||!level){e="Required cached actual GetCurrentLevel receiver";return false;}
  CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e)||current.level()!=level){if(e.empty())e="Cached tweaker Level retired before GetLevelConfig";return false;}
  auto fields=level->constructor_borrow_v3();if(!fields.config38||!*fields.config38){e="Original Level.GetLevelConfig NULL assertion edge";return false;}
  auto bag=w->stage34_native_v80;
  if(!bag||!bag->config){e="Required existing published canonical LevelConfig borrower";return false;}
  if(!bag->config(*fields.config38,out,e)||!out||out->identity()!=*fields.config38){if(e.empty())e="Foreign actual LevelConfig38 tweaker receiver";return false;}return true;
 };
 dh2::application::PlayerLightTweakServicesV90 s;s.provider=source_world->files_owner;
 s.current_level=[scope](auto& out,std::string& e){std::shared_ptr<SourceWorldBorrowV61> w;if(!scope(w,e))return false;CanonicalCurrentLevelBorrowV1 current;if(!borrow_current_native_level_v27(current,e))return false;out=current.level();e.clear();return true;};
 // Exact driver setParameter<SColor>(basefa,row0) writes existing material
 // backing, independent of enabled432. Source color bytes are copied as-is.
 s.driver_fog_color=[scene](const auto& value,std::string& e){std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots;if(!scene(roots,e))return false;auto& frame=roots->source_frame_fields_v67();frame.fog_color=value;++frame.parameter_revision;e.clear();return true;};
 s.driver_fog_distances=[scene](float start,float end,std::string& e){std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots;if(!scene(roots,e))return false;auto& frame=roots->source_frame_fields_v67();frame.fog_start_end={start,end};++frame.parameter_revision;e.clear();return true;};
 s.scene_direction=[scene](const auto& value,std::string& e){std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots;if(!scene(roots,e))return false;auto& f=roots->source_frame_fields_v67();f.fog_transform458[0]=value[0];f.fog_transform458[1]=value[1];f.fog_transform458[2]=value[2];++f.parameter_revision;e.clear();return true;};
 s.config_color=[config](const auto& level,std::uint32_t offset,const auto& value,std::string& e){
  std::shared_ptr<dh2::world::CanonicalLevelConfigV1> cfg;if(!config(level,cfg,e))return false;
  auto* r=cfg->source_vector_component_v90(offset,0);auto* g=cfg->source_vector_component_v90(offset,1);auto* b=cfg->source_vector_component_v90(offset,2);
  if(!r||!g||!b){e="Required SAME produced LevelConfig color Point3D";return false;}
  *b=value[2];*r=value[0];*g=value[1];e.clear();return true; // exact B/R/G source stores
 };
 s.config_int=[config](const auto& level,std::uint32_t offset,std::int32_t value,std::string& e){std::shared_ptr<dh2::world::CanonicalLevelConfigV1> cfg;if(!config(level,cfg,e))return false;auto fields=cfg->properties().fields;return fields.write_int&&fields.write_int(fields.context,offset,value,e);};
 s.level_enable_fog=[config,scene](const auto& level,std::string& e){
  // Whole original EnableFog3f1430 borrows config38 once and current scene,
  // then delegates existing SceneManager.EnableFog. It does NOT call Debug
  // or replace Main's separate UpdateFog3f2304 body.
  std::shared_ptr<dh2::world::CanonicalLevelConfigV1> cfg;if(!config(level,cfg,e))return false;
  std::shared_ptr<dh2::world::GameObjectSceneRootRegistryV1> roots;if(!scene(roots,e))return false;
  const auto* start=cfg->integer(0x1d8);const auto* end=cfg->integer(0x1dc);const auto* color=cfg->vector(0x1e0);
  if(!start||!end||!color){e="Required SAME source config fog values";return false;}
  roots->source_enable_fog_v68(static_cast<float>(*start),static_cast<float>(*end),*color);e.clear();return true;
 };
 s.light=[weak](std::uintptr_t id,dh2::application::TweakLightV90& out,std::string& e){auto w=weak.lock();return w&&borrow_source_campaign_tweak_light_v113(w,id,out,e);};
 out=std::move(s);e.clear();return true;
}

bool set_campaign_player_light_tweak_value_v90(
 const std::shared_ptr<SourceWorldBorrowV61>& world,std::size_t slot,
 const dh2::application::TweakAttributesV90& attrs,std::int32_t index,std::string& e){
 if(!world||!world->application||slot>=4){e="Required actual App58..64 source receiver";return false;}
 auto owner=world->application->source_tweaker58_v90(slot);
 dh2::application::PlayerLightTweakServicesV90 services;
 if(!owner||!bind_campaign_player_light_tweak_services_v90(world,services,e))return false;
 return owner->set_value(attrs,index,services,e);
}
