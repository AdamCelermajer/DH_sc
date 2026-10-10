#pragma once

// Intro movie segment table (B054). Data next to the movie (intro_v1.segments.txt), not code in the
// player: each segment starts at a movie time and says whether the SKIP button is shown and a press
// skips. Original evidence: the Android GLMediaPlayer/MyVideoView ignores taps until the playback
// position passes 7300 ms (MyVideoView.o = 0x1C84, onTouchEvent), so the Gameloft logo part is
// uninterruptible and the story cinematic is skippable.
//
// File format, one segment per line (comments with #):  <start_seconds> <skippable 0|1> [name]
// Segments must be listed in ascending start order; the first one must start at 0.

#include <string>
#include <vector>

namespace dh::foundation::startup {

struct IntroSegment {
    double start_seconds = 0.0;
    bool skippable = true;
    std::string name;
};

struct IntroSegmentTable {
    std::vector<IntroSegment> segments;
    bool loaded = false;  // false: no table, the whole movie counts as skippable (legacy behavior)

    bool skippable_at(double movie_seconds) const noexcept;
    const IntroSegment* segment_at(double movie_seconds) const noexcept;
};

bool parse_intro_segments(const std::string& text, IntroSegmentTable& table, std::string& error);

} // namespace dh::foundation::startup
