#pragma once
#include <string>
#include <utility>

namespace dh2::ui {
struct DeferredHudRefreshV1 {
 bool display_right_hud_pending=false;
 bool fill_action_icon_pending=false;
 void request() noexcept {
  display_right_hud_pending=true;
  fill_action_icon_pending=true;
 }
};

// Source NativeSwapEquipment requires the gameplay HUD callbacks after its
// native mutation. Validate that receiver before mutating the selected set.
template<class SwapEquipment>
bool swap_equipment_and_defer_hud_refresh_v1(DeferredHudRefreshV1& pending,
                                             bool hud_receiver_ready,
                                             SwapEquipment&& swap_equipment,
                                             std::string& error) {
 if(!hud_receiver_ready){
  error="NativeSwapEquipment requires the bound authored gameplay HUD refresh owner";
  return false;
 }
 if(!std::forward<SwapEquipment>(swap_equipment)(error))return false;
 pending.request();error.clear();return true;
}

// NativeSwapEquipment re-enters ActionScript after its native mutation. Run
// those callbacks only at a safe boundary, in original order, and retain the
// failed step when the shared SWF core is busy.
template<class DisplayRightHud,class FillActionIcon>
bool flush_deferred_hud_refresh_v1(DeferredHudRefreshV1& pending,
                                   DisplayRightHud&& display_right_hud,
                                   FillActionIcon&& fill_action_icon,
                                   std::string& error) {
 if(pending.display_right_hud_pending){
  if(!std::forward<DisplayRightHud>(display_right_hud)(error)){
   if(error=="SWF core busy"){error.clear();return true;}
   return false;
  }
  pending.display_right_hud_pending=false;
 }
 if(pending.fill_action_icon_pending){
  if(!std::forward<FillActionIcon>(fill_action_icon)(error)){
   if(error=="SWF core busy"){error.clear();return true;}
   return false;
  }
  pending.fill_action_icon_pending=false;
 }
 error.clear();
 return true;
}
}
