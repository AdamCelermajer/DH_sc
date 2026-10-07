#include "character_world_skill_execution_v6_fixture.inc"
#include "../character_world_skill_execution_v6.hpp"
#include "../character_world_aggro_event_v1.hpp"
#include "../character_world_self_dot_v7.hpp"
#include "../character_dot_calculate_services_v7.hpp"
namespace {
struct ExecutionActor {CombatActor* actor;character::CharacterConstructorCombatFieldsV1 fields{};std::uintptr_t controller{};const data::FreshInventoryOwnedV4* inventory{};bool controller_available=true;
 static int refresh(void*p,sk::WorldSkillExecutionActorV6*out){auto&x=*static_cast<ExecutionActor*>(p);auto&a=*x.actor;a.facts.state=a.machine.current;a.facts.combo_hits=x.fields.combo;
 *out={a.id,&a.view,&a.life,&a.facts,&a.machine,&x.fields,x.inventory,nullptr,nullptr,nullptr,x.controller_available?&x.controller:nullptr,&a.outgoing,&a.incoming};return 0;}
};
int debug_missing(void*,const char*,std::uintptr_t*out){*out=0;return 0;}int debug_close(void*,std::uintptr_t){return -1;}
struct ExecutionProviders {Providers base;
 static int aggro(void*p,unsigned,std::uintptr_t,std::uintptr_t){++static_cast<Providers*>(p)->aggro_events;return 0;}

 static int hit(void*p,character::HitActor32*a,const character::HitRequest32*q,std::uintptr_t*out){if(q->service==sk::hit_local_player_v6){*out=1;return 0;}return Providers::hit(&static_cast<ExecutionProviders*>(p)->base,a,q,out);}
};
}
int main(int argc,char**argv){try{check(argc==2);const std::string root=argv[1];std::string error;Inputs raw((root+"/port/level-world/reference/character-game-design/real-cache-inputs.bin").c_str());CharacterGameDesign design;check(design.initialize(raw.input,error),error);auto d=design.borrow();CombatActor player,enemy;
auto selected=std::find(d.characters()->names.begin(),d.characters()->names.end(),"KnightPlayerBase");check(selected!=d.characters()->names.end());data::reset_properties(*d.rules(),player.properties,&d.characters()->rows[selected-d.characters()->names.begin()]);check(data::recalc_properties_with_class(*d.classes(),*d.rules(),player.properties,error),error);
bool found=false;for(const auto&r:d.characters()->rows){data::reset_properties(*d.rules(),enemy.properties,&r);if(!data::recalc_properties_with_class(*d.classes(),*d.rules(),enemy.properties,error))continue;auto ai=data::ai_props(*d.ai(),enemy.properties.resolved[1]);if(ai&&ai->type==4&&enemy.properties.resolved[36]>1024){found=true;break;}}check(found);player.initialize(0x100000001,*d.rules());enemy.initialize(0x200000002,*d.rules());
sk::CharacterWorldRuntimeV1 world(*d.ai());check(!world.add({player.id,1,&player,CombatActor::world,&player.machine,&player.shared,&player.current}));check(!world.add({enemy.id,2,&enemy,CombatActor::world,&enemy.machine,&enemy.shared,&enemy.current}));
data::LootTablesV2 loots;data::SkillTables skills;auto load=[&](auto&owner,const std::string&base){auto a=file(base+"_pyarray.bin"),b=file(base+"_pyarraynames.bin"),c=file(base+"_pystructnames.bin");check(owner.load(bytes(a),bytes(b),bytes(c),error),error);};load(loots,root+"/.local-inputs/skill-execution-assets-v6/loot_table");load(skills,root+"/port/android-native/app/src/main/assets/data/skills");
auto same_properties=std::shared_ptr<data::PropertyState>(&player.properties,[](auto*){});data::LootRandom8V2 loot_rng{1,0};data::FreshInventoryOwnedV4 gear(player.id,loots.borrow(),loot_rng,-1,same_properties);character::NativeFsm24 fsm{&player.machine,player.id,0,0};sk::SkillManaServicesV5 mana{};sk::CharacterSkillNativeReadOnlyBindingsV6 readonly(gear,fsm,mana,world.native_world(player.id,nullptr));
character::DebugFileServices24 files{nullptr,debug_missing,debug_close};auto*debug=dh2_character_debug_create();check(debug);auto a=file(root+"/.local-inputs/trophy-owner-assets-v1/trophies_pyarray.bin"),b=file(root+"/.local-inputs/trophy-owner-assets-v1/trophies_pyarraynames.bin"),c=file(root+"/.local-inputs/trophy-owner-assets-v1/trophies_pystructnames.bin");auto catalog=dh2::trophies::TrophyCatalogV1::load(bytes(a),bytes(b),bytes(c),error);check(bool(catalog),error);auto trophy=dh2::trophies::TrophyManagerOwnerV1::create(catalog,debug,&files,{},error);check(bool(trophy),error);dh2::trophies::TrophyNativeBindingsV1 trophy_native(*trophy);
ExecutionProviders providers;providers.base.main=player.id;sk::WorldSkillCombatBackendsV6 backends{{&providers,ExecutionProviders::hit},{&providers.base,Providers::app,nullptr},&providers.base,ExecutionProviders::aggro,nullptr};character::DotCombatContext32 common_cf{};data::CombatRandom shared_rng{17,0};sk::CharacterWorldSkillExecutionV6 execution(world,*d.ai(),readonly,gear,fsm,skills.borrow(),common_cf,shared_rng,*debug,files,&trophy_native,backends);
ExecutionActor pa{&player},ea{&enemy};pa.controller=player.id+10;pa.inventory=&gear;ea.controller=enemy.id+10;check(!execution.add({player.id,&pa,ExecutionActor::refresh}),execution.error());check(!execution.add({enemy.id,&ea,ExecutionActor::refresh}),execution.error());

auto native_world=execution.combat().native_world();sk::SkillAttackActorV6* attack_actor{};sk::SkillApplyActorV6* application_actor{};
check(!native_world.actor(native_world.context,player.id,&attack_actor,&application_actor));
std::uint8_t low_health=0,tutorial=0;
character::HitPlayerTailBorrowV7 tail_actor{player.id,player.id,&player.view,&low_health,&tutorial};
character::HitPlayerTailServicesV7 tail_services{};
tail_services.is_player=[](void*,std::uintptr_t,bool*out){*out=true;return 0;}; // Actual selected-player fixture predicate.
character::DotPlayerReactionServicesV7 reaction{&player.machine,
 [](void*p,std::uintptr_t,bool*out){*out=static_cast<character::State*>(p)->current==3;return 0;}};
sk::DotPlayerServicesV7 player_services{&tail_actor,&tail_services,&reaction};
auto calculate_services=sk::dot_calculate_services_v7(*execution.combat().application_services().debug);
player.machine.current=3; // Explicit source Idle fixture, not fake injury acceptance.
check(!dh2_property_set(&player.view,36,player.view.resolved[38]),"Explicit initialized current HP input");
const auto before=player.view.resolved[36];const auto rng_before=shared_rng.seed;
sk::WorldSelfDotOutputV7 dot{};
const auto result=sk::character_world_self_dot_v7(&dot,execution,common_cf,player.id,256,-1,&calculate_services,&player_services);
check(result==-2,"Missing source Injure provider must remain explicit");
check(dot.calculate.status==1&&dot.attack.mask==0x20080000u&&dot.attack.amount==256);
check(dot.apply.hit.status==1,"Full same-player nonlethal source HitFor reaches Apply reaction");
check(dot.attack.outcomes==16&&dot.apply.phase==sk::skill_apply_injure_v6,"Original idle player DoT Injure prefix");
check(player.view.resolved[36]==before-256,"Actual native source HitFor property Add, no hand subtraction");
check(common_cf.attacker==player.id&&common_cf.defender==player.id&&common_cf.element==-1);
check(shared_rng.seed==rng_before&&shared_rng.calls==0,"Direct damage branch consumes no shared random");
check(player.life.combo_hits==pa.fields.combo&&pa.fields.combo==1);
check(player.outgoing.count==0&&player.incoming.count==0,"Actual source player/self Aggro guard preserves same tables");
check(!player.life.dead,"No fake lethal lifecycle");
dh2_character_debug_destroy(debug);
std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"same_world_self_dot_native_hp_prefix\":true,\"missing_actual_injure\":true,\"full_player_application\":false}"<<std::endl;return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<std::endl;return 1;}}
