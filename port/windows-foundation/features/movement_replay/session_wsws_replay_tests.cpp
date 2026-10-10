#include "../../combat_session.hpp"
#include "../../actor_movement.hpp"
#include "../../collision_scene.hpp"
#include "../../gameplay_camera.hpp"
#include "../../original_actor_motion_flags.hpp"
#include "../../original_actor_properties.hpp"
#include "../platform_input/semantic_input.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool condition,const std::string& message){if(!condition)throw std::runtime_error(message);}
void floor_rect(CollisionScene& scene,float extent){
    const Vec3 a{-extent,-extent,0},b{extent,-extent,0},c{extent,extent,0},d{-extent,extent,0};
    scene.triangles.push_back({a,b,c,true});scene.triangles.push_back({a,c,d,true});
}
}

int main(int argc,char** argv){try{
    check(argc==3,"Supply repository root and CSV output path");
    const auto repository=std::filesystem::path(argv[1]);
    AssetCatalog assets(repository/".local-inputs/windows-shared-assets"),metadata(assets.root());
    std::string error;OriginalPropertyDatabase database;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    OriginalActorProperties properties;
    check(resolve_original_actor_properties(database.characters,database.classes,"KnightPlayerBase",{256,true},properties,error),error);
    check(bindings.load(metadata,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";
    customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"session-wsws-replay",plan,error),error);
    const auto* run=plan.phase("Run",0,{0});
    const auto* walk=plan.phase("Walk",0,{0});
    check(run&&run->has_visual()&&run->moveGO,"Original Knight Run leaf/MoveGO unavailable");
    check(walk&&walk->has_visual()&&walk->moveGO,"Original Knight Walk leaf/MoveGO unavailable");
    constexpr float pi=3.14159265358979323846f;
    const double dt=1.0/60.0;const unsigned framesPerKey=45;

    CombatSessionConfig config;config.playerId=1;config.playerProfileId="KnightPlayerBase";
    config.diagnosticRngSeed=20261010;config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=plan.config;config.playerVisualConfig.motion_node_id="auto";
    config.playerVisualConfig.consume_root_motion=true;
    config.mainItemId="Longsword01";config.equippedItemIds={config.mainItemId};
    CombatSessionProfile player;player.retainedPhaseClock=true;player.initialIdle={"Idle",0,{0}};
    player.damageMarkerNames={"attack_mainhand"};player.propertyOptions={256,true};
    OriginalAttackSelection attack;attack.state="AttackStatic";player.sequenceAction=attack;
    config.profiles.emplace(config.playerProfileId,player);
    CharacterVisual visual;ActorPopulation population;CombatSession session;
    check(session.initialize(assets,database,bindings,config,visual,population,{0,0,0},customization,error),error);
    check(session.bind_player_locomotion("run",{"Run",0,{0}},properties.walk_multiplier,true,error),error);
    check(session.bind_player_locomotion("walk",{"Walk",0,{0}},properties.walk_multiplier,true,error),error);
    auto* actor=session.actor(1);check(actor&&actor->alive(),"Same Session did not bind the source player");
    const auto bindingLease=session.actor_binding_lease();const auto* poseOwner=session.retained_player_pose();
    check(poseOwner&&!bindingLease.expired(),"Source retained pose owner was not issued");

    CameraPose camera;camera.position={100,-100,150};camera.target={0,0,0};camera.up={0,0,1};
    const auto basis=cameraMovementBasis(camera);
    ActorMovementConfig movementConfig;movementConfig.turnSpeedRadians=properties.turn_radians_per_second;
    movementConfig.bodyRadius=1;movementConfig.bodyHeight=2;movementConfig.maxStepUp=10;
    movementConfig.maxStepDown=10;movementConfig.maxSlopeDegrees=45;
    movementConfig.forwardAxis={0,-1,0};movementConfig.runAnimation="run";
    ActorMovement movement(movementConfig);
    movement.setPosition({0,0,0});
    check(movement.setDesiredHeading({basis.forward.x,basis.forward.y,0}),"Initial camera-forward heading rejected");
    movement.setFacingRadians(movement.state().desiredHeading.sourceAngleRadians);
    actor->transform.rotation[2]=movement.state().facingRadians;
    CollisionScene collision;floor_rect(collision,10000);
    platform_input::SemanticInput keyboard;
    OriginalMotionPrefixResult focus;
    check(original_motion_focus_prefix(OriginalMotionFocusPrefix::move,actor->source_flags520,
        actor->source_movement_type,0,focus,error)&&focus.flags_written&&*actor->source_flags520==0x23c1,error);
    std::uint32_t* turnPositive=nullptr;
    check(session.borrow_rotation_turn(actor->id,turnPositive,error)&&turnPositive,error);

    const auto outputCsvPath=std::filesystem::path(argv[2]);
    std::ofstream csv(outputCsvPath);check(bool(csv),"Movement replay CSV output unavailable");
    csv<<"frame,key,shift,run,alias,move_x,move_y,basis_right_x,basis_right_y,basis_forward_x,basis_forward_y,desired_heading,facing_before,facing_after,local_root_x,local_root_y,local_root_z,requested_world_x,requested_world_y,admitted_world_x,admitted_world_y,collision_moved,collision_blocked,actor_x,actor_y,motion_calls,current_slot,current_time_ms,generation_sum\n"<<std::setprecision(9);
    struct FrameTrace {unsigned calls=0,nonzeroCalls=0;Vec3 local{},requested{},admitted{};bool moved=false,blocked=false;};
    FrameTrace trace;unsigned frame=0,totalCalls=0,admittedFrames=0,blockedFrames=0,multiCallbackFrames=0,maxMotionCalls=0;
    double movementMsFraction=0;float totalLocal=0,totalWorld=0,cameraLeft=0;
    session.set_motion_handler([&](ActorState& current,Vec3 delta,bool moveGo,std::string& callbackError){
        if(&current!=actor||current.id!=session.player_id()) {callbackError="Root motion reached a foreign Session actor";return false;}
        movement.setFacingRadians(current.transform.rotation[2]);
        if(!moveGo)delta={};
        const float facing=movement.state().facingRadians,c=std::cos(facing),s=std::sin(facing);
        const Vec3 requested{(delta.x*c-delta.y*s),(delta.x*s+delta.y*c),0};
        const Vec3 before=movement.state().position;
        const bool moved=movement.apply_root_motion(delta,collision);
        const auto after=movement.state().position;
        trace.local.x+=delta.x;trace.local.y+=delta.y;trace.local.z+=delta.z;
        if(std::hypot(delta.x,delta.y)>1e-6f)++trace.nonzeroCalls;
        trace.requested.x+=requested.x;trace.requested.y+=requested.y;
        trace.admitted.x+=after.x-before.x;trace.admitted.y+=after.y-before.y;
        trace.moved=trace.moved||moved;trace.blocked=trace.blocked||(!moved&&(std::abs(requested.x)+std::abs(requested.y)>1e-6f));
        ++trace.calls;++totalCalls;
        current.transform.position={after.x,after.y,after.z};
        callbackError.clear();return true;
    });

    const char keys[]{'W','S','W','S'};
    const bool shifted[]{false,true,false,true};
    char held=0;bool shiftHeld=false;std::uint64_t previousGenerationSum=0;
    unsigned settleFrames[4]{};float previousDesired=0;
    for(unsigned segment=0;segment<4;++segment){
        if(held)keyboard.key(held,false);
        if(shiftHeld)keyboard.key(0x10,false);
        held=keys[segment];keyboard.key(held,true);
        shiftHeld=shifted[segment];if(shiftHeld)keyboard.key(0x10,true);
        for(unsigned inSegment=0;inSegment<framesPerKey;++inSegment,++frame){
            const auto inputFrame=keyboard.take_frame();const auto input=inputFrame.actions;
            const float expectedY=held=='W'?1.f:-1.f;
            check(input.move2D.x==0&&input.move2D.y==expectedY&&input.run==!shiftHeld,
                  "Semantic keyboard did not produce the expected default-run/Shift-walk W/S intent");
            check(movement.steer_source_intent(input,basis,dt),"Source intent steering rejected");
            const float desiredHeading=movement.state().desiredHeading.sourceAngleRadians;
            if(inSegment==0&&segment>0)check(std::abs(std::abs(std::remainder(desiredHeading-previousDesired,2*pi))-pi)<1e-5f,
                  "W/S transition was not a nonaligned 180-degree reversal");
            const std::string alias=input.run?"run":"walk";
            check(session.select_player_locomotion(alias,error),error);
            trace={};const float facingBefore=actor->transform.rotation[2];
            const auto positionBefore=movement.state().position;
            check(session.update(dt,input,positionBefore,facingBefore,error),error);
            check(session.retained_player_pose()==poseOwner&&!bindingLease.expired(),
                  "W/S replay replaced the retained pose owner or Session lease");
            std::uint64_t generationSum=0;
            for(const auto& slot:poseOwner->slots())generationSum+=slot.generation;
            const bool firstFrame=inSegment==0;
            check(generationSum>=previousGenerationSum,"Retained locomotion generation moved backward");
            if(firstFrame&&segment>0)check(generationSum>previousGenerationSum,
                  "Run/walk input transition did not select its new source locomotion clip");
            if(!firstFrame)check(generationSum-previousGenerationSum<=1,
                  "One held input frame selected multiple locomotion clips");
            const auto generationDelta=generationSum-previousGenerationSum;
            if(!firstFrame&&generationDelta>0)check(generationDelta==1&&
                poseOwner->slots()[poseOwner->current_slot()].timeline.current_ms==0&&trace.calls==2,
                "Retained generation changed away from the source clip completion/repeat boundary");
            if(trace.calls==2)check(generationDelta==1&&
                poseOwner->slots()[poseOwner->current_slot()].timeline.current_ms==0,
                "Multiple root callbacks were not the source completion plus same-time repeat");
            previousGenerationSum=generationSum;
            check(trace.calls>0,"Same Session did not deliver its retained root-motion sample");
            check(trace.nonzeroCalls==(frame==0?0u:1u),"Session root displacement count="+
                  std::to_string(trace.nonzeroCalls)+" at frame="+std::to_string(frame));
            check(std::abs(actor->transform.position[0]-movement.state().position.x)<1e-5f&&
                  std::abs(actor->transform.position[1]-movement.state().position.y)<1e-5f,
                  "Session ActorState diverged from the connected movement owner");
            const auto heading=movement.state().desiredHeading.sourceAngleRadians;
            const double elapsedMs=dt*1000.0+movementMsFraction;
            const auto sourceMs=static_cast<std::uint32_t>(elapsedMs);movementMsFraction=elapsedMs-sourceMs;
            ActorMovement::SourceRotationBorrow rotation{actor->transform.rotation.data(),&heading,turnPositive};
            auto sync=[&](const float* actual,std::string& syncError){
                if(actual!=actor->transform.rotation.data()){syncError="Late rotation used a different Session Euler";return false;}
                syncError.clear();return true;
            };
            check(movement.apply_source_character_rotation_late(rotation,&*actor->source_flags520,
                properties.sheets.resolved.data(),sourceMs,true,sync,error),error);
            const float facingAfter=actor->transform.rotation[2];
            if(segment>0&&settleFrames[segment]==0&&
               std::abs(std::remainder(desiredHeading-facingAfter,2*pi))<1e-5f)
                settleFrames[segment]=inSegment+1;
            if(inSegment+1==framesPerKey){
                previousDesired=desiredHeading;
                if(segment>0)check(settleFrames[segment]>0&&settleFrames[segment]<=16,
                    "Source bounded rotation failed to settle after the W/S reversal within 16 frames");
            }
            const Vec3 admitted{movement.state().position.x-positionBefore.x,
                                movement.state().position.y-positionBefore.y,0};
            const float reqWorldX=trace.requested.x,reqWorldY=trace.requested.y;
            totalLocal+=std::hypot(trace.local.x,trace.local.y);
            totalWorld+=std::hypot(admitted.x,admitted.y);
            cameraLeft+=admitted.x*basis.right.x+admitted.y*basis.right.y;
            if(trace.calls>1)++multiCallbackFrames;
            maxMotionCalls=std::max(maxMotionCalls,trace.calls);
            admittedFrames+=trace.moved?1u:0u;blockedFrames+=trace.blocked?1u:0u;
            csv<<frame<<','<<held<<','<<(shiftHeld?1:0)<<','<<(input.run?1:0)<<','<<alias<<','<<input.move2D.x<<','<<input.move2D.y<<','
               <<basis.right.x<<','<<basis.right.y<<','<<basis.forward.x<<','<<basis.forward.y<<','
               <<heading<<','<<facingBefore<<','<<facingAfter<<','<<trace.local.x<<','<<trace.local.y<<','<<trace.local.z<<','
               <<reqWorldX<<','<<reqWorldY<<','<<trace.admitted.x<<','<<trace.admitted.y<<','
               <<(trace.moved?1:0)<<','<<(trace.blocked?1:0)<<','<<actor->transform.position[0]<<','
               <<actor->transform.position[1]<<','<<trace.calls<<','<<poseOwner->current_slot()<<','
               <<poseOwner->slots()[poseOwner->current_slot()].timeline.current_ms<<','<<generationSum<<'\n';
        }
    }
    keyboard.key(held,false);if(shiftHeld)keyboard.key(0x10,false);
    check(totalCalls>=4*framesPerKey&&admittedFrames>0,"Run/walk trace did not deliver/apply all source root motion");
    check(blockedFrames==0,"Clear-floor W/S fixture unexpectedly collided");
    check(cameraLeft<-1.0f,"Expected bounded-turn/root-motion camera-left arc was not reproduced");
    check(std::abs(actor->transform.position[0]-movement.state().position.x)<1e-5f&&
          std::abs(actor->transform.position[1]-movement.state().position.y)<1e-5f,
          "Final actor/motor positions differ after W/S replay");
    std::cout<<std::setprecision(9)<<"PASS sameSession=1 frames="<<frame<<" runClip="<<run->resolvedPath<<" walkClip="<<walk->resolvedPath
             <<" rate="<<run->speed*properties.walk_multiplier<<" basisForward="<<basis.forward.x<<','<<basis.forward.y
             <<" localRootXY="<<totalLocal<<" admittedWorldXY="<<totalWorld
             <<" cameraLeftDelta="<<cameraLeft<<" collisionAcceptedFrames="<<admittedFrames
             <<" collisionBlockedFrames="<<blockedFrames<<" modeTransitions=3 multiCallbackFrames="<<multiCallbackFrames
             <<" maxMotionCalls="<<maxMotionCalls<<" actor="<<actor->transform.position[0]<<','
             <<actor->transform.position[1]<<" csv="<<outputCsvPath.string()<<'\n';
    return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
