#pragma once
#include "lifecycle_v36_counter_borrow.hpp"
namespace dh2::loader {
struct LifecycleTeardownServicesV36 {
 // Native safety addition for incomplete preparation. MUST NOT route through
 // full source Unload's SG_SaveAllPlayer path. Releases real retained owners
 // without publishing save effects or inventing completed gameplay state.
 std::function<LifecycleStepV36(const LifecycleBorrowV36&,std::string&)> abort_incomplete;
 // Actual source Unload plus real owner destruction after completed loading.
 // Save effects belong to main's authoritative services. Unload alone does not
 // prove object/visual destruction; return complete only after real teardown.
 std::function<LifecycleStepV36(const LifecycleBorrowV36&,std::string&)> unload_and_destroy_completed;
};
inline std::function<LifecycleStepV36(std::string&)> lifecycle_teardown_provider_v36(
 LifecycleBorrowV36 actual,LifecycleTeardownServicesV36 services) {
 return [actual=std::move(actual),services=std::move(services),selected=false,
         completed_path=false,done=false,failed=false](std::string& error) mutable {
  if(done)return LifecycleStepV36::complete;
  if(failed){error="Actual Level teardown already failed";return LifecycleStepV36::failed;}
  if(!actual.actual_level_owner||!actual.fields.state130){failed=true;error="Required actual pinned Level teardown receiver";return LifecycleStepV36::dependency_missing;}
  if(!selected){
   const auto state=*actual.fields.state130;
   if(state>38){failed=true;error="Invalid actual Level state for teardown";return LifecycleStepV36::failed;}
   completed_path=state==38;selected=true;
  }
  // Pin the selected path even when actual Unload resets state130 to0 before
  // asynchronous owner destruction completes; never switch into abort then.
  const auto& provider=completed_path?services.unload_and_destroy_completed:services.abort_incomplete;
  if(!provider){failed=true;error=completed_path?"Required actual completed-Level Unload/destruction provider":"Required separate incomplete-Level abort teardown provider";return LifecycleStepV36::dependency_missing;}
  const auto result=provider(actual,error);
  if(result==LifecycleStepV36::complete){done=true;actual={};services={};}
  else if(result!=LifecycleStepV36::pending)failed=true;
  return result;
 };
}
}
