// v21.8 w4-hud: original-touch gameplay HUD layout and hit testing.
// Expected centres are the authored dqhud_droid menu_HUD_2 values of features/hud_panels/probe_touch_hud_layout_v1.py
// (stage = 480x320 authored px) and were matched against the supplied original mobile screenshot.
#include "pc_gameplay_hud_v1.hpp"
#include "../platform_input/semantic_input.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

PcSkillHudFrameV1 source_frame() {
    PcSkillHudFrameV1 source;
    source.character_state_id = "touch-test-character";
    source.source_actor = 7;
    source.equipment_set = 0;
    const std::array<std::uint32_t, 3> slots{{2, 0, 1}};
    const std::array<std::string, 3> icons{{"sword_explosion", "fire_right", "swipe"}};
    for (unsigned i = 0; i < 3; ++i) {
        auto& cell = source.left_middle_right[i];
        cell.physical_position = i;
        cell.pc_key_number = i + 1;
        cell.source_slot = slots[i];
        cell.key_label = std::to_string(i + 1);
        cell.assigned = true;
        cell.source_icon_key = icons[i];
    }
    return source;
}

struct Spot { const char* name; float x, y; platform_input::Control control; std::uint64_t item; };

platform_input::Hit hit_at(const PcGameplayHudPresentationV1& frame, float x, float y) {
    platform_input::Hit hit;
    std::string error;
    check(pc_gameplay_hud_hit_test_v1(frame, x, y, hit, error), error.c_str());
    return hit;
}

bool near(float a, float b, float tolerance) { return std::abs(a - b) <= tolerance; }

float circle_extent(const frontend::art::HitRegion& region, bool x_axis, bool max_side) {
    float value = max_side ? -1e9f : 1e9f;
    for (const auto& v : region.triangles) {
        const float c = x_axis ? v.x : v.y;
        value = max_side ? std::max(value, c) : std::min(value, c);
    }
    return value;
}
}

int main() {
    try {
        using platform_input::Control;
        const auto source = source_frame();
        PcTouchGameplayHudLayoutV1 layout;
        layout.active_faery_id = 0;
        layout.potion_count = PcGameplayHudLayoutV1::PotionCount{source.character_state_id, source.source_actor, 5};
        PcGameplayHudPresentationV1 frame;
        std::string error;
        check(compose_pc_touch_gameplay_hud_v1(source, 1, layout, frame, error), error.c_str());
        check(frame.art.bitmap_ids.size() == frame.art.batches.size() &&
              frame.art.batch_colors.size() == frame.art.batches.size(), "art vectors must stay parallel");
        check(frame.art.hit_regions.size() == 5 && frame.extra_hits.size() == 2, "five buttons plus attack and music");

        // 1. Authored centres (stage px) and the receiver each one returns. Physical left/middle/right skills are the
        //    authored btn_skill3 / btn_skill1 / btn_skill2 (source slots 2, 0, 1).
        const Spot spots[] = {
            {"skill left (btn_skill3)",   346.9f, 258.3f, Control::skill1, 1},
            {"skill middle (btn_skill1)", 369.2f, 197.8f, Control::skill2, 2},
            {"skill right (btn_skill2)",  434.2f, 186.3f, Control::skill3, 3},
            {"faery (btn_spell)",         436.1f, 123.7f, Control::spell,  4},
            {"potion (btn_potion)",       445.7f,  34.7f, Control::potion, 5},
            {"attack (btn_interact)",     435.4f, 257.1f, Control::attack, 0},
            {"music (btn_itunes)",        372.4f,  29.3f, Control::music,  0},
        };
        for (const auto& spot : spots) {
            const auto hit = hit_at(frame, spot.x, spot.y);
            if (hit.control != spot.control || hit.item != spot.item) {
                std::cerr << "hit mismatch for " << spot.name << '\n';
                throw std::runtime_error("original-touch hit test returned the wrong receiver");
            }
        }

        // 2. The old desktop bottom row and the (undrawn) joystick area are empty.
        check(hit_at(frame, 128.0f, 270.0f).control == Control::none, "desktop skill row must not exist in the touch layout");
        check(hit_at(frame, 240.0f, 270.0f).control == Control::none, "desktop skill row must not exist in the touch layout");
        check(hit_at(frame, 72.1f, 247.2f).control == Control::none, "joystick centre must not be a receiver");
        check(hit_at(frame, 20.0f, 20.0f).control == Control::none, "empty stage must not be a receiver");
        check(frame.joystick_zone && (*frame.joystick_zone)[0] < 72.1f && (*frame.joystick_zone)[2] > 72.1f &&
              (*frame.joystick_zone)[1] < 247.2f && (*frame.joystick_zone)[3] > 247.2f,
              "joystick seam must expose the authored stick hit zone around the authored joystick");
        for (const auto& batch : frame.art.batches) check(batch.role.find("joystick") == std::string::npos, "no joystick art");

        // 3. Everything lies inside the authored stage and the circles do not overlap each other.
        std::vector<const frontend::art::HitRegion*> regions;
        for (const auto& r : frame.art.hit_regions) regions.push_back(&r);
        for (const auto& e : frame.extra_hits) regions.push_back(&e.region);
        for (const auto* r : regions) {
            check(circle_extent(*r, true, false) >= 0 && circle_extent(*r, true, true) <= 480 &&
                  circle_extent(*r, false, false) >= 0 && circle_extent(*r, false, true) <= 320,
                  "hit circle outside the authored 480x320 stage");
        }
        for (std::size_t i = 0; i < regions.size(); ++i) for (std::size_t j = i + 1; j < regions.size(); ++j) {
            const float cxi = (circle_extent(*regions[i], true, false) + circle_extent(*regions[i], true, true)) * .5f;
            const float cyi = (circle_extent(*regions[i], false, false) + circle_extent(*regions[i], false, true)) * .5f;
            const float cxj = (circle_extent(*regions[j], true, false) + circle_extent(*regions[j], true, true)) * .5f;
            const float cyj = (circle_extent(*regions[j], false, false) + circle_extent(*regions[j], false, true)) * .5f;
            const float ri = (circle_extent(*regions[i], true, true) - circle_extent(*regions[i], true, false)) * .5f;
            const float rj = (circle_extent(*regions[j], true, true) - circle_extent(*regions[j], true, false)) * .5f;
            check(std::hypot(cxi - cxj, cyi - cyj) >= ri + rj - 1.5f, "touch hit circles overlap");
        }

        // 4. Original layout facts: all five right-hand controls are in the right half, the top-right pair is the
        //    music note and the potion, no key legends exist, the potion count is the authored counter badge.
        for (const auto& spot : spots) check(spot.x > 340.0f, "right cluster is in the right part of the stage");
        check(frame.art.text_fields.size() == 1 && frame.art.text_fields[0].path == "pc_hud/potion_count" &&
              frame.art.text_fields[0].initial_text == "5", "only the potion counter text exists (no key legends)");
        check(near(frame.art.text_fields[0].bounds[0], 452.79f, 0.05f) && near(frame.art.text_fields[0].bounds[2], 47.85f, 0.05f),
              "potion counter sits in the authored cnt.value rectangle");
        {
            auto no_count = layout;
            no_count.potion_count.reset();
            PcGameplayHudPresentationV1 other;
            check(compose_pc_touch_gameplay_hud_v1(source, 1, no_count, other, error), error.c_str());
            check(other.art.text_fields.empty(), "unknown inventory must not draw a number");
        }

        // 5. Right-edge anchor: the whole right cluster moves by the shift, the original positions are empty again.
        {
            auto wide = layout;
            wide.stage_shift_x = 88.89f; // 1280x720 window: 1280/(720/320) - 480
            PcGameplayHudPresentationV1 shifted;
            check(compose_pc_touch_gameplay_hud_v1(source, 1, wide, shifted, error), error.c_str());
            for (const auto& spot : spots) {
                const auto hit = hit_at(shifted, spot.x + wide.stage_shift_x, spot.y);
                check(hit.control == spot.control && hit.item == spot.item, "shifted cluster hit test");
                check(hit_at(shifted, spot.x, spot.y).control != spot.control, "unshifted position must no longer hit");
            }
            check(near(shifted.art.text_fields[0].bounds[0], 452.79f + 88.89f, 0.05f), "counter text follows the shift");
            check(shifted.art.batches.size() == frame.art.batches.size(), "shift must not change the batch set");
        }

        // 6. Dynamic state: pressed ring, action icon frame, cooldown wedge, faery icon, unassigned skill.
        {
            auto pressed = layout;
            pressed.action_pressed = true;
            pressed.action_icon = 99; // out of range -> Attack
            PcGameplayHudPresentationV1 other;
            check(compose_pc_touch_gameplay_hud_v1(source, 1, pressed, other, error), error.c_str());
            check(other.art.batches.size() == frame.art.batches.size(), "pressed ring replaces the idle ring one for one");
            check(hit_at(other, 435.4f, 257.1f).control == Control::attack, "pressed attack stays a receiver");
            auto cool = layout;
            cool.faery_cooldown_frame = 50;
            PcGameplayHudPresentationV1 cooled;
            check(compose_pc_touch_gameplay_hud_v1(source, 1, cool, cooled, error), error.c_str());
            check(cooled.art.batches.size() == frame.art.batches.size() + 1, "faery cooldown wedge is one batch");
            auto frame_with_empty = source;
            frame_with_empty.left_middle_right[1].assigned = false;
            frame_with_empty.left_middle_right[1].source_icon_key.reset();
            PcGameplayHudPresentationV1 grey;
            check(compose_pc_touch_gameplay_hud_v1(frame_with_empty, 1, layout, grey, error), error.c_str());
            bool saw_grey = false;
            for (const auto& batch : grey.art.batches) saw_grey = saw_grey || batch.role.find("pc_hud/skill2/grey") == 0;
            check(saw_grey, "unassigned slot draws the grey overlay");
            check(hit_at(grey, 369.2f, 197.8f).control == Control::skill2, "unassigned slot is still a receiver");
        }

        // 7. Failures leave the output intact.
        {
            PcGameplayHudPresentationV1 intact = frame;
            const auto batches = intact.art.batches.size();
            auto bad = layout;
            bad.stage_shift_x = NAN;
            check(!compose_pc_touch_gameplay_hud_v1(source, 1, bad, intact, error) && !error.empty() && intact.art.batches.size() == batches,
                  "non-finite shift must fail without touching the output");
            check(!compose_pc_touch_gameplay_hud_v1(source, 3, layout, intact, error) && intact.art.batches.size() == batches,
                  "unsupported class frame must fail");
            bad = layout;
            bad.potion_count = PcGameplayHudLayoutV1::PotionCount{"someone-else", source.source_actor, 5};
            check(!compose_pc_touch_gameplay_hud_v1(source, 1, bad, intact, error) && intact.art.batches.size() == batches,
                  "foreign potion count must fail");
            bad = layout;
            bad.faery_cooldown_frame = 100;
            check(!compose_pc_touch_gameplay_hud_v1(source, 1, bad, intact, error), "cooldown frame 100 must fail");
            bad = layout;
            bad.active_faery_id = 13;
            check(!compose_pc_touch_gameplay_hud_v1(source, 1, bad, intact, error), "faery id 13 must fail");
            platform_input::Hit scratch;
            check(!pc_gameplay_hud_hit_test_v1(intact, NAN, 0.0f, scratch, error), "non-finite pointer must fail");
        }

        // 8. Semantic input: a held attack/skill/spell/potion circle behaves like its key; the music note is a click.
        {
            platform_input::SemanticInput input;
            platform_input::Surface surface;
            surface.hit = [&](platform_input::Point p) { return hit_at(frame, p.x, p.y); };
            input.set_surface(surface);
            input.pointer(1, platform_input::PointerPhase::down, {435.4f, 257.1f});
            auto taken = input.take_frame();
            check(taken.attack.pressed && taken.attack.held && taken.actions.attack, "pointer on the attack ring = Space held");
            input.pointer(1, platform_input::PointerPhase::up, {435.4f, 257.1f});
            taken = input.take_frame();
            check(taken.attack.released && !taken.attack.held, "release ends the attack");
            input.pointer(2, platform_input::PointerPhase::down, {346.9f, 258.3f});
            input.pointer(2, platform_input::PointerPhase::up, {346.9f, 258.3f});
            taken = input.take_frame();
            check(taken.skills[0].pressed && taken.skills[0].released, "left skill circle = key 1");
            input.pointer(3, platform_input::PointerPhase::down, {372.4f, 29.3f});
            input.pointer(3, platform_input::PointerPhase::up, {372.4f, 29.3f});
            taken = input.take_frame();
            check(taken.music_pressed && !taken.attack.pressed, "music note release = music_pressed");
            input.pointer(4, platform_input::PointerPhase::down, {372.4f, 29.3f});
            input.pointer(4, platform_input::PointerPhase::up, {200.0f, 100.0f});
            taken = input.take_frame();
            check(!taken.music_pressed, "releasing off the music note cancels the click");
            input.set_menu_open(true);
            input.pointer(5, platform_input::PointerPhase::down, {372.4f, 29.3f});
            input.pointer(5, platform_input::PointerPhase::up, {372.4f, 29.3f});
            taken = input.take_frame();
            check(!taken.music_pressed, "a modal menu never receives the HUD music click");
        }
        std::cout << "pc touch gameplay hud v1 tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAILED: " << error.what() << '\n';
        return 1;
    }
}
