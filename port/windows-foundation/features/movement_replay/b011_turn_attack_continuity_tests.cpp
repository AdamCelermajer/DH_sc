#include "../../actor_movement.hpp"
#include "../../combat_session.hpp"
#include "../../collision_scene.hpp"
#include "../../gameplay_camera.hpp"
#include "../../content_paths.hpp"
#include "../../original_actor_properties.hpp"
#include "../platform_input/semantic_input.hpp"
#include "../combat/runtime_player_combo_chain_v1.hpp"
#include "../../../level-world/actor_rotation.hpp"
#include "../../../level-world/navigation_heading.hpp"

#include <cmath>
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation;
namespace {
constexpr float pi=3.14159265358979323846f;
void check(bool ok,const std::string& why){if(!ok)throw std::runtime_error(why);}
void run_profile(const AssetCatalog& assets,const OriginalPropertyDatabase& properties,
                 const OriginalMeleeBindings& bindings,const std::string& profileId,
                 const std::string& skin,const std::string& label){
    std::string error;ActorCustomization custom;custom.skin_id_contains=skin;
    OriginalActorProperties resolved;
    check(resolve_original_actor_properties(properties.characters,properties.classes,profileId,{256,true},resolved,error),error);
    custom.expected_controller_count=4;custom.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    check(build_original_combat_visual_plan(assets,bindings,profileId,custom,
          "b011-"+label,plan,error),error);
    const auto* runClip=plan.phase("Run",0,{0});const auto* walkClip=plan.phase("Walk",0,{0});
    check(runClip&&runClip->has_visual()&&runClip->moveGO&&walkClip&&walkClip->has_visual()&&walkClip->moveGO,
          label+" actual authored Run/Walk MoveGO leaves absent");
    plan.config.motion_node_id="auto";plan.config.consume_root_motion=true;
    CombatSessionConfig config;config.playerId=1;config.playerProfileId=profileId;
    config.diagnosticRngSeed=20261010;config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=plan.config;
    CombatSessionProfile player;player.initialIdle={"Idle",0,{0}};
    player.sequenceAction=OriginalAttackSelection{"AttackStatic",0,{}};
    player.damageMarkerNames={"attack_mainhand"};player.propertyOptions={256,true};
    player.customization=custom;player.retainedPhaseClock=true;player.sourceCombo=true;
    player.sourceAnimationClips=plan.config.clips;config.profiles.emplace(profileId,player);
    CombatSessionProfile enemy;enemy.action={"Attack",0,{0,1}};enemy.initialIdle={"Idle",0,{0}};
    enemy.damageMarkerNames={"attack_mainhand"};enemy.propertyOptions={std::nullopt,true};
    enemy.customization.allow_missing_animation_targets=true;config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
    config.mainItemId="Longsword01";config.equippedItemIds={config.mainItemId};
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="b011-target-"+label;
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,50,-50,0,1};
    placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    CharacterVisual visual;CombatSession session;
    check(session.initialize(assets,properties,bindings,config,visual,population,{0,0,0},custom,error),error);
    for(const auto& row:std::array<std::pair<const char*,const char*>,3>{{
            {"idle","Idle"},{"run","Run"},{"walk","Walk"}}}){
        const auto* sequence=plan.sequence(row.second,0);check(sequence&&!sequence->phases.empty(),
            label+" source locomotion sequence missing: "+row.second);
        check(session.bind_player_locomotion(row.first,{row.second,0,{0}},
              resolved.walk_multiplier,sequence->loop!=0,error),error);
    }
    auto* actor=session.actor(1);auto* target=session.actor(2);
    check(actor&&target&&actor->alive()&&target->alive(),label+" Session actors absent");

    ActorMovementConfig mc;mc.turnSpeedRadians=resolved.turn_radians_per_second;
    mc.bodyRadius=1;mc.bodyHeight=2;mc.maxStepUp=10;mc.maxStepDown=10;
    mc.maxSlopeDegrees=45;mc.forwardAxis={0,-1,0};
    ActorMovement movement(mc);movement.setPosition({0,0,0},0);
    CameraPose camera;camera.position={100,-100,150};camera.target={0,0,0};camera.up={0,0,1};
    const auto basis=cameraMovementBasis(camera);platform_input::SemanticInput keyboard;
    std::uint32_t motionCalls=0,nonzeroMotionCalls=0,moveGoCalls=0;
    float cameraLeft=0;Vec3 rootLocal{},rootWorld{};
    session.set_motion_handler([&](ActorState& current,Vec3 delta,bool moveGo,std::string& callbackError){
        if(&current!=actor){callbackError="Foreign actor received B011 root motion";return false;}
        movement.setFacingRadians(current.transform.rotation[2]);if(moveGo)++moveGoCalls;else delta={};
        const auto before=movement.state().position;
        bool moved=false;
        const ActorMovement::SourceMotionAdmission clearFloor=[&](Vec3 from,Vec3 world,
            ActorMovement::SourceMotionAdmissionResult& admitted,std::string&){
            admitted={{from.x+world.x,from.y+world.y,from.z},true};return true;
        };
        if(!movement.apply_root_motion(delta,clearFloor,moved,callbackError))return false;
        const auto after=movement.state().position;
        rootLocal.x+=delta.x;rootLocal.y+=delta.y;rootLocal.z+=delta.z;
        rootWorld.x+=after.x-before.x;rootWorld.y+=after.y-before.y;
        cameraLeft+=(after.x-before.x)*basis.right.x+(after.y-before.y)*basis.right.y;
        current.transform.position={after.x,after.y,after.z};++motionCalls;
        if(std::hypot(delta.x,delta.y)>1e-6f)++nonzeroMotionCalls;
        callbackError.clear();return true;
    });

    // Sample alternating keyboard reversals in the same retained Session and
    // verify a released input actually requests no translation on the next tick.
    const char keys[]={'W','S','W','S'};char held=0;float previousDesired=0;
    for(unsigned segment=0;segment<4;++segment){
        if(held)keyboard.key(held,false);held=keys[segment];keyboard.key(held,true);
        unsigned settled=0;
        for(unsigned tick=0;tick<20;++tick){
            auto frame=keyboard.take_frame();const auto input=frame.actions;
            check(input.move2D.y==(held=='W'?1.f:-1.f),label+" keyboard W/S adapter changed axis sign");
            check(movement.steer_source_intent(input,basis,1.0/60.0),label+" source movement intent failed");
            check(session.select_player_locomotion(held=='W'?"run":"walk",error),error);
            const float wanted=movement.state().desiredHeading.sourceAngleRadians;
            if(tick==0&&segment>0)check(std::abs(std::abs(std::remainder(wanted-previousDesired,2*pi))-pi)<1e-5f,
                label+" W/S switch was not a non-collinear 180-degree source heading reversal");
            const auto before=movement.state().position;
            check(session.update(1.0/60.0,input,before,actor->transform.rotation[2],error),error);
            const float heading=movement.state().desiredHeading.sourceAngleRadians;
            const auto* sourceProperties=session.world()->combat_properties(1);
            check(sourceProperties,label+" source player property state missing");
            dh2::actor::RotationState rotation{{actor->transform.rotation[0],actor->transform.rotation[1],actor->transform.rotation[2]},
                heading,0,0};const dh2::actor::RotationPolicy policy{
                original_speed_modifier(sourceProperties->sheets.resolved[47]),16,1,1};std::uint32_t sync=0;
            check(dh2_actor_update_rotation(&rotation,&policy,&sync)==0,label+" recovered late-turn oracle rejected state");
            actor->transform.rotation[2]=rotation.rotation[2];movement.setFacingRadians(rotation.rotation[2]);
            if(segment>0&&settled==0&&std::abs(std::remainder(wanted-rotation.rotation[2],2*pi))<1e-5f)settled=tick+1;
        }
        if(segment>0)check(settled>0&&settled<=16,label+" source rotation did not settle after W/S reversal within 16 frames");
        previousDesired=movement.state().desiredHeading.sourceAngleRadians;
    }
    keyboard.key(held,false);auto released=keyboard.take_frame();
    check(released.actions.move2D.x==0&&released.actions.move2D.y==0,
          label+" key release left semantic movement held");
    check(movement.steer_source_intent(released.actions,basis,1.0/60.0),label+" neutral intent rejected");
    check(session.select_player_locomotion("idle",error),error);
    const auto releasePosition=movement.state().position;
    check(session.update(1.0/60.0,released.actions,releasePosition,actor->transform.rotation[2],error),error);
    check(std::hypot(movement.state().position.x-releasePosition.x,movement.state().position.y-releasePosition.y)<1e-6f,
          label+" released input continued to translate actor");

    // Select a non-collinear target, then use the original heading and bounded
    // rotation oracle over admitted attack frames. The target crosses to the
    // opposite side while the same attack generation remains active.
    target->transform.position[0]=actor->transform.position[0]+50;
    target->transform.position[1]=actor->transform.position[1]-50;
    const auto attackRootStart=rootLocal;const auto attackRootCountStart=nonzeroMotionCalls;
    const auto attackMoveGoStart=moveGoCalls;
    InputActions input;input.targetSelect=true;
    check(session.update(0,input,{movement.state().position.x,movement.state().position.y,0},actor->transform.rotation[2],error),error);
    check(actor->target_id==2,label+" non-collinear target selection failed");
    input={};input.attack=true;std::uint32_t attackFrames=0;
    const float startX=actor->transform.position[0],startY=actor->transform.position[1];
    for(unsigned tick=0;tick<5;++tick){
        const auto before=movement.state().position;
        check(session.update(1.0/60.0,input,before,actor->transform.rotation[2],error),error);
        const float dx=target->transform.position[0]-actor->transform.position[0];
        const float dy=target->transform.position[1]-actor->transform.position[1];
        float desired=0;const float direction[]{dx,dy,0};
        check(dh2_nav_look_towards(&desired,direction)==0,label+" non-collinear target heading rejected");
        const auto* sourceProperties=session.world()->combat_properties(1);
        dh2::actor::RotationState rotation{{actor->transform.rotation[0],actor->transform.rotation[1],actor->transform.rotation[2]},desired,0,0};
        const dh2::actor::RotationPolicy policy{original_speed_modifier(sourceProperties->sheets.resolved[47]),16,1,1};
        std::uint32_t sync=0;check(dh2_actor_update_rotation(&rotation,&policy,&sync)==0,label+" target turn oracle failed");
        actor->transform.rotation[2]=rotation.rotation[2];movement.setFacingRadians(rotation.rotation[2]);++attackFrames;
    }
    check(session.owns_pose(1)&&actor->action==CharacterAction::attacking,
          label+" admitted attack did not retain its source pose");
    const float firstTargetHeading=std::atan2(target->transform.position[0]-actor->transform.position[0],
                                               -(target->transform.position[1]-actor->transform.position[1]));
    check(std::abs(firstTargetHeading-pi/4)<1e-5f,label+" initial target was not at non-collinear heading +pi/4");
    target->transform.position[0]=actor->transform.position[0]-50;
    target->transform.position[1]=actor->transform.position[1]-50;
    for(unsigned tick=0;tick<5;++tick){
        const auto before=movement.state().position;
        check(session.update(1.0/60.0,input,before,actor->transform.rotation[2],error),error);
        const float dx=target->transform.position[0]-actor->transform.position[0];
        const float dy=target->transform.position[1]-actor->transform.position[1];float desired=0;const float direction[]{dx,dy,0};
        check(dh2_nav_look_towards(&desired,direction)==0,label+" retarget heading rejected");
        if(tick==0)check(std::abs(desired+pi/4)<1e-5f,label+" retarget did not cross to source heading -pi/4");
        const auto* sourceProperties=session.world()->combat_properties(1);
        dh2::actor::RotationState rotation{{actor->transform.rotation[0],actor->transform.rotation[1],actor->transform.rotation[2]},desired,0,0};
        const dh2::actor::RotationPolicy policy{original_speed_modifier(sourceProperties->sheets.resolved[47]),16,1,1};
        std::uint32_t sync=0;check(dh2_actor_update_rotation(&rotation,&policy,&sync)==0,label+" retarget turn oracle failed");
        actor->transform.rotation[2]=rotation.rotation[2];movement.setFacingRadians(rotation.rotation[2]);++attackFrames;
    }
    check(session.owns_pose(1)&&actor->target_id==2&&attackFrames==10,
          label+" non-collinear retarget interrupted the same Session attack/target");
    check(motionCalls>0&&nonzeroMotionCalls>0,
          label+" same-Session W/S movement delivered no source root motion calls="+
          std::to_string(motionCalls)+" nonzero="+std::to_string(nonzeroMotionCalls)+
          " actor="+std::to_string(actor->transform.position[0])+","+
          std::to_string(actor->transform.position[1]));
    check(cameraLeft<-1.0f,label+" W/S reversal did not reproduce camera-left root-motion arc");
    check(std::isfinite(firstTargetHeading)&&std::isfinite(actor->transform.rotation[2]),label+" heading became nonfinite");
    const float attackRootX=rootLocal.x-attackRootStart.x,attackRootY=rootLocal.y-attackRootStart.y;
    const auto attackRootCount=nonzeroMotionCalls-attackRootCountStart;
    const auto attackMoveGoCount=moveGoCalls-attackMoveGoStart;
    std::cout<<"PASS "<<label<<" sameSession=1 W/S/release=1 targetHeading=noncollinear retarget=1"
             <<" runClip="<<runClip->resolvedPath<<" walkClip="<<walkClip->resolvedPath
             <<" attackFrames="<<attackFrames<<" motionCalls="<<motionCalls<<" nonzeroRoot="<<nonzeroMotionCalls
             <<" attackMoveGO="<<attackMoveGoCount<<" attackRootCount="<<attackRootCount
             <<" attackLocalRoot="<<attackRootX<<","<<attackRootY
             <<" localRoot="<<rootLocal.x<<","<<rootLocal.y<<" admittedRoot="<<rootWorld.x<<","<<rootWorld.y
             <<" cameraLeft="<<cameraLeft
             <<" attackStart="<<startX<<","<<startY<<" yaw="<<actor->transform.rotation[2]<<"\n";
}
}
int main(int argc,char** argv){try{
    check(argc==2,"Supply unified original asset root");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase properties;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",properties,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    run_profile(assets,properties,bindings,"KnightPlayerBase","_default_warrior-mesh-skin","knight");
    run_profile(assets,properties,bindings,"RoguePlayerBase","_default_rogue-mesh-skin","rogue");
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
