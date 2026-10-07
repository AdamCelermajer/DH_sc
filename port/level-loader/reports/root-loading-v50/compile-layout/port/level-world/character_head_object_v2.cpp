#include "character_head_object_v2.hpp"
namespace dh2::character {
bool character_command_head_object_v2(const CharacterHeadObjectBorrowV2& b,std::uintptr_t target,std::string& e){
 if(!b.controller||!b.heading||!b.source_heading_active1b5){e="Required SAME controller/Character heading storage";return false;}
 const auto& c=*b.controller;if(!c.forced&&(c.global_blocked||c.locked))return true;
 bool remote{};if(!b.services.remotely_updated||!b.services.remotely_updated(b.services.context,remote,e))return false;
 if(remote)return true;
 if(!target){if(!*b.source_heading_active1b5)return true;if(!b.source_vec3_origin){e="Required actual Point3D<float> Vec3fOrigin";return false;}return b.heading->character_head_towards(b.source_vec3_origin,e);}
 const float* other=nullptr;const float* owner=nullptr;
 if(!b.services.target_position||!b.services.target_position(b.services.context,target,other,e)||!other||
    !b.services.target_position(b.services.context,c.owner,owner,e)||!owner)return false;
 const float difference[]{other[0]-owner[0],other[1]-owner[1],other[2]-owner[2]};
 return b.heading->character_head_towards(difference,e);
}
}
