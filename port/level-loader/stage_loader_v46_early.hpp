#pragma once
#include "stage_loader_v39_file_counter.hpp"
#include <exception>
namespace dh2::loader {
struct Stage0LevelBorrowV46 {LifecycleBorrowV36 loading;std::uint32_t* file13c{};std::uint8_t* byte144{};};
template<class Context> bool borrow_stage0_level_v46(const std::shared_ptr<Context>& level,Stage0LevelBorrowV46& out,std::string& error){
 LifecycleBorrowV36 actual;if(!borrow_lifecycle_fields_v36(level,actual,error))return false;
 FileCounterBorrowV39 file;if(!borrow_file_counter_v39(level,file,error))return false;
 auto c1=level->constructor_borrow_v3();static_assert(std::is_same_v<decltype(c1.fields->byte144),std::uint8_t>);
 if(file.actual_level_owner.get()!=actual.actual_level_owner.get()||file.identity!=actual.identity||file.state130!=actual.fields.state130){error="Stage0 file borrow must use SAME completed C1";return false;}
 out={std::move(actual),file.file13c,&c1.fields->byte144};error.clear();return true;
}
// Application/device/audio owners supply their actual views. No modern copy
// of g_bigI/g_bigV/Application.byteb4 or audio state may be passed as production.
struct Stage0ApplicationBorrowV46 {
 std::shared_ptr<void> application_owner,global_words_owner;
 std::uintptr_t application_identity{};
 std::uint32_t *g_bigI{},*g_bigV{};std::uint8_t* byteb4{};
};
struct Stage0ServicesV46 {
 // Actual Application.CleanGlitch31f55c: globalApp+10 ->owner+10 ->virtualac.
 std::function<bool(const Stage0ApplicationBorrowV46&,std::string&)> clean_glitch;
 std::shared_ptr<void> sound_manager_owner;std::uintptr_t sound_manager_identity{};
 std::function<bool(std::uintptr_t,int,std::string&)> stop_all_sounds;
};
inline std::function<LifecycleStepV36(std::string&)> stage0_body_v46(Stage0LevelBorrowV46 level,Stage0ApplicationBorrowV46 app,Stage0ServicesV46 services){
 return [level=std::move(level),app=std::move(app),services=std::move(services),done=false,failed=false,busy=false,failure=std::string{}](std::string& error)mutable{
  auto reject=[&](const std::string& e){failed=true;failure=e;error=e;return LifecycleStepV36::failed;};
  if(busy)return reject("Stage0 reentered; original prefix retained");
  if(failed){error="Stage0 original prefix already failed; refusing replay";return LifecycleStepV36::failed;}
  if(done){error="Stage0 already completed; refusing body replay";return LifecycleStepV36::failed;}
  struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy);
  auto f=level.loading.fields;
  if(!level.loading.actual_level_owner||!f.state130||!f.counter134||!f.current138||!level.file13c||!level.byte144||*f.state130!=0)return reject("Stage0 requires SAME completed actual C1 at state130=0");
  if(!app.application_owner||!app.application_identity||!app.global_words_owner||!app.g_bigI||!app.g_bigV||!app.byteb4)return reject("Required actual Application/global words/byteb4 borrow");
  // Exact3f74e4/3f74ec prefix before the first missing body may fail.
  *app.g_bigI=0;*app.g_bigV=0;
  if(!services.clean_glitch)return reject("Required original Application.CleanGlitch31f55c");
  try{
   if(!services.clean_glitch(app,error))return reject(error.empty()?"Application.CleanGlitch failed":error);
   if(failed){error=failure;return LifecycleStepV36::failed;}
   if(*f.state130!=0)return reject("Application.CleanGlitch mutated actual loadingstate");
   // ShowMemoryStats31041c is genuine BX LR, no invented body service.
   if(!services.sound_manager_owner||!services.sound_manager_identity||!services.stop_all_sounds)return reject("Required original VoxSoundManager.StopAllSounds369990(500)");
   if(!services.stop_all_sounds(services.sound_manager_identity,500,error))return reject(error.empty()?"VoxSoundManager.StopAllSounds failed":error);
   if(failed){error=failure;return LifecycleStepV36::failed;}
   if(*f.state130!=0)return reject("VoxSoundManager.StopAllSounds mutated actual loadingstate");
  }catch(const std::exception& e){return reject(e.what());}catch(...){return reject("Stage0 provider threw");}
  *app.byteb4=1;*f.counter134=0;*f.current138=0;*level.file13c=0;*level.byte144=0;done=true;error.clear();return LifecycleStepV36::complete;
  // Dispatcher130 increment and original progress/tail are caller owned.
 };
}
inline constexpr const char* level_loading_trace_key_v46="isTracingLevel_Loading";
// Stages2..6 all query this exact key, result ignored. Actual DebugSwitches
// GetInstance330740 and GetSwitch337a88 belong to the source provider.
struct EarlyLoadingDebugV46 {
 std::shared_ptr<void> actual_owner;
 std::function<bool(const char*,bool&,std::string&)> get_instance_and_switch;
};
inline bool early_loading_trace_v46(const EarlyLoadingDebugV46& debug,std::string& error){
 if(!debug.actual_owner||!debug.get_instance_and_switch){error="Required actual DebugSwitches.GetInstance/GetSwitch(isTracingLevel_Loading)";return false;}
 bool ignored=false;return debug.get_instance_and_switch(level_loading_trace_key_v46,ignored,error);
}
}
