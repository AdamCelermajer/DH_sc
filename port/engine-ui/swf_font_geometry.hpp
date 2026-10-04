#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace gameswf { struct font; }
namespace dh2::ui {
struct SwfQuadraticEdge { float control_x{}, control_y{}, anchor_x{}, anchor_y{}; };
struct SwfGlyphPath {
    std::int32_t fill0{}, fill1{}, line{};
    float start_x{}, start_y{};
    bool new_shape{};
    std::vector<SwfQuadraticEdge> edges;
};
struct SwfGlyphOutline {
    std::string font_name;
    std::int32_t glyph_index{}, character_code{};
    bool bold{}, italic{}, alignment_zones{}, shape_present{};
    // Coordinates are the actual core's font SHAPE units, without screen scaling.
    std::vector<SwfGlyphPath> paths;
};
struct SwfGlyphMetrics {
    std::int32_t glyph_index{-1};
    float advance{512.0f};
    bool bitmap_present{}, outline_present{}, alignment_zones{};
};
// Borrow the font only for this call. Returned strings/paths own all their data.
// This never changes the core's global glyph provider or rasterizes an outline.
// 1 copied, 0 index/code absent, -1 invalid input/core data; output is atomic.
int swf_glyph_outline(SwfGlyphOutline& out, const gameswf::font& font,
                      std::int32_t glyph_index, std::string& error);
int swf_glyph_outline_for_code(SwfGlyphOutline& out, const gameswf::font& font,
                               std::uint32_t code, std::string& error);
// Invoke the current upstream provider normally. This reports upstream behavior;
// original DH2 provider-first advance differs when an embedded advance is present.
int swf_glyph_metrics(SwfGlyphMetrics& out, const gameswf::font& font,
                     std::uint32_t code, std::int32_t font_size, std::string& error);
}
