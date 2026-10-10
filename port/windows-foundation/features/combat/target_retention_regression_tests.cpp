#include "../../combat_session.hpp"
#include "../../camera.hpp"
#include "auto_target_marker_v1.hpp"
#include "session_source_object_interest_v1.hpp"
#include "../skills_animation/source_skill_animation.hpp"
#include "../../../level-world/character_skill_ai_v3.hpp"
#include "../../../level-world/character_skill_callbacks_v3.hpp"
#include "../../../level-world/character_skill_state_v4.hpp"
#include "../../../game-data/skill_tables.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;

namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
struct SkillHost {
    CombatSession* session=nullptr;
    dh2::character::State state;
    bool using_skill=false;
    unsigned pres=0,uses=0,posts=0;
    static int ai(void* context,dh2::character::skills::SkillAIContextV3*,
                  const dh2::character::skills::SkillAIRequest32V3* request,
                  dh2::character::skills::SkillAIResponse32V3* response){
        using namespace dh2::character::skills;auto& self=*static_cast<SkillHost*>(context);*response={};
        if(request->operation==skill_ai_using_v3){response->word=self.using_skill;return 0;}
        if(request->operation==skill_ai_casting_v3||request->operation==skill_ai_player_v3){response->word=0;return 0;}
        if(request->operation!=skill_ai_callback_v3)return -1;
        if(request->value==skill_check_usable_v3){response->word=1;return 0;}
        if(request->value==skill_pre_v3){++self.pres;response->word=1;return 0;}
        if(request->value==skill_use_v3){++self.uses;response->word=1;return 0;}
        if(request->value==skill_post_v3){
            ++self.posts;
            if(!self.session||!self.session->actor(1))return -1;
            // The real BashDown Lua Post calls ClearTarget; this callback fixture
            // publishes SetTarget(NULL,false) then SyncLastTarget to the same
            // Session source target owner.
            std::string error;
            if(!self.session->set_source_target(1,invalid_actor_id,false,error))return -1;
            auto presentation=self.session->target_presentation_state(1);
            if(presentation.current!=invalid_actor_id||!presentation.last_target_known||presentation.last_target!=2)return -1;
            if(!self.session->sync_source_last_target(1,error))return -1;
            presentation=self.session->target_presentation_state(1);
            return presentation.last_target_known&&presentation.last_target==invalid_actor_id?0:-1;
        }
        if(request->value==skill_check_active_v3){response->word=0;return 0;}
        return -1;
    }
    static int state_service(void* context,dh2::character::skills::SkillStateV4*,
                  const dh2::character::skills::SkillStateRequest32V4* request,
                  dh2::character::skills::SkillStateResponse16V4* response){
        using namespace dh2::character::skills;auto& self=*static_cast<SkillHost*>(context);*response={};
        switch(request->operation){
        case skill_state_debug_load_v4:case skill_state_debug_get_v4:case skill_state_debug_destroy_v4:return 0;
        case skill_state_debug_construct_v4:response->identity=1;return 0;
        case skill_state_constant_v4:response->word=0;return 0;
        case skill_state_state_event_v4:case skill_state_transition_v4:
            if(request->value!=50005)return -1;self.state.current=6;self.using_skill=true;return 0;
        case skill_state_current_v4:response->word=static_cast<std::uint32_t>(self.state.current);return 0;
        case skill_state_step_index_v4:response->word=0;return 0;
        case skill_state_step_count_v4:response->word=1;return 0;
        case skill_state_monster_v4:case skill_state_miniboss_v4:case skill_state_boss_v4:response->word=0;return 0;
        default:return 0;
        }
    }
};
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
    check(argc==2,"Supply original shared asset root");
    AssetCatalog assets(argv[1]);OriginalPropertyDatabase database;OriginalMeleeBindings bindings;std::string error;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);

    const auto read=[&](const char* name){auto value=assets.read(std::string("original-cache/data/pydata/")+name);return value;};
    const auto skillRecords=read("skills_pyarray.bin"),skillNames=read("skills_pyarraynames.bin"),skillSchema=read("skills_pystructnames.bin");
    dh2::data::SkillTables skillOwner;
    check(skillOwner.load({skillRecords.data(),skillRecords.size()},{skillNames.data(),skillNames.size()},
                          {skillSchema.data(),skillSchema.size()},error),"Load original SkillTable: "+error);
    const auto skillTables=skillOwner.borrow();
    int bashRow=-1,jumpKickRow=-1;
    for(std::size_t i=0;i<skillTables.skills().size();++i){
        const auto& row=skillTables.skills()[i];
        if(row.script.find("prince_warrior_bashdown")!=std::string::npos)bashRow=static_cast<int>(i);
        if(row.script.find("prince_rogue_jump_kick")!=std::string::npos)jumpKickRow=static_cast<int>(i);
    }
    check(bashRow==7,"Knight SkillTable row7 is no longer the actual BashDown script");
    check(jumpKickRow>=0,"Original Rogue JumpKick SkillTable row missing");

    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";
    customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,
          "target-retention-regression",plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.mainItemId="Longsword01";config.equippedItemIds={"Longsword01"};
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
    placed.definition.stableId=2;placed.definition.sourceId="target-retention-regression-lizard";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    CharacterVisual visual;CombatSession session;
    check(session.initialize(assets,database,bindings,config,visual,population,{0,0,0},customization,error),error);

    std::uintptr_t sourceInterestObject=invalid_actor_id;
    std::int32_t sourceInterestType=-1;
    std::int16_t sourceInterestDelay=0;
    std::uint8_t sourceInterestBlocked=0,sourceInterestChanged=0,sourceInterestDisabled=0;
    auto sourceInterestLease=std::make_shared<int>(7);
    dh2::character::CharacterInterestFieldsV106 sourceInterestFields{
        1,&sourceInterestObject,&sourceInterestType,&sourceInterestDelay,
        &sourceInterestBlocked,&sourceInterestChanged};
    unsigned sourceInterestSearches=0;
    dh2::character::CharacterInterestServicesV106 sourceInterestServices;
    sourceInterestServices.owner=sourceInterestLease;
    sourceInterestServices.interactive=[&](std::uintptr_t candidate,std::uintptr_t owner,bool& value,std::string&){
        const auto* candidateActor=session.actor(static_cast<ActorId>(candidate));
        value=owner==1&&candidateActor&&session.actor(candidateActor->id)==candidateActor&&candidateActor->alive();
        return true;
    };
    sourceInterestServices.disabled=[&](std::uintptr_t candidate,const std::uint8_t*& value,
        std::shared_ptr<void>& pin,std::string&){
        if(!session.actor(static_cast<ActorId>(candidate)))return false;
        value=&sourceInterestDisabled;pin=sourceInterestLease;return true;
    };
    sourceInterestServices.interaction_type=[&](std::uintptr_t candidate,std::uintptr_t owner,
        std::int32_t& value,std::string&){
        const auto* actor=session.actor(static_cast<ActorId>(candidate));
        const auto* traits=session.world()->traits(static_cast<ActorId>(candidate));
        const auto* properties=session.world()->combat_properties(static_cast<ActorId>(candidate));
        if(!actor||!traits||!properties||owner!=session.player_id())return false;
        const auto* ai=dh2::data::ai_props(session.world()->factions(),properties->sheets.resolved[1]);
        if(!actor->alive())value=-1;
        else if(ai&&ai->type==4)value=8;
        else value=3;
        return true;
    };
    sourceInterestServices.search=[&](std::uintptr_t owner,
        std::vector<dh2::character::CharacterInterestCandidateV106>& candidates,std::string&){
        ++sourceInterestSearches;
        const auto* ownerActor=session.actor(static_cast<ActorId>(owner));
        if(!ownerActor)return false;
        for(const auto& pair:session.world()->actors())
            if(pair.first!=owner&&session.world()->eligible_target(*ownerActor,pair.second))
                candidates.push_back({pair.first,1u});
        return true;
    };
    sourceInterestServices.item_owner3bc=[](std::uintptr_t,std::uintptr_t& owner,std::string&){
        owner=invalid_actor_id;return true;
    };
    bool sourceInterestCandidateIsCharacter=true;
    const auto resolveSourceCharacter=[&](std::uintptr_t object,ActorId& actor,bool& isCharacter,std::string&){
        actor=static_cast<ActorId>(object);isCharacter=session.actor(actor)!=nullptr&&
            (object==1||sourceInterestCandidateIsCharacter);return session.actor(actor)!=nullptr;
    };
    SessionSourceObjectInterestStatusV1 sourceInterestStatus{};

    InputActions input;input.targetSelect=true;
    check(session.update(0,input,{0,0,0},0,error),error);
    check(session.actor(1)->target_id==2&&session.selectedactor()&&session.selectedactor()->id==2,
          "Target selection did not publish the actual Lizard ActorId");
    auto targetState=session.target_presentation_state(1);
    check(targetState.current==2&&targetState.last_target_known&&targetState.last_target==2&&
          !targetState.object_of_interest_known,
          "Explicit selection did not publish current and source last-target independently with OOI unknown");
    auto marker=resolve_auto_target_marker_v1(1,
        targetState.last_target_known?targetState.last_target:invalid_actor_id,
        targetState.object_of_interest_known?targetState.object_of_interest:invalid_actor_id);
    check(marker.identity==2&&marker.source==AutoTargetMarkerSourceV1::last_target,
          "Marker did not resolve from genuine Session last-target state");
    check(update_session_source_object_interest_v1(session,1,0,sourceInterestFields,
          sourceInterestServices,resolveSourceCharacter,sourceInterestStatus,error),error);
    targetState=session.target_presentation_state(1);
    check(sourceInterestSearches==1&&sourceInterestStatus==SessionSourceObjectInterestStatusV1::published&&
          targetState.object_of_interest_known&&targetState.object_of_interest==2&&
          targetState.object_of_interest_type==8,
          "Source OOI update did not publish the same-session TargetList Character and signed interaction type");
    check(session.set_source_target(1,invalid_actor_id,true,error),error);
    targetState=session.target_presentation_state(1);
    check(targetState.current==invalid_actor_id&&targetState.last_target_known&&targetState.last_target==2,
          "SetTarget(true) did not change current target independently of last-target");
    check(session.set_source_target(1,2,true,error),error);
    targetState=session.target_presentation_state(1);
    check(targetState.current==2&&targetState.last_target_known&&targetState.last_target==2,
          "Restoring current target through SetTarget(true) rewrote source last-target");
    ActorId reselected=invalid_actor_id;
    check(session.select_next_player_target(reselected,error),error);
    check(reselected==2&&session.selectedactor()&&session.selectedactor()->id==2,
          "Explicit target-selection command did not restore the host selection after direct source setter exercise");
    const auto selected=session.selectedactor()->id;

    input.targetSelect=false;input.attack=true;
    check(session.update(0,input,{0,0,0},0,error),error);
    check(session.owns_pose(1),"Held Space did not admit the source attack sequence");
    input.attack=false;
    for(unsigned frame=0;frame<1000&&session.owns_pose(1);++frame)
        check(session.update(.016,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
    check(!session.owns_pose(1),"Releasing Space did not let the admitted attack finish");
    check(session.actor(1)->target_id==selected&&session.selectedactor()&&session.selectedactor()->id==selected,
          "Space key-up or source attack completion cleared a living selected target");
    targetState=session.target_presentation_state(1);
    check(targetState.current==selected&&targetState.last_target_known&&targetState.last_target==selected,
          "Released attack lost current or source last-target projection");
    input.attack=true;
    check(session.update(0,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
    check(session.owns_pose(1)&&session.actor(1)->target_id==selected&&session.selectedactor()&&
          session.selectedactor()->id==selected,
          "Second Space press between attacks lost its valid selected target");
    input.attack=false;
    for(unsigned frame=0;frame<1000&&session.owns_pose(1);++frame)
        check(session.update(.016,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
    check(!session.owns_pose(1)&&session.actor(1)->target_id==selected&&session.selectedactor()&&
          session.selectedactor()->id==selected,
          "Second released attack did not finish with the same living target selected");
    targetState=session.target_presentation_state(1);
    check(targetState.current==selected&&targetState.last_target_known&&targetState.last_target==selected,
          "Second released attack lost source target fields");
    CameraPose camera;camera.position={0,0,10};camera.target={0,0,0};camera.up={0,1,0};
    const auto* selectedTarget=session.selectedactor();
    check(selectedTarget&&selectedTarget->alive()&&!project_visible(camera,
          {selectedTarget->transform.position[0],selectedTarget->transform.position[1],selectedTarget->transform.position[2]},16.f/9.f),
          "Target selection and offscreen HUD projection were not independently observable");

    // Run the actual source SkillTable row through its native callback/state
    // adapter on the same Session. The Lua callback provider is a narrow fixture
    // that publishes the authored BashDown Post ClearTarget to the Session actor.
    using namespace dh2::character::skills;
    SkillHost skillHost;skillHost.session=&session;
    const auto& bashRecord=skillTables.skills()[static_cast<std::size_t>(bashRow)];
    Instance32 instance{1,bashRecord.script.c_str(),0,0,0,0};const Instance32* instances[]{&instance};
    State40 slots{1,{instances,1,0},{}};SkillAIOwnerV3 owner{1,0,0};SkillAIStateV3 fields{};
    SkillAIContextV3 ai{&owner,&slots,&fields,7,0};SkillStateV4 skillState{&skillHost.state,1,0,0,0,0,0,0,{}};
    dh2::foundation::skills_animation::SourceSkillAnimation sourceSkill(skillTables,ai,skillState,
        [bashRow](std::uint32_t slot,std::int32_t& row){if(slot!=0)return false;row=bashRow;return true;},
        {&skillHost,SkillHost::ai},{&skillHost,SkillHost::state_service});
    std::uint32_t skillAnswer=0;
    check(sourceSkill.command(skill_ai_begin_v3,0,skillAnswer)==0&&skillAnswer==1,
          "Actual BashDown row did not pass the native skill begin path");
    check(sourceSkill.state_operation(skill_state_focus_v4,0,0)==0,
          "Actual BashDown Focus did not complete through the same skill state owner");
    check(sourceSkill.command(skill_ai_focus_v3,0,skillAnswer)==0&&skillHost.pres==1,
          "Actual BashDown Pre callback was not routed once");
    check(sourceSkill.authored_event("do_skill")==0&&skillHost.uses==1,
          "Actual BashDown do_skill marker did not reach Use once");
    check(sourceSkill.command(skill_ai_blur_v3,0,skillAnswer)==0&&skillHost.posts==1&&
          session.actor(1)->target_id==invalid_actor_id,
          "Actual BashDown Post ClearTarget was not published to the same Session");
    check(update_session_source_object_interest_v1(session,1,100,sourceInterestFields,
          sourceInterestServices,resolveSourceCharacter,sourceInterestStatus,error),error);
    targetState=session.target_presentation_state(1);
    marker=resolve_auto_target_marker_v1(1,targetState.last_target_known?targetState.last_target:invalid_actor_id,
        targetState.object_of_interest_known?targetState.object_of_interest:invalid_actor_id);
    check(targetState.current==invalid_actor_id&&targetState.last_target_known&&
          targetState.last_target==invalid_actor_id&&targetState.object_of_interest_known&&
          targetState.object_of_interest==2&&marker.identity==2&&
          marker.source==AutoTargetMarkerSourceV1::object_of_interest,
          "BashDown Post did not clear current/last independently while the retained source OOI remained the marker fallback");
    check(sourceInterestSearches==1,
          "BashDown Post incorrectly forced an OOI TargetList refresh before its source 500 ms timer");
    input={};
    check(session.update(0,input,{0,0,0},0,error),error);
    sourceInterestCandidateIsCharacter=false;
    check(update_session_source_object_interest_v1(session,1,100,sourceInterestFields,
          sourceInterestServices,resolveSourceCharacter,sourceInterestStatus,error),error);
    targetState=session.target_presentation_state(1);
    check(sourceInterestStatus==SessionSourceObjectInterestStatusV1::source_object_not_a_session_actor&&
          !targetState.object_of_interest_known,
          "Unrepresented non-Character source OOI was aliased into the Session Character marker projection");
    sourceInterestCandidateIsCharacter=true;
    check(update_session_source_object_interest_v1(session,1,300,sourceInterestFields,
          sourceInterestServices,resolveSourceCharacter,sourceInterestStatus,error),error);
    targetState=session.target_presentation_state(1);
    check(sourceInterestSearches==2&&targetState.object_of_interest_known&&
          targetState.object_of_interest==2&&sourceInterestChanged==0,
          "Source OOI 500 ms timer did not requery the live same-session target list without a false changed latch");
    check(session.actor(1)->target_id==invalid_actor_id&&!session.selectedactor(),
          "Explicit source-skill ClearTarget was resurrected by stale selection retention");
    // Keep the authored target alive through the release/requery branch; the
    // distinct death branch below then tests source OOI invalidation explicitly.
    session.actor(2)->health=session.actor(2)->max_health;
    input.attack=true;
    check(session.update(0,input,{0,0,0},0,error),error);
    check(session.owns_pose(1),"Next Space attack did not enter the source attack path after skill Post");
    check(session.actor(1)->target_id==selected&&session.selectedactor()&&session.selectedactor()->id==selected,
          "Next source attack did not reacquire the living in-range target after skill Post");
    check(update_session_source_object_interest_v1(session,1,100,sourceInterestFields,
          sourceInterestServices,resolveSourceCharacter,sourceInterestStatus,error),error);
    targetState=session.target_presentation_state(1);
    marker=resolve_auto_target_marker_v1(1,targetState.last_target_known?targetState.last_target:invalid_actor_id,
        targetState.object_of_interest_known?targetState.object_of_interest:invalid_actor_id);
    check(targetState.current==selected&&targetState.last_target_known&&targetState.last_target==selected&&marker.identity==selected,
          "Post-clear reacquisition did not publish fresh last-target marker candidate");

    // This attack is implicit: the target was cleared by BashDown Post, no Tab
    // or targetSelect event follows, and Space must use source target search.
    input.attack=false;
    for(unsigned frame=0;frame<1000&&session.owns_pose(1);++frame)
        check(session.update(.016,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
    check(!session.owns_pose(1),"Implicit post-skill Space attack did not complete after release");
    check(update_session_source_object_interest_v1(session,1,500,sourceInterestFields,
          sourceInterestServices,resolveSourceCharacter,sourceInterestStatus,error),error);
    targetState=session.target_presentation_state(1);
    marker=resolve_auto_target_marker_v1(1,targetState.last_target_known?targetState.last_target:invalid_actor_id,
        targetState.object_of_interest_known?targetState.object_of_interest:invalid_actor_id);
    check(targetState.current==invalid_actor_id&&targetState.last_target_known&&
          targetState.last_target==invalid_actor_id&&targetState.object_of_interest_known&&
          targetState.object_of_interest==selected&&marker.identity==selected&&
          marker.source==AutoTargetMarkerSourceV1::object_of_interest,
          "Implicit attack release lost source last-target/OOI candidate state: current="+
          std::to_string(targetState.current)+" last="+std::to_string(targetState.last_target)+
          " lastKnown="+std::to_string(targetState.last_target_known)+" ooiKnown="+
          std::to_string(targetState.object_of_interest_known)+" ooi="+
          std::to_string(targetState.object_of_interest)+" searches="+std::to_string(sourceInterestSearches)+
          " marker="+std::to_string(marker.identity));

    // Re-establish a genuine source SetTarget before killing the actor so the
    // current-target invalidation and unfiltered last-target marker are distinct.
    check(session.set_source_target(1,selected,false,error),error);
    session.actor(2)->health=0;input={};
    check(session.update(0,input,{0,0,0},0,error),error);
    check(update_session_source_object_interest_v1(session,1,500,sourceInterestFields,
          sourceInterestServices,resolveSourceCharacter,sourceInterestStatus,error),error);
    check(session.actor(1)->target_id==invalid_actor_id&&!session.selectedactor(),
          "Dead target was retained after ordinary invalid-target housekeeping");
    targetState=session.target_presentation_state(1);
    marker=resolve_auto_target_marker_v1(1,targetState.last_target_known?targetState.last_target:invalid_actor_id,
        targetState.object_of_interest_known?targetState.object_of_interest:invalid_actor_id);
    check(targetState.current==invalid_actor_id&&targetState.last_target_known&&targetState.last_target==selected&&
          targetState.object_of_interest_known&&targetState.object_of_interest==invalid_actor_id&&
          marker.identity==selected&&marker.source==AutoTargetMarkerSourceV1::last_target,
          "Target death did not clear OOI on source requery while preserving unfiltered source last-target marker precedence");

    std::cout<<"PASS source target/OOI retention across Space release; actual BashDown Pre/Use/Post cleared current/last while independent OOI survived, decayed/requeried at 500 ms, rejected unrepresented non-Character identity, and cleared on target death; next attack reacquired living target (BashDown row "
             <<bashRow<<", Rogue JumpKick row "<<jumpKickRow<<")\n";
    return 0;
}catch(const std::exception& failure){std::cerr<<failure.what()<<'\n';return 1;}}
