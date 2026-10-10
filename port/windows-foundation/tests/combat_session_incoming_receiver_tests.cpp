#include "../combat_session.hpp"
#include "../game_save.hpp"
#include "../features/enemy_ai/runtime_enemy_controller_v1.hpp"
#include "../../game-data/animation_tables.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
void run(const char* root,const std::string& profile,bool minimal,std::uint32_t requestedStep){
    AssetCatalog assets(root);std::string error;OriginalPropertyDatabase db;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",db,error)&&bindings.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    check(build_original_combat_visual_plan(assets,bindings,profile,customization,"incoming-test",plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=19;config.playerId=1;config.playerProfileId=profile;
    config.tableRoot="original-cache/data/pydata";config.playerVisualConfig=plan.config;
    config.playerVisualConfig.motion_node_id="auto";config.playerVisualConfig.consume_root_motion=true;
    const bool rogue=profile=="RoguePlayerBase";
    config.mainItemId=rogue?"Dagger01":"Staff01";if(rogue)config.offItemId="Dagger01";
    const std::string family=rogue?"Rogue":"Mage";
    config.equippedItemIds={"StartingSuit"+family,"StartingBoots"+family,"StartingGloves"+family,config.mainItemId};
    if(rogue)config.equippedItemIds.push_back("Dagger01");
    CombatSessionProfile player;player.animationOnly=true;player.receiveDamage=true;player.reactionMinimalRandoms=minimal;
    player.propertyOptions={256,true};player.initialIdle={"Idle",0,{0}};
    player.reaction=CombatSessionChoice{"Injured",0,{0}};player.death=CombatSessionChoice{"Died",0,{0}};
    config.profiles.emplace(profile,player);
    CombatSessionProfile enemy;enemy.initialIdle={"Idle",0,{0}};enemy.damageMarkerNames={"attack_mainhand"};
    OriginalAttackSelection attack;attack.state="Attack";attack.group_path={0};enemy.sequenceAction=attack;
    enemy.retainedPhaseClock=true;enemy.customization=customization;enemy.propertyOptions={256,true};enemy.motionRoot="auto";
    config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="incoming-source-lizard";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};placed.transform=placed.definition.placement;
    population.actors().push_back(std::move(placed));CharacterVisual visual;CombatSession session;
    auto initialize=[&](){
        check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
        check(session.update(0,{},Vec3{0,0,0},0,error),error);
    };
    initialize();check(session.actor(1)->alive()&&session.world()->traits(1)->targetable&&session.actor(1)->attack_ids.empty(),
          "Incoming receiver missing real vitals/target role or gained outgoing attacks");
    check(session.world()->eligible_target(*session.actor(2),*session.actor(1)),"Enemy cannot target incoming receiver");
    const auto originalRng=session.world()->random_state();
    check(!session.request_actor_attack(1,2,0,error)&&session.actor(1)->attack_ids.empty()&&
          session.world()->random_state().seed==originalRng.seed&&session.world()->random_state().calls==originalRng.calls,
          "Incoming-only actor admitted/rerolled outgoing combat");
    enemy_ai::RuntimeEnemyControllerServicesV1 aiServices;
    aiServices.can_see=[](auto&,const auto&,const auto&,const auto&,const auto&,const auto&,bool& visible,std::string&){visible=true;return true;};
    aiServices.approach=[](auto&,auto&,auto&,const auto&,std::string&){return true;};
    aiServices.stop=[&](CombatSession& current,ActorState& source,std::string&){
        return &current==&session&&current.actor(source.id)==&source;
    };
    enemy_ai::RuntimeEnemyControllerV1 controller(std::move(aiServices));
    session.set_actor_decision_provider([&](auto& current,double dt,std::string& e){return controller.update(current,dt,e);});
    const auto health=session.actor(1)->health;bool markerDamage=false;
    for(unsigned i=0;i<360&&!markerDamage;++i){
        check(session.update(1.0/60,{},Vec3{0,0,0},0,error),error);
        for(const auto& hit:session.events())if(hit.attacker==2&&hit.target==1&&hit.applied&&hit.health_removed>0)markerDamage=true;
    }
    check(markerDamage&&session.actor(1)->health<health&&controller.report().acquired_targets>0&&controller.report().attack_requests>0,
          "Actual enemy controller/retained marker did not damage incoming class");
    // Exercise the otherwise-zero ordinary Lizard Injury rating through a
    // controlled original formula sheet, never fabricated outcome events.
    auto formula=session.world()->combat_properties(2)->sheets.resolved;formula[135]=formula[182]=100*256;
    OriginalMeleeDamageProvider probe;
    check(probe.bind_actor(1,*session.world()->combat_properties(1),error)&&probe.bind_actor(2,*session.world()->combat_properties(2),error),error);
    const auto category=session.world()->combat_properties(2)->facts.main_damage_class;
    std::uint32_t seed=0;OriginalMeleeResolution expected;dh2::data::CombatRandom expectedRng;
    for(std::uint32_t i=1;i<50000&&!seed;++i){dh2::data::CombatRandom rng{i,0};OriginalMeleeResolution result;
        check(probe.resolve_result(2,1,0x22aab5u,category,-1,0,rng,result,error,&formula),error);
        if((result.original.outcomes&0x10u)&&result.damage>0&&result.damage<session.actor(1)->max_health){
            auto selectedRng=rng;const auto step=minimal?0u:dh2_animation_random(&selectedRng.seed,&selectedRng.calls,3);
            if(step==requestedStep){seed=i;expected=result;expectedRng=rng;}
        }
    }
    check(seed!=0,"No controlled source Injury formula seed");config.diagnosticRngSeed=seed;initialize();
    std::uint32_t expectedStep=0;
    if(!minimal)expectedStep=dh2_animation_random(&expectedRng.seed,&expectedRng.calls,3);
    const auto* sameOwner=session.retained_actor_pose(1);std::vector<CombatSessionStepEntry> entries;
    session.set_step_entry_observer([&](const auto& event){if(event.actor==1)entries.push_back(event);});
    float motion=0;const auto recordMotion=[&](ActorState&,Vec3 delta,bool,std::string&){motion+=std::abs(delta.x)+std::abs(delta.y);return true;};
    session.set_motion_handler(recordMotion);
    CombatSessionSourceHit hit;hit.attacker=2;hit.target=1;hit.binding_lease=session.actor_binding_lease();
    hit.generation=1;hit.source_id="controlled-source-injury";hit.marker_name="attack_mainhand";
    hit.mask=0x22aab5u;hit.category=category;hit.attacker_formula_sheet=&formula;
    DamageEvent receipt;check(session.apply_source_result(hit,receipt,error)&&receipt.applied,error);
    check(receipt.source_outcomes&&(*receipt.source_outcomes&0x10u)&&session.actor(1)->action==CharacterAction::hurt&&
          entries.size()==1&&entries[0].sequence_id==267&&entries[0].step==expectedStep&&
          session.world()->random_state().seed==expectedRng.seed&&session.world()->random_state().calls==expectedRng.calls,
          "Source Type2 incoming injury lost shared RNG/selected step/pose");
    const auto* chosen=plan.phase("Injured",0,{expectedStep});
    const auto selectedPath=[&](){
        const auto* pose=session.retained_actor_pose(1);const auto* current=visual.configuration();
        check(pose&&current,"Current retained visual unavailable");const auto& name=pose->slots()[pose->current_slot()].clip_id;
        for(const auto& clip:current->clips)if(clip.first==name)return clip.second;
        throw std::runtime_error("Retained source clip missing from actual visual bank");
    };
    check(chosen&&selectedPath()==chosen->resolvedPath&&session.retained_actor_pose(1)==sameOwner,
          "Incoming class substituted injury clip or replaced retained owner");
    const auto after=session.world()->random_state();
    check(session.apply_source_result(hit,receipt,error)&&!receipt.applied&&session.world()->random_state().calls==after.calls,
          "Duplicate Injury replayed formula/selection");
    // A new original formula still runs, but the positive3000ms gate prevents
    // an extra animation selection draw. Compare exact source RNG after formula.
    ++hit.generation;auto nextRng=after;OriginalMeleeResolution next;
    check(probe.resolve_result(2,1,hit.mask,category,-1,0,nextRng,next,error,&formula),error);
    check(session.apply_source_result(hit,receipt,error)&&entries.size()==1&&session.world()->random_state().seed==nextRng.seed&&
          session.world()->random_state().calls==nextRng.calls,"Suppressed Injury consumed a Type2 selection draw");
    for(unsigned i=0;i<240;++i)check(session.update(1.0/60,{},Vec3{0,0,0},0,error),error);
    check(session.actor(1)->action==CharacterAction::idle&&session.world()->traits(1)->targetable&&session.retained_actor_pose(1)==sameOwner,
          "Incoming injury recovery lost role/owner");
    hit.source_id="source-incoming-lethal";++hit.generation;hit.mask=0x20080000u;hit.category=-1;hit.attacker_formula_sheet=nullptr;
    hit.direct_amount=static_cast<std::int32_t>(std::ceil(session.actor(1)->health*256));
    check(session.apply_source_result(hit,receipt,error)&&receipt.target_died&&session.owns_pose(1),error);
    const auto* death=plan.phase("Died",0,{0});
    check(death&&selectedPath()==death->resolvedPath&&entries.size()==2&&entries.back().sequence_id==259,
          "Incoming ordinary death used wrong source root");
    for(unsigned i=0;i<240;++i)check(session.update(1.0/60,{},Vec3{0,0,0},0,error),error);
    const auto corpse=*session.actor(1);const auto corpseRng=session.world()->random_state();const auto corpseMotion=motion;
    auto state=make_default_character("incoming-test",family,profile);session.actor(1)->persistent_character_id=state.id;
    state.stats.health=corpse.health;state.stats.max_health=corpse.max_health;state.stats.resource=corpse.resource;state.stats.max_resource=corpse.max_resource;
    GameSave saved;check(capture_game_save("incoming-world",1,state,*session.world(),saved,error),error);
    initialize();session.actor(1)->persistent_character_id=state.id;session.detach_for_restore();
    session.set_motion_handler(recordMotion);
    check(restore_game_save(saved,"incoming-world",*session.world(),state,error)&&session.rebind_after_restore(error),error);
    check(session.world()->traits(1)->targetable&&session.actor(1)->attack_ids.empty()&&!session.actor(1)->alive()&&session.owns_pose(1),
          "Incoming corpse restore changed role/attack capability");
    for(unsigned i=0;i<20;++i)check(session.update(1.0/60,{},Vec3{0,0,0},0,error),error);
    check(session.actor(1)->transform.position==corpse.transform.position&&session.world()->random_state().seed==corpseRng.seed&&
          session.world()->random_state().calls==corpseRng.calls&&motion==corpseMotion,"Saved corpse replayed pose motion/RNG");
    auto invalid=config;invalid.profiles.at(profile).death.reset();
    check(!session.initialize(assets,db,bindings,invalid,visual,population,{0,0,0},customization,error),
          "Incoming capability without original death pose accepted");
    std::cout<<"PASS incoming "<<profile<<" minimalRandoms="<<minimal<<" type2Step="<<expectedStep<<" enemyHits/actualPose/sharedRNG/terminalSave/noOutgoing\n";
}
}
int main(int argc,char** argv){try{
    check(argc==2,"Original shared asset root required");
    for(const auto* profile:{"RoguePlayerBase","MagePlayerBase"}){
        for(const auto step:{0u,1u,2u})run(argv[1],profile,false,step);
        run(argv[1],profile,true,0);
    }
    return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
