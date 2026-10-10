#include "../../combat_session.hpp"
#include "../../retained_pose_playback.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
bool same_pose(const SkeletalPose& a,const SkeletalPose& b){
    if(a.size()!=b.size())return false;
    for(std::size_t i=0;i<a.size();++i){
        if(a[i].id!=b[i].id)return false;
        for(unsigned j=0;j<3;++j)if(std::abs(a[i].translation[j]-b[i].translation[j])>1e-6f||
            std::abs(a[i].scale[j]-b[i].scale[j])>1e-6f)return false;
        for(unsigned j=0;j<4;++j)if(std::abs(a[i].quaternion[j]-b[i].quaternion[j])>1e-6f)return false;
    }
    return true;
}
}
int main(int argc,char** argv){try{
    check(argc==2,"Supply original shared asset root");
    AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase database;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan playerPlan,npcPlan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"b012-player",playerPlan,error),error);
    check(build_original_combat_visual_plan(assets,bindings,"Swamp_LizadMan_Type1",customization,"b012-lizard",npcPlan,error),error);
    const auto* died=npcPlan.phase("Died",0,{0});
    check(died&&!died->clipName.empty()&&died->moveGO,"Authored Lizard Died leaf/MoveGO is unavailable");

    CombatSessionConfig config;config.diagnosticRngSeed=20261010;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=playerPlan.config;
    CombatSessionProfile player;player.action={"AttackStatic",0,{0,1}};player.initialIdle={"Idle",0,{0}};
    player.damageMarkerNames={"attack_mainhand"};player.customization=customization;player.propertyOptions={256,true};
    config.profiles.emplace(config.playerProfileId,player);
    CombatSessionProfile enemy;enemy.action={"Attack",0,{0,1}};enemy.initialIdle={"Idle",0,{0}};
    enemy.death=CombatSessionChoice{"Died",0,{0}};enemy.damageMarkerNames={"attack_mainhand"};
    enemy.sequenceAction=OriginalAttackSelection{"Attack",0,{0}};enemy.retainedPhaseClock=true;
    enemy.customization=customization;enemy.motionRoot="auto";enemy.propertyOptions={std::nullopt,true};
    config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="b012-mid-swing-lizard";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-10,17,1};placed.transform=placed.definition.placement;
    population.actors().push_back(std::move(placed));
    CharacterVisual playerVisual;CombatSession session;
    check(session.initialize(assets,database,bindings,config,playerVisual,population,{0,0,0},customization,error),error);
    auto* actor=session.actor(2);check(actor&&actor->alive(),"Lizard did not bind in the same Session");
    const std::string idleClip=population.actors().front().visual.animation_name();
    check(session.request_actor_attack(2,1,0,error),error);
    check(actor->action==CharacterAction::attacking&&session.owns_pose(2),
          "Lizard did not enter its authored attack before the lethal hit");
    const auto* attackPose=session.retained_actor_pose(2);
    check(attackPose&&attackPose->slots()[attackPose->current_slot()].clip_id!=idleClip,
          "Lizard attack visual was not active before the lethal hit; clip="+
          (attackPose?attackPose->slots()[attackPose->current_slot()].clip_id:"<no retained pose>"));

    const auto lease=session.actor_binding_lease();
    CombatSessionSourceHit hit;hit.attacker=1;hit.target=2;hit.binding_lease=lease;
    hit.source_id="b012-mid-swing-lethal";hit.marker_name="do_skill";hit.generation=1;
    hit.mask=0x20080000u;hit.direct_amount=static_cast<std::int32_t>(std::ceil(actor->health*256.0f));
    DamageEvent receipt;check(session.apply_source_result(hit,receipt,error)&&receipt.target_died,error);
    check(actor->action==CharacterAction::dead&&session.owns_pose(2),
          "Lethal hit did not replace the in-flight attack with a death-owned pose");
    const auto* deathPose=session.retained_actor_pose(2);
    check(deathPose,"Died lost its retained pose owner");
    const std::string deathClip=deathPose->slots()[deathPose->current_slot()].clip_id;
    check(deathClip.find("/Died/sequence-0/phase-0")!=std::string::npos,
          "Lethal in-flight attack selected "+deathClip+" instead of source Died leaf");
    std::int32_t deathStart=0,deathEnd=0;
    const auto* deathVisual=session.retained_actor_visual_borrow(2);
    check(deathVisual&&deathVisual->animation_range(deathClip,deathStart,deathEnd,error),error);
    check(!deathPose->current_ended(),"Died began ended; timeline="+
          std::to_string(deathPose->slots()[deathPose->current_slot()].timeline.current_ms)+" end="+
          std::to_string(deathEnd)+" clip="+deathPose->slots()[deathPose->current_slot()].clip_id);

    bool collectingDeathMotion=false;unsigned deathMotionSamples=0;double appliedX=0,appliedY=0;
    session.set_motion_handler([&](ActorState& current,Vec3 delta,bool moveGo,std::string&){
        check(&current==session.actor(2),"Died root motion reached a foreign actor");
        if(collectingDeathMotion){++deathMotionSamples;if(moveGo){
            current.transform.position[0]+=delta.x;current.transform.position[1]+=delta.y;
            appliedX+=delta.x;appliedY+=delta.y;
        }}
        return true;
    });
    collectingDeathMotion=true;
    InputActions input;const double dt=1.0/60.0;
    const unsigned frames=static_cast<unsigned>(std::ceil((deathEnd-deathStart)/1000.0/dt))+6;
    bool observedProgress=false;
    for(unsigned frame=0;frame<frames;++frame){
        check(session.update(dt,input,{0,0,0},0,error),error);
        const auto* pose=session.retained_actor_pose(2);
        check(pose&&pose->slots()[pose->current_slot()].clip_id==deathClip,
              "In-flight death changed visual owner before terminal completion");
        const auto& timeline=pose->slots()[pose->current_slot()].timeline;
        observedProgress=observedProgress||timeline.current_ms>0;
        check(timeline.current_ms<=deathEnd,"Died timeline advanced beyond its authored end");
        if(pose->current_ended())break;
    }
    deathPose=session.retained_actor_pose(2);
    check(observedProgress&&deathPose&&deathPose->current_ended(),
          "In-flight Died clip froze before its authored terminal sample");
    check(deathPose->slots()[deathPose->current_slot()].timeline.current_ms==deathEnd,
          "In-flight Died did not publish its authored terminal timestamp");
    check(deathMotionSamples>0&&std::abs(appliedX)+std::abs(appliedY)>1.0,
          "Authored Died MoveGO root motion was not delivered while the clip advanced");
    SkeletalPose terminalPose;check(deathVisual->current_local_pose(terminalPose,error),error);
    const auto terminal=actor->transform.position;
    for(unsigned frame=0;frame<4;++frame)check(session.update(0,input,{0,0,0},0,error),error);
    check(actor->transform.position==terminal,"Terminal Died hold replayed root displacement");
    SkeletalPose heldPose;check(deathVisual->current_local_pose(heldPose,error),error);
    check(same_pose(terminalPose,heldPose),"Terminal Died pose changed during zero-time held updates");
    check(std::abs(actor->transform.position[2]-17.0f)<1e-6f,
          "Source root motion changed actor Z without an authored Z displacement");
    std::cout<<"PASS lethal mid-swing clip="<<deathClip<<" range="<<deathStart<<".."<<deathEnd
             <<" terminal=held deathMotionSamples="<<deathMotionSamples
             <<" appliedXY="<<appliedX<<','<<appliedY<<" actorZ="<<actor->transform.position[2]<<'\n';
    return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
