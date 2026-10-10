#include "../combat_session.hpp"
#include "../camera.hpp"
#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;

namespace {
void check(bool condition,const std::string& message){if(!condition)throw std::runtime_error(message);}

bool project_visible(const CameraPose& camera,Vec3 point,float aspect){
    const auto view=cameraViewMatrix(camera);
    const auto projection=cameraProjectionMatrix(camera.verticalFovDegrees,aspect,.5f,100000.f);
    const float world[]{point.x,point.y,point.z,1};float eye[4]{},clip[4]{};
    for(unsigned row=0;row<4;++row)for(unsigned col=0;col<4;++col)eye[row]+=view[col*4+row]*world[col];
    for(unsigned row=0;row<4;++row)for(unsigned col=0;col<4;++col)clip[row]+=projection[col*4+row]*eye[col];
    if(!std::isfinite(clip[3])||clip[3]<=0)return false;
    const float x=clip[0]/clip[3],y=clip[1]/clip[3],z=clip[2]/clip[3];
    return x>=-1&&x<=1&&y>=-1&&y<=1&&z>=-1&&z<=1;
}
}

int main(int argc,char** argv){try{
    check(argc==2,"Supply original shared asset root");AssetCatalog assets(argv[1]);
    OriginalPropertyDatabase database;OriginalMeleeBindings bindings;std::string error;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";
    customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"target-retention-test",plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=plan.config;config.playerVisualConfig.motion_node_id="auto";
    config.playerVisualConfig.consume_root_motion=true;
    CombatSessionProfile player;player.initialIdle={"Idle",0,{0}};player.damageMarkerNames={"attack_mainhand"};
    player.propertyOptions={256,true};OriginalAttackSelection selection;selection.state="AttackStatic";
    player.sequenceAction=selection;player.retainedPhaseClock=true;player.sourceCombo=true;
    config.profiles.emplace("KnightPlayerBase",player);
    CombatSessionProfile target;target.action={"Attack",0,{0,1}};target.initialIdle={"Idle",0,{0}};
    target.damageMarkerNames={"attack_mainhand"};target.propertyOptions={std::nullopt,true};
    target.customization.allow_missing_animation_targets=true;
    config.profiles.emplace("Swamp_LizadMan_Type1",target);
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="explicit-target-retention-test";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    CharacterVisual visual;CombatSession session;
    check(session.initialize(assets,database,bindings,config,visual,population,{0,0,0},customization,error),error);

    InputActions input;input.targetSelect=true;check(session.update(0,input,{0,0,0},0,error),error);
    check(session.actor(1)->target_id==2&&session.selectedactor()&&session.selectedactor()->id==2,
          "Manual target selection did not publish the same ActorId through target_id and selectedactor");

    // Keep a valid target through a complete source combo. The live target is
    // deliberately outside this camera's view, so selection and presentation
    // visibility are checked as independent facts.
    CameraPose camera;camera.position={0,0,10};camera.target={0,0,0};camera.up={0,1,0};
    check(!project_visible(camera,{0,-100,0},16.f/9.f),"Offscreen target unexpectedly passed the HUD projection gate");
    input.targetSelect=false;input.attack=true;check(session.update(0,input,{0,0,0},0,error),error);
    check(session.owns_pose(1),"Player source combo did not start");input.attack=false;
    for(unsigned frame=0;frame<1000&&session.owns_pose(1);++frame)
        check(session.update(.016,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
    check(!session.owns_pose(1),"Released source combo did not finish");
    check(session.actor(1)->target_id==2&&session.selectedactor()&&session.selectedactor()->id==2,
          "Valid manually selected target was lost at source-combo finish");
    check(!project_visible(camera,{session.selectedactor()->transform.position[0],
                                   session.selectedactor()->transform.position[1],
                                   session.selectedactor()->transform.position[2]},16.f/9.f),
          "Presentation projection no longer demonstrates the offscreen case");

    // Model the authoritative target being cleared between session frames, as
    // an explicit skill ClearTarget publishes. Do not resurrect that clear.
    session.actor(1)->target_id=invalid_actor_id;input={};
    check(session.update(0,input,{0,0,0},0,error),error);
    check(session.actor(1)->target_id==invalid_actor_id&&!session.selectedactor(),
          "An explicit target clear was resurrected by sticky-target retention");

    input.targetSelect=true;check(session.update(0,input,{0,0,0},0,error),error);
    check(session.selectedactor()&&session.selectedactor()->id==2,"Target could not be selected again before death invalidation");
    session.actor(2)->health=0;input.targetSelect=false;
    check(session.update(0,input,{0,0,0},0,error),error);
    check(session.actor(1)->target_id==invalid_actor_id&&!session.selectedactor(),
          "Dead target remained selected after ordinary invalid-target housekeeping");

    std::cout<<"PASS manual source-combo target retention, explicit clear/death invalidation, and offscreen HUD projection distinction\n";
    return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
