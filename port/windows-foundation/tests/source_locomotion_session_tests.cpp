#include "../combat_session.hpp"
#include "../features/equipment/runtime_player_locomotion_program_v1.hpp"
#include "../retained_pose_playback.hpp"
#include "../content_paths.hpp"
#include "../../script-runtime/script_constants.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::equipment_menu;
void require(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}
int main(int argc,char** argv){try{
    require(argc==2,"Supply unified original asset root");AssetCatalog assets(argv[1]);std::string error;
    OriginalPropertyDatabase properties;OriginalMeleeBindings bindings;
    require(load_original_property_tables(assets,"original-cache/data/pydata",properties,error),error);
    require(bindings.load(assets,"original-melee-bindings.xml",error),error);
    const auto read=[&](const char* name){return read_content(assets,std::string("data/pydata/")+name);};
    const auto bytes=[](const std::vector<std::uint8_t>& data){return dh2::data::Bytes{data.data(),data.size()};};
    auto clipNames=read("animations_dictionary_pyarraynames.bin"),clipValues=read("animations_dictionary_pyarray.bin");
    dh2::data::Dictionary dictionary;require(dh2::data::load_dictionary(bytes(clipNames),bytes(clipValues),dictionary,error),error);
    auto records=read("animations_pyarray.bin"),names=read("animations_pyarraynames.bin"),fields=read("animations_pystructnames.bin");
    dh2::data::AnimationTables animations;require(dh2::data::load_animation_tables(bytes(records),bytes(names),bytes(fields),dictionary,animations,error),error);
    records=read("loot_table_pyarray.bin");names=read("loot_table_pyarraynames.bin");fields=read("loot_table_pystructnames.bin");
    dh2::data::ItemTable items;require(dh2::data::load_items(bytes(records),bytes(names),bytes(fields),items,error),error);
    const auto constantsBytes=read("animations_pycst.bin");
    auto* constants=dh2_script_constants_create();require(constants!=nullptr,"Constants allocation");
    dh2_script_constants_reload reload{};
    require(dh2_script_constants_load(constants,constantsBytes.data(),std::uint32_t(constantsBytes.size()),&reload)==0,"Constants load");
    RuntimePlayerLocomotionConstantsV1 policies;
    require(load_runtime_player_locomotion_constants_v1([&](const char* group,const char* key,std::int32_t& value,std::string& message){
        if(dh2_script_constants_get(constants,group,key,&value)!=0){message="Missing original stance constant";return false;}return true;
    },policies,error),error);
    dh2_script_constants_destroy(constants);
    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan initial;require(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",customization,"source-session-test",initial,error),error);
    std::vector<RuntimePlayerLocomotionProgramV1> programs;
    std::vector<std::int32_t> stanceIds;
    const auto id=[&](const char* name){const auto result=dh2::data::item_id(items,name);require(result>=0,std::string("Missing original item ")+name);return result;};
    // The source table ID is projected from the loaded original profile, not a
    // class/weapon branch in the runtime. Profiles expose it independently.
    ActorProfileLibrary profiles;require(profiles.load(assets,"actor-profiles-v2.xml",error),error);
    const auto* profile=profiles.find("KnightPlayerBase");require(profile!=nullptr,"Missing source visual profile");
    const std::pair<std::int32_t,std::int32_t> equipment[]={{id("Longsword01"),-1},{id("Longsword01"),id("Dagger01")},{id("Staff01"),-1}};
    CombatSessionConfig config;config.playerId=1;config.playerProfileId="KnightPlayerBase";config.diagnosticRngSeed=1234;config.tableRoot="original-cache/data/pydata";
    config.playerVisualConfig=initial.config;config.playerVisualConfig.consume_root_motion=true;config.playerVisualConfig.motion_node_id="auto";
    for(std::size_t index=0;index<3;++index){
        RuntimePlayerLocomotionV1 projection;
        require(resolve_runtime_player_locomotion_v1(std::stoi(profile->animation_table),items,equipment[index].first,equipment[index].second,0,policies,animations,dictionary,projection,error),error);
        RuntimePlayerLocomotionProgramV1 program;
        require(build_runtime_player_locomotion_program_v1(assets,projection,animations,dictionary,initial.config,"source-stance-"+std::to_string(index),program,error),error);
        for(const auto& clip:program.named_clips)config.playerVisualConfig.clips.emplace_back(clip.named_alias,clip.resolved_path);
        programs.push_back(std::move(program));
        stanceIds.push_back(projection.stance);
    }
    CombatSessionProfile player;player.retainedPhaseClock=true;player.initialIdle={"Idle",0,{0}};player.damageMarkerNames={"attack_mainhand"};player.propertyOptions={256,true};
    OriginalAttackSelection attack;attack.state="AttackStatic";player.sequenceAction=attack;config.profiles.emplace(config.playerProfileId,player);
    config.mainItemId="Longsword01";config.equippedItemIds={config.mainItemId};
    CharacterVisual visual;ActorPopulation population;CombatSession session;
    require(session.initialize(assets,properties,bindings,config,visual,population,{0,0,0},customization,error),error);
    const auto lease=session.actor_binding_lease();const auto* owner=session.retained_player_pose();
    std::vector<std::string> idleClips;
    for(std::size_t index=0;index<programs.size();++index)for(const char* state: {"Idle","Walk","Run"}){
        const auto* sequence=programs[index].plan.sequence(state,0);require(sequence&&!sequence->phases.empty(),"Source state has no actual leaf");
        const auto& phase=sequence->phases.front();const std::string alias=std::string(state)+std::to_string(index);
        require(session.bind_actor_locomotion_from_bank(1,alias,programs[index].plan,{state,0,phase.sourcePath},1,sequence->loop!=0,error),error);
        require(session.select_actor_locomotion(1,alias,error),error);
        InputActions input;if(std::string(state)!="Idle")input.move2D={0,1};
        require(session.update(.016,input,{0,0,0},0,error),error);
        const auto* pose=session.retained_player_pose();const auto slot=pose->current_slot();
        require(pose==owner&&!lease.expired()&&pose->slots()[slot].clip_id==phase.clipName,"Source stance replaced actor/pose or selected wrong clip");
        const auto before=pose->slots()[slot].timeline.current_ms;
        require(session.update(.032,input,{0,0,0},0,error),error);
        require(pose->slots()[pose->current_slot()].timeline.current_ms>before,"Source stance clock did not advance");
        if(std::string(state)=="Idle")idleClips.push_back(phase.resolvedPath);
        std::cout<<"PASS stance="<<stanceIds[index]<<" state="<<state<<" sequence="<<sequence->id<<" clip="<<phase.resolvedPath<<" sourceSpeed="<<phase.speed<<" sameOwner=1\n";
    }
    require(stanceIds==std::vector<std::int32_t>{0,2,3}&&idleClips.size()==3&&idleClips[0]!=idleClips[1],"Original ordinary/dual/staff source stance identities were not selected");
    // Production movement keeps the logical alias stable when equipment
    // changes. Exercise replacement on the already-running same pose owner.
    for(std::size_t index: {std::size_t(0),std::size_t(1),std::size_t(0)}){
        const auto* sequence=programs[index].plan.sequence("Idle",0);
        const auto& phase=sequence->phases.front();
        require(session.bind_actor_locomotion_from_bank(1,"idle",programs[index].plan,{"Idle",0,phase.sourcePath},1,true,error),error);
        require(session.select_actor_locomotion(1,"idle",error),error);
        InputActions idle;
        require(session.update(.016,idle,{0,0,0},0,error),error);
        const auto* pose=session.retained_player_pose();
        const auto slot=pose->current_slot();const auto time=pose->slots()[slot].timeline.current_ms;
        require(pose==owner&&!lease.expired()&&pose->slots()[slot].clip_id==phase.clipName,
                "Replacing logical locomotion alias lost current original stance/owner");
        require(session.select_actor_locomotion(1,"idle",error),error);
        require(session.update(.032,idle,{0,0,0},0,error),error);
        require(pose->slots()[pose->current_slot()].timeline.current_ms>time,
                "Repeated logical locomotion selection reset the source clock");
        std::cout<<"PASS equipment stance switch="<<stanceIds[index]<<" alias=idle sameOwner=1 clockContinues=1\n";
    }
    std::cout<<"PASS original ordinary/dual/staff Idle/Walk/Run through one actual CombatSession\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
