#include "pc_gameplay_hud_v1.hpp"

#include "../skill_ui/original_skill_art.hpp"
#include "pc_gameplay_hud_button_art_v1.hpp"
#include "../hud_panels/hud_panels_art_v1.hpp"
#include "../hud_panels/touch_hud_art_v1.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace dh::foundation::generic_skills {
namespace {
constexpr std::array<std::uint32_t, 3> source_slots{{2, 0, 1}};
constexpr unsigned circle_segments = 48;
constexpr float pi = 3.14159265358979323846f;
// Source CoolDown sprite350 has frames 0..100; FastUpdate clamps GotoFrame to 0..99.
constexpr std::int32_t max_cooldown_frame = 99;

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

// AS's gotoAndStop is one-based. The source onPush adds one to the active ID, yielding the same zero-based frame
// index. The -1 sentinel requests frame0 (invalid); the new btimg instance remains at its authored initial frame0,
// matching source timeline startup. Missing = the caller has no source value and adds no icon.
bool resolve_faery_icon(const std::optional<std::int32_t>& active_faery_id,
                        const PcGameplayHudSourceFaeryIconV1*& faery_icon, std::string& error) {
    faery_icon = nullptr;
    if (!active_faery_id) return true;
    if (*active_faery_id < -1 || *active_faery_id > 12)
        return fail(error, "Source HUD active Faery ID is outside the authored btn_spell frame domain");
    const auto source_frame = static_cast<std::uint32_t>(*active_faery_id < 0 ? 0 : *active_faery_id);
    const auto& icons = original_pc_gameplay_hud_faery_icons_v1();
    faery_icon = &icons[source_frame];
    if (faery_icon->source_frame != source_frame)
        return fail(error, "Original Faery art table does not match the source one-based frame mapping");
    return true;
}


// One transform for the whole original button family (ring/base, Grey, CoolDown)
// so the source's relative geometry is preserved. The ring's extent maps to the
// placement diameter; the centre of the base bounds maps to the placement centre.
struct ButtonFit {
    float source_cx = 0.0f;
    float source_cy = 0.0f;
    float scale = 1.0f;
    float center_x = 0.0f;
    float center_y = 0.0f;
};

bool fit_button_family(const std::vector<PcGameplayHudArtBatchV1>& base,
                       const PcGameplayHudCirclePlacementV1& p,
                       ButtonFit& fit, std::string& error) {
    float xmin = std::numeric_limits<float>::infinity();
    float xmax = -xmin;
    float ymin = xmin;
    float ymax = -xmin;
    for (const auto& batch : base) for (const auto& v : batch.triangles) {
        xmin = std::min(xmin, v.x); xmax = std::max(xmax, v.x);
        ymin = std::min(ymin, v.y); ymax = std::max(ymax, v.y);
    }
    if (base.empty() || !(xmax > xmin && ymax > ymin))
        return fail(error, "Original PC HUD button base art is empty");
    const float extent = std::max(xmax - xmin, ymax - ymin);
    fit.source_cx = (xmin + xmax) * 0.5f;
    fit.source_cy = (ymin + ymax) * 0.5f;
    fit.scale = p.radius * 2.0f / extent;
    fit.center_x = p.center_x;
    fit.center_y = p.center_y;
    return true;
}

void append_fitted_batches(const std::vector<PcGameplayHudArtBatchV1>& source,
                           const ButtonFit& fit, const std::string& role,
                           frontend::art::ScreenArt& art) {
    for (const auto& src : source) {
        HudGeometryBatch batch;
        batch.role = role + "/shape" + std::to_string(src.source_shape_id);
        batch.shape_id = src.source_shape_id;
        batch.triangles = src.triangles;
        for (auto& v : batch.triangles) {
            v.x = (v.x - fit.source_cx) * fit.scale + fit.center_x;
            v.y = (v.y - fit.source_cy) * fit.scale + fit.center_y;
        }
        art.batches.push_back(std::move(batch));
        // Bitmap shapes sample the original MenusGraphics atlas (bitmap1); solid
        // shapes tint the renderer's white texel with their source colour.
        art.bitmap_ids.push_back(src.bitmap ? 1u : 0u);
        art.batch_colors.push_back({src.rgba[0], src.rgba[1], src.rgba[2], src.rgba[3]});
    }
}

frontend::art::HitRegion circle_hit_region(const PcGameplayHudCirclePlacementV1& p,
                                           const std::string& path) {
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
    return hit;
}

void append_button_hit_region(const PcGameplayHudCirclePlacementV1& p,
                              const std::string& path, frontend::art::ScreenArt& art) {
    art.hit_regions.push_back(circle_hit_region(p, path));
}

bool cooldown_frame_is_valid(std::int32_t frame) {
    return frame >= 0 && frame <= max_cooldown_frame;
}

// Original button drawing order: ring/base, icon holder, Grey (empty), CoolDown.
// Icon is appended by the caller between base and overlays.
bool append_button_base(const std::vector<PcGameplayHudArtBatchV1>& base,
                        const PcGameplayHudCirclePlacementV1& p,
                        const std::string& path, ButtonFit& fit,
                        frontend::art::ScreenArt& art, std::string& error) {
    if (!fit_button_family(base, p, fit, error)) return false;
    append_fitted_batches(base, fit, path + "/ring", art);
    return true;
}

bool append_button_cooldown(std::int32_t frame, const ButtonFit& fit,
                            const std::string& path, frontend::art::ScreenArt& art,
                            std::string& error) {
    if (!cooldown_frame_is_valid(frame))
        return fail(error, "PC HUD cooldown frame is outside the source CoolDown domain");
    if (frame == 0) return true; // Source frame0 is the empty ready state.
    append_fitted_batches(original_pc_gameplay_hud_cooldown_frame_v1(std::size_t(frame)),
                          fit, path + "/cooldown", art);
    return true;
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

// P16 HUDART: original stage-space batches (btn_interact art, already in the 480x320 stage: no fit, no offset).
void append_stage_batches(const std::vector<PcGameplayHudArtBatchV1>& source,
                          const std::string& role, frontend::art::ScreenArt& art) {
    for (const auto& src : source) {
        HudGeometryBatch batch;
        batch.role = role + "/shape" + std::to_string(src.source_shape_id);
        batch.shape_id = src.source_shape_id;
        batch.triangles = src.triangles;
        art.batches.push_back(std::move(batch));
        art.bitmap_ids.push_back(src.bitmap ? 1u : 0u); // MenusGraphics_droid atlas bitmap1, or solid tint
        art.batch_colors.push_back({src.rgba[0], src.rgba[1], src.rgba[2], src.rgba[3]});
    }
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

// ---- v21.8 w4-hud: original-touch composition helpers (authored dqhud_droid menu_HUD_2 placements) ----

struct StageRect {
    float x0 = std::numeric_limits<float>::infinity();
    float y0 = std::numeric_limits<float>::infinity();
    float x1 = -std::numeric_limits<float>::infinity();
    float y1 = -std::numeric_limits<float>::infinity();
    bool valid() const { return x1 > x0 && y1 > y0; }
    void add(float x, float y) { x0 = std::min(x0, x); x1 = std::max(x1, x); y0 = std::min(y0, y); y1 = std::max(y1, y); }
};

StageRect batches_bounds(const std::vector<PcGameplayHudArtBatchV1>& batches) {
    StageRect r;
    for (const auto& b : batches) for (const auto& v : b.triangles) r.add(v.x, v.y);
    return r;
}

// Circle that inscribes a stage rectangle (centre, radius = half of the smaller side).
PcGameplayHudCirclePlacementV1 circle_of(const StageRect& r) {
    PcGameplayHudCirclePlacementV1 c;
    c.center_x = (r.x0 + r.x1) * 0.5f;
    c.center_y = (r.y0 + r.y1) * 0.5f;
    c.radius = std::min(r.x1 - r.x0, r.y1 - r.y0) * 0.5f;
    return c;
}

// The authored transform maps the whole button family (ring/base, Grey, CoolDown) from sprite-local px to the stage:
// stage = local * scale + origin (+ the screen-edge shift). The circle is the ring's footprint under that transform.
bool fit_authored_family(const std::vector<PcGameplayHudArtBatchV1>& base,
                         const hud_panels::TouchHudPlacementV1& place, float shift_x,
                         ButtonFit& fit, PcGameplayHudCirclePlacementV1& circle, std::string& error) {
    const auto bounds = batches_bounds(base);
    if (base.empty() || !bounds.valid())
        return fail(error, "Original PC HUD button base art is empty");
    if (!(place.scale > 0.0f) || !std::isfinite(place.scale))
        return fail(error, "Authored touch HUD placement has no scale");
    fit.source_cx = (bounds.x0 + bounds.x1) * 0.5f;
    fit.source_cy = (bounds.y0 + bounds.y1) * 0.5f;
    fit.scale = place.scale;
    fit.center_x = fit.source_cx * place.scale + place.origin_x + shift_x;
    fit.center_y = fit.source_cy * place.scale + place.origin_y;
    circle.center_x = fit.center_x;
    circle.center_y = fit.center_y;
    circle.radius = std::max(bounds.x1 - bounds.x0, bounds.y1 - bounds.y0) * place.scale * 0.5f;
    return true;
}

// Stage-space art (already authored at a stage position) shifted by (dx, dy).
void append_stage_batches_shifted(const std::vector<PcGameplayHudArtBatchV1>& source, const std::string& role,
                                  float dx, float dy, frontend::art::ScreenArt& art) {
    for (const auto& src : source) {
        HudGeometryBatch batch;
        batch.role = role + "/shape" + std::to_string(src.source_shape_id);
        batch.shape_id = src.source_shape_id;
        batch.triangles = src.triangles;
        for (auto& v : batch.triangles) { v.x += dx; v.y += dy; }
        art.batches.push_back(std::move(batch));
        art.bitmap_ids.push_back(src.bitmap ? 1u : 0u);
        art.batch_colors.push_back({src.rgba[0], src.rgba[1], src.rgba[2], src.rgba[3]});
    }
}

std::vector<PcGameplayHudArtBatchV1> shifted_copy(const std::vector<PcGameplayHudArtBatchV1>& source, float dx, float dy) {
    auto copy = source;
    for (auto& b : copy) for (auto& v : b.triangles) { v.x += dx; v.y += dy; }
    return copy;
}

// Sprite-local art (potion badge) placed by an authored transform.
void append_authored_batches(const std::vector<PcGameplayHudArtBatchV1>& source,
                             const hud_panels::TouchHudPlacementV1& place, float shift_x,
                             const std::string& role, frontend::art::ScreenArt& art) {
    for (const auto& src : source) {
        HudGeometryBatch batch;
        batch.role = role + "/shape" + std::to_string(src.source_shape_id);
        batch.shape_id = src.source_shape_id;
        batch.triangles = src.triangles;
        for (auto& v : batch.triangles) {
            v.x = v.x * place.scale + place.origin_x + shift_x;
            v.y = v.y * place.scale + place.origin_y;
        }
        art.batches.push_back(std::move(batch));
        art.bitmap_ids.push_back(src.bitmap ? 1u : 0u);
        art.batch_colors.push_back({src.rgba[0], src.rgba[1], src.rgba[2], src.rgba[3]});
    }
}

void append_count_text(const std::array<float, 4>& rect, float height, const std::string& text,
                       float shift_x, frontend::art::ScreenArt& art) {
    frontend::art::TextField field;
    field.path = "pc_hud/potion_count";
    field.font_id = 5;
    field.source_height = height;
    field.bounds = {rect[0] + shift_x, rect[2] + shift_x, rect[1], rect[3]};
    field.local_bounds = {0.0f, rect[2] - rect[0], 0.0f, rect[3] - rect[1]};
    field.rgba = {255, 255, 255, 255};
    field.align = 2; // authored cnt.value is centred (EditText layout align 2)
    field.matrix = {1.0f, 0.0f, 0.0f, 1.0f, rect[0] + shift_x, rect[1]};
    field.initial_text = text;
    art.text_fields.push_back(std::move(field));
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
    if (!cooldown_frame_is_valid(layout.faery_cooldown_frame))
        return fail(error, "PC HUD Faery cooldown frame is outside the source CoolDown domain");
    const PcGameplayHudSourceFaeryIconV1* faery_icon = nullptr;
    if (!resolve_faery_icon(layout.active_faery_id, faery_icon, error)) return false;
    for (unsigned i = 0; i < 3; ++i) {
        const auto& cell = source.left_middle_right[i];
        if (cell.physical_position != i || cell.pc_key_number != i + 1 ||
            cell.source_slot != source_slots[i] || cell.key_label != std::to_string(i + 1))
            return fail(error, "PC gameplay HUD frame does not preserve physical keys/source order [2,0,1]");
        if (cell.source_cooldown_frame && !cooldown_frame_is_valid(*cell.source_cooldown_frame))
            return fail(error, "PC HUD skill cooldown frame is outside the source CoolDown domain");
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
        const auto& cell = source.left_middle_right[i];
        ButtonFit fit;
        if (!append_button_base(original_pc_gameplay_hud_skill_base_v1(), layout.skills[i],
                                path, fit, next.art, error)) return false;
        if (!append_source_icon(source_class_frame, cell, layout.skills[i], next.art, error)) return false;
        if (!cell.assigned)
            append_fitted_batches(original_pc_gameplay_hud_grey_overlay_v1(), fit, path + "/grey", next.art);
        if (!append_button_cooldown(cell.source_cooldown_frame.value_or(0), fit, path, next.art, error))
            return false;
        append_button_hit_region(layout.skills[i], path, next.art);
        append_key_label(layout.skills[i], std::to_string(i + 1), layout.key_label_height, next.art);
        next.actions[i] = {static_cast<platform_input::Control>(
            static_cast<unsigned>(platform_input::Control::skill1) + i), i + 1};
    }
    {
        ButtonFit fit;
        if (!append_button_base(original_pc_gameplay_hud_spell_base_v1(), layout.faery,
                                "pc_hud/faery", fit, next.art, error)) return false;
        if (faery_icon && !faery_icon->triangles.empty()) {
            if (faery_icon->source_shape_ids.size() != 1 ||
                !append_source_hud_icon(faery_icon->triangles,
                                        faery_icon->source_shape_ids.front(),
                                        layout.faery,
                                        "pc_hud/faery/source_frame_" + std::to_string(faery_icon->source_frame),
                                        next.art, error)) return false;
        }
        // Faery grey (locked/empty) is not drawn: the source Faery state is not
        // part of this frame. The cooldown wedge is the source btn_spell CoolDown.
        if (!append_button_cooldown(layout.faery_cooldown_frame, fit, "pc_hud/faery", next.art, error))
            return false;
        append_button_hit_region(layout.faery, "pc_hud/faery", next.art);
        append_key_label(layout.faery, "4", layout.key_label_height, next.art);
        next.actions[3] = {platform_input::Control::spell, 4};
    }
    {
        ButtonFit fit;
        if (!append_button_base(original_pc_gameplay_hud_potion_base_v1(), layout.potion,
                                "pc_hud/potion", fit, next.art, error)) return false;
        if (!append_source_hud_icon(original_pc_gameplay_hud_potion_icon_v1(), 105,
                                    layout.potion, "pc_hud/potion/source_shape_105",
                                    next.art, error)) return false;
        // btn_potion has no CoolDown child in the source movie (no potion cooldown).
        append_button_hit_region(layout.potion, "pc_hud/potion", next.art);
    }
    const auto potion_label = layout.potion_count
        ? "5 Potion: " + std::to_string(layout.potion_count->quantity)
        : std::string("5");
    append_key_label(layout.potion, potion_label, layout.key_label_height, next.art);
    next.actions[4] = {platform_input::Control::potion, 5};
    // P16 HUDART: bottom-right action button = the original btn_interact movie (ring, pressed ring and the btimg icon),
    // drawn at its authored stage position. The pressed ring shows while Space is held (PC adaptation of the touch-down state).
    if (layout.action_enabled) {
        const auto& idle = hud_panels::action_base_v1();
        const auto& pressed = hud_panels::action_pressed_v1();
        append_stage_batches(layout.action_pressed ? pressed : idle, "pc_hud/action", next.art);
        const auto icon = layout.action_icon >= 0 &&
            static_cast<std::size_t>(layout.action_icon) < hud_panels::action_icon_count_v1()
            ? static_cast<std::size_t>(layout.action_icon) : std::size_t{5}; // out of range -> Attack (source table)
        append_stage_batches(hud_panels::action_icon_v1(icon), "pc_hud/action_icon", next.art);
    }
    output = std::move(next);
    error.clear();
    return true;
}

bool compose_pc_touch_gameplay_hud_v1(const PcSkillHudFrameV1& source,
                                      unsigned source_class_frame,
                                      const PcTouchGameplayHudLayoutV1& layout,
                                      PcGameplayHudPresentationV1& output,
                                      std::string& error) {
    error.clear();
    if (source_class_frame > 2 || !std::isfinite(layout.stage_shift_x) ||
        layout.stage_shift_x < -480.0f || layout.stage_shift_x > 4000.0f)
        return fail(error, "PC touch HUD requires a supported class frame and a finite screen-edge shift");
    if (layout.potion_count &&
        (layout.potion_count->character_state_id.empty() ||
         layout.potion_count->character_state_id != source.character_state_id ||
         layout.potion_count->source_actor == invalid_actor_id ||
         layout.potion_count->source_actor != source.source_actor))
        return fail(error, "PC HUD potion count does not belong to the exact projected CharacterState and actor");
    if (!cooldown_frame_is_valid(layout.faery_cooldown_frame))
        return fail(error, "PC HUD Faery cooldown frame is outside the source CoolDown domain");
    const PcGameplayHudSourceFaeryIconV1* faery_icon = nullptr;
    if (!resolve_faery_icon(layout.active_faery_id, faery_icon, error)) return false;
    for (unsigned i = 0; i < 3; ++i) {
        const auto& cell = source.left_middle_right[i];
        if (cell.physical_position != i || cell.pc_key_number != i + 1 ||
            cell.source_slot != source_slots[i] || cell.key_label != std::to_string(i + 1))
            return fail(error, "PC gameplay HUD frame does not preserve physical keys/source order [2,0,1]");
        if (cell.source_cooldown_frame && !cooldown_frame_is_valid(*cell.source_cooldown_frame))
            return fail(error, "PC HUD skill cooldown frame is outside the source CoolDown domain");
    }

    const auto& authored = hud_panels::touch_hud_authored_layout_v1();
    const float shift = layout.stage_shift_x;
    // Source slot n is the authored btn_skill{n+1} (InfoHUDManager::FastUpdate reads btn_skill%d for slot i).
    const std::array<const hud_panels::TouchHudPlacementV1*, 3> skill_by_slot{
        {&authored.skill1, &authored.skill2, &authored.skill3}};

    PcGameplayHudPresentationV1 next;
    for (unsigned i = 0; i < 3; ++i) {
        const auto path = "pc_hud/skill" + std::to_string(i + 1);
        const auto& cell = source.left_middle_right[i];
        ButtonFit fit;
        PcGameplayHudCirclePlacementV1 circle;
        if (!fit_authored_family(original_pc_gameplay_hud_skill_base_v1(), *skill_by_slot[cell.source_slot],
                                 shift, fit, circle, error)) return false;
        append_fitted_batches(original_pc_gameplay_hud_skill_base_v1(), fit, path + "/ring", next.art);
        if (!append_source_icon(source_class_frame, cell, circle, next.art, error)) return false;
        if (!cell.assigned)
            append_fitted_batches(original_pc_gameplay_hud_grey_overlay_v1(), fit, path + "/grey", next.art);
        if (!append_button_cooldown(cell.source_cooldown_frame.value_or(0), fit, path, next.art, error))
            return false;
        append_button_hit_region(circle, path, next.art);
        next.actions[i] = {static_cast<platform_input::Control>(
            static_cast<unsigned>(platform_input::Control::skill1) + i), i + 1};
    }
    {
        ButtonFit fit;
        PcGameplayHudCirclePlacementV1 circle;
        if (!fit_authored_family(original_pc_gameplay_hud_spell_base_v1(), authored.spell, shift, fit, circle, error))
            return false;
        append_fitted_batches(original_pc_gameplay_hud_spell_base_v1(), fit, "pc_hud/faery/ring", next.art);
        if (faery_icon && !faery_icon->triangles.empty()) {
            if (faery_icon->source_shape_ids.size() != 1 ||
                !append_source_hud_icon(faery_icon->triangles, faery_icon->source_shape_ids.front(), circle,
                                        "pc_hud/faery/source_frame_" + std::to_string(faery_icon->source_frame),
                                        next.art, error)) return false;
        }
        if (!append_button_cooldown(layout.faery_cooldown_frame, fit, "pc_hud/faery", next.art, error))
            return false;
        append_button_hit_region(circle, "pc_hud/faery", next.art);
        next.actions[3] = {platform_input::Control::spell, 4};
    }
    {
        // Potion: btframe (shape 105, the bottle in its holder; sprite space shifted by the btframe placement), then
        // the pd6 counter badge (shape 109); the number sits in the authored cnt.value field. Both under btn_potion's
        // authored transform. No CoolDown child exists in the source movie.
        const auto& place = authored.potion;
        const auto& icon_source = original_pc_gameplay_hud_potion_icon_v1();
        if (icon_source.empty() || icon_source.size() % 3 != 0)
            return fail(error, "Original PC HUD potion icon triangles are incomplete");
        HudGeometryBatch icon;
        icon.role = "pc_hud/potion/source_shape_105";
        icon.shape_id = 105;
        icon.triangles = icon_source;
        StageRect bounds;
        for (auto& v : icon.triangles) {
            if (!std::isfinite(v.x) || !std::isfinite(v.y) || !std::isfinite(v.u) || !std::isfinite(v.v) ||
                v.u < 0.0f || v.u > 1.0f || v.v < 0.0f || v.v > 1.0f)
                return fail(error, "Original PC HUD icon contains invalid source geometry or atlas UVs");
            v.x = (v.x + authored.potion_icon_offset[0]) * place.scale + place.origin_x + shift;
            v.y = (v.y + authored.potion_icon_offset[1]) * place.scale + place.origin_y;
            bounds.add(v.x, v.y);
        }
        if (!bounds.valid()) return fail(error, "Original PC HUD icon has empty source bounds");
        next.art.batches.push_back(std::move(icon));
        next.art.bitmap_ids.push_back(1);
        next.art.batch_colors.push_back({1.0f, 1.0f, 1.0f, 1.0f});
        append_authored_batches(original_pc_gameplay_hud_potion_base_v1(), place, shift, "pc_hud/potion/badge", next.art);
        append_button_hit_region(circle_of(bounds), "pc_hud/potion", next.art);
        if (layout.potion_count)
            append_count_text(authored.potion_count_rect, authored.potion_count_height * place.scale,
                              std::to_string(layout.potion_count->quantity), shift, next.art);
        next.actions[4] = {platform_input::Control::potion, 5};
    }
    {
        // Attack / context action: btn_interact (art generated at menu_HUD_0, same sprite) moved to its menu_HUD_2 origin.
        const float dx = authored.action_translate[0] + shift;
        const float dy = authored.action_translate[1];
        const auto& idle = hud_panels::action_base_v1();
        const auto& pressed = hud_panels::action_pressed_v1();
        const auto ring = batches_bounds(shifted_copy(idle, dx, dy));
        if (!ring.valid()) return fail(error, "Original action button art is empty");
        append_stage_batches_shifted(layout.action_pressed ? pressed : idle, "pc_hud/action", dx, dy, next.art);
        const auto icon = layout.action_icon >= 0 &&
            static_cast<std::size_t>(layout.action_icon) < hud_panels::action_icon_count_v1()
            ? static_cast<std::size_t>(layout.action_icon) : std::size_t{5}; // out of range -> Attack (source table)
        append_stage_batches_shifted(hud_panels::action_icon_v1(icon), "pc_hud/action_icon", dx, dy, next.art);
        next.extra_hits.push_back({{platform_input::Control::attack, 0}, circle_hit_region(circle_of(ring), "pc_hud/action")});
    }
    {
        // Music note: btn_itunes (itunes_controls) idle frame at its authored position.
        const auto& music = hud_panels::touch_music_button_v1();
        const auto bounds = batches_bounds(shifted_copy(music, shift, 0.0f));
        if (!bounds.valid()) return fail(error, "Original music button art is empty");
        append_stage_batches_shifted(music, "pc_hud/music", shift, 0.0f, next.art);
        next.extra_hits.push_back({{platform_input::Control::music, 0}, circle_hit_region(circle_of(bounds), "pc_hud/music")});
    }
    next.joystick_zone = authored.joystick_zone; // seam only: no joystick art, no hit region
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
    for (std::size_t i = frame.extra_hits.size(); i-- > 0;) { // original-touch attack / music, drawn last
        if (circle_contains(frame.extra_hits[i].region, stage_x, stage_y)) {
            output = frame.extra_hits[i].hit;
            error.clear();
            return true;
        }
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
