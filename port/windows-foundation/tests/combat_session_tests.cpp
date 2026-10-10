#include "../combat_session.hpp"
#include "../game_save.hpp"
#include "../retained_pose_playback.hpp"
#include "../original_actor_lifecycle.hpp"
#include "../retained_animation_owner.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
using namespace dh::foundation;
void check(bool condition,const std::string& error){if(!condition)throw std::runtime_error(error);}
int main(int argc,char** argv){try{
    check(argc==2,"Supply original shared asset root");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase database;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    const auto itemData=assets.read(config.tableRoot+"/loot_table_pyarray.bin"),itemNames=assets.read(config.tableRoot+"/loot_table_pyarraynames.bin"),itemFields=assets.read(config.tableRoot+"/loot_table_pystructnames.bin");
    dh2::data::ItemTable items;check(dh2::data::load_items({itemData.data(),itemData.size()},{itemNames.data(),itemNames.size()},{itemFields.data(),itemFields.size()},items,error),error);
    check(items.identifiers.size()>664,"Original equipment test row absent");config.mainItemId=items.identifiers[664];config.equippedItemIds={config.mainItemId};
    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;check(build_original_combat_visual_plan(assets,bindings,config.playerProfileId,customization,"test-player",plan,error),error);
    config.playerVisualConfig=plan.config;config.playerVisualConfig.clips.clear();config.playerVisualConfig.clips={{"idle",plan.phase("Idle",0,{0})->resolvedPath},{"walk",plan.phase("Idle",0,{0})->resolvedPath}};
    config.playerVisualConfig.motion_node_id="auto";config.playerVisualConfig.consume_root_motion=true;
    CombatSessionProfile knight;knight.action={"AttackStatic",0,{0,1}};knight.initialIdle={"Idle",0,{0}};knight.reaction=CombatSessionChoice{"Injured",0,{0}};knight.death=CombatSessionChoice{"Died",0,{0}};
    knight.damageMarkerNames={"attack_mainhand"};knight.propertyOptions={256,true};config.profiles.emplace(config.playerProfileId,knight);
    CombatSessionProfile lizard;lizard.action={"Attack",0,{0,1}};lizard.initialIdle={"Idle",0,{0}};lizard.death=CombatSessionChoice{"Died",0,{0}};lizard.damageMarkerNames={"attack_mainhand"};lizard.propertyOptions={std::nullopt,true};lizard.customization.allow_missing_animation_targets=true;
    config.profiles.emplace("Swamp_LizadMan_Type1",lizard);
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";placed.definition.stableId=2;placed.definition.sourceId="explicit-test-placement";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};placed.transform={2,0,0,0,0,2,0,0,0,0,2,0,0,-100,0,1};population.actors().push_back(std::move(placed));const auto originalPlacement=population.actors().front().transform;
    CharacterVisual player;CombatSession session;
    auto bad=config;bad.diagnosticRngSeed.reset();check(!session.initialize(assets,database,bindings,bad,player,population,{0,0,0},customization,error),"Missing diagnostic seed accepted");
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    {
        auto continued=config;continued.diagnosticRngSeed.reset();
        dh2::data::CombatRandom startup{1234,0};dh2_combat_random(&startup,5);
        continued.initialRandomState=startup;
        check(session.initialize(assets,database,bindings,continued,player,population,{0,0,0},customization,error),error);
        const auto actual=session.world()->random_state();
        check(actual.seed==startup.seed&&actual.calls==startup.calls,"Startup RNG state/calls reset at session handoff");
        continued.diagnosticRngSeed=1234;
        check(!session.initialize(assets,database,bindings,continued,player,population,{0,0,0},customization,error),"Ambiguous RNG authorities accepted");
        check(session.world()->random_state().seed==startup.seed&&session.world()->random_state().calls==startup.calls,"Rejected RNG config changed live world");
        check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    }
    check(session.world()->actors().size()==2&&session.player_id()==1,"Shared source registry missing actors");
    {
        const auto originalTraits=*session.world()->traits(1);
        auto unequippedTraits=originalTraits;unequippedTraits.main_item.reset();
        check(originalTraits.main_item.has_value(),"Equipment regression requires original main weapon");
        check(session.world()->update_combat_properties(1,*session.world()->combat_properties(1),unequippedTraits,error),error);
        InputActions noInput;
        check(session.update(.016,noInput,{0,0,0},0,error),error);
        check(!session.world()->traits(1)->main_item,"Action facts update restored stale unequipped weapon");
        check(session.world()->update_combat_properties(1,*session.world()->combat_properties(1),originalTraits,error),error);
        check(session.update(.016,noInput,{0,0,0},0,error),error);
        check(session.world()->traits(1)->main_item.has_value(),"Action facts update discarded re-equipped weapon");
        std::cout<<"Shared equipment traits survive ordinary action facts updates\n";
    }
    check(session.actor(1)->max_health>0&&session.actor(2)->max_health>0,"Source vitals unbound");
    check(population.actors().front().transform==originalPlacement,"Population visual scale/placement overwritten");
    check(session.owns_population_pose(2)&&!session.owns_pose(1),"Pose ownership incorrect");
    check(player.select("walk",true,error),"Host locomotion alias dropped");
    InputActions input;input.targetSelect=true;check(session.update(0,input,{0,0,0},0,error),error);
    check(session.selectedactor()&&session.selectedactor()->id==2,"Target cycling did not use original hostility");
    input.targetSelect=false;input.attack=true;check(session.update(0,input,{0,0,0},0,error),error);check(session.owns_pose(1),"Source action did not acquire pose");
    input.attack=false;std::size_t markers=0;float removed=0;
    for(unsigned frame=0;frame<120;++frame){check(session.update(1.0/60,input,{0,0,0},0,error),error);for(const auto& event:session.events()){++markers;removed+=event.health_removed;}}
    check(markers>0,"Source attack marker was not dispatched");check(!session.owns_pose(1),"Source action did not release pose");
    check(session.actor(2)->health==session.actor(2)->max_health-removed,"Shared target health differs from applied source events");
    check(session.actor(1)->health==session.actor(1)->max_health,"NPC attacked without diagnostic policy");
    check(!session.update(-1,input,{0,0,0},0,error),"Invalid frame interval accepted");
    std::cout<<"combat session source test passed actors=2 playerHP="<<session.actor(1)->max_health<<" targetHP="<<session.actor(2)->max_health<<" authoredMarkers="<<markers<<" removed="<<removed<<'\n';
    config.profiles.at("Swamp_LizadMan_Type1").diagnosticAIEnabled=true;
    config.profiles.at("Swamp_LizadMan_Type1").requiredAIState="explicit-test-gate";
    population.actors().front().definition.properties["ai_state"]="other-test-gate";
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    check(session.update(0,input,{0,0,0},0,error),error);check(!session.owns_pose(2),"Mismatched authored AI gate enabled action");
    population.actors().front().definition.properties["ai_state"]="explicit-test-gate";
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    check(session.update(0,input,{0,0,0},0,error),error);check(session.owns_pose(2),"Explicit matching diagnostic AI gate did not attack");
    check(session.update(0.15,input,{0,0,0},0,error),error);
    check(!session.events().empty()&&session.events().front().attacker==2,"NPC source attack marker not delivered");
    std::cout<<"diagnostic source AI gate and NPC marker passed\n";
    config.profiles.at("Swamp_LizadMan_Type1").diagnosticAIEnabled=false;
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    unsigned decisionTicks=0;
    session.set_actor_decision_provider([&](CombatSession& same,double dt,std::string& e){
        check(&same==&session&&same.actor(2)==session.world()->find_actor(2),"AI decisions received another session or actor graph");
        ++decisionTicks;return same.request_actor_attack(2,1,dt,e);
    });
    check(!session.request_actor_attack(999999,1,0,error),"Unbound actor attack command accepted");
    check(!session.request_actor_attack(2,1,-1,error),"Invalid AI command interval accepted");
    check(session.update(0,input,{0,0,0},0,error)&&session.owns_pose(2),error);
    check(session.update(.15,input,{0,0,0},0,error),error);
    check(decisionTicks==2&&!session.events().empty()&&session.events().front().attacker==2,
          "Shared gameplay decision provider lost authored NPC attack timing");
    session.clear_actor_decision_provider();
    std::cout<<"Generic actor decisions use same session attack admission and original NPC marker timing\n";
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    session.actor(2)->transform.position={100,0,0};input.attack=true;
    check(session.update(0.01,input,{0,0,0},0,error),error);
    check(session.actor(1)->transform.rotation[2]>0&&session.actor(1)->transform.rotation[2]<1.5707964f,"Attack target turn snapped or used reversed heading");
    check(session.actor(1)->transform.position==std::array<float,3>{0,0,0},"Target turn moved player before attack");
    std::cout<<"bounded original target turn passed angle="<<session.actor(1)->transform.rotation[2]<<'\n';
    auto character=make_default_character("restore-player","Player","warrior");
    session.actor(1)->persistent_character_id=character.id;
    apply_actor_damage(*session.actor(2),session.actor(2)->health);
    GameSave captured;check(capture_game_save("source-test-level",1,character,*session.world(),captured,error),error);
    const auto savePath=std::filesystem::temp_directory_path()/"dh-combat-session-source-test.save";
    check(save_game(savePath,captured,error),error);GameSave loaded;check(load_game(savePath,loaded,error),error);
    const auto expectedRandom=loaded.random;
    session.detach_for_restore();check(!session.owns_pose(1)&&session.events().empty(),"Restore detach retained old action/event bindings");
    check(!session.update(0,input,{0,0,0},0,error),"Detached session accepted updates");
    check(restore_game_save(loaded,"source-test-level",*session.world(),character,error),error);
    check(session.rebind_after_restore(error),error);
    check(session.actor(1)->target_id==0&&session.actor(1)->action==CharacterAction::idle&&!session.owns_pose(1),"Restored action/target survived reset");
    check(session.actor(2)->action==CharacterAction::dead&&session.owns_pose(2),"Restored dead actor did not restart configured death pose");
    check(session.world()->random_state().seed==expectedRandom.seed&&session.world()->random_state().calls==expectedRandom.calls,"Restore rebind changed exact RNG");
    session.detach_for_restore();check(!restore_game_save(loaded,"wrong-level",*session.world(),character,error),"Wrong-level restore accepted");
    check(session.rebind_after_restore(error),error);input.attack=false;
    check(session.update(0,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
    std::filesystem::remove(savePath);
    std::cout<<"live save restore and failed-restore safe rebind passed\n";
    AssetCatalog sequenceMetadata(assets.root().parent_path()/"windows-melee-bindings");
    check(bindings.load(sequenceMetadata,"original-melee-bindings.xml",error),error);
    OriginalAttackSelection playerSequence;playerSequence.state="AttackStatic";playerSequence.group_path={0};playerSequence.actor_rate=1;
    OriginalAttackSelection npcSequence;npcSequence.state="Attack";npcSequence.group_path={0};npcSequence.actor_rate=1;
    config.profiles.at("KnightPlayerBase").sequenceAction=playerSequence;
    config.profiles.at("Swamp_LizadMan_Type1").sequenceAction=npcSequence;
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    const auto* fullPlayer=session.attack_sequence(1);const auto* fullNPC=session.attack_sequence(2);
    check(fullPlayer&&fullNPC&&fullPlayer->phases().size()==3&&fullNPC->phases().size()==3,"Full source phase groups were not bound");
    for(ActorId id:{ActorId(1),ActorId(2)}){
        const float originalRate=original_speed_modifier(session.world()->combat_properties(id)->sheets.resolved[48]);
        for(const auto& phase:session.attack_sequence(id)->phases()){
            const float expected=originalRate*static_cast<float>(phase.source.speed);
            check(std::abs(phase.rate-expected)<0.000001,"Source property48 and per-phase rate not composed exactly");
        }
    }
    input={};input.attack=true;check(session.update(0,input,{0,0,0},0,error),error);input.attack=false;
    check(player.animation_name()==fullPlayer->phases()[0].source.resolvedPath,"Action did not begin with original pre phase");
    const auto hit=std::find_if(fullPlayer->scheduled_markers().begin(),fullPlayer->scheduled_markers().end(),[](const auto& marker){return marker.source.name=="attack_mainhand";});
    check(hit!=fullPlayer->scheduled_markers().end(),"Merged fullgroup damage track absent");
    check(session.update(hit->virtual_ms/1000.0-0.001,input,{0,0,0},0,error),error);check(session.events().empty(),"Attack damage preceded authored fullgroup timeline");
    check(session.update(0.002,input,{0,0,0},0,error),error);check(!session.events().empty(),"Merged fullgroup damage marker missing");
    check(player.animation_name()==fullPlayer->phases()[1].source.resolvedPath,"Damage marker did not align with actual strike pose");
    double playerDuration=0;for(const auto& phase:fullPlayer->phases())playerDuration+=phase.wall_duration_seconds;
    check(session.update(playerDuration,input,{0,0,0},0,error),error);check(!session.owns_pose(1)&&fullPlayer->finished(),"Original full recovery did not release action");
    config.profiles.at("Swamp_LizadMan_Type1").diagnosticAIEnabled=true;
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    fullNPC=session.attack_sequence(2);check(session.update(0,input,{0,0,0},0,error),error);
    check(population.actors()[0].visual.animation_name()==fullNPC->phases()[0].source.resolvedPath,"NPC skipped source pre phase");
    double npcDuration=0;for(const auto& phase:fullNPC->phases())npcDuration+=phase.wall_duration_seconds;
    check(session.update(npcDuration+0.001,input,{0,0,0},0,error),error);
    std::size_t npcHits=0;for(const auto& event:session.events())if(event.attacker==2)++npcHits;
    check(npcHits==2&&fullNPC->finished(),"NPC fullgroup lost repeated source attack markers or recovery");
    std::cout<<"full source groups passed playerPhases=3 npcPhases=3 npcAuthoredHits="<<npcHits<<" property48Rates=verified\n";
    session.actor(1)->persistent_character_id=character.id;
    check(capture_game_save("source-test-level",1,character,*session.world(),captured,error),error);
    session.detach_for_restore();check(restore_game_save(captured,"source-test-level",*session.world(),character,error),error);
    check(session.rebind_after_restore(error),error);
    input.attack=true;check(session.update(0,input,{0,0,0},session.actor(1)->transform.rotation[2],error),error);
    fullPlayer=session.attack_sequence(1);fullNPC=session.attack_sequence(2);
    check(session.owns_pose(1)&&session.owns_pose(2)&&player.animation_name()==fullPlayer->phases()[0].source.resolvedPath&&
          population.actors()[0].visual.animation_name()==fullNPC->phases()[0].source.resolvedPath,"Fullgroup callback owners or source cooldown reset failed after restore");
    check(session.world()->random_state().seed==captured.random.seed&&session.world()->random_state().calls==captured.random.calls,"Fullgroup restore rebind consumed RNG");
    std::cout<<"fullgroup save rebind and subsequent action callbacks passed\n";
    unsigned motionSegments=0;
    session.set_motion_handler([&](ActorState& actor,Vec3 delta,bool,std::string&){
        check(session.actor(actor.id)==&actor,"Motion callback borrowed a stale restored actor");
        check(std::isfinite(delta.x)&&std::isfinite(delta.y)&&std::isfinite(delta.z),"Nonfinite motion delivered");
        ++motionSegments;return true;
    });
    check(session.update(.1,input,{0,0,0},0,error)&&motionSegments>0,error.empty()?"Restored phase callbacks lost motion host":error);
    check(capture_game_save("source-test-level",1,character,*session.world(),captured,error),error);
    session.detach_for_restore();check(restore_game_save(captured,"source-test-level",*session.world(),character,error)&&session.rebind_after_restore(error),error);
    motionSegments=0;check(session.update(0,input,{0,0,0},0,error)&&session.update(.1,input,{0,0,0},0,error),error);
    check(motionSegments>0,"Save rebind failed to retain motion service");
    config.profiles.at("Swamp_LizadMan_Type1").motionRoot="auto";
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    check(std::string(population.actors()[0].visual.root_motion_node_id()).size()>0,"Explicit NPC source root not bound");
    unsigned npcMotionSegments=0;double npcMotionLength=0;
    session.set_motion_handler([&](ActorState& actor,Vec3 delta,bool,std::string&){if(actor.id==2){++npcMotionSegments;npcMotionLength+=std::hypot(delta.x,delta.y);}return true;});
    input={};check(session.update(0,input,{0,0,0},0,error)&&session.update(npcDuration+.001,input,{0,0,0},0,error),error);
    check(npcMotionSegments>0,"NPC source phases bypassed shared motion service");
    std::cout<<"NPC original root segments="<<npcMotionSegments<<" localXY="<<npcMotionLength<<'\n';
    config.profiles.at(config.playerProfileId).retainedPhaseClock=true;
    config.profiles.at("Swamp_LizadMan_Type1").retainedPhaseClock=true;
    config.profiles.at("Swamp_LizadMan_Type1").diagnosticAIEnabled=false;
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    std::vector<std::string> sourceFrameOrder;
    check(session.bind_actor_locomotion(2,"enemy-idle",config.profiles.at("Swamp_LizadMan_Type1").initialIdle,1,true,error),error);
    check(session.select_actor_locomotion(2,"enemy-idle",error),error);
    const auto* enemyPose=session.retained_actor_pose(2);check(enemyPose,"Enemy locomotion has no shared retained pose");
    const auto enemyClock=enemyPose->slots()[enemyPose->current_slot()].timeline.current_ms;
    input={};check(session.update(.016,input,{0,0,0},0,error)&&session.update(.016,input,{0,0,0},0,error),error);
    check(enemyPose==session.retained_actor_pose(2)&&enemyPose->slots()[enemyPose->current_slot()].timeline.current_ms!=enemyClock,
          "Enemy locomotion was reset to idle or advanced outside shared session");
    unsigned retainedSegments=0;session.set_motion_handler([&](ActorState& a,Vec3,bool,std::string&){++retainedSegments;if(a.id==1)sourceFrameOrder.push_back("motion");return true;});
    unsigned synchronousHitForwards=0;
    const auto healthBeforeSourceHit=session.actor(2)->health;
    CombatSessionAnimationNotificationServices hitForward;
    hitForward.state_event=[&](ActorId id,std::uint32_t event,const RetainedAnimationEvent* named,std::string&){
        if(id==1&&event==0x28&&named&&named->name=="attack_mainhand") {
            check(session.actor(2)->health<healthBeforeSourceHit,"Named state forwarding observed health before source hit application");
            check(!session.events().empty()&&session.events().back().attacker==1,"Source hit receipt was unavailable inside state forwarding");
            const auto& pending=session.world()->pending_resolutions();
            check(!pending.empty()&&pending.back().attacker==1&&pending.back().marker_name==named->name,
                  "Exact hit resolution unavailable before retained audio/motion boundary");
            ++synchronousHitForwards;
            sourceFrameOrder.push_back("hit");
        }
        return true;
    };
    session.set_animation_notification_services(hitForward);
    input={};input.attack=true;check(session.update(0,input,{0,0,0},0,error),error);input.attack=false;
    unsigned firstSourceHit=0;bool closed=false;
    for(unsigned frame=1;frame<200;++frame){sourceFrameOrder.clear();check(session.update(.016,input,{0,0,0},0,error),error);
        const auto hit=std::find(sourceFrameOrder.begin(),sourceFrameOrder.end(),"hit");
        if(hit!=sourceFrameOrder.end())check(std::next(hit)!=sourceFrameOrder.end()&&*std::next(hit)=="motion","Source root displacement preceded its hit/state callback");
        if(!session.events().empty()&&!firstSourceHit)firstSourceHit=frame;if(!session.owns_pose(1)){closed=true;break;}}
    check(firstSourceHit&&closed&&retainedSegments>0,"Retained source attack did not emit damage, motion or actual completion");
    check(synchronousHitForwards>0,"No authored hit was checked inside actual source state forwarding");
    session.clear_animation_notification_services();
    check(population.actors()[0].visual.animation_name()!=nullptr,"Retained victim pose unavailable");
    session.actor(1)->persistent_character_id=character.id;
    check(capture_game_save("source-test-level",1,character,*session.world(),captured,error),error);
    session.detach_for_restore();check(restore_game_save(captured,"source-test-level",*session.world(),character,error)&&session.rebind_after_restore(error),error);
    input.attack=true;check(session.update(0,input,{0,0,0},0,error),error);input.attack=false;bool restoredHit=false;
    for(unsigned frame=0;frame<100&&!restoredHit;++frame){check(session.update(.016,input,{0,0,0},0,error),error);restoredHit=!session.events().empty();}
    check(restoredHit,"Retained owners or event services lost after save rebind");
    std::cout<<"Retained source combat passed firstHitFrame="<<firstSourceHit<<" motionSegments="<<retainedSegments<<" saveRebind=verified\n";
    session.actor(2)->health=0;const auto savedCorpsePosition=session.actor(2)->transform.position;
    check(capture_game_save("source-test-level",1,character,*session.world(),captured,error),error);
    session.detach_for_restore();check(restore_game_save(captured,"source-test-level",*session.world(),character,error)&&session.rebind_after_restore(error),error);
    input={};for(unsigned frame=0;frame<100;++frame)check(session.update(.016,input,{0,0,0},0,error),error);
    check(session.actor(2)->transform.position==savedCorpsePosition&&session.owns_pose(2),"Restored source death replay moved saved corpse");
    std::cout<<"Retained saved corpse position preserved with terminal source pose\n";
    session.actor(2)->health=session.actor(2)->max_health;session.actor(2)->action=CharacterAction::idle;
    session.actor(2)->transform.position={10000,0,0};session.actor(1)->target_id=invalid_actor_id;input.attack=true;
    check(session.update(0,input,{0,0,0},0,error)&&session.actor(1)->target_id==invalid_actor_id,"Basic melee auto-target selected an actor beyond original melee reach");
    check(!session.uses_retained_player_locomotion()&&!session.has_player_locomotion("walk"),"Unbound locomotion aliases were inferred");
    check(!session.bind_player_locomotion("run",{"MissingSourceState",0,{0}},1,true,error),"Unknown run source was guessed");
    check(!session.bind_player_locomotion("fabricated-loop",{"Died",0,{0}},1,true,error),"Finite source state was fabricated into infinite locomotion");
    check(session.bind_player_locomotion("idle",config.profiles.at(config.playerProfileId).initialIdle,1,true,error),error);
    check(session.bind_player_locomotion("walk",{"Walk",0,{0}},.75,true,error),error);
    {
        // Caller logical names need not exist in the original exported actor
        // bank. The actual Idle phase/policy and preloaded resource stay exact.
        OriginalCombatVisualPlan external;
        auto idle=*plan.sequence("Idle",0);idle.state="ExternalSourceIdle";
        idle.phases.at(0).clipName="idle";
        external.sequences.push_back(idle);
        check(session.bind_actor_locomotion_from_bank(1,"external-idle",external,{"ExternalSourceIdle",0,{0}},1,true,error),error);
        auto unavailable=external;unavailable.sequences.at(0).phases.at(0).clipName="unloaded-source-clip";
        check(!session.bind_actor_locomotion_from_bank(1,"external-idle",unavailable,{"ExternalSourceIdle",0,{0}},1,true,error),"External bank accepted an unloaded source clip");
    }
    check(session.select_player_locomotion("external-idle",error),error);
    input={};check(session.update(.016,input,{0,0,0},0,error),error);
    const auto* externalPose=session.retained_player_pose();
    check(externalPose&&externalPose->slots()[externalPose->current_slot()].clip_id=="idle","External bank metadata did not survive caller lifetime or rejected rebind");
    const auto externalClock=externalPose->slots()[externalPose->current_slot()].timeline.current_ms;
    check(session.update(.016,input,{0,0,0},0,error),error);
    check(externalPose==session.retained_player_pose()&&externalPose->slots()[externalPose->current_slot()].timeline.current_ms>externalClock,"External locomotion did not advance on the same session clock");
    check(session.uses_retained_player_locomotion()&&session.has_player_locomotion("idle")&&session.has_player_locomotion("walk")&&!session.has_player_locomotion("run"),"Explicit retained alias bindings unavailable");
    check(!session.select_player_locomotion("run",error),"Unmapped run silently fell back");
    check(session.select_player_locomotion("walk",error),error);input={};input.move2D={0,1};
    double locomotionMotion=0;unsigned locomotionSegments=0;session.set_motion_handler([&](ActorState&a,Vec3 delta,bool enabled,std::string&){if(a.id==1){check(a.action==CharacterAction::moving,"Locomotion motion used attack/idle actor state");++locomotionSegments;if(enabled)locomotionMotion+=std::hypot(delta.x,delta.y);}return true;});
    for(unsigned i=0;i<8;++i){check(session.select_player_locomotion("walk",error)&&session.update(.05,input,{0,0,0},0,error),error);}
    const auto* pose=session.retained_player_pose();check(pose,"Retained player pose diagnostic unavailable");const auto walkSlot=pose->current_slot();const auto generation=pose->slots()[walkSlot].generation;const auto beforeClock=pose->samples().front().wall_timestamp_ms;
    check(std::abs(pose->slots()[walkSlot].timeline.scale-.75f*1.2999999523162842f)<.000001f,"Locomotion caller/source rate multiplication differs");
    check(session.select_player_locomotion("walk",error)&&session.update(.032,input,{0,0,0},0,error),error);pose=session.retained_player_pose();const auto clockDelta=pose->samples().front().wall_timestamp_ms-beforeClock;
    check(pose->current_slot()==walkSlot&&pose->slots()[walkSlot].generation==generation,"Repeated locomotion selection restarted retained slot");check(clockDelta>=31&&clockDelta<=33,"Session locomotion advanced more than once per frame");check(locomotionSegments>0&&locomotionMotion>0,"Authored walk MoveGO bypassed shared motion handler");
    for(unsigned i=0;i<180;++i)check(session.select_player_locomotion("walk",error)&&session.update(.032,input,{0,0,0},0,error),error);pose=session.retained_player_pose();check(pose->slots()[pose->current_slot()].timeline.loop==0&&(pose->slots()[0].generation>generation||pose->slots()[1].generation>generation),"Source locomotion used raw wrap instead of completion/NewAnim repeat");
    session.set_motion_handler([](ActorState&,Vec3,bool,std::string&){return true;});
    check(session.select_player_locomotion("idle",error),error);input={};check(session.update(.05,input,{0,0,0},0,error),error);
    session.actor(2)->transform.position={0,-100,0};session.actor(2)->health=session.actor(2)->max_health;session.actor(2)->action=CharacterAction::idle;
    const auto outgoingSlot=session.retained_player_pose()->current_slot();const auto outgoingClip=session.retained_player_pose()->slots()[outgoingSlot].clip_id;const auto outgoingTime=session.retained_player_pose()->slots()[outgoingSlot].timeline.last_seconds;
    input.attack=true;check(session.update(0,input,{0,0,0},0,error)&&session.owns_pose(1),error);
    {
        const auto* active=session.retained_player_pose();const auto slot=active->current_slot();
        const auto clip=active->slots()[slot].clip_id;const auto generation=active->slots()[slot].generation;
        const auto time=active->slots()[slot].timeline.current_ms;
        check(session.bind_player_locomotion("walk",{"Walk",0,{0}},.75,true,error),error);
        check(session.owns_pose(1)&&session.retained_player_pose()==active&&active->current_slot()==slot&&
              active->slots()[slot].clip_id==clip&&active->slots()[slot].generation==generation&&
              active->slots()[slot].timeline.current_ms==time,
              "Future locomotion binding replaced or advanced the combat-owned pose");
    }
    check(!session.select_player_locomotion("walk",error),"Locomotion stole combat-owned player pose");input={};check(session.update(.016,input,{0,0,0},0,error),error);
    check(session.retained_player_pose()->slots()[outgoingSlot].clip_id==outgoingClip&&session.retained_player_pose()->slots()[outgoingSlot].timeline.last_seconds>outgoingTime,"Attack fade retained a stale outgoing locomotion clock");
    for(unsigned i=0;i<200&&session.owns_pose(1);++i)check(session.update(.016,input,{0,0,0},0,error),error);
    check(!session.owns_pose(1)&&session.select_player_locomotion("walk",error),"Locomotion did not recover after retained action closure");
    input.move2D={0,1};check(session.update(.05,input,{0,0,0},0,error),error);
    session.actor(1)->persistent_character_id=character.id;check(capture_game_save("source-test-level",1,character,*session.world(),captured,error),error);const auto locoRandom=captured.random;
    session.detach_for_restore();check(!session.uses_retained_player_locomotion()&&!session.select_player_locomotion("walk",error),"Detached locomotion retained stale access");check(restore_game_save(captured,"source-test-level",*session.world(),character,error)&&session.rebind_after_restore(error),error);
    check(session.has_player_locomotion("walk")&&session.has_player_locomotion("idle")&&session.select_player_locomotion("walk",error),"Restore lost explicit locomotion definitions");check(session.update(.05,input,{0,0,0},0,error),error);check(session.world()->random_state().seed==locoRandom.seed&&session.world()->random_state().calls==locoRandom.calls,"Locomotion rebind consumed combat RNG");
    std::cout<<"Retained player locomotion passed authored Walk/rate/MoveGO, single session clock, combat handoff, missing run rejection and save rebind\n";
    config.profiles.at("Swamp_LizadMan_Type1").diagnosticAIEnabled=true;population.actors().front().definition.properties["ai_state"]="explicit-test-gate";
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    OriginalActorLifecycle lifecycle;unsigned sourceCompletions=0,spawnFocus=0;bool physicalPresent=true;
    CombatSessionStateAnimationServices lifecycleAnimation; lifecycleAnimation.event=[&](ActorId id,const RetainedAnimationEvent& event,std::string&e){return lifecycle.animation_event(id,event.name,e);};lifecycleAnimation.finished=[&](ActorId id,std::string&e){++sourceCompletions;return lifecycle.animation_finished(id,e);};
    lifecycle.bind({[](std::array<float,3>,bool&found,float&,std::string&){found=false;return true;},[&](const OriginalLifecycleRequest&r,std::string&e){
        switch(r.operation){
        case OriginalLifecycleOperation::set_flags:return session.set_actor_original_state(r.actor->id,r.state,e);
        case OriginalLifecycleOperation::set_enabled:return session.refresh_actor_combat_permissions(e);
        case OriginalLifecycleOperation::restore_initial_position:r.actor->transform.position=r.initial_transform.position;return true;
        case OriginalLifecycleOperation::restore_initial_rotation:r.actor->transform.rotation=r.initial_transform.rotation;return true;
        case OriginalLifecycleOperation::revive:r.actor->health=r.actor->max_health;r.actor->action=CharacterAction::idle;return true;
        case OriginalLifecycleOperation::select_state_animation:
            if(r.state==1){++spawnFocus;OriginalAttackSelection spawn;spawn.state="Spawn";spawn.actor_rate=1;return session.play_actor_state_sequence(r.actor->id,spawn,lifecycleAnimation,e);}
            return session.select_actor_state_leaf(r.actor->id,config.profiles.at("Swamp_LizadMan_Type1").initialIdle,1,false,lifecycleAnimation,e);
        case OriginalLifecycleOperation::freeze_animation_speed:return session.freeze_actor_state_animation(r.actor->id,e);
        case OriginalLifecycleOperation::remove_physical:physicalPresent=false;return true;
        case OriginalLifecycleOperation::init_physical:physicalPresent=true;return true;
        case OriginalLifecycleOperation::clear_aggro:case OriginalLifecycleOperation::clear_and_sync_target:r.actor->target_id=invalid_actor_id;return true;
        default:return true;
        }
    }});
    session.set_actor_combat_permission_provider([&](ActorId id){return !lifecycle.status(id)||lifecycle.combat_enabled(id);});
    OriginalLifecycleFacts spawnFacts;spawnFacts.preset_ai_state="PreSpawn";spawnFacts.initial_transform=session.actor(2)->transform;spawnFacts.initially_enabled=true;spawnFacts.pre_spawn_has_animation=false;spawnFacts.pre_spawn_stay_enabled=false;
    check(lifecycle.add(*session.actor(2),spawnFacts,error),error);check(lifecycle.status(2)->state==17&&!lifecycle.status(2)->enabled&&!physicalPresent,"Actual PreSpawn source focus not applied");check(session.world()->combat_properties(2)->facts.original_state==17&&!session.world()->traits(2)->targetable,"PreSpawn original state/target permission not published");
    const auto* sameOwner=session.retained_actor_pose(2);check(sameOwner,"Source actor retained owner unavailable");const auto frozenSlot=sameOwner->current_slot();const auto frozenMs=sameOwner->slots()[frozenSlot].timeline.current_ms;
    input={};input.targetSelect=true;for(unsigned i=0;i<30;++i)check(session.update(.016,input,{0,0,0},0,error),error);input={};
    check(session.actor(1)->target_id==invalid_actor_id&&session.actor(2)->action!=CharacterAction::attacking&&!session.world()->eligible_target(*session.actor(1),*session.actor(2)),"Hidden PreSpawn entered targeting or diagnostic AI");check(session.retained_actor_pose(2)==sameOwner&&sameOwner->slots()[sameOwner->current_slot()].timeline.scale==0&&sameOwner->slots()[sameOwner->current_slot()].timeline.current_ms==frozenMs,"PreSpawn zero-speed retained clock advanced");
    check(!session.validate_lifecycle_checkpoint(error),"Unpersisted campaign lifecycle checkpoint was accepted");
    check(lifecycle.spawn(2,error),error);check(lifecycle.status(2)->state==1&&spawnFocus==1&&!lifecycle.combat_enabled(2),"Original Spawn focus unavailable");check(session.retained_actor_pose(2)==sameOwner,"Spawn replaced retained pose/root contexts");
    for(unsigned i=0;i<200&&!sourceCompletions;++i){check(session.update(.016,input,{0,0,0},0,error),error);if(!sourceCompletions)check(!session.world()->eligible_target(*session.actor(1),*session.actor(2))&&session.actor(2)->action!=CharacterAction::attacking,"Spawning actor became combat eligible before whole completion");}
    check(sourceCompletions==1&&lifecycle.status(2)->state==3&&lifecycle.combat_enabled(2)&&physicalPresent&&session.world()->traits(2)->targetable,"Actual whole Spawn completion did not transition to Idle3");check(session.retained_actor_pose(2)==sameOwner,"Spawn→Idle replaced retained owner");
    check(session.update(0,input,{0,0,0},0,error)&&session.actor(2)->action==CharacterAction::attacking,"Source Idle did not release diagnostic AI gate/reprepare attack");check(session.retained_actor_pose(2)==sameOwner,"Attack reprepare lost source retained contexts");
    session.set_actor_combat_permission_provider([](ActorId id){return id!=2;});check(session.refresh_actor_combat_permissions(error)&&session.actor(2)->action!=CharacterAction::attacking&&!session.world()->traits(2)->targetable,"Mutable permission did not interrupt forbidden combat");
    check(session.set_actor_original_state(2,1,error),error);OriginalAttackSelection wholeGroup;wholeGroup.state="Attack";wholeGroup.group_path={0};wholeGroup.actor_rate=1;unsigned groupFinished=0,groupMarkers=0;
    CombatSessionStateAnimationServices groupServices;groupServices.event=[&](ActorId,const RetainedAnimationEvent& event,std::string&){if(event.name=="attack_mainhand")++groupMarkers;return true;};groupServices.finished=[&](ActorId id,std::string&e){++groupFinished;return session.select_actor_state_leaf(id,config.profiles.at("Swamp_LizadMan_Type1").initialIdle,1,false,{},e)&&session.set_actor_original_state(id,3,e);};
    check(session.play_actor_state_sequence(2,wholeGroup,groupServices,error),error);for(unsigned i=0;i<500&&!groupFinished;++i){check(session.update(.016,input,{0,0,0},0,error),error);if(groupMarkers==1)check(groupFinished==0,"Whole source state callback fired at first strike/leaf completion");}
    check(groupMarkers==2&&groupFinished==1&&session.retained_actor_pose(2)==sameOwner,"Full generic state group lost repeated markers/whole completion/retained ownership");
    // An independently supplied source program may have a state namespace that
    // does not exist in the actor's normal profile. Metadata is copied, and the
    // same live actor still owns marker delivery, completion and retained slots.
    unsigned externalFinished=0,externalMarkers=0;
    CombatSessionStateAnimationServices externalServices;
    externalServices.event=[&](ActorId id,const RetainedAnimationEvent& event,std::string&){check(id==2,"External marker delivered to foreign actor");if(event.name=="attack_mainhand")++externalMarkers;return true;};
    externalServices.finished=[&](ActorId id,std::string&e){++externalFinished;return session.select_actor_state_leaf(id,config.profiles.at("Swamp_LizadMan_Type1").initialIdle,1,false,{},e);};
    check(session.set_actor_original_state(2,1,error),error);
    {
        OriginalCombatVisualPlan externalPlan;
        check(build_original_combat_visual_plan(assets,bindings,"Swamp_LizadMan_Type1",config.profiles.at("Swamp_LizadMan_Type1").customization,"actor-2",externalPlan,error),error);
        OriginalSequencePolicies externalPolicies;check(original_sequence_policies(bindings,externalPolicies,error),error);
        for(auto& sequence:externalPlan.sequences)if(sequence.state=="Attack")sequence.state="ExternalSourceAction";
        auto externalSelection=wholeGroup;externalSelection.state="ExternalSourceAction";
        auto invalidSelection=externalSelection;invalidSelection.state="AbsentSourceAction";
        const auto slotBefore=sameOwner->current_slot();const auto clockBefore=sameOwner->slots()[slotBefore].timeline.current_ms;
        check(!session.play_actor_source_sequence(2,externalPlan,externalPolicies,invalidSelection,externalServices,error),"Missing external source state accepted");
        check(session.retained_actor_pose(2)==sameOwner&&sameOwner->current_slot()==slotBefore&&sameOwner->slots()[slotBefore].timeline.current_ms==clockBefore,"Rejected external plan changed retained owner/clock");
        check(!session.play_actor_source_sequence(999,externalPlan,externalPolicies,externalSelection,externalServices,error),"Foreign external source actor accepted");
        check(!session.play_actor_source_sequence(2,externalPlan,externalPolicies,externalSelection,{},error),"External program missing completion provider accepted");
        check(session.play_actor_source_sequence(2,externalPlan,externalPolicies,externalSelection,externalServices,error),error);
    }
    for(unsigned i=0;i<500&&!externalFinished;++i)check(session.update(.016,input,{0,0,0},0,error),error);
    check(externalFinished==1&&externalMarkers==2&&session.retained_actor_pose(2)==sameOwner,"External original program lost metadata lifetime, marker/completion or retained ownership");
    check(session.set_actor_original_state(2,3,error),error);
    std::cout<<"External original source program passed sameSessionOwner/copiedMetadata/twoMarkers/oneCompletion/invalidAdmission\n";
    lifecycle.clear();check(session.clear_lifecycle_services(error)&&session.validate_lifecycle_checkpoint(error),error);
    std::cout<<"Original hidden actor17 → actual Spawn sequence → whole completion → Idle3 → combat passed sameRetainedOwner/targetGate/frozenClock/checkpointRejection\n";
    // Source controller wrappers surround the deliberately diagnostic attack
    // body for BOTH actor roles. Controller locks are not lifecycle interrupts.
    config.profiles.at("Swamp_LizadMan_Type1").diagnosticAIEnabled=true;
    config.profiles.at("Swamp_LizadMan_Type1").requiredAIState="explicit-test-gate";
    population.actors().front().definition.properties["ai_state"]="explicit-test-gate";
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    DiagnosticControllerAdmissionFacts playerFlags{1,1,1,1,0,0},npcFlags{2,2,1,0,0,0};
    unsigned playerNetworkQueries=0,npcNetworkQueries=0;bool online=false;
    session.set_diagnostic_controller_admission_provider(
        [&](ActorId id,DiagnosticControllerAdmissionFacts& flags,std::string&){flags=id==1?playerFlags:npcFlags;return true;},
        [&](ActorId id,bool& value,std::string&){if(id==1)++playerNetworkQueries;else ++npcNetworkQueries;value=online;return true;});
    check(session.uses_diagnostic_controller_admission()&&session.original_ai_tables()==&session.world()->factions(),
        "Session lost typed source-controller/AI table binding");
    input={};input.attack=true;
    check(session.update(.01,input,{0,0,0},0,error),error);
    check(!session.owns_pose(1)&&!session.owns_pose(2)&&session.actor(1)->target_id==0&&session.actor(2)->target_id==0&&
        session.actor(1)->transform.rotation[2]==0&&session.actor(2)->transform.rotation[2]==0&&
        playerNetworkQueries==0&&npcNetworkQueries==0,"blocked command mutated target/turn/pose or queried network");
    playerFlags.forced=255;
    check(session.update(.01,input,{0,0,0},0,error),error);
    check(session.owns_pose(1)&&!session.owns_pose(2)&&session.actor(1)->target_id==2&&session.actor(2)->target_id==0&&
        playerNetworkQueries==1&&npcNetworkQueries==0,"forced player bypass or blocked AI distinction differs");
    const auto targetBeforeBlocked=session.actor(1)->target_id;
    const auto facingBeforeBlocked=session.actor(1)->transform.rotation[2];
    playerFlags.forced=0;
    const auto* sequence=session.attack_sequence(1);
    const auto admittedMarker=std::find_if(sequence->scheduled_markers().begin(),sequence->scheduled_markers().end(),
        [](const auto& marker){return marker.source.name=="attack_mainhand";});
    check(admittedMarker!=sequence->scheduled_markers().end(),"Actual admitted source action marker unavailable");
    bool admittedMarkerDelivered=false;
    for(unsigned frame=0;frame<120&&!admittedMarkerDelivered;++frame){
        check(session.update(.016,input,{0,0,0},facingBeforeBlocked,error),error);
        admittedMarkerDelivered=std::any_of(session.events().begin(),session.events().end(),[](const auto& event){return event.attacker==1;});
        check(session.actor(1)->target_id==targetBeforeBlocked&&session.actor(1)->transform.rotation[2]==facingBeforeBlocked&&
            playerNetworkQueries==1,"blocked fresh command mutated targeting/turn or queried network");
    }
    check(admittedMarkerDelivered,"blocking new commands interrupted active authored markers");
    const auto& namedDispatches=session.original_animation_dispatches();
    const auto namedHelper=std::find_if(namedDispatches.begin(),namedDispatches.end(),[](const auto& dispatch){
        return dispatch.actor==1&&dispatch.event==0x28&&dispatch.authored_name=="attack_mainhand"&&
            dispatch.service==dh2::character::ai_event_helper;});
    check(namedHelper!=namedDispatches.end()&&std::next(namedHelper)!=namedDispatches.end()&&
        std::next(namedHelper)->event==0x28&&std::next(namedHelper)->service==dh2::character::ai_event_state_event,
        "Authored damage string did not route source28 helper then same-actor state event under block");
    bool genuineClosureDelivered=false;
    for(unsigned frame=0;frame<200&&!genuineClosureDelivered;++frame){
        check(session.update(.016,input,{0,0,0},facingBeforeBlocked,error),error);
        const auto& notifications=session.original_animation_dispatches();
        const auto end=std::find_if(notifications.begin(),notifications.end(),[](const auto& dispatch){
            return dispatch.actor==1&&dispatch.event==0x22&&dispatch.service==dh2::character::animation_end_virtual;});
        genuineClosureDelivered=end!=notifications.end();
        if(genuineClosureDelivered)check(std::next(end)!=notifications.end()&&
            std::next(end)->event==0x22&&std::next(end)->service==dh2::character::animation_state_event,
            "Genuine finite closure did not preserve locked end-virtual/state order");
    }
    check(genuineClosureDelivered,"Genuine finite retained action closure was not routed as numeric22");
    check(!session.validate_lifecycle_checkpoint(error),"Unpersisted controller provider accepted by checkpoint validation");
    const auto retainedOwner=session.retained_actor_pose(1);
    const auto targetBeforeProviderClear=session.actor(1)->target_id;
    session.clear_diagnostic_controller_admission_provider();
    check(!session.uses_diagnostic_controller_admission()&&session.validate_lifecycle_checkpoint(error)&&
        session.retained_actor_pose(1)==retainedOwner&&session.actor(1)->target_id==targetBeforeProviderClear,
        "Clearing transient admission interrupted source pose/target");
    // Forced AI goes through the same source kernel while player remains blocked.
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    playerFlags={1,1,0,255,0,0};npcFlags={2,2,1,255,255,0};playerNetworkQueries=npcNetworkQueries=0;
    session.set_diagnostic_controller_admission_provider(
        [&](ActorId id,DiagnosticControllerAdmissionFacts& flags,std::string&){flags=id==1?playerFlags:npcFlags;return true;},
        [&](ActorId id,bool& value,std::string&){if(id==1)++playerNetworkQueries;else ++npcNetworkQueries;value=false;return true;});
    check(session.update(.01,input,{0,0,0},0,error)&&!session.owns_pose(1)&&session.owns_pose(2)&&
        session.actor(1)->target_id==0&&session.actor(2)->target_id==1&&playerNetworkQueries==0&&npcNetworkQueries==1,
        "AI forced/global/local admission differs from player wrapper");
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    playerFlags={1,1,0,0,0,1};npcFlags={2,2,1,0,0,0};online=true;
    session.set_diagnostic_controller_admission_provider(
        [&](ActorId id,DiagnosticControllerAdmissionFacts& flags,std::string&){flags=id==1?playerFlags:npcFlags;return true;},
        [&](ActorId,bool& value,std::string&){value=online;return true;});
    const auto randomBeforeUnsupported=session.world()->random_state();
    check(!session.update(.01,input,{0,0,0},0,error)&&!session.owns_pose(1)&&session.actor(1)->target_id==0&&
        session.actor(1)->transform.rotation[2]==0&&session.world()->random_state().calls==randomBeforeUnsupported.calls,
        "Unsupported original online attack silently used diagnostic target/turn/body");
    session.clear_diagnostic_controller_admission_provider();
    // Numeric notifications are explicit scheduler inputs, not guessed strings.
    playerFlags={1,1,1,1,0,0};npcFlags={2,2,1,1,0,0};
    session.set_diagnostic_controller_admission_provider(
        [&](ActorId id,DiagnosticControllerAdmissionFacts& flags,std::string&){flags=id==1?playerFlags:npcFlags;return true;},
        [](ActorId,bool& value,std::string&){value=false;return true;});
    session.actor(1)->action=CharacterAction::attacking;session.actor(2)->action=CharacterAction::attacking;
    unsigned numericBegin=0,numericEndVirtual=0,numericState=0;
    CombatSessionAnimationNotificationServices notifications;
    notifications.notification=[&](ActorId id,const dh2::character::AnimationEventRequest& request,std::int32_t& result,std::string&){
        check(session.actor(id)!=nullptr,"Animation notification used another actor owner");
        if(request.service==dh2::character::animation_attack_begin){++numericBegin;result=1;}
        if(request.service==dh2::character::animation_end_virtual)++numericEndVirtual;
        return true;};
    notifications.state_event=[&](ActorId id,std::uint32_t event,const RetainedAnimationEvent* named,std::string&){
        check((id==1||id==2)&&!named&&(event==0x22||event==0x26),"Numeric state notification ownership/payload differs");
        ++numericState;return true;};
    session.set_animation_notification_services(notifications);
    check(session.deliver_original_animation_notification(1,0x26,0,error)&&
        session.deliver_original_animation_notification(2,0x26,0,error)&&numericBegin==0&&numericState==2,
        "Blocked player/enemy numeric begin consumed AI callback instead of state forwarding");
    playerFlags.forced=255;
    check(session.deliver_original_animation_notification(1,0x26,0,error)&&numericBegin==1&&numericState==3,
        "Forced numeric notification did not use live shared attack state");
    playerFlags.forced=0;
    check(session.deliver_original_animation_notification(1,0x22,0,error)&&
        session.deliver_original_animation_notification(2,0x22,0,error)&&numericEndVirtual==2&&numericState==5,
        "Blocked genuine numeric closure omitted end virtual/state event");
    check(session.actor(1)->action==CharacterAction::attacking&&session.actor(2)->action==CharacterAction::attacking,
        "Notification bridge invented begin/end FSM effects");
    session.clear_diagnostic_controller_admission_provider();
    check(!session.validate_lifecycle_checkpoint(error),"Unpersisted animation consumers accepted in checkpoint");
    session.clear_animation_notification_services();check(session.validate_lifecycle_checkpoint(error),error);
    session.set_retained_frame_audio_observer([](ActorId,const RetainedAnimationEvent&,std::uint32_t,
        const RetainedFrameAudioClock*,std::string& e){e.clear();return RetainedFrameAudioObserverStatus::not_applicable;});
    check(session.validate_lifecycle_checkpoint(error),"Presentation-only audio observer rejected gameplay checkpoint");
    session.clear_retained_frame_audio_observer();
    std::uint32_t* borrowedTurn=nullptr;check(session.borrow_rotation_turn(1,borrowedTurn,error)&&borrowedTurn,"Same session rotation turn field unavailable");
    auto* sameTurn=borrowedTurn;check(session.borrow_rotation_turn(1,borrowedTurn,error)&&borrowedTurn==sameTurn,"Rotation borrow manufactured another turn owner");
    check(!session.borrow_rotation_turn(999999,borrowedTurn,error),"Unknown actor acquired a rotation turn field");
    // Authored companions share this session's retained animation owner while
    // remaining outside attack admission and target selection.
    auto companionConfig=config;companionConfig.profiles.clear();
    companionConfig.profiles.emplace("KnightPlayerBase",config.profiles.at("KnightPlayerBase"));
    ActorCustomization companionCustomization;companionCustomization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan priestPlan,fairyPlan;
    check(build_original_combat_visual_plan(assets,bindings,"WanderingPriest",companionCustomization,"actor-7",priestPlan,error),error);
    check(build_original_combat_visual_plan(assets,bindings,"DefaultFairy",companionCustomization,"actor-8",fairyPlan,error),error);
    check(priestPlan.sequence("Idle",0)&&priestPlan.sequence("Idle",0)->loop==-1&&
          fairyPlan.sequence("Idle",0)&&fairyPlan.sequence("Idle",0)->loop==-1,
          "Companion test no longer uses the exact authored infinite Idle source records");
    ActorPopulation companionPopulation;
    for(const auto& source:std::vector<std::pair<ActorId,std::string>>{{7,"WanderingPriest"},{8,"DefaultFairy"}}){
        PopulationActor placed;placed.profileId=source.second;placed.definition.stableId=source.first;
        placed.definition.sourceId=source.second=="WanderingPriest"?"_prim_NPC_PriestGood":"_prim_Faery";
        placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,float(source.first*3),0,0,1};
        companionPopulation.actors().push_back(std::move(placed));
        CombatSessionProfile visualOnly;visualOnly.initialIdle={"Idle",0,{0}};visualOnly.animationOnly=true;
        visualOnly.propertyOptions={std::nullopt,false};visualOnly.customization.allow_missing_animation_targets=true;
        companionConfig.profiles.emplace(source.second,std::move(visualOnly));
    }
    CharacterVisual companionPlayer;CombatSession companionSession;
    check(companionSession.initialize(assets,database,bindings,companionConfig,companionPlayer,companionPopulation,
          {0,0,0},customization,error),error);
    {
        auto companionCharacter=make_default_character("companion-save-owner","Player","warrior");
        companionSession.actor(1)->persistent_character_id=companionCharacter.id;
        const auto priestSheets=companionSession.world()->combat_properties(7)->sheets;
        const auto fairySheets=companionSession.world()->combat_properties(8)->sheets;
        GameSave companionSnapshot;
        check(capture_game_save("source-companion-test",1,companionCharacter,*companionSession.world(),companionSnapshot,error),error);
        const auto companionSavePath=std::filesystem::temp_directory_path()/"dh-animation-only-companions.save";
        check(save_game(companionSavePath,companionSnapshot,error),error);
        GameSave companionLoaded;check(load_game(companionSavePath,companionLoaded,error),error);
        auto changedSentinel=companionLoaded;bool modifiedSentinel=false;
        for(auto& savedActor:changedSentinel.actors)if(savedActor.actor.id==8)
            for(const unsigned index:{36u,38u,41u,43u})if(savedActor.combat.sheets.resolved[index]<0){
                --savedActor.combat.sheets.resolved[index];modifiedSentinel=true;break;
            }
        check(modifiedSentinel,"Source Faery fixture has no unavailable vital sentinel");
        const auto rngBeforeInvalid=companionSession.world()->random_state();
        const auto* stableFairy=companionSession.actor(8);
        check(!restore_game_save(changedSentinel,"source-companion-test",*companionSession.world(),companionCharacter,error)&&
              companionSession.actor(8)==stableFairy&&companionSession.world()->combat_properties(8)->sheets.resolved==fairySheets.resolved&&
              companionSession.world()->random_state().seed==rngBeforeInvalid.seed&&companionSession.world()->random_state().calls==rngBeforeInvalid.calls,
              "Changed source vital sentinel mutated live companion/world RNG");
        auto forgedCombat=companionLoaded;
        for(auto& savedActor:forgedCombat.actors)if(savedActor.actor.id==8)savedActor.traits.targetable=true;
        check(!validate_game_save(forgedCombat,error),"Unavailable vital projection accepted as combat stats");
        const auto previousLease=companionSession.actor_binding_lease();
        companionSession.detach_for_restore();
        check(restore_game_save(companionLoaded,"source-companion-test",*companionSession.world(),companionCharacter,error),error);
        check(companionSession.rebind_after_restore(error),error);
        check(previousLease.expired()&&companionSession.retained_actor_pose(7)&&companionSession.retained_actor_pose(8),"Disk restore did not rebind companion pose ownership");
        const auto& priestRestored=companionSession.world()->combat_properties(7)->sheets;
        const auto& fairyRestored=companionSession.world()->combat_properties(8)->sheets;
        check(priestRestored.base==priestSheets.base&&priestRestored.saved==priestSheets.saved&&priestRestored.resolved==priestSheets.resolved&&
              fairyRestored.base==fairySheets.base&&fairyRestored.saved==fairySheets.saved&&fairyRestored.resolved==fairySheets.resolved,
              "Companion disk save changed original vital sentinel/source sheets");
        std::error_code cleanup;std::filesystem::remove(companionSavePath,cleanup);
    }
    for(const auto id:{ActorId(7),ActorId(8)}){
        const auto* actor=companionSession.actor(id);const auto* properties=companionSession.world()->combat_properties(id);
        check(actor&&properties&&actor->attack_ids.empty()&&!companionSession.world()->traits(id)->targetable,
              "Animation-only source actor acquired combat identity/targetability");
        check(actor->health==std::max(0.0f,original_signed256(properties->sheets.resolved[36]))&&
              actor->max_health==std::max(0.0f,original_signed256(properties->sheets.resolved[38]))&&
              actor->resource==std::max(0.0f,original_signed256(properties->sheets.resolved[41]))&&
              actor->max_resource==std::max(0.0f,original_signed256(properties->sheets.resolved[43])),
              "Animation-only actor vitals differ from exact source rows/nonnegative sentinel projection");
        check(companionSession.retained_actor_pose(id)!=nullptr,"Source Idle did not bind retained pose owner");
        check(!companionSession.request_actor_attack(id,1,0,error)&&
              error.find("Animation-only actor")!=std::string::npos,
              "Animation-only actor accepted attack request");
        check(!companionSession.world()->eligible_target(*companionSession.actor(1),*actor),
              "Animation-only source actor entered target selection");
    }
    check(companionSession.retained_actor_pose(7)->slots()[companionSession.retained_actor_pose(7)->current_slot()].clip_id==
          priestPlan.phase("Idle",0,{0})->clipName&&
          companionSession.retained_actor_pose(8)->slots()[companionSession.retained_actor_pose(8)->current_slot()].clip_id==
          fairyPlan.phase("Idle",0,{0})->clipName,
          "Animation-only actors did not select their exact authored source Idle leaves");
    const auto idlePriestBefore=companionSession.retained_actor_pose(7)->slots()[companionSession.retained_actor_pose(7)->current_slot()].timeline.current_ms;
    const auto idleFairyBefore=companionSession.retained_actor_pose(8)->slots()[companionSession.retained_actor_pose(8)->current_slot()].timeline.current_ms;
    InputActions companionInput;companionInput.targetSelect=true;
    check(companionSession.update(.25,companionInput,{0,0,0},0,error),error);
    check(companionSession.actor(1)->target_id==invalid_actor_id&&
          companionSession.retained_actor_pose(7)->slots()[companionSession.retained_actor_pose(7)->current_slot()].timeline.current_ms!=idlePriestBefore&&
          companionSession.retained_actor_pose(8)->slots()[companionSession.retained_actor_pose(8)->current_slot()].timeline.current_ms!=idleFairyBefore,
          "Shared session did not advance authored Priest/Faery Idle while excluding them from combat");
    check(companionSession.select_actor_state_leaf(8,{"Idle",0,{0}},1,false,{},error),error);
    OriginalAttackSelection fairyCast;fairyCast.state="Attack";fairyCast.variant=0;
    unsigned animationOnlySequenceClosed=0;CombatSessionStateAnimationServices animationServices;
    animationServices.finished=[&](ActorId id,std::string&){check(id==8,"Animation-only sequence changed ActorId owner");++animationOnlySequenceClosed;return true;};
    check(companionSession.play_actor_state_sequence(8,fairyCast,animationServices,error),error);
    for(unsigned frame=0;frame<200&&!animationOnlySequenceClosed;++frame)
        check(companionSession.update(.05,{}, {0,0,0},0,error),error);
    check(animationOnlySequenceClosed==1&&companionSession.actor(8)->attack_ids.empty(),
          "Animation-only actor did not finish its exact source sequence without combat admission");
    // The explicit campaign completion callback is transient and still follows
    // the existing checkpoint contract; this test restores ordinary Idle.
    check(companionSession.clear_lifecycle_services(error),error);
    const auto companionLease=companionSession.actor_binding_lease();
    companionSession.detach_for_restore();check(!companionSession.retained_actor_pose(8),"Detached animation-only pose remained borrowable");
    check(companionLease.expired(),"Detached animation-only session retained its old lifetime lease");
    check(companionSession.rebind_after_restore(error),error);
    check(companionSession.retained_actor_pose(7)&&companionSession.retained_actor_pose(8)&&
          companionSession.actor(7)->attack_ids.empty()&&companionSession.actor(8)->attack_ids.empty(),
          "Restore did not rebind genuine animation-only companion owners");
    std::cout<<"Animation-only WanderingPriest/DefaultFairy retain exact vitals, authored idle and restore ownership without combat IDs\n";
    std::cout<<"Same-owner source animation28 names bypass command block; numeric22/26 gate ordering and explicit state consumers passed\n";
    std::cout<<"Typed original controller admission passed blocked/forced shared player+AI, no early target/turn, active markers continue, transient checkpoint rejection\n";
    return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}

