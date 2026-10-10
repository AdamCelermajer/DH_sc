#include "../../combat_session.hpp"
#include "../../content_paths.hpp"
#include "../../actor_profiles.hpp"
#include "../equipment/runtime_player_locomotion_v1.hpp"
#include "runtime_player_profile_attack_bank_v1.hpp"
#include "../skills_animation/skill_animation_program.hpp"
#include "../../../game-data/animation_tables.hpp"
#include "../../../game-data/items.hpp"
#include "../../../script-runtime/script_constants.hpp"
#include <cmath>
#include <iostream>
#include <map>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool ok,const std::string& what){if(!ok)throw std::runtime_error(what);}
bool histogramMode=false;
struct OutcomeCase{const char* label;std::uint32_t required,forbidden;bool damage;bool reaction;bool suppress_status=false,boost_injury=false,lethal=false,suppress_injury_only=false,source_gap=false;};
const OutcomeCase cases[]={
 {"ordinary hit",0,0x1f7u,true,false,true},
 {"miss",1,0,false,false},
 {"dodge",2,0,false,false},
 {"block",4,0x1f0u,true,false,true},
 {"Injure",0x10,0x7u,true,true,true,true},
 {"lethal",0,0,false,true,false,false,true},
 {"push dispatch gap",0x80,0x17u,true,false,false,false,false,true,true}
};
std::vector<std::uint8_t> read(const AssetCatalog& assets,const std::string& path){return read_content(assets,path);}
template<class T> dh2::data::Bytes bytes(const T& v){return {v.data(),v.size()};}
void prepare_player_attack(const AssetCatalog& assets,const std::string& profile,
 const OriginalCombatVisualPlan& visual,combat::RuntimePlayerProfileAttackBankPlanV1& bank,
 CharacterVisualConfig& actorVisual,std::vector<std::pair<std::string,std::string>>& sourceClips,
 bool shield,std::string& error){
 const auto readPy=[&](const char* name){return read(assets,std::string("original-cache/data/pydata/")+name);};
 const auto clipNames=read(assets,"data/animations_dictionary_pyarraynames.bin");
 const auto clipValues=readPy("animations_dictionary_pyarray.bin");dh2::data::Dictionary clips;
 check(dh2::data::load_dictionary(bytes(clipNames),bytes(clipValues),clips,error),error);
 const auto records=readPy("animations_pyarray.bin"),names=readPy("animations_pyarraynames.bin"),fields=readPy("animations_pystructnames.bin");
 dh2::data::AnimationTables animations;check(dh2::data::load_animation_tables(bytes(records),bytes(names),bytes(fields),clips,animations,error),error);
 const auto itemRecords=readPy("loot_table_pyarray.bin"),itemNames=readPy("loot_table_pyarraynames.bin"),itemFields=readPy("loot_table_pystructnames.bin");
 dh2::data::ItemTable items;check(dh2::data::load_items(bytes(itemRecords),bytes(itemNames),bytes(itemFields),items,error),error);
 const auto constantsBytes=read(assets,"data/animations_pycst.bin");auto* constants=dh2_script_constants_create();
 check(constants,"Source animation constants unavailable");dh2_script_constants_reload reload{};
 check(dh2_script_constants_load(constants,constantsBytes.data(),static_cast<std::uint32_t>(constantsBytes.size()),&reload)==0,"Source constants failed");
 equipment_menu::RuntimePlayerLocomotionConstantsV1 stance;
 const bool stanceOk=equipment_menu::load_runtime_player_locomotion_constants_v1(
  [&](const char* group,const char* key,std::int32_t& value,std::string& e){if(dh2_script_constants_get(constants,group,key,&value)){e="Missing source stance constant";return false;}return true;},stance,error);
 dh2_script_constants_destroy(constants);check(stanceOk,error);
 ActorProfileLibrary profiles;check(profiles.load(assets,"actor-profiles-v2.xml",error),error);
 const auto* sourceProfile=profiles.find(profile);check(sourceProfile,"Original player profile missing");
 const auto mainName=profile=="RoguePlayerBase"?"Dagger01":"Longsword01";const auto main=dh2::data::item_id(items,mainName);
 const auto off=shield?dh2::data::item_id(items,"Shield01"):-1;
 check(main>=0&&(!shield||off>=0),"Original player weapon/shield item missing");equipment_menu::RuntimePlayerLocomotionV1 equipped;
 check(equipment_menu::resolve_runtime_player_locomotion_v1(std::stoi(sourceProfile->animation_table),items,
  main,off,0,stance,animations,clips,equipped,error),error);
 check(combat::plan_runtime_player_profile_attack_bank_v1(assets,*sourceProfile,equipped,
  stance.stanced_list_mask,animations,clips,visual,"b013-player",bank,error),error);
 skills_animation::SkillAnimationPrograms skillBank;
 check(skills_animation::build_skill_animation_programs(assets,animations,clips,visual.config,
  {347},"b013-player-skills",skillBank,error),error);
 actorVisual=skillBank.plan.config;actorVisual.motion_node_id="auto";actorVisual.consume_root_motion=true;
 sourceClips=skillBank.plan.config.clips;
}
void run(const char* root,const std::string& profile,const OutcomeCase& wanted){
 AssetCatalog assets(root);OriginalPropertyDatabase db;OriginalMeleeBindings melee;std::string error;
 check(load_original_property_tables(assets,"original-cache/data/pydata",db,error),error);
 check(melee.load(assets,"original-melee-bindings.xml",error),error);
 ActorCustomization custom;custom.allow_missing_animation_targets=true;OriginalCombatVisualPlan plan;
 check(build_original_combat_visual_plan(assets,melee,profile,custom,"b013-player",plan,error),error);
 const bool rogue=profile=="RoguePlayerBase";const bool shield=wanted.required==4&&profile=="KnightPlayerBase";
 combat::RuntimePlayerProfileAttackBankPlanV1 attackBank;CharacterVisualConfig actorVisual;
 std::vector<std::pair<std::string,std::string>> sourceClips;
 prepare_player_attack(assets,profile,plan,attackBank,actorVisual,sourceClips,shield,error);
 const std::string family=rogue?"Rogue":"";
 CombatSessionConfig config;config.playerId=1;config.playerProfileId=profile;config.tableRoot="original-cache/data/pydata";
 config.playerVisualConfig=actorVisual;config.diagnosticRngSeed=1;config.mainItemId=rogue?"Dagger01":"Longsword01";
 config.offItemId=shield?"Shield01":"";
 config.equippedItemIds={"StartingSuit"+family,"StartingBoots"+family,"StartingGloves"+family,config.mainItemId};
 if(shield)config.equippedItemIds.push_back("Shield01");
 CombatSessionProfile player;player.sequenceAction=attackBank.static_selection;player.sourceAttackBank=attackBank.bank;
 player.sourceAttackPolicies=attackBank.sequence_policies;player.sourceAttackStateSelection=false;
 player.sourceAnimationClips=sourceClips;player.initialIdle={"Idle",0,{0}};
 player.damageMarkerNames={"attack_mainhand"};player.customization=custom;player.propertyOptions={256,true};player.retainedPhaseClock=true;
 player.reaction=CombatSessionChoice{"Injured",0,{0}};player.death=CombatSessionChoice{"Died",0,{0}};player.motionRoot="auto";
 config.profiles.emplace(profile,player);
 CombatSessionProfile enemy;enemy.action={"Attack",0,{0,1}};enemy.initialIdle={"Idle",0,{0}};
 enemy.damageMarkerNames={"attack_mainhand"};enemy.customization=custom;enemy.propertyOptions={20*256,true};
 enemy.reaction=CombatSessionChoice{"Injured",0,{0}};enemy.death=CombatSessionChoice{"Died",0,{0}};enemy.motionRoot="auto";
 config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
 ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
 placed.definition.stableId=2;placed.definition.sourceId="b013-lizard";
 placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};placed.transform=placed.definition.placement;
 population.actors().push_back(std::move(placed));CharacterVisual visual;CombatSession session;
 auto initialize=[&](){check(session.initialize(assets,db,melee,config,visual,population,{0,0,0},custom,error),"Session init: "+error);
  check(session.update(0,{},Vec3{0,0,0},0,error),error);};

 config.diagnosticRngSeed=1;initialize();auto* seedSource=session.world()->combat_properties(2);
 auto* seedTarget=session.world()->combat_properties(1);check(seedSource&&seedTarget,"Source combatants missing");
 auto seedFormula=seedSource->sheets.resolved;if(wanted.suppress_status){for(const auto id:{135,137,139,142,145,182,183,184,186,188})seedFormula[id]=0;}
 if(wanted.suppress_injury_only)seedFormula[135]=seedFormula[182]=0;
 if(wanted.boost_injury)seedFormula[135]=seedFormula[182]=100*256;
 OriginalMeleeDamageProvider seedProvider;
 check(seedProvider.bind_actor(2,*seedSource,error)&&seedProvider.bind_actor(1,*seedTarget,error),error);
 const auto seedCategory=seedSource->facts.main_damage_class;
 if(histogramMode){ // B013 probe: outcome distribution of the seeded lizard formula against the player.
  std::map<std::uint32_t,unsigned> counts;unsigned positive=0,samples=0;
  const auto rawFormula=seedSource->sheets.resolved;
  for(std::uint32_t candidate=1;candidate<=5000;++candidate){
   auto rng=dh2::data::CombatRandom{candidate,0};OriginalMeleeResolution r;
   check(seedProvider.resolve_result(2,1,0x22aab5u,seedCategory,-1,0,rng,r,error,&rawFormula),error);
   ++counts[r.original.outcomes&0x1ffu];++samples;if(r.damage>0)++positive;
  }
  std::cout<<profile<<" samples="<<samples<<" positive-damage="<<positive<<"\n";
  std::cout<<"  attacker 135="<<rawFormula[135]<<" 182="<<rawFormula[182]<<" 19="<<rawFormula[19]<<" 134="<<rawFormula[134]
   <<" | defender 135="<<seedTarget->sheets.resolved[135]<<" 19="<<seedTarget->sheets.resolved[19]<<" 134="<<seedTarget->sheets.resolved[134]<<"\n";
  for(const auto& entry:counts)std::cout<<"  outcomes=0x"<<std::hex<<entry.first<<std::dec<<" count="<<entry.second<<"\n";
  return;
 }
 std::uint32_t seed=wanted.lethal?1u:0u;OriginalMeleeResolution expected{};dh2::data::CombatRandom expectedRng{1,0};
 for(std::uint32_t candidate=1;candidate<50000&&!seed;++candidate){
  auto rng=dh2::data::CombatRandom{candidate,0};OriginalMeleeResolution result;
  check(seedProvider.resolve_result(2,1,0x22aab5u,seedCategory,-1,0,rng,result,error,&seedFormula),error);
  const bool bits=(result.original.outcomes&wanted.required)==wanted.required&&
      !(result.original.outcomes&wanted.forbidden);
  if(bits&&((result.damage>0)==wanted.damage)){seed=candidate;expected=result;expectedRng=rng;}
 }
 if(!seed){std::cout<<profile<<" "<<wanted.label<<" unavailable: no source result in 1..49999 for this authored actor/loadout\n";return;}
 config.diagnosticRngSeed=seed;initialize();auto* source=session.world()->combat_properties(2);
 auto formula=source->sheets.resolved;if(wanted.suppress_status){for(const auto id:{135,137,139,142,145,182,183,184,186,188})formula[id]=0;}
 if(wanted.suppress_injury_only)formula[135]=formula[182]=0;
 if(wanted.boost_injury)formula[135]=formula[182]=100*256;
 check(session.world()->random_state().seed==seed&&session.world()->random_state().calls==0,
       "Fixture startup consumed unexpected combat RNG before authored result");
 check(session.request_actor_attack(1,2,0,error),"Attack admission: "+error);
 for(unsigned frame=0;frame<3;++frame)check(session.update(1.0/60,{},Vec3{0,0,0},0,error),error);
 const auto* owner=session.retained_actor_pose(1);if(!owner)owner=session.retained_player_pose();
 if(!owner)std::cerr<<"attack state="<<int(session.actor(1)->action)<<" owns="<<session.owns_pose(1)<<" target="<<session.actor(1)->target_id<<"\n";
 check(owner,"Knight/Rogue attack pose owner missing");
 const auto slot=owner->current_slot();const auto clip=owner->slots()[slot].clip_id;
 const auto poseGeneration=owner->slots()[slot].generation;const auto poseTime=owner->slots()[slot].timeline.current_ms;
 check(session.actor(1)->action==CharacterAction::attacking&&session.actor(1)->target_id==2,
       "Attack admission did not produce active target/attack state");
 const auto category=source->facts.main_damage_class;CombatSessionSourceHit hit;
 hit.attacker=2;hit.target=1;hit.binding_lease=session.actor_binding_lease();hit.generation=1;
 hit.source_id=std::string("b013-")+wanted.label;hit.marker_name="attack_mainhand";
 hit.mask=wanted.lethal?0x20080000u:0x22aab5u;
 hit.category=wanted.lethal?-1:category;hit.element=-1;hit.attacker_formula_sheet=wanted.lethal?nullptr:&formula;
 if(wanted.lethal)hit.direct_amount=static_cast<std::int32_t>(std::ceil(session.actor(1)->health*256.0f));
 DamageEvent receipt;check(session.apply_source_result(hit,receipt,error),error);
 check(receipt.applied,"Source hit did not apply");
 if(!wanted.lethal)check(receipt.source_outcomes&&receipt.source_mask&&
       receipt.source_outcomes==expected.original.outcomes&&receipt.source_mask==expected.original.mask&&
       receipt.requested_damage==expected.damage,"Session changed original outcome/damage/mask");
 else check(receipt.target_died&&session.actor(1)->action==CharacterAction::dead,
       "Lethal source result did not interrupt active attack through death state");
 check(session.world()->random_state().seed==expectedRng.seed&&session.world()->random_state().calls==expectedRng.calls,
       "Applying result changed source RNG call stream");
 if(wanted.lethal){
  const auto* after=session.retained_actor_pose(1);if(!after)after=session.retained_player_pose();
  check(after&&after->slots()[after->current_slot()].clip_id!=clip,"Lethal result retained attack pose");
 }else if(wanted.reaction){
  const auto* after=session.retained_actor_pose(1);if(!after)after=session.retained_player_pose();
  const auto injuryClip=after?after->slots()[after->current_slot()].clip_id:std::string{};
  check(receipt.source_outcomes&&(*receipt.source_outcomes&0x10u)&&after&&injuryClip!=clip&&
        session.actor(1)->action==CharacterAction::hurt,
        "Explicit original Injure outcome did not interrupt active player attack: action="+
        std::to_string(int(session.actor(1)->action))+" old="+clip+" new="+injuryClip);
 }else{
  const auto* after=session.retained_actor_pose(1);if(!after)after=session.retained_player_pose();
  check(after==owner&&after->current_slot()==slot&&
        after->slots()[slot].clip_id==clip&&after->slots()[slot].generation==poseGeneration&&
        after->slots()[slot].timeline.current_ms==poseTime&&session.actor(1)->action==CharacterAction::attacking&&
        session.actor(1)->target_id==2,
        std::string(wanted.label)+" changed active source attack/target/pose clock");
  if(wanted.source_gap)check(receipt.source_outcomes&&(*receipt.source_outcomes&0x80u),
        "Push gap probe did not retain the source Push outcome bit");
  check(session.update(1.0/60,{},Vec3{0,0,0},0,error),error);
  const auto* continued=session.retained_actor_pose(1);if(!continued)continued=session.retained_player_pose();
  check(continued==owner&&continued->slots()[slot].generation==poseGeneration&&
        continued->slots()[slot].timeline.current_ms>poseTime&&session.actor(1)->action==CharacterAction::attacking&&
        session.actor(1)->target_id==2,"Attack clock did not continue after non-Injure result");
 }
 const auto afterRng=session.world()->random_state();DamageEvent duplicate;
 check(session.apply_source_result(hit,duplicate,error)&&!duplicate.applied,error);
 check(session.world()->random_state().seed==afterRng.seed&&session.world()->random_state().calls==afterRng.calls,
       "Duplicate result replayed original RNG");
 std::cout<<profile<<" "<<wanted.label<<(wanted.source_gap?" GAP-UNIMPLEMENTED":"")<<" outcomes=0x"<<std::hex<<(receipt.source_outcomes?*receipt.source_outcomes:0)<<std::dec
  <<" seed="<<seed<<" hit-generation="<<hit.generation<<" pose-generation="<<poseGeneration
  <<" time-ms="<<poseTime<<" damage="<<receipt.requested_damage<<" active-attack="<<!wanted.reaction<<"\n";
}
}
int main(int argc,char** argv){try{
 check(argc==2||(argc==3&&std::string(argv[2])=="histogram"),"Supply unified original assets root");
 if(argc==3){histogramMode=true;run(argv[1],"KnightPlayerBase",cases[0]);return 0;}
 for(const auto* profile:{"KnightPlayerBase","RoguePlayerBase"})for(const auto& item:cases)run(argv[1],profile,item);
 std::cout<<"PASS B013 actual CombatSession: no-proc ordinary hit/miss preserve active Knight/Rogue attack; Injury/lethal interrupt; Push gap is exposed; duplicate delivery does not reroll. Dodge/block seeds are reported above.\n";
 return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
