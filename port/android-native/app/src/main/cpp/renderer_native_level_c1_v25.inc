// Real GS/C1 on the selected development Level, retained across GL recreation.
// GSLevel owns the sole current-Level publication. Neither this nor the renderer sets
// loading phase38 or disables loot to make an unfinished Level appear ready.
namespace {
struct RendererLevelFilesV25 {
 dh2::android_ui::OriginalCacheAssetsV1 cache;
 std::string directory;
 std::shared_ptr<dh2::application::ApplicationSaveFilesOwnerV61> actual_save_files;
 RendererLevelFilesV25(AAssetManager* assets,std::string path):cache(assets),directory(std::move(path)){}
 bool saved(const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string& e){
  if(actual_save_files)return actual_save_files->read_save(name,found,bytes,e);
  return dh2::android_ui::read_private_level_save_v39(directory,name,dh2::android_ui::level_save_transport_budget_v39,found,bytes,e);
 }
};
struct RendererLevelApplicationV25 {
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 std::shared_ptr<RendererLevelFilesV25> files;
 std::shared_ptr<dh2::loader::NativeLevelApplicationV25> source;
 RendererLevelApplicationV25(std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> app,AAssetManager* assets,const std::string& directory):application(std::move(app)),files(std::make_shared<RendererLevelFilesV25>(assets,directory)){
  std::string file_error;
  if(!dh2::application::ApplicationSaveFilesOwnerV61::acquire(application,directory,files->actual_save_files,file_error))
   throw std::runtime_error("Actual Application save files/jobs: "+file_error);
  dh2::loader::NativeLevelFilesV25 io;io.owner=files;
  io.script=[held=files](const auto& uri,auto& bytes,bool& found,auto& e){return held->cache.read(uri,found,bytes,e);};
  io.saved=[held=files](const auto& name,bool& found,auto& bytes,auto& e){return held->saved(name,found,bytes,e);};
  source=std::make_shared<dh2::loader::NativeLevelApplicationV25>(std::move(io));
 }
};
std::shared_ptr<RendererLevelApplicationV25> renderer_level_application_v25;
std::shared_ptr<dh2::loader::NativeGSLevelGlobalsV27> renderer_gs_globals_v27=std::make_shared<dh2::loader::NativeGSLevelGlobalsV27>();
void construct_native_level_c1_v25(AAssetManager* assets,const std::string& selected_mlx_uri,const std::shared_ptr<WorldScriptContext>& world,bool native_gs_launch_v27=false){
 using namespace dh2::loader;
 std::string error;
 if(!world||!equipment_platform||equipment_platform->world!=world||selected_mlx_uri.empty())throw std::runtime_error("Required actual selected World/Level C1 providers");
 if(world->native_level_c1_v25){
  const auto& c=*world->native_level_c1_v25;
  if(!c.complete()||!c.candidate()||c.candidate()->source_request().definition!=selected_mlx_uri||
     (native_gs_launch_v27&&(!world->native_gslevel_v27||!world->native_gslevel_v27->connection()||
      world->native_gslevel_v27->connection()->lifecycle().stage()!=GSLevelLifecycleStageV2::constructed)))
   throw std::runtime_error("Native GS restoration selection differs or prior source constructor failed");
  return;
 }
 std::shared_ptr<dh2::application::ApplicationServicesOwnerV5> application;
 if(!model_renderer::borrow_actual_application_services_v5(application,error))throw std::runtime_error(error);
 if(!renderer_level_application_v25||renderer_level_application_v25->application!=application||renderer_level_application_v25->files->directory!=world->files_directory)
  renderer_level_application_v25=std::make_shared<RendererLevelApplicationV25>(application,assets,world->files_directory);
 auto actual_design=std::make_shared<dh2::character::CharacterGameDesign::Borrow>(world->design->borrow());
 LevelConstructorApplicationV4 app;app.owner=renderer_level_application_v25;
 app.levels=actual_design->levels();app.lua.owner=actual_design;app.lua.design=*actual_design->design();
 app.private_vm_limit=16u*1024u*1024u; // Same explicit private-VM policy as source-owner proof.
 app.online_byte5=[weak=std::weak_ptr<WorldScriptContext>(world)](auto& online,auto& e){
  auto current=weak.lock();if(!current||!equipment_platform||equipment_platform->world!=current){e="Required current same World GetOnline receiver for Level C1";return false;}
  std::uintptr_t identity{};std::int32_t value{};
  if(!EquipmentPlatform::query(equipment_platform.get(),dh2::player::EquipmentWorldQueryV1::online,0,identity,value,e))return false;
  online=std::uint8_t(value);return true;
 };
 app=renderer_level_application_v25->source->bind(std::move(app));
 const auto* levels=world->tables.levels();const auto row=world->host_level.row_index;
 if(!levels||row<0||std::size_t(row)>=levels->levels.size())throw std::runtime_error("Required actual development Crypt LevelList selection");
 std::string name=levels->levels[std::size_t(row)].file;
 std::transform(name.begin(),name.end(),name.begin(),[](unsigned char ch){return char(std::tolower(ch));});
 // The launch remains the explicit Crypt demo. Actual row filename, current
 // player's cached level and development seed/mode feed the ORIGINAL C1;
 // this does not claim NativeStartGame/save-restored campaign selection.
 LevelSourceRequestV1 request;request.identity=name;request.definition=selected_mlx_uri;request.seed=0;
 if(!native_gs_launch_v27){
  // Explicit existing Crypt demo transport. It retains the full source C1
  // but does not publish a GS current Level or push a loading menu that the
  // still-unported Level.Init cannot finish. The campaign contract must call
  // the staged GS path once it supplies genuine Init and Unload endpoints.
  LevelConstructorArgumentsV3 c1{name.c_str(),world->host_player.cached_level,0,1,0,1,0,-1,0};
  world->native_level_c1_v25=std::make_shared<NativeLevelConnectionV25>();
  if(!world->native_level_c1_v25->construct(std::move(request),c1,std::move(app),error))throw std::runtime_error("Actual native Level C1 failed: "+error);
  const auto& fields=world->native_level_c1_v25->candidate()->constructor_fields_v3();
  world->host_level.row_index=fields.row3c;world->host_level.difficulty=fields.difficulty40;
  __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actual demo Level C1 retained | row %d difficulty %d | gate150 %u phase130 %u | GS/Level.Init campaign contract pending",fields.row3c,fields.difficulty40,world->native_level_c1_v25->candidate()->kill_level()->loot_gate150,fields.field130);
  return;
 }
 GSLevelArgumentsV2 arguments{name,world->host_player.cached_level,0,1,0,1,0,-1,0};
 world->native_gslevel_v27=std::make_shared<NativeGSLevelRuntimeV27>(renderer_gs_globals_v27);
 // Online34 is genuinely unproduced in this offline development transport.
 // The actual loading callback queries it only with NULL current Level;
 // the outer GS constructor likewise queries it only with Online.byte5=1.
 auto online_state=[](std::uint32_t&,std::string& e){e="Required original OnlineGameState34 producer";return false;};
 if(!world->native_gslevel_v27->prepare_loading(online_state,error))throw std::runtime_error(error);
 prepare_gameplay_camera_application_v27();
 GSLevelServicesV2<CanonicalLevelContextV1> gs;
 gs.flush_animation_sets=[](auto& e){try{flush_gameplay_animation_sets_v27();return true;}catch(const std::exception& failure){e=failure.what();return false;}};
 gs.online_byte5=app.online_byte5;
 gs.online_state34=[](std::int32_t&,auto& e){e="Required original OnlineGameState34 producer";return false;};
 // Actual Unload/Level destruction have not yet been reconstructed. Keeping
 // these reached endpoints required is essential: clearing a renderer is
 // not the original destructor and cannot certify Level lifecycle parity.
 gs.unload_level=[](const auto&,auto& e){e="Required whole original Level.Unload";return false;};
 gs.destroy_level=[](const auto&,auto& e){e="Required whole original Level destruction";return false;};
 auto debug=[weak=std::weak_ptr<WorldScriptContext>(world)](const char* key,std::int32_t& value,auto& e){
  auto current=weak.lock();if(!current||live_script_world!=current){e="Required same World Debug receiver";return false;}
  std::uint32_t source{};if(dh2_character_debug_load(current->debug.get(),&current->debug_files)!=1||
     (key&&dh2_character_debug_get(&source,current->debug.get(),key,&current->debug_files)!=1)){e="Required source MenuManager Debug delivery";return false;}
  value=std::int32_t(source);return true;
 };
 if(!model_renderer::borrow_native_gs_menu_v27(world,world->native_gslevel_v27->loading_services(),std::move(debug),gs,error))throw std::runtime_error("Actual native GS Menu bootstrap: "+error);
 const bool constructed=world->native_gslevel_v27->construct(std::move(request),std::move(arguments),std::move(app),std::move(gs),online_state,error);
 // Keep the same C1 allocation even if a synchronous menu continuation
 // fails after publication. Neither failure nor GL restore may replay C1.
 if(world->native_gslevel_v27->connection())world->native_level_c1_v25=world->native_gslevel_v27->connection()->level_connection();
 if(!constructed)throw std::runtime_error("Actual native GS constructor: "+error);
 const auto& c=*world->native_level_c1_v25->candidate();const auto& fields=c.constructor_fields_v3();
 world->host_level.row_index=fields.row3c;world->host_level.difficulty=fields.difficulty40;
 __android_log_print(ANDROID_LOG_INFO,"DH2Native","Actual GS C1 retained | Level %016llx | row %d difficulty %d | Lua44 %016llx Save_ec %016llx events %016llx | gate150 %u phase130 %llu | Application counter %u | same GS current-Level published; Level.Init pending",
  static_cast<unsigned long long>(c.identity()),fields.row3c,fields.difficulty40,
  static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(fields.script44.get())),static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(fields.save_ec.get())),
  static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(fields.events.get())),c.kill_level()->loot_gate150,static_cast<unsigned long long>(fields.field130),renderer_level_application_v25->source->debug_level_load_count);
}
}
