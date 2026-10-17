#include "../deferred_hud_refresh_v1.hpp"
#include <string>
#include <vector>

int main(){
 dh2::ui::DeferredHudRefreshV1 pending;std::string error;unsigned mutations=0;
 if(dh2::ui::swap_equipment_and_defer_hud_refresh_v1(pending,false,
      [&](std::string&){++mutations;return true;},error)||mutations||
    pending.display_right_hud_pending||pending.fill_action_icon_pending||
    error!="NativeSwapEquipment requires the bound authored gameplay HUD refresh owner")return 1;
 if(!dh2::ui::swap_equipment_and_defer_hud_refresh_v1(pending,true,
      [&](std::string&){++mutations;return true;},error)||mutations!=1||
    !error.empty()||!pending.display_right_hud_pending||!pending.fill_action_icon_pending)return 2;
 std::vector<int> calls;
 const auto busy_display=[&](std::string& e){calls.push_back(1);e="SWF core busy";return false;};
 const auto fill=[&](std::string&){calls.push_back(2);return true;};
 if(!dh2::ui::flush_deferred_hud_refresh_v1(pending,busy_display,fill,error)||
    !error.empty()||calls!=std::vector<int>{1}||!pending.display_right_hud_pending||
    !pending.fill_action_icon_pending)return 3;
 const auto display=[&](std::string&){calls.push_back(1);return true;};
 if(!dh2::ui::flush_deferred_hud_refresh_v1(pending,display,fill,error)||
    !error.empty()||calls!=std::vector<int>{1,1,2}||
    pending.display_right_hud_pending||pending.fill_action_icon_pending)return 4;
 return 0;
}
