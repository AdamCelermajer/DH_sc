#include "../combat_session.hpp"
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
static void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
int main(int argc,char** argv){try{
    check(argc==2,"Supply original shared assets");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase db;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",db,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"target-cycle-player",plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";config.playerVisualConfig=plan.config;
    config.mainItemId="Longsword01";config.equippedItemIds={"Longsword01"};
    CombatSessionProfile knight;knight.action={"AttackStatic",0,{0,1}};knight.initialIdle={"Idle",0,{0}};
    knight.damageMarkerNames={"attack_mainhand"};knight.propertyOptions={256,true};knight.customization=customization;
    config.profiles.emplace("KnightPlayerBase",knight);
    CombatSessionProfile lizard;lizard.action={"Attack",0,{0,1}};lizard.initialIdle={"Idle",0,{0}};
    lizard.damageMarkerNames={"attack_mainhand"};lizard.propertyOptions={std::nullopt,true};lizard.customization=customization;
    config.profiles.emplace("Swamp_LizadMan_Type1",lizard);
    ActorPopulation population;
    for(const auto placement:{std::pair<ActorId,float>{2,-10000},std::pair<ActorId,float>{3,-100}}){
        PopulationActor actor;actor.profileId="Swamp_LizadMan_Type1";actor.definition.stableId=placement.first;
        actor.definition.sourceId="target-cycle-source-"+std::to_string(placement.first);
        actor.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,placement.second,0,1};
        actor.transform=actor.definition.placement;population.actors().push_back(std::move(actor));
    }
    CharacterVisual visual;CombatSession session;
    check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
    InputActions input;input.targetSelect=true;
    const auto rng=session.world()->random_state();
    const auto serial=session.update_serial();const auto health=session.actor(1)->health;
    const auto resource=session.actor(1)->resource;const auto position=session.actor(1)->transform.position;
    ActorId selected=999;
    check(session.select_next_player_target(selected,error)&&selected==3&&session.selectedactor()->id==3,
          "Direct command disagreed with nearest eligible target");
    check(session.update_serial()==serial&&session.events().empty()&&
          session.actor(1)->health==health&&session.actor(1)->resource==resource&&
          session.actor(1)->transform.position==position&&
          session.world()->random_state().seed==rng.seed&&session.world()->random_state().calls==rng.calls,
          "Direct target command advanced a frame, vitals, transform or RNG");
    unsigned commands=0;session.set_frame_begin_provider([&](CombatSession& same,double,std::string& e){
        ++commands;return same.select_next_player_target(selected,e);
    });
    check(session.update(0,{},Vec3{0,0,0},0,error)&&commands==1&&selected==2&&
          session.actor(1)->target_id==2&&session.update_serial()==serial+1,
          "Frame-begin target transport duplicated the command or frame");
    session.clear_frame_begin_provider();session.actor(1)->target_id=invalid_actor_id;
    check(session.update(0,input,{0,0,0},0,error),error);
    check(session.actor(1)->target_id==3&&session.selectedactor()->id==3,
          "PC target cycle picked far smaller ObjectId instead of nearest eligible actor");
    check(session.world()->random_state().seed==rng.seed&&session.world()->random_state().calls==rng.calls,
          "Selection consumed gameplay RNG");
    check(session.update(0,input,{0,0,0},0,error)&&session.actor(1)->target_id==2,"Explicit next target was not retained");
    check(session.update(0,input,{0,0,0},0,error)&&session.actor(1)->target_id==3,"Target cycle did not wrap to nearest");
    bool reentry_rejected=false;unsigned transitions=0;
    check(session.bind_reconstructible_actor_transition_handler(
        [&](const CombatSessionActorTransition&,std::string& e){
            ++transitions;ActorId untouched=999;std::string rejected;
            reentry_rejected=!session.select_next_player_target(untouched,rejected)&&untouched==999;
            check(reentry_rejected,"Target command reentered accepted transition delivery");
            e.clear();return true;
        },[](std::string& e){e.clear();return true;},error),error);
    input.targetSelect=false;input.attack=true;const auto farHealth=session.actor(2)->health;
    bool hit=false;
    for(unsigned i=0;i<180;++i){
        check(session.update(1.0/60,input,{0,0,0},0,error),error);
        for(const auto& event:session.events())if(event.attacker==1&&event.target==3&&event.health_removed>0)hit=true;
        if(hit)break;
    }
    check(hit&&session.actor(3)->health<session.actor(3)->max_health&&session.actor(2)->health==farHealth,
          "Fresh selected-near held attack did not hit its actual source enemy without reload");
    check(transitions>0&&reentry_rejected,"Real attack did not probe transition target-command rejection");
    // Equal-distance tie is deterministic and selection of an invalid old
    // target begins at nearest. No camera visibility or new range is invented.
    session.actor(2)->transform.position={0,-100,0};session.actor(1)->target_id=0;
    input.attack=false;input.targetSelect=true;
    check(session.update(0,input,{0,0,0},0,error)&&session.actor(1)->target_id==2,
          "Equal-distance selection did not use stable ID tie-break");
    session.set_actor_combat_permission_provider([](ActorId id){return id!=2;});
    check(session.update(0,input,{0,0,0},0,error)&&session.actor(1)->target_id!=2,
          "Selection retained a forbidden actor");
    session.set_actor_combat_permission_provider([](ActorId){return false;});
    check(session.select_next_player_target(selected,error)&&selected==invalid_actor_id&&!session.selectedactor(),
          "Empty permitted target set did not publish a clear command");
    session.detach_for_restore();selected=999;
    check(!session.select_next_player_target(selected,error)&&selected==999,"Detached target command mutated intent");
    CombatSession unbound;check(!unbound.select_next_player_target(selected,error)&&selected==999,
          "Unbound target command mutated output");
    std::cout<<"PASS direct/input nearest-first target command, one frame-begin transport, stable tie/RNG, reentry/detach rejection, actual source held-attack damage without R/F9\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
