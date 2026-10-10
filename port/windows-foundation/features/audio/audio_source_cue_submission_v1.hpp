#pragma once

#include "../../../engine-audio/audio_gameplay_runtime_v42.hpp"

#include <cstdint>
#include <string>

namespace dh::foundation::audio {

// Source ordinal + caller-captured event time, resolved against the same
// registered source table and selected soundpack used by the V42 runtime.
struct AudioSourceCueSubmissionV1 {
    std::int32_t source_id{-1};
    std::int32_t xml_sound_uid{-1};
    std::int64_t event_monotonic_ns{};
    std::string source_name;
    std::string exact_resource_uri;
};

bool resolve_audio_source_cue_v1(
    const dh2::audio::AudioSourceBindingsV38&,
    const dh2::audio::AudioCatalogV34&,
    std::int32_t source_id,
    std::int64_t actual_event_monotonic_ns,
    AudioSourceCueSubmissionV1&, std::string& error);

// This wraps the existing typed Play(source ordinal, monotonic timestamp)
// call. The caller supplies the authentic Play emitter/property constructor;
// this helper never invents menu or spatial emitter state.
bool submit_audio_source_cue_v1(
    const AudioSourceCueSubmissionV1&,
    dh2::audio::AudioGameplayRuntimeV42&,
    const dh2::audio::AudioGameplayRuntimeV42::PlainCommand&,
    std::string& error);

} // namespace dh::foundation::audio
