#include "character_fx_floor_query_v3.hpp"
#include <cstring>
namespace dh2::fx {
bool character_fx_floor_query_v3(const navigation::CollisionWorld* world,
 const float position[3],float normal[3],std::string& error){
 if(!world||!position||!normal){error="Required same PFWorld for AnimatedFX floor query";return false;}
 navigation::HeightHit hit{};
 std::memcpy(hit.normal,normal,sizeof(hit.normal));
 // Original 492da8/492e78: NULL height/room/floor and includeSpecial=false.
 // The native selector's height output is discarded, just as the source call.
 const int status=dh2_nav_world_height(&hit,world,position,0);
 if(status<0){error="Required valid PFWorld AnimatedFX floor query";return false;}
 std::memcpy(normal,hit.normal,sizeof(hit.normal));
 return true;
}
}
