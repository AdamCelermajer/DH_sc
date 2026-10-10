#include "../combat_session.hpp"
#include "../actor_movement.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool condition,const std::string& message){if(!condition)throw std::runtime_error(message);}
}
int main(int argc,char** argv){try{
    check(argc==2,"Supply original shared asset root");
    AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase database;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);

    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan playerPlan,npcPlan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"death-motion-player",playerPlan,error),error);
    check(build_original_combat_visual_plan(assets,bindings,"Swamp_LizadMan_Type1",customization,"death-motion-npc",npcPlan,error),error);
    const auto* death=npcPlan.phase("Died",0,{0});
    check(death&&!death->clipName.empty()&&death->speed>0,"Actual NPC Died phase is unavailable");
    check(death->moveGO!=0,"Selected authored NPC Died phase does not enable source MoveGO");

    CombatSessionConfig config;config.diagnosticRngSeed=20261009;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=playerPlan.config;config.playerVisualConfig.motion_node_id="auto";
    config.playerVisualConfig.consume_root_motion=true;
    CombatSessionProfile player;player.action={"AttackStatic",0,{0,1}};
    player.initialIdle={"Idle",0,{0}};player.damageMarkerNames={"attack_mainhand"};
    player.customization=customization;player.propertyOptions={256,true};
    config.profiles.emplace(config.playerProfileId,player);
    CombatSessionProfile enemy;enemy.action={"Attack",0,{0,1}};enemy.initialIdle={"Idle",0,{0}};
    enemy.death=CombatSessionChoice{"Died",0,{0}};enemy.damageMarkerNames={"attack_mainhand"};
    enemy.customization=customization;enemy.motionRoot="auto";enemy.propertyOptions={std::nullopt,true};
    config.profiles.emplace("Swamp_LizadMan_Type1",enemy);

    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="death-motion-source-fixture";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,17,1};
    placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    CharacterVisual playerVisual;CombatSession session;
    check(session.initialize(assets,database,bindings,config,playerVisual,population,
          {0,0,0},customization,error),error);
    const auto actorId=ActorId(2);auto* actor=session.actor(actorId);
    check(actor&&actor->alive(),"Actual NPC actor did not bind to the same Session");
    const auto initial=actor->transform.position;
    unsigned motionCalls=0,enabledCalls=0,nonzeroZCalls=0,zeroDeltaCalls=0;
    double appliedX=0,appliedY=0;
    session.set_motion_handler([&](ActorState& current,Vec3 delta,bool enabled,std::string&){
        check(&current==session.actor(actorId),"Death root motion reached a foreign ActorState");
        ++motionCalls;
        if(std::abs(delta.x)+std::abs(delta.y)+std::abs(delta.z)<1e-6f)++zeroDeltaCalls;
        if(std::abs(delta.z)>1e-6f)++nonzeroZCalls;
        if(enabled){++enabledCalls;current.transform.position[0]+=delta.x;current.transform.position[1]+=delta.y;
            appliedX+=delta.x;appliedY+=delta.y;}
        return true;
    });

    CharacterVisual expectedVisual;auto expectedConfig=npcPlan.config;
    expectedConfig.motion_node_id="auto";expectedConfig.consume_root_motion=true;
    check(expectedVisual.load(assets,expectedConfig,error),error);
    check(expectedVisual.restart(death->clipName,false,error),error);
    expectedVisual.take_root_motion();
    std::int32_t clipStart=0,clipEnd=0;
    check(expectedVisual.animation_range(death->clipName,clipStart,clipEnd,error),error);
    const double sourceDuration=(clipEnd-clipStart)/1000.0;
    const double wallDuration=sourceDuration/death->speed;
    const double dt=1.0/60.0;
    double expectedX=0,expectedY=0,expectedZ=0,elapsed=0;
    while(elapsed<wallDuration-1e-10){
        const double wall=std::min(dt,wallDuration-elapsed);
        check(expectedVisual.update(wall*death->speed,error),error);
        const auto delta=expectedVisual.take_root_motion();
        expectedX+=delta.x;expectedY+=delta.y;expectedZ+=delta.z;elapsed+=wall;
    }

    check(apply_actor_damage(*actor,actor->health)>0&&!actor->alive(),"Actual NPC lethal transition failed");
    InputActions input;
    const unsigned frames=static_cast<unsigned>(std::ceil(wallDuration/dt))+12;
    for(unsigned frame=0;frame<frames;++frame)
        check(session.update(dt,input,{0,0,0},0,error),error);
    check(session.owns_population_pose(actorId),"Completed Died pose was not retained as terminal");
    check(std::string(population.actors().front().visual.animation_name())==expectedVisual.animation_name(),
          std::string("Completed actual Died clip changed to another visual: actual=")+
          population.actors().front().visual.animation_name()+" expected="+expectedVisual.animation_name());
    check(enabledCalls>0&&motionCalls>=enabledCalls,"Authored MoveGO root deltas were not delivered");
    check(std::abs(appliedX-expectedX)<0.02&&std::abs(appliedY-expectedY)<0.02,
          "Actual Died clip XY displacement was not applied exactly once");
    check(std::abs(actor->transform.position[0]-(initial[0]+expectedX))<0.02&&
          std::abs(actor->transform.position[1]-(initial[1]+expectedY))<0.02,
          "Death actor position does not match once-applied authored XY motion");
    check(std::abs(actor->transform.position[2]-initial[2])<1e-6,
          "Source death root motion changed actor Z");

    const auto terminal=actor->transform.position;
    const unsigned callsAtEnd=motionCalls;
    const unsigned zeroCallsAtEnd=zeroDeltaCalls;
    for(unsigned frame=0;frame<4;++frame)check(session.update(0,input,{0,0,0},0,error),error);
    check(actor->transform.position==terminal,"Unchanged terminal sample duplicated root displacement");
    check(motionCalls==callsAtEnd+4&&zeroDeltaCalls==zeroCallsAtEnd+4,
          "Terminal hold did not drain four unchanged zero-motion samples");
    std::cout<<"PASS actual Died clip="<<death->clipName<<" MoveGO="<<death->moveGO
             <<" callbacks="<<motionCalls<<" appliedXY="<<appliedX<<','<<appliedY
             <<" authoredRootZ="<<expectedZ<<" nonzeroZSamples="<<nonzeroZCalls
             <<" actorZ="<<actor->transform.position[2]<<" terminal=held\n";
    return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
