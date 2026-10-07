#include "character_animation_swoosh_v4.hpp"
namespace dh2::character {
int character_animation_swoosh_v4(bool anchored,const AnimationSwooshServicesV4& s,
 bool& fallback_sound,bool& fallback_fx,std::string& error){
 auto fail=[&](const char* what){error=std::string("Required source animation Swoosh ")+what;return -1;};
 if(!s.equipped)return fail("equipped item query");
 std::uintptr_t main=0,off=0;if(s.equipped(s.context,1,main))return fail("main-hand query");
 if(s.equipped(s.context,2,off))return fail("offhand query");
 auto values=[&](std::uintptr_t item,std::int32_t& sound,std::int32_t& fx){sound=fx=-1;if(!item)return 0;if(!s.effects||s.effects(s.context,item,sound,fx))return -1;return 0;};
 std::int32_t ms,mf,os,of;if(values(main,ms,mf))return fail("main-hand sound effects");
 bool sound_missing=ms==-1;
 if(!sound_missing&&(!s.play_sound||s.play_sound(s.context,ms)))return fail("main-hand sound delivery");
 if(values(main,ms,mf))return fail("main-hand FX effects");bool fx_missing=mf==-1;
 if(!fx_missing&&(!s.play_fx||s.play_fx(s.context,mf,anchored)))return fail("main-hand FX delivery");
 // Offhand effects are queried only when either corresponding MAIN slot
 // needs fallback, preserving the source service/mutation order.
 if(sound_missing){if(values(off,os,of))return fail("offhand sound effects");if(os!=-1){if(!s.play_sound||s.play_sound(s.context,os))return fail("offhand sound delivery");sound_missing=false;}}
 if(fx_missing){if(values(off,os,of))return fail("offhand FX effects");if(of!=-1&&(!s.play_fx||s.play_fx(s.context,of,anchored)))return fail("offhand FX delivery");}
 fallback_sound=sound_missing;fallback_fx=fx_missing;return 0;
}
}
