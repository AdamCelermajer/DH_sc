#include "../combat_session.hpp"
#include "../game_save.hpp"
#include "../content_paths.hpp"
#include "../features/combat/runtime_player_profile_attack_bank_v1.hpp"
#include "../features/equipment/runtime_player_locomotion_v1.hpp"
#include "../features/skills_animation/skill_animation_program.hpp"
#include "../../script-runtime/script_constants.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
Vec3 actor_position(const ActorState& actor){return {actor.transform.position[0],actor.transform.position[1],actor.transform.position[2]};}
bool same_owner(const std::weak_ptr<const void>& a,const std::weak_ptr<const void>& b){
    const auto x=a.lock(),y=b.lock();return x&&y&&!x.owner_before(y)&&!y.owner_before(x);
}
struct Fixture {
    AssetCatalog assets;
    OriginalPropertyDatabase database;
    OriginalMeleeBindings melee;
    ActorCustomization customization;
    OriginalCombatVisualPlan knight_plan,lizard_plan;
    combat::RuntimePlayerProfileAttackBankPlanV1 knight_attacks;
    skills_animation::SkillAnimationPrograms source_skill_bank;
    CombatSessionConfig config;
    ActorPopulation population;
    CharacterVisual visual;
    CombatSession session;
    std::string error;

    explicit Fixture(const char* root,bool incoming=false,std::uint32_t seed=17,bool useSourceAttackBank=false):assets(root){
        check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
        check(melee.load(assets,"original-melee-bindings.xml",error),error);
        customization.allow_missing_animation_targets=true;
        check(build_original_combat_visual_plan(assets,melee,"KnightPlayerBase",customization,
              "accepted-transition-knight",knight_plan,error),error);
        check(build_original_combat_visual_plan(assets,melee,"Swamp_LizadMan_Type1",customization,
              "accepted-transition-lizard",lizard_plan,error),error);
        // Use the complete authored Knight Attack/AttackStatic graphs so the
        // transition test exercises predecessor-state selection, not a
        // hand-authored Attack selection.  This stays a level-20 continuity
        // fixture for the enemy; it is not an Act1 balance claim.
        {
            const auto read=[&](const char* name){return read_content(assets,std::string("original-cache/data/pydata/")+name);};
            const auto bytes=[](const auto& buffer){return dh2::data::Bytes{buffer.data(),buffer.size()};};
            const auto clipNames=read_content(assets,"data/animations_dictionary_pyarraynames.bin");
            const auto clipValues=read("animations_dictionary_pyarray.bin");
            dh2::data::Dictionary clips;check(dh2::data::load_dictionary(bytes(clipNames),bytes(clipValues),clips,error),error);
            const auto records=read("animations_pyarray.bin"),names=read("animations_pyarraynames.bin"),fields=read("animations_pystructnames.bin");
            dh2::data::AnimationTables animations;check(dh2::data::load_animation_tables(bytes(records),bytes(names),bytes(fields),clips,animations,error),error);
            const auto itemRecords=read("loot_table_pyarray.bin"),itemNames=read("loot_table_pyarraynames.bin"),itemFields=read("loot_table_pystructnames.bin");
            dh2::data::ItemTable items;check(dh2::data::load_items(bytes(itemRecords),bytes(itemNames),bytes(itemFields),items,error),error);
            const auto constantBytes=read_content(assets,"data/animations_pycst.bin");
            auto* constants=dh2_script_constants_create();check(constants,"Source constants unavailable");
            dh2_script_constants_reload reload{};
            check(dh2_script_constants_load(constants,constantBytes.data(),static_cast<std::uint32_t>(constantBytes.size()),&reload)==0,
                  "Source constants failed");
            equipment_menu::RuntimePlayerLocomotionConstantsV1 stance;
            const bool stanceOk=equipment_menu::load_runtime_player_locomotion_constants_v1(
                [&](const char* group,const char* key,std::int32_t& value,std::string& e){
                    if(dh2_script_constants_get(constants,group,key,&value)){e="Missing source constant";return false;}return true;
                },stance,error);
            dh2_script_constants_destroy(constants);check(stanceOk,error);
            ActorProfileLibrary profiles;check(profiles.load(assets,"actor-profiles-v2.xml",error),error);
            const auto* sourceKnight=profiles.find("KnightPlayerBase");check(sourceKnight,"Actual Knight profile missing");
            equipment_menu::RuntimePlayerLocomotionV1 equipped;
            const auto longsword=dh2::data::item_id(items,"Longsword01");check(longsword>=0,"Source Longsword missing");
            check(equipment_menu::resolve_runtime_player_locomotion_v1(std::stoi(sourceKnight->animation_table),items,
                longsword,-1,0,stance,animations,clips,equipped,error),error);
            check(combat::plan_runtime_player_profile_attack_bank_v1(assets,*sourceKnight,equipped,
                stance.stanced_list_mask,animations,clips,knight_plan,"accepted-transition-knight",knight_attacks,error),error);
            check(skills_animation::build_skill_animation_programs(assets,animations,clips,knight_plan.config,
                {347},"accepted-transition-knight-skill",source_skill_bank,error),error);
        }
        auto playerVisual=source_skill_bank.plan.config;playerVisual.motion_node_id="auto";playerVisual.consume_root_motion=true;
        config.diagnosticRngSeed=seed;config.playerId=1;config.playerProfileId="KnightPlayerBase";
        config.tableRoot="original-cache/data/pydata";config.playerVisualConfig=playerVisual;
        config.mainItemId="Longsword01";
        config.equippedItemIds={"StartingSuit","StartingBoots","StartingGloves","Longsword01"};
        CombatSessionProfile player;player.initialIdle={"Idle",0,{0}};
        if(!incoming){
            if(useSourceAttackBank){
                player.sequenceAction=knight_attacks.static_selection;
                player.sourceAttackBank=knight_attacks.bank;
                player.sourceAttackPolicies=knight_attacks.sequence_policies;
                player.sourceAttackStateSelection=true;
            }else player.sequenceAction=OriginalAttackSelection{"Attack",0,{0}};
            player.damageMarkerNames={"attack_mainhand"};
            player.sourceAnimationClips=source_skill_bank.plan.config.clips;player.customization=customization;
        }
        player.retainedPhaseClock=true;
        player.propertyOptions={256,true};player.reaction=CombatSessionChoice{"Injured",0,{0}};
        player.death=CombatSessionChoice{"Died",0,{0}};player.motionRoot="auto";
        if(incoming){player.animationOnly=true;player.receiveDamage=true;player.reactionMinimalRandoms=true;}
        config.profiles.emplace(config.playerProfileId,player);
        CombatSessionProfile enemy;enemy.action={"Attack",0,{0,1}};enemy.initialIdle={"Idle",0,{0}};
        enemy.damageMarkerNames={"attack_mainhand"};
        enemy.propertyOptions={20*256,true};enemy.customization=customization;enemy.motionRoot="auto";
        enemy.reaction=CombatSessionChoice{"Injured",0,{0}};enemy.death=CombatSessionChoice{"Died",0,{0}};
        config.profiles.emplace("Swamp_LizadMan_Type1",enemy);
        PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
        placed.definition.stableId=2;placed.definition.sourceId="accepted-transition-source-lizard";
        placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
        placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
        initialize();
    }
    void initialize(){
        check(session.initialize(assets,database,melee,config,visual,population,{0,0,0},customization,error),error);
        check(session.update(0,{},Vec3{0,0,0},0,error),error);
        session.actor(1)->persistent_character_id="accepted-transition-character";
    }
    bool bind(CombatSession::ActorTransitionHandler handler){
        const auto lease=session.actor_binding_lease();
        return session.bind_reconstructible_actor_transition_handler(std::move(handler),[&,lease](std::string& e){
            if(!same_owner(lease,session.actor_binding_lease())){e="fixture transition validator saw stale lease";return false;}
            for(const auto id:{ActorId(1),ActorId(2)})if(!session.actor(id)||!session.world()->combat_properties(id)){
                e="fixture transition facts unavailable";return false;
            }
            e.clear();return true;
        },error);
    }
};

struct Recorder {
    Fixture& f;std::vector<CombatSessionActorTransition> events;
    bool assertSourceAttackSelection=false;
    bool reject_once=false;CombatSessionTransitionStage reject_stage=CombatSessionTransitionStage::focus_prefix;
    bool probe_reentry=false,reentry_rejected=false;
    bool operator()(const CombatSessionActorTransition& event,std::string& error){
        check(event.actor==1,"Unexpected transition actor in player trace");
        check(same_owner(event.binding_lease,f.session.actor_binding_lease()),"Transition receipt used stale actor lease");
        const auto* props=f.session.world()->combat_properties(event.actor);const auto* actor=f.session.actor(event.actor);
        check(props&&actor,"Transition callback lost current actor facts");
        if(event.stage==CombatSessionTransitionStage::blur){
            check(props->facts.original_state==event.from_state,"Blur did not observe old canonical source state");
            if(event.cause==CombatRuntimeTransitionCause::locomotion){
                check(actor->action==CharacterAction::idle,"Move Blur ran after locomotion action mutation");
            }else if(event.cause==CombatRuntimeTransitionCause::attack){
                check((event.from_state==4?actor->action==CharacterAction::moving:actor->action==CharacterAction::idle)&&actor->target_id==2,
                      "Attack Blur changed old action/target first: action="+std::to_string(static_cast<int>(actor->action))+" target="+std::to_string(actor->target_id));
                if(assertSourceAttackSelection){
                    const char* expectedState=event.from_state==4?"Attack":"AttackStatic";
                    const auto* selected=f.session.attack_sequence(event.actor);
                    const auto* expected=f.knight_attacks.bank.sequence(expectedState,0);
                check(selected&&expected&&!selected->phases().empty()&&!expected->phases.empty()&&
                      selected->phases()[0].source.sourceUri==expected->phases[0].sourceUri,
                      std::string("Blur did not observe actual predecessor-selected ")+expectedState+" source root");
                    check(event.source_attack_moving.has_value()&&*event.source_attack_moving==(event.from_state==4),
                          "Attack Blur receipt did not carry its exact prepared moving/static root fact");
                }
            }else if(event.cause==CombatRuntimeTransitionCause::completion){
                if(event.from_state==5)check(actor->action==CharacterAction::attacking&&actor->target_id==2,
                    "Attack completion Blur saw action="+std::to_string(static_cast<int>(actor->action))+" target="+std::to_string(actor->target_id));
                else if(event.from_state==11)check(actor->action==CharacterAction::hurt,
                    "Injury completion Blur ran after hurt-action cleanup");
            }else if(event.cause==CombatRuntimeTransitionCause::injury){
                check(actor->action==CharacterAction::idle,"Injury Blur ran after reaction action mutation");
            }else if(event.cause==CombatRuntimeTransitionCause::death){
                check(!actor->alive()&&actor->action!=CharacterAction::dead&&actor->target_id==2,
                      "Death Blur did not preserve HP-zero/old-action/target facts");
            }
        }else{
            check(props->facts.original_state==event.to_state,"Focus stage did not observe committed canonical state");
            if(event.stage==CombatSessionTransitionStage::focus_suffix&&event.cause==CombatRuntimeTransitionCause::locomotion)
                check(actor->action==CharacterAction::moving,"Move focus suffix preceded locomotion publication");
            if(event.stage==CombatSessionTransitionStage::focus_prefix&&event.cause==CombatRuntimeTransitionCause::attack)
                check((event.from_state==4?actor->action==CharacterAction::moving:actor->action==CharacterAction::idle)&&actor->target_id==2,
                      "Attack focus prefix ran after gameplay action commit");
            if(event.stage==CombatSessionTransitionStage::focus_suffix&&event.cause==CombatRuntimeTransitionCause::attack)
                check(actor->action==CharacterAction::attacking&&actor->target_id==2&&f.session.owns_pose(1),
                      "Attack focus suffix preceded successful action/pose publication");
            if(event.stage==CombatSessionTransitionStage::focus_suffix&&event.cause==CombatRuntimeTransitionCause::completion){
                check(actor->action==CharacterAction::idle&&!f.session.owns_pose(1),
                      "Completion focus suffix preceded action/pose cleanup");
                if(event.from_state==5)check(actor->target_id==invalid_actor_id,
                      "Attack completion focus suffix preceded target cleanup");
            }
            if(event.stage==CombatSessionTransitionStage::focus_prefix&&event.cause==CombatRuntimeTransitionCause::injury)
                check(actor->action==CharacterAction::idle,"Injury focus prefix ran after reaction action mutation");
            if(event.stage==CombatSessionTransitionStage::focus_suffix&&event.cause==CombatRuntimeTransitionCause::injury)
                check(actor->action==CharacterAction::hurt&&f.session.owns_pose(1),"Injury focus suffix preceded reaction publication");
            if(event.stage==CombatSessionTransitionStage::focus_prefix&&event.cause==CombatRuntimeTransitionCause::death)
                check(actor->action!=CharacterAction::dead,"Death focus prefix ran after corpse action mutation");
            if(event.stage==CombatSessionTransitionStage::focus_suffix&&event.cause==CombatRuntimeTransitionCause::death)
                check(actor->action==CharacterAction::dead&&f.session.owns_pose(1),"Death focus suffix preceded corpse publication");
        }
        if(probe_reentry&&event.stage==CombatSessionTransitionStage::blur){
            probe_reentry=false;std::string nestedError;
            reentry_rejected=!f.session.request_actor_attack(1,2,0,nestedError)&&!nestedError.empty();
            check(reentry_rejected,"Nested attack command was not rejected during transition delivery");
        }
        events.push_back(event);
        if(reject_once&&event.stage==reject_stage){reject_once=false;error="fixture rejected transition after observed prefix";return false;}
        return true;
    }
};
void check_triplet(const std::vector<CombatSessionActorTransition>& trace,std::size_t at,
                   std::int32_t from,std::int32_t to,CombatRuntimeTransitionCause cause){
    check(trace.size()>=at+3,"Transition trace missing one of its three delivery stages");
    const auto& a=trace[at];const auto& b=trace[at+1];const auto& c=trace[at+2];
    check(a.stage==CombatSessionTransitionStage::blur&&b.stage==CombatSessionTransitionStage::focus_prefix&&
          c.stage==CombatSessionTransitionStage::focus_suffix&&a.from_state==from&&a.to_state==to&&
          a.cause==cause&&a.actor==b.actor&&a.actor==c.actor&&a.from_state==b.from_state&&a.from_state==c.from_state&&
          a.to_state==b.to_state&&a.to_state==c.to_state&&a.generation==b.generation&&a.generation==c.generation&&
          a.occurrence==b.occurrence&&a.occurrence==c.occurrence&&a.update_serial==b.update_serial&&
          a.update_serial==c.update_serial&&same_owner(a.binding_lease,b.binding_lease)&&same_owner(a.binding_lease,c.binding_lease),
          "Transition receipt stages disagree on owner/from/to/generation/occurrence/frame/lease");
    if(cause==CombatRuntimeTransitionCause::attack&&a.source_attack_moving)
        check(a.source_attack_moving&&b.source_attack_moving&&c.source_attack_moving&&
              *a.source_attack_moving==*b.source_attack_moving&&*a.source_attack_moving==*c.source_attack_moving&&
              *a.source_attack_moving==(from==4),"Attack transition stages disagreed on prepared source root");
}

void run_attack_and_restore(const char* root){
    Fixture f(root);Recorder recorder{f};recorder.probe_reentry=true;
    check(f.session.bind_player_locomotion("walk",{"Walk",0,{0}},1.0,true,f.error),f.error);
    check(f.bind([&](const auto& e,std::string& error){return recorder(e,error);}),f.error);
    check(f.session.select_player_locomotion("walk",f.error),f.error);
    check(f.session.world()->combat_properties(1)->facts.original_state==4,
          "Moving input did not publish canonical Move4 before attack admission");
    check_triplet(recorder.events,0,3,4,CombatRuntimeTransitionCause::locomotion);
    check(recorder.reentry_rejected,"Transition consumer reentry was not rejected");
    const auto moveTrace=recorder.events.size();const auto* locomotionPose=f.session.retained_actor_pose(1);
    const auto moveSlot=locomotionPose->current_slot();const auto beforeSlots=locomotionPose->slots();
    check(f.session.update(0,{},actor_position(*f.session.actor(1)),f.session.actor(1)->transform.rotation[2],f.error),f.error);
    const auto afterSlots=locomotionPose->slots();
    check(f.session.actor(1)->action==CharacterAction::moving&&
          f.session.world()->combat_properties(1)->facts.original_state==4&&locomotionPose->current_slot()==moveSlot&&
          beforeSlots[0].timeline.current_ms==afterSlots[0].timeline.current_ms&&
          beforeSlots[1].timeline.current_ms==afterSlots[1].timeline.current_ms&&recorder.events.size()==moveTrace,
          "Zero-time neutral frame did not preserve the admitted Move state/clock");
    f.session.actor(1)->target_id=2; // Target selection precedes attack admission, as in the source input path.
    const auto targetBefore=f.session.actor(2)->transform.position;
    f.session.actor(2)->transform.position[0]=100000.0f;
    const auto afterMove=recorder.events.size();
    check(f.session.request_actor_attack(1,2,0,f.error),f.error);
    check(recorder.events.size()==afterMove&&!f.session.owns_pose(1),"Out-of-range attack dispatched an accepted transition");
    f.session.actor(2)->transform.position=targetBefore;
    f.session.actor(1)->action=CharacterAction::dead;
    (void)f.session.request_actor_attack(1,2,0,f.error);
    check(recorder.events.size()==afterMove&&!f.session.owns_pose(1),
          "Invalid actor action dispatched an accepted transition");
    f.session.actor(1)->action=CharacterAction::moving;
    check(f.session.request_actor_attack(1,2,0,f.error),f.error);
    check_triplet(recorder.events,afterMove,4,5,CombatRuntimeTransitionCause::attack);
    const auto whileAttacking=recorder.events.size();
    (void)f.session.request_actor_attack(1,2,0,f.error);
    check(recorder.events.size()==whileAttacking,"A second command while the accepted attack owns the pose emitted transition callbacks");
    check(f.session.retained_actor_pose(1)->slots()[f.session.retained_actor_pose(1)->current_slot()].clip_id.find("/Attack/")!=std::string::npos,
          "Move4 attack did not retain its authored moving Attack root");
    const auto admitted=recorder.events.size();InputActions held;
    for(unsigned frame=0;frame<1200&&f.session.owns_pose(1);++frame)
        check(f.session.update(1.0/60,held,actor_position(*f.session.actor(1)),f.session.actor(1)->transform.rotation[2],f.error),f.error);
    check(!f.session.owns_pose(1),"Source attack did not complete within the bounded fixture");
    check_triplet(recorder.events,admitted,5,3,CombatRuntimeTransitionCause::completion);
    check(recorder.events.size()==admitted+3,"Held animation frames emitted duplicate accepted transition receipts");
    auto character=make_default_character("accepted-transition-character","Test","warrior");
    character.stats.health=f.session.actor(1)->health;character.stats.max_health=f.session.actor(1)->max_health;
    character.stats.resource=f.session.actor(1)->resource;character.stats.max_resource=f.session.actor(1)->max_resource;
    GameSave saved;check(capture_game_save("accepted-transition",1,character,*f.session.world(),saved,f.error),f.error);
    const auto completedTrace=recorder.events.size();f.session.detach_for_restore();
    check(restore_game_save(saved,"accepted-transition",*f.session.world(),character,f.error)&&f.session.rebind_after_restore(f.error),f.error);
    check(f.session.actor(1)->persistent_character_id==character.id,"Restored transition actor identity changed");
    check(!f.session.update(0,{},actor_position(*f.session.actor(1)),f.session.actor(1)->transform.rotation[2],f.error),
          "Restored Session updated without a fresh transition consumer");
    Recorder rebound{f};check(f.bind([&](const auto& e,std::string& error){return rebound(e,error);}),f.error);
    check(f.session.update(0,{},actor_position(*f.session.actor(1)),f.session.actor(1)->transform.rotation[2],f.error),f.error);
    check(rebound.events.empty()&&recorder.events.size()==completedTrace,"Restore/rebind replayed a transition event");
}

void run_static_attack_selection(const char* root){
    Fixture f(root,false,17,true);Recorder recorder{f};recorder.assertSourceAttackSelection=true;
    check(f.bind([&](const auto& event,std::string& error){return recorder(event,error);}),f.error);
    f.session.actor(1)->target_id=2;
    check(f.session.request_actor_attack(1,2,0,f.error),f.error);
    check_triplet(recorder.events,0,3,5,CombatRuntimeTransitionCause::attack);
    check(f.session.attack_sequence(1)->phases()[0].source.sourceUri==
          f.knight_attacks.bank.sequence("AttackStatic",0)->phases[0].sourceUri,
          "Idle3 attack did not retain the authored AttackStatic graph");

    Fixture moving(root,false,17,true);Recorder movingRecorder{moving};movingRecorder.assertSourceAttackSelection=true;
    check(moving.session.bind_player_locomotion("walk",{"Walk",0,{0}},1.0,true,moving.error),moving.error);
    check(moving.bind([&](const auto& event,std::string& error){return movingRecorder(event,error);}),moving.error);
    check(moving.session.select_player_locomotion("walk",moving.error),moving.error);
    check_triplet(movingRecorder.events,0,3,4,CombatRuntimeTransitionCause::locomotion);
    moving.session.actor(1)->target_id=2;
    const auto targetPosition=moving.session.actor(2)->transform.position;
    moving.session.actor(2)->transform.position[0]=100000.0f;
    const auto beforeReject=movingRecorder.events.size();
    check(moving.session.request_actor_attack(1,2,0,moving.error),moving.error);
    check(movingRecorder.events.size()==beforeReject&&!moving.session.owns_pose(1)&&
          moving.session.attack_sequence(1)->phases()[0].source.sourceUri==
              moving.knight_attacks.bank.sequence("AttackStatic",0)->phases[0].sourceUri,
          "Rejected moving request changed the selected source root");
    moving.session.actor(2)->transform.position=targetPosition;
    check(moving.session.request_actor_attack(1,2,0,moving.error),moving.error);
    check_triplet(movingRecorder.events,beforeReject,4,5,CombatRuntimeTransitionCause::attack);
    check(moving.session.attack_sequence(1)->phases()[0].source.sourceUri==
          moving.knight_attacks.bank.sequence("Attack",0)->phases[0].sourceUri,
          "Move4 attack did not retain the authored Attack graph");
}

void run_source_combo_target_continuity(const char* root){
    Fixture f(root,false,17,true);
    f.config.profiles.at(f.config.playerProfileId).sourceCombo=true;
    f.initialize();
    Recorder recorder{f};recorder.assertSourceAttackSelection=true;
    check(f.bind([&](const auto& event,std::string& error){return recorder(event,error);}),f.error);
    InputActions input;input.targetSelect=true;
    check(f.session.update(0,input,actor_position(*f.session.actor(1)),0,f.error),f.error);
    check(f.session.actor(1)->target_id==2&&f.session.selectedactor()&&f.session.selectedactor()->id==2,
          "Source-combo fixture did not select its original Lizard enemy");
    input.targetSelect=false;input.attack=true;
    check(f.session.update(0,input,actor_position(*f.session.actor(1)),0,f.error)&&f.session.owns_pose(1),f.error);
    check_triplet(recorder.events,0,3,5,CombatRuntimeTransitionCause::attack);
    std::set<std::uint32_t> groups{0};std::uint64_t generation=0;
    for(const auto& boundary:f.session.combo_boundaries())if(boundary.beginning&&boundary.depth==0)generation=boundary.generation;
    check(generation!=0,"Full-bank source combo did not expose its original action generation");
    for(unsigned frame=0;frame<1000&&f.session.owns_pose(1);++frame){
        const auto* source=f.session.source_attack_state(1);
        check(source,"Full-bank source combo lost its current authored group");
        input.attack=source->index<2;
        check(f.session.update(.016,input,actor_position(*f.session.actor(1)),
              f.session.actor(1)->transform.rotation[2],f.error),f.error);
        for(const auto& boundary:f.session.combo_boundaries())if(boundary.beginning&&boundary.depth==0){
            groups.insert(boundary.step);
            check(boundary.generation==generation,"Held source combo replaced its attack generation");
        }
        check(f.session.actor(1)->target_id==2&&f.session.selectedactor()&&f.session.selectedactor()->id==2,
              "Held full-bank source combo changed its live enemy target");
    }
    check(!f.session.owns_pose(1)&&groups==std::set<std::uint32_t>{0,1,2},
          "Held full-bank source combo did not complete all three authored Knight groups");
    check(f.session.actor(1)->target_id==2&&f.session.selectedactor()&&f.session.selectedactor()->id==2,
          "Full-bank source combo completion cleared its still-live selected target");
    std::cout<<"PASS Knight full-bank sourceCombo kept target_id/selectedactor=2 through groups 0/1/2\n";
}

void run_actual_positive_attack_delay(const char* root){
    // The original player AI row44 has AttackDelay=0, so this source-backed
    // timing branch uses the original Swamp Lizard AI row30 (800ms). Its
    // authored animation lasts longer than 800ms, which distinguishes the
    // required post-Blur departure timer from an incorrectly start-based one.
    Fixture f(root);
    const auto* sourceEnemy=f.melee.find_actor("Swamp_LizadMan_Type1");
    check(sourceEnemy&&sourceEnemy->aiProperties.count("AttackDelay")&&
          sourceEnemy->aiProperties.at("AttackDelay")=="800",
          "Positive cooldown fixture is not the authored Swamp Lizard AI delay");
    // Give the original player an unchanged higher-level source property row
    // so the real Lizard's authored attack can finish without killing it.
    f.config.profiles.at(f.config.playerProfileId).propertyOptions={20*256,true};
    f.initialize();
    std::vector<CombatSessionActorTransition> transitions;
    check(f.bind([&](const CombatSessionActorTransition& event,std::string& error){
        if(event.actor!=2){error.clear();return true;}
        check(event.actor==2&&same_owner(event.binding_lease,f.session.actor_binding_lease()),
              "Lizard AttackDelay transition used another actor/session");
        const auto* props=f.session.world()->combat_properties(2);
        check(props&&props->facts.original_state==(event.stage==CombatSessionTransitionStage::blur?
              event.from_state:event.to_state),"Positive-delay transition saw wrong state order");
        if(event.cause==CombatRuntimeTransitionCause::completion&&event.stage==CombatSessionTransitionStage::blur)
            check(event.from_state==5&&event.to_state==3&&f.session.actor(2)->action==CharacterAction::attacking&&
                  f.session.actor(2)->target_id==1,"AttackDelay departure Blur lost the outgoing attack/target");
        if(event.cause==CombatRuntimeTransitionCause::completion&&event.stage==CombatSessionTransitionStage::focus_suffix)
            check(f.session.actor(2)->action==CharacterAction::idle&&
                  f.session.actor(2)->target_id==invalid_actor_id,
                  "AttackDelay completion did not retire the attack at Focus suffix");
        transitions.push_back(event);error.clear();return true;
    }),f.error);
    check(f.session.request_actor_attack(2,1,0,f.error),f.error);
    check_triplet(transitions,0,3,5,CombatRuntimeTransitionCause::attack);
    check(f.session.owns_pose(2)&&f.session.actor(2)->action==CharacterAction::attacking,
          "Positive-delay Lizard did not begin its actual attack pose");
    unsigned frames=0;
    for(;frames<1200&&f.session.owns_pose(2);++frames)
        check(f.session.update(1.0/60,{},actor_position(*f.session.actor(1)),0,f.error),f.error);
    const double elapsed=frames/60.0;
    check(!f.session.owns_pose(2)&&elapsed>.05&&transitions.size()==6,
          "Original Lizard attack did not leave after a nonzero authored animation duration");
    check_triplet(transitions,3,5,3,CombatRuntimeTransitionCause::completion);
    const auto afterCompletion=transitions.size();
    check(f.session.request_actor_attack(2,1,0,f.error)&&!f.session.owns_pose(2)&&transitions.size()==afterCompletion,
          "800ms original AttackDelay was not active immediately after attack departure");
    // Delay starts on Attack Blur, so 750ms after departure is still blocked.
    // Starting it with the attack would include the elapsed pose duration and
    // expire early; the authored pose is separately asserted nonzero above.
    check(f.session.update(.75,{},actor_position(*f.session.actor(1)),0,f.error),f.error);
    check(f.session.request_actor_attack(2,1,0,f.error)&&!f.session.owns_pose(2)&&transitions.size()==afterCompletion,
          "Lizard cooldown expired before its authored 800ms departure delay");
    check(f.session.update(.06,{},actor_position(*f.session.actor(1)),0,f.error),f.error);
    check(f.session.request_actor_attack(2,1,0,f.error)&&f.session.owns_pose(2)&&
          transitions.size()==afterCompletion+3,
          "Lizard did not admit a fresh attack after its actual 800ms departure cooldown");
    std::cout<<"PASS Swamp Lizard AI row30 AttackDelay=800ms; source attack frames="<<frames
             <<"; blocked at departure and +750ms, admitted after +810ms\n";
}

void run_generic_skill_cast_order(const char* root){
    for(const std::int32_t state:{6,7}){
        Fixture f(root);std::vector<std::string> order;std::vector<CombatSessionActorTransition> events;
        unsigned finished=0,departed=0;
        check(f.bind([&](const CombatSessionActorTransition& event,std::string& error){
            check(event.actor==1&&same_owner(event.binding_lease,f.session.actor_binding_lease()),
                  "Generic source transition used a stale owner");
            const auto* properties=f.session.world()->combat_properties(1);
            check(properties&&properties->facts.original_state==(event.stage==CombatSessionTransitionStage::blur?
                event.from_state:event.to_state),"Generic source transition stage exposed wrong canonical state");
            if(event.cause==CombatRuntimeTransitionCause::source_program){
                check(event.from_state==3&&event.to_state==state,"Skill/Cast start did not enter its source state");
                order.push_back(event.stage==CombatSessionTransitionStage::blur?"start-blur":
                    event.stage==CombatSessionTransitionStage::focus_prefix?"start-prefix":"start-suffix");
            }else if(event.cause==CombatRuntimeTransitionCause::completion){
                check(event.from_state==state&&event.to_state==3,"Skill/Cast completion did not return to Idle3");
                if(event.stage==CombatSessionTransitionStage::blur)
                    check(f.session.actor(1)->action==CharacterAction::casting,
                          "Normal Skill/Cast completion Blur lost the old action before Post");
                order.push_back(event.stage==CombatSessionTransitionStage::blur?"completion-blur":
                    event.stage==CombatSessionTransitionStage::focus_prefix?"completion-prefix":"completion-suffix");
            }else check(false,"Generic Skill/Cast emitted an unrelated transition cause");
            events.push_back(event);error.clear();return true;
        }),f.error);
        CombatSessionStateAnimationServices services;
        services.finished=[&](ActorId id,std::string& error){
            check(id==1&&f.session.original_actor_state(1)==state,
                  "Normal source Post ran after destination state publication");
            ++finished;order.push_back("finished-post");f.session.actor(1)->action=CharacterAction::idle;
            error.clear();return true;
        };
        services.departed=[&](ActorId,std::int32_t,std::int32_t,std::string& error){
            ++departed;order.push_back("interrupted-departure");error.clear();return true;
        };
        services.checkpoint=[](std::string& error){error.clear();return true;};
        OriginalAttackSelection selection;selection.state=f.source_skill_bank.plan.sequences.front().state;
        CombatSessionSourceSequencePolicy policy{state,state==6?0x6341u:0x6301u,100+static_cast<std::uint64_t>(state)};
        check(f.session.play_actor_source_sequence(1,f.source_skill_bank.plan,f.source_skill_bank.policies,
              selection,std::move(services),policy,f.error),f.error);
        check(events.size()==3&&events[0].stage==CombatSessionTransitionStage::blur&&
              events[1].stage==CombatSessionTransitionStage::focus_prefix&&
              events[2].stage==CombatSessionTransitionStage::focus_suffix&&
              f.session.original_actor_state(1)==state,"Skill/Cast start did not complete its source transition stages");
        f.session.actor(1)->action=CharacterAction::casting;
        for(unsigned frame=0;frame<1200&&f.session.original_actor_state(1)!=3;++frame)
            check(f.session.update(1.0/60,{},actor_position(*f.session.actor(1)),0,f.error),f.error);
        check(finished==1&&departed==0&&f.session.original_actor_state(1)==3,
              "Finite Skill/Cast completion used interruption Post or failed to return to Idle");
        check(order==std::vector<std::string>{"start-blur","start-prefix","start-suffix","completion-blur",
              "finished-post","completion-prefix","completion-suffix"},
              "Normal Skill/Cast completion did not order Blur, finished/Post, and Focus exactly once");
        check(events.size()==6&&events[3].generation==policy.generation&&events[4].generation==policy.generation&&
              events[5].generation==policy.generation,"Skill/Cast completion changed its source occurrence");
    }
    Fixture f(root);std::vector<CombatSessionActorTransition> events;unsigned finished=0,departed=0;
    check(f.bind([&](const CombatSessionActorTransition& event,std::string& error){events.push_back(event);error.clear();return true;}),f.error);
    CombatSessionStateAnimationServices services;
    services.finished=[&](ActorId,std::string& error){++finished;error="fixture normal Post failure";return false;};
    services.departed=[&](ActorId,std::int32_t,std::int32_t,std::string& error){++departed;error.clear();return true;};
    services.checkpoint=[](std::string& error){error.clear();return true;};
    OriginalAttackSelection selection;selection.state=f.source_skill_bank.plan.sequences.front().state;
    const CombatSessionSourceSequencePolicy policy{6,0x6341u,206};
    check(f.session.play_actor_source_sequence(1,f.source_skill_bank.plan,f.source_skill_bank.policies,
        selection,std::move(services),policy,f.error),f.error);
    f.session.actor(1)->action=CharacterAction::casting;
    bool rejected=false;
    for(unsigned frame=0;frame<1200&&!rejected;++frame)
        rejected=!f.session.update(1.0/60,{},actor_position(*f.session.actor(1)),0,f.error);
    check(rejected&&finished==1&&departed==0,"Failed normal Post was not reached exactly once or used interruption callback");
    const auto eventCount=events.size();check(eventCount==4&&events.back().stage==CombatSessionTransitionStage::blur&&
        events.back().cause==CombatRuntimeTransitionCause::completion,
        "Failed normal Post incorrectly reached Focus before reporting its failure");
    check(!f.session.update(1.0/60,{},actor_position(*f.session.actor(1)),0,f.error)&&
          events.size()==eventCount&&finished==1&&departed==0,
          "Failed normal Post replayed its completion receipt or callback");
}

void run_failure_prefix(const char* root){
    Fixture f(root);Recorder recorder{f};recorder.reject_once=true;
    check(f.session.bind_player_locomotion("walk",{"Walk",0,{0}},1.0,true,f.error),f.error);
    check(f.bind([&](const auto& e,std::string& error){return recorder(e,error);}),f.error);
    check(!f.session.select_player_locomotion("walk",f.error)&&
          recorder.events.size()==2&&recorder.events.back().stage==CombatSessionTransitionStage::focus_prefix,
          "Rejected focus prefix was not retained as a reached transition failure");
    const auto count=recorder.events.size();const auto serial=f.session.update_serial();
    check(!f.session.update(0,{},actor_position(*f.session.actor(1)),f.session.actor(1)->transform.rotation[2],f.error)&&
          f.session.update_serial()==serial,"Failed transition prefix allowed a later frame");
    check(!f.session.validate_lifecycle_checkpoint(f.error),"Failed transition prefix admitted a checkpoint");
    check(!f.session.request_actor_attack(1,2,0,f.error)&&recorder.events.size()==count,
          "Failed transition occurrence replayed on retry");
}

std::uint32_t find_double_injury_seed(Fixture& fixture){
    auto formula=fixture.session.world()->combat_properties(2)->sheets.resolved;
    formula[135]=formula[182]=100*256;
    OriginalMeleeDamageProvider provider;
    check(provider.bind_actor(1,*fixture.session.world()->combat_properties(1),fixture.error)&&
          provider.bind_actor(2,*fixture.session.world()->combat_properties(2),fixture.error),fixture.error);
    const auto category=fixture.session.world()->combat_properties(2)->facts.main_damage_class;
    for(std::uint32_t seed=1;seed<50000;++seed){
        dh2::data::CombatRandom rng{seed,0};OriginalMeleeResolution a,b;
        if(!provider.resolve_result(2,1,0x22aab5u,category,-1,0,rng,a,fixture.error,&formula)||
           !provider.resolve_result(2,1,0x22aab5u,category,-1,0,rng,b,fixture.error,&formula))continue;
        if((a.original.outcomes&0x10u)&&(b.original.outcomes&0x10u)&&a.damage>0&&b.damage>0&&
           a.damage<fixture.session.actor(1)->health&&b.damage<fixture.session.actor(1)->health-a.damage)return seed;
    }
    throw std::runtime_error("No deterministic consecutive source Injury seed");
}

void run_incoming_transitions(const char* root){
    Fixture seedFixture(root,true,17);const auto seed=find_double_injury_seed(seedFixture);
    Fixture f(root,true,seed);Recorder recorder{f};
    check(f.bind([&](const auto& event,std::string& error){return recorder(event,error);}),f.error);
    auto formula=f.session.world()->combat_properties(2)->sheets.resolved;formula[135]=formula[182]=100*256;
    const auto category=f.session.world()->combat_properties(2)->facts.main_damage_class;
    CombatSessionSourceHit hit;hit.attacker=2;hit.target=1;hit.binding_lease=f.session.actor_binding_lease();
    hit.generation=1;hit.source_id="accepted-transition-hp-only";hit.marker_name="source-hit";
    hit.mask=0x20080000u;hit.direct_amount=256;
    DamageEvent damage;const float initial=f.session.actor(1)->health;
    check(f.session.apply_source_result(hit,damage,f.error)&&damage.applied&&damage.health_removed>0,f.error);
    check(f.session.actor(1)->health<initial&&f.session.actor(1)->action==CharacterAction::idle&&recorder.events.empty(),
          "HP-only source result incorrectly admitted an Injury transition");

    hit.source_id="accepted-transition-injury";hit.generation=2;hit.mask=0x22aab5u;hit.direct_amount=0;
    hit.category=category;hit.element=0;hit.attacker_formula_sheet=&formula;
    check(f.session.apply_source_result(hit,damage,f.error)&&damage.applied&&damage.source_outcomes&&
          (*damage.source_outcomes&0x10u),f.error);
    const auto afterInjury=recorder.events.size();
    check_triplet(recorder.events,0,3,11,CombatRuntimeTransitionCause::injury);
    auto injuryRng=f.session.world()->random_state();OriginalMeleeDamageProvider provider;
    check(provider.bind_actor(1,*f.session.world()->combat_properties(1),f.error)&&
          provider.bind_actor(2,*f.session.world()->combat_properties(2),f.error),f.error);
    OriginalMeleeResolution expectedSuppressed;
    check(provider.resolve_result(2,1,hit.mask,category,-1,0,injuryRng,expectedSuppressed,f.error,&formula),f.error);
    check((expectedSuppressed.original.outcomes&0x10u)!=0,"Fixture's second source result did not exercise Injury gate suppression");
    ++hit.generation;hit.source_id="accepted-transition-gated-injury";
    check(f.session.apply_source_result(hit,damage,f.error)&&damage.applied&&damage.source_outcomes&&
          (*damage.source_outcomes&0x10u),f.error);
    check(recorder.events.size()==afterInjury&&f.session.actor(1)->action==CharacterAction::hurt,
          "Positive 3000ms Injury gate emitted a second transition or replaced the active reaction");
    for(unsigned frame=0;frame<240;++frame)
        check(f.session.update(1.0/60,{},actor_position(*f.session.actor(1)),0,f.error),f.error);
    f.session.actor(1)->target_id=2;const auto deathStart=recorder.events.size();
    hit.source_id="accepted-transition-death";++hit.generation;hit.mask=0x20080000u;hit.category=-1;
    hit.attacker_formula_sheet=nullptr;hit.direct_amount=static_cast<std::int32_t>(std::ceil(f.session.actor(1)->health*256.0f));
    check(f.session.apply_source_result(hit,damage,f.error)&&damage.target_died,f.error);
    check_triplet(recorder.events,deathStart,3,12,CombatRuntimeTransitionCause::death);
    check(recorder.events.size()==deathStart+3,"Death transition emitted extra stages");
}
}
int main(int argc,char** argv){try{
    check(argc==2,"Original shared asset root required");
    run_attack_and_restore(argv[1]);
    run_static_attack_selection(argv[1]);
    run_source_combo_target_continuity(argv[1]);
    run_actual_positive_attack_delay(argv[1]);
    run_generic_skill_cast_order(argv[1]);
    run_failure_prefix(argv[1]);
    run_incoming_transitions(argv[1]);
    std::cout<<"PASS accepted source transition stages, predecessor-selected Attack/AttackStatic roots, held full-bank sourceCombo target continuity, authored positive AttackDelay departure ordering, generic Skill/Cast normal Post ordering and failure prefix, rejected actions, pose-owned retry, zero-time Move freeze, HP-only and gated Injury, death ordering, restore rebind and failure-prefix no replay\n";
    return 0;
}catch(const std::exception& exception){std::cerr<<"FAIL: "<<exception.what()<<'\n';return 1;}}
