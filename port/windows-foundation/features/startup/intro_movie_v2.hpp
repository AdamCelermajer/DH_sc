#pragma once

// Intro movie (Preview 15): MPEG-1 video + MP2 audio in one MPEG Program Stream
// (intro_v1.mpg), converted offline from original-media/intro.mp4 by
// tools/convert_intro_video.ps1 (ffmpeg at asset-prep time only). Decoded at
// runtime by pl_mpeg (third_party/pl_mpeg/pl_mpeg.h, MIT, single C header).
// Plain C++ API: no platform calls, no FFmpeg or Media Foundation at runtime.

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh::foundation::startup {

struct IntroMovieInfo {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    double fps = 0.0;
    std::uint32_t sample_rate = 0;   // soundtrack rate (48000)
    std::uint32_t channels = 0;      // 2
};

class IntroMovieV2 {
public:
    IntroMovieV2() noexcept;
    ~IntroMovieV2();
    IntroMovieV2(const IntroMovieV2&) = delete;
    IntroMovieV2& operator=(const IntroMovieV2&) = delete;

    // Takes ownership of the file bytes and parses the MPEG-PS headers.
    bool open(std::vector<std::uint8_t> bytes, std::string& error);
    bool valid() const noexcept;
    const IntroMovieInfo& info() const noexcept;

    // Decodes the whole soundtrack as interleaved stereo int16 at info().sample_rate.
    // Independent of the picture cursor (uses its own decoder over the same bytes).
    bool decode_soundtrack(std::vector<std::int16_t>& pcm, std::string& error);

    // Picture cursor. Decodes frames up to time_seconds and writes the newest one with
    // presentation time <= time_seconds into rgba (width*height*4, row 0 = top, alpha 255)
    // when one became due: updated = true. updated = false means keep showing the previous
    // frame. Returns false only on a decode error (error is set).
    bool frame_at(double time_seconds, std::vector<std::uint8_t>& rgba, bool& updated, std::string& error);
    // True after the last frame has been delivered (or the stream failed).
    bool at_end() const noexcept;
    std::uint64_t frames_decoded() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace dh::foundation::startup
