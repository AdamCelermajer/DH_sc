#include "hud_player_values.hpp"
#include <cstring>
#include <limits>
namespace {
using namespace dh2::ui;
template<class T> bool aligned(const T* p){return p && (reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0);}
bool valid(const HudValuesState24* s,const HudValueServices16* svc){return aligned(s)&&aligned(svc)&&s->reserved==0;}
std::int32_t signed_word(std::uint32_t x){std::int32_t y;std::memcpy(&y,&x,4);return y;}
int call(HudValuesState24* s,const HudValueServices16* svc,const HudValueRequest32& r,HudValueResponse16& o){o={};return svc->invoke&&svc->invoke(svc->context,s,&r,&o)==1?0:-2;}
int quotient(HudValuesState24* s,const HudValueServices16* svc,unsigned current,unsigned maximum,HudValueClip index,std::int32_t& q){
 const auto numerator=signed_word(std::uint32_t(s->resolved[current])*100u);const auto denominator=s->resolved[maximum];
 if(denominator){q=signed_word(std::uint32_t(std::int64_t(numerator)/std::int64_t(denominator)));return 0;}
 HudValueResponse16 out{};const int result=call(s,svc,{HudValueOperation::divide_zero,index,numerator,0,0,0},out);q=out.value;return result;
}
}
namespace dh2::ui {
const char* hud_value_clip_path(HudValueClip clip) noexcept {switch(clip){case HudValueClip::hp:return "HUDelements.HealthBars.player.bar_hp";case HudValueClip::mp:return "HUDelements.HealthBars.player.bar_mp";case HudValueClip::xp:return "HUDelements.HealthBars.player.bar_xp";case HudValueClip::distress:return "HUDelements.HealthBars.btn_potion.DistressGlow";case HudValueClip::hurt:return "_root.HurtCorners";}return nullptr;}
}
extern "C" int dh2_ui_hud_goto_frame(dh2::ui::HudValuesState24* s,dh2::ui::HudValueClip index,std::uintptr_t fx,std::uintptr_t clip,std::int32_t frame,std::uint32_t play,const dh2::ui::HudValueServices16* svc) noexcept {
 using namespace dh2::ui;if(!valid(s,svc)||unsigned(index)>4)return -1;if(!clip)return 0;
 HudValueResponse16 out{};int rc=call(s,svc,{HudValueOperation::is_sprite,index,2,0,fx,clip},out);if(rc||!out.value)return rc;
 rc=call(s,svc,{HudValueOperation::goto_frame,index,frame,0,fx,clip},out);if(rc)return rc;
 return call(s,svc,{HudValueOperation::set_play_state,index,signed_word(play^1u),0,fx,clip},out);
}
extern "C" int dh2_ui_hud_player_values(dh2::ui::HudValuesState24* s,const dh2::ui::HudValueServices16* svc) noexcept {
 using namespace dh2::ui;if(!valid(s,svc)||!aligned(s->resolved)||s->count<44||s->count>65536)return -1;
 // Re-read each source word in original order. Runtime zero-division services
 // may mutate the borrowed sheet; later quotients therefore see live values.
 std::int32_t hp,mp,xp;int rc=quotient(s,svc,36,38,HudValueClip::hp,hp);if(rc)return rc;
 hp=signed_word(std::uint32_t(hp)-1u);
 rc=quotient(s,svc,41,43,HudValueClip::mp,mp);if(rc)return rc;
 mp=signed_word(std::uint32_t(mp)-1u);if(hp<0)hp=0;
 rc=quotient(s,svc,33,34,HudValueClip::xp,xp);if(rc)return rc;
 if(xp>=99)xp=99;
 for(unsigned i=0;i<5;++i){const auto index=HudValueClip(i);const auto fx=s->render_fx;HudValueResponse16 out{};
  rc=call(s,svc,{HudValueOperation::resolve_clip,index,0,0,fx,0},out);if(rc)return rc;
  if(i==0&&hp>=99)hp=99;if(i==1&&mp>=99)mp=99;
  const auto frame=i==1?mp:i==2?xp:hp;
  rc=dh2_ui_hud_goto_frame(s,index,fx,out.clip,frame,0,svc);if(rc)return rc;
 }
 return 0;
}
