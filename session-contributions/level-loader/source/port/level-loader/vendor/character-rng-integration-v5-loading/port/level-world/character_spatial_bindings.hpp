#pragma once
#include "../script-runtime/script_runtime.h"
#include <cstddef>
#include <cstdint>
namespace dh2::character {
struct SpatialBindings40 {
 const float* owner_position=nullptr; // Raw original GameObject+160/+164/+168.
 void* context=nullptr;
 // Genuine ObjectManager GetObjectByName(name,-1,false,nullptr), then GameObject
 // conversion boundary. Return0 delivered; null position is a source miss.
 int(*named_position)(void*,const char*,const float**)=nullptr;
 // Source projected kind7 identity: native64 GameObject→borrowed position.
 // No light-userdata2 acceptance and no pointer truncation. Null identity is
 // source miss without calling this service. Nonzero status is provider failure.
 int(*userdata_position)(void*,std::uintptr_t,const float**)=nullptr;
 std::uint64_t reserved=0;
};
static_assert(sizeof(SpatialBindings40)==40);
// Receiver/services and positions must outlive callbacks. Resolved positions
// remain valid through later resolution calls in the SAME callback; values may
// change there. Between reads BOTH positions only AFTER both lookups finish.
// No scaling, sqrt, Scene mutation or body movement is performed.
}
extern "C" int dh2_character_get_position(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
extern "C" int dh2_character_get_distance_from(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
extern "C" int dh2_character_get_distance_between(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
// Borrowed source receiver must outlive VM callbacks/finalizers. Existing
// source-values projection and >16-argument rejection remain unchanged.
extern "C" int dh2_character_spatial_bind(dh2_script_vm*,const dh2::character::SpatialBindings40*);
