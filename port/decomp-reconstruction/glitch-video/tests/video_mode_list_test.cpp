#include "glitch-video/video_mode_list.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    glitch::video::CVideoModeList empty;
    CHECK(empty.getVideoModeCount() == 0);
    CHECK(empty.getVideoModeResolution(0).Width == 0);
    CHECK(empty.getVideoModeDepth(0) == 0);
    CHECK(empty.getDesktopDepth() == 0);

    glitch::video::CVideoModeList list(
        {{{640, 480}, 32}, {{1280, 720}, 24}, {{1920, 1080}, 32}},
        {1920, 1080}, 32);
    CHECK(list.getVideoModeCount() == 3);
    CHECK(list.getVideoModeResolution(1).Width == 1280);
    CHECK(list.getVideoModeResolution(1).Height == 720);
    CHECK(list.getVideoModeDepth(1) == 24);
    CHECK(list.getVideoModeResolution(3).Width == 0);   // past the end: zeroed
    CHECK(list.getVideoModeDepth(-1) == 0);            // negative: zeroed
    CHECK(list.getDesktopResolution().Width == 1920);
    CHECK(list.getDesktopDepth() == 32);

    glitch::video::CVideoModeList added;
    added.addMode({800, 600}, 32);
    added.addMode({800, 600}, 32);                     // duplicate: ignored
    added.addMode({800, 600}, 16);                     // same size, other depth: kept
    CHECK(added.getVideoModeCount() == 2);
    CHECK(added.getVideoModeDepth(1) == 16);

    std::printf("%s\n", failures ? "video_mode_list: FAILED" : "video_mode_list: ok");
    return failures != 0;
}
