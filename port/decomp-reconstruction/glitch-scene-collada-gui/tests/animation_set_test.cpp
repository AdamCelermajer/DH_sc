#include "glitch-scene-collada-gui/animation_set.hpp"

#include <cstdio>

using namespace glitch::collada;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    std::uint32_t channels[] = {7, 8, 9};
    std::uint32_t tracks[] = {100, 101};
    std::uint32_t lengths[] = {30, 60};
    std::uint32_t starts[] = {1, 2};
    std::uint32_t ends[] = {10, 20, 30};
    CAnimationSet s;
    s.channels = channels;
    s.tracks = tracks;
    s.lengths_begin = lengths;
    s.lengths_end = lengths + 2;
    s.starts_begin = starts;
    s.starts_end = starts + 2;
    s.ends_begin = ends;
    s.ends_end = ends + 3;
    s.animatedTrackCount = 5;
    CHECK(getChannel(&s, 1) == 8);
    CHECK(getAnimationTrackEx(&s, 1) == 101);
    CHECK(getAnimatedTrackCount(&s) == 5);
    CHECK(getAnimationLength(&s, 1) == 60);
    CHECK(getAnimationLength(&s, 2) == 0);   // out of range
    CHECK(getAnimationStart(&s, 0) == 1);
    CHECK(getAnimationEnd(&s, 2) == 30);
    CHECK(getAnimationEnd(&s, 3) == 0);
    CHECK(remAnimation(&s, nullptr) == 0);
    std::printf("%s\n", failures ? "animation_set: FAILED" : "animation_set: ok");
    return failures != 0;
}
