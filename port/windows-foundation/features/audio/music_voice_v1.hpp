#pragma once

#include "../../../engine-audio/audio_gameplay_runtime_v42.hpp"
#include <cstdint>
#include <string>

namespace dh::foundation::audio {

// Original VoxSoundManager::PlayMusic/StopMusic voice semantics (IDA 0x36bd78), shared by every
// owner that plays one music track at a time on an AudioGameplayRuntimeV42: the gameplay level
// music (RuntimeAudioHostV1) and the frontend title/main-menu music (FrontendMenuAudioSessionV1).
// The same ordinal that is still playing resumes (0.05 s, no restart); a new ordinal first stops
// the previous track with the fade, then starts the new row at its fresh native state (VXN) with a
// fade-in. Looping comes from the sounds.xml row (loop="yes").
enum class MusicVoiceActionV1 : std::uint8_t {unchanged,resumed,started,switched};

struct MusicVoiceV1 {
    std::int32_t ordinal{-1};
    bool play(dh2::audio::AudioGameplayRuntimeV42& runtime,std::int32_t ordinal_to_play,int fade_ms,
        MusicVoiceActionV1& action,std::string& error);
    bool stop(dh2::audio::AudioGameplayRuntimeV42& runtime,int fade_ms,std::string& error);
};

} // namespace dh::foundation::audio
