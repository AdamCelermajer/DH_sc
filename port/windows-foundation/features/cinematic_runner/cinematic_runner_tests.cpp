// P16 CINE: cinematic runner unit tests (caption queue, cutscene contract, SKIP hit test, frame content).
#include "cinematic_runner.hpp"
#include "dialog_tables.hpp"

#include <cmath>
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
    check(!dialog_style_waits_for_tap(3), "EnterLocationDialog title runs by itself (reference video)");
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
    check(r.tap() && r.update(0) == 1 && !r.waiting(), "B068 PC adaptation: a tap during the hide ends it at once (the next line / the end follows)");
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

// B068: the caption box follows the line's DialogStyles variant; the name plate shows the speaker; a tap during the hide runs on.
void variant_and_speaker() {
    using namespace dh::foundation;
    CinematicRunner r;
    r.set_active(true);
    CaptionLine l = line(5, "He's dead alright...");
    l.style_label = "MiddleBubble";
    l.speaker = "Celeste";
    r.enqueue(l);
    auto frame = r.build_frame();
    const hud_panels::DialogVariantV1* middle = nullptr;
    const hud_panels::DialogVariantV1* chest = nullptr;
    for (const auto& v : hud_panels::dialog_variants_v1()) {
        if (std::string(v.label) == "MiddleBubble") middle = &v;
        if (std::string(v.label) == "ChestTuto") chest = &v;
    }
    check(middle && chest && !middle->batches->empty(), "MiddleBubble and ChestTuto variants are exported");
    check(frame.panels.size() == 1 && frame.panels[0] == middle->batches, "the MiddleBubble line draws the MiddleBubble art");
    check(frame.texts.size() == 2, "line text and speaker name are both drawn");
    float lineLeft = 0, lineRight = 0;
    bool haveName = false;
    for (const auto& t : frame.texts) {
        if (t.text == "Celeste") haveName = true;
        else { lineLeft = t.x; lineRight = t.x + t.w; }
    }
    check(haveName, "speaker name plate text is the resolved name");
    // MiddleBubble text band is centred on the stage (3.9..478.7), the ChestTuto tutorial band leaves room for its icon (3.9..370.3).
    check(std::abs((lineLeft + lineRight) * 0.5f - 241.3f) < 2.f, "MiddleBubble text is centred on the 480 px stage");
    CinematicRunner t;
    t.set_active(true);
    CaptionLine tuto = line(6, "When wielding a melee weapon");
    tuto.style_label = "ChestTuto";
    tuto.speaker = "Tutorial";
    t.enqueue(tuto);
    const auto tf = t.build_frame();
    check(tf.panels.size() == 1 && tf.panels[0] == chest->batches, "a tutorial style draws its own variant");
    float tutoRight = 0;
    for (const auto& x : tf.texts) if (x.text != "Tutorial") tutoRight = x.x + x.w;
    check(tutoRight < 372.f, "the tutorial text band stops left of the icon");
    // No speaker: no name text.
    CinematicRunner n;
    n.set_active(true);
    CaptionLine anon = line(7, "narration");
    anon.actor = -1;
    anon.style_label = "MiddleBubble";
    n.enqueue(anon);
    check(n.build_frame().texts.size() == 1, "no speaker, no name plate text");
    // Unknown style label falls back to the default variant art.
    CinematicRunner u;
    u.set_active(true);
    CaptionLine odd = line(8, "x");
    odd.style_label = "NoSuchStyle";
    u.enqueue(odd);
    check(u.build_frame().panels.size() == 1 && u.build_frame().panels[0] == &hud_panels::caption_batches_v1(), "unknown style falls back to the default art");
}

void dialog_tables() {
    std::vector<std::uint8_t> bytes;
    const auto word = [&](std::uint32_t v) { for (int i = 0; i < 4; ++i) bytes.push_back(std::uint8_t(v >> (8 * i))); };
    word(2); word(49); word(0); word(0x170117); word(28); word(3); for (char c : std::string("abc")) bytes.push_back(std::uint8_t(c)); word(0x170088);
    std::vector<DialogActorRow> rows;
    std::string e;
    check(decode_dialog_actors(bytes, rows, e) && rows.size() == 2 && rows[0].name_string_id == 0x170117 && rows[1].frame_label == "abc", "DialogActors rows decode");
    bytes.pop_back();
    check(!decode_dialog_actors(bytes, rows, e) && rows.empty(), "a truncated DialogActors table is rejected");
    std::vector<std::uint8_t> cst;
    const auto cword = [&](std::uint32_t v) { for (int i = 0; i < 4; ++i) cst.push_back(std::uint8_t(v >> (8 * i))); };
    const auto cstr = [&](const std::string& s) { cword(std::uint32_t(s.size())); for (char c : s) cst.push_back(std::uint8_t(c)); };
    cword(1); cstr("DialogStyles"); cword(2); cstr("MiddleBubble"); cword(4); cstr("ChestTuto"); cword(13);
    std::map<std::int32_t, std::string> styles;
    check(decode_dialog_styles(cst, styles, e) && styles.at(4) == "MiddleBubble" && styles.at(13) == "ChestTuto", "DialogStyles constants decode");
    cst.resize(cst.size() - 2);
    check(!decode_dialog_styles(cst, styles, e), "a truncated DialogStyles group is rejected");
}

}  // namespace

// SKIP16: the hit area is the exported SKIP art bounds (every triangle of the drawn control), and it follows the same
// letterbox mapping as the draw in main.cpp at every window size: the centre of the art hits, the margins miss.
void skip_hit_art_at_window_sizes() {
    const auto art = skip_art_layout();
    check(art.x > 3.0f && art.x < 3.6f && art.y == 0.f && art.w > 178.f && art.w < 180.f && art.h > 54.f && art.h < 55.f,
          "SKIP hit area is the exported art bounds (3.35..182.35 x 0..54.45)");
    CinematicRunner r;
    r.set_active(true);
    r.set_skip_visible(true);
    const float cx = art.x + art.w * 0.5f, cy = art.y + art.h * 0.5f;
    const std::pair<float, float> sizes[] = {{480, 320}, {960, 640}, {1280, 720}, {1920, 1080}, {800, 600}, {1000, 320}, {320, 480}, {1280, 1024}};
    for (const auto& size : sizes) {
        const auto v = viewport_for(size.first, size.second);
        const float wx = (cx + v.offset) * v.scale, wy = cy * v.scale;
        check(r.skip_hit(wx, wy, size.first, size.second), "SKIP centre hits at window " + std::to_string(int(size.first)) + "x" + std::to_string(int(size.second)));
        const float left = (art.x - 2.f + v.offset) * v.scale, bottom = (art.y + art.h + 2.f) * v.scale;
        check(!r.skip_hit(left, wy, size.first, size.second), "two authored px left of the art misses at " + std::to_string(int(size.first)) + "x" + std::to_string(int(size.second)));
        check(!r.skip_hit(wx, bottom, size.first, size.second), "two authored px below the art misses at " + std::to_string(int(size.first)) + "x" + std::to_string(int(size.second)));
    }
}

int main() {
    try {
        duration_policy();
        skip_hit_art_at_window_sizes();
        tap_driven_queue();
        tap_during_show_hides_at_once();
        auto_line_plays_through();
        auto_tap_harness();
        flush_and_end_drop_everything();
        skip_hit_follows_viewport();
        frame_content();
        variant_and_speaker();
        dialog_tables();
        std::cout << "cinematic_runner tests passed\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "cinematic_runner test failed: " << e.what() << '\n';
        return 1;
    }
}
