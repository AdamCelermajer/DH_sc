#pragma once
#include "stage_loader_v46_early.hpp"
#include "level_script_paths_v51.hpp"
#include <cmath>
#include <limits>
namespace dh2::loader {
struct Stage8LevelBorrowV51 {LifecycleBorrowV36 loading;std::uintptr_t* config38{};};
template<class Context>bool borrow_stage8_level_v51(const std::shared_ptr<Context>& level,Stage8LevelBorrowV51& out,std::string& error){
 LifecycleBorrowV36 actual;if(!borrow_lifecycle_fields_v36(level,actual,error))return false;auto config=level->config_fields();
 if(!config.config38||config.level_owner.get()!=actual.actual_level_owner.get()||config.level_owner.owner_before(actual.actual_level_owner)||actual.actual_level_owner.owner_before(config.level_owner)){error="Stage8 config38 requires SAME completed C1 Level owner";return false;}
 out={std::move(actual),config.config38};error.clear();return true;
}
// Exact typed views of the SAME produced LevelConfig properties. Native
// provider uses existing CanonicalLevelConfigV1 strings/dfog_colors/scalar.
struct Stage8ConfigBorrowV51 {
 std::shared_ptr<void> actual_owner;std::uintptr_t identity{};
 const std::string* script150{};
 // Source vector is never copied into a replacement module registry.
 const std::vector<std::array<float,3>>* fog204{};const float* distance210{};
 std::function<bool(const std::string&,std::string&)> write_script150;
};
struct ScriptManagerServicesV51 {
 // This is original global ScriptManager, NOT the unrelated LuaScript44.
 std::shared_ptr<void> actual_owner;std::uintptr_t identity{};
 std::function<bool(std::uintptr_t,std::string&)> unload_all;
 std::function<bool(std::uintptr_t,const char*,bool,std::string&)> load_commands,load_names;
};
struct Stage8ServicesV51 {
 EarlyLoadingDebugV46 debug;
 // Required only NULL38: whole original app override LoadFile + default
 // LevelConfig Spawn(false,true)/resolve/type4/SetLevelConfig branch.
 std::function<LifecycleStepV36(const Stage8LevelBorrowV51&,std::string&)> produce_missing_config;
 std::function<bool(std::uintptr_t,Stage8ConfigBorrowV51&,std::string&)> borrow_actual_config;
 std::shared_ptr<void> object_manager_owner;std::uintptr_t object_manager_identity{};
 std::function<bool(std::uintptr_t,const std::vector<std::array<float,3>>&,std::int32_t,std::string&)> init_modules_fog;
 ScriptManagerServicesV51 script_manager;
};
inline std::function<LifecycleStepV36(std::string&)> stage8_config_scripts_v51(Stage8LevelBorrowV51 level,Stage8ServicesV51 services){
 enum class Phase {trace,config,fog,unload,common_commands,common_names,local_commands,local_names,complete,failed};
 return [level=std::move(level),services=std::move(services),phase=Phase::trace,busy=false,failure=std::string{},config=Stage8ConfigBorrowV51{},paths=LevelScriptPathsV51{}](std::string& error)mutable{
  auto fail=[&](const std::string& e){phase=Phase::failed;failure=e;error=e;return LifecycleStepV36::failed;};
  if(busy)return fail("Stage8 reentered; source prefix retained");if(phase==Phase::failed){error=failure;return LifecycleStepV36::failed;}if(phase==Phase::complete)return fail("Stage8 completed; refusing body replay");
  struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy);
  auto state=level.loading.fields.state130;
  if(!level.loading.actual_level_owner||!state||!level.config38||*state!=8)return fail("Stage8 requires SAME completed C1 state1308/config38");
  auto stable=[&](){return phase!=Phase::failed&&*state==8;};
  auto same_config=[&](){return stable()&&config.actual_owner&&config.identity==*level.config38;};
  auto script_call=[&](const auto& callback,const char* name,bool common){auto& sm=services.script_manager;return callback&&callback(sm.identity,name,common,error);};
  try{
   if(phase==Phase::trace){if(!early_loading_trace_v46(services.debug,error))return fail(error);if(!stable())return fail(failure.empty()?"Stage8 trace changed actual loadingstate":failure);phase=Phase::config;}
   if(phase==Phase::config){
    if(!*level.config38){if(!services.produce_missing_config)return fail("Required original stage8 Application override/default LevelConfig producer3f8070..3f8194");auto s=services.produce_missing_config(level,error);if(!stable())return fail(failure.empty()?"Stage8 fallback mutated loadingstate":failure);if(s==LifecycleStepV36::failed)return fail(error);if(s==LifecycleStepV36::pending){error.clear();return s;}if(!*level.config38)return fail("Original stage8 produced no usable LevelConfig; NULL dereference rejected");}
    if(!services.borrow_actual_config)return fail("Required SAME actual LevelConfig38 property borrow");
    if(!services.borrow_actual_config(*level.config38,config,error))return fail(error);
    if(!same_config()||!config.script150||!config.fog204)return fail("Actual LevelConfig38 borrow has foreign identity/missing produced properties");phase=Phase::fog;
   }
   if(phase==Phase::fog){
    if(!same_config())return fail(failure.empty()?"Stage8 actual config38 changed":failure);
    if(!config.fog204->empty()){
     if(!config.distance210)return fail("Required produced LevelConfig float210");const double distance=*config.distance210;
     if(!std::isfinite(distance)||distance<std::numeric_limits<std::int32_t>::min()||distance>=2147483648.0)return fail("Original fog float210 outside native signed conversion domain");
     if(!services.object_manager_owner||!services.object_manager_identity||!services.init_modules_fog)return fail("Required original ObjectManager.InitModulesFogColor347100");
     if(!services.init_modules_fog(services.object_manager_identity,*config.fog204,static_cast<std::int32_t>(distance),error))return fail(error);
     if(!stable())return fail(failure.empty()?"Fog provider changed actual loadingstate":failure);
    }phase=Phase::unload;
   }
   auto& sm=services.script_manager;
   if(!sm.actual_owner||!sm.identity)return fail("Required original global ScriptManager owner (distinct from LuaScript44)");
   if(phase==Phase::unload){if(!sm.unload_all)return fail("Required ScriptManager.UnLoadAllScripts45a1c8");if(!sm.unload_all(sm.identity,error))return fail(error);if(!stable())return fail(failure.empty()?"Script unload changed loadingstate":failure);phase=Phase::common_commands;}
   if(phase==Phase::common_commands){if(!sm.load_commands)return fail("Required ScriptManager.LoadScriptFile45b264");if(!script_call(sm.load_commands,common_command_file_v51,true))return fail(error);if(!stable())return fail(failure.empty()?"Common commands changed loadingstate":failure);phase=Phase::common_names;}
   if(phase==Phase::common_names){if(!sm.load_names)return fail("Required ScriptManager.LoadScriptFileNames45ad40");if(!script_call(sm.load_names,common_command_names_v51,true))return fail(error);if(!stable())return fail(failure.empty()?"Common names changed loadingstate":failure);
    // Original _LoadScripts rereads field38 AFTER common callbacks. A real
    // changed config is legal and must be reborrowed, never an old script copy.
    if(!*level.config38)return fail("Required original _LoadScripts NULL-config diagnostic/assert path; invalid dereference rejected");
    Stage8ConfigBorrowV51 current;
    if(!services.borrow_actual_config(*level.config38,current,error))return fail(error);
    config=std::move(current);
    if(!same_config()||!config.script150)return fail("_LoadScripts requires current actual config38/string150");
    std::string normalized=*config.script150;paths=level_script_paths_v51(normalized);
    if(paths.reached){if(!config.write_script150)return fail("Required SAME LevelConfig string150 producer for slash normalization");if(!config.write_script150(normalized,error))return fail(error);if(!same_config()||*config.script150!=normalized)return fail(failure.empty()?"LevelConfig script150 normalization did not reach actual field":failure);phase=Phase::local_commands;}else{phase=Phase::complete;error.clear();return LifecycleStepV36::complete;}
   }
   if(phase==Phase::local_commands){if(!script_call(sm.load_commands,paths.command_file.c_str(),false))return fail(error);if(!stable())return fail(failure.empty()?"Level commands changed loadingstate":failure);phase=Phase::local_names;}
   if(phase==Phase::local_names){if(!script_call(sm.load_names,paths.name_file.c_str(),false))return fail(error);if(!stable())return fail(failure.empty()?"Level names changed loadingstate":failure);phase=Phase::complete;error.clear();return LifecycleStepV36::complete;}
   return fail("Stage8 unexpected native orchestration phase");
  }catch(const std::exception& e){return fail(e.what());}catch(...){return fail("Stage8 original body service threw");}
 };
 // state130/progress tail remain lifecycle dispatcher-owned.
}
}
