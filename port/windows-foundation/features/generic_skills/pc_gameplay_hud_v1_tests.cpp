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

double triangle_area(const HudGeometryVertex& a, const HudGeometryVertex& b,
                     const HudGeometryVertex& c) {
    return std::abs(double((b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x))) * 0.5;
}

double area(const HudGeometryBatch& batch) {
    double sum = 0.0;
    for (std::size_t i = 0; i + 2 < batch.triangles.size(); i += 3)
        sum += triangle_area(batch.triangles[i], batch.triangles[i + 1], batch.triangles[i + 2]);
    return sum;
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

        check(presentation.art.batches.size() == 15 &&
              presentation.art.bitmap_ids.size() == presentation.art.batches.size() &&
              presentation.art.batch_colors.size() == presentation.art.batches.size(),
              "PC HUD did not produce paired circle/icon draw batches and renderer metadata");
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
        for (unsigned i = 0; i < 3; ++i) {
            const auto& fill = presentation.art.batches[i * 3];
            const auto& ring = presentation.art.batches[i * 3 + 1];
            const auto& icon = presentation.art.batches[i * 3 + 2];
            const double r = layout.skills[i].radius;
            check(fill.triangles.size() == 48 * 3 && ring.triangles.size() == 48 * 6,
                  "Circle raster mesh lost its 48-segment fill or outline");
            check(std::abs(area(fill) - 3.141592653589793 * r * r) < 6.0,
                  "PC circle fill pixel geometry does not cover its declared radius");
            check(std::abs(area(ring) - 3.141592653589793 * r * r * (1.0 - .84 * .84)) < 6.0,
                  "PC ring pixel geometry does not match the declared annulus");
            check(icon.role.find("pc_hud/skill" + std::to_string(i + 1) + "/") == 0 &&
                  presentation.art.bitmap_ids[i * 3 + 2] == 1 && !icon.triangles.empty(),
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
            if (expected_shape == 0) {
                check(mapped.art.batches.size() == 14,
                      "Active Faery ID8 did not preserve the authored empty source frame");
            } else {
                check(mapped.art.batches.size() == 15 &&
                      mapped.art.batches[11].role == "pc_hud/faery/source_frame_" + std::to_string(active_id) &&
                      mapped.art.batches[11].shape_id == expected_shape,
                      "Active Faery ID did not resolve to its exact source frame/shape");
            }
        }
        check(presentation.art.batches[11].role == "pc_hud/faery/source_frame_0" &&
              presentation.art.batches[11].shape_id == 383 &&
              presentation.art.batches[11].triangles.size() == 6 &&
              presentation.art.bitmap_ids[11] == 1,
              "Faery key4 did not use the exact original active-ID+1 icon frame/shape");
        auto no_active_faery = layout;
        no_active_faery.active_faery_id = -1;
        PcGameplayHudPresentationV1 default_faery_frame;
        check(compose_pc_gameplay_hud_v1(source, 1, no_active_faery, default_faery_frame, error), error.c_str());
        check(default_faery_frame.art.batches[11].shape_id == 383 &&
              default_faery_frame.art.batches[11].role == "pc_hud/faery/source_frame_0",
              "No-active sentinel did not preserve the authored initial Faery frame0");
        const auto& potion_source = original_pc_gameplay_hud_potion_icon_v1();
        check(potion_source.size() == 12 &&
              presentation.art.batches[14].role == "pc_hud/potion/source_shape_105" &&
              presentation.art.batches[14].shape_id == 105 &&
              presentation.art.batches[14].triangles.size() == 12 &&
              presentation.art.bitmap_ids[14] == 1 &&
              presentation.art.batches[14].triangles.front().u == potion_source.front().u &&
              presentation.art.batches[14].triangles.front().v == potion_source.front().v,
              "Potion key5 did not use original dqhud shape105 bottle art and atlas coordinates");

        auto empty_source_faery = layout;
        empty_source_faery.active_faery_id = 8; // Authored sprite397 frame8 has no placed artwork.
        PcGameplayHudPresentationV1 empty_frame;
        check(compose_pc_gameplay_hud_v1(source, 1, empty_source_faery, empty_frame, error), error.c_str());
        check(empty_frame.art.batches.size() == 14 &&
              empty_frame.art.batches[13].role == "pc_hud/potion/source_shape_105",
              "Source-empty Faery frame8 was replaced or shifted the exact potion art");
        auto invalid_faery = layout;
        invalid_faery.active_faery_id = 13;
        auto preserved_icon_frame = presentation;
        check(!compose_pc_gameplay_hud_v1(source, 1, invalid_faery, preserved_icon_frame, error) &&
              !error.empty() && preserved_icon_frame.art.batches.size() == 15,
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
        std::cout << "pc_gameplay_hud_v1_tests PASS: original SkillIcon UV/pixels, 48-segment PC circle geometry, "
                     "centered labels across 800/1201/1920 viewport widths, non-overlap rejection, "
                     "physical hit order1/2/3, Faery4, same-state potion count, strict identity and deduplicated edges\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
