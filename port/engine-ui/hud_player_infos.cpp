#include "hud_player_infos.hpp"
#include <cmath>
#include <cstring>
#include <limits>
using namespace dh2::ui;
namespace {
bool aligned(const void* p,std::uintptr_t a){return p&&reinterpret_cast<std::uintptr_t>(p)%a==0;}
int call(const HudInfosServices16* s,HudInfosOperation op,std::uint32_t i,std::int32_t v,std::uint32_t type,std::uintptr_t actor,std::uintptr_t object,HudInfosResponse16& out){if(!s->invoke)return -2;HudInfosRequest32 r{op,i,v,type,actor,object};return s->invoke(s->context,&r,&out)?0:-2;}
std::int32_t signed_word(std::uint32_t u){std::int32_t i;std::memcpy(&i,&u,4);return i;}
std::int32_t trunc32(float f){if(std::isnan(f))return 0;if(f>=2147483648.f)return INT32_MAX;if(f<=-2147483648.f)return INT32_MIN;return static_cast<std::int32_t>(f);}
}
namespace dh2::ui {const char* hud_infos_member_name(HudInfosMember m){static const char* names[]={"PlayerActive","LEVEL","HP_PCT","HP_LOWPCT","MP_PCT","XP_PCT","SPELL_PCT","SPELL_MP","SKILL1_PCT","SKILL1_MP","SKILL2_PCT","SKILL2_MP","SKILL3_PCT","SKILL3_MP","NB_POTIONS","AVAIL_POINTS","TouchToMove"};const auto i=static_cast<unsigned>(m);return i<17?names[i]:nullptr;}}
extern "C" int dh2_ui_hud_player_infos_v1(const HudInfosInput24* in,const HudInfosServices16* services){
 if(!aligned(in,8)||!aligned(services,8)||in->reserved)return -1;
 if(in->argument_count!=2&&in->argument_count!=3)return 0;
 if(!in->output_object)return -1;
 const auto object=in->output_object;HudInfosResponse16 response{};
 int e=call(services,HudInfosOperation::player,static_cast<std::uint32_t>(in->player_index),in->argument_count==3?static_cast<std::int32_t>(in->remote):0,0,0,object,response);if(e)return e;
 auto* a=reinterpret_cast<const HudInfosActor32*>(response.identity);
 if(a&&(!aligned(a,8)||a->reserved0||a->reserved1))return -1;
 const auto actor=a?a->identity:0;
 auto request=[&](HudInfosOperation op,std::uint32_t i,std::int32_t v,HudInfosResponse16& out){return call(services,op,i,v,0,actor,object,out);};
 auto write=[&](HudInfosMember member,std::int32_t value,std::uint32_t type){HudInfosResponse16 out{};return call(services,HudInfosOperation::write_member,static_cast<unsigned>(member),value,type,actor,object,out);};
 if(!a||a->removed||!a->active)return write(HudInfosMember::player_active,0,1);
 if(!aligned(a->resolved,4)||a->count<44||a->count>65536||!actor)return -1;
 std::int32_t slots[3]{},usable[3]{};float fractions[3]{};
 for(unsigned i=0;i<3;++i){response={};if((e=request(HudInfosOperation::skill_slot,i,0,response)))return e;slots[i]=response.value;}
 for(unsigned i=0;i<3;++i)if(slots[i]!=-1){response={};if((e=request(HudInfosOperation::skill_usable,static_cast<unsigned>(slots[i]),0,response)))return e;usable[i]=response.value;response={};if((e=request(HudInfosOperation::skill_level,static_cast<unsigned>(slots[i]),0,response)))return e;const auto level=response.value;response={};if((e=request(HudInfosOperation::skill_info,static_cast<unsigned>(slots[i]),level,response)))return e;fractions[i]=response.fraction;}
 response={};if((e=request(HudInfosOperation::spell_info,0,0,response)))return e;const float spell=response.fraction;
 if((e=write(HudInfosMember::player_active,1,1)))return e;
 response={};if((e=request(HudInfosOperation::property_int,19,0,response)))return e;if((e=write(HudInfosMember::level,response.value,2)))return e;
 auto pct=[&](unsigned cur,unsigned maximum,HudInfosMember member){const std::int32_t num=signed_word(std::uint32_t(a->resolved[cur])*100u),den=a->resolved[maximum];std::int32_t result;
  if(!den){HudInfosResponse16 out{};int rc=request(HudInfosOperation::divide_zero,cur,num,out);if(rc)return rc;result=out.value;}else if(num==INT32_MIN&&den==-1)result=INT32_MIN;else result=num/den;
  return write(member,result,2);};
 if((e=pct(36,38,HudInfosMember::hp_pct)))return e;
 response={};if((e=request(HudInfosOperation::low_health_constant,0,0,response)))return e;if((e=write(HudInfosMember::hp_lowpct,response.value,2)))return e;
 if((e=pct(41,43,HudInfosMember::mp_pct)))return e;if((e=pct(33,34,HudInfosMember::xp_pct)))return e;
 if((e=write(HudInfosMember::spell_pct,trunc32(spell*100.f),2)))return e;
 response={};if((e=request(HudInfosOperation::spell_usable,0,0,response)))return e;if((e=write(HudInfosMember::spell_mp,static_cast<std::int32_t>(static_cast<std::uint32_t>(response.value)&255u),1)))return e;
 for(unsigned i=0;i<3;++i){if((e=write(static_cast<HudInfosMember>(8+2*i),trunc32(fractions[i]*100.f),2)))return e;if((e=write(static_cast<HudInfosMember>(9+2*i),static_cast<std::int32_t>(static_cast<std::uint32_t>(usable[i])&255u),1)))return e;}
 response={};if((e=request(HudInfosOperation::potions,0,0,response)))return e;if((e=write(HudInfosMember::potions,response.value,2)))return e;
 response={};if((e=request(HudInfosOperation::property_int,148,0,response)))return e;if((e=write(HudInfosMember::available_points,response.value!=0,1)))return e;
 response={};if((e=request(HudInfosOperation::saved_dpad,0,0,response)))return e;return write(HudInfosMember::touch_to_move,response.value==0,1);
}
