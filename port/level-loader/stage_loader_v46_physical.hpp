#pragma once
#include "stage_loader_v46_early.hpp"
#include <physical_world.hpp>
namespace dh2::loader {
// The shared_ptr can alias Application's actual PhysicalWorld owner. It must
// refer to that SAME receiver; no substitute world or Application is created.
inline std::function<LifecycleStepV36(std::string&)> stage5_physical_body_v46(LifecycleBorrowV36 level,std::shared_ptr<physical::NativeWorld> world,EarlyLoadingDebugV46 debug){
 return [level=std::move(level),world=std::move(world),debug=std::move(debug),attempted=false,failed=false,failure=std::string{}](std::string& error)mutable{
  auto reject=[&](const std::string& reason){attempted=true;failed=true;failure=reason;error=reason;return LifecycleStepV36::failed;};
  if(attempted)return reject("Stage5 body already attempted; refusing physical reset replay");
  if(!level.actual_level_owner||!level.fields.state130||*level.fields.state130!=5)return reject("Stage5 requires SAME completed actual C1 at state130=5");
  attempted=true;try{
   if(!early_loading_trace_v46(debug,error))return LifecycleStepV36::dependency_missing;
   if(failed){error=failure;return LifecycleStepV36::failed;}
   if(*level.fields.state130!=5)return reject("Debug query mutated actual loadingstate");
   if(!world){error="Required actual Application.PhysicalWorld+44 owner";return LifecycleStepV36::dependency_missing;}
   constexpr float original_bounds[4]{-2000.0f,-2000.0f,2000.0f,2000.0f};
   world->load(original_bounds); // Genuine retained PhysicalWorld::load body.
   if(!world->backend())return reject("Actual PhysicalWorld.load did not produce its backend");
   error.clear();return LifecycleStepV36::complete;
  }catch(const std::exception& e){return reject(e.what());}catch(...){return reject("Stage5 actual physics provider threw");}
 };
}
}
