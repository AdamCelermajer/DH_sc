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
// Omits action buttons, text, and original AS viewport reflow.
// This legacy overload also omits the XP bar (shapes 148/150).
bool compose_original_hud(unsigned style, unsigned hp_source_frame,
                          unsigned mp_source_frame, unsigned portrait_source_frame,
                          HudGeometry& geometry, std::string& error);

// Same, plus the original XP bar (bar_xp, char152: background 148 + shrinking
// cover 150). xp_source_frame 0=empty .. 99=full; the original producer
// (InfoHUDManager::FastUpdate 0x41e064) is min(99, 100*xp/xp_for_level) from
// player properties 33/34, with no -1 (unlike HP/MP) and frame100 never used.
bool compose_original_hud(unsigned style, unsigned hp_source_frame,
                          unsigned mp_source_frame, unsigned xp_source_frame,
                          unsigned portrait_source_frame,
                          HudGeometry& geometry, std::string& error);

// Original InfoHUDManager::FastUpdate (0x41e064) bar_xp frame from the raw
// resolved player-sheet values (property 33 = XP, 34 = XP for this level; the
// original reads Character+4220/+4224 as signed 32-bit ints). Returns false when
// the maximum is zero. 100*xp wraps in 32 bits like the ARM code, the quotient
// is the signed C division, and the result is capped at 99 as in the original;
// a negative quotient (not producible by valid sheets) is clamped to frame 0.
inline bool original_hud_xp_frame(std::int32_t xp, std::int32_t xp_for_level,
                                  unsigned& frame) {
    if (xp_for_level == 0) return false;
    const auto scaled = static_cast<std::int32_t>(static_cast<std::uint32_t>(xp) * 100u);
    const auto quotient = static_cast<std::int64_t>(scaled) / xp_for_level;
    frame = quotient < 0 ? 0u : quotient > 99 ? 99u : static_cast<unsigned>(quotient);
    return true;
}

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
