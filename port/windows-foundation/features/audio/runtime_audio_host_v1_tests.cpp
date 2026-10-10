#include "runtime_audio_host_v1.hpp"
#include "../../../engine-audio/integration-v40/runtime/default-source-fields/default_source_fields_v40.hpp"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <thread>

using namespace dh::foundation::audio;
using namespace dh2::audio;

static void check(bool value,const std::string& detail) {
    if(!value)throw std::runtime_error(detail);
}

struct ProviderFixture {
    RuntimeAudioHostFactsV1 facts;
    unsigned snapshots{};
};

static bool snapshot(void* raw,RuntimeAudioHostFactsV1& facts,std::string& error) {
    auto& fixture=*static_cast<ProviderFixture*>(raw);
    ++fixture.snapshots;facts=fixture.facts;error.clear();return true;
}

static bool select_first(void*,int& value) {value=0;return true;}

static bool wait_ready(RuntimeAudioHostV1& host,bool expected) {
    const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(4);
    while(std::chrono::steady_clock::now()<deadline) {
        if(host.ready()==expected)return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return host.ready()==expected;
}

int main(int argc,char** argv) {
    try {
        check(argc==3,"Supply exact source audio assets and legacy sounds_pyarray.bin");
        auto provider=std::make_shared<ProviderFixture>();
        provider->facts.gameplay_active=true;
        provider->facts.listener_initialized=true;
        provider->facts.world_epoch=1;
        provider->facts.listener_world_epoch=1;
        provider->facts.listener.listener.position[0]=0.f;
        provider->facts.listener.listener.position[1]=0.f;
        provider->facts.listener.listener.position[2]=0.f;
        provider->facts.listener.listener.front[0]=0.f;
        provider->facts.listener.listener.front[1]=1.f;
        provider->facts.listener.listener.front[2]=0.f;
        provider->facts.listener.listener.up[0]=0.f;
        provider->facts.listener.listener.up[1]=0.f;
        provider->facts.listener.listener.up[2]=1.f;
        provider->facts.listener.reference_distance=100;
        provider->facts.listener.maximum_distance=100000;
        provider->facts.listener.rolloff=1.f;
        provider->facts.native_initial_state=0;

        RuntimeAudioHostConfigV1 config;
        config.actual_manager_identity=reinterpret_cast<std::uintptr_t>(provider.get());
        config.gameplay_context_identity=reinterpret_cast<std::uintptr_t>(provider.get());
        const auto manager_identity=config.actual_manager_identity;
        const auto gameplay_identity=config.gameplay_context_identity;
        config.exact_asset_root=argv[1];
        config.provider_lease=provider;
        config.providers.context=provider.get();config.providers.snapshot=snapshot;
        config.providers.source_event_random={nullptr,select_first};
        RuntimeAudioHostV1 host(std::move(config));
        check(host.publish_window_activity(1,1,true,true,true,false),
              "Could not publish initial foreground activity facts");
        std::string error;check(host.start(error),"Real WinMM/source host startup failed: "+error);
        check(wait_ready(host,true),"Same WinMM output did not publish focused source readiness");
        dh::foundation::RetainedFrameAudioClock first_clock;
        check(host.device_clock(first_clock,error)&&first_clock.valid(),
              "Host did not expose actual WinMM sample/QPC clock: "+error);
        const auto ordinal=host.source_ordinal("WeaponSwoosh1");
        check(ordinal>=0,"Actual generated source binding lacks WeaponSwoosh1");

        std::ifstream char_sounds_input(argv[2],std::ios::binary);
        check(bool(char_sounds_input),"Cannot read actual source CharSounds table for enrollment factory");
        std::vector<std::uint8_t> char_sounds_bytes{
            std::istreambuf_iterator<char>(char_sounds_input),{}};
        dh2::character::CharacterCombatSoundTablesV2 char_sounds;
        check(char_sounds.load(char_sounds_bytes,error),"Actual CharSounds table failed to load: "+error);
        dh::foundation::CombatSession enrollment_fixture;
        auto adapter=host.make_combat_audio(enrollment_fixture,char_sounds,{});
        check(bool(adapter),"RuntimeCombatAudioV1 enrollment factory did not bind the live host session");

        dh2::character::CombatSoundPlayV1 play;
        play.manager=manager_identity;
        play.target=gameplay_identity;
        play.sound_id=ordinal;play.position={1.f,2.f,3.f};
        play.source_bool=false;play.source_integer=1;
        play.source_float0=-1.f;play.source_float1=-1.f;
        check(host.submit_actual_play(play,first_clock.qpc_monotonic_ns,error),
              "Typed gameplay gate + exact source asset failed through same V42 runtime: "+error);

        bool started=false;dh::foundation::RetainedFrameAudioClock last_clock;
        const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(3);
        while(std::chrono::steady_clock::now()<deadline&&!started) {
            check(host.update(error),"Same producer receipt update failed: "+error);
            dh2::audio::AudioReceiptV34 receipt;
            while(host.take_receipt(receipt))
                started|=receipt.kind==dh2::audio::AudioReceiptKindV34::started;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        check(started,"Real WinMM did not start the source-selected original sample");
        check(host.device_clock(last_clock,error)&&last_clock.valid()&&
              last_clock.output_generation==first_clock.output_generation&&
              last_clock.device_samples>first_clock.device_samples,
              "Host clock did not expose advancing TIME_SAMPLES/QPC in one output generation");

        play.sound_id=328; // Genuine TrollReturn source ordinal; its exact WAV is absent.
        auto absent_clock=last_clock;absent_clock.qpc_monotonic_ns+=1000000;
        check(!host.submit_actual_play(play,absent_clock.qpc_monotonic_ns,error)&&
              error.find("Unavailable original audio asset: data/sounds/sfx_cave_troll_smash_ground.wav")!=std::string::npos,
              "Missing original Troll source asset was substituted or failed without its exact diagnostic");
        play.sound_id=ordinal;

        const auto before_disabled=provider->snapshots;
        provider->facts.audio_disabled=true;
        auto disabled_clock=last_clock;disabled_clock.qpc_monotonic_ns+=1000000;
        check(host.submit_actual_play(play,disabled_clock.qpc_monotonic_ns,error),
              "Original disabled gate should be a non-error early exit: "+error);
        check(provider->snapshots==before_disabled+1,
              "Disabled branch did not return before current-level/source operations");
        provider->facts.audio_disabled=false;

        const auto before_online_mute=provider->snapshots;
        provider->facts.online=true;provider->facts.network_muted=true;
        auto muted_clock=disabled_clock;muted_clock.qpc_monotonic_ns+=1000000;
        check(host.submit_actual_play(play,muted_clock.qpc_monotonic_ns,error),
              "Online muted source gate should be a non-error early exit: "+error);
        check(provider->snapshots==before_online_mute+4,
              "Online mute path did not execute disabled/current-level/online/network-muted gates in order");
        provider->facts.online=false;provider->facts.network_muted=false;

        const auto before_inactive=provider->snapshots;
        provider->facts.gameplay_active=false;
        auto inactive_clock=disabled_clock;inactive_clock.qpc_monotonic_ns+=1000000;
        check(host.submit_actual_play(play,inactive_clock.qpc_monotonic_ns,error),
              "Modern gameplay-inactive gate should be a non-error early exit: "+error);
        check(provider->snapshots==before_inactive+2,
              "Inactive gameplay branch did not stop after the current-level gate");
        provider->facts.gameplay_active=true;

        check(host.publish_window_activity(1,2,true,false,false,false),
              "Could not publish window-focus loss");
        check(wait_ready(host,false),"WinMM output did not pause when window focus was lost");
        dh::foundation::RetainedFrameAudioClock unfocused;
        const auto focus_loss_deadline=std::chrono::steady_clock::now()+std::chrono::seconds(2);
        bool clock_invalidated=false;
        while(std::chrono::steady_clock::now()<focus_loss_deadline) {
            if(!host.device_clock(unfocused,error)&&!unfocused.valid()) {clock_invalidated=true;break;}
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        check(clock_invalidated,
              "Unfocused V42 clock exposed a valid source sample");
        check(host.publish_window_activity(1,3,true,true,true,false),
              "Could not publish window-focus regain");
        check(wait_ready(host,true),"Same WinMM output did not resume after focus regain");
        dh::foundation::RetainedFrameAudioClock resumed;
        check(host.device_clock(resumed,error)&&resumed.valid()&&
              resumed.output_generation==first_clock.output_generation,
              "Focus resume did not restore a valid clock on retained output generation");

        provider->facts.listener_initialized=false;
        auto failed_clock=resumed;failed_clock.qpc_monotonic_ns+=1000000;
        check(!host.submit_actual_play(play,failed_clock.qpc_monotonic_ns,error)&&
              error.find("Required current actual camera/listener snapshot")!=std::string::npos,
              "Missing same-world actual listener snapshot did not fail source command closed");

        check(host.shutdown(error),"Checked WinMM session close/join/drain failed: "+error);
        std::cout<<"PASS RuntimeAudioHostV1: one AudioNativeSessionV42 + WinMM; exact source ordinal="
                 <<ordinal<<" sample started; actual TIME_SAMPLES/QPC advanced; combat-audio adapter factory; disabled/online-muted/inactive gate order; absent Troll328 exact asset diagnostic/no substitution; focus pause/resume; missing listener fail-closed; checked shutdown; no native GS/Level or audible-parity claim\n";
    } catch(const std::exception& ex) {
        std::cerr<<ex.what()<<'\n';return 1;
    }
}
