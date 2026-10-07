#pragma once
#include "game_event_manager_v50.hpp"
#include "game_event_level_fields_v50.hpp"
#include "stage_loader_v46_early.hpp"
namespace dh2::loader {
struct Stage6PlayerBorrowV50 {std::shared_ptr<void> actual_owner;std::uintptr_t identity{};std::uint8_t* byte6c9{};};
struct Stage6ServicesV50 {
 EarlyLoadingDebugV46 debug;
 Stage6PlayerBorrowV50 player;
 std::function<bool(const Stage6PlayerBorrowV50&,std::string&)> player_update;
 GameEventTablesBorrowV50 actual_events;
 GameEventStoragePolicyV50 modern_allocation_policy;
};
// The dispatcher remains the ONLY owner of state130++ and progress tail.
// Stage6 source: trace3f72d0 ->PMbyte6c9=1 ->Update378fb4 ->new12/C1479d08
// ->sameLevel194=manager ->Load479e5c ->dispatcher may advance6->7.
inline std::function<LifecycleStepV36(std::string&)> stage6_game_events_v50(GameEventLevelFieldsV50 level,Stage6ServicesV50 services){
 return [level=std::move(level),services=std::move(services),started=false,done=false,failed=false,busy=false,failure=std::string{}](std::string& error)mutable{
  auto reject=[&](const std::string& e){failed=true;failure=e;error=e;return LifecycleStepV36::failed;};
  if(busy)return reject("Stage6 GameEvent body reentered; original prefix retained");
  if(failed){error=failure;return LifecycleStepV36::failed;}if(done)return reject("Stage6 body already completed; refusing replay");
  struct Busy{bool& b;explicit Busy(bool& v):b(v){b=true;}~Busy(){b=false;}} guard(busy);
  if(!level.level_owner||!level.identity||!level.state130||!level.field194||!level.owner194||*level.state130!=6)return reject("Required SAME completed C1 Level state1306/field194/retained194 lease");
  try{
   if(!started){
    if(*level.field194||*level.owner194)return reject("Stage6 actual Level194 already owned; refusing replacement");
    if(!early_loading_trace_v46(services.debug,error))return reject(error);
    if(failed){error=failure;return LifecycleStepV36::failed;}
    if(*level.state130!=6||*level.field194||*level.owner194)return reject("Stage6 trace mutated actual Level194/loadingstate");
    if(!services.player.actual_owner||!services.player.identity||!services.player.byte6c9)return reject("Required actual PlayerManager byte6c9 borrow");
    *services.player.byte6c9=1;
    if(!services.player_update)return reject("Required original PlayerManager.Update378fb4");
    if(!services.player_update(services.player,error))return reject(error.empty()?"PlayerManager.Update failed":error);
    if(failed){error=failure;return LifecycleStepV36::failed;}
    if(*level.state130!=6||*level.field194||*level.owner194)return reject("PlayerManager.Update mutated actual Level194/loadingstate");
    // Native pure storage C1 recovers original empty vector, no service stub.
    auto manager=std::make_shared<GameEventManagerV50>(services.modern_allocation_policy);
    *level.owner194=std::move(manager);*level.field194=reinterpret_cast<std::uintptr_t>(level.owner194->get());started=true;
   }
   if(!*level.owner194||*level.field194!=reinterpret_cast<std::uintptr_t>(level.owner194->get()))return reject("Actual Level194 no longer aliases its SAME retained manager");
   auto manager=*level.owner194; // Pin SAME receiver through policy/callback boundaries.
   const auto result=manager->load_step(services.actual_events);
   if(failed){error=failure;return LifecycleStepV36::failed;}
   if(*level.state130!=6||level.owner194->get()!=manager.get()||level.owner194->owner_before(manager)||manager.owner_before(*level.owner194)||*level.field194!=reinterpret_cast<std::uintptr_t>(manager.get()))return reject("GameEvent.Load mutated actual loadingstate/retained194 alias");
   if(result==GameEventLoadStatusV50::failed)return reject(manager->diagnostics().error);
   if(result==GameEventLoadStatusV50::pending){error.clear();return LifecycleStepV36::pending;}
   done=true;error.clear();return LifecycleStepV36::complete;
  }catch(const std::exception& e){return reject(e.what());}catch(...){return reject("Stage6 GameEvent provider threw");}
 };
}
}
