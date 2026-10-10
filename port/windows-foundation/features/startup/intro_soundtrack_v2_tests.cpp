// B065: intro soundtrack owner against the real shared mixer: audible clock, skip (stop) silences the
// output and releases the voice within the render the stop is posted for, a late/duplicate stop is harmless,
// and a silent head (the Gameloft logo has no sound in the source) is not mistaken for a failed start.
#include "intro_soundtrack_v2.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <vector>

using namespace dh::foundation::startup;
using dh2::audio::AudioMixerV34;

static int failures = 0;
#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                          \
        }                                                                        \
    } while (0)

namespace {
constexpr std::uint32_t kRate = 48000;

// 2 s: first second silent (logo), then a constant-amplitude tone.
std::vector<std::int16_t> make_pcm() {
    std::vector<std::int16_t> pcm(std::size_t(kRate) * 2 * 2, 0);
    for (std::size_t i = kRate; i < std::size_t(kRate) * 2; ++i) {
        const auto v = static_cast<std::int16_t>(12000.0 * std::sin(double(i) * 0.05));
        pcm[i * 2] = v;
        pcm[i * 2 + 1] = v;
    }
    return pcm;
}

// Renders frames in 512-frame blocks (the platform ring unit); returns the peak |sample|.
float render(AudioMixerV34& mixer, unsigned frames) {
    std::vector<float> buffer(512 * 2);
    float peak = 0.0f;
    for (unsigned done = 0; done < frames; done += 512) {
        mixer.render(buffer.data(), 512);
        for (float v : buffer) peak = std::fmax(peak, std::fabs(v));
    }
    return peak;
}
} // namespace

int main() {
    const auto pcm = make_pcm();
    // Skip inside the story: the voice is audible, then stop_all silences everything and releases the voice.
    {
        auto mixerOwner = std::make_unique<AudioMixerV34>();
        AudioMixerV34& mixer = *mixerOwner;
        IntroSoundtrackV2 track;
        std::string error;
        CHECK(track.start(mixer, pcm, kRate, 0, 0, error));
        CHECK(track.started());
        CHECK(std::fabs(track.duration_seconds() - 2.0) < 1e-6);
        CHECK(track.seconds(mixer) < 0.0);              // nothing rendered yet: clock not audible
        CHECK(render(mixer, 512 * 10) == 0.0f);          // the logo head is silent in the source: voice active, no sound
        CHECK(mixer.active_voices() == 1);
        CHECK(track.seconds(mixer) > 0.0);               // the clock runs although the head is silent
        const float tone = render(mixer, kRate);
        CHECK(tone > 0.05f);                              // the story part is audible
        CHECK(!track.released(mixer));
        track.stop(mixer);                                // SKIP
        const float after = render(mixer, 512);           // the very next ring block
        CHECK(mixer.active_voices() == 0);
        CHECK(track.released(mixer));
        CHECK(after == 0.0f || after < 0.05f);            // fade-free retirement: nothing queued after the stop block
        CHECK(render(mixer, 512 * 8) == 0.0f);            // the whole 85 ms ring after the stop is silent
        track.drain_receipts(mixer);
        track.stop(mixer);                                // duplicate stop is harmless
        CHECK(render(mixer, 512) == 0.0f && mixer.active_voices() == 0);
    }
    // Natural end: the voice retires by itself and the stop afterwards stays a no-op.
    {
        auto mixerOwner = std::make_unique<AudioMixerV34>();
        AudioMixerV34& mixer = *mixerOwner;
        IntroSoundtrackV2 track;
        std::string error;
        CHECK(track.start(mixer, pcm, kRate, 0, 0, error));
        render(mixer, kRate * 2 + 4096);
        track.drain_receipts(mixer);
        CHECK(mixer.active_voices() == 0);
        CHECK(track.released(mixer));
        track.stop(mixer);
        CHECK(render(mixer, 512) == 0.0f);
    }
    // Second start on the same owner is refused (one voice per owner).
    {
        auto mixerOwner = std::make_unique<AudioMixerV34>();
        AudioMixerV34& mixer = *mixerOwner;
        IntroSoundtrackV2 track;
        std::string error;
        CHECK(track.start(mixer, pcm, kRate, 0, 0, error));
        CHECK(!track.start(mixer, pcm, kRate, 0, 0, error));
    }
    if (failures == 0) std::puts("intro_soundtrack_v2 tests: all passed");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
