#pragma once
#include "character_design_services.hpp"
#include "../game-data/animation_scheduler.hpp"
namespace dh2::character {
// Borrows the SAME Character/App Debug owner and file transport. Each native
// _SetAnim occurrence traces before event36 and reads its random policy after
// that callback, only for a type2 row. No copied switch snapshot is retained.
class CharacterAnimationSelectionDebugV126 final {
 DebugSwitches* debug_;const DebugFileServices24* files_;
 bool query(const char* name,std::uint32_t& value,std::string& error){
  if(!debug_||!files_||dh2_character_debug_load(debug_,files_)!=1||
     dh2_character_debug_get(&value,debug_,name,files_)!=1){
   error=std::string("Required SAME CharAnimator Debug load/query: ")+name;return false;
  }
  return true;
 }
 static bool trace(void* raw,std::string& error){std::uint32_t ignored{};
  return static_cast<CharacterAnimationSelectionDebugV126*>(raw)->query("isTracingCharAnimator",ignored,error);
 }
 static bool random(void* raw,bool& enabled,std::string& error){std::uint32_t minimal{};
  if(!static_cast<CharacterAnimationSelectionDebugV126*>(raw)->query("MP_MinimalRandoms",minimal,error))return false;
  enabled=(minimal^1u)&0xffu;return true;
 }
public:
 CharacterAnimationSelectionDebugV126(DebugSwitches* debug,const DebugFileServices24* files):debug_(debug),files_(files){}
 data::AnimationSelectionPolicyServicesV126 services()noexcept{return {this,trace,random};}
};
}
