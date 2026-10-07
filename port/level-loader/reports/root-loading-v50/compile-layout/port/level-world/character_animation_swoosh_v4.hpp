#pragma once
#include <cstdint>
#include <string>
namespace dh2::character {
struct AnimationSwooshServicesV4 {
 void* context{};
 // Source inventory GetEquippedItem(kind1 main, kind2 offhand), then source
 // ItemInstance.GetItem row +14 sound/+18 FX. A null item is an actual result.
 int(*equipped)(void*,std::int32_t,std::uintptr_t&){};
 int(*effects)(void*,std::uintptr_t,std::int32_t& sound,std::int32_t& fx){};
 int(*play_sound)(void*,std::int32_t){};
 int(*play_fx)(void*,std::int32_t,bool anchored){};
};
// Whole _SetAnimStep Swoosh prefix 3caa10..3cab20. The fallback FX gate
// deliberately depends on MAIN-hand playback only; offhand is still played.
int character_animation_swoosh_v4(bool anchored,const AnimationSwooshServicesV4&,
 bool& fallback_sound,bool& fallback_fx,std::string&);
}
