// Include after source candidate. Installs the ONE genuine parsed-command
// ScriptManager in Application; it is distinct from the Level's LuaScript44.
namespace {
using SourceScriptFactoryV55=decltype(dh2::loader::ScriptManagerServicesV52::command_factory);
[[maybe_unused]] bool borrow_source_script_manager_v55(const std::shared_ptr<RendererLevelApplicationV25>& application,
 SourceScriptFactoryV55 factory,std::shared_ptr<dh2::loader::ScriptManagerOwnerV52>& out,std::string& error){
 using namespace dh2::loader;
 if(!application||!application->application||!application->files){error="Required actual Application/cache ScriptManager services";return false;}
 auto& authority=*application->application;
 if(authority.source_script_manager_v52()){out=authority.source_script_manager_v52();error.clear();return true;}
 if(!factory){error="Required genuine canonical command C1 factory before ScriptManager publication";return false;}
 ScriptManagerServicesV52 services;services.owner=application->files;
 services.open_file=[files=application->files](const std::string& name,ScriptFileV52& out,std::string& e){
  auto bytes=std::make_shared<std::vector<std::uint8_t>>();bool found{};
  if(!files->cache.read(name,found,*bytes,e))return false;
  out={bytes,bytes->data(),bytes->size(),found};e.clear();return true;
 };
 services.command_factory=std::move(factory); // Real canonical command C1, never an opcode placeholder.
 auto actual=std::make_shared<ScriptManagerOwnerV52>(std::move(services));
 if(!authority.publish_source_script_manager_v52(actual,error))return false;
 out=std::move(actual);error.clear();return true;
}
[[maybe_unused]] bool bind_source_stage8_v55(const std::shared_ptr<RendererSourceCandidateV55>& candidate,
 const std::shared_ptr<dh2::loader::CanonicalLevelContextV1>& level,
 const std::shared_ptr<dh2::loader::ScriptManagerOwnerV52>& scripts,
 dh2::loader::EarlyLoadingDebugV46 debug,dh2::loader::SourceLoadingInputsV43& loading,std::string& error){
 using namespace dh2;using namespace dh2::loader;
 if(!candidate||!candidate->factory||!candidate->preparation||candidate->preparation->level()!=level||!scripts||loading.external.stage_body[8]){
  error="Stage8 requires SAME candidate/C1/sole actual ScriptManager and empty body slot";return false;
 }
 Stage8LevelBorrowV51 actual;if(!borrow_stage8_level_v51(level,actual,error))return false;
 Stage8ServicesV51 services;services.debug=std::move(debug);
 const auto canonical=candidate->canonical;const auto weak=std::weak_ptr<RendererSourceFactoryCoreV55>(candidate->factory);
 services.borrow_actual_config=[weak,canonical](std::uintptr_t identity,Stage8ConfigBorrowV51& out,std::string& e){
  auto core=weak.lock();const auto* config=core?core->modules->level_config(identity):nullptr;
  if(!config){e="Required actual registered LevelConfig38 source receiver";return false;}
  const world::CanonicalObjectBorrowV1* object{};std::int32_t key{};
  bool more=canonical->manager.source_ordered_begin_v38(key,object);
  while(more&&(!object||object->identity!=identity))more=canonical->manager.source_ordered_next_v38(key,key,object);
  if(!more||!object||!object->lease){e="LevelConfig38 is not published in SAME canonical manager";return false;}
  auto* actual_config=const_cast<world::CanonicalLevelConfigV1*>(config);
  out={object->lease,identity,config->string(0x150),&config->dfog_colors(),config->scalar_float_v55(0x210),
   [weak,identity,actual_config](const std::string& text,std::string& e){
    auto core=weak.lock();if(!core||core->modules->level_config(identity)!=actual_config){e="Released or foreign LevelConfig string150 producer";return false;}
    auto fields=actual_config->properties().fields;
    if(!fields.write_string){e="Required genuine LevelConfig CString property writer";return false;}
    return fields.write_string(fields.context,0x150,text,e);
   }};
  e.clear();return true;
 };
 // NULL38 default-Spawn/override and nonempty fog manager calls remain
 // required; an empty authored fog vector is its genuine source branch.
 services.object_manager_owner=canonical;services.object_manager_identity=reinterpret_cast<std::uintptr_t>(&canonical->manager);
 services.script_manager=bind_script_manager_v52(scripts);
 loading.external.stage_body[8]=stage8_config_scripts_v51(std::move(actual),std::move(services));
 loading.resource_pins.push_back(scripts);error.clear();return true;
}
}
