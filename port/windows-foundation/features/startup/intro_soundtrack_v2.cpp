#include "intro_soundtrack_v2.hpp"

#include "pcm_wav_v2.hpp"

#include <cstring>

namespace dh::foundation::startup {
namespace {

constexpr std::uint64_t kBankIdIntro = 0x1001;  // private bank: the intro never competes with game sources
constexpr std::uint64_t kTokenIntro = 0x1001;

} // namespace

IntroSoundtrackV2::~IntroSoundtrackV2() = default;

bool IntroSoundtrackV2::start(dh2::audio::AudioMixerV34& mixer, const std::vector<std::int16_t>& pcm, std::uint32_t rate,
                              std::uint64_t lead_frames, std::uint64_t latency_frames, std::string& error) {
    if (started_) {
        error = "intro soundtrack already started";
        return false;
    }
    if (pcm.size() < 2 || rate == 0) {
        error = "intro soundtrack: no samples";
        return false;
    }
    if (!mixer.set_rate(rate)) {
        error = "intro soundtrack: mixer rate rejected";
        return false;
    }
    wav_ = std::make_shared<const std::vector<std::uint8_t>>(make_pcm16_wav(pcm, rate, 2));
    sample_ = std::make_unique<dh2::audio::AudioSampleV34>();
    std::string openError;
    if (!dh2::audio::audio_sample_open_v34(wav_, *sample_, openError)) {
        error = "intro soundtrack sample: " + openError;
        sample_.reset();
        return false;
    }
    dh2::audio::AudioBankV34 bank{};
    bank.id = static_cast<std::int32_t>(kBankIdIntro);
    bank.minimum_priority = -128;
    bank.max_playbacks = 1;
    bank.behavior = 3;
    if (!mixer.configure_banks(&bank, 1)) {
        error = "intro soundtrack: bank configuration refused (mixer already playing?)";
        sample_.reset();
        return false;
    }
    startFrame_ = mixer.output_frame() + lead_frames;
    latencyFrames_ = latency_frames;
    rate_ = rate;
    frames_ = pcm.size() / 2;
    dh2::audio::AudioCommandV34 play;
    play.kind = dh2::audio::AudioCommandKindV34::play;
    play.token = kTokenIntro;
    play.start_frame = startFrame_;
    play.sample = sample_.get();
    play.bank = static_cast<std::int32_t>(kBankIdIntro);
    play.priority = 0;
    play.volume_group = 0;
    if (!mixer.post(play)) {
        error = "intro soundtrack: mixer command queue full";
        sample_.reset();
        return false;
    }
    started_ = true;
    return true;
}

double IntroSoundtrackV2::duration_seconds() const noexcept {
    return rate_ ? double(frames_) / double(rate_) : 0.0;
}

double IntroSoundtrackV2::seconds(const dh2::audio::AudioMixerV34& mixer) const {
    if (!started_ || rate_ == 0) return -1.0;
    const std::uint64_t rendered = mixer.output_frame();
    // Audible position = rendered frames minus what the platform output still holds queued.
    const std::uint64_t audible = rendered > latencyFrames_ ? rendered - latencyFrames_ : 0;
    if (audible <= startFrame_) return -1.0;
    return double(audible - startFrame_) / double(rate_);
}

void IntroSoundtrackV2::drain_receipts(dh2::audio::AudioMixerV34& mixer) const {
    dh2::audio::AudioReceiptV34 receipt;
    while (mixer.receipt(receipt)) {
    }
}

void IntroSoundtrackV2::stop(dh2::audio::AudioMixerV34& mixer) {
    if (!started_) return;
    dh2::audio::AudioCommandV34 stop;
    stop.kind = dh2::audio::AudioCommandKindV34::stop_all;
    mixer.post(stop);
}

bool IntroSoundtrackV2::released(const dh2::audio::AudioMixerV34& mixer) const {
    return !started_ || mixer.active_voices() == 0;
}

} // namespace dh::foundation::startup
