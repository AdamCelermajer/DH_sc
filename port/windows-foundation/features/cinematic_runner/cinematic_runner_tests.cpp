// P16 CINE: cinematic runner unit tests (caption queue, cutscene contract, SKIP hit test, frame content).
#include "cinematic_runner.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation::cinematic_runner;

namespace {

void check(bool value, const std::string& message) {
    if (!value) throw std::runtime_error(message);
}

CaptionLine line(std::int32_t id, const std::string& text) {
    CaptionLine c;
    c.text_id = id;
    c.style = 4;
    c.actor = 49;
    c.text = text;
    return c;
}

void duration_policy() {
    // 30 fps: show 11 steps = 367 ms, hide 20 steps = 667 ms, auto 38 steps = 1267 ms (rounded).
    check(frames_to_ms(kDialogShowFrames) == 367, "show animation 1->12 is 11 steps (367 ms)");
    check(frames_to_ms(kDialogHideFrames) == 667, "hide animation 19->39 is 20 steps (667 ms)");
    check(frames_to_ms(kDialogAutoFrames) == 1267, "non-skippable box plays 1->39 (1267 ms)");
    check(dialog_style_waits_for_tap(4) && dialog_style_waits_for_tap(11) && dialog_style_waits_for_tap(12),
          "MiddleBubble and the tutorial styles wait for a tap");
    check(!dialog_style_waits_for_tap(6) && !dialog_style_waits_for_tap(7) && !dialog_style_waits_for_tap(10) &&
          !dialog_style_waits_for_tap(19), "ScrollingDialogFull, TopBubble, Warning, QuestCompleted do not wait");
    check(dialog_style_waits_for_tap(99), "unknown style waits for a tap");
}

void tap_driven_queue() {
    CinematicRunner r;
    r.set_active(true);
    check(!r.waiting(), "nothing queued means WaitDialog does not block");
    r.enqueue(line(1, "first"));
    r.enqueue(line(2, "second"));
    check(r.waiting(), "a queued line blocks WaitDialog");
    check(r.current() && r.current()->text_id == 1, "first enqueued line is shown first");
    check(r.pending() == 2, "pending counts shown plus queued");
    check(r.update(frames_to_ms(kDialogShowFrames) - 1) == 0 && r.phase() == CinematicRunner::Phase::Showing, "show animation runs first");
    check(r.update(1) == 0 && r.phase() == CinematicRunner::Phase::WaitTap, "a skippable line stops at its show end");
    check(r.update(60000) == 0 && r.current()->text_id == 1, "a skippable line waits for the tap (no timeout)");
    check(r.tap() && r.phase() == CinematicRunner::Phase::Hiding, "tap starts the hide animation");
    check(r.update(frames_to_ms(kDialogHideFrames) - 1) == 0 && r.current()->text_id == 1, "hide animation holds the line");
    check(r.update(1) == 1 && r.current() && r.current()->text_id == 2, "the next line starts when the hide ends");
    check(r.lines_shown() == 2, "each line counted once");
    check(r.phase() == CinematicRunner::Phase::Showing, "the next line starts its own show animation");
    check(r.update(frames_to_ms(kDialogShowFrames)) == 0 && r.phase() == CinematicRunner::Phase::WaitTap, "second line waits at its show end");
    check(r.tap() && r.phase() == CinematicRunner::Phase::Hiding, "the second line is tapped away too");
    check(!r.tap(), "a tap during the hide is ignored");
    check(r.update(frames_to_ms(kDialogHideFrames)) == 1 && !r.waiting(), "second line ends after its hide");
    check(!r.waiting() && r.current() == nullptr, "WaitDialog releases after the last line");
}

void tap_during_show_hides_at_once() {
    CinematicRunner r;
    r.set_active(true);
    r.enqueue(line(1, "a"));
    check(r.tap() && r.phase() == CinematicRunner::Phase::Hiding, "tap during the show animation hides the box");
    check(r.update(frames_to_ms(kDialogHideFrames)) == 1 && !r.waiting(), "the hide ends the line");
}

void auto_line_plays_through() {
    CinematicRunner r;
    r.set_active(true);
    CaptionLine l = line(3, "title card");
    l.style = 10;   // Warning: not skippable
    r.enqueue(l);
    r.enqueue(line(4, "after"));
    check(r.phase() == CinematicRunner::Phase::AutoRun, "non-skippable line runs without input");
    check(!r.tap(), "a tap does nothing to a non-skippable line");
    check(r.update(frames_to_ms(kDialogAutoFrames) - 1) == 0, "auto line holds until frame 39");
    check(r.update(1) == 1 && r.current()->text_id == 4, "the next line starts after the auto line");
}

void auto_tap_harness() {
    CinematicRunner r;
    r.set_active(true);
    r.set_auto_tap_ms(500);
    r.enqueue(line(1, "a"));
    r.enqueue(line(2, "b"));
    // show 367 ms, wait 500 ms, tap, hide 667 ms: the first line ends at 1534 ms.
    std::size_t done = 0;
    for (int i = 0; i < 1533; ++i) done += r.update(1);
    check(done == 0 && r.current()->text_id == 1, "the hide after the auto tap is still running at 1533 ms");
    done += r.update(1);
    check(done == 1 && r.current()->text_id == 2, "second line starts after the auto-tapped hide");
}

void flush_and_end_drop_everything() {
    CinematicRunner r;
    r.set_active(true);
    r.set_skip_visible(true);
    r.enqueue(line(1, "a"));
    r.enqueue(line(2, "b"));
    r.flush();
    check(!r.waiting() && r.pending() == 0, "FlushMessages drops shown and queued lines");
    r.enqueue(line(3, "c"));
    r.set_active(false);
    check(!r.active() && !r.skip_visible() && !r.waiting(), "cutscene end clears lines and the SKIP control");
}

void skip_hit_follows_viewport() {
    CinematicRunner r;
    check(!r.skip_hit(20, 20, 960, 640), "no hit before the cutscene starts");
    r.set_active(true);
    check(!r.skip_hit(20, 20, 960, 640), "no hit while SKIP is not visible");
    r.set_skip_visible(true);
    // 960x640: scale 2, offset 0. Authored (10,10) is window (20,20).
    check(r.skip_hit(20, 20, 960, 640), "inside the SKIP bar (scale 2)");
    check(!r.skip_hit(400, 400, 960, 640), "outside the SKIP bar");
    // 1280x320: scale 1, offset 400. Authored (10,10) is window (410,10).
    check(r.skip_hit(410, 10, 1280, 320), "inside the SKIP bar (letterboxed width)");
    check(!r.skip_hit(20, 10, 1280, 320), "left letterbox margin is not the SKIP bar");
    const auto v = viewport_for(1280, 320);
    check(v.scale == 1.f && v.offset == 400.f, "viewport matches the PC HUD mapping");
}

void frame_content() {
    CinematicRunner r;
    check(r.build_frame().panels.empty() && r.build_frame().texts.empty(), "inactive frame is empty");
    r.set_active(true);
    r.set_skip_visible(true);
    auto frame = r.build_frame();
    check(frame.panels.size() == 1 && frame.panels[0] == &dh::foundation::hud_panels::skip_batches_v1(), "SKIP original art alone when no caption");
    check(frame.texts.size() == 1 && frame.texts[0].text == dh::foundation::hud_panels::kSkipLabelV1, "SKIP label");
    r.enqueue(line(9, "Is he... already dead?"));
    frame = r.build_frame();
    check(frame.panels.size() == 2 && frame.panels[1] == &dh::foundation::hud_panels::caption_batches_v1(), "caption box art beside SKIP");
    check(frame.texts.size() == 2 && frame.texts[1].text == "Is he... already dead?", "caption text is the authored line");
    check(frame.texts[1].align == 2, "caption text is centred as in the source EditText");
    // The caption box is drawn in every phase the original dialog is on screen, and gone once the line has ended.
    check(r.tap() && r.build_frame().panels.size() == 2, "caption box stays up while the hide animation runs");
    check(r.update(frames_to_ms(kDialogHideFrames)) == 1, "the line ends after its hide");
    frame = r.build_frame();
    check(frame.panels.size() == 1 && frame.texts.size() == 1, "caption box is gone after the line ends (SKIP only)");
}

}  // namespace

int main() {
    try {
        duration_policy();
        tap_driven_queue();
        tap_during_show_hides_at_once();
        auto_line_plays_through();
        auto_tap_harness();
        flush_and_end_drop_everything();
        skip_hit_follows_viewport();
        frame_content();
        std::cout << "cinematic_runner tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "cinematic_runner test failed: " << e.what() << '\n';
        return 1;
    }
}
