#pragma once
#include "../level-world/character_animation_swoosh_v4.hpp"
namespace dh2::audio {
// Literal _SetAnimStep3caa10..3cab20. At3cab18 the OFFHAND FX result
// overwrites r8 (sound fallback), while r7 retains MAIN FX fallback.
// Do not simplify these two flags into independent missing sound/FX states.
inline int audio_animation_swoosh_v38(bool anchored,
 const character::AnimationSwooshServicesV4& s,bool& fallback_sound,
 bool& fallback_fx,std::string& error){
 auto fail=[&](const char* what){error=std::string("Required source animation Swoosh ")+what;return -1;};
 if(!s.equipped)return fail("equipped item query");
 std::uintptr_t main=0,off=0;
 if(s.equipped(s.context,1,main))return fail("main-hand query");
 if(s.equipped(s.context,2,off))return fail("offhand query");
 auto values=[&](std::uintptr_t item,std::int32_t& sound,std::int32_t& fx){
  sound=fx=-1;if(!item)return 0;
  if(!s.effects||s.effects(s.context,item,sound,fx))return -1;return 0;
 };
 std::int32_t sound=-1,fx=-1;
 if(values(main,sound,fx))return fail("main-hand sound effects");
 bool sf=sound==-1;
 if(!sf&&(!s.play_sound||s.play_sound(s.context,sound)))return fail("main-hand sound delivery");
 if(values(main,sound,fx))return fail("main-hand FX effects");
 const bool ff=fx==-1;
 if(!ff&&(!s.play_fx||s.play_fx(s.context,fx,anchored)))return fail("main-hand FX delivery");
 if(sf){
  if(values(off,sound,fx))return fail("offhand sound effects");
  if(sound!=-1){if(!s.play_sound||s.play_sound(s.context,sound))return fail("offhand sound delivery");sf=false;}
 }
 if(ff){
  if(values(off,sound,fx))return fail("offhand FX effects");
  sf=fx==-1;
  if(!sf&&(!s.play_fx||s.play_fx(s.context,fx,anchored)))return fail("offhand FX delivery");
 }
 fallback_sound=sf;fallback_fx=ff;return 0;
}
}
