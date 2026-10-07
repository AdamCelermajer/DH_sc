#pragma once
#include "character_fx_floor_query_v3.hpp"
#include <algorithm>
namespace dh2::fx {
// SyncIrrData492aa0: anchor PFObject.normal (+1ec) is an input, not
// an orientation. The resulting local normal is discarded by the caller.
inline bool character_fx_floor_sync_v29(const navigation::CollisionWorld* world,
 const float position[3], const float* anchor_normal, float normal[3],
 std::string& error) {
 if(!position||!normal){error="Required actual FX position/normal storage";return false;}
 if(anchor_normal)std::copy_n(anchor_normal,3,normal);
 else std::fill_n(normal,3,0.f);
 const auto squared=[](const float* n){return (n[0]*n[0]+n[1]*n[1])+n[2]*n[2];};
 // Original anchor branch skips even the World lookup for a zero input.
 if(anchor_normal&&squared(normal)==0.f)return true;
 if(!character_fx_floor_query_v3(world,position,normal,error))return false;
 // Only the anchored branch installs this temporary literal after the query.
 if(anchor_normal&&squared(normal)==0.f){normal[0]=0.f;normal[1]=0.f;normal[2]=1.f;}
 return true;
}
}
