#include "intro_segments_v1.hpp"

#include <cstdio>

using namespace dh::foundation::startup;

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); ++failures; } } while (0)

int main() {
    IntroSegmentTable t;
    CHECK(t.skippable_at(1.0));  // no table: legacy, whole movie skippable
    std::string err;
    CHECK(parse_intro_segments("# c\n0.0 0 logo\n7.3 1 story # tail\n", t, err));
    CHECK(t.loaded && t.segments.size() == 2);
    CHECK(!t.skippable_at(0.0));
    CHECK(!t.skippable_at(5.9));   // logo / black gap / fade-in
    CHECK(!t.skippable_at(7.29));
    CHECK(t.skippable_at(7.3));    // boundary belongs to the story
    CHECK(t.skippable_at(49.0));
    CHECK(t.segment_at(3.0) && t.segment_at(3.0)->name == "logo");
    IntroSegmentTable bad;
    CHECK(!parse_intro_segments("", bad, err) && !bad.loaded);
    CHECK(!parse_intro_segments("1.0 1 x\n", bad, err));        // must start at 0
    CHECK(!parse_intro_segments("0 0 a\n0 1 b\n", bad, err));   // ascending
    CHECK(!parse_intro_segments("0 2 a\n", bad, err));          // flag
    CHECK(!parse_intro_segments("abc\n", bad, err));
    if (failures) return 1;
    std::puts("startup_intro_segments_v1_tests passed");
    return 0;
}
