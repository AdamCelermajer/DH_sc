#include "../control_policy.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace {
void require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "Irrlicht SWAMP control policy failed: %s\n", message);
    std::exit(1);
}
}

int main() {
    using dh2::irrlicht_swamp::FixedStepAccumulator;
    using dh2::irrlicht_swamp::prince_walk_requested;
    using dh2::irrlicht_swamp::source_stick_from_screen_delta;
    using dh2::irrlicht_swamp::utf8_codepoints;

    const auto status_glyph = utf8_codepoints("blocked \xc2\xb7 stop");
    require(status_glyph.size() == 14 && status_glyph[8] == 0x00b7U,
            "UTF-8 status punctuation must decode to U+00B7, not two byte glyphs");
    const auto truncated = utf8_codepoints("bad \xe2\x80");
    require(truncated.size() == 6 && truncated[4] == 0xfffdU &&
            truncated[5] == 0xfffdU,
            "truncated UTF-8 must be replaced instead of byte-cast");

    auto stick = source_stick_from_screen_delta(0.0f, 0.0f, 100.0f);
    require(stick.x == 0.0f && stick.y == 0.0f, "center drag must remain idle");
    stick = source_stick_from_screen_delta(100.0f, 0.0f, 100.0f);
    require(stick.x == 1.0f && stick.y == 0.0f, "right drag must map to source +X");
    stick = source_stick_from_screen_delta(0.0f, -100.0f, 100.0f);
    require(stick.x == 0.0f && stick.y == 1.0f, "up drag must map to source +Y");
    stick = source_stick_from_screen_delta(300.0f, 400.0f, 100.0f);
    require(std::fabs(std::hypot(stick.x, stick.y) - 1.0f) < 1.0e-6f,
            "diagonal drag must clamp to unit length");
    stick = source_stick_from_screen_delta(1.0f, 1.0f, 0.0f);
    require(stick.x == 0.0f && stick.y == 0.0f, "invalid radius must stop movement");
    require(!prince_walk_requested(0.0f, 0.0f, true),
            "released zero input must select source Idle");
    require(prince_walk_requested(0.7f, 0.0f, true),
            "accepted directional input must select source Walk");
    require(!prince_walk_requested(0.7f, 0.0f, false),
            "blocked movement must return source animation to Idle");

    FixedStepAccumulator clock;
    require(clock.advance(19000000U) == 0, "sub-step frame must not tick");
    require(clock.advance(1000000U) == 1, "20 ms must yield one tick");
    require(clock.remainder_nanoseconds() == 0, "completed step must clear remainder");
    require(clock.advance(250000000U) == 5, "long frame must clamp to five ticks");
    require(clock.advance(0) == 0, "zero elapsed time must not tick");
    clock.advance(19000000U);
    clock.reset();
    require(clock.advance(1000000U) == 0,
            "pause reset must discard pre-pause accumulator remainder");

    std::puts("Irrlicht SWAMP control policy: touch axes, diagonal clamp, Idle release, blocked Idle, fixed-step, frame cap and pause reset pass");
    return 0;
}
