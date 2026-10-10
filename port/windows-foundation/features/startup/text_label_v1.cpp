#include "text_label_v1.hpp"

#include "asset_catalog.hpp"
#include "content_paths.hpp"
#include "hud_glyphs.hpp"
#include "renderer.hpp"

#include <algorithm>

namespace dh::foundation::startup {
namespace {

bool load_font(const AssetCatalog& assets, HudGlyphFont& font, std::string& error) {
    return font.load(resolve_content_path(assets, "data/Fontin SmallCaps.ttf"), error);
}

// Appends the glyph sprites of one run at (x0, baseline).
void append_run(Renderer& renderer, const HudGlyphRun& run, float x0, float baseline, std::array<float, 4> color,
                TextLabel& label) {
    for (const auto& g : run.glyphs) {
        if (g.image.rgba.empty()) continue;  // blank glyphs (spaces) draw nothing
        const std::uint32_t texture = renderer.createTexture(int(g.image.width), int(g.image.height), g.image.rgba.data());
        label.textures.push_back(texture);
        OverlaySprite s;
        s.x = x0 + g.x;
        s.y = baseline + g.y;
        s.width = g.width;
        s.height = g.height;
        s.u1 = g.u1;
        s.v1 = g.v1;
        s.texture = texture;
        s.color = color;
        label.sprites.push_back(s);
    }
}

} // namespace

void build_text_label(Renderer& renderer, const AssetCatalog& assets, const char* text, int size, LabelAnchor anchor,
                      int w, int h, TextLabel& label) {
    label.built = true;
    HudGlyphFont font;
    std::string error;
    if (!load_font(assets, font, error)) {
        label.error = std::string(text) + " font: " + error;
        return;
    }
    HudGlyphRun run;
    if (!font.raster(text, size, 1.0f, run, error)) {
        label.error = std::string(text) + " raster: " + error;
        return;
    }
    const bool right = anchor == LabelAnchor::right_bottom;
    const float baseline = right ? float(h) * 0.92f : float(h) * 0.86f;
    const float x0 = right ? float(w) * 0.96f - run.advance : (float(w) - run.advance) * 0.5f;
    append_run(renderer, run, x0, baseline, {1, 1, 1, 1}, label);
}

void build_wrapped_label(Renderer& renderer, const AssetCatalog& assets, const std::string& text, int size,
                         float x, float y, float width, std::array<float, 4> color, TextLabel& label) {
    label.built = true;
    HudGlyphFont font;
    std::string error;
    if (!load_font(assets, font, error)) {
        label.error = "tip font: " + error;
        return;
    }
    // Greedy word wrap, measured with the real font advances (source pixels).
    std::vector<std::string> lines;
    std::string current;
    std::size_t start = 0;
    while (start <= text.size()) {
        const std::size_t end = text.find(' ', start);
        const std::string word = text.substr(start, end == std::string::npos ? std::string::npos : end - start);
        const std::string candidate = current.empty() ? word : current + " " + word;
        HudGlyphRun probe;
        if (!font.raster(candidate, size, 1.0f, probe, error)) {
            label.error = "tip raster: " + error;
            return;
        }
        if (!current.empty() && probe.advance > width) {
            lines.push_back(current);
            current = word;
        } else {
            current = candidate;
        }
        if (end == std::string::npos) break;
        start = end + 1;
    }
    if (!current.empty()) lines.push_back(current);

    const float lineHeight = float(size) * 1.25f;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        HudGlyphRun run;
        if (!font.raster(lines[i], size, 1.0f, run, error)) {
            label.error = "tip raster: " + error;
            return;
        }
        const float baseline = y + float(size) + float(i) * lineHeight;
        append_run(renderer, run, x + (width - run.advance) * 0.5f, baseline, color, label);
    }
    label.height = float(lines.size()) * lineHeight;
}

void destroy_text_label(Renderer& renderer, TextLabel& label) {
    for (auto texture : label.textures) renderer.destroyTexture(texture);
    label.textures.clear();
    label.sprites.clear();
}

} // namespace dh::foundation::startup
