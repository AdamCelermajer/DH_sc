#include "character_world_runtime_v1.hpp"
#include "world_click_fields_owner_v1.hpp"
#include "world_click_application_v1.hpp"
#include "world_touch_projection_v1.hpp"
#include "floors.hpp"
#include "../game-data/design_settings.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include "renderer_world_touch_live_v2.inc"
int main(){dh2::world::RendererWorldTouchLiveV2 owner;bool hit=true;std::string error;
 if(owner.dispatch(1,1,true,hit,error)||hit||error.empty())return 1;
 unsigned calls=0;std::array<float,3> seen{};bool seen_release=false;
 owner.same.source_click_move_v20=[&](const std::array<float,3>& point,bool released,std::string&){++calls;seen=point;seen_release=released;return true;};
 const std::array<float,3> press{1,2,3},release{4,5,6};
 if(!owner.invoke_source_click_move_v20(press,false,error)||calls!=1||seen!=press||seen_release)return 2;
 if(!owner.invoke_source_click_move_v20(release,true,error)||calls!=2||seen!=release||!seen_release)return 3;
 return 0;}
