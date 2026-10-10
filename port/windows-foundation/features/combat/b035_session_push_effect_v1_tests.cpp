#define main b013_attack_continuity_original_main
#include "b013_attack_continuity_v1_tests.cpp"
#undef main

#include "b035_session_push_effect_v1.hpp"

using namespace dh::foundation::combat;

namespace {
struct PushCase {
    const char* name;
    std::uint32_t exact_outcomes;
    bool suppress_all_status;
    bool suppress_injury;
};

void run_case(const char* root, const PushCase& wanted) {
    AssetCatalog assets(root);
    OriginalPropertyDatabase db;
    OriginalMeleeBindings melee;
    std::string error;
    check(load_original_property_tables(assets,"original-cache/data/pydata",db,error),error);
    check(melee.load(assets,"original-melee-bindings.xml",error),error);

    ActorCustomization custom;
    custom.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan visual_plan;
    check(build_original_combat_visual_plan(assets,melee,"KnightPlayerBase",custom,
        "b035-player",visual_plan,error),error);
    const auto readPy=[&](const char* name){return read(assets,std::string("original-cache/data/pydata/")+name);};
    auto clip_keys=read(assets,"data/animations_dictionary_pyarraynames.bin");
    auto clip_values=readPy("animations_dictionary_pyarray.bin");dh2::data::Dictionary clip_dictionary;
    check(dh2::data::load_dictionary(bytes(clip_keys),bytes(clip_values),clip_dictionary,error),error);
    auto anim_records=readPy("animations_pyarray.bin"),anim_names=readPy("animations_pyarraynames.bin"),anim_fields=readPy("animations_pystructnames.bin");
    dh2::data::AnimationTables source_animations;
    check(dh2::data::load_animation_tables(bytes(anim_records),bytes(anim_names),bytes(anim_fields),clip_dictionary,source_animations,error),error);
    combat::RuntimePlayerProfileAttackBankPlanV1 attack_bank;
    CharacterVisualConfig actor_visual;
    std::vector<std::pair<std::string,std::string>> source_clips;
    prepare_player_attack(assets,"KnightPlayerBase",visual_plan,attack_bank,actor_visual,
        source_clips,false,error);
    ActorProfileLibrary actor_profiles;
    check(actor_profiles.load(assets,"actor-profiles-v2.xml",error),error);
    const auto* player_source_profile=actor_profiles.find("KnightPlayerBase");
    check(player_source_profile,"Push target source visual profile missing");
    auto item_records=readPy("loot_table_pyarray.bin"),item_names=readPy("loot_table_pyarraynames.bin"),item_fields=readPy("loot_table_pystructnames.bin");
    dh2::data::ItemTable source_items;
    check(dh2::data::load_items(bytes(item_records),bytes(item_names),bytes(item_fields),source_items,error),error);
    auto* source_constants=dh2_script_constants_create();check(source_constants,"Source animation constants unavailable");
    auto constants_bytes=read(assets,"data/animations_pycst.bin");dh2_script_constants_reload reload{};
    check(dh2_script_constants_load(source_constants,constants_bytes.data(),
        static_cast<std::uint32_t>(constants_bytes.size()),&reload)==0,"Source constants failed");
    equipment_menu::RuntimePlayerLocomotionConstantsV1 stance_constants;
    check(equipment_menu::load_runtime_player_locomotion_constants_v1(
        [&](const char* group,const char* key,std::int32_t& value,std::string& e){
            if(dh2_script_constants_get(source_constants,group,key,&value)){
                e="Missing source stance constant";return false;}return true;
        },stance_constants,error),error);
    dh2_script_constants_destroy(source_constants);
    const auto main_item=dh2::data::item_id(source_items,"Longsword01");
    check(main_item>=0,"Knight source main-hand item missing");
    equipment_menu::RuntimePlayerLocomotionV1 target_locomotion;
    check(equipment_menu::resolve_runtime_player_locomotion_v1(
        std::stoi(player_source_profile->animation_table),source_items,main_item,-1,0,
        stance_constants,source_animations,clip_dictionary,target_locomotion,error),error);
    SessionPushAnimationBankV1 push_bank;
    check(build_session_push_animation_bank_v1(assets,source_animations,clip_dictionary,
        std::stoi(player_source_profile->animation_table),target_locomotion.stance,
        static_cast<std::uint32_t>(stance_constants.stanced_list_mask),actor_visual,
        "b035-player-push",push_bank,error),error);

    CombatSessionConfig config;
    config.playerId=1;
    config.playerProfileId="KnightPlayerBase";
    config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=actor_visual;
    config.diagnosticRngSeed=1;
    config.mainItemId="Longsword01";
    config.equippedItemIds={"StartingSuit","StartingBoots","StartingGloves","Longsword01"};
    CombatSessionProfile player;
    player.sequenceAction=attack_bank.static_selection;
    player.sourceAttackBank=attack_bank.bank;
    player.sourceAttackPolicies=attack_bank.sequence_policies;
    player.sourceAttackStateSelection=false;
    player.sourceAnimationClips=source_clips;
    player.sourceAnimationClips.insert(player.sourceAnimationClips.end(),
        push_bank.plan.config.clips.begin(),push_bank.plan.config.clips.end());
    player.customization=custom;
    player.initialIdle={"Idle",0,{0}};
    player.damageMarkerNames={"attack_mainhand"};
    player.retainedPhaseClock=true;
    player.propertyOptions={256,true};
    player.reaction=CombatSessionChoice{"Injured",0,{0}};
    player.death=CombatSessionChoice{"Died",0,{0}};
    player.motionRoot="auto";
    config.profiles.emplace("KnightPlayerBase",player);
    CombatSessionProfile enemy;
    enemy.action={"Attack",0,{0,1}};
    enemy.initialIdle={"Idle",0,{0}};
    enemy.damageMarkerNames={"attack_mainhand"};
    enemy.customization=custom;
    enemy.propertyOptions={20*256,true};
    enemy.reaction=CombatSessionChoice{"Injured",0,{0}};
    enemy.death=CombatSessionChoice{"Died",0,{0}};
    enemy.motionRoot="auto";
    config.profiles.emplace("Swamp_LizadMan_Type1",enemy);

    ActorPopulation population;
    PopulationActor placed;
    placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;
    placed.definition.sourceId="b035-lizard";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    placed.transform=placed.definition.placement;
    population.actors().push_back(std::move(placed));
    CharacterVisual visual;
    CombatSession session;
    const auto initialize=[&](){
        check(session.initialize(assets,db,melee,config,visual,population,{0,0,0},custom,error),error);
        session.set_motion_handler([](ActorState& actor,Vec3 delta,bool enabled,std::string&){
            if(enabled){actor.transform.position[0]+=delta.x;actor.transform.position[1]+=delta.y;
                actor.transform.position[2]+=delta.z;}
            return true;
        });
        check(session.update(0,{},Vec3{0,0,0},0,error),error);
    };

    initialize();
    const auto* source=session.world()->combat_properties(2);
    const auto* target=session.world()->combat_properties(1);
    check(source&&target,"Source Lizard/player combatants missing");
    auto formula=source->sheets.resolved;
    if(wanted.suppress_all_status)
        for(const auto id:{135,137,139,142,145,182,183,184,186,188})formula[id]=0;
    if(wanted.suppress_injury)formula[135]=formula[182]=0;

    OriginalMeleeDamageProvider provider;
    check(provider.bind_actor(2,*source,error)&&provider.bind_actor(1,*target,error),error);
    std::uint32_t seed=0;
    OriginalMeleeResolution expected{};
    dh2::data::CombatRandom expected_rng{1,0};
    for(std::uint32_t candidate=1;candidate<50000&&!seed;++candidate){
        auto rng=dh2::data::CombatRandom{candidate,0};
        OriginalMeleeResolution result;
        check(provider.resolve_result(2,1,0x22aab5u,source->facts.main_damage_class,
            -1,0,rng,result,error,&formula),error);
        if(result.damage>0&&result.original.outcomes==wanted.exact_outcomes){
            seed=candidate;expected=result;expected_rng=rng;
        }
    }
    check(seed,"Original Lizard source calculation did not produce exact requested result");

    config.diagnosticRngSeed=seed;
    initialize();
    source=session.world()->combat_properties(2);
    formula=source->sheets.resolved;
    if(wanted.suppress_all_status)
        for(const auto id:{135,137,139,142,145,182,183,184,186,188})formula[id]=0;
    if(wanted.suppress_injury)formula[135]=formula[182]=0;
    check(session.world()->random_state().seed==seed&&session.world()->random_state().calls==0,
        "Session startup consumed combat RNG before the source result");

    CombatSessionSourceHit hit;
    hit.attacker=2;hit.target=1;hit.binding_lease=session.actor_binding_lease();
    hit.generation=1;hit.event_index=0;hit.source_id="b035-"+std::string(wanted.name);
    hit.marker_name="attack_mainhand";hit.mask=0x22aab5u;
    hit.category=source->facts.main_damage_class;hit.element=-1;hit.attacker_formula_sheet=&formula;
    SessionPushAdmissionV1 admission;
    check(capture_session_push_admission_v1(session,hit,admission,error),error);
    check(admission.source_push_gate_admitted&&admission.target_state_before_hit==3,
        "Pre-hit Session Player/Idle gate was not captured");
    SessionPushAdmissionV1 failing_admission;
    check(capture_session_push_admission_v1(session,hit,failing_admission,error),error);
    const auto rng_before_hit=session.world()->random_state();
    DamageEvent receipt;
    check(session.apply_source_result(hit,receipt,error),error);
    check(receipt.applied&&receipt.source_outcomes&&*receipt.source_outcomes==wanted.exact_outcomes
        &&receipt.source_mask&&*receipt.source_mask==expected.original.mask
        &&receipt.requested_damage==expected.damage,
        "CombatSession did not retain the exact source result payload");
    check(session.world()->random_state().seed==expected_rng.seed&&
        session.world()->random_state().calls==expected_rng.calls&&
        session.world()->random_state().calls>=rng_before_hit.calls,
        "Push result consumption changed the source calculation RNG stream");

    unsigned calls=0;
    SessionPushRequestV1 seen;
    bool push_pose_finished=false;
    CombatSessionStateAnimationServices push_pose_services;
    push_pose_services.finished=[&](ActorId actor,std::string&){
        check(actor==1,"Push pose completion changed target actor");
        push_pose_finished=true;return true;
    };
    const SessionPushEffectSinkV1 sink=[&](CombatSession& current,
        const SessionPushRequestV1& request,std::string&){
        check(&current==&session,"Push sink received another Session");
        check(current.actor(request.target)==session.actor(1),"Push target left the same Session");
        std::string pose_error;
        if(!play_session_push_animation_v1(current,request,push_bank,push_pose_services,pose_error))
            throw std::runtime_error(pose_error);
        ++calls;seen=request;return true;
    };
    bool consumed=false;
    const bool should_push=(wanted.exact_outcomes&0x80u)!=0;
    if(should_push){
        auto missing_outcome=receipt;missing_outcome.source_outcomes.reset();
        bool rejected_consumption=false;
        check(!consume_session_push_result_v1(session,admission,missing_outcome,sink,
            rejected_consumption,error)&&!rejected_consumption&&calls==0,
            "Missing original outcome bits were not rejected fail-closed");
        auto missing_mask=receipt;missing_mask.source_mask.reset();
        check(!consume_session_push_result_v1(session,admission,missing_mask,sink,
            rejected_consumption,error)&&!rejected_consumption&&calls==0,
            "Missing original result mask was not rejected fail-closed");
        check(!consume_session_push_result_v1(session,admission,receipt,{},
            rejected_consumption,error)&&!rejected_consumption&&calls==0,
            "Missing admitted Push effect provider was not rejected fail-closed");
        auto wrong_occurrence=receipt;wrong_occurrence.marker_name+="-other";
        check(!consume_session_push_result_v1(session,admission,wrong_occurrence,sink,
            rejected_consumption,error)&&!rejected_consumption&&calls==0,
            "Mismatched source occurrence reached the Push provider");

        const SessionPushEffectSinkV1 failing_sink=[](CombatSession&,
            const SessionPushRequestV1&,std::string&){return false;};
        check(!consume_session_push_result_v1(session,failing_admission,receipt,failing_sink,
            rejected_consumption,error)&&!rejected_consumption&&failing_admission.consumed,
            "Failed Push provider did not fail closed after one-shot admission");
        check(consume_session_push_result_v1(session,failing_admission,receipt,sink,
            rejected_consumption,error)&&!rejected_consumption&&calls==0,
            "Failed Push provider was replayed on duplicate delivery");

        check(consume_session_push_result_v1(session,admission,receipt,sink,consumed,error),error);
        check(consumed&&calls==1,
            "Admitted Push outcome did not reach its same-Session effect provider");
        check(session.actor(1)->action==CharacterAction::knocked_back&&session.original_actor_state(1)==10&&
            session.owns_pose(1),
            "Push did not retain same-Session ownership of its authored KnockedBack10 pose");
        check(seen.attacker==hit.attacker&&seen.target==hit.target&&seen.source_id==hit.source_id
            &&seen.marker_name==hit.marker_name&&seen.source_outcomes==wanted.exact_outcomes
            &&seen.source_mask==expected.original.mask&&seen.target_state_before_hit==3
            &&seen.great==((expected.original.mask&0x00100000u)!=0)
            &&seen.direct==((expected.original.mask&0x18000000u)!=0),
            "Push sink request changed source identities, result mask or state-order facts");
        bool duplicate=false;
        check(consume_session_push_result_v1(session,admission,receipt,sink,duplicate,error),error);
        check(!duplicate&&calls==1,"Duplicate Push result replayed the effect sink");
        const auto starting_position=session.actor(1)->transform.position;
        for(unsigned frame=0;frame<240&&!push_pose_finished;++frame){
            if(!session.update(1.0/60.0,{},Vec3{0,0,0},0,error))
                throw std::runtime_error("Push pose update frame "+std::to_string(frame)+": "+error);
        }
        check(push_pose_finished,"Authored same-Session knockback clip did not complete");
        check(session.actor(1)->action==CharacterAction::idle,
            "Completed Push pose did not return the live Session action to idle");
        const auto final_position=session.actor(1)->transform.position;
        const float moved=std::abs(final_position[0]-starting_position[0])+
            std::abs(final_position[1]-starting_position[1])+std::abs(final_position[2]-starting_position[2]);
        check(moved>0.001f,"Authored Push MoveGO clip produced no body displacement");
        std::cout<<"  knockback-pose=KnockedBack moved-scene-units="<<moved<<"\n";
    }else{
        check(consume_session_push_result_v1(session,admission,receipt,sink,consumed,error),error);
        check(!consumed&&calls==0,
            "Ordinary source result incorrectly reached the Push effect provider");
    }
    check(session.world()->random_state().seed==expected_rng.seed&&
        session.world()->random_state().calls==expected_rng.calls,
        "Consuming the Push result changed the same-Session source RNG");
    std::cout<<wanted.name<<" source-outcomes=0x"<<std::hex<<wanted.exact_outcomes
        <<" source-mask=0x"<<expected.original.mask<<std::dec<<" seed="<<seed
        <<" sink-calls="<<calls<<" consumed="<<consumed<<'\n';
}
}

int main(int argc,char** argv){try{
    check(argc==2,"Supply unified original assets root");
    run_case(argv[1],{"ordinary",0x8u,true,false});
    run_case(argv[1],{"natural-lizard-push-injury-critical",0x98u,false,false});
    run_case(argv[1],{"controlled-push-critical",0x88u,false,true});
    std::cout<<"PASS B035 exact source-result controls: ordinary 0x8 no-op; natural 0x98 and controlled 0x88 dispatch once through the captured same-Session pre-hit Player/Idle gate; no RNG or duplicate replay.\n";
    return 0;
}catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<'\n';return 1;}}
