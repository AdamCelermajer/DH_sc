#include "../../combat_session.hpp"
#include "../../game_save.hpp"
#include <iostream>
#include <algorithm>
#include <set>
#include <stdexcept>
using namespace dh::foundation;
void check(bool condition,const std::string& error){if(!condition)throw std::runtime_error(error);}
int main(int argc,char** argv){try{
    check(argc==2,"Supply repository root");const auto repository=std::filesystem::path(argv[1]);
    AssetCatalog assets(repository/".local-inputs/windows-shared-assets"),metadata(repository/".local-inputs/windows-melee-bindings");
    OriginalPropertyDatabase database;OriginalMeleeBindings bindings;std::string error;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);check(bindings.load(metadata,"original-melee-bindings.xml",error),error);
    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"host-bank",plan,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=plan.config;config.playerVisualConfig.motion_node_id="auto";config.playerVisualConfig.consume_root_motion=true;
    CombatSessionProfile player;player.initialIdle={"Idle",0,{0}};player.damageMarkerNames={"attack_mainhand"};player.propertyOptions={256,true};
    OriginalAttackSelection selection;selection.state="AttackStatic";player.sequenceAction=selection;player.retainedPhaseClock=true;player.sourceCombo=true;
    config.profiles["KnightPlayerBase"]=player;
    CombatSessionProfile target;target.action={"Attack",0,{0,1}};target.initialIdle={"Idle",0,{0}};target.damageMarkerNames={"attack_mainhand"};target.propertyOptions={std::nullopt,true};target.customization.allow_missing_animation_targets=true;
    config.profiles["Swamp_LizadMan_Type1"]=target;
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";placed.definition.stableId=2;placed.definition.sourceId="explicit-source-combo-test";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    CharacterVisual visual;CombatSession session;
    const auto run=[&](int mode,std::size_t expected){
        check(session.initialize(assets,database,bindings,config,visual,population,{0,0,0},customization,error),error);
        std::vector<CombatSessionStepEntry> steps;
        session.set_step_entry_observer([&](const CombatSessionStepEntry& event){
            check(!event.binding_lease.expired(),"Step observer lost actual session binding");
            check(event.actor==1&&event.update_serial==session.update_serial(),"Step entry lost actor/frame identity");
            check(event.leaf_path.size()>event.depth&&event.leaf_path[event.depth]==event.step,"Step entry hierarchy identity changed");
            check(steps.empty()||event.occurrence>steps.back().occurrence,"Repeated step entry occurrence identity");
            steps.push_back(event);
            if(mode==3)throw std::runtime_error("expected presentation failure");
        });
        bool block=false;session.set_diagnostic_controller_admission_provider([&](ActorId id,DiagnosticControllerAdmissionFacts& facts,std::string&){facts={id,id,unsigned(block),0,0,0};return true;},
            [](ActorId,bool& online,std::string&){online=false;return true;});
        const RetainedFrameAudioClock pairedClock{9,123,456,true};
        InputActions input;input.attack=true;check(session.update(0,input,{0,0,0},0,error,&pairedClock),error);check(session.owns_pose(1),"Live source command did not begin combo");
        check(!steps.empty()&&steps.front().audio_clock&&steps.front().audio_clock->output_generation==9&&steps.front().audio_clock->device_samples==123&&steps.front().audio_clock->qpc_monotonic_ns==456,"Step entry did not copy caller-paired clock");
        check(!session.combo_boundaries().empty(),"Live source hierarchy trace unavailable");
        const auto generation=session.combo_boundaries().front().generation;
        std::set<std::uint32_t> rootSteps;
        for(const auto& boundary:session.combo_boundaries())if(boundary.beginning&&boundary.depth==0)rootSteps.insert(boundary.step);
        const float before=session.actor(2)->health;float removed=0;std::vector<int> hitGroups;bool acceptedReleased=false;
        for(unsigned frame=0;frame<1000&&session.owns_pose(1);++frame){
            const auto* ai=session.source_attack_state(1);check(ai,"Same source AI projection missing");
            if(mode==0)input.attack=ai->index<2;
            else if(mode==1)input.attack=false;
            else if(mode==2){if(ai->continued&&ai->index==0)acceptedReleased=true;input.attack=!acceptedReleased;}
            else {block=true;input.attack=true;}
            check(session.update(.016,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
            for(const auto& boundary:session.combo_boundaries()){
                check(boundary.generation==generation,"Redirected combo group created a new action generation");
                if(boundary.beginning&&boundary.depth==0)rootSteps.insert(boundary.step);
            }
            for(const auto& event:session.events())if(event.attacker==1){
                const auto group=session.source_attack_state(1)->index;
                check(std::any_of(steps.begin(),steps.end(),[&](const CombatSessionStepEntry& step){
                    return step.sequence_id==470+group&&step.step==1&&step.update_serial<=session.update_serial();
                }),"Source strike step entry was not observed before its hit marker");
                hitGroups.push_back(group);removed+=event.health_removed;
            }
        }
        check(!session.owns_pose(1),"Live released combo did not depart source action");
        check(hitGroups.size()==expected,"Live combo same-generation marker count differs expected="+std::to_string(expected)+" actual="+std::to_string(hitGroups.size()));
        check(session.actor(2)->health==before-removed,"Live combo original damage receipts differ from shared health");
        check(!steps.empty()&&steps.front().sequence_id==243&&steps.front().step==0,"Original root step entry missing");
        check(mode==3?!session.step_entry_diagnostics().empty():session.step_entry_diagnostics().empty(),"Presentation exceptions were not isolated from gameplay");
        session.clear_step_entry_observer();
        if(expected==3)check(hitGroups==std::vector<int>{0,1,2},"Live held command repeated same swing or deduplicated later groups");
        if(expected==3)check(rootSteps==std::set<std::uint32_t>{0,1,2},"Live source hierarchy trace did not prove three distinct root groups");
        if(mode==2)check(acceptedReleased&&hitGroups==std::vector<int>{0,1},"Release discarded previously accepted source continuation");
        std::cout<<"live combo mode="<<mode<<" hits="<<hitGroups.size()<<" removed="<<removed<<" groups=";for(auto group:hitGroups)std::cout<<group<<',';std::cout<<'\n';
    };
    run(0,3);run(1,1);run(2,2);run(3,1);
    check(session.initialize(assets,database,bindings,config,visual,population,{0,0,0},customization,error),error);
    auto character=make_default_character("combo-save-player","Player","warrior");session.actor(1)->persistent_character_id=character.id;
    InputActions held;held.attack=true;check(session.update(0,held,{0,0,0},0,error),error);
    for(unsigned frame=0;frame<100&&!session.source_attack_state(1)->continued;++frame)
        check(session.update(.016,held,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
    check(session.source_attack_state(1)->continued==1,"Accepted continuation save fixture absent");
    GameSave saved;check(capture_game_save("combo-source-level",1,character,*session.world(),saved,error),error);
    session.detach_for_restore();check(restore_game_save(saved,"combo-source-level",*session.world(),character,error),error);
    check(session.rebind_after_restore(error),error);
    check(session.source_attack_state(1)->continued==0&&session.source_attack_state(1)->target==0&&session.source_attack_state(1)->index==0,"Restore retained unpersisted combo continuation/index/target residue");
    check(session.update(0,held,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
    check(session.owns_pose(1)&&session.source_attack_state(1)->index==0&&session.source_attack_state(1)->continued==0,"Fresh restored combo skipped first authored attack");
    check(session.world()->random_state().seed==saved.random.seed&&session.world()->random_state().calls==saved.random.calls,"Combo restore/fresh pre consumed original combat RNG");
    std::cout<<"PASS combo save accepted continuation resets; fresh restored attack starts root0 without RNG consumption\n";
    std::cout<<"PASS live CombatSession held three varied swings, release one, accepted release two, blocked one; original health receipts and same-generation hits preserved\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
