#include "player_progression_v1.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
namespace dh2::character {
namespace {
std::int32_t word(std::uint32_t x){std::int32_t y;std::memcpy(&y,&x,4);return y;}
std::int32_t asr8(std::int32_t x){const auto u=std::uint32_t(x);return word((u>>8)|((u>>31)?0xff000000u:0));}
std::int32_t integer(float f){
 std::uint32_t b;std::memcpy(&b,&f,4);const auto e=(b>>23)&255;
 if(e==255&&(b&0x7fffff))return 0;if(e<127)return 0;
 if(e>=158)return b>>31?INT32_MIN:INT32_MAX;
 auto v=(b&0x7fffff)|0x800000u;v=e>=150?v<<(e-150):v>>(150-e);
 return b>>31?-std::int32_t(v):std::int32_t(v);
}
float setting(const data::DesignSettingsProjection176& d,unsigned offset){float f;std::memcpy(&f,&d.words[offset/4],4);return f;}
bool valid(const ProgressionActorV1& a){
 return a.identity&&a.state&&a.properties&&a.properties->base==a.state->base.data()&&
 a.properties->saved==a.state->saved.data()&&a.properties->gear==a.state->gear.data()&&
 a.properties->resolved==a.state->resolved.data()&&!dh2_property_validate(a.properties)&&
 (!a.save||a.save->character()==a.identity);
}
bool fail(std::string& e,const char* text){e=text;return false;}
bool debug(const ProgressionServicesV1& s,const char* name,bool& result,std::string& e){
 if(!s.debug)return fail(e,"Required XP source Debug load/query provider");return s.debug(name,result,e);
}
bool maximum(ProgressionActorV1& a,const ProgressionServicesV1& s,std::int32_t& out,std::string& e){
 if(!s.constant)return fail(e,"Required XP source CharacterDesign constant provider");
 if(!s.constant("MaxLevelBNormal",out,e))return false;
 const auto unlocked=a.save?a.save->unlocked_difficulty():-1;
 if(unlocked==1)return s.constant("MaxLevelCHard",out,e);
 if(unlocked==2)return s.constant("MaxLevelDVeryHard",out,e);return true;
}
bool level_up(ProgressionActorV1& a,std::int32_t carry,const ProgressionServicesV1& s,
 ProgressionResultV1& r,std::string& e){
 std::int32_t cap;bool ignored{};if(!maximum(a,s,cap,e))return false;
 auto* p=a.properties;if(asr8(p->resolved[19])>=cap)return true;
 // LevelUp reloads this exact CharacterDesign row before class recalculation.
 // Do not let an invalid renderer borrow fall through to the saved-property
 // defaults: that would award a level while silently dropping authored class
 // progression (including the visible XP threshold and derived point totals).
 if(!a.actors||!a.classes||!a.class_count||a.actor_index<0||
    std::size_t(a.actor_index)>=a.actors->rows.size())
  return fail(e,"Required LevelUp same actor/class cache provider");
 if(!debug(s,"isTracingChar_Stats",ignored,e))return false;
 if(dh2_property_add(p,19,256)||dh2_property_set_int(p,33,0))return fail(e,"Source LevelUp property prefix failed");
 r.leveled=true;
 std::copy_n(p->defaults,224,a.state->base.begin());
 a.state->base=a.actors->rows[std::size_t(a.actor_index)];
 if(dh2_class_recalc_base(a.classes,a.class_count,a.state->base.data(),p))return fail(e,"Source LevelUp class recalculation failed");
 if(!s.regen_full)return fail(e,"Required LevelUp source RegenHP/MP provider");
 if(!s.regen_full(a,false,e)||!s.regen_full(a,true,e))return false;
 if(a.save){a.save->set_player_level(asr8(p->resolved[19]));
  if(!s.save)return fail(e,"Required LevelUp original SG_Save delivery provider");if(!s.save(a,e))return false;}
 if(a.player){if(!s.level_presentation)return fail(e,"Required source LevelUp localized/menu/FX/tutorial/achievement branch");
  if(!s.level_presentation(a,asr8(p->resolved[19]),e))return false;}
 // Original performs ONE level increment, then clamps excessive integer
 // carry. It does not recursively LevelUp a giant award.
 if(asr8(p->resolved[19])<cap){
  if(carry>asr8(p->resolved[34]))carry=word(std::uint32_t(asr8(p->resolved[34]))-1);
  if(dh2_property_add(p,33,word(std::uint32_t(carry)<<8)))return fail(e,"Source LevelUp XP carry failed");
 }return true;
}
}
float progression_scaled_xp_v1(float base,std::int32_t player,std::int32_t victim,const data::DesignSettingsProjection176& d) noexcept{
 auto delta=word(std::uint32_t(victim)-std::uint32_t(player));delta=std::min(delta,integer(setting(d,0x9c)));
 float pct=100.f;
 if(delta) {const auto factor=integer(setting(d,delta>0?0x8c:0x90));
  const auto product=word(std::uint32_t(factor)*std::uint32_t(delta));pct=float(product)+100.f;}
 const float low=setting(d,0x98),high=setting(d,0x94);
 if(!(low<pct))pct=low;if(!(high>pct))pct=high;
 const float fraction=pct/100.f;return fraction*base;
}
std::int32_t progression_modified_xp_v1(std::int32_t raw,std::int32_t bonus) noexcept{
 const auto pct=word(std::uint32_t(bonus)+0x6400u)/100;
 return asr8(word(std::uint32_t(pct)*std::uint32_t(raw)));
}
std::int32_t progression_award_raw_v1(float scaled) noexcept{return word(std::uint32_t(integer(scaled+1.f))<<8);}
bool progression_give_xp_v1(ProgressionActorV1& a,std::int32_t raw,bool stats,
 const ProgressionServicesV1& s,ProgressionResultV1& r,std::string& e){
 e.clear();r={};r.raw_requested=raw;if(!valid(a))return fail(e,"XP requires same retained property/save actor authority");
 auto* p=a.properties;r.level_before=asr8(p->resolved[19]);r.level_after=r.level_before;r.xp_after=p->resolved[33];
 std::int32_t cap;if(!maximum(a,s,cap,e))return false;
 if(r.level_before>=cap||!a.player||a.remotely_updated||a.current_level_policy){r.complete=true;return true;}
 if(raw<0){if(!s.negative_award_assert)return fail(e,"Required source negative XP Debug assertion policy");if(!s.negative_award_assert(e))return false;}
 bool force{};if(!debug(s,"OneKillLevelUp",force,e))return false;
 if(force)raw=word(std::uint32_t(p->resolved[34])-std::uint32_t(p->resolved[33]));
 if((a.save?a.save->unlocked_difficulty():-1)<a.current_difficulty)raw=256;
 r.raw_added=progression_modified_xp_v1(raw,p->resolved[201]);
 if(dh2_property_add(p,33,r.raw_added))return fail(e,"Source XP Add failed");
 bool ignored{};if(!debug(s,"isTracingChar_Stats",ignored,e))return false;
 if(p->resolved[33]>=p->resolved[34]){
  const auto carry=asr8(word(std::uint32_t(p->resolved[33])-std::uint32_t(p->resolved[34])));
  if(!level_up(a,carry,s,r,e)){r.level_after=asr8(p->resolved[19]);r.xp_after=p->resolved[33];return false;}
  if(p->resolved[33]>p->resolved[34]&&dh2_property_set(p,33,p->resolved[34]))return fail(e,"Source XP post-LevelUp clamp failed");
 }
 if(stats){if(!s.statistics_player_lookup)return fail(e,"Required XP source PlayerManager lookup before empty IncreaseStat");if(!s.statistics_player_lookup(a,e))return false;}
 r.accepted=true;r.complete=true;r.level_after=asr8(p->resolved[19]);r.xp_after=p->resolved[33];return true;
}
bool progression_distribute_xp_v1(ProgressionActorV1* killer,ProgressionActorV1& victim,
 ProgressionActorV1* const* players,std::uint32_t count,const data::DesignSettingsProjection176& d,
 const ProgressionServicesV1& s,std::vector<ProgressionResultV1>& results,std::string& e){
 e.clear();results.clear();if(!valid(victim)||(killer&&!valid(*killer))||count>4||(count&&!players))return fail(e,"Source DistributeXP requires actual victim/player-manager domain <=4");
 float base=float(asr8(victim.properties->resolved[35]));if(!(base>0))base=0;
 bool ignored{};if(!debug(s,"isTracingXPDistributionCst",ignored,e)||!debug(s,"isTracingXPDistribution",ignored,e)||!debug(s,"isTracingXPDistribution",ignored,e))return false;
 float shares[4]{};unsigned qualifying=0;const auto* origin=killer?killer:&victim;
 for(unsigned i=0;i<count;++i){auto* a=players[i];if(!a)return fail(e,"Required source PlayerManager player actor (null assertion policy absent)");if(!valid(*a))return fail(e,"DistributeXP player authority mismatch");
  shares[i]=progression_scaled_xp_v1(base,asr8(a->properties->resolved[19]),asr8(victim.properties->resolved[19]),d);
  if(!debug(s,"isTracingXPDistribution",ignored,e))return false;
  if(shares[i]>=0){const float dx=a->x-origin->x,dy=a->y-origin->y;
   // Original calls two fmul imports followed by fadd; retain their rounding
   // on ARM64 even when surrounding renderer enables contraction.
   volatile float dx2=dx*dx,dy2=dy*dy;const float sum=dx2+dy2;const float distance=std::sqrt(sum);
   if(a==killer||setting(d,0xa0)>=distance){++qualifying;if(!debug(s,"isTracingXPDistribution",ignored,e))return false;}
   else{shares[i]=0;if(!debug(s,"isTracingXPDistribution",ignored,e))return false;}}
 }
 if(!qualifying){return debug(s,"isTracingXPDistribution",ignored,e)&&debug(s,"isTracingXPDistribution",ignored,e)&&debug(s,"isTracingXPDistribution",ignored,e);}
 const float deduction=float(qualifying-1)*setting(d,0xa4);
 if(!debug(s,"isTracingXPDistribution",ignored,e))return false;
 results.resize(count);
 for(unsigned i=0;i<count;++i){const float numerator=(100.f-deduction)*shares[i];const float scaled=numerator/100.f;
  if(scaled>=0){auto& a=*players[i];auto raw=progression_award_raw_v1(scaled);
   if(!progression_give_xp_v1(a,raw,true,s,results[i],e))return false;
   if(results[i].accepted&&a.local){if((a.save?a.save->unlocked_difficulty():-1)<a.current_difficulty)raw=256;
    const auto modified=asr8(progression_modified_xp_v1(raw,a.properties->resolved[201]));
    if(!s.xp_text)return fail(e,"Required source XP HudManager display/color/scrolling-text provider");if(!s.xp_text(victim,a,modified,e))return false;}}
  if(!debug(s,"isTracingXPDistribution",ignored,e))return false;
 }
 return debug(s,"isTracingXPDistribution",ignored,e)&&debug(s,"isTracingXPDistribution",ignored,e);
}
bool ProgressionDeathReceiptV1::dispatch(ProgressionActorV1* killer,ProgressionActorV1& victim,
 bool transition,ProgressionActorV1* const* players,std::uint32_t count,const data::DesignSettingsProjection176& d,
 const ProgressionServicesV1& s,std::vector<ProgressionResultV1>& r,std::string& e){
 if(victim.identity!=victim_)return fail(e,"XP death receipt actor identity mismatch");
 if(!transition||attempted_){r.clear();e.clear();return true;}attempted_=true;
 return progression_distribute_xp_v1(killer,victim,players,count,d,s,r,e);
}
}
