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
int main(int argc,char** argv){try{
 check(argc==2);std::string root=argv[1],error;Inputs raw((root+"/port/level-world/reference/character-game-design/real-cache-inputs.bin").c_str());CharacterGameDesign design;check(design.initialize(raw.input,error),error);auto d=design.borrow();CombatActor player,enemy;
 auto row=std::find(d.characters()->names.begin(),d.characters()->names.end(),"KnightPlayerBase");check(row!=d.characters()->names.end());data::reset_properties(*d.rules(),player.properties,&d.characters()->rows[row-d.characters()->names.begin()]);check(data::recalc_properties_with_class(*d.classes(),*d.rules(),player.properties,error),error);
 bool found=false;for(const auto& r:d.characters()->rows){data::reset_properties(*d.rules(),enemy.properties,&r);if(!data::recalc_properties_with_class(*d.classes(),*d.rules(),enemy.properties,error))continue;auto ai=data::ai_props(*d.ai(),enemy.properties.resolved[1]);if(ai&&ai->type==4&&enemy.properties.resolved[36]>1024){found=true;break;}}check(found);
 player.initialize(0x100000001,*d.rules());enemy.initialize(0x200000002,*d.rules());sk::CharacterWorldRuntimeV1 world(*d.ai());
 check(!world.add({player.id,1,&player,CombatActor::world,&player.machine,&player.shared,&player.current}));check(!world.add({enemy.id,2,&enemy,CombatActor::world,&enemy.machine,&enemy.shared,&enemy.current}));
 Providers providers;providers.main=player.id;sk::SkillAttackNativeServicesV6 debug{&providers,Providers::debug};sk::WorldSkillCombatBackendsV6 backends{{&providers,Providers::hit},{&providers,Providers::app,&debug},&providers,Providers::aggro,nullptr};sk::CharacterWorldSkillCombatV6 combat(world,*d.ai(),debug,backends);
 check(!combat.add({player.id,&player,CombatActor::combat}));check(!combat.add({enemy.id,&enemy,CombatActor::combat}));auto native=combat.native_world();sk::SkillAttackActorV6* a{};sk::SkillApplyActorV6* b{};check(!native.actor(native.context,enemy.id,&a,&b)&&a->properties==&enemy.view&&b->properties==a->properties&&b->hit_services==&combat.hit_services());
 const auto hp=enemy.view.resolved[36];data::CombatResult result;result.amount=256;result.mask=0x20000000;sk::SkillApplyOutputV6 output{};check(combat.apply(&output,&result,player.id,enemy.id)==-2);check(enemy.view.resolved[36]==hp-256,"Genuine same-sheet HP prefix before missing trophy manager");check(output.hit.phase==sk::hit_trophy_manager_v6+1&&output.hit.status==-2);check(!enemy.life.dead&&player.combo==1&&providers.tokens.empty());check(enemy.outgoing.count==1&&player.incoming.count==1&&providers.aggro_events==1,"Genuine reciprocal source threat tables and ordered OnAggro delivery");
 result.amount=256;check(combat.apply(&output,&result,player.id,enemy.id)==-2);check(enemy.view.resolved[36]==hp-512&&providers.aggro_events==1,"Existing source threat update does not invent repeated OnAggro");
 auto saved=enemy.application.properties;enemy.application.properties=&player.view;check(native.actor(native.context,enemy.id,&a,&b)!=0,"Reject foreign property owner");enemy.application.properties=saved;
 enemy.life.dead=1;auto hit=combat.hit_services();character::HitRequest32 dead{character::hit_is_dead,0,enemy.id,0,nullptr};std::uintptr_t value{};check(!hit.invoke(hit.context,&enemy.hit,&dead,&value)&&value==1,"Same authoritative life queried");
 std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"actual_design_properties\":true,\"same_world_property_life_handle_threat\":true,\"genuine_hp_prefix\":true,\"missing_trophy_explicit_after_hp\":true,\"full_skill_application\":false}\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
