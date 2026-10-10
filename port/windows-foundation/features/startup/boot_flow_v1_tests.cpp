#include "boot_flow_v1.hpp"

#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace dh::foundation::startup;

static int failures = 0;
#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                          \
        }                                                                        \
    } while (0)

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main() {
    // Default timing: fade 0.5 s, hold 2.0 s, fade 0.5 s -> logo lasts 3.0 s.
    {
        BootFlowV1 flow;
        CHECK(flow.phase() == BootPhase::logo);
        CHECK(near(static_cast<float>(flow.logo_total_seconds()), 3.0f));
        CHECK(near(flow.logo_alpha(0.0), 0.0f));
        CHECK(near(flow.logo_alpha(0.25), 0.5f));  // mid fade-in
        CHECK(near(flow.logo_alpha(1.0), 1.0f));   // hold
        CHECK(near(flow.logo_alpha(2.75), 0.5f));  // mid fade-out
        // A press during the logo is ignored: the logo is not skippable.
        flow.update(0.1, true);
        CHECK(flow.phase() == BootPhase::logo);
        flow.update(2.99, false);
        CHECK(flow.phase() == BootPhase::logo);
        flow.update(3.0, false);
        CHECK(flow.phase() == BootPhase::movie);
    }
    // Movie: a press skips it to the title; movie_finished also moves on.
    {
        BootFlowV1 flow({}, false);
        flow.update(3.0, false);
        CHECK(flow.phase() == BootPhase::movie);
        flow.update(4.0, false);
        CHECK(flow.phase() == BootPhase::movie);  // no timer for the movie
        flow.update(4.5, true);                   // skip button
        CHECK(flow.phase() == BootPhase::title);
    }
    {
        BootFlowV1 flow({}, false);
        flow.update(3.0, false);
        flow.movie_finished(20.0);  // stream ended naturally
        CHECK(flow.phase() == BootPhase::title);
    }
    // Movie finished while not in the movie phase is ignored.
    {
        BootFlowV1 flow({}, false);
        flow.movie_finished(0.0);
        CHECK(flow.phase() == BootPhase::logo);
    }
    // Title waits for touch-to-continue, duplicate presses after complete are inert.
    {
        BootFlowV1 flow({}, false);
        flow.update(3.0, false);
        flow.movie_finished(10.0);
        flow.update(60.0, false);  // title does not time out
        CHECK(flow.phase() == BootPhase::title);
        flow.update(61.0, true);
        CHECK(flow.phase() == BootPhase::complete);
        CHECK(flow.complete());
        flow.update(62.0, true);
        CHECK(flow.phase() == BootPhase::complete);
    }
    // skip_boot starts complete, quit is terminal.
    {
        BootFlowV1 flow({}, true);
        CHECK(flow.complete());
        flow.update(1.0, true);
        CHECK(flow.complete());
    }
    {
        BootFlowV1 flow({}, false);
        flow.request_quit();
        CHECK(flow.phase() == BootPhase::quit);
        flow.update(5.0, true);
        CHECK(flow.phase() == BootPhase::quit);
    }
    // A backwards clock is clamped and cannot shorten a phase.
    {
        BootFlowV1 flow({}, false);
        flow.update(2.0, false);
        flow.update(1.0, false);  // clamped to 2.0
        CHECK(flow.phase() == BootPhase::logo);
        flow.update(3.0, false);
        CHECK(flow.phase() == BootPhase::movie);
    }
    // Custom timing, no hold and no fade: logo ends exactly at total time.
    {
        BootTiming t;
        t.logo_fade_seconds = 0.0;
        t.logo_hold_seconds = 0.0;
        BootFlowV1 flow(t, false);
        CHECK(flow.logo_total_seconds() == 0.0);
        flow.update(0.0, false);  // elapsed 0 >= total 0
        CHECK(flow.phase() == BootPhase::movie);
    }

    if (failures == 0) std::puts("boot_flow_v1 tests: all passed");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
