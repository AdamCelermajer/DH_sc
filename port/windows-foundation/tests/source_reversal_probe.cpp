#include "../actor_movement.hpp"
#include "../gameplay_camera.hpp"
#include "../original_actor_properties.hpp"
#include "../original_combat_visual_plan.hpp"
#include "../retained_animation_owner.hpp"
#include "../../level-world/visual_motion.hpp"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool ok,const std::string& error){if(!ok)throw std::runtime_error(error);}
int main(int argc,char** argv){try{
    check(argc==3,"Supply repository root and output CSV path");const auto repository=std::filesystem::path(argv[1]);
    AssetCatalog assets(repository/".local-inputs/windows-source-clock-checkpoint-v17/assets"),metadata(assets.root());
    OriginalPropertyDatabase database;OriginalActorProperties properties;OriginalMeleeBindings bindings;std::string error;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(resolve_original_actor_properties(database.characters,database.classes,"KnightPlayerBase",{256,true},properties,error),error);
    check(bindings.load(metadata,"original-melee-bindings.xml",error),error);
    ActorCustomization custom;custom.skin_id_contains="_default_warrior-mesh-skin";custom.expected_controller_count=4;custom.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",custom,"source-reversal",plan,error),error);
    const auto* run=plan.phase("Run",0,{0});check(run&&run->has_visual(),"Original source run phase absent");
    auto config=plan.config;config.clips={{run->clipName,run->resolvedPath}};config.motion_node_id="auto";config.consume_root_motion=true;
    CharacterVisual visual;check(visual.load(assets,config,error),error);
    auto sampler=[&](const std::string& clip,int ms,std::array<float,3>& scratch,std::string& e){Vec3 point{scratch[0],scratch[1],scratch[2]};if(!visual.sample_source_root_translation(clip,ms,point,e))return false;scratch={point.x,point.y,point.z};return true;};
    RetainedAnimationOwner animation(visual,sampler);const auto bytes=assets.read(run->resolvedPath);check(animation.bind_events(run->clipName,bytes.data(),bytes.size(),error),error);
    const float rate=static_cast<float>(run->speed)*properties.walk_multiplier;
    RetainedAnimationFrame selected;check(animation.select(run->clipName,false,rate,static_cast<int>(run->blendOut),run->moveGO!=0,selected,error),error);
    ActorMovementConfig movementConfig;movementConfig.bodyRadius=1;movementConfig.bodyHeight=2;movementConfig.maxStepUp=10;movementConfig.maxStepDown=10;movementConfig.maxSlopeDegrees=45;movementConfig.forwardAxis={0,-1,0};movementConfig.runAnimation=run->clipName;
    ActorMovement movement(movementConfig);constexpr float pi=3.14159265358979323846f;movement.setPosition({0,0,0},-pi);
    float euler[3]{0,0,-pi};std::uint32_t turn=0,flags=0x23c1;double fractionMs=0;
    CameraMovementBasis basis;basis.forward={0,1,0};basis.right={1,0,0};
    float directPosition[3]{};float totalMotion=0,maxDifference=0;float reversalStartX=0;unsigned settledFrames=0;
    std::ofstream csv(argv[2]);check(bool(csv),"CSV output unavailable");csv<<"frame,input_y,heading,facing_before,facing_after,root_x,root_y,world_x,world_y,direct_original_x,direct_original_y\n"<<std::setprecision(9);
    ActorMovement::SourceMotionAdmission freeFloor=[](Vec3 from,Vec3 delta,ActorMovement::SourceMotionAdmissionResult& output,std::string&){output={{from.x+delta.x,from.y+delta.y,0},true};return true;};
    constexpr double dt=1.0/60.0;
    for(unsigned frame=0;frame<225;++frame){
        InputActions input;input.move2D.y=((frame/45)%2)==0?1.f:-1.f;input.run=true;
        check(movement.steer_source_intent(input,basis,dt),"Source intent rejected");
        const float before=euler[2];RetainedAnimationFrame sampled;check(animation.advance(dt,sampled,error),error);
        dh2::visual::Root source{};source.position[0]=directPosition[0];source.position[1]=directPosition[1];source.scale[0]=source.scale[1]=source.scale[2]=1;source.presence=1;
        check(dh2_visual_rotation(source.quaternion,euler)==0,"Original visual rotation rejected");
        const float delta[]{sampled.authored_motion.x,sampled.authored_motion.y,sampled.authored_motion.z};
        const dh2::visual::Displacement displacement{&source,delta,1,0};check(dh2_visual_displace(&displacement)>=0,"Original displacement rejected");
        directPosition[0]=source.position[0];directPosition[1]=source.position[1];
        bool moved=false;check(movement.apply_root_motion(sampled.authored_motion,freeFloor,moved,error),error);
        maxDifference=std::max(maxDifference,std::hypot(movement.state().position.x-directPosition[0],movement.state().position.y-directPosition[1]));
        const double milliseconds=dt*1000+fractionMs;const auto integerMs=static_cast<std::uint32_t>(milliseconds);fractionMs=milliseconds-integerMs;
        const float heading=movement.state().desiredHeading.sourceAngleRadians;
        check(movement.apply_source_character_rotation_late({euler,&heading,&turn},&flags,properties.sheets.resolved.data(),integerMs,true,
            [](const float*,std::string&){return true;},error),error);
        if(frame==44)reversalStartX=movement.state().position.x;
        if(frame>=45&&frame<90&&std::abs(euler[2]-heading)<0.00001f&&settledFrames==0)settledFrames=frame-44;
        totalMotion+=std::hypot(sampled.authored_motion.x,sampled.authored_motion.y);
        csv<<frame<<','<<input.move2D.y<<','<<heading<<','<<before<<','<<euler[2]<<','<<delta[0]<<','<<delta[1]<<','<<movement.state().position.x<<','<<movement.state().position.y<<','<<directPosition[0]<<','<<directPosition[1]<<'\n';
        const auto completion=animation.take_completion();if(completion.pending)check(animation.select(run->clipName,false,rate,static_cast<int>(run->blendOut),run->moveGO!=0,selected,error,completion.extra_ms),error);
    }
    check(maxDifference<0.001f,"Current root world transform differs from direct original visual displacement");
    check(movement.state().position.x<reversalStartX,"Axis-aligned alternating reversals did not reproduce source left arc");
    std::cout<<std::setprecision(9)<<"source reversal sourceClip="<<run->resolvedPath<<" rate="<<rate<<" rotationMultiplier="<<properties.rotation_multiplier<<" settledFrames="<<settledFrames<<" leftDelta="<<movement.state().position.x-reversalStartX<<" rootDistance="<<totalMotion<<" directOriginalMaxDifference="<<maxDifference<<'\n';
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
