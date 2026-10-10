#include "music_voice_v1.hpp"
#include "level_music_v1.hpp"
#include "platform_source_audio.hpp"
#include "../../../engine-audio/audio_source_command_v40.hpp"

namespace dh::foundation::audio {

bool MusicVoiceV1::play(dh2::audio::AudioGameplayRuntimeV42& runtime,std::int32_t ordinal_to_play,int fade_ms,
    MusicVoiceActionV1& action,std::string& error) {
    action=MusicVoiceActionV1::unchanged;
    if(ordinal_to_play<0||fade_ms<0) {
        error="Required SAME source runtime, music row and nonnegative fade";return false;
    }
    dh2::audio::AudioDeviceClockV40 clock;
    if(!runtime.clock().snapshot(clock)||!clock.ready||!clock.rate) {
        error="Required actual output rate for music fade";return false;
    }
    const auto fade_frames=std::uint32_t(std::uint64_t(fade_ms)*clock.rate/1000);
    if(ordinal_to_play==ordinal) {
        bool playing{};
        if(!runtime.source_ordinal_playing(ordinal_to_play,playing,error))return false;
        if(playing) {
            // PlayMusic same-id branch: Resume(emitter, 0.05 s), no restart.
            if(!runtime.resume_source_ordinal(ordinal_to_play,std::uint32_t(std::uint64_t(kLevelMusicResumeMs)*clock.rate/1000),error))return false;
            action=MusicVoiceActionV1::resumed;
            error.clear();return true;
        }
        // Same track whose voice already ended: a fresh start, not a resume.
        action=MusicVoiceActionV1::started;
    } else if(ordinal>=0) {
        if(!stop(runtime,fade_ms,error))return false;
        action=MusicVoiceActionV1::switched;
    } else {
        action=MusicVoiceActionV1::started;
    }
    std::int64_t event_ns{};
    if(!winmm_monotonic_ns(event_ns,error))return false;
    if(!runtime.submit_plain_source(ordinal_to_play,event_ns,
        [&](const dh2::audio::AudioSoundV34& sound,const dh2::audio::AudioGroupV34& group,
            dh2::audio::AudioCommandV34& command,std::string& e) {
            if(sound.format==2) {
                // VXN initial state comes from the same decoded sample (fresh row 0).
                int state{};
                if(!runtime.fresh_native_state_v68(sound.uid,state,e))return false;
                command.native_state=state;
            }
            command.left=command.right=dh2::audio::original_fresh_emitter_gain_v40();
            command.pitch=dh2::audio::original_fresh_emitter_pitch_v40();
            command.volume_group=group.volume_group;
            command.fade_frames=fade_frames;
            e.clear();return true;
        },error))return false;
    ordinal=ordinal_to_play;error.clear();return true;
}

bool MusicVoiceV1::stop(dh2::audio::AudioGameplayRuntimeV42& runtime,int fade_ms,std::string& error) {
    if(ordinal<0) {error.clear();return true;}
    if(!runtime.stop_source_sound_v106(ordinal,fade_ms,error))return false;
    ordinal=-1;error.clear();return true;
}

} // namespace dh::foundation::audio
