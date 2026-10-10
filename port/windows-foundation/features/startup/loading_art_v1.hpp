#pragma once

// Original campaign loading menu geometry (menu_Loading in data/menus/dqshared.swf), exported by
// tools/export_loading_art.py. Coordinates are authored stage pixels (480x320); texture indices are
// 1 = MenusGraphics_droid.tga (0 = solid colour, see Layer::color).

#include <array>
#include <cstdint>
#include <vector>

namespace dh::foundation::startup {

struct ArtVertex { float x, y, u, v; };
struct Layer {
    int texture;                    // 1 = MenusGraphics_droid.tga, 0 = solid colour
    std::array<float, 4> color;
    std::vector<ArtVertex> verts;   // triangle list
};
struct LayerSpan { const Layer* layers; int count; };
struct FrameInfo {
    float maskX0, maskX1, maskY0, maskY1;   // progress mask rectangle (clips the red fill)
    float spark[6];                         // spark matrix a,b,c,d,tx,ty (stage px)
    float sparkAlpha;
};
struct TextRect { float x, y, w, h, size; std::uint8_t rgba[4]; };
struct LoadingArt {
    LayerSpan bg, frame, track, fill, spark;
    const FrameInfo* frames;        // 101 entries, one per loading_anim frame (0..100 %)
    TextRect heading, tip;
    float stageW, stageH;
};

const LoadingArt& loading_art() noexcept;

}  // namespace dh::foundation::startup
