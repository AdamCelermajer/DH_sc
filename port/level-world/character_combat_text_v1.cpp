#include "character_combat_text_v1.hpp"
namespace dh2::character::skills {
int character_combat_text_v1(const data::CombatResult& r,std::uintptr_t attacker,std::uintptr_t target,const CombatTextServicesV1& s){
 if(!attacker||!target||!s.follower||!s.position||!s.height||!s.constant||!s.enqueue)return -1;
 bool follower{};if(s.follower(s.context,target,&follower))return -2;if(follower)return 1;
 CombatTextRequestV1 q{};float height{},unused[3]{};
 if(s.position(s.context,target,q.position)||s.height(s.context,target,&height))return -2;
 volatile float raised=q.position[2]+height;q.position[2]=raised;
 if(s.position(s.context,attacker,unused))return -2; // original unused query
 auto text=[&](const char* style,const char* symbol,const char* color){
  std::int32_t id{};const char* value{};
  q.style=style;q.numeric=false;
  if(!s.localized||s.constant(s.context,"StrID",symbol,&id)||s.localized(s.context,id,&value)||!value||s.constant(s.context,"ScrollingCombatText",color,&q.color))return -2;
  q.text=value;return s.enqueue(s.context,&q)?-2:1;
 };
 if(r.outcomes&1)return text("anim_sct_block","INGAME_ATTACK_MISS","MissColor");
 if(r.outcomes&2)return text("anim_sct_block","INGAME_ATTACK_DODGE","DodgeColor");
 if(r.outcomes&4){auto result=text("anim_sct_block","INGAME_ATTACK_BLOCK","BlockColor");if(result<0)return result;}
 const bool offhand=(r.mask&0x10000)!=0;
 if(r.outcomes&0x20){std::int32_t fear{};if(!s.property||s.property(s.context,attacker,offhand?187:143,&fear))return -2;if((std::uint32_t(fear)>>8)!=0){auto result=text("anim_sct_stun","INGAME_ATTACK_FEAR","FearColor");if(result<0)return result;}}
 else if(r.outcomes&0x100){std::int32_t slow{};if(!s.property||s.property(s.context,attacker,offhand?189:146,&slow))return -2;if((std::uint32_t(slow)>>8)!=0){auto result=text("anim_sct_stun","INGAME_ATTACK_SLOW","SlowColor");if(result<0)return result;}}
 if(r.amount<=0)return 1;
 const bool critical=(r.outcomes&8)!=0;
 if(r.mask&0x20000000)q.style="anim_sct_dot";
 else {bool dual{};if(!s.dual_wield||s.dual_wield(s.context,attacker,&dual))return -2;
  q.style=dual?((r.mask&0x04000000)?(critical?"anim_sct_critleft":"anim_sct_normaldamageleft"):(critical?"anim_sct_critright":"anim_sct_normaldamageright")):(critical?"anim_sct_crit":"anim_sct_normaldamage");
 }
 bool player{};if(!s.is_player||s.is_player(s.context,target,&player))return -2;
 if(s.constant(s.context,"ScrollingCombatText",player?(critical?"PlayerCritDamageColor":"PlayerDamageColor"):(critical?"CritDamageColor":"DamageColor"),&q.color))return -2;
 q.numeric=true;q.text=nullptr;q.number=r.amount>>8;return s.enqueue(s.context,&q)?-2:1;
}
}
