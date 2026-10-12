// B144: HUD status line queue + authored itemname_text timeline.
#include "status_message_v1.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

using dh::foundation::hud_panels::StatusMessageQueueV1;
using dh::foundation::hud_panels::StatusMessageViewV1;

namespace {
int failures = 0;
void check(bool ok, const char* what) {
    if (!ok) { ++failures; std::cerr << "FAIL: " << what << '\n'; }
}
bool near(float a, float b, float tolerance) { return std::fabs(a - b) <= tolerance; }
constexpr std::uint32_t tick_ms = 34; // one 1/30 s movie frame per tick (cadence 33.33 ms)
}

int main() {
    check(StatusMessageQueueV1::frame_count() == 48, "authored clip has 48 frames");
    check(StatusMessageQueueV1::scroll_frame() == 8, "scroll label at frame 8");
    check(near(StatusMessageQueueV1::seconds_per_frame(), 1.f / 30.f, 1e-6f), "HUD movie runs at 30 fps");

    StatusMessageQueueV1 queue;
    StatusMessageViewV1 view;
    check(!queue.view(view), "empty queue has no view");
    queue.advance(1000);
    check(queue.size() == 0, "advancing an empty queue does nothing");

    queue.enqueue("Imbued Belt", 0x33CC33);
    queue.enqueue("Useless Mace", 0xFFFFFF);
    check(queue.size() == 2, "two queued messages");
    check(queue.view(view) && *view.text == "Imbued Belt" && view.rgb == 0x33CC33, "first message plays first, rarity colour kept");
    check(view.frame == 8 && view.alpha == 0.f, "message starts invisible at the scroll label");

    // Second enqueue must not restart the playing clip.
    int firstFrames = 0;
    float peakAlpha = 0, holdCentreX = 0, holdBaseline = 0, minAlphaAfterPeak = 1.f;
    bool seenFull = false;
    while (queue.size() == 2 && firstFrames < 200) {
        queue.advance(tick_ms);
        ++firstFrames;
        if (queue.size() != 2) break;
        queue.view(view);
        peakAlpha = std::fmax(peakAlpha, view.alpha);
        if (view.frame == 25) { holdCentreX = view.centre_x; holdBaseline = view.baseline_y; check(view.alpha == 1.f && near(view.y_scale, 1.f, 0.01f), "hold frame fully opaque, unscaled"); }
        if (view.alpha == 1.f) seenFull = true;
        if (seenFull) minAlphaAfterPeak = std::fmin(minAlphaAfterPeak, view.alpha);
    }
    check(firstFrames == 39, "first message ends after 39 movie frames (scroll 8 .. last frame 47)");
    check(peakAlpha == 1.f && minAlphaAfterPeak < 0.2f, "fades in to opaque and out again");
    // Fixed HUD placement from the SWF chain menu_HUD_0 -> HUDelements -> itemname_text -> container -> infotext.
    check(near(holdCentreX, 239.7f, 0.5f), "text centre x about the stage centre (239.7)");
    check(near(holdBaseline, 205.4f, 0.6f), "baseline y 205.4 of 320");

    check(queue.view(view) && *view.text == "Useless Mace" && view.rgb == 0xFFFFFF && view.frame == 8, "second message starts at the scroll label after the first ends");
    int secondFrames = 0;
    while (queue.size() == 1 && secondFrames < 200) { queue.advance(tick_ms); ++secondFrames; }
    check(secondFrames == 39 && queue.size() == 0, "second message completes and the queue empties");
    check(!queue.view(view), "nothing drawn after the queue drained");

    // Duration: 39 frames at 30 fps = 1.3 s.
    queue.enqueue("a", 0xFFFFFF);
    int ticks60 = 0;
    while (queue.size() == 1 && ticks60 < 1000) { queue.advance(16); ++ticks60; }
    check(ticks60 >= 74 && ticks60 <= 84, "about 1.3 s at 60 ticks per second");

    // At most one authored frame per application tick (catch_up=false): a huge dt is one frame.
    queue.enqueue("b", 0xFFFFFF);
    queue.advance(5000);
    queue.view(view);
    check(view.frame == 9, "a long tick advances a single authored frame");

    queue.clear();
    check(queue.size() == 0 && !queue.view(view), "clear");

    if (failures) return 1;
    std::cout << "status_message_v1 tests passed\n";
    return 0;
}
