#pragma once
#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <utility>

namespace dh2::loader {
// Source 3f69b0..3f6b30 envelope only. Fields are real retained borrows;
// callbacks implement original bodies, not availability/default fallbacks.
struct LoadPrefixLevelV50 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 const std::uint32_t* state130{};
};
struct LoadPrefixPlayerV50 {
 std::shared_ptr<void> owner;
 std::uintptr_t record_identity{};
 const std::uintptr_t* character660{};
};
struct LoadPrefixWorldMapV50 {
 std::shared_ptr<void> owner;
 std::uintptr_t identity{};
 std::uint8_t* byte12c{};
};
struct LoadPrefixServicesV50 {
 std::shared_ptr<void> actual_application_owner,cheat_global_owner,debug_owner;
 const std::uint8_t* handle_cheats_inGame{};
 // Application.GetCurrentLevel uses the sole retained GS global.
 // Genuine empty returns a zero borrow; missing provider is an error.
 std::function<bool(LoadPrefixLevelV50&,std::string&)> current_level;
 std::function<bool(std::int32_t,bool,LoadPrefixPlayerV50&,std::string&)> local_player;
 std::function<bool(std::uintptr_t,std::string&)> unlock_fast_travels;
 std::function<bool(LoadPrefixWorldMapV50&,std::string&)> world_map;
 std::function<bool(std::string&)> debug_load;
 std::function<bool(const char*,bool&,std::string&)> debug_get_switch;
 std::function<bool(std::uint32_t,const char*&,std::string&)> source_step_name;
 std::function<bool(const char*,std::string&)> menu_debug_set_text;
 std::function<bool(std::uint32_t,std::string&)> debug_out_loading_step;
 // Original stages3/16/19 use GetInstance then GetSwitch, without another
 // unconditional load call. The provider must preserve that source ordering.
 std::function<bool(const char*,bool&,std::string&)> debug_instance_get_switch;
};

class SourceLoadProcessPrefixV50 final {
 LoadPrefixLevelV50 level_;
 LoadPrefixServicesV50 services_;
 bool busy_{},failed_{};
 std::string failure_;
 bool reject(const char* required,std::string& error) {
  if(error.empty()) error=required;
  if(!failed_) failure_=error;
  failed_=true;
  error=failure_;
  return false;
 }
 template<class F,class... A> bool call(const char* name,const F& f,std::string& e,A&&... a) {
  if(!f || !f(std::forward<A>(a)...,e) || failed_) return reject(name,e);
  return true;
 }
 struct BusyGuard {bool& value;explicit BusyGuard(bool& v):value(v){value=true;}~BusyGuard(){value=false;}};
public:
 SourceLoadProcessPrefixV50(LoadPrefixLevelV50 level,LoadPrefixServicesV50 services)
  :level_(std::move(level)),services_(std::move(services)){}
 bool before_dispatch(std::uint32_t /*entry observation*/,std::string& error) {
  if(failed_) {error=failure_;return false;}
  if(busy_) return reject("Original loading prefix reentered",error);
  BusyGuard guard(busy_);
  if(!level_.owner || !level_.identity || !level_.state130 ||
     !services_.actual_application_owner || !services_.cheat_global_owner ||
     !services_.handle_cheats_inGame || !services_.debug_owner)
   return reject("Required actual C1/Application/handle_cheats_inGame/Debug borrows",error);
  try {
   if(*services_.handle_cheats_inGame) {
    LoadPrefixLevelV50 current;
    if(!call("Application.GetCurrentLevel31f594",services_.current_level,error,current))return false;
    if(current.identity) {
     if(!current.owner || !current.state130)return reject("Required actual current Level borrow",error);
     LoadPrefixPlayerV50 player;
     if(!call("PlayerManager.GetLocalPlayer36e478(0,true)",services_.local_player,error,0,true,player))return false;
     if(!player.owner || !player.record_identity || !player.character660)
      return reject("Required actual PlayerInfo.character660 borrow",error);
     const auto character=*player.character660;
     if(character) {
      if(!call("Character.SG_UnlockAllFastTravels3bba70",services_.unlock_fast_travels,error,character))return false;
      LoadPrefixWorldMapV50 menu;
      if(!call("MenuWorldMap.GetInstance4364f8",services_.world_map,error,menu))return false;
      if(!menu.owner || !menu.identity || !menu.byte12c)return reject("Required actual MenuWorldMap.byte12c borrow",error);
      *menu.byte12c=1;
     }
    } else if(current.owner || current.state130) {
     return reject("Empty current Level must have an empty actual borrow",error);
    }
   }
   if(!call("DebugSwitches.load337888",services_.debug_load,error))return false;
   bool display{};
   if(!call("DebugSwitches.GetSwitch337a88(IsDisplayLoadingStepName)",services_.debug_get_switch,error,"IsDisplayLoadingStepName",display))return false;
   if(display) {
    const char* text{};
    // Reread after Debug callbacks, exactly at3f6ad0. Never use entry phase.
    if(!call("DBG_GetLoadingStepName3ef18c",services_.source_step_name,error,*level_.state130,text))return false;
    // Source phase37 may return its original unusual pointer. No invented
    // label/null normalization: genuine SetText owns its original argument.
    if(!call("MenuDebug.GetInstance/SetText42a08c",services_.menu_debug_set_text,error,text))return false;
   }
   // Reread again after MenuDebug, exactly at3f6a1c.
   if(!call("_DEBUG_OUT324114 loading step",services_.debug_out_loading_step,error,*level_.state130))return false;
   error.clear();return true;
  }catch(const std::exception& e){error=e.what();return reject("Original loading prefix exception",error);}
  catch(...){return reject("Original loading prefix unknown exception",error);}
 }
 bool debug_only_stage(std::uint32_t stage,std::string& error) {
  if(failed_){error=failure_;return false;}
  if(busy_)return reject("Original debug loading stage reentered",error);
  BusyGuard guard(busy_);
  if(stage!=3 && stage!=16 && stage!=19)return reject("Original debug-only stage must be3/16/19",error);
  bool ignored{};
  try {
   if(!services_.debug_owner)return reject("Required actual Debug owner",error);
   if(!call("DebugSwitches.GetInstance/GetSwitch(isTracingLevel_Loading)",services_.debug_instance_get_switch,error,"isTracingLevel_Loading",ignored))return false;
   error.clear();return true;
  }catch(const std::exception& e){error=e.what();return reject("Original debug stage exception",error);}
  catch(...){return reject("Original debug stage unknown exception",error);}
 }
};
}
