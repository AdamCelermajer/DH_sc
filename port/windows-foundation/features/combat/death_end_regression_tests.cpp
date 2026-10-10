#include "../../combat_session.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace dh::foundation;

namespace {
void check(bool condition,const std::string& message){if(!condition)throw std::runtime_error(message);}

struct EndTrace {
    std::vector<std::uint32_t> virtual_events;
    std::vector<std::uint32_t> state_events;
    unsigned dead_state_end_calls=0;
};

unsigned count_end(const std::vector<std::uint32_t>& events){
    return static_cast<unsigned>(std::count(events.begin(),events.end(),0x22u));
}

void expect(bool condition,const std::string& message,std::vector<std::string>& failures){
    if(!condition)failures.push_back(message);
}

void bind_end_trace(CombatSession& session,EndTrace& trace){
    CombatSessionAnimationNotificationServices services;
    services.notification=[&trace](ActorId,const dh2::character::AnimationEventRequest& request,
                                   std::int32_t& result,std::string&){
        trace.virtual_events.push_back(request.event);result=0;return true;
    };
    services.state_event=[&session,&trace](ActorId actor,std::uint32_t event,
                                            const RetainedAnimationEvent*,std::string&){
        trace.state_events.push_back(event);
        if(event==0x22){
            const auto* state=session.actor(actor);
            // Observe the same-actor end-event handoff. The actual source
            // Dead::OnEvent body (which detaches physics) remains an external
            // source-state service; this fixture never fabricates that effect.
            if(state&&state->action==CharacterAction::dead)++trace.dead_state_end_calls;
        }
        return true;
    };
    session.set_animation_notification_services(std::move(services));
}

}

int main(int argc,char**argv){try{
    check(argc==2,"Supply original shared asset root");
    AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase database;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan player_plan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,
          "death-end-player",player_plan,error),error);

    CombatSessionConfig config;config.diagnosticRngSeed=17;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=player_plan.config;config.playerVisualConfig.motion_node_id="auto";
    config.playerVisualConfig.consume_root_motion=true;
    CombatSessionProfile player;player.action={"AttackStatic",0,{0,1}};player.initialIdle={"Idle",0,{0}};
    player.damageMarkerNames={"attack_mainhand"};player.customization=customization;player.propertyOptions={256,true};
    config.profiles.emplace(config.playerProfileId,player);
    CombatSessionProfile lizard;lizard.action={"Attack",0,{0,1}};lizard.initialIdle={"Idle",0,{0}};
    lizard.reaction=CombatSessionChoice{"Injured",0,{0}};lizard.death=CombatSessionChoice{"Died",0,{0}};
    lizard.damageMarkerNames={"attack_mainhand"};lizard.customization=customization;
    lizard.motionRoot="auto";lizard.propertyOptions={std::nullopt,true};lizard.retainedPhaseClock=true;
    OriginalAttackSelection attack;attack.state="Attack";attack.group_path={0};lizard.sequenceAction=attack;
    config.profiles.emplace("Swamp_LizadMan_Type1",lizard);
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="death-end-lizard";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,17,1};
    placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    CharacterVisual player_visual;CombatSession session;
    check(session.initialize(assets,database,bindings,config,player_visual,population,
          {0,0,0},customization,error),error);
    std::vector<std::string> failures;
    OriginalMeleeDamageProvider probe;
    check(probe.bind_actor(1,*session.world()->combat_properties(1),error)&&
          probe.bind_actor(2,*session.world()->combat_properties(2),error),error);
    const auto category=session.world()->combat_properties(1)->facts.main_damage_class;
    std::uint32_t injury_seed=0;
    for(std::uint32_t candidate=1;candidate<50000&&!injury_seed;++candidate){
        dh2::data::CombatRandom rng{candidate,0};OriginalMeleeResolution result;
        check(probe.resolve_result(1,2,0x22aab5u,category,-1,0,rng,result,error),error);
        if((result.original.outcomes&0x10u)&&result.damage>0&&
           result.damage<session.actor(2)->max_health)injury_seed=candidate;
    }
    check(injury_seed!=0,"No actual source nonlethal Injury outcome seed");
    // Reinitialize with the source RNG state which produces an accepted Injure bit.
    config.diagnosticRngSeed=injury_seed;
    check(session.initialize(assets,database,bindings,config,player_visual,population,
          {0,0,0},customization,error),error);
    EndTrace trace;bind_end_trace(session,trace);
    auto* actor=session.actor(2);check(actor&&actor->alive(),"Actual lizard did not bind alive");
    InputActions input;
    CombatSessionSourceHit hit;hit.attacker=1;hit.target=2;hit.binding_lease=session.actor_binding_lease();
    hit.source_id="death-end-real-melee";hit.marker_name="attack_mainhand";hit.mask=0x22aab5u;
    hit.category=category;hit.element=-1;hit.generation=1;
    DamageEvent receipt;check(session.apply_source_result(hit,receipt,error),error);
    check(receipt.applied&&receipt.source_outcomes&&(*receipt.source_outcomes&0x10u)&&
          !receipt.target_died&&actor->action==CharacterAction::hurt,
          "Actual source hit failed to admit nonlethal Injury");
    const auto* visual=session.retained_actor_visual_borrow(2);check(visual,"Lizard visual borrow unavailable");
    const auto* injury_pose=session.retained_actor_pose(2);
    check(injury_pose,"Accepted Injury has no retained pose owner");
    const auto injury_slot=injury_pose->current_slot();
    const auto injury_clip=injury_pose->slots()[injury_slot].clip_id;
    const auto injury_start=injury_pose->slots()[injury_slot].timeline.start_ms;
    const auto injury_end=injury_pose->slots()[injury_slot].timeline.end_ms;
    const auto injury_rate=injury_pose->slots()[injury_slot].timeline.scale;
    check(injury_clip.find("/Injured/")!=std::string::npos&&injury_end>injury_start&&injury_rate>0,
          "Actual source Injured leaf/range/rate was not selected");
    check(!injury_pose->current_ended()&&count_end(trace.virtual_events)==0,
          "Injury End34 fired at admission instead of authored completion");
    bool injury_ended=false;double injury_wall=0;std::int32_t prior_ms=injury_start;
    const double injury_wall_limit=double(injury_end-injury_start)/1000.0/injury_rate+0.5;
    for(unsigned frame=0;frame<1000&&injury_wall<injury_wall_limit;++frame){
        check(session.update(0.016,input,{0,0,0},0,error),error);injury_wall+=0.016;
        const auto* pose=session.retained_actor_pose(2);check(pose,"Injury pose owner lost before completion");
        const auto& current=pose->slots()[pose->current_slot()];
        const bool selected_clip_ended=pose->current_ended();
        const bool released_at_source_end=current.clip_id!=injury_clip&&actor->action==CharacterAction::idle&&
            injury_wall>=double(injury_end-injury_start)/1000.0/injury_rate-0.016;
        if(current.clip_id==injury_clip){
            check(current.timeline.current_ms>=prior_ms,"Injury source clock moved backward");
            prior_ms=current.timeline.current_ms;
        }else check(released_at_source_end,"Injury clip was replaced before its authored completion");
        if(selected_clip_ended||released_at_source_end){injury_ended=true;
    expect(count_end(trace.virtual_events)==1&&count_end(trace.state_events)==1,
           "Accepted nonlethal Injury completion did not dispatch exactly one End34 through the same-actor state path",failures);
    expect(actor->action==CharacterAction::idle,
           "Current runtime did not release the completed hurt pose at current_ended",failures);
            break;
        }
        expect(count_end(trace.virtual_events)==0,"Injury End34 fired before authored clip completion",failures);
    }
    check(injury_ended,"Actual Injured clip did not complete within its authored duration");
    std::cout<<"Injury clip="<<injury_clip<<" authored_ms="<<(injury_end-injury_start)<<" wall_s="<<injury_wall
             <<" End34_virtual="<<count_end(trace.virtual_events)
             <<" End34_state="<<count_end(trace.state_events)<<" DeadHandlerCalls="<<trace.dead_state_end_calls<<'\n';
    const auto injury_end_virtual_count=count_end(trace.virtual_events);
    const auto injury_end_state_count=count_end(trace.state_events);
    expect(trace.dead_state_end_calls==0,
           "Nonlethal Injury End34 was routed through the Dead state handler",failures);
    check(session.update(0.016,input,{0,0,0},0,error),error);
    expect(count_end(trace.virtual_events)==injury_end_virtual_count&&
           count_end(trace.state_events)==injury_end_state_count,
           "Injury End34 replayed after terminal completion",failures);

    // A source DOT-style result is lethal at the actor's current HP; this is a
    // source result application, not a guessed damage amount or timer.
    hit.source_id="death-end-real-dot";hit.marker_name="death_direct";hit.mask=0x20080000u;
    hit.category=-1;hit.element=-1;hit.direct_amount=static_cast<std::int32_t>(std::ceil(actor->health*256.0f));
    ++hit.generation;
    const bool lethal_applied=session.apply_source_result(hit,receipt,error);
    check(lethal_applied&&receipt.target_died&&actor->action==CharacterAction::dead,
          "Actual lethal source result failed to enter Died: applied="+std::to_string(lethal_applied)+
          " died="+std::to_string(receipt.target_died)+" action="+
          std::to_string(static_cast<int>(actor->action))+" health="+std::to_string(actor->health)+
          " err="+error);
    const auto death_hp0_virtual_count=count_end(trace.virtual_events);
    const auto death_hp0_state_count=count_end(trace.state_events);
    expect(death_hp0_virtual_count==injury_end_virtual_count&&
           death_hp0_state_count==injury_end_state_count&&trace.dead_state_end_calls==0,
           "Died End34 was dispatched at HP zero before authored clip sampling",failures);
    visual=session.retained_actor_visual_borrow(2);
    std::int32_t death_start=0,death_end=0;
    const auto* death_pose=session.retained_actor_pose(2);check(death_pose,"Lethal result has no retained death owner");
    const auto death_slot=death_pose->current_slot();
    const auto death_clip=death_pose->slots()[death_slot].clip_id;
    death_start=death_pose->slots()[death_slot].timeline.start_ms;
    death_end=death_pose->slots()[death_slot].timeline.end_ms;
    const auto death_rate=death_pose->slots()[death_slot].timeline.scale;
    check(death_clip.find("/Died/")!=std::string::npos&&death_end>death_start&&death_rate>0,
          "Actual source Died leaf/range/rate was not selected");
    check(!death_pose->current_ended(),"Died was already ended at HP zero");
    const auto death_origin=actor->transform.position;
    unsigned motion_samples=0;double moved_xy=0,applied_x=0,applied_y=0;
    session.set_motion_handler([&](ActorState& current,Vec3 delta,bool move_go,std::string&){
        if(&current!=actor)return false;
        ++motion_samples;
        if(move_go){current.transform.position[0]+=delta.x;current.transform.position[1]+=delta.y;
            moved_xy+=std::hypot(delta.x,delta.y);applied_x+=delta.x;applied_y+=delta.y;}
        return true;
    });
    bool death_ended=false;double death_wall=0;prior_ms=death_start;
    const double death_wall_limit=double(death_end-death_start)/1000.0/death_rate+0.5;
    for(unsigned frame=0;frame<1000&&death_wall<death_wall_limit;++frame){
        check(session.update(0.016,input,{0,0,0},0,error),error);death_wall+=0.016;
        const auto* pose=session.retained_actor_pose(2);check(pose,"Died pose owner lost before completion");
        const auto& current=pose->slots()[pose->current_slot()];
        check(current.clip_id==death_clip,"Died clip changed before authored completion");
        check(current.timeline.current_ms>=prior_ms,"Died source clock moved backward");prior_ms=current.timeline.current_ms;
        if(pose->current_ended()){death_ended=true;break;}
        expect(count_end(trace.virtual_events)==death_hp0_virtual_count&&
               count_end(trace.state_events)==death_hp0_state_count,
               "Died End34 fired before authored clip completion",failures);
    }
    check(death_ended,"Actual Died clip did not complete within its authored duration");
    std::cout<<"Died completed authored_ms="<<(death_end-death_start)<<" wall_s="<<death_wall
             <<" End34_virtual="<<count_end(trace.virtual_events)
             <<" End34_state="<<count_end(trace.state_events)
             <<" motion_samples="<<motion_samples<<" travel_xy="<<moved_xy<<'\n';
    expect(count_end(trace.virtual_events)==death_hp0_virtual_count+1&&
           count_end(trace.state_events)==death_hp0_state_count+1,
           "Died authored completion did not dispatch exactly one additional End34",failures);
    expect(trace.dead_state_end_calls==1,
           "Died End34 did not reach the same-actor Dead state end-event path once",failures);
    check(motion_samples>0&&moved_xy>0,"Died root-motion/pose sampling was not continuous before terminal hold");
    expect(std::abs(actor->transform.position[0]-(death_origin[0]+applied_x))<0.01&&
           std::abs(actor->transform.position[1]-(death_origin[1]+applied_y))<0.01,
           "Died world transform does not equal once-applied authored XY motion",failures);
    const auto corpse_position=actor->transform.position;
    const auto end_virtual_count=count_end(trace.virtual_events);
    const auto end_state_count=count_end(trace.state_events);
    for(unsigned frame=0;frame<4;++frame)check(session.update(0.016,input,{0,0,0},0,error),error);
    expect(actor->transform.position==corpse_position&&
          count_end(trace.virtual_events)==end_virtual_count&&count_end(trace.state_events)==end_state_count,
          "Terminal Died hold replayed End34 or root displacement",failures);
    session.clear_animation_notification_services();
    session.detach_for_restore();check(session.rebind_after_restore(error),error);
    actor=session.actor(2);check(actor,"Restored lizard actor pointer unavailable");
    bind_end_trace(session,trace);
    const auto restore_virtual_count=count_end(trace.virtual_events);
    const auto restore_state_count=count_end(trace.state_events);
    for(unsigned frame=0;frame<20;++frame)check(session.update(0.016,input,{0,0,0},0,error),error);
    expect(actor->transform.position==corpse_position&&
          count_end(trace.virtual_events)==restore_virtual_count&&count_end(trace.state_events)==restore_state_count,
          "Restored corpse replayed End34 or death displacement",failures);
    if(failures.empty()){
    std::cout<<"PASS Injury End34=1/no Dead effect; Died End34=1 at authored end/no replay after restore; actual lizard clips\n";
        return 0;
    }
    std::cout<<"REGRESSION FAILURES="<<failures.size()<<'\n';
    for(const auto& failure:failures)std::cout<<" - "<<failure<<'\n';
    std::cout<<"OBSERVED InjuryEnd="<<injury_end_virtual_count
             <<" End34VirtualTotal="<<count_end(trace.virtual_events)
             <<" End34StateTotal="<<count_end(trace.state_events)
             <<" DeadStateEndCalls="<<trace.dead_state_end_calls
             <<" DiedAtHP0="<<death_hp0_virtual_count-injury_end_virtual_count
             <<" authoredInjuryMs="<<(injury_end-injury_start)<<" authoredDiedMs="<<(death_end-death_start)
             <<" injuryWall="<<injury_wall<<" deathWall="<<death_wall<<"\n";
    return 2;
}catch(const std::exception& failure){std::cerr<<"FAIL: "<<failure.what()<<'\n';return 1;}}
