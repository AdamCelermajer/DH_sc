#include "pc_gameplay_hud_v1.hpp"
#include "../skill_ui/original_skill_art.hpp"
#include "../platform_input/semantic_input.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <utility>

using namespace dh::foundation;
using namespace dh::foundation::generic_skills;

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

PcGameplayHudCirclePlacementV1 place(float x) {
    PcGameplayHudCirclePlacementV1 p;
    p.center_x = x;
    p.center_y = 270.0f;
    p.radius = 24.0f;
    p.key_label_bounds = {x - 7.0f, x + 7.0f, 298.0f, 316.0f};
    return p;
}
}

int main() {
    try {
        PcSkillHudFrameV1 source;
        source.character_state_id = "render-test-source-character";
        source.source_actor = 7;
        source.equipment_set = 0;
        const std::array<std::uint32_t, 3> source_slots{{2, 0, 1}};
        const std::array<std::string, 3> icons{{"sword_explosion", "fire_right", "swipe"}};
        for (unsigned i = 0; i < 3; ++i) {
            auto& cell = source.left_middle_right[i];
            cell.physical_position = i;
            cell.pc_key_number = i + 1;
            cell.source_slot = source_slots[i];
            cell.key_label = std::to_string(i + 1);
            cell.assigned = true;
            cell.source_icon_key = icons[i];
        }

        PcGameplayHudLayoutV1 layout;
        layout.skills = {{place(66.0f), place(126.0f), place(186.0f)}};
        layout.faery = place(282.0f);
        layout.potion = place(390.0f);
        layout.active_faery_id = 0;
        layout.faery.key_label_bounds = {259.0f, 305.0f, 298.0f, 316.0f};
        layout.potion.key_label_bounds = {359.0f, 421.0f, 298.0f, 316.0f};
        layout.potion_count = PcGameplayHudLayoutV1::PotionCount{
            source.character_state_id, source.source_actor, 5};
        PcGameplayHudPresentationV1 presentation;
        std::string error;
        check(compose_pc_gameplay_hud_v1(source, 1, layout, presentation, error), error.c_str());

        check(presentation.art.bitmap_ids.size() == presentation.art.batches.size() &&
              presentation.art.batch_colors.size() == presentation.art.batches.size() &&
              presentation.art.batches.size() >= 13, // 2 skill rings + 3 icons-at-least-1 + faery ring/icon + potion ring/icon
              "PC HUD did not produce paired original-button and icon draw batches with renderer metadata");
        // HUDBTN: the rings are original dqhud button shapes, not fill/outline circles.
        const auto index_of = [&](const PcGameplayHudPresentationV1& p, const std::string& role) -> std::size_t {
            for (std::size_t i = 0; i < p.art.batches.size(); ++i)
                if (p.art.batches[i].role == role) return i;
            return p.art.batches.size();
        };
        check(index_of(presentation, "pc_hud/skill1/ring/shape167") < presentation.art.batches.size() &&
              index_of(presentation, "pc_hud/skill1/ring/shape168") < presentation.art.batches.size() &&
              index_of(presentation, "pc_hud/faery/ring/shape381") < presentation.art.batches.size() &&
              index_of(presentation, "pc_hud/potion/ring/shape109") < presentation.art.batches.size(),
              "PC HUD buttons do not draw the original dqhud ring/base shapes");
        check(index_of(presentation, "pc_hud/skill1/grey/shape248") == presentation.art.batches.size() &&
              index_of(presentation, "pc_hud/skill2/grey/shape248") == presentation.art.batches.size() &&
              index_of(presentation, "pc_hud/skill3/grey/shape248") == presentation.art.batches.size(),
              "Assigned skill cells draw the empty Grey overlay");
        check(presentation.art.hit_regions.size() == 5 && presentation.art.text_fields.size() == 5,
              "PC HUD did not produce five action hit contours and key legends");
        check(presentation.art.text_fields[0].initial_text == "1" &&
              presentation.art.text_fields[1].initial_text == "2" &&
              presentation.art.text_fields[2].initial_text == "3" &&
              presentation.art.text_fields[3].initial_text == "4" &&
              presentation.art.text_fields[4].initial_text == "5 Potion: 5",
              "PC HUD labels are not physical keys1/2/3, Faery4 and potion5");
        const std::array<const PcGameplayHudCirclePlacementV1*, 5> label_placements{{
            &layout.skills[0], &layout.skills[1], &layout.skills[2], &layout.faery, &layout.potion}};
        const std::array<float, 5> label_advances{{5.0f, 5.0f, 5.0f, 32.0f, 55.0f}};
        const std::array<std::pair<int, int>, 3> viewport_sizes{{
            std::pair<int, int>{800, 600}, {1201, 720}, {1920, 1080}}};
        for (unsigned i = 0; i < label_placements.size(); ++i) {
            const auto& field = presentation.art.text_fields[i];
            const auto& p = *label_placements[i];
            check(std::abs(field.local_bounds[0]) < 1e-5f &&
                  std::abs(field.local_bounds[2]) < 1e-5f &&
                  std::abs(field.local_bounds[1] - (p.key_label_bounds[1] - p.key_label_bounds[0])) < 1e-5f &&
                  std::abs(field.local_bounds[3] - (p.key_label_bounds[3] - p.key_label_bounds[2])) < 1e-5f &&
                  std::abs(field.matrix[4] - p.key_label_bounds[0]) < 1e-5f &&
                  std::abs(field.matrix[5] - p.key_label_bounds[2]) < 1e-5f,
                  "PC label text-local box or stage translation is inconsistent");
            for (const auto& viewport : viewport_sizes) {
                const float scale = float(viewport.second) / 320.0f;
                const float offset = (float(viewport.first) / scale - 480.0f) * 0.5f;
                const float x_scale = 480.0f / (float(viewport.first) / scale);
                const float local_baseline = (field.local_bounds[1] - label_advances[i] - 4.0f) * 0.5f;
                const float screen_run_center =
                    (x_scale * (local_baseline + label_advances[i] * 0.5f) +
                     (field.matrix[4] + offset) * x_scale) * float(viewport.first) / 480.0f;
                const float screen_circle_center = (p.center_x + offset) * scale;
                check(std::abs(screen_run_center - (screen_circle_center - 2.0f * scale)) < 0.02f,
                      "Centered PC key label drifted from its control under the viewport transform");
            }
        }
        // Fitted original ring: its bounds are centred on the placement and their larger
        // half-extent equals the placement radius (HUDBTN button fit).
        const auto fitted = [](const HudGeometryBatch& batch, const PcGameplayHudCirclePlacementV1& p) {
            float xmin = 1e9f, xmax = -1e9f, ymin = 1e9f, ymax = -1e9f;
            for (const auto& v : batch.triangles) {
                xmin = std::min(xmin, v.x); xmax = std::max(xmax, v.x);
                ymin = std::min(ymin, v.y); ymax = std::max(ymax, v.y);
            }
            return std::abs((xmin + xmax) * 0.5f - p.center_x) < 0.05f &&
                   std::abs((ymin + ymax) * 0.5f - p.center_y) < 0.05f &&
                   std::abs(std::max(xmax - xmin, ymax - ymin) * 0.5f - p.radius) < 0.05f;
        };
        for (unsigned i = 0; i < 3; ++i) {
            const auto ring_index = index_of(presentation, "pc_hud/skill" + std::to_string(i + 1) + "/ring/shape167");
            check(ring_index < presentation.art.batches.size() &&
                  fitted(presentation.art.batches[ring_index], layout.skills[i]),
                  "Original skill ring is not fitted to its declared placement");
            // Icon: first bitmap batch of this skill that is neither ring nor overlay.
            std::size_t icon_index = presentation.art.batches.size();
            for (std::size_t k = 0; k < presentation.art.batches.size(); ++k) {
                const auto& role = presentation.art.batches[k].role;
                if (role.find("pc_hud/skill" + std::to_string(i + 1) + "/") == 0 &&
                    role.find("/ring/") == std::string::npos && role.find("/grey/") == std::string::npos &&
                    role.find("/cooldown/") == std::string::npos) { icon_index = k; break; }
            }
            const auto& icon = presentation.art.batches[icon_index];
            check(icon_index < presentation.art.batches.size() && presentation.art.bitmap_ids[icon_index] == 1 &&
                  !icon.triangles.empty(),
                  "Source SkillIcon pixels are not ordered or mapped to the original menu atlas");
            const auto& original = skill_ui::original_skill_icon_states();
            const auto state = std::find_if(original.begin(), original.end(), [&](const auto& item) {
                return item.label == icons[i];
            });
            check(state != original.end() && !state->batches.empty() &&
                  icon.triangles.front().u == state->batches.front().triangles.front().u &&
                  icon.triangles.front().v == state->batches.front().triangles.front().v,
                  "Rendered icon altered source atlas pixel coordinates");
            check(presentation.actions[i].control == static_cast<platform_input::Control>(
                      static_cast<unsigned>(platform_input::Control::skill1) + i) &&
                  presentation.actions[i].item == i + 1,
                  "Skill action identity is not the corresponding physical PC key");
        }
        check(presentation.actions[3].control == platform_input::Control::spell &&
              presentation.actions[3].item == 4 &&
              presentation.actions[4].control == platform_input::Control::potion &&
              presentation.actions[4].item == 5,
              "Faery and potion actions do not retain PC key4/key5 identities");

        const auto& faery_source = original_pc_gameplay_hud_faery_icons_v1();
        const std::array<std::uint32_t, 13> faery_shapes{{383,385,387,389,391,367,393,395,0,363,357,359,361}};
        check(faery_source.size() == faery_shapes.size(), "Original dqhud Faery frame table has the wrong size");
        for (std::size_t frame = 0; frame < faery_source.size(); ++frame) {
            check(faery_source[frame].source_frame == frame &&
                  (faery_shapes[frame] == 0
                       ? faery_source[frame].triangles.empty() && faery_source[frame].source_shape_ids.empty()
                       : faery_source[frame].source_shape_ids == std::vector<std::uint32_t>{faery_shapes[frame]} &&
                         faery_source[frame].triangles.size() == 6),
                  "Original Faery frame no longer matches dqhud sprite397 source display-list mapping");
        }
        for (std::int32_t active_id = 0; active_id <= 12; ++active_id) {
            auto mapped_layout = layout;
            mapped_layout.active_faery_id = active_id;
            PcGameplayHudPresentationV1 mapped;
            check(compose_pc_gameplay_hud_v1(source, 1, mapped_layout, mapped, error), error.c_str());
            const auto expected_shape = faery_shapes[static_cast<std::size_t>(active_id)];
            const auto faery_icon_index = index_of(mapped, "pc_hud/faery/source_frame_" + std::to_string(active_id));
            if (expected_shape == 0) {
                check(faery_icon_index == mapped.art.batches.size(),
                      "Active Faery ID8 did not preserve the authored empty source frame");
            } else {
                check(faery_icon_index < mapped.art.batches.size() &&
                      mapped.art.batches[faery_icon_index].shape_id == expected_shape,
                      "Active Faery ID did not resolve to its exact source frame/shape");
            }
        }
        const auto faery0 = index_of(presentation, "pc_hud/faery/source_frame_0");
        check(faery0 < presentation.art.batches.size() &&
              presentation.art.batches[faery0].shape_id == 383 &&
              presentation.art.batches[faery0].triangles.size() == 6 &&
              presentation.art.bitmap_ids[faery0] == 1,
              "Faery key4 did not use the exact original active-ID+1 icon frame/shape");
        auto no_active_faery = layout;
        no_active_faery.active_faery_id = -1;
        PcGameplayHudPresentationV1 default_faery_frame;
        check(compose_pc_gameplay_hud_v1(source, 1, no_active_faery, default_faery_frame, error), error.c_str());
        check(default_faery_frame.art.batches[faery0].shape_id == 383 &&
              default_faery_frame.art.batches[faery0].role == "pc_hud/faery/source_frame_0",
              "No-active sentinel did not preserve the authored initial Faery frame0");
        const auto& potion_source = original_pc_gameplay_hud_potion_icon_v1();
        const auto potion_icon = index_of(presentation, "pc_hud/potion/source_shape_105");
        check(potion_source.size() == 12 && potion_icon < presentation.art.batches.size() &&
              presentation.art.batches[potion_icon].shape_id == 105 &&
              presentation.art.batches[potion_icon].triangles.size() == 12 &&
              presentation.art.bitmap_ids[potion_icon] == 1 &&
              presentation.art.batches[potion_icon].triangles.front().u == potion_source.front().u &&
              presentation.art.batches[potion_icon].triangles.front().v == potion_source.front().v,
              "Potion key5 did not use original dqhud shape105 bottle art and atlas coordinates");

        auto empty_source_faery = layout;
        empty_source_faery.active_faery_id = 8; // Authored sprite397 frame8 has no placed artwork.
        PcGameplayHudPresentationV1 empty_frame;
        check(compose_pc_gameplay_hud_v1(source, 1, empty_source_faery, empty_frame, error), error.c_str());
        check(index_of(empty_frame, "pc_hud/faery/source_frame_8") == empty_frame.art.batches.size() &&
              index_of(empty_frame, "pc_hud/potion/source_shape_105") < empty_frame.art.batches.size(),
              "Source-empty Faery frame8 was replaced or shifted the exact potion art");
        auto invalid_faery = layout;
        invalid_faery.active_faery_id = 13;
        auto preserved_icon_frame = presentation;
        const auto preserved_size = preserved_icon_frame.art.batches.size();
        check(!compose_pc_gameplay_hud_v1(source, 1, invalid_faery, preserved_icon_frame, error) &&
              !error.empty() && preserved_icon_frame.art.batches.size() == preserved_size,
              "Invalid Faery ID changed the prior authored-icon packet");

        platform_input::SemanticInput input;
        input.set_surface({[&](platform_input::Point p) {
            platform_input::Hit hit;
            std::string hit_error;
            if (!pc_gameplay_hud_hit_test_v1(presentation, p.x, p.y, hit, hit_error))
                throw std::runtime_error(hit_error);
            return hit;
        }, {}});
        for (unsigned i = 0; i < 5; ++i) {
            const PcGameplayHudCirclePlacementV1& p = i < 3 ? layout.skills[i]
                                                              : (i == 3 ? layout.faery : layout.potion);
            platform_input::Hit hit;
            check(pc_gameplay_hud_hit_test_v1(presentation, p.center_x, p.center_y, hit, error),
                  error.c_str());
            check(hit.control == presentation.actions[i].control && hit.item == i + 1,
                  "Circle hit did not emit its physical PC control/key");
            input.pointer(static_cast<std::int64_t>(i + 1), platform_input::PointerPhase::down,
                          {p.center_x, p.center_y});
            const auto pressed = input.take_frame();
            if (i < 3) check(pressed.skills[i].pressed && pressed.skills[i].held,
                              "Physical circle did not press its matching SemanticInput skill key");
            else if (i == 3) check(pressed.spell.pressed && pressed.spell.held,
                                   "Faery circle did not press SemanticInput key4");
            else check(pressed.potion.pressed && pressed.potion.held,
                       "Potion circle did not press SemanticInput potion input");
            input.pointer(static_cast<std::int64_t>(i + 1), platform_input::PointerPhase::down,
                          {p.center_x, p.center_y});
            const auto repeated = input.take_frame();
            if (i < 3) check(!repeated.skills[i].pressed && repeated.skills[i].held,
                              "Repeated pointer-down emitted a duplicate PC skill edge");
            else if (i == 3) check(!repeated.spell.pressed && repeated.spell.held,
                                   "Repeated pointer-down emitted a duplicate Faery edge");
            else check(!repeated.potion.pressed && repeated.potion.held,
                       "Repeated pointer-down emitted a duplicate potion edge");
            input.pointer(static_cast<std::int64_t>(i + 1), platform_input::PointerPhase::up,
                          {p.center_x, p.center_y});
            const auto released = input.take_frame();
            if (i < 3) check(released.skills[i].released, "Skill circle release edge was lost");
            else if (i == 3) check(released.spell.released, "Faery release edge was lost");
            else check(released.potion.released, "Potion release edge was lost");
        }
        platform_input::Hit miss{platform_input::Control::attack, 999};
        check(pc_gameplay_hud_hit_test_v1(presentation, 10.0f, 10.0f, miss, error) &&
              miss.control == platform_input::Control::none && miss.item == 0,
              "Outside pointer point fabricated a HUD action");

        PcGameplayHudLayoutV1 withoutCount = layout;
        withoutCount.potion_count.reset();
        PcGameplayHudPresentationV1 noCountPresentation;
        check(compose_pc_gameplay_hud_v1(source, 1, withoutCount, noCountPresentation, error) &&
              noCountPresentation.art.text_fields[4].initial_text == "5",
              "Missing potion provider was fabricated as a zero count");

        PcSkillHudFrameV1 invalid = source;
        invalid.left_middle_right[0].source_slot = 0;
        PcGameplayHudPresentationV1 unchanged;
        unchanged.art.text_fields.push_back({});
        check(!compose_pc_gameplay_hud_v1(invalid, 1, layout, unchanged, error) &&
              unchanged.art.text_fields.size() == 1,
              "Stale source-slot order was accepted or failure partially replaced output");
        PcGameplayHudLayoutV1 offCenterLabel = layout;
        offCenterLabel.skills[0].key_label_bounds[0] += 5.0f;
        offCenterLabel.skills[0].key_label_bounds[1] += 5.0f;
        check(!compose_pc_gameplay_hud_v1(source, 1, offCenterLabel, unchanged, error) &&
              unchanged.art.text_fields.size() == 1,
              "Off-center PC key label was accepted or failure partially replaced output");
        PcGameplayHudLayoutV1 overlappingLabels = layout;
        overlappingLabels.potion.key_label_bounds[0] = 300.0f;
        overlappingLabels.potion.key_label_bounds[1] = 480.0f;
        check(!compose_pc_gameplay_hud_v1(source, 1, overlappingLabels, unchanged, error) &&
              unchanged.art.text_fields.size() == 1,
              "Overlapping Faery/potion labels were accepted or failure partially replaced output");
        PcGameplayHudLayoutV1 staleCount = layout;
        staleCount.potion_count->character_state_id = "another-character";
        check(!compose_pc_gameplay_hud_v1(source, 1, staleCount, unchanged, error) &&
              unchanged.art.text_fields.size() == 1,
              "Potion count from a different CharacterState was accepted");
        staleCount = layout;
        staleCount.potion_count->source_actor = 8;
        check(!compose_pc_gameplay_hud_v1(source, 1, staleCount, unchanged, error) &&
              unchanged.art.text_fields.size() == 1,
              "Potion count from a different source actor was accepted");
        // HUDBTN: source CoolDown frame = (int)(remaining*100)-1 clamped to 0..99; remaining = 1 - elapsed/total.
        check(pc_cooldown_frame_from_remaining_v1(1.0) == 99 && pc_cooldown_frame_from_remaining_v1(1.5) == 99 &&
              pc_cooldown_frame_from_remaining_v1(0.999) == 98 && pc_cooldown_frame_from_remaining_v1(0.5) == 49 &&
              pc_cooldown_frame_from_remaining_v1(0.0099) == 0 && pc_cooldown_frame_from_remaining_v1(0.0) == 0 &&
              pc_cooldown_frame_from_remaining_v1(-0.2) == 0 && pc_cooldown_frame_from_remaining_v1(std::nan("")) == 0,
              "Cooldown frame does not follow (int)(remaining*100)-1 clamped to 0..99");
        check(std::abs(pc_cooldown_remaining_fraction_v1(5000, 0, 5000) - 1.0) < 1e-9 &&
              std::abs(pc_cooldown_remaining_fraction_v1(5000, 2500, 5000) - 0.5) < 1e-9 &&
              pc_cooldown_remaining_fraction_v1(5000, 5000, 5000) == 0.0 &&
              pc_cooldown_remaining_fraction_v1(5000, 6000, 5000) == 0.0 &&
              pc_cooldown_remaining_fraction_v1(5000, 0, 0) == 0.0,
              "Remaining cooldown fraction is not 1 - elapsed/total with ready and zero-total clamps");
        // Real frames from the source CoolDown sprite: frame f is shape 249+f; frame 0 is empty.
        PcSkillHudFrameV1 cooling = source;
        cooling.left_middle_right[0].source_cooldown_frame = 49;
        cooling.left_middle_right[1].source_cooldown_frame = 99;
        cooling.left_middle_right[2].source_cooldown_frame = 0;
        PcGameplayHudLayoutV1 cooling_layout = layout;
        cooling_layout.faery_cooldown_frame = 10;
        PcGameplayHudPresentationV1 cooling_out;
        check(compose_pc_gameplay_hud_v1(cooling, 1, cooling_layout, cooling_out, error), error.c_str());
        const auto cool1 = index_of(cooling_out, "pc_hud/skill1/cooldown/shape298");
        const auto cool2 = index_of(cooling_out, "pc_hud/skill2/cooldown/shape348");
        const auto cool_faery = index_of(cooling_out, "pc_hud/faery/cooldown/shape259");
        check(cool1 < cooling_out.art.batches.size() && cooling_out.art.batches[cool1].shape_id == 298 &&
              cooling_out.art.bitmap_ids[cool1] == 0 && cooling_out.art.batch_colors[cool1][3] > 0.5f &&
              cool2 < cooling_out.art.batches.size() && cooling_out.art.batches[cool2].shape_id == 348 &&
              cool_faery < cooling_out.art.batches.size() && cooling_out.art.batches[cool_faery].shape_id == 259,
              "Source CoolDown wedge did not resolve to shape 249+frame for skill and Faery");
        bool skill3_cooling = false;
        bool potion_cooling = false;
        for (const auto& batch : cooling_out.art.batches) {
            if (batch.role.find("pc_hud/skill3/cooldown/") == 0) skill3_cooling = true;
            if (batch.role.find("pc_hud/potion/cooldown") == 0) potion_cooling = true;
        }
        check(!skill3_cooling && !potion_cooling,
              "Ready (frame0) or potion controls drew a cooldown overlay");
        // The wedge shares the ring's fitted transform: all vertices stay inside the button.
        const auto& wedge = cooling_out.art.batches[cool2];
        for (const auto& v : wedge.triangles) {
            const double dx = v.x - layout.skills[1].center_x, dy = v.y - layout.skills[1].center_y;
            check(std::sqrt(dx * dx + dy * dy) <= layout.skills[1].radius + 1.0,
                  "Cooldown wedge escaped the fitted button disc");
        }
        // Empty (unassigned) skill cells show the original Grey overlay and no icon.
        PcSkillHudFrameV1 empty_cell = source;
        empty_cell.left_middle_right[1].assigned = false;
        empty_cell.left_middle_right[1].source_icon_key.reset();
        PcGameplayHudPresentationV1 empty_out;
        check(compose_pc_gameplay_hud_v1(empty_cell, 1, layout, empty_out, error), error.c_str());
        const auto grey = index_of(empty_out, "pc_hud/skill2/grey/shape248");
        check(grey < empty_out.art.batches.size() && empty_out.art.batches[grey].shape_id == 248 &&
              empty_out.art.bitmap_ids[grey] == 0 && empty_out.art.batch_colors[grey][3] > 0.5f,
              "Empty skill cell did not draw the original Grey overlay");
        bool skill2_icon = false;
        for (const auto& batch : empty_out.art.batches)
            if (batch.role.find("pc_hud/skill2/") == 0 && batch.role.find("/ring/") == std::string::npos &&
                batch.role.find("/grey/") == std::string::npos) skill2_icon = true;
        check(!skill2_icon, "Empty skill cell drew a SkillIcon");
        // Failure cases keep the prior packet intact.
        PcSkillHudFrameV1 bad_frame = cooling;
        bad_frame.left_middle_right[0].source_cooldown_frame = 100; // source sprite frame100 is unreachable
        PcGameplayHudPresentationV1 kept = cooling_out;
        const auto kept_size = kept.art.batches.size();
        check(!compose_pc_gameplay_hud_v1(bad_frame, 1, cooling_layout, kept, error) &&
              !error.empty() && kept.art.batches.size() == kept_size,
              "Out-of-domain skill CoolDown frame was accepted or partially replaced the packet");
        PcGameplayHudLayoutV1 bad_faery = cooling_layout;
        bad_faery.faery_cooldown_frame = -1;
        check(!compose_pc_gameplay_hud_v1(cooling, 1, bad_faery, kept, error) &&
              kept.art.batches.size() == kept_size,
              "Negative Faery CoolDown frame was accepted");
        std::cout << "pc_gameplay_hud_v1_tests PASS: original dqhud ring fit, SkillIcon UV/pixels, grey empty state, "
                     "CoolDown frame math and wedge shapes (skill/Faery, frame0 empty, potion none), "
                     "centered labels across 800/1201/1920 viewport widths, non-overlap rejection, "
                     "physical hit order1/2/3, Faery4, same-state potion count, strict identity and deduplicated edges\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
