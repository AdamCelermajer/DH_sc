#include "../combat_session.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
namespace {
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
void run(const char* root,bool retained){
    AssetCatalog assets(root);std::string error;OriginalPropertyDatabase db;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",db,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"pose-step-player",plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=17;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=plan.config;config.mainItemId="Longsword01";config.equippedItemIds={"Longsword01"};
    config.playerVisualConfig.motion_node_id="auto";config.playerVisualConfig.consume_root_motion=true;
    CombatSessionProfile knight;knight.action={"AttackStatic",0,{0,1}};knight.initialIdle={"Idle",0,{0}};
    knight.damageMarkerNames={"attack_mainhand"};knight.customization=customization;knight.propertyOptions={256,true};
    CombatSessionProfile lizard;lizard.action={"Attack",0,{0,1}};lizard.initialIdle={"Idle",0,{0}};
    lizard.reaction=CombatSessionChoice{"Injured",0,{0}};lizard.death=CombatSessionChoice{"Died",0,{0}};
    lizard.damageMarkerNames={"attack_mainhand"};lizard.customization=customization;
    lizard.motionRoot="auto";lizard.propertyOptions={std::nullopt,true};
    if(retained){
        knight.retainedPhaseClock=lizard.retainedPhaseClock=true;
        OriginalAttackSelection attack;attack.state="AttackStatic";attack.variant=0;attack.group_path={0};knight.sequenceAction=attack;
        attack.state="Attack";lizard.sequenceAction=attack;
    }
    config.profiles.emplace("KnightPlayerBase",knight);config.profiles.emplace("Swamp_LizadMan_Type1",lizard);
    ActorPopulation population;PopulationActor actor;actor.profileId="Swamp_LizadMan_Type1";
    actor.definition.stableId=2;actor.definition.sourceId="source-pose-step-enemy";
    actor.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,17,1};actor.transform=actor.definition.placement;
    population.actors().push_back(std::move(actor));CharacterVisual visual;CombatSession session;
    check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
    OriginalMeleeDamageProvider probe;
    check(probe.bind_actor(1,*session.world()->combat_properties(1),error),error);
    check(probe.bind_actor(2,*session.world()->combat_properties(2),error),error);
    const auto category=session.world()->combat_properties(1)->facts.main_damage_class;
    std::uint32_t seed=0;
    for(std::uint32_t candidate=1;candidate<50000&&!seed;++candidate){
        dh2::data::CombatRandom rng{candidate,0};OriginalMeleeResolution result;
        check(probe.resolve_result(1,2,0x22aab5u,category,-1,0,rng,result,error),error);
        if((result.original.outcomes&0x10u)&&result.damage>0&&result.damage<session.actor(2)->max_health)seed=candidate;
    }
    check(seed!=0,"No actual source melee Injury seed");config.diagnosticRngSeed=seed;
    check(session.initialize(assets,db,bindings,config,visual,population,{0,0,0},customization,error),error);
    std::vector<CombatSessionStepEntry> entries;
    session.set_step_entry_observer([&](const CombatSessionStepEntry& event){
        entries.push_back(event);
        if(entries.size()==1)throw std::runtime_error("fixture presentation failure");
    });
    CombatSessionSourceHit hit;hit.attacker=1;hit.target=2;hit.binding_lease=session.actor_binding_lease();
    hit.source_id="actual-source-melee-pose-test";hit.marker_name="attack_mainhand";
    hit.mask=0x22aab5u;hit.category=category;hit.element=-1;hit.generation=1;
    DamageEvent receipt;check(session.apply_source_result(hit,receipt,error),error);
    check(receipt.applied&&receipt.source_outcomes&&(*receipt.source_outcomes&0x10u)&&
          session.actor(2)->action==CharacterAction::hurt,"Source Injury admission was lost");
    check(entries.size()==1&&entries[0].actor==2&&entries[0].sequence_id==377&&entries[0].depth==0&&
          entries[0].step==0&&entries[0].container_path.empty()&&entries[0].leaf_path==std::vector<std::size_t>{0}&&
          !entries[0].binding_lease.expired()&&!entries[0].audio_clock,
          "Actual Injured leaf did not publish its source row/lease or invented an audio clock");
    check(session.step_entry_diagnostics().size()==1,"Presentation exception vetoed/lost its diagnostic");
    const auto after=session.world()->random_state();
    check(session.apply_source_result(hit,receipt,error)&&!receipt.applied&&entries.size()==1,error);
    check(session.world()->random_state().seed==after.seed&&session.world()->random_state().calls==after.calls,
          "Duplicate Injury rerolled");
    // Distinct source hits are still calculated; the shared 3000ms gate must
    // suppress a second pose entry even when another real result has bit0x10.
    bool repeat=false;
    for(unsigned i=0;i<500&&!repeat;++i){
        session.actor(2)->health=session.actor(2)->max_health;++hit.generation;
        check(session.apply_source_result(hit,receipt,error),error);
        repeat=receipt.applied&&receipt.source_outcomes&&(*receipt.source_outcomes&0x10u);
    }
    check(repeat&&entries.size()==1,"Suppressed source Injury replayed the pose entry");
    InputActions input;check(session.update(3.1,input,{0,0,0},0,error),error);
    const auto prior=entries.size();
    // Clock values here are a test input, not a hardware/audibility claim.
    const RetainedFrameAudioClock paired{7,1234,987654321,true};bool emitted=false;
    session.set_actor_decision_provider([&](CombatSession& current,double,std::string& e){
        for(unsigned i=0;i<500&&!emitted;++i){
            current.actor(2)->health=current.actor(2)->max_health;++hit.generation;
            if(!current.apply_source_result(hit,receipt,e))return false;
            emitted=receipt.applied&&receipt.source_outcomes&&(*receipt.source_outcomes&0x10u);
        }
        return emitted;
    });
    check(session.update(0,input,{0,0,0},0,error,&paired),error);session.clear_actor_decision_provider();
    check(emitted&&entries.size()==prior+1&&entries.back().sequence_id==377&&entries.back().audio_clock&&
          entries.back().audio_clock->output_generation==paired.output_generation&&
          entries.back().audio_clock->device_samples==paired.device_samples&&
          entries.back().audio_clock->qpc_monotonic_ns==paired.qpc_monotonic_ns&&
          entries.back().update_serial==session.update_serial()&&entries.back().occurrence>entries.front().occurrence,
          "Expired gate did not publish one Injury with the caller's exact frame clock");
    hit.source_id="source-dot-death-pose-test";hit.mask=0x20080000u;hit.category=-1;++hit.generation;
    hit.direct_amount=static_cast<std::int32_t>(std::ceil(session.actor(2)->health*256));
    check(session.apply_source_result(hit,receipt,error)&&receipt.target_died,error);
    check(entries.size()==prior+2&&entries.back().sequence_id==374&&entries.back().step==0&&
          !entries.back().audio_clock,"Death leaf step entry missing or retained an expired frame clock");
    const auto deadCount=entries.size();
    check(session.update(5,input,{0,0,0},0,error),error);
    check(session.update(0,input,{0,0,0},0,error)&&entries.size()==deadCount,"Death terminal hold replayed step entry");
    session.detach_for_restore();check(session.rebind_after_restore(error),error);
    check(entries.size()==deadCount,"Restore of existing corpse replayed entry presentation");
    check(session.update(0,input,{0,0,0},0,error)&&entries.size()==deadCount,"Post-restore corpse replayed entry");
    std::cout<<"pose-step path="<<(retained?"retained":"direct")<<" source_injury_seed="<<seed
             <<" injury_row=377 death_row=374 entries="<<deadCount<<" restore_entries="<<entries.size()<<'\n';
}
}
int main(int argc,char** argv){try{check(argc==2,"Supply original shared assets");run(argv[1],false);run(argv[1],true);
    std::cout<<"PASS direct and retained same-Session original Injury/death step entries, gate/duplicate suppression, exact borrowed clock, non-vetoing observer, terminal/restore no replay\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
