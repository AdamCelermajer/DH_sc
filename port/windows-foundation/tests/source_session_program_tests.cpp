#include "../combat_session.hpp"
#include "../features/skills_animation/skill_animation_program.hpp"
#include "../retained_animation_owner.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cmath>
using namespace dh::foundation;
static void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
static dh2::data::Bytes bytes(const std::vector<std::uint8_t>& value){return {value.data(),value.size()};}
int main(int argc,char** argv){try{
    check(argc==2,"Repository root required");const auto repo=std::filesystem::path(argv[1]);
    AssetCatalog assets(repo/".local-inputs/windows-shared-assets");std::string error;
    OriginalPropertyDatabase database;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    const auto rows=assets.read("original-cache/data/pydata/animations_pyarray.bin");
    const auto names=assets.read("original-cache/data/pydata/animations_pyarraynames.bin");
    const auto fields=assets.read("original-cache/data/pydata/animations_pystructnames.bin");
    const auto values=assets.read("original-cache/data/pydata/animations_dictionary_pyarray.bin");
    std::ifstream inputFile(repo/".local-inputs/actors/animations_dictionary_pyarraynames.bin",std::ios::binary);
    check(bool(inputFile),"Original animation dictionary names absent");
    const std::vector<std::uint8_t> keys{std::istreambuf_iterator<char>(inputFile),{}};
    dh2::data::Dictionary clips;dh2::data::AnimationTables tables;
    check(dh2::data::load_dictionary(bytes(keys),bytes(values),clips,error),error);
    check(dh2::data::load_animation_tables(bytes(rows),bytes(names),bytes(fields),clips,tables,error),error);
    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";
    customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan actorPlan;
    check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"actor-1",actorPlan,error),error);
    auto visualConfig=actorPlan.config;visualConfig.motion_node_id="auto";visualConfig.consume_root_motion=true;
    skills_animation::SkillAnimationPrograms bank;
    check(skills_animation::build_skill_animation_programs(assets,tables,clips,visualConfig,{347,413,521},"actor-1",bank,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=visualConfig;
    CombatSessionProfile profile;profile.initialIdle={"Idle",0,{0}};
    profile.sequenceAction=OriginalAttackSelection{};profile.sequenceAction->state="AttackStatic";
    profile.retainedPhaseClock=true;profile.damageMarkerNames={"attack_mainhand"};profile.propertyOptions={256,true};
    // Source clip bank admission is per actor profile, including NPC profiles.
    profile.sourceAnimationClips=bank.plan.config.clips;
    config.profiles.emplace(config.playerProfileId,profile);
    ActorPopulation population;CharacterVisual player;CombatSession session;
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    const auto* sameOwner=session.retained_actor_pose(1);check(sameOwner,"Session retained owner absent");
    unsigned markerCount=0,completionCount=0,motionCount=0;
    session.set_motion_handler([&](ActorState& actor,Vec3 motion,bool,std::string&){check(actor.id==1,"Motion reached foreign actor");check(std::isfinite(motion.x)&&std::isfinite(motion.y)&&std::isfinite(motion.z),"Nonfinite original skill motion");++motionCount;return true;});
    CombatSessionStateAnimationServices services;
    services.event=[&](ActorId id,const RetainedAnimationEvent& event,std::string&){check(id==1,"Skill marker reached foreign actor");if(event.name=="do_skill")++markerCount;return true;};
    services.finished=[&](ActorId id,std::string& e){++completionCount;return session.select_actor_state_leaf(id,profile.initialIdle,1,false,{},e);};
    for(const auto& program:bank.plan.sequences){
        const auto priorMarkers=markerCount,priorCompletions=completionCount,priorMotion=motionCount;
        OriginalAttackSelection selection;selection.state=program.state;
        check(session.play_actor_source_sequence(1,bank.plan,bank.policies,selection,services,error),error);
        check(session.retained_actor_pose(1)==sameOwner,"Skill selection replaced animation owner");
        for(unsigned frame=0;frame<240&&completionCount==priorCompletions;++frame)
            check(session.update(1.0/60,{},Vec3{0,0,0},0,error),error);
        check(markerCount==priorMarkers+1&&completionCount==priorCompletions+1,"Skill lost authored marker or whole completion");
        check(motionCount>priorMotion&&session.retained_actor_pose(1)==sameOwner,"Skill bypassed motion or replaced retained owner");
        check(session.update(.1,{},Vec3{0,0,0},0,error),error);
        check(markerCount==priorMarkers+1&&completionCount==priorCompletions+1,"Completed skill redelivered callbacks");
        std::cout<<"PASS Session original skill "<<program.name<<" sameOwner/marker/completion/motion\n";
    }
    check(!session.validate_lifecycle_checkpoint(error),"Unpersisted source skill playback admitted durable save");
    check(session.clear_lifecycle_services(error),error);
    auto conflicting=config;conflicting.profiles.at(config.playerProfileId).sourceAnimationClips.emplace_back(actorPlan.config.clips.front().first,"absent-conflicting-resource.bdae");
    check(!session.initialize(assets,database,bindings,conflicting,player,population,{0,0,0},customization,error),"Conflicting source alias accepted");
    check(session.retained_actor_pose(1)==sameOwner,"Failed source clip bank initialization replaced live owner");
    // NPC banks follow the same path, using the NPC's genuine model and clips.
    AssetCatalog sequenceMetadata(assets.root().parent_path()/"windows-melee-bindings");
    check(bindings.load(sequenceMetadata,"original-melee-bindings.xml",error),error);
    config.profiles.at(config.playerProfileId).sequenceAction->group_path={0};
    CombatSessionProfile npc; npc.initialIdle={"Idle",0,{0}};
    npc.sequenceAction=OriginalAttackSelection{};npc.sequenceAction->state="Attack";
    npc.sequenceAction->group_path={0};
    npc.retainedPhaseClock=true;npc.damageMarkerNames={"attack_mainhand"};
    npc.propertyOptions={std::nullopt,true};
    npc.customization.allow_missing_animation_targets=true;npc.motionRoot="auto";
    OriginalCombatVisualPlan npcBank;
    check(build_original_combat_visual_plan(assets,bindings,"Swamp_LizadMan_Type1",npc.customization,"external-npc",npcBank,error),error);
    npc.sourceAnimationClips=npcBank.config.clips;
    config.profiles.emplace("Swamp_LizadMan_Type1",npc);
    PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";placed.definition.stableId=2;
    placed.definition.sourceId="source-bank-test-placement";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    placed.transform=placed.definition.placement;population.actors().push_back(std::move(placed));
    check(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    const auto* npcOwner=session.retained_actor_pose(2);unsigned npcMarkers=0,npcFinished=0;
    CombatSessionStateAnimationServices npcServices;
    npcServices.event=[&](ActorId id,const RetainedAnimationEvent& event,std::string&){check(id==2,"NPC marker reached player");if(event.name=="attack_mainhand")++npcMarkers;return true;};
    npcServices.finished=[&](ActorId id,std::string& e){++npcFinished;return session.select_actor_state_leaf(id,npc.initialIdle,1,false,{},e);};
    OriginalSequencePolicies npcPolicies;check(original_sequence_policies(bindings,npcPolicies,error),error);
    OriginalAttackSelection npcSelection;npcSelection.state="Attack";npcSelection.group_path={0};
    check(session.play_actor_source_sequence(2,npcBank,npcPolicies,npcSelection,npcServices,error),error);
    session.set_motion_handler({});
    for(unsigned frame=0;frame<500&&!npcFinished;++frame)check(session.update(1.0/60,{},Vec3{0,0,0},0,error),error);
    check(npcMarkers==2&&npcFinished==1&&session.retained_actor_pose(2)==npcOwner,"NPC source bank lost own clips/markers/completion/owner");
    std::cout<<"PASS NPC original source bank sameOwner/twoMarkers/wholeCompletion\n";
    session.detach_for_restore();OriginalAttackSelection selection;selection.state=bank.plan.sequences.front().state;
    check(!session.play_actor_source_sequence(1,bank.plan,bank.policies,selection,services,error),"Detached skill session accepted playback");
    std::cout<<"PASS three genuine skill programs through shared live Session; native skill gameplay still unbound\n";
    return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
