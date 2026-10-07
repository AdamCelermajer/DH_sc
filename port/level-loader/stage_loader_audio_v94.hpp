#pragma once
#include "stage_loader_v46_early.hpp"
#include <audio_level_routing_stop_v94.hpp>
namespace dh2::loader {
struct Stage15LevelBorrowV94 {LifecycleBorrowV36 loading;std::int32_t* row3c{};};
template<class Context>bool borrow_stage15_level_v94(const std::shared_ptr<Context>& level,Stage15LevelBorrowV94& out,std::string& e){
 LifecycleBorrowV36 loading;if(!borrow_lifecycle_fields_v36(level,loading,e))return false;
 auto actual=level->constructor_borrow_v3();
 if(!actual.owner||!actual.fields||actual.identity!=loading.identity||actual.owner.owner_before(loading.actual_level_owner)||loading.actual_level_owner.owner_before(actual.owner)){e="Stage15 requires SAME actual Level3c/loading fields";return false;}
 out={std::move(loading),&actual.fields->row3c};e.clear();return true;
}
struct Stage15AudioServicesV94 {
 EarlyLoadingDebugV46 debug;
 // Capture actual nullable process SoundManager AFTER the debug prefix and
 // source Level3c read. No copied World-owned sound manager slot.
 std::function<bool(audio::AudioApplicationBorrowV42&,std::string&)> sound_manager;
 audio::AudioLevelRoutingSourcesV94 routing;
};
inline std::function<LifecycleStepV36(std::string&)> stage15_audio_source_v94(Stage15LevelBorrowV94 level,Stage15AudioServicesV94 services){
 return [level=std::move(level),services=std::move(services),done=false,failed=false,busy=false,failure=std::string{}](std::string& e)mutable{
  auto fail=[&](std::string why){if(!failed){failed=true;failure=std::move(why);}e=failure;return LifecycleStepV36::failed;};
  if(busy)return fail("Stage15 reentered; original prefix retained");if(failed){e=failure;return LifecycleStepV36::failed;}if(done)return fail("Stage15 source body cannot replay");
  struct Guard{bool& b;Guard(bool& v):b(v){b=true;}~Guard(){b=false;}} guard(busy);
  auto state=level.loading.fields.state130;
  if(!level.loading.actual_level_owner||!state||!level.row3c||*state!=15)return fail("Required SAME completed Level at state15");
  auto latched=[&](){if(failed){e=failure;return true;}return false;};
  try{
   std::string reached;const bool traced=early_loading_trace_v46(services.debug,reached);
   if(latched())return LifecycleStepV36::failed;
   if(!traced)return fail(reached.empty()?"Required Stage15 debug delivery":reached);
   if(*state!=15)return fail("Stage15 debug changed actual loadingstate");
   const auto row=*level.row3c;audio::AudioApplicationBorrowV42 captured;
   reached.clear();const bool borrowed=services.sound_manager&&services.sound_manager(captured,reached);
   if(latched())return LifecycleStepV36::failed;
   if(!borrowed)return fail(reached.empty()?"Required actual process SoundManager borrower":reached);
   if(*state!=15)return fail("Stage15 SoundManager borrower changed actual loadingstate");
   // This call-local guard runs after the actual RES_PATH loan and before the
   // inherited driver leaf. It never outlives this synchronous source body.
   auto routing=services.routing;auto current=routing.validate_delivery;
   routing.validate_delivery=[&failed,&failure,state,current](std::string& receipt){
    if(failed){receipt=failure;return false;}
    if(*state!=15){receipt="Stage15 prefix borrower changed actual loadingstate";return false;}
    const bool delivered=!current||current(receipt);
    if(failed){receipt=failure;return false;}
    return delivered;
   };
   bool ignored{};reached.clear();const bool routed=audio::set_level_routing_source_v94(captured,row,routing,ignored,reached);
   if(latched())return LifecycleStepV36::failed;
   if(!routed)return fail(reached.empty()?"Required actual Stage15 routing delivery":reached);
   if(*state!=15)return fail("Stage15 routing changed actual loadingstate");
  }catch(const std::exception& ex){return fail(ex.what());}catch(...){return fail("Stage15 source audio provider threw");}
  done=true;e.clear();return LifecycleStepV36::complete;
  // Original dispatcher owns state130 increment/progress tail3f7870..78.
 };
}
}
