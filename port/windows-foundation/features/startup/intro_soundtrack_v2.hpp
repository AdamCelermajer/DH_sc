#pragma once

// Intro soundtrack owner (Preview 15). Plays the movie's decoded MP2 soundtrack as one
// voice of the shared audio mixer (engine-audio AudioMixerV34) and exposes the mixer's
// output frame counter as the movie's master clock: the picture follows this clock, so
// lips and picture stay in step. The owner is portable (no platform calls); the host
// pumps the platform output (WinMM, SDL2, AAudio) that calls AudioMixerV34::render.

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../../../engine-audio/audio_mixer_v34.hpp"
#include "pcm_wav_v2.hpp"

namespace dh::foundation::startup {

class IntroSoundtrackV2 {
public:
    IntroSoundtrackV2() = default;
    ~IntroSoundtrackV2();
    IntroSoundtrackV2(const IntroSoundtrackV2&) = delete;
    IntroSoundtrackV2& operator=(const IntroSoundtrackV2&) = delete;

    // Configures the bank and posts the play command. pcm is interleaved stereo int16 at rate.
    // lead_frames delays the start on the mixer timeline; latency_frames is the amount of
    // rendered-but-not-yet-audible audio queued by the platform output (the clock subtracts it).
    bool start(dh2::audio::AudioMixerV34& mixer, const std::vector<std::int16_t>& pcm, std::uint32_t rate,
               std::uint64_t lead_frames, std::uint64_t latency_frames, std::string& error);

    // Soundtrack time (seconds) that is audible now. Negative until the voice has started.
    double seconds(const dh2::audio::AudioMixerV34& mixer) const;
    // Total soundtrack length in seconds (0 before start).
    double duration_seconds() const noexcept;
    bool started() const noexcept { return started_; }

    // Drains the mixer receipts (the mixer stops starting voices when its receipt queue is full).
    void drain_receipts(dh2::audio::AudioMixerV34& mixer) const;
    // Posts stop_all for this owner's voice. The voice is released once the mixer renders the command;
    // released() reports it. Keep the owner alive until then (the mixer reads the sample).
    void stop(dh2::audio::AudioMixerV34& mixer);
    bool released(const dh2::audio::AudioMixerV34& mixer) const;

private:
    std::unique_ptr<dh2::audio::AudioSampleV34> sample_;
    std::shared_ptr<const std::vector<std::uint8_t>> wav_;
    std::uint64_t startFrame_ = 0;
    std::uint64_t latencyFrames_ = 0;
    std::uint32_t rate_ = 0;
    std::uint64_t frames_ = 0;
    bool started_ = false;
};


} // namespace dh::foundation::startup
