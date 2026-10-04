#include "swf_font_geometry.hpp"
#include "gameswf/gameswf_font.h"
#include "gameswf/gameswf_shape.h"
#include <cmath>
#include <utility>
#ifdef isfinite
#undef isfinite
#endif

namespace dh2::ui {
namespace {
bool finite(float v) { return std::isfinite(v); }
constexpr int max_glyphs = 65536;
constexpr int max_paths = 1 << 20;
constexpr std::size_t max_edges = 1 << 22;
}
int swf_glyph_outline(SwfGlyphOutline& out, const gameswf::font& font,
                     std::int32_t index, std::string& error) {
    error.clear();
    const int count = font.get_glyph_count();
    if (count < 0 || count > max_glyphs) { error = "invalid core glyph count"; return -1; }
    if (index < 0 || index >= count) return 0;
    SwfGlyphOutline next;
    next.font_name.assign(font.get_name().c_str(), font.get_name().size());
    next.glyph_index = index; next.character_code = font.get_code_by_index(index);
    next.bold = font.is_bold(); next.italic = font.is_italic();
    next.alignment_zones = font.is_define_font3();
    const auto* shape = font.get_glyph_by_index(index);
    next.shape_present = shape != nullptr;
    if (shape) {
        const auto& paths = shape->get_paths();
        if (paths.size() < 0 || paths.size() > max_paths) { error = "invalid core path count"; return -1; }
        std::size_t edges = 0;
        next.paths.reserve(static_cast<std::size_t>(paths.size()));
        for (int i = 0; i < paths.size(); ++i) {
            const auto& path = paths[i];
            if (!finite(path.m_ax) || !finite(path.m_ay) || path.m_edges.size() < 0 ||
                static_cast<std::size_t>(path.m_edges.size()) > max_edges - edges) {
                error = "invalid core glyph path"; return -1;
            }
            edges += static_cast<std::size_t>(path.m_edges.size());
            SwfGlyphPath copied;
            copied.fill0 = path.m_fill0; copied.fill1 = path.m_fill1; copied.line = path.m_line;
            copied.start_x = path.m_ax; copied.start_y = path.m_ay; copied.new_shape = path.m_new_shape;
            copied.edges.reserve(static_cast<std::size_t>(path.m_edges.size()));
            for (int j = 0; j < path.m_edges.size(); ++j) {
                const auto& edge = path.m_edges[j];
                if (!finite(edge.m_cx) || !finite(edge.m_cy) || !finite(edge.m_ax) || !finite(edge.m_ay)) {
                    error = "invalid core glyph edge"; return -1;
                }
                copied.edges.push_back({edge.m_cx, edge.m_cy, edge.m_ax, edge.m_ay});
            }
            next.paths.push_back(std::move(copied));
        }
    }
    out = std::move(next); return 1;
}
int swf_glyph_outline_for_code(SwfGlyphOutline& out, const gameswf::font& font,
                              std::uint32_t code, std::string& error) {
    error.clear();
    if (code > 65535) { error = "SWF font code exceeds 16 bits"; return -1; }
    const int count = font.get_glyph_count();
    if (count < 0 || count > max_glyphs) { error = "invalid core glyph count"; return -1; }
    for (int i = 0; i < count; ++i)
        if (font.get_code_by_index(i) == static_cast<int>(code)) return swf_glyph_outline(out, font, i, error);
    return 0;
}
int swf_glyph_metrics(SwfGlyphMetrics& out, const gameswf::font& font,
                     std::uint32_t code, std::int32_t size, std::string& error) {
    error.clear();
    if (code > 65535 || size <= 0) { error = "invalid glyph metrics request"; return -1; }
    gameswf::glyph glyph;
    if (!font.get_glyph(&glyph, static_cast<Uint16>(code), size)) return 0;
    if (!finite(glyph.m_glyph_advance)) { error = "nonfinite core glyph advance"; return -1; }
    SwfGlyphMetrics next;
    next.glyph_index = glyph.m_glyph_index; next.advance = glyph.m_glyph_advance;
    next.bitmap_present = glyph.m_bitmap_info != nullptr; next.outline_present = glyph.m_shape_glyph != nullptr;
    next.alignment_zones = font.is_define_font3(); out = next; return 1;
}
}
