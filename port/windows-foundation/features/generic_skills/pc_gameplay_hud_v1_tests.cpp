#include "pc_gameplay_hud_v1.hpp"
#include "../skill_ui/original_skill_art.hpp"
#include "../platform_input/semantic_input.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

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
        PcGameplayHudPresentationV1 presentation;
        std::string error;
        check(compose_pc_gameplay_hud_v1(source, 1, layout, presentation, error), error.c_str());

        check(presentation.art.batches.size() == 13 &&
              presentation.art.bitmap_ids.size() == presentation.art.batches.size() &&
              presentation.art.batch_colors.size() == presentation.art.batches.size(),
              "PC HUD did not produce paired circle/icon draw batches and renderer metadata");
        check(presentation.art.hit_regions.size() == 5 && presentation.art.text_fields.size() == 5,
              "PC HUD did not produce five action hit contours and key legends");
        check(presentation.art.text_fields[0].initial_text == "1" &&
              presentation.art.text_fields[1].initial_text == "2" &&
              presentation.art.text_fields[2].initial_text == "3" &&
              presentation.art.text_fields[3].initial_text == "4" &&
              presentation.art.text_fields[4].initial_text == "5",
              "PC HUD labels are not physical keys1/2/3, Faery4 and potion5");
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

        PcSkillHudFrameV1 invalid = source;
        invalid.left_middle_right[0].source_slot = 0;
        PcGameplayHudPresentationV1 unchanged;
        unchanged.art.text_fields.push_back({});
        check(!compose_pc_gameplay_hud_v1(invalid, 1, layout, unchanged, error) &&
              unchanged.art.text_fields.size() == 1,
              "Stale source-slot order was accepted or failure partially replaced output");
        std::cout << "pc_gameplay_hud_v1_tests PASS: original SkillIcon UV/pixels, 48-segment PC circle geometry, "
                     "physical hit order1/2/3, Faery4, potion5 and SemanticInput press/release edges\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
