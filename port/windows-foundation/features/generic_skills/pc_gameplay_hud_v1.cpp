#include "pc_gameplay_hud_v1.hpp"

#include "../skill_ui/original_skill_art.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace dh::foundation::generic_skills {
namespace {
constexpr std::array<std::uint32_t, 3> source_slots{{2, 0, 1}};
constexpr unsigned circle_segments = 48;
constexpr float pi = 3.14159265358979323846f;

bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

bool finite_placement(const PcGameplayHudCirclePlacementV1& p) {
    if (!std::isfinite(p.center_x) || !std::isfinite(p.center_y) ||
        !std::isfinite(p.radius) || p.radius < 8.0f || p.radius > 90.0f ||
        p.center_x - p.radius < 0.0f || p.center_x + p.radius > 480.0f ||
        p.center_y - p.radius < 0.0f || p.center_y + p.radius > 320.0f) return false;
    const auto& b = p.key_label_bounds;
    return std::isfinite(b[0]) && std::isfinite(b[1]) &&
           std::isfinite(b[2]) && std::isfinite(b[3]) &&
           b[0] >= 0.0f && b[2] >= 0.0f && b[1] <= 480.0f && b[3] <= 320.0f &&
           b[0] < b[1] && b[2] < b[3];
}

bool label_is_below_and_centered(const PcGameplayHudCirclePlacementV1& p) {
    const auto& b = p.key_label_bounds;
    const float label_center = (b[0] + b[1]) * 0.5f;
    return std::abs(label_center - p.center_x) <= 0.5f &&
           b[2] >= p.center_y + p.radius;
}

HudGeometryVertex vertex(float x, float y) { return {x, y, 0.0f, 0.0f}; }

void append_circle(const PcGameplayHudCirclePlacementV1& p,
                   const std::string& path, frontend::art::ScreenArt& art) {
    HudGeometryBatch fill;
    fill.role = path + "/pc-fill";
    HudGeometryBatch ring;
    ring.role = path + "/pc-ring";
    const float inner = p.radius * 0.84f;
    fill.triangles.reserve(circle_segments * 3);
    ring.triangles.reserve(circle_segments * 6);
    for (unsigned i = 0; i < circle_segments; ++i) {
        const float a0 = (2.0f * pi * float(i)) / float(circle_segments);
        const float a1 = (2.0f * pi * float(i + 1)) / float(circle_segments);
        const auto outer0 = vertex(p.center_x + p.radius * std::cos(a0),
                                   p.center_y + p.radius * std::sin(a0));
        const auto outer1 = vertex(p.center_x + p.radius * std::cos(a1),
                                   p.center_y + p.radius * std::sin(a1));
        const auto inner0 = vertex(p.center_x + inner * std::cos(a0),
                                   p.center_y + inner * std::sin(a0));
        const auto inner1 = vertex(p.center_x + inner * std::cos(a1),
                                   p.center_y + inner * std::sin(a1));
        fill.triangles.insert(fill.triangles.end(),
                              {vertex(p.center_x, p.center_y), outer0, outer1});
        ring.triangles.insert(ring.triangles.end(),
                              {outer0, outer1, inner1, outer0, inner1, inner0});
    }
    art.batches.push_back(std::move(fill));
    art.bitmap_ids.push_back(0); // renderer's white texel, tinted by batch_colors
    art.batch_colors.push_back({0.10f, 0.12f, 0.16f, 0.88f});
    art.batches.push_back(std::move(ring));
    art.bitmap_ids.push_back(0);
    art.batch_colors.push_back({0.80f, 0.72f, 0.50f, 1.0f});

    frontend::art::HitRegion hit;
    hit.button_path = path;
    hit.triangles.reserve(circle_segments * 3);
    for (unsigned i = 0; i < circle_segments; ++i) {
        const float a0 = (2.0f * pi * float(i)) / float(circle_segments);
        const float a1 = (2.0f * pi * float(i + 1)) / float(circle_segments);
        hit.triangles.insert(hit.triangles.end(), {
            vertex(p.center_x, p.center_y),
            vertex(p.center_x + p.radius * std::cos(a0), p.center_y + p.radius * std::sin(a0)),
            vertex(p.center_x + p.radius * std::cos(a1), p.center_y + p.radius * std::sin(a1))});
    }
    art.hit_regions.push_back(std::move(hit));
}

bool append_source_icon(unsigned class_frame, const PcSkillHudCellV1& cell,
                        const PcGameplayHudCirclePlacementV1& p,
                        frontend::art::ScreenArt& art, std::string& error) {
    if (!cell.assigned) {
        if (cell.source_icon_key)
            return fail(error, "Unassigned PC HUD cell carries a source icon");
        return true;
    }
    if (!cell.source_icon_key || cell.source_icon_key->empty())
        return fail(error, "Assigned PC HUD cell has no original SkillIcon key");
    if (class_frame > 2)
        return fail(error, "PC HUD source class frame is outside Warrior/Rogue/Mage");
    const auto& states = skill_ui::original_skill_icon_states();
    const auto state = std::find_if(states.begin(), states.end(), [&](const auto& candidate) {
        return candidate.label == *cell.source_icon_key;
    });
    if (state == states.end())
        return fail(error, "PC HUD SkillIcon key is absent from original authored icon states");

    float xmin = std::numeric_limits<float>::infinity();
    float xmax = -xmin;
    float ymin = xmin;
    float ymax = -xmin;
    for (const auto& batch : state->batches) for (const auto& v : batch.triangles) {
        xmin = std::min(xmin, v.x); xmax = std::max(xmax, v.x);
        ymin = std::min(ymin, v.y); ymax = std::max(ymax, v.y);
    }
    if (!(xmax > xmin && ymax > ymin))
        return fail(error, "Assigned source SkillIcon has no authored pixel triangles");
    const float target = p.radius * 1.18f;
    const float scale = std::min(target / (xmax - xmin), target / (ymax - ymin));
    const float tx = p.center_x - (xmin + xmax) * scale * 0.5f;
    const float ty = p.center_y - (ymin + ymax) * scale * 0.5f;
    for (const auto& source : state->batches) {
        auto batch = source;
        batch.role = "pc_hud/skill" + std::to_string(cell.pc_key_number) + "/" + source.role;
        for (auto& v : batch.triangles) {
            v.x = v.x * scale + tx;
            v.y = v.y * scale + ty;
        }
        art.batches.push_back(std::move(batch));
        // SkillIcon UVs are rendered with the original MenusGraphics_droid
        // atlas already used by the live HUD/character-menu renderer.
        art.bitmap_ids.push_back(1);
        art.batch_colors.push_back({1.0f, 1.0f, 1.0f, 1.0f});
    }
    (void)class_frame; // source icon label is already resolved from this frame
    return true;
}

bool append_source_hud_icon(const std::vector<HudGeometryVertex>& source,
                            std::uint32_t source_shape_id,
                            const PcGameplayHudCirclePlacementV1& p,
                            const std::string& role,
                            frontend::art::ScreenArt& art,
                            std::string& error) {
    if (source.empty()) return true; // Source Faery frame8 is deliberately empty.
    if (source.size() % 3 != 0)
        return fail(error, "Original PC HUD icon triangles are incomplete");
    float xmin = std::numeric_limits<float>::infinity();
    float xmax = -xmin;
    float ymin = xmin;
    float ymax = -xmin;
    for (const auto& v : source) {
        if (!std::isfinite(v.x) || !std::isfinite(v.y) ||
            !std::isfinite(v.u) || !std::isfinite(v.v) ||
            v.u < 0.0f || v.u > 1.0f || v.v < 0.0f || v.v > 1.0f)
            return fail(error, "Original PC HUD icon contains invalid source geometry or atlas UVs");
        xmin = std::min(xmin, v.x); xmax = std::max(xmax, v.x);
        ymin = std::min(ymin, v.y); ymax = std::max(ymax, v.y);
    }
    if (!(xmax > xmin && ymax > ymin))
        return fail(error, "Original PC HUD icon has empty source bounds");
    const float target = p.radius * 1.18f;
    const float scale = std::min(target / (xmax - xmin), target / (ymax - ymin));
    const float tx = p.center_x - (xmin + xmax) * scale * 0.5f;
    const float ty = p.center_y - (ymin + ymax) * scale * 0.5f;
    HudGeometryBatch batch;
    batch.role = role;
    batch.shape_id = source_shape_id;
    batch.triangles = source;
    for (auto& v : batch.triangles) {
        v.x = v.x * scale + tx;
        v.y = v.y * scale + ty;
    }
    art.batches.push_back(std::move(batch));
    art.bitmap_ids.push_back(1); // Exact MenusGraphics_droid atlas bitmap1.
    art.batch_colors.push_back({1.0f, 1.0f, 1.0f, 1.0f});
    return true;
}

void append_key_label(const PcGameplayHudCirclePlacementV1& p,
                      const std::string& label, float height,
                      frontend::art::ScreenArt& art) {
    frontend::art::TextField field;
    field.path = "pc_hud/key_" + label;
    field.font_id = 5;
    field.source_height = height;
    field.bounds = p.key_label_bounds;
    // Text layout works in field-local coordinates before applying matrix.
    // Keep the caller's stage-space rectangle as the matrix translation and
    // make the local text box start at zero. Passing the global left/top here
    // shifts centered text left by roughly half its stage-space origin.
    field.local_bounds = {0.0f,
                          p.key_label_bounds[1] - p.key_label_bounds[0],
                          0.0f,
                          p.key_label_bounds[3] - p.key_label_bounds[2]};
    field.rgba = {255, 255, 255, 255};
    field.align = 2;
    field.matrix = {1.0f, 0.0f, 0.0f, 1.0f,
                    p.key_label_bounds[0], p.key_label_bounds[2]};
    field.initial_text = label;
    art.text_fields.push_back(std::move(field));
}

bool circle_contains(const frontend::art::HitRegion& region, float x, float y) {
    for (std::size_t i = 0; i + 2 < region.triangles.size(); i += 3) {
        const auto& a = region.triangles[i];
        const auto& b = region.triangles[i + 1];
        const auto& c = region.triangles[i + 2];
        const float e0 = (b.x-a.x)*(y-a.y) - (b.y-a.y)*(x-a.x);
        const float e1 = (c.x-b.x)*(y-b.y) - (c.y-b.y)*(x-b.x);
        const float e2 = (a.x-c.x)*(y-c.y) - (a.y-c.y)*(x-c.x);
        if ((e0 >= -1e-5f && e1 >= -1e-5f && e2 >= -1e-5f) ||
            (e0 <= 1e-5f && e1 <= 1e-5f && e2 <= 1e-5f)) return true;
    }
    return false;
}
} // namespace

bool compose_pc_gameplay_hud_v1(const PcSkillHudFrameV1& source,
                                unsigned source_class_frame,
                                const PcGameplayHudLayoutV1& layout,
                                PcGameplayHudPresentationV1& output,
                                std::string& error) {
    error.clear();
    if (source_class_frame > 2 || !std::isfinite(layout.key_label_height) ||
        layout.key_label_height <= 0.0f || layout.key_label_height > 48.0f)
        return fail(error, "PC gameplay HUD requires a supported class frame and positive label height");
    if (layout.potion_count &&
        (layout.potion_count->character_state_id.empty() ||
         layout.potion_count->character_state_id != source.character_state_id ||
         layout.potion_count->source_actor == invalid_actor_id ||
         layout.potion_count->source_actor != source.source_actor))
        return fail(error, "PC HUD potion count does not belong to the exact projected CharacterState and actor");
    const PcGameplayHudSourceFaeryIconV1* faery_icon = nullptr;
    if (layout.active_faery_id) {
        if (*layout.active_faery_id < -1 || *layout.active_faery_id > 12)
            return fail(error, "Source HUD active Faery ID is outside the authored btn_spell frame domain");
        // AS's gotoAndStop is one-based. The source onPush adds one to the
        // active ID, yielding the same zero-based frame index. The -1 sentinel
        // requests frame0 (invalid); the new btimg instance remains at its
        // authored initial frame0, matching source timeline startup.
        const auto source_frame = static_cast<std::uint32_t>(
            *layout.active_faery_id < 0 ? 0 : *layout.active_faery_id);
        const auto& icons = original_pc_gameplay_hud_faery_icons_v1();
        faery_icon = &icons[source_frame];
        if (faery_icon->source_frame != source_frame)
            return fail(error, "Original Faery art table does not match the source one-based frame mapping");
    }
    for (unsigned i = 0; i < 3; ++i) {
        const auto& cell = source.left_middle_right[i];
        if (cell.physical_position != i || cell.pc_key_number != i + 1 ||
            cell.source_slot != source_slots[i] || cell.key_label != std::to_string(i + 1))
            return fail(error, "PC gameplay HUD frame does not preserve physical keys/source order [2,0,1]");
    }
    for (const auto& p : layout.skills) if (!finite_placement(p) || !label_is_below_and_centered(p))
        return fail(error, "PC skill circle or its centered label is invalid or outside authored 480x320 stage");
    if (!finite_placement(layout.faery) || !finite_placement(layout.potion) ||
        !label_is_below_and_centered(layout.faery) || !label_is_below_and_centered(layout.potion))
        return fail(error, "PC Faery/potion circle or its centered label is invalid or outside authored stage");
    std::array<const PcGameplayHudCirclePlacementV1*, 5> controls{
        &layout.skills[0], &layout.skills[1], &layout.skills[2], &layout.faery, &layout.potion};
    for (std::size_t i = 0; i < controls.size(); ++i) for (std::size_t j = i + 1; j < controls.size(); ++j) {
        const auto& a = controls[i]->key_label_bounds;
        const auto& b = controls[j]->key_label_bounds;
        const bool overlap_x = a[0] < b[1] && b[0] < a[1];
        const bool overlap_y = a[2] < b[3] && b[2] < a[3];
        if (overlap_x && overlap_y)
            return fail(error, "PC HUD key-label rectangles overlap");
    }

    PcGameplayHudPresentationV1 next;
    for (unsigned i = 0; i < 3; ++i) {
        const auto path = "pc_hud/skill" + std::to_string(i + 1);
        append_circle(layout.skills[i], path, next.art);
        if (!append_source_icon(source_class_frame, source.left_middle_right[i],
                               layout.skills[i], next.art, error)) return false;
        append_key_label(layout.skills[i], std::to_string(i + 1), layout.key_label_height, next.art);
        next.actions[i] = {static_cast<platform_input::Control>(
            static_cast<unsigned>(platform_input::Control::skill1) + i), i + 1};
    }
    append_circle(layout.faery, "pc_hud/faery", next.art);
    if (faery_icon && !faery_icon->triangles.empty()) {
        if (faery_icon->source_shape_ids.size() != 1 ||
            !append_source_hud_icon(faery_icon->triangles,
                                    faery_icon->source_shape_ids.front(),
                                    layout.faery,
                                    "pc_hud/faery/source_frame_" + std::to_string(faery_icon->source_frame),
                                    next.art, error)) return false;
    }
    append_key_label(layout.faery, "4", layout.key_label_height, next.art);
    next.actions[3] = {platform_input::Control::spell, 4};
    append_circle(layout.potion, "pc_hud/potion", next.art);
    if (!append_source_hud_icon(original_pc_gameplay_hud_potion_icon_v1(), 105,
                                layout.potion, "pc_hud/potion/source_shape_105",
                                next.art, error)) return false;
    const auto potion_label = layout.potion_count
        ? "5 Potion: " + std::to_string(layout.potion_count->quantity)
        : std::string("5");
    append_key_label(layout.potion, potion_label, layout.key_label_height, next.art);
    next.actions[4] = {platform_input::Control::potion, 5};
    output = std::move(next);
    error.clear();
    return true;
}

bool pc_gameplay_hud_hit_test_v1(const PcGameplayHudPresentationV1& frame,
                                 float stage_x, float stage_y,
                                 platform_input::Hit& output,
                                 std::string& error) {
    error.clear();
    if (!std::isfinite(stage_x) || !std::isfinite(stage_y))
        return fail(error, "PC gameplay HUD hit point is not finite");
    if (frame.art.hit_regions.size() != frame.actions.size())
        return fail(error, "PC gameplay HUD hit packet is incomplete");
    const std::array<platform_input::Hit, 5> expected{{
        {platform_input::Control::skill1, 1},
        {platform_input::Control::skill2, 2},
        {platform_input::Control::skill3, 3},
        {platform_input::Control::spell, 4},
        {platform_input::Control::potion, 5}}};
    const std::array<std::string, 5> expected_paths{{
        "pc_hud/skill1", "pc_hud/skill2", "pc_hud/skill3", "pc_hud/faery", "pc_hud/potion"}};
    for (std::size_t i = 0; i < expected.size(); ++i) {
        if (frame.actions[i].control != expected[i].control ||
            frame.actions[i].item != expected[i].item ||
            frame.art.hit_regions[i].button_path != expected_paths[i])
            return fail(error, "PC HUD hit order/action identity was changed after composition");
    }
    for (std::size_t i = frame.art.hit_regions.size(); i-- > 0;) {
        if (circle_contains(frame.art.hit_regions[i], stage_x, stage_y)) {
            output = frame.actions[i];
            error.clear();
            return true;
        }
    }
    output = {};
    error.clear();
    return true;
}

} // namespace dh::foundation::generic_skills
