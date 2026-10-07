#define main existing_v3_player_fixture_main
#include "character_player_skills_v3.cpp"
#undef main
#include "../character_world_skill_combat_v6.hpp"
namespace {
namespace sk=dh2::character::skills;
struct CombatActor {
 std::uintptr_t id{},current{},node{};std::uint8_t enabled=0,invulnerable=0,push=0;std::uint16_t combo=0;std::int32_t network=-1;
 data::PropertyState properties;data::PropertyView view{};data::CombatActorState life{};character::State machine{};
 target_search::Object48 search{};sk::SkillTargetCharacterV6 target{};scene::Scene scene{};target_providers::Handle16 shared{};float cache[3]{},heading{},controller_heading{};
 data::CombatantView facts{};sk::SkillAttackActorV6 attack{};sk::SkillApplyActorV6 application{};character::HitActor32 hit{};
 data::AggroEntry outgoing_storage[8]{},incoming_storage[8]{};data::AggroTable outgoing{outgoing_storage,0,8},incoming{incoming_storage,0,8};
 static int world(void* p,sk::WorldTargetActorBorrowV1* b){auto& a=*static_cast<CombatActor*>(p);*b={a.id,&a.search,&a.target,&a.scene,&a.life,&a.node,&a.enabled,a.cache,a.search.position,&a.heading,&a.controller_heading,nullptr,nullptr};return 0;}
 static int combat(void* p,sk::WorldSkillCombatBorrowV6* b){auto& a=*static_cast<CombatActor*>(p);*b={&a.attack,&a.application,&a.life,nullptr,&a.outgoing,&a.incoming};return 0;}
 void initialize(std::uintptr_t identity,const data::PropertyRules& rules){id=identity;view=data::property_view(rules,properties);search.identity=id;search.visible=1;machine.flags=0x2000;target={id,view.resolved,"fixture",0,0,0,1,1};facts.properties=view.resolved;attack={id,&view,&facts};hit={id,&view,id+10,0,0};application={};application.identity=id;application.properties=&view;application.hit=&hit;application.combo=&combo;application.invulnerable=&invulnerable;application.push_death=&push;application.network_id=&network;}
};
struct Providers {
 std::uintptr_t main{};std::map<std::uintptr_t,std::string> tokens;unsigned serial=1,aggro_events=0;bool reject_trophy=true;
 static int debug(void* p,const sk::SkillAttackNativeRequestV6* q,std::uintptr_t* out){auto& s=*static_cast<Providers*>(p);*out=0;switch(q->service){case sk::skill_attack_debug_load_v6:return 0;case sk::skill_attack_string_construct_v6:if(!q->name)return -1;*out=++s.serial;s.tokens[*out]=q->name;return 0;case sk::skill_attack_debug_get_v6:return s.tokens.count(q->subject)?0:-1;case sk::skill_attack_string_destroy_v6:return s.tokens.erase(q->subject)==1?0:-1;default:return -1;}}
 static int app(void*,const sk::SkillApplyRequestV6* q,sk::SkillApplyResponseV6* out,data::CombatResult*){switch(q->service){case sk::skill_apply_online_v6:case sk::skill_apply_saved_option_v6:out->word=0;return 0;case sk::skill_apply_party_count_v6:out->word=1;return 0;default:return -1;}}
 static int hit(void* p,character::HitActor32*,const character::HitRequest32* q,std::uintptr_t* out){auto& s=*static_cast<Providers*>(p);switch(q->service){case character::hit_main_player:*out=s.main;return 0;case character::hit_online:case character::hit_application_switch:case character::hit_is_remotely_updated:*out=0;return 0;default:return -1;}}
 static int aggro(void* p,unsigned event,std::uintptr_t owner,std::uintptr_t target){auto& s=*static_cast<Providers*>(p);check(event==data::aggro_notify_target&&owner!=target);++s.aggro_events;return 0;}
};
}
