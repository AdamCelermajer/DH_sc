#include "gameplay_hud.hpp"
#include "faery_gameplay_v1.hpp"
#include "hud_player_infos.hpp"
#include "character_skill_combat_v6.hpp"
#include "script_design_bindings.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <memory>
namespace model_renderer {
namespace {
using namespace dh2;
struct Snapshot {
 const PlayerGameplayBinding& p;ui::HudInfosActor32 actor{};std::vector<int> out=std::vector<int>(27,0);int last_operation{-1};
 Snapshot(const PlayerGameplayBinding& b):p(b){for(unsigned i=17;i<20;++i)out[i]=-1;out[23]=-1;
  actor={b.properties?b.properties->resolved:nullptr,224,std::uint8_t(b.active),0,0,b.character,0};}
 static int invoke(void* context,const ui::HudInfosRequest32* q,ui::HudInfosResponse16* r){
  auto& s=*static_cast<Snapshot*>(context);s.last_operation=int(q->operation);auto& b=s.p;using O=ui::HudInfosOperation;
  switch(q->operation){
   case O::player:r->identity=reinterpret_cast<std::uintptr_t>(&s.actor);return 1;
   case O::skill_slot:r->value=b.save?b.save->skill_in_slot(int(q->index)):-1;return 1;
   case O::skill_level:r->value=b.save?b.save->skill_level(q->index):0;return 1;
   case O::skill_usable:{std::uint32_t answer=0;if(!b.skills)return 0;
    if(b.skills->skill_ai(character::skills::skill_ai_usable_v3,q->index,&answer))return 0;r->value=int(answer);return 1;}
   case O::skill_info:if(!b.skills)return 0;return b.skills->info(q->index,unsigned(q->value),&r->fraction)==0;
   case O::spell_info:{r->fraction=0;if(!b.save||s.out[23]<0)return 1;std::string error;return dh2::android_ui::faery_spell_info_v1(b,&r->fraction,error)==0;}
   case O::spell_usable:{r->value=0;if(!b.save||s.out[23]<0)return 1;bool answer=false;std::string error;if(dh2::android_ui::faery_spell_usable_v1(b,&answer,error))return 0;r->value=answer;return 1;}
   case O::property_int:{std::int32_t v;if(!b.properties||dh2_property_resolve(b.properties,int(q->index),&v))return 0;r->value=v>>8;return 1;}
   case O::low_health_constant:{auto* design=b.design.design();return design&&design->lookup&&design->lookup(design->context,0,"CharacterDesign","LowHealthPercentage",&r->value)==0;}
   case O::potions:r->value=b.gear&&b.gear->inventory()?b.gear->inventory()->num_potions():0;return 1;
    case O::saved_dpad:if(!b.settings)return 0;r->value=b.settings->saved_option("DPad");return 1;
   case O::write_member:if(q->index>=17)return 0;s.out[q->index]=q->value;return 1;
   case O::divide_zero:return 0;
  }return 0;
 }
};
struct Debug {
 const PlayerGameplayBinding& b;std::unique_ptr<std::string> token;
 static int invoke(void* context,const character::skills::SkillAttackNativeRequestV6* q,std::uintptr_t* out){
  auto& d=*static_cast<Debug*>(context);using namespace character::skills;
  if(!d.b.debug||!d.b.debug_files)return -1;
  if(q->service==skill_attack_debug_load_v6)return dh2_character_debug_load(d.b.debug,d.b.debug_files)==1?0:-1;
  if(q->service==skill_attack_string_construct_v6&&q->name){d.token=std::make_unique<std::string>(q->name);*out=reinterpret_cast<std::uintptr_t>(d.token.get());return 0;}
  if(q->subject!=reinterpret_cast<std::uintptr_t>(d.token.get())||!d.token)return -1;
  if(q->service==skill_attack_debug_get_v6){std::uint32_t v;if(dh2_character_debug_get(&v,d.b.debug,d.token->c_str(),d.b.debug_files)!=1)return -1;*out=v;return 0;}
  if(q->service==skill_attack_string_destroy_v6){d.token.reset();return 0;}return -1;
 }
};
}
std::vector<int> gameplay_hud_snapshot(const PlayerGameplayBinding& b){
 Snapshot s(b);
 if(b.save){for(unsigned i=0;i<3;++i){auto row=b.save->skill_in_slot(int(i));if(row>=0){s.out[17+i]=b.save->skill_id(unsigned(row));s.out[20+i]=b.save->skill_level(unsigned(row));}}
  auto faery=b.save->current_faery(unsigned(b.difficulty));
  if(faery>=0&&faery<5&&b.difficulty>=0&&b.difficulty<3&&b.save->faeries_initialized()[unsigned(b.difficulty)]&&b.save->faeries()[unsigned(b.difficulty)][unsigned(faery)].state){s.out[23]=faery;s.out[24]=b.save->faery_level(unsigned(faery),unsigned(b.difficulty));}}
 ui::HudInfosServices16 services{&s,Snapshot::invoke};ui::HudInfosInput24 input{1,0,0,2,0};
 // Preserve source writes reached before a provider failure. Potion inventory
 // is independently available from this SAME Gear even if a prior skill query
 // prevents the authored algorithm reaching its potion write.
 const auto status=dh2_ui_hud_player_infos_v1(&input,&services);
 s.out[25]=status;s.out[26]=status?s.last_operation:-1;
 if(status&&b.active&&b.gear&&b.gear->inventory())s.out[14]=b.gear->inventory()->num_potions();
 return s.out;
}
std::vector<std::string> gameplay_hud_icon_names(const PlayerGameplayBinding& b){
 std::vector<std::string> names(5);names[3]="Faery";names[4]="HUDPotion";
 if(!b.save||!b.skill_tables)return names;
 for(unsigned slot=0;slot<3;++slot){auto row=b.save->skill_in_slot(int(slot));
  auto id=row<0?-1:b.save->skill_id(unsigned(row));
  if(id>=0&&std::size_t(id)<b.skill_tables.skills().size())names[slot]=b.skill_tables.skills()[std::size_t(id)].icon;
 }
 return names;
}
std::string gameplay_use_potion(const PlayerGameplayBinding& b){
 if(!b.active||!b.gear||!b.properties||!b.life||b.life->dead)return "Player cannot drink a potion";
 const auto* inventory=b.gear->inventory();if(!inventory||inventory->num_potions()<=0)return "No health potions";
 auto* p=b.properties->resolved;
 if(!(float(p[36])/float(p[38])<1.f)&&!(float(p[41])/float(p[43])<1.f))return "Health and mana are full";
 std::string error;if(!b.gear->remove_one_potion(error))return "Potion failed: "+error;
 if(dh2_property_add(b.properties,219,256))return "Potion consumed; counter update failed";
 // Original local-player trophy continuation reaches >99. Keep the reached
 // prefix and report the missing trophy producer instead of faking an unlock.
 if((p[219]>>8)>99)return "Potion consumed; required potion trophy producer unavailable";
 Debug debug{b,{}};character::skills::SkillAttackNativeServicesV6 services{&debug,Debug::invoke};
 if(character::skills::dh2_character_skill_regen_v6(b.properties,0,-1,&services)||character::skills::dh2_character_skill_regen_v6(b.properties,1,-1,&services))return "Potion consumed; required regeneration failed";
 return "Potion used: health and mana restored";
}
}
