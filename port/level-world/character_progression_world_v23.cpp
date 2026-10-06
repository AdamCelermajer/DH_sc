#include "character_progression_world_v23.hpp"
#include <cmath>
#include <cstring>
namespace dh2::character {namespace {
bool fail(std::string& e,const char* text){if(e.empty())e=text;return false;}
std::int32_t signed_word(std::uint32_t bits){std::int32_t value;std::memcpy(&value,&bits,4);return value;}
std::int32_t asr8(std::int32_t v){const auto bits=std::uint32_t(v);return signed_word((bits>>8)|((bits>>31)?0xff000000u:0));}
float setting(const data::DesignSettingsProjection176& s,unsigned offset){float f;std::memcpy(&f,&s.words[offset/4],4);return f;}
bool valid(const ProgressionActorV1& a){return a.identity&&a.state&&a.properties&&
 a.properties->base==a.state->base.data()&&a.properties->saved==a.state->saved.data()&&
 a.properties->gear==a.state->gear.data()&&a.properties->resolved==a.state->resolved.data()&&
 !dh2_property_validate(a.properties)&&(!a.save||a.save->character()==a.identity);}
}
bool CharacterProgressionWorldV23::route(KillActor56& victim,const KillRequest56& q,
 KillResponse16& out,bool& handled,std::string& error){
 handled=q.service==kill_distribute_xp;if(!handled)return true;
 if(q.target!=victim.identity){error="Source Kill XP victim identity differs";return false;}
 if(!distribute(q.subject,q.target,error))return false;out={};return true;
}
bool CharacterProgressionWorldV23::distribute(std::uintptr_t killer_id,std::uintptr_t victim_id,std::string& error){
 error.clear();receipt_={};receipt_.killer=killer_id;receipt_.victim=victim_id;
 if(running_||!services_.owner||!services_.players||!services_.settings||!services_.actor){
  return fail(error,"Required same nonreentrant World/PlayerManager/XP settings actors");}
 running_=true;struct Guard{bool& b;~Guard(){b=false;}}guard{running_};
 ProgressionActorV1 victim{},killer{};
 if(!victim_id||!services_.actor(victim_id,victim,error)||!valid(victim)||victim.identity!=victim_id)return fail(error,"Required actual victim XP/property owner");
 if(killer_id&&(!services_.actor(killer_id,killer,error)||!valid(killer)||killer.identity!=killer_id))return fail(error,"Required actual credited killer Character owner");
 const auto& source=services_.source;
 auto debug=[&](const char* key){bool flag{};return source.debug&&source.debug(key,flag,error);};
 float base=float(asr8(victim.properties->resolved[35]));if(!(base>0))base=0;
 if(!debug("isTracingXPDistributionCst")||!debug("isTracingXPDistribution")||!debug("isTracingXPDistribution"))return fail(error,"Required source XP Debug load/query");
 const auto count=*services_.players->character_count_field();receipt_.source_count=count;
 if(count>4||count<0)return fail(error,"Source XP count assertion outside proven actual PlayerManager domain0..4");
 float shares[4]{};
 for(std::int32_t index=0;index<count;++index){
  player::PlayerInfoFieldsV1* info{};
  if(!services_.players->get_player(index,true,info,error))return false;
  if(!info||!info->character660)return fail(error,"Required source XP first-pass null Character assertion policy");
  ProgressionActorV1 player{};if(!services_.actor(info->character660,player,error)||!valid(player))return fail(error,"Required same XP recipient property owner");
  shares[index]=progression_scaled_xp_v1(base,asr8(player.properties->resolved[19]),asr8(victim.properties->resolved[19]),*services_.settings);
  if(!debug("isTracingXPDistribution"))return false;
  if(shares[index]>=0){
   const auto& origin=killer_id?killer:victim;
   const float dx=player.x-origin.x,dy=player.y-origin.y;
   volatile float dx2=dx*dx,dy2=dy*dy;const float distance=std::sqrt(dx2+dy2);
   if(player.identity==killer_id||setting(*services_.settings,0xa0)>=distance)++receipt_.qualifying;
   else shares[index]=0;
   if(!debug("isTracingXPDistribution"))return false;
  }
 }
 if(!receipt_.qualifying)return debug("isTracingXPDistribution")&&debug("isTracingXPDistribution")&&debug("isTracingXPDistribution");
 const float deduction=float(receipt_.qualifying-1)*setting(*services_.settings,0xa4);
 if(!debug("isTracingXPDistribution"))return false;
 receipt_.recipients.resize(std::size_t(count));
 for(std::int32_t index=0;index<count;++index){
  // Original second GetPlayer(index,true) is fresh, rather than a cached roster.
  player::PlayerInfoFieldsV1* info{};if(!services_.players->get_player(index,true,info,error))return false;
  if(!info||!info->character660)continue;
  ProgressionActorV1 player{};if(!services_.actor(info->character660,player,error)||!valid(player))return fail(error,"Required actual second-pass XP recipient");
  const float scaled=((100.f-deduction)*shares[index])/100.f;
  if(scaled>=0){
   auto raw=progression_award_raw_v1(scaled);auto& result=receipt_.recipients[std::size_t(index)];
   if(!progression_give_xp_v1(player,raw,true,source,result,error))return false;
   if(result.accepted){++receipt_.given;
    // Fresh post-Give local/source policy borrow: notifications may have
    // synchronously mutated the same actor. No ownership/state copy is written.
    ProgressionActorV1 after{};if(!services_.actor(player.identity,after,error)||!valid(after))return fail(error,"Required post-XP local Character borrow");
    if(after.local){
     if((after.save?after.save->unlocked_difficulty():-1)<after.current_difficulty)raw=256;
     const auto integer=asr8(progression_modified_xp_v1(raw,after.properties->resolved[201]));
     if(!source.xp_text||!source.xp_text(victim,after,integer,error))return fail(error,"Required source XPColor/style/localized scrolling text");
     ++receipt_.local_texts;
    }
   }
  }
  if(!debug("isTracingXPDistribution"))return false;
 }
 return debug("isTracingXPDistribution")&&debug("isTracingXPDistribution");
}
}
