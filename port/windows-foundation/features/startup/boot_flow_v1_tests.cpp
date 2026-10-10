#include "boot_flow_v1.hpp"

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

int main() {
    // Default boot starts with the intro movie (it contains the Gameloft logo).
    {
        BootFlowV1 flow;
        CHECK(flow.phase() == BootPhase::movie);
        flow.update(0.0, false);
        flow.update(40.0, false);  // the movie has no timer; only its end or a press leaves it
        CHECK(flow.phase() == BootPhase::movie);
    }
    // A press skips the movie to the title; the title waits for touch-to-continue.
    {
        BootFlowV1 flow(false);
        flow.update(4.5, true);
        CHECK(flow.phase() == BootPhase::title);
        flow.update(60.0, false);  // title does not time out
        CHECK(flow.phase() == BootPhase::title);
        flow.update(61.0, true);
        CHECK(flow.phase() == BootPhase::complete);
        CHECK(flow.complete());
        flow.update(62.0, true);  // duplicate presses after complete are inert
        CHECK(flow.phase() == BootPhase::complete);
    }
    // The movie ending naturally also moves to the title.
    {
        BootFlowV1 flow(false);
        flow.movie_finished(50.3);
        CHECK(flow.phase() == BootPhase::title);
        // Ignored outside the movie phase.
        flow.movie_finished(51.0);
        CHECK(flow.phase() == BootPhase::title);
    }
    // skip_boot starts complete; quit is terminal.
    {
        BootFlowV1 flow(true);
        CHECK(flow.complete());
        flow.update(1.0, true);
        CHECK(flow.complete());
    }
    {
        BootFlowV1 flow(false);
        flow.request_quit();
        CHECK(flow.phase() == BootPhase::quit);
        flow.update(5.0, true);
        CHECK(flow.phase() == BootPhase::quit);
    }
    // A backwards clock is clamped and cannot shorten a phase.
    {
        BootFlowV1 flow(false);
        flow.update(2.0, false);
        flow.update(1.0, false);
        CHECK(flow.phase() == BootPhase::movie);
        flow.movie_finished(0.5);  // clamped to 2.0, still a valid end
        CHECK(flow.phase() == BootPhase::title);
    }
    if (failures == 0) std::puts("boot_flow_v1 tests: all passed");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
