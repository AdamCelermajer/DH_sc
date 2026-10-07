#include "lifecycle_v36.hpp"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cstring>
#include <exception>
#include <utility>
namespace dh2::loader {
const std::array<LifecycleStageV36,38>& lifecycle_stages_v36() noexcept {
 static constexpr std::array<LifecycleStageV36,38> stages{{
 {0,0x3f74c8,"Application.CleanGlitch/stop-sounds/reset-load-counters",true},
 {1,0x3f753c,"source-default-increment",false},
 {2,0x3f745c,"MenuManager.UnloadMenu(2,1,3)",true},
 {3,0x3f7414,"source-debug-only-increment",false},
 {4,0x3f73c0,"VisualFXManager.BuildLibraries",true},
 {5,0x3f734c,"PhysicalWorld.load",true},
 {6,0x3f72d0,"PlayerManager.Update/GameEventManager.Load",true},
 {7,0x3f7204,"seeds/GenerateRandomLevel/Level.LoadFile",true},
 {8,0x3f7174,"LevelConfig/fog/Level._LoadScripts",true},
 {9,0x3f7128,"Level._LoadLightSet",true},
 {10,0x3f70cc,"ObjectManager.InitPost",true},
 {11,0x3f707c,"PFWorld.PostLoad",true},
 {12,0x3f7a84,"MenuManager.LoadMenu(1)",true},
 {13,0x3f7a34,"ScriptManager.InitCommands",true},
 {14,0x3f7880,"Level._LoadPlayer/online-bindings",true},
 {15,0x3f7828,"VoxSoundManager.SetLevelRouting",true},
 {16,0x3f780c,"source-debug-only-increment",false},
 {17,0x3f77c0,"Level._LoadFinalInit",true},
 {18,0x3f76c8,"Level._LoadCharStates",true},
 {19,0x3f76ac,"source-debug-only-increment",false},
 {20,0x3f7b84,"Level._LoadCamera",true},
 {21,0x3f7b38,"Level._LoadBatchInit",true},
 {22,0x3f7aec,"Level._LoadBatchList",true},
 {23,0x3f7aa0,"Level._LoadBatchMap",true},
 {24,0x3f7df4,"Level._LoadBatching",true},
 {25,0x3f7da4,"Application.CleanGlitch",true},
 {26,0x3f6fb0,"HUD/PostInitCharacters/cache-refresh",true},
 {27,0x3f6e90,"source-default-increment",false},
 {28,0x3f6e90,"source-default-increment",false},
 {29,0x3f7c3c,"ItemManager/ProjectileManager.PreCache",true},
 {30,0x3f7be8,"VisualFXManager.PreCacheLibraries",true},
 {31,0x3f7bd0,"GameEventManager.Compile",true},
 {32,0x3f7714,"ambient/ObjectManager.Update/AI/physical/camera",true},
 {33,0x3f7630,"LevelSavegame.Load/spawn-restoration",true},
 {34,0x3f6b34,"level-id/camera/environment/checkpoint-finalize",true},
 {35,0x3f753c,"source-default-increment",false},
 {36,0x3f7558,"online-loading/network/quest-sync/wait-EndLoading",true},
 {37,0x3f7c98,"menu/input/final-load-completion",true}
 }};return stages;
}
static std::int32_t signed_word(std::uint32_t raw) noexcept {std::int32_t value;std::memcpy(&value,&raw,4);return value;}
void lifecycle_progress_tail_v36(LifecycleFieldsV36 f) noexcept {
 const auto state=*f.state130;
 if(state==38){*f.progress30=100;return;}
 if(signed_word(*f.current138)<signed_word(*f.counter134))*f.counter134=*f.current138;
 // Accepted source range 0..38: exact signed magic-division result /38.
 const auto product=signed_word(std::uint32_t(100)*state);
 *f.progress30=state==36?100:std::uint32_t(std::min(product/38,100));
}
void LifecycleV36::fail(std::int32_t state,const std::string& service,const std::string& error){
 diagnostics_.status=LifecycleStatusV36::failed;diagnostics_.failed_state=state;
 diagnostics_.required_service=service;diagnostics_.error=error.empty()?"Required actual provider: "+service:error;
}
LifecycleV36::LifecycleV36(LifecycleFieldsV36 f,std::shared_ptr<void> level,
 std::vector<std::shared_ptr<const void>> pins,LifecycleServicesV36 s):
 fields_(f),actual_level_pin_(std::move(level)),resource_pins_(std::move(pins)),services_(std::move(s)) {
 diagnostics_.owned_pins=resource_pins_.size()+(actual_level_pin_?1:0);
 if(!actual_level_pin_||!f.progress30||!f.state130||!f.counter134||!f.current138){
  fail(-1,"actual-Level-field-borrow","Require pinned actual Level fields30/130/134/138");return;
 }
 expected_state_=signed_word(*f.state130);diagnostics_.source_state=expected_state_;diagnostics_.source_progress=signed_word(*f.progress30);
 if(expected_state_<0||expected_state_>38)fail(expected_state_,"actual-Level-state","Invalid actual loading state");
}
bool LifecycleV36::publish(){
 try {
 diagnostics_.source_state=*fields_.state130;diagnostics_.source_progress=*fields_.progress30;
 if(services_.resource_counts){std::string error;diagnostics_.actual_resources_available=services_.resource_counts(diagnostics_.actual_resources,error);
  if(!diagnostics_.actual_resources_available){fail(*fields_.state130,"actual-resource-counts",error);return false;}}
 if(!services_.publish_progress){fail(*fields_.state130,"original-progress-callback",{});return false;}
 std::string error;if(!services_.publish_progress(*fields_.state130,*fields_.progress30,error)){fail(*fields_.state130,"original-progress-callback",error);return false;}return true;
 }catch(const std::exception& e){fail(*fields_.state130,"progress/resource-provider",e.what());return false;}
 catch(...){fail(*fields_.state130,"progress/resource-provider","Provider threw an unknown exception");return false;}
}
LifecycleStatusV36 LifecycleV36::tick(){
 if(busy_){fail(diagnostics_.source_state,"runtime-thread/nonreentrancy","Level load reentered");return diagnostics_.status;}
 if(diagnostics_.status==LifecycleStatusV36::cancelled)return diagnostics_.status;
 if(diagnostics_.status==LifecycleStatusV36::failed&&!cancel_requested_)return diagnostics_.status;
 struct Busy {bool& b;explicit Busy(bool&v):b(v){b=true;}~Busy(){b=false;}} busy(busy_);
 const auto started=std::chrono::steady_clock::now();const auto cpu=std::clock();
 struct Time {LifecycleDiagnosticsV36& d;std::chrono::steady_clock::time_point at;std::clock_t cpu;
  ~Time(){d.elapsed_nanoseconds+=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-at).count();const auto end=std::clock();if(cpu!=std::clock_t(-1)&&end!=std::clock_t(-1)&&end>=cpu)d.cpu_ticks+=end-cpu;}}
  timer{diagnostics_,started,cpu};
 if(cancel_requested_){
  diagnostics_.status=LifecycleStatusV36::cancelling;
  if(!services_.cancel_and_unload){fail(expected_state_,"Level.Unload/owned-resource-teardown",{});return diagnostics_.status;}
  std::string error;LifecycleStepV36 r;
  try {++diagnostics_.service_calls;r=services_.cancel_and_unload(error);}
  catch(const std::exception& e){fail(expected_state_,"Level.Unload/owned-resource-teardown",e.what());cancel_requested_=false;return diagnostics_.status;}
  catch(...){fail(expected_state_,"Level.Unload/owned-resource-teardown","Provider threw an unknown exception");cancel_requested_=false;return diagnostics_.status;}
   // A nested tick can latch failure inside the teardown callback. Preserve
   // that reached failure and its pins instead of accepting the outer result.
   if(diagnostics_.status==LifecycleStatusV36::failed){cancel_requested_=false;return diagnostics_.status;}
  if(r==LifecycleStepV36::complete){resource_pins_.clear();actual_level_pin_.reset();diagnostics_.owned_pins=0;diagnostics_.status=LifecycleStatusV36::cancelled;}
  else if(r!=LifecycleStepV36::pending){fail(expected_state_,"Level.Unload/owned-resource-teardown",error);cancel_requested_=false;}
  return diagnostics_.status;
 }
 const auto state=signed_word(*fields_.state130);
 // The menu's real NativeEndLoading is allowed to advance 36->37 between ticks.
 if(state!=expected_state_&&!(expected_state_==36&&state==37)){fail(state,"single-actual-loading-authority","Unexpected actual loading-state mutation; refusing replay");return diagnostics_.status;}
 expected_state_=state;
 if(state==38){diagnostics_.status=LifecycleStatusV36::source_finished;lifecycle_progress_tail_v36(fields_);publish();return diagnostics_.status;}
 const auto& stage=lifecycle_stages_v36()[state];LifecycleStepV36 result=LifecycleStepV36::complete;
 if(stage.source_has_body){
  if(!services_.stage_body[state]){fail(state,stage.service,{});return diagnostics_.status;}
  std::string error;
  try {++diagnostics_.service_calls;result=services_.stage_body[state](error);}
  catch(const std::exception& e){fail(state,stage.service,e.what());return diagnostics_.status;}
  catch(...){fail(state,stage.service,"Provider threw an unknown exception");return diagnostics_.status;}
  if(diagnostics_.status==LifecycleStatusV36::failed)return diagnostics_.status;
  if(*fields_.state130!=std::uint32_t(state)){fail(state,stage.service,"Stage provider mutated field130; require body excluding dispatcher increment");return diagnostics_.status;}
  if(result==LifecycleStepV36::failed||result==LifecycleStepV36::dependency_missing){fail(state,stage.service,error);return diagnostics_.status;}
 }
 if(result==LifecycleStepV36::complete&&state!=36){
  ++*fields_.state130;expected_state_=*fields_.state130;
  const auto& after=services_.after_source_increment[state];
  if(state==7&&!after){fail(state,"source-file13c-after130",{});return diagnostics_.status;}
  if(after){std::string error;bool ok{};
   try {ok=after(error);}catch(const std::exception& e){error=e.what();}catch(...){error="Post-increment provider threw";}
   if(!ok){fail(state,"source-after130-increment",error);return diagnostics_.status;}
   if(*fields_.state130!=std::uint32_t(state+1)){fail(state,"source-after130-increment","Provider mutated field130");return diagnostics_.status;}
  }
  ++diagnostics_.completed_stages;
 }
 if(state==36)diagnostics_.status=LifecycleStatusV36::awaiting_end_loading;
 else diagnostics_.status=*fields_.state130==38?LifecycleStatusV36::source_finished:LifecycleStatusV36::loading;
 lifecycle_progress_tail_v36(fields_);publish();return diagnostics_.status;
}
}
