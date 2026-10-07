#pragma once
#include "character_target_search.hpp"
#include "actor_rotation.hpp"
#include <cmath>
#include <cstring>
namespace dh2::character::skills {
// ObjectSearcher Search calls GameObject.GetLookAtVec393ae4, whose sole angle
// read is actual EulerZ at+174. Desired heading+178 is a different source field.
// Borrow the SAME retained actor's RotationState; preserve all other search
// query flags, cache/target fields and its canonical identity.
inline int character_world_target_pose_v2(target_search::Object48& search,
 std::uintptr_t identity,const float* position160,const actor::RotationState* source_rotation){
 if(!identity||!position160||!source_rotation||!std::isfinite(source_rotation->rotation[2]))return -1;
 for(unsigned i=0;i<3;++i)if(!std::isfinite(position160[i]))return -1;
 search.identity=identity;
 std::memmove(search.position,position160,12);
 search.rotation=source_rotation->rotation[2];
 return 0;
}
// Character cached payload begins at ff8. Source IsCharacterValid4a1ab8
// compares detection(+1314) against sneak(+1310) as SIGNED words. Borrow
// the current resolved payload, including negative values, without resolving
// equipment again or introducing an independent property authority.
inline int character_world_target_cached_fields_v2(target_search::Object48& search,
 const std::int32_t* resolved,std::size_t count){
 if(!resolved||count<200)return -1;
 search.character_word1310=resolved[198];
 search.character_word1314=resolved[199];
 return 0;
}
}
