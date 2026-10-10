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
    check(caption_duration_ms_placeholder("") == kCaptionBaseMsPlaceholder, "empty caption uses the base hold");
    const std::string twenty_two(22, 'x');
    check(caption_duration_ms_placeholder(twenty_two) == 2500 + 22 * 60, "per-character term");
    const std::string huge(1000, 'x');
    check(caption_duration_ms_placeholder(huge) == kCaptionMaxMsPlaceholder, "hold is capped");
}

void queue_order_and_waiting() {
    CinematicRunner r;
    r.set_active(true);
    check(!r.waiting(), "nothing queued means WaitDialog does not block");
    r.enqueue(line(1, "first"));
    r.enqueue(line(2, "second"));
    check(r.waiting(), "a queued line blocks WaitDialog");
    check(r.current() && r.current()->text_id == 1, "first enqueued line is shown first");
    check(r.pending() == 2, "pending counts shown plus queued");
    const auto first = caption_duration_ms_placeholder("first");
    check(r.update(first - 1) == 0 && r.current()->text_id == 1, "line is held for its duration");
    check(r.update(1) == 1 && r.current() && r.current()->text_id == 2, "second line starts when the first ends");
    check(r.update(caption_duration_ms_placeholder("second")) == 1, "second line finishes");
    check(!r.waiting() && r.current() == nullptr, "WaitDialog releases after the last line");
    check(r.lines_shown() == 2, "each line counted once");
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
    check(r.build_frame().rects.empty() && r.build_frame().texts.empty(), "inactive frame is empty");
    r.set_active(true);
    r.set_skip_visible(true);
    auto frame = r.build_frame();
    check(frame.rects.size() == 1 && std::string(frame.rects[0].role) == "placeholder-skip", "SKIP alone when no caption");
    check(frame.texts.size() == 1 && frame.texts[0].text == kSkipLabelPlaceholder, "SKIP label");
    r.enqueue(line(9, "Is he... already dead?"));
    frame = r.build_frame();
    check(frame.rects.size() == 2 && std::string(frame.rects[1].role) == "placeholder-box", "caption box beside SKIP");
    check(frame.texts.size() == 2 && frame.texts[1].text == "Is he... already dead?", "caption text is the authored line");
}

}  // namespace

int main() {
    try {
        duration_policy();
        queue_order_and_waiting();
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
