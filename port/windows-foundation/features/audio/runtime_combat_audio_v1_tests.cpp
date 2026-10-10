#include "runtime_combat_audio_v1.hpp"
#include "runtime_attack_sound_v1.hpp"
#include "runtime_session_audio_v1.hpp"
#include "audio_source_target_position_v1.hpp"
#include "feature_audio.hpp"
#include "windows_source_session_control_v1.hpp"
#include "../../original_combat_visual_plan.hpp"
#include "../../original_attack_sequence.hpp"
#include "../../original_actor_target_position.hpp"
#include "../../../level-world/character_combat_sound_tables_v2.hpp"
#include "../../../engine-audio/integration-v40/runtime/default-source-fields/default_source_fields_v40.hpp"
#include "../../../engine-audio/audio_world_producer_v38.hpp"
#include "../../../game-data/animation_tables.hpp"
#include <chrono>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <stdexcept>
#include <sstream>
#include <thread>

using namespace dh::foundation;
using namespace dh::foundation::audio;
using namespace dh2::audio;

static void check(bool ok,const std::string& why){if(!ok)throw std::runtime_error(why);}
static std::vector<std::uint8_t> read_bytes(const std::filesystem::path& path){
    std::ifstream input(path,std::ios::binary);
    if(!input)throw std::runtime_error("Cannot read actual source CharSounds table: "+path.string());
    return {std::istreambuf_iterator<char>(input),{}};
}
static void test_audio_source_target_position(){
    ActorState actor;actor.id=1;actor.target_id=2;
    actor.transform.position={12.0f,-4.0f,7.5f};actor.source_flags520=0x1234u;
    original_target_position_ctor_prefix(actor.source_target_node180,
                                         actor.source_target_position184);
    check(actor.source_target_node180&&*actor.source_target_node180==0&&
          actor.source_target_position184&&*actor.source_target_position184==
              std::array<float,3>{0,0,0},
          "Original GameObject constructor did not retain known-null node and zero point fixture");
    check(audio_source_target_position_v1(actor)==actor.transform.position,
          "Known-null own target node let the stale constructor zero cache replace current transform");
    check(actor.source_flags520&&*actor.source_flags520==0x1234u,
          "Audio target-position resolution mutated original source flags");
    actor.source_target_node180=std::uintptr_t(0x1234);
    actor.source_target_position184=std::array<float,3>{3.0f,8.0f,-2.0f};
    check(audio_source_target_position_v1(actor)==*actor.source_target_position184,
          "Known nonnull own node did not select its valid cached target point");
    actor.source_target_node180.reset();
    actor.source_target_position184=std::array<float,3>{0,0,0};
    check(audio_source_target_position_v1(actor)==actor.transform.position,
          "Unbound node borrowed a cached point instead of the current actor transform");
}

struct SourceFixture {bool block_gate=false;unsigned plays=0,random_draws=0;CombatSession* session{};std::vector<int> sound_ids,sound_uids;};
static int source_gate(void* raw,const dh2::sound::VoxPlay3DRequestV2& request,
                       dh2::sound::VoxPlay3DResponseV2& response){
    auto& fixture=*static_cast<SourceFixture*>(raw);
    using O=dh2::sound::VoxPlay3DOperationV2;
    if(fixture.block_gate){return -1;}
    if(request.operation==O::disabled)response.value=0;
    else if(request.operation==O::current_level){response.identity=0x38;response.value=38;}
    else if(request.operation==O::online||request.operation==O::platform_route||request.operation==O::trace)response.value=0;
    else return -1; // sound-row/bank-info/emit are the actual V42 owner
    return 0;
}
static bool shared_source_random(void* raw,int& value) {
    auto& fixture=*static_cast<SourceFixture*>(raw);
    if(!fixture.session||!fixture.session->world())return false;
    std::uint32_t draw{};std::string error;
    if(!fixture.session->world()->random_uniform(0x7fffffffU,draw,error))return false;
    value=static_cast<int>(draw);++fixture.random_draws;return true;
}
static bool source_command(void* raw,const dh2::character::CombatSoundPlayV1& play,
    const AudioSoundV34& sound,const AudioGroupV34& group,AudioCommandV34& command,std::string&){
    auto& fixture=*static_cast<SourceFixture*>(raw);++fixture.plays;fixture.sound_ids.push_back(play.sound_id);fixture.sound_uids.push_back(sound.uid);
    command.left=command.right=original_fresh_emitter_gain_v40();
    command.pitch=original_fresh_emitter_pitch_v40();command.volume_group=group.volume_group;
    command.source_emitter_position=play.position;return true;
}

int main(int argc,char** argv){try{
    check(argc==4,"Supply shared original assets, feature audio assets, and actual sounds_pyarray.bin");
    test_audio_source_target_position();
    const std::filesystem::path shared=argv[1],audio_assets=argv[2];
    AudioFilesystem files{audio_assets.string()};SourceFixture source_fixture;
    AudioGameplaySourcesV40 sources;sources.context=&source_fixture;
    sources.exact_assets={&files,AudioFilesystem::read,{}};
    sources.random={&source_fixture,shared_source_random};
    sources.gates={&source_fixture,source_gate};sources.source_command=source_command;
    auto provider=std::make_shared<unsigned>(7);AudioLifecycleGateV40 output_gate;
    check(output_gate.publish_activity(1,1,true,true,true,false),"Could not publish focused output lifecycle fixture");
    std::string error;
    AudioNativeSessionV42 output(reinterpret_cast<std::uintptr_t>(&files),sources,provider,
        output_gate,windows_source_session_control_v1());
    check(output.initialize(error),"Actual single V42/WinMM startup failed: "+error);
    struct Cleanup {AudioNativeSessionV42& output;bool done=false;~Cleanup(){if(!done){std::string ignored;output.shutdown(ignored);}}} cleanup{output};
    auto* runtime=output.runtime_on_producer();check(runtime&&runtime->source_data_initialized(),"Same initialized V42 runtime missing");
    const auto clock_now=[&](){AudioDeviceClockV40 device;check(runtime->clock().snapshot(device)&&device.ready&&
        device.generation&&device.position>=0&&device.monotonic_ns>0,"No actual published focused WinMM device sample/QPC pair");
        return RetainedFrameAudioClock{device.generation,device.position,device.monotonic_ns,device.ready};};
    const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(!output.ready_for_current_source()&&std::chrono::steady_clock::now()<deadline)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    check(output.ready_for_current_source(),"Single WinMM control did not publish a focused source epoch");

    dh2::character::CharacterCombatSoundTablesV2 tables;
    check(tables.load(read_bytes(argv[3]),error),"Actual CharSounds parse failed: "+error);
    check(tables.size()>2&&tables.get(-1)==tables.get(2)&&
          tables.get(static_cast<std::int32_t>(tables.size()))==tables.get(2),
          "Original invalid CharSounds IDs did not fall back to row 2");
    std::int32_t missing_audio_row=-1,missing_ordinal=-1;
    std::uint32_t missing_audio_index=0;
    std::string missing_hit_filename;std::uint32_t death_list_count=0,death_ordinal=0;
    std::int32_t fallback_missing_audio_row=-1;
    for(std::int32_t row_id=0;row_id<static_cast<std::int32_t>(tables.size());++row_id){
        const auto* row=tables.get(row_id);if(!row)continue;
        for(std::uint32_t i=0;i<row->hit.count;++i){
            const auto ordinal=row->hit.ids[i];const auto* binding=runtime->bindings().row(ordinal);
            const auto* sound=binding&&!binding->event?runtime->catalog().sound(binding->uid):nullptr;
            if(!sound)continue;
            std::shared_ptr<const std::vector<std::uint8_t>> sample;std::string read_error;
            const auto uri="data/sounds/"+sound->filename;
            if(!files.read(&files,uri.c_str(),sample,read_error)||!sample){
                if(fallback_missing_audio_row<0){
                    fallback_missing_audio_row=row_id;missing_audio_index=i;missing_ordinal=ordinal;
                    missing_hit_filename=sound->filename;
                }
                if(row->death.count&&row->death.count!=row->hit.count){
                    missing_audio_row=row_id;missing_audio_index=i;missing_ordinal=ordinal;
                    missing_hit_filename=sound->filename;death_list_count=row->death.count;
                    death_ordinal=row->death.ids[0];break;
                }
            }
        }
        if(missing_audio_row>=0)break;
    }
    if(missing_audio_row<0)missing_audio_row=fallback_missing_audio_row;
    if(missing_audio_row>=0&&!death_list_count){
        const auto* selected=tables.get(missing_audio_row);
        if(selected&&selected->death.count&&selected->death.count!=selected->hit.count){
            death_list_count=selected->death.count;death_ordinal=selected->death.ids[0];
        }
    }
    check(missing_audio_row>=0,"Actual CharSounds table contains no hit-list reference with an absent selected source sample");
    AssetCatalog assets(shared);OriginalPropertyDatabase database;OriginalMeleeBindings bindings;
    check(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    check(bindings.load(assets,"original-melee-bindings.xml",error),error);
    dh2::data::ItemTable item_table;
    const auto item_records=assets.read("original-cache/data/pydata/loot_table_pyarray.bin");
    const auto item_names=assets.read("original-cache/data/pydata/loot_table_pyarraynames.bin");
    const auto item_fields=assets.read("original-cache/data/pydata/loot_table_pystructnames.bin");
    check(dh2::data::load_items({item_records.data(),item_records.size()},
          {item_names.data(),item_names.size()},{item_fields.data(),item_fields.size()},item_table,error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    config.mainItemId="Longsword01";config.equippedItemIds={config.mainItemId};
    check(dh2::data::item(item_table,dh2::data::item_id(item_table,config.mainItemId))!=nullptr,
          "Actual source Longsword ItemTable row is absent");
    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";
    customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    check(build_original_combat_visual_plan(assets,bindings,config.playerProfileId,customization,
        "runtime-combat-audio-player",plan,error),error);
    config.playerVisualConfig=plan.config;config.playerVisualConfig.clips.clear();
    config.playerVisualConfig.motion_node_id="auto";config.playerVisualConfig.consume_root_motion=true;
    config.playerVisualConfig.clips={{"idle",plan.phase("Idle",0,{0})->resolvedPath}};
    CombatSessionProfile knight;knight.action={"AttackStatic",0,{0,1}};knight.initialIdle={"Idle",0,{0}};
    knight.damageMarkerNames={"attack_mainhand"};knight.propertyOptions={256,true};
    knight.retainedPhaseClock=true;knight.sourceCombo=true;config.profiles.emplace(config.playerProfileId,knight);
    CombatSessionProfile lizard;lizard.action={"Attack",0,{0,1}};lizard.initialIdle={"Idle",0,{0}};
    lizard.reaction=CombatSessionChoice{"Injured",0,{0}};
    lizard.death=CombatSessionChoice{"Died",0,{0}};
    lizard.damageMarkerNames={"attack_mainhand"};lizard.propertyOptions={std::nullopt,true};
    lizard.customization.allow_missing_animation_targets=true;lizard.retainedPhaseClock=true;
    lizard.motionRoot="auto";
    config.profiles.emplace("Swamp_LizadMan_Type1",lizard);
    AssetCatalog sequence_metadata(shared.parent_path()/"windows-melee-bindings");
    check(bindings.load(sequence_metadata,"original-melee-bindings.xml",error),error);
    OriginalAttackSelection player_sequence;player_sequence.state="AttackStatic";player_sequence.variant=0;
    config.profiles.at(config.playerProfileId).sequenceAction=player_sequence;
    OriginalAttackSelection npc_sequence;npc_sequence.state="Attack";npc_sequence.variant=0;npc_sequence.group_path={0};
    config.profiles.at("Swamp_LizadMan_Type1").sequenceAction=npc_sequence;
    ActorPopulation population;PopulationActor npc;npc.profileId="Swamp_LizadMan_Type1";
    npc.definition.stableId=2;npc.definition.sourceId="runtime-combat-audio-fixture";
    npc.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    npc.transform={2,0,0,0,0,2,0,0,0,0,2,0,0,-100,0,1};population.actors().push_back(std::move(npc));
    CharacterVisual player;CombatSession combat;
    check(combat.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
    source_fixture.session=&combat;

    // Load the actual typed animation source, then bind the additive step
    // observer beside the existing retained hit observer below.
    const auto clip_names=read_bytes(shared/"data/animations_dictionary_pyarraynames.bin");
    const auto clip_values=read_bytes(shared/"data/animations_dictionary_pyarray.bin");
    const auto animation_records=read_bytes(shared/"original-cache/data/pydata/animations_pyarray.bin");
    const auto animation_names=read_bytes(shared/"original-cache/data/pydata/animations_pyarraynames.bin");
    const auto animation_fields=read_bytes(shared/"original-cache/data/pydata/animations_pystructnames.bin");
    dh2::data::Dictionary clip_dictionary;dh2::data::AnimationTables animations;
    check(dh2::data::load_dictionary({clip_names.data(),clip_names.size()},
          {clip_values.data(),clip_values.size()},clip_dictionary,error),error);
    check(dh2::data::load_animation_tables({animation_records.data(),animation_records.size()},
          {animation_names.data(),animation_names.size()},
          {animation_fields.data(),animation_fields.size()},clip_dictionary,animations,error),error);
    for(const auto sequence_id:{470,471,472}) {
        check(static_cast<std::size_t>(sequence_id)<animations.sequences.size()&&
              animations.sequences[sequence_id].steps.size()>1&&
              animations.sequences[sequence_id].steps[1].sound==289&&
              animations.sequences[sequence_id].steps[1].swoosh,
              "Actual Human_AttackStatic combo strike step lost Sound289/Swoosh source fields");
    }
    // Source-backed row checks pair with a genuine injury occurrence later in
    // this same session. The live Swamp Lizard source reaction is row377.
    check(animations.sequence_names.size()>377&&
          animations.sequence_names[377]=="LizardMan_Injured"&&
          !animations.sequences[377].steps.empty()&&
          animations.sequences[377].steps[0].sound==477&&
          !animations.sequences[377].steps[0].swoosh,
          "Actual LizardMan_Injured AnimTable row377 step0 Sound477 changed");
    check(animations.sequence_names.size()>374&&
          animations.sequence_names[374]=="LizardMan_Died"&&
          !animations.sequences[374].steps.empty()&&
          animations.sequences[374].steps[0].sound==476&&
          !animations.sequences[374].steps[0].swoosh,
          "Actual LizardMan_Died AnimTable row374 step0 Sound476 changed");
    const auto* injury_binding=runtime->bindings().row(477);
    check(injury_binding&&!injury_binding->event&&injury_binding->uid==284,
          "Actual source Sound477 binding no longer resolves to sample UID284");
    const auto* injury_sound=runtime->catalog().sound(284);
    check(injury_sound&&injury_sound->filename=="sfx_lizardman_hurt.wav",
          "Actual UID284 source sound metadata changed");
    std::shared_ptr<const std::vector<std::uint8_t>> injury_sample;
    std::string injury_asset_error;
    const auto injury_uri="data/sounds/"+injury_sound->filename;
    check(!files.read(&files,injury_uri.c_str(),injury_sample,injury_asset_error)&&!injury_sample,
          "Exact recovered lizard hurt WAV unexpectedly appeared; update the missing-asset expectation");
    const auto injury_clock=clock_now();
    check(injury_clock.valid()&&injury_clock.qpc_monotonic_ns>0,
          "Source-row coverage lacks a real paired WinMM sample/QPC snapshot");
    const auto injury_position=combat.actor(2)->transform.position;
    const auto injury_request=dh2::audio::audio_world_request_v38(runtime->manager(),
        static_cast<std::uintptr_t>(ActorId(2)),animations.sequences[377].steps[0].sound,
        injury_position);
    const auto injury_health_before=combat.actor(2)->health;
    const auto injury_event_count_before=combat.events().size();
    const auto injury_rng_before=combat.world()->random_state();
    check(!output.submit_actual_play(injury_request,injury_clock.qpc_monotonic_ns,
                                     injury_asset_error)&&
          injury_asset_error.find("Unavailable original audio asset: "+injury_uri)!=std::string::npos,
          "Exact source-row sound did not fail diagnostically on its unavailable original WAV");
    const auto injury_rng_after=combat.world()->random_state();
    check(combat.actor(2)->health==injury_health_before&&
          combat.events().size()==injury_event_count_before&&
          injury_rng_after.seed==injury_rng_before.seed&&
          injury_rng_after.calls==injury_rng_before.calls,
          "Missing hurt sample changed Session gameplay state during source-row component coverage");

    // Exercise the production default against both source-backed actors before
    // the failure fixture installs its explicit row override below.
    std::array<std::int32_t,2> resolved_sound_rows{};
    std::size_t sound_actor_index=0;
    for(const ActorId actor_id:{ActorId(1),ActorId(2)}){
        std::int32_t sound_id=-1;
        check(runtime_combat_audio_char_sound_id_v1(combat,actor_id,sound_id,error),
              "Default same-session CharSounds field-8 resolver failed: "+error);
        const auto* source_properties=combat.world()->combat_properties(actor_id);
        check(source_properties&&sound_id==source_properties->sheets.resolved[8],
              "Default CharSounds resolver did not use same actor's resolved Sounds field 8");
        check(tables.get(sound_id)!=nullptr,
              "Real player/NPC Sounds property did not resolve through source CharSounds table fallback");
        resolved_sound_rows[sound_actor_index++]=sound_id;
    }

    RuntimeCombatAudioServicesV1 audio_services;
    audio_services.cached_char_sound_id=[&](ActorId,std::int32_t& row,std::string&){
        row=missing_audio_row;return true; // Explicit missing-sample fixture override only.
    };
    std::vector<std::uint32_t> random_list_counts;
    audio_services.minimal_randoms=[](std::uint32_t& value,std::string&){value=0;return true;};
    audio_services.random=[&](std::uint32_t count,std::uint32_t& index,std::string&){
        random_list_counts.push_back(count);
        index=count==death_list_count&&death_list_count?0:
               count==tables.get(missing_audio_row)->hit.count?missing_audio_index:0;
        return index<count;
    };
    std::vector<RuntimeCombatAudioDiagnosticV1> diagnostics;
    std::vector<RuntimeAttackSoundDiagnosticV1> attack_sound_diagnostics;
    std::vector<std::int32_t> step_submit_ids;
    std::vector<std::int64_t> step_submit_times;
    std::vector<CombatSessionStepEntry> lizard_reaction_entries;
    RuntimeAttackSoundV1 attack_audio(
        [&](ActorId actor,std::int32_t sound,const std::array<float,3>& position,
            std::int64_t qpc,std::string& submit_error) {
            step_submit_ids.push_back(sound);step_submit_times.push_back(qpc);
            return output.submit_actual_play(dh2::audio::audio_world_request_v38(
                runtime->manager(),static_cast<std::uintptr_t>(actor),sound,position),qpc,submit_error);
        },[&]{return output.ready_for_current_source();},combat,animations,item_table,
        [&](const RuntimeAttackSoundDiagnosticV1& item){attack_sound_diagnostics.push_back(item);});
    const auto attack_step_observer=attack_audio.step_entry_observer();
    std::optional<CombatSessionStepEntry> captured_combo_step;
    std::ostringstream presentation_log;StepEntryPresentationObserversV1 presentation_registry;
    std::vector<std::string> presentation_order;std::size_t presentation_tail_calls=0;
    std::string registration_error;
    check(presentation_registry.add(
        [&](const CombatSessionStepEntry&){presentation_order.push_back("fx-first");},registration_error),registration_error);
    check(presentation_registry.add(
        [](const CombatSessionStepEntry&){throw std::runtime_error("intentional step presentation diagnostic");},registration_error),registration_error);
    check(presentation_registry.add(
        [&](const CombatSessionStepEntry&){presentation_order.push_back("fx-last");++presentation_tail_calls;},registration_error),registration_error);
    const auto composed_step_observer=presentation_registry.compose(
        [&](const CombatSessionStepEntry& item){
        presentation_order.push_back("audio-first");
        if(item.sequence_id==470&&item.step==1&&!captured_combo_step)captured_combo_step=item;
        if(item.actor==2&&(item.sequence_id==377||item.sequence_id==374))
            lizard_reaction_entries.push_back(item);
        attack_step_observer(item);
    },[&](const std::string& message){presentation_log<<message<<'\n';});
    combat.set_step_entry_observer(composed_step_observer);
    RetainedFrameAudioObserver base_observer;
    const RetainedAnimationEvent* active_event=nullptr;
    ActorId active_actor=invalid_actor_id;std::uint32_t active_event_index=0;
    RetainedFrameAudioClock active_clock{};
    std::vector<std::string> attack_occurrence_trace;
    std::map<std::string,std::uint64_t> zero_result_updates;
    bool identical_zero_result_across_updates=false,current_zero_result=false;
    std::size_t zero_hit_adapter_dispatches=0;
    bool hit_qpc_replay_attempted=false;
    RetainedFrameAudioObserverStatus hit_qpc_replay_status=RetainedFrameAudioObserverStatus::dispatched;
    audio_services.diagnostic=[&](const auto& item){
        diagnostics.push_back(item);
        if(current_zero_result&&item.event_name=="attack_mainhand"&&item.target!=invalid_actor_id)
            ++zero_hit_adapter_dispatches;
        if(!hit_qpc_replay_attempted&&active_event&&item.event_name=="attack_mainhand"&&base_observer){
            hit_qpc_replay_attempted=true;auto changed_qpc=active_clock;
            changed_qpc.qpc_monotonic_ns+=1000000;std::string duplicate_detail;
            hit_qpc_replay_status=base_observer(active_actor,*active_event,active_event_index,
                                                &changed_qpc,duplicate_detail);
        }
    };
    RuntimeCombatAudioV1 adapter(output,combat,tables,std::move(audio_services));
    const auto clock=clock_now();base_observer=adapter.retained_event_observer();auto observer=base_observer;
    InputActions observer_fixture_input;
    check(combat.update(0,observer_fixture_input,{0,0,0},0,error,&clock),error);
    const auto tail_calls_before_rebind=presentation_tail_calls;
    combat.set_step_entry_observer({});
    combat.set_step_entry_observer(composed_step_observer);
    RetainedAnimationEvent marker{"sfx_WeaponSwoosh1","retained-sfx-fixture",0,1000,9,0};
    std::string detail;
    const auto marker_status=observer(2,marker,3,&clock,detail);
    check(marker_status==RetainedFrameAudioObserverStatus::dispatched,
          "Authored named sound did not reach same runtime/output: "+detail);
    check(source_fixture.plays==1,"Named authored marker did not submit exactly one source command");
    auto changed_qpc_clock=clock;changed_qpc_clock.qpc_monotonic_ns+=1000000;
    const auto duplicate=observer(2,marker,3,&changed_qpc_clock,detail);
    check(duplicate==RetainedFrameAudioObserverStatus::not_applicable&&source_fixture.plays==1,
          "A repeated exact retained occurrence with a changed QPC replayed audio");
    source_fixture.block_gate=true;const auto rejected=observer(2,marker,4,&clock,detail);
    check(rejected==RetainedFrameAudioObserverStatus::required_owner_unavailable,
          "Missing required World/GS Play3D owner was not distinct from asset failure");
    source_fixture.block_gate=false;
    combat.set_retained_frame_audio_observer([&](ActorId actor,const RetainedAnimationEvent& event,
        std::uint32_t ordinal,const RetainedFrameAudioClock* paired_clock,std::string& observer_error){
        current_zero_result=false;
        if(event.name=="attack_mainhand")attack_occurrence_trace.push_back(
            std::to_string(actor)+"/"+std::to_string(event.generation)+"/"+event.clip_id+"/"+
            std::to_string(event.slot)+"/"+std::to_string(event.wall_timestamp_ms)+"/"+
            std::to_string(event.lag_ms)+"/"+std::to_string(ordinal)+"/"+
            std::to_string(paired_clock?paired_clock->qpc_monotonic_ns:0));
        if(event.name=="attack_mainhand"){
            const auto* world=combat.world();const auto serial=combat.update_serial();
            if(world&&serial){
                const auto& pending=world->pending_resolutions();const auto& damage=combat.events();
                for(std::size_t i=0;i<pending.size();++i){const auto& resolution=pending[i];
                    if(resolution.attacker!=actor||resolution.marker_name!=event.name)continue;
                    for(const auto& hit:damage){if(!hit.applied||hit.attacker!=resolution.attacker||
                        hit.target!=resolution.victim||hit.marker_name!=resolution.marker_name)continue;
                        if(hit.health_removed>0&&resolution.melee.original.amount>0&&
                           resolution.melee.original.outcomes==0)continue;
                        current_zero_result=true;
                        const auto& original=resolution.melee.original;
                        const auto signature=std::to_string(hit.attacker)+"/"+
                            std::to_string(hit.target)+"/"+hit.marker_name+"/"+
                            std::to_string(original.amount)+"/"+std::to_string(original.outcomes)+"/"+
                            std::to_string(original.mask)+"/"+std::to_string(resolution.melee.damage)+"/"+
                            std::to_string(hit.health_removed)+"/"+std::to_string(hit.target_died);
                        const auto previous=zero_result_updates.find(signature);
                        if(previous!=zero_result_updates.end()&&previous->second!=serial)
                            identical_zero_result_across_updates=true;
                        else zero_result_updates[signature]=serial;
                        break;
                    }
                    if(current_zero_result)break;
                }
            }
        }
        active_actor=actor;active_event=&event;active_event_index=ordinal;
        active_clock=paired_clock?*paired_clock:RetainedFrameAudioClock{};
        const auto status=base_observer(actor,event,ordinal,paired_clock,observer_error);
        active_event=nullptr;current_zero_result=false;return status;
    });

    InputActions input;input.targetSelect=true;check(combat.update(0,input,{0,0,0},0,error,&clock),error);
    combat.actor(2)->transform.position={0.5f,0,0};
    bool npc_attack_started=false;
    combat.set_actor_decision_provider([&](CombatSession& same,double dt,std::string& request_error){
        if(same.owns_pose(2)||!same.actor(2)->alive())return true;
        if(!same.request_actor_attack(2,1,dt,request_error))return false;
        npc_attack_started=npc_attack_started||same.owns_pose(2);return true;
    });
    input.targetSelect=false;input.attack=true;check(combat.update(0,input,{0,0,0},0,error,&clock),error);input.attack=false;
    check(combat.owns_pose(1),"Actual retained CombatSession did not acquire original player action");
    combat.actor(1)->max_health=10000;combat.actor(1)->health=10000;
    combat.actor(2)->max_health=10000;combat.actor(2)->health=10000;
    std::size_t actual_hits=0,max_hits_in_update=0;bool saw_missing_sample=false;
    for(unsigned frame=0;frame<120;++frame){
        input.attack=true;
        const auto frame_clock=clock_now();
        const auto before=diagnostics.size();
        check(combat.update(.30,input,{0,0,0},0,error,&frame_clock),error);
        actual_hits+=combat.events().size();
        max_hits_in_update=std::max(max_hits_in_update,combat.events().size());
        for(std::size_t i=before;i<diagnostics.size();++i){
            const auto& item=diagnostics[i];
            if(item.status==RuntimeCombatAudioStatusV1::playback_diagnostic&&
               item.detail.find("Unavailable original audio asset: ")!=std::string::npos&&
               item.detail.find(missing_hit_filename)!=std::string::npos)
                saw_missing_sample=true;
        }
    }
    check(presentation_order.size()>=3&&presentation_order[0]=="audio-first"&&
          presentation_order[1]=="fx-first"&&presentation_order[2]=="fx-last",
          "Audio-first/presentation order or independent exception isolation failed on actual Session steps");
    check(presentation_log.str().find("threw: intentional step presentation diagnostic")!=std::string::npos,
          "Throwing step presenter did not produce an isolated diagnostic");
    check(npc_attack_started,"Same-session second actor did not acquire its authored attack");
    check(actual_hits>=2&&max_hits_in_update>=2,
          "One CombatSession update did not produce multiple retained combat resolutions");
    bool saw_actual_combo_step=false;std::set<std::int64_t> actual_combo_sequences;
    for(const auto& item:attack_sound_diagnostics)
        if((item.sequence_id==470||item.sequence_id==471||item.sequence_id==472)&&item.step==1&&
           item.sound_id==289&&(item.status==RuntimeAttackSoundStatusV1::dispatched||
                                item.status==RuntimeAttackSoundStatusV1::playback_diagnostic)) {
            saw_actual_combo_step=true;
            actual_combo_sequences.insert(item.sequence_id);
        }
    check(saw_actual_combo_step,
          "Same live CombatSession did not expose actual combo strike step entries to audio observer");
    check(actual_combo_sequences==std::set<std::int64_t>{470,471,472},
          "Same live source combo did not preserve all three distinct authored swing step rows");
    // Cause an authentic source Injury in an actual CombatSession update
    // while this same V42/WinMM output owns the paired clock. An isolated
    // session keeps the proof from perturbing the unrelated lethal-hit case.
    CombatSessionConfig injury_config=config;
    ActorPopulation injury_population;PopulationActor injury_npc;
    injury_npc.profileId="Swamp_LizadMan_Type1";injury_npc.definition.stableId=2;
    injury_npc.definition.sourceId="runtime-combat-audio-injury";
    injury_npc.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    injury_npc.transform={2,0,0,0,0,2,0,0,0,0,2,0,0,-100,0,1};
    injury_population.actors().push_back(std::move(injury_npc));
    CharacterVisual injury_player;CombatSession injury_session;
    check(injury_session.initialize(assets,database,bindings,injury_config,injury_player,
        injury_population,{0,0,0},customization,error),error);
    auto* injury_target=injury_session.actor(2);
    check(injury_target!=nullptr,"Same-session target actor is unavailable for source position test");
    original_target_position_ctor_prefix(injury_target->source_target_node180,
                                         injury_target->source_target_position184);
    const auto expected_injury_position=injury_target->transform.position;
    std::vector<std::array<float,3>> injury_submitted_positions;
    std::vector<RuntimeAttackSoundDiagnosticV1> injury_attack_diagnostics;
    RuntimeAttackSoundV1 injury_attack_audio(
        [&](ActorId actor,std::int32_t sound,const std::array<float,3>& position,
            std::int64_t qpc,std::string& submit_error) {
            injury_submitted_positions.push_back(position);
            return output.submit_actual_play(dh2::audio::audio_world_request_v38(
                runtime->manager(),static_cast<std::uintptr_t>(actor),sound,position),qpc,submit_error);
        },[&]{return output.ready_for_current_source();},injury_session,animations,item_table,
        [&](const RuntimeAttackSoundDiagnosticV1& item){injury_attack_diagnostics.push_back(item);});
    std::vector<CombatSessionStepEntry> injury_entries;
    injury_session.set_step_entry_observer([&](const CombatSessionStepEntry& entry){
        injury_entries.push_back(entry);injury_attack_audio.step_entry_observer()(entry);
    });
    CombatSessionSourceHit source_injury;source_injury.attacker=1;source_injury.target=2;
    source_injury.binding_lease=injury_session.actor_binding_lease();
    source_injury.source_id="runtime-combat-audio-source-injury";
    source_injury.marker_name="attack_mainhand";source_injury.mask=0x22aab5u;
    source_injury.category=injury_session.world()->combat_properties(1)->facts.main_damage_class;
    source_injury.element=-1;
    bool injury_applied=false;
    injury_session.set_actor_decision_provider([&](CombatSession& same,double,std::string& request_error){
        for(unsigned attempt=0;attempt<500&&!injury_applied;++attempt){
            same.actor(2)->health=same.actor(2)->max_health;
            ++source_injury.generation;DamageEvent receipt;
            if(!same.apply_source_result(source_injury,receipt,request_error))return false;
            injury_applied=receipt.applied&&receipt.source_outcomes&&((*receipt.source_outcomes&0x10u)!=0);
        }
        return injury_applied;
    });
    const auto injury_frame_clock=clock_now();
    check(injury_session.update(0,input,{0,0,0},0,error,&injury_frame_clock),error);
    injury_session.clear_actor_decision_provider();
    check(injury_applied,"Actual source hit did not select the recovered Swamp Lizard Injury branch");
    check(!injury_entries.empty(),"Same CombatSession did not publish an actual row377 reaction leaf");
    const auto& reaction_entry=injury_entries.back();
    check(reaction_entry.actor==2&&reaction_entry.sequence_id==377&&reaction_entry.step==0&&
          reaction_entry.audio_clock&&reaction_entry.audio_clock->valid()&&
          reaction_entry.audio_clock->output_generation==injury_frame_clock.output_generation&&
          reaction_entry.audio_clock->device_samples==injury_frame_clock.device_samples&&
          reaction_entry.audio_clock->qpc_monotonic_ns==injury_frame_clock.qpc_monotonic_ns&&
          !reaction_entry.binding_lease.expired(),
          "Source Injury row377 event did not preserve actor lease and exact caller-paired WinMM clock");
    const auto injury_diagnostic=std::find_if(injury_attack_diagnostics.begin(),injury_attack_diagnostics.end(),
        [&](const RuntimeAttackSoundDiagnosticV1& item){return item.actor==2&&
            item.occurrence==reaction_entry.occurrence&&item.update_serial==reaction_entry.update_serial&&
            item.sequence_id==377&&item.step==0&&item.sound_id==477;});
    check(injury_diagnostic!=injury_attack_diagnostics.end()&&
          injury_diagnostic->status==RuntimeAttackSoundStatusV1::playback_diagnostic&&
          injury_diagnostic->detail.find("Unavailable original audio asset: "+injury_uri)!=std::string::npos,
          "Actual Injury Sound477 did not reach the same V42 output as the exact missing-WAV diagnostic");
    check(!injury_submitted_positions.empty()&&
          injury_submitted_positions.back()==expected_injury_position,
          "Actual null-node injury event submitted stale ctor cache instead of current actor transform");
    const auto* death_binding=runtime->bindings().row(476);
    check(death_binding&&!death_binding->event&&death_binding->uid==285,
          "Actual source Death Sound476 binding no longer resolves to UID285");
    const auto* death_sound=runtime->catalog().sound(285);
    check(death_sound&&death_sound->filename=="sfx_lizardman_die.wav",
          "Actual UID285 source death sound metadata changed");
    std::shared_ptr<const std::vector<std::uint8_t>> death_sample;
    std::string death_asset_error;const auto death_uri="data/sounds/"+death_sound->filename;
    check(!files.read(&files,death_uri.c_str(),death_sample,death_asset_error)&&!death_sample,
          "Exact recovered lizard death WAV unexpectedly appeared; update missing-asset expectation");
    CombatSessionSourceHit source_death=source_injury;
    source_death.source_id="runtime-combat-audio-source-death";
    source_death.mask=0x20080000u;source_death.category=-1;++source_death.generation;
    source_death.direct_amount=static_cast<std::int32_t>(std::ceil(injury_session.actor(2)->health*256));
    bool death_applied=false;
    injury_session.set_actor_decision_provider([&](CombatSession& same,double,std::string& request_error){
        DamageEvent receipt;
        if(!same.apply_source_result(source_death,receipt,request_error))return false;
        death_applied=receipt.applied&&receipt.target_died;return death_applied;
    });
    const auto death_frame_clock=clock_now();
    check(injury_session.update(0,input,{0,0,0},0,error,&death_frame_clock),error);
    injury_session.clear_actor_decision_provider();
    check(death_applied&&!injury_session.actor(2)->alive(),
          "Actual source death result did not kill the configured Swamp Lizard");
    check(injury_entries.size()>=2,"Same CombatSession did not publish both injury and death leaves");
    const auto& death_entry=injury_entries.back();
    check(death_entry.actor==2&&death_entry.sequence_id==374&&death_entry.step==0&&
          death_entry.audio_clock&&death_entry.audio_clock->valid()&&
          death_entry.audio_clock->output_generation==death_frame_clock.output_generation&&
          death_entry.audio_clock->device_samples==death_frame_clock.device_samples&&
          death_entry.audio_clock->qpc_monotonic_ns==death_frame_clock.qpc_monotonic_ns,
          "Source death row374 event did not preserve the exact caller-paired WinMM clock");
    const auto death_diagnostic=std::find_if(injury_attack_diagnostics.begin(),injury_attack_diagnostics.end(),
        [&](const RuntimeAttackSoundDiagnosticV1& item){return item.actor==2&&
            item.occurrence==death_entry.occurrence&&item.update_serial==death_entry.update_serial&&
            item.sequence_id==374&&item.step==0&&item.sound_id==476;});
    check(death_diagnostic!=injury_attack_diagnostics.end()&&
          death_diagnostic->status==RuntimeAttackSoundStatusV1::playback_diagnostic&&
          death_diagnostic->detail.find("Unavailable original audio asset: "+death_uri)!=std::string::npos,
          "Actual Death Sound476 did not reach the same V42 output as the exact missing-WAV diagnostic");
    check(injury_submitted_positions.size()>=2&&
          injury_submitted_positions.back()==expected_injury_position,
          "Actual null-node death event submitted stale ctor cache instead of current actor transform");
    injury_session.clear_step_entry_observer();
    check(presentation_tail_calls>tail_calls_before_rebind,
          "Registered step presenters did not survive Session observer detach/rebind");
    const auto* step_sound_binding=runtime->bindings().row(289);
    check(step_sound_binding&&step_sound_binding->event&&step_sound_binding->uid==6,
          "Actual AnimTable Sound289 does not map to generated source event UID6");
    check(std::find(step_submit_ids.begin(),step_submit_ids.end(),289)!=step_submit_ids.end(),
          "Original Longsword Swoosh decision did not submit conditional AnimationStep.Sound289 fallback");
    check(!step_submit_times.empty()&&std::all_of(step_submit_times.begin(),step_submit_times.end(),
          [](std::int64_t time){return time>0;}),
          "Step sound submissions did not use the caller-paired positive output QPC sample");
    check(source_fixture.random_draws>0,
          "Generated AnimTable source event did not consume the same CombatSession gameplay RNG");
    check(captured_combo_step&&captured_combo_step->audio_clock&&captured_combo_step->audio_clock->valid(),
          "Actual strike boundary did not retain the paired WinMM sample/QPC clock");
    const auto submissions_before_replay=step_submit_ids.size();
    auto replay=*captured_combo_step;replay.audio_clock->qpc_monotonic_ns+=1000000;
    attack_step_observer(replay);
    check(step_submit_ids.size()==submissions_before_replay,
          "Changed-QPC replay of the same exact retained step occurrence resubmitted audio");
    std::vector<RuntimeAttackSoundDiagnosticV1> failed_step_diagnostics;
    RuntimeAttackSoundV1 failed_attack_audio(
        [](ActorId,std::int32_t,const std::array<float,3>&,std::int64_t,std::string& detail) {
            detail="Unavailable original audio asset: fixture missing source sample";return false;
        },[]{return true;},combat,animations,item_table,
        [&](const RuntimeAttackSoundDiagnosticV1& item){failed_step_diagnostics.push_back(item);});
    const auto health_before_audio_failure=combat.actor(1)->health;
    const auto gameplay_events_before_audio_failure=combat.events().size();
    failed_attack_audio.step_entry_observer()(*captured_combo_step);
    check(failed_step_diagnostics.size()==1&&
          failed_step_diagnostics.front().status==RuntimeAttackSoundStatusV1::playback_diagnostic&&
          failed_step_diagnostics.front().detail.find("Unavailable original audio asset:")!=std::string::npos,
          "Reached step audio asset failure was not retained as a diagnostic");
    check(combat.actor(1)->health==health_before_audio_failure&&
          combat.events().size()==gameplay_events_before_audio_failure,
          "Step audio asset failure changed same-session gameplay state");
    check(source_fixture.random_draws>0,
          "Generated AnimTable source event did not consume the same CombatSession gameplay RNG");
    check(identical_zero_result_across_updates&&zero_hit_adapter_dispatches>=2,
          "Identical zero/miss resolution across distinct update_serial values was not dispatched twice");
    check(hit_qpc_replay_attempted&&
          hit_qpc_replay_status==RetainedFrameAudioObserverStatus::not_applicable,
          "Replaying the actual retained hit occurrence with changed QPC was not suppressed");
    if(!saw_missing_sample){std::string summary="Missing original hit-list sample was not diagnostic-only through the real hit adapter; expected="+missing_hit_filename;
        for(const auto& item:diagnostics)summary+=" | "+item.event_name+":"+item.detail;throw std::runtime_error(summary);}
    check(std::find(source_fixture.sound_ids.begin(),source_fixture.sound_ids.end(),missing_ordinal)==source_fixture.sound_ids.end(),
          "Missing selected hit ordinal unexpectedly reached the source command provider");
    check(combat.actor(1)->health<combat.actor(1)->max_health,
          "Audio asset failure vetoed original CombatSession health progression");

    check(death_list_count>0&&static_cast<std::uint32_t>(tables.get(missing_audio_row)->death.ids[0])==death_ordinal,
          "Selected actual CharSounds fixture row has no death-list entry");
    input.attack=false;
    combat.clear_actor_decision_provider();
    check(combat.update(1.0,input,{0,0,0},0,error,&clock),error);
    combat.actor(1)->health=1.0f;bool lethal_npc_attack_started=false;
    combat.set_actor_decision_provider([&](CombatSession& same,double dt,std::string& request_error){
        if(lethal_npc_attack_started)return true;
        if(!same.request_actor_attack(2,1,dt,request_error))return false;
        lethal_npc_attack_started=same.owns_pose(2);return true;
    });
    for(unsigned frame=0;frame<120&&combat.actor(1)->alive();++frame){
        const auto frame_clock=clock_now();
        check(combat.update(1.0/60,input,{0,0,0},0,error,&frame_clock),error);
    }
    check(lethal_npc_attack_started,"Lethal NPC source attack did not acquire its action");
    check(!combat.actor(1)->alive(),"Lethal same-session hit did not apply target death");
    if(std::find(random_list_counts.begin(),random_list_counts.end(),death_list_count)==random_list_counts.end()){
        std::string summary="Lethal hit did not select from death list count="+std::to_string(death_list_count)+" random counts=";
        for(const auto count:random_list_counts)summary+=std::to_string(count)+",";
        for(const auto& occurrence:attack_occurrence_trace)summary+=" | occurrence="+occurrence;
        for(const auto& item:diagnostics)summary+=" | "+std::to_string(item.actor)+":"+item.event_name+":"+item.detail;
        throw std::runtime_error(summary);
    }

    check(diagnostics.size()>=3,"Runtime audio statuses/diagnostics were not retained");
    combat.clear_retained_frame_audio_observer();combat.set_step_entry_observer({});
    presentation_registry.clear();
    check(output.shutdown(error),"Same WinMM runtime did not close/drain: "+error);cleanup.done=true;
    std::cout<<"PASS same CombatSession + AudioNativeSessionV42/WinMM: default CharSounds field8 player="<<resolved_sound_rows[0]<<" NPC="<<resolved_sound_rows[1]<<"; row2 fallback verified; actual source injury row377 Sound477→UID284 and death row374 Sound476→UID285 reached same output with exact paired WinMM clocks and missing-WAV diagnostics; hit ordinal="<<missing_ordinal<<"; boundary observer matched multiple hits/death before motion; changed-QPC replay suppressed; missing samples diagnostic-only; hits="<<actual_hits<<"\n";
    return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
