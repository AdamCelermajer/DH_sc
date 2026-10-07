#pragma once
#include "character_loot_scatter_v8.hpp"
#include "point3d_normalize_v2.hpp"
#include "canonical_point3d_globals_v1.hpp"
namespace dh2::character {
inline bool character_loot_scatter_connected_v9(data::LootRandom8V2& same_random,const float* source,const float* killer,float* out,std::string& error){
 LootScatterServicesV8 s;s.normalize=[](void*,float* xyz,std::string& e){if(!dh2_point3d_normalize_v2(xyz)){e="Required source Point3D receiver";return false;}return true;};s.vec3f_k=world::canonical_vec3_k_v1().data();
 return character_loot_scatter_v8(same_random,source,killer,out,s,error);
}
}
