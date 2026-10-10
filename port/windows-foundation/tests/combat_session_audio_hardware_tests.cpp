#include "../combat_session.hpp"
#include "../retained_animation_owner.hpp"
#include "../features/audio/feature_audio.hpp"
#include "../features/audio/windows_source_session_control_v1.hpp"
#include "../../engine-audio/integration-v42/audio_native_session_v42.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
static void require(bool ok,const std::string& message){if(!ok)throw std::runtime_error(message);}

int main(int argc,char** argv){try{
    require(argc==3,"Supply shared original assets and audio assets");
    audio::AudioFilesystem files{argv[2]};dh2::audio::AudioGameplaySourcesV40 sources;
    sources.exact_assets={&files,audio::AudioFilesystem::read,{}};
    auto provider=std::make_shared<unsigned>(1);
    dh2::audio::AudioLifecycleGateV40 gate;std::string error;
    require(gate.publish_activity(1,1,true,true,true,false),"Focused lifecycle publication failed");
    dh2::audio::AudioNativeSessionV42 output(reinterpret_cast<std::uintptr_t>(&files),sources,provider,gate,
                                           dh2::audio::windows_source_session_control_v1());
    require(output.initialize(error),error);
    struct CheckedAudioCleanup {
        dh2::audio::AudioNativeSessionV42& output;bool completed=false;
        ~CheckedAudioCleanup(){if(!completed){std::string failure;if(!output.shutdown(failure))std::cerr<<"Audio test cleanup failed: "<<failure<<'\n';}}
    } audio_cleanup{output};
    const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(!output.ready_for_current_source()&&std::chrono::steady_clock::now()<deadline)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    require(output.ready_for_current_source(),"Actual WinMM output did not become ready");
    auto* runtime=output.runtime_on_producer();require(runtime!=nullptr,"Same V42 runtime unavailable");
    auto device_clock=[&](){dh2::audio::AudioDeviceClockV40 published;
        require(runtime->clock().snapshot(published)&&published.ready,"Actual published device clock unavailable");
        RetainedFrameAudioClock clock{published.generation,published.position,published.monotonic_ns,published.ready};
        require(clock.valid(),"Published TIME_SAMPLES/QPC pair invalid");return clock;};

    AssetCatalog assets(argv[1]);OriginalPropertyDatabase database;OriginalMeleeBindings bindings;
    require(load_original_property_tables(assets,"original-cache/data/pydata",database,error),error);
    require(bindings.load(assets,"original-melee-bindings.xml",error),error);
    CombatSessionConfig config;config.diagnosticRngSeed=1234;config.playerId=1;
    config.playerProfileId="KnightPlayerBase";config.tableRoot="original-cache/data/pydata";
    ActorCustomization customization;customization.skin_id_contains="_default_warrior-mesh-skin";
    customization.expected_controller_count=4;customization.allow_missing_animation_targets=true;
    OriginalCombatVisualPlan plan;
    require(build_original_combat_visual_plan(assets,bindings,config.playerProfileId,customization,"audio-frame-player",plan,error),error);
    config.playerVisualConfig=plan.config;config.playerVisualConfig.clips.clear();
    config.playerVisualConfig.motion_node_id="auto";config.playerVisualConfig.consume_root_motion=true;
    config.playerVisualConfig.clips={{"idle",plan.phase("Idle",0,{0})->resolvedPath}};
    CombatSessionProfile knight;knight.action={"AttackStatic",0,{0,1}};knight.initialIdle={"Idle",0,{0}};
    knight.damageMarkerNames={"attack_mainhand"};knight.propertyOptions={256,true};knight.retainedPhaseClock=true;
    config.profiles.emplace(config.playerProfileId,knight);
    CombatSessionProfile lizard;lizard.action={"Attack",0,{0,1}};lizard.initialIdle={"Idle",0,{0}};
    lizard.damageMarkerNames={"attack_mainhand"};lizard.propertyOptions={std::nullopt,true};
    lizard.customization.allow_missing_animation_targets=true;lizard.retainedPhaseClock=true;lizard.diagnosticAIEnabled=true;
    lizard.motionRoot="auto";
    lizard.requiredAIState="explicit-audio-regression-gate";
    config.profiles.emplace("Swamp_LizadMan_Type1",lizard);
    AssetCatalog sequence_metadata(assets.root().parent_path()/"windows-melee-bindings");
    require(bindings.load(sequence_metadata,"original-melee-bindings.xml",error),error);
    OriginalAttackSelection npc_sequence;npc_sequence.state="Attack";npc_sequence.variant=0;npc_sequence.group_path={0};
    config.profiles.at("Swamp_LizadMan_Type1").sequenceAction=npc_sequence;
    OriginalAttackSelection player_sequence;player_sequence.state="AttackStatic";player_sequence.variant=0;player_sequence.group_path={0};
    config.profiles.at(config.playerProfileId).sequenceAction=player_sequence;
    ActorPopulation population;PopulationActor placed;placed.profileId="Swamp_LizadMan_Type1";
    placed.definition.stableId=2;placed.definition.sourceId="explicit-audio-regression-placement";
    placed.definition.properties["ai_state"]="explicit-audio-regression-gate";
    placed.definition.placement={1,0,0,0,0,1,0,0,0,0,1,0,0,-100,0,1};
    placed.transform={2,0,0,0,0,2,0,0,0,0,2,0,0,-100,0,1};population.actors().push_back(std::move(placed));
    CharacterVisual player;CombatSession session;
    struct Result {float health{};std::size_t hits{};unsigned motion{};Vec3 displacement{};std::uint64_t rng_calls{};};
    Result baseline;unsigned total_observations=0;bool ordinal_one=false;
    // Each run uses identical original actor data and RNG. Only the optional
    // presentation observer changes; no native AI completeness is asserted.
    for(unsigned mode=0;mode<4;++mode){
        require(session.initialize(assets,database,bindings,config,player,population,{0,0,0},customization,error),error);
        unsigned hit_forwards=0,motion=0,observations=0;bool pending_audio=false;Vec3 displacement{};
        RetainedFrameAudioClock expected_clock;
        CombatSessionAnimationNotificationServices notification;
        notification.state_event=[&](ActorId id,std::uint32_t event,const RetainedAnimationEvent* named,std::string&){
            if(id==2&&event==0x28&&named&&named->name=="attack_mainhand"){
                ++hit_forwards;pending_audio=true;
                require(!session.events().empty()&&session.events().back().attacker==2,"Audio boundary preceded actual hit receipt");
            }return true;};
        session.set_animation_notification_services(notification);
        session.set_motion_handler([&](ActorState& actor,Vec3 delta,bool,std::string&){if(actor.id==2){
            if(mode==1||mode==2)require(!pending_audio,"Motion preceded audio observation after hit");
            ++motion;displacement.x+=delta.x;displacement.y+=delta.y;displacement.z+=delta.z;}return true;});
        if(mode)session.set_retained_frame_audio_observer([&](ActorId id,const RetainedAnimationEvent& event,
                std::uint32_t ordinal,const RetainedFrameAudioClock* clock,std::string& detail){
            require(mode!=3,"Observer invoked with unavailable clock");
            require(clock&&clock->valid(),"Observer did not receive caller's actual published clock");
            require(clock->output_generation==expected_clock.output_generation&&clock->device_samples==expected_clock.device_samples&&
                    clock->qpc_monotonic_ns==expected_clock.qpc_monotonic_ns,"Observer changed the actual caller-published device pair");
            if(id==2&&event.name=="attack_mainhand"){
                require(pending_audio&&hit_forwards>observations,"Observer preceded original gameplay consumer");
                pending_audio=false;++observations;++total_observations;if(ordinal==1)ordinal_one=true;
            }
            if(mode==2)throw std::runtime_error("intentional audio presentation failure");
            detail="intentional required audio-owner rejection";return RetainedFrameAudioObserverStatus::required_owner_unavailable;
        });
        auto clock=device_clock();expected_clock=clock;InputActions input;
        require(session.update(0,input,{0,0,0},0,error,mode==3?nullptr:&clock),error);
        require(session.owns_pose(2),"Explicit regression NPC did not enter attack");
        const auto* sequence=session.attack_sequence(2);require(sequence!=nullptr,"Original enemy sequence unavailable");
        Result result;std::vector<RetainedFrameAudioObserverDiagnostic> diagnostics;
        // Advance the actual retained slots and their source phase transitions.
        for(unsigned frame=0;frame<200&&session.owns_pose(2);++frame){
            const auto* pose=session.retained_actor_pose(2);require(pose!=nullptr,"Same retained enemy pose unavailable");
            const auto& selected=pose->slots()[pose->current_slot()].clip_id;
            const bool strike=sequence->phases().size()>1&&(sequence->phases()[1].source.resolvedPath==selected||sequence->phases()[1].source.clipName==selected);
            clock=device_clock();expected_clock=clock;
            require(session.update(strike?sequence->phases()[1].wall_duration_seconds:.016,input,{0,0,0},0,error,mode==3?nullptr:&clock),error);
            for(const auto& event:session.events())if(event.attacker==2)++result.hits;
            const auto& current=session.retained_frame_audio_diagnostics();diagnostics.insert(diagnostics.end(),current.begin(),current.end());
        }
        result.health=session.actor(1)->health;result.motion=motion;result.displacement=displacement;result.rng_calls=session.world()->random_state().calls;
        require(result.hits==2&&hit_forwards==2&&motion>0,"Original two-hit enemy timeline did not reach hit and motion consumers: mode="+
                std::to_string(mode)+" hits="+std::to_string(result.hits)+" forwards="+std::to_string(hit_forwards)+" motion="+std::to_string(motion));
        if(!mode)baseline=result;else{
            require(result.health==baseline.health&&result.hits==baseline.hits&&result.motion==baseline.motion&&result.rng_calls==baseline.rng_calls,
                    "Audio diagnostic changed gameplay health, hit count, motion or RNG");
            require(result.displacement.x==baseline.displacement.x&&result.displacement.y==baseline.displacement.y&&result.displacement.z==baseline.displacement.z,
                    "Audio diagnostic changed original root displacement");
            require(mode==3?observations==0:observations==2,"Incorrect retained-frame observation count");
            unsigned diagnostic_hits=0;
            for(const auto& diagnostic:diagnostics)if(diagnostic.actor==2&&diagnostic.event_name=="attack_mainhand"){
                ++diagnostic_hits;
                require(diagnostic.status==(mode==1?RetainedFrameAudioObserverStatus::required_owner_unavailable:
                    mode==2?RetainedFrameAudioObserverStatus::playback_diagnostic:RetainedFrameAudioObserverStatus::unavailable_clock),
                    "Audio diagnostic status not retained");
            }
            require(diagnostic_hits==2,"Same frame diagnostic lost original enemy markers");
        }
        session.clear_animation_notification_services();session.clear_retained_frame_audio_observer();session.set_motion_handler({});
    }
    require(ordinal_one,"Two original events did not expose ordinal1 in the same retained frame");
    require(output.shutdown(error),error);
    audio_cleanup.completed=true;
    std::cout<<"PASS actual CombatSession + WinMM published clock: original two-hit frame ordinals0/1; gameplay then audio then motion; rejection/throw/missing-clock preserve health/hits/motion/RNG; observations="<<total_observations<<"\n";
    return 0;
}catch(const std::exception& ex){std::cerr<<ex.what()<<'\n';return 1;}}
