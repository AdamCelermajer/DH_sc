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
 return owner.dispatch(1,1,true,hit,error)||hit||error.empty()?1:0;}
