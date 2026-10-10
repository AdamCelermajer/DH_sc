#pragma once

// Rasterised authored text for the startup screens (Preview 15): one texture per glyph,
// built once. Font = original Fontin SmallCaps (data/Fontin SmallCaps.ttf) through HudGlyphFont.
// The screens' exact typography is not verified against the original.

#include "overlay_renderer.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace dh::foundation {
class AssetCatalog;
class Renderer;
}

namespace dh::foundation::startup {

struct TextLabel {
    std::vector<OverlaySprite> sprites;
    std::vector<std::uint32_t> textures;
    bool built = false;
    float height = 0.0f;   // total height of the block (wrapped labels)
    std::string error;
};

enum class LabelAnchor { center, right_bottom };

// One centred or right-anchored line at the given source size, placed in a w x h window.
void build_text_label(Renderer& renderer, const AssetCatalog& assets, const char* text, int size, LabelAnchor anchor,
                      int w, int h, TextLabel& label);

// Word-wrapped, centred block inside [x, x+width] starting at top y (pixels). Lines use the
// same font; line advance = size * 1.25. Color multiplies the glyph alpha.
void build_wrapped_label(Renderer& renderer, const AssetCatalog& assets, const std::string& text, int size,
                         float x, float y, float width, std::array<float, 4> color, TextLabel& label);

void destroy_text_label(Renderer& renderer, TextLabel& label);

} // namespace dh::foundation::startup
