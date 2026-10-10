#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation {
struct HudGlyphRun;

struct HudGeometryVertex { float x{}, y{}, u{}, v{}; };
struct HudShapeGeometry {
    std::string role;
    std::uint32_t shape_id{};
    // Source-local twips; bitmap coordinates already normalized to atlas1024.
    std::vector<HudGeometryVertex> triangles;
};
struct HudGeometryBatch {
    std::string role;
    std::uint32_t shape_id{};
    // Authored 480x320 SWF pixel coordinates, top-left origin.
    std::vector<HudGeometryVertex> triangles;
};
struct HudGeometry {
    float width{480}, height{320};
    std::vector<HudGeometryBatch> batches;
};

struct HudTargetTextField {
    std::string role; // enemy_name / enemy_level
    std::uint32_t source_character{}, source_font{};
    float source_height{}; // original font height in source pixels
    std::array<float,4> bounds{}; // xmin,xmax,ymin,ymax in target-local pixels
    std::array<float,6> matrix{}; // original font/field transform in source pixels
    std::array<std::uint8_t,4> rgba{};
    unsigned align{}; // original SWF 0left,1right,2centre
    float leading{}; // source pixels
    std::array<float,4> local_bounds{}; // source pixels, before matrix
    std::array<float,3> font_metrics{}; // ascent,descent,leading normalized1024-em
    std::array<float,3> margins{}; // left,right,indent in source pixels
};
struct HudTargetGeometry {
    HudGeometry art;
    std::vector<HudTargetTextField> text_fields;
    std::array<float,4> bounds{}; // includes original name/level text fields
};
struct HudTargetMarkerArt {
    unsigned interaction_type{};
    int effect_set{};
    std::string model_uri; // empty means source deliberately has no marker
    bool self_illum{}, scale_with_anchor{};
};
const std::vector<HudTargetMarkerArt>& original_target_marker_art();

// Generated offline from original shape edges, never shape-bound rectangles.
const std::vector<HudShapeGeometry>& original_hud_shapes();

// Original source frame numbers: HP/MP 0=empty through99=full; portrait0=Warrior,
// 1=Rogue,2=Mage. Invalid frames/layouts reject, preserving output.
// Includes original contour/matrix art for portrait, HP/MP and player framing.
// Omits XP timeline, action buttons, text, and original AS viewport reflow.
bool compose_original_hud(unsigned style, unsigned hp_source_frame,
                          unsigned mp_source_frame, unsigned portrait_source_frame,
                          HudGeometry& geometry, std::string& error);

// Real frame155 oval aperture, xmin,xmax,ymin,ymax in authored480x320 pixels.
// Apply exactly the same uniform scale and origin used when drawing the HUD.
// Portrait paint is aligned to this aperture by translation only, matching the
// existing art-derived Android correction; original scale, UVs and border stay.
bool original_hud_portrait_bounds(unsigned style, std::array<float,4>& bounds,
                                  std::string& error);

// Original enemy clip142, source 'show' frame2; HP0empty..99full. Coordinates are
// target-local pixels. Caller supplies actual name/level and projects its actor.
// Anchor the complete bounds' bottom-centre to projected actor head, following
// enemy_hud_presentation_v2; name/level typography metadata is source-derived.
bool compose_original_target_hud(unsigned hp_source_frame,
                                 HudTargetGeometry& geometry, std::string& error);

// Recovered single-line plain-text formatter: original font leading/descent,
// field margins and source WIDTH_FUDGE80twips alignment. Returns target-local
// pixel baseline; transform glyph offsets with field.matrix's linear part.
bool layout_original_target_text(const HudTargetTextField& field,
                                 float source_run_advance,
                                 std::array<float,2>& baseline,
                                 std::string& error);

// Apply original SWF font7 kerning/embedded advance overrides once to a fresh
// original-device-font raster run, before measuring or centring the text.
bool apply_original_target_font_layout(std::uint32_t source_font,
                                      float source_height,
                                      HudGlyphRun& run, std::string& error);

} // namespace dh::foundation
