#include "intro_movie_v2.hpp"

#include <algorithm>
#include <cmath>
#include <utility>

// pl_mpeg (MIT, Dominic Szablewski) is C99. It is compiled here, once, as C++.
#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"

namespace dh::foundation::startup {

struct IntroMovieV2::Impl {
    std::vector<std::uint8_t> bytes;  // owned; both decoders point into it
    plm_t* picture = nullptr;         // picture cursor (audio decoding disabled)
    IntroMovieInfo info;
    bool valid = false;
    bool ended = false;
    bool pending = false;             // a decoded frame waits for its presentation time
    double pendingTime = 0.0;
    std::vector<std::uint8_t> pendingRgba;
    std::uint64_t frames = 0;

    ~Impl() {
        if (picture) plm_destroy(picture);
    }

    // Converts the decoder's current frame to RGBA8 (alpha 255). pl_mpeg leaves alpha untouched, so it is set here.
    void convert(plm_frame_t* frame, std::vector<std::uint8_t>& rgba) const {
        rgba.resize(std::size_t(info.width) * info.height * 4);
        plm_frame_to_rgba(frame, rgba.data(), int(info.width * 4));
        for (std::size_t i = 3; i < rgba.size(); i += 4) rgba[i] = 255;
    }
};

IntroMovieV2::IntroMovieV2() noexcept : impl_(std::make_unique<Impl>()) {}
IntroMovieV2::~IntroMovieV2() = default;

bool IntroMovieV2::open(std::vector<std::uint8_t> bytes, std::string& error) {
    impl_ = std::make_unique<Impl>();
    if (bytes.empty()) {
        error = "intro movie: empty file";
        return false;
    }
    impl_->bytes = std::move(bytes);
    // free_when_done = 0: the bytes stay owned by Impl (decode_soundtrack reuses them).
    impl_->picture = plm_create_with_memory(impl_->bytes.data(), impl_->bytes.size(), 0);
    if (!impl_->picture || !plm_has_headers(impl_->picture)) {
        error = "intro movie: not an MPEG-PS stream with MPEG-1 video and MP2 audio";
        return false;
    }
    if (plm_get_num_video_streams(impl_->picture) < 1 || plm_get_num_audio_streams(impl_->picture) < 1) {
        error = "intro movie: video or audio stream missing";
        return false;
    }
    impl_->info.width = static_cast<std::uint32_t>(plm_get_width(impl_->picture));
    impl_->info.height = static_cast<std::uint32_t>(plm_get_height(impl_->picture));
    impl_->info.fps = plm_get_framerate(impl_->picture);
    impl_->info.sample_rate = static_cast<std::uint32_t>(plm_get_samplerate(impl_->picture));
    impl_->info.channels = 2;
    if (impl_->info.width == 0 || impl_->info.height == 0 || !(impl_->info.fps > 0.0) || impl_->info.sample_rate == 0) {
        error = "intro movie: invalid headers";
        return false;
    }
    impl_->valid = true;
    return true;
}

bool IntroMovieV2::valid() const noexcept { return impl_->valid; }
const IntroMovieInfo& IntroMovieV2::info() const noexcept { return impl_->info; }
bool IntroMovieV2::at_end() const noexcept { return !impl_->valid || (impl_->ended && !impl_->pending); }
std::uint64_t IntroMovieV2::frames_decoded() const noexcept { return impl_->frames; }

bool IntroMovieV2::decode_soundtrack(std::vector<std::int16_t>& pcm, std::string& error) {
    pcm.clear();
    if (!impl_->valid) {
        error = "intro movie: not opened";
        return false;
    }
    plm_t* audio = plm_create_with_memory(impl_->bytes.data(), impl_->bytes.size(), 0);
    if (!audio || !plm_has_headers(audio)) {
        if (audio) plm_destroy(audio);
        error = "intro movie: audio pass cannot read headers";
        return false;
    }
    plm_set_video_enabled(audio, 0);
    plm_set_loop(audio, 0);
    pcm.reserve(std::size_t(impl_->info.sample_rate) * 2 * 60);
    while (plm_samples_t* samples = plm_decode_audio(audio)) {
        const std::size_t base = pcm.size();
        const std::size_t values = std::size_t(samples->count) * 2;
        pcm.resize(base + values);
        for (std::size_t i = 0; i < values; ++i) {
            const float v = std::isfinite(samples->interleaved[i]) ? samples->interleaved[i] : 0.0f;
            pcm[base + i] = static_cast<std::int16_t>(std::clamp(v, -1.0f, 1.0f) * 32767.0f);
        }
    }
    plm_destroy(audio);
    if (pcm.empty()) {
        error = "intro movie: no audio decoded";
        return false;
    }
    return true;
}

bool IntroMovieV2::frame_at(double time_seconds, std::vector<std::uint8_t>& rgba, bool& updated, std::string& error) {
    updated = false;
    if (!impl_->valid) {
        error = "intro movie: not opened";
        return false;
    }
    while (true) {
        if (impl_->pending) {
            if (impl_->pendingTime > time_seconds) break;  // next frame not due yet
            rgba.swap(impl_->pendingRgba);
            impl_->pending = false;
            updated = true;
            continue;
        }
        if (impl_->ended) break;
        plm_frame_t* frame = plm_decode_video(impl_->picture);
        if (!frame) {
            if (plm_has_ended(impl_->picture)) {
                impl_->ended = true;
                break;
            }
            error = "intro movie: video decode failed";
            return false;
        }
        ++impl_->frames;
        if (frame->time <= time_seconds) {
            impl_->convert(frame, rgba);  // due now; a later due frame replaces it
            updated = true;
        } else {
            impl_->convert(frame, impl_->pendingRgba);
            impl_->pendingTime = frame->time;
            impl_->pending = true;
            break;
        }
    }
    return true;
}

} // namespace dh::foundation::startup
