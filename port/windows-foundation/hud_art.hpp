#pragma once

#include "texture_loader.hpp"
#include <array>

namespace dh::foundation {

// Original SWF bitmap fill information, independent of the ActionScript graph.
// Matrix order [a,b,c,d,tx,ty]: x=a*u+c*v+tx; y=b*u+d*v+ty.
// x/y are shape-local twips, u/v are source atlas pixels.
struct HudArtRegion {
    std::string role;
    std::uint32_t shape_id{};
    std::array<double, 4> bounds_twips{}; // xmin,xmax,ymin,ymax
    std::array<double, 6> bitmap_matrix_twips{};
};

struct HUDSkin {
    TextureImage atlas;
    std::vector<HudArtRegion> regions;
};

// Exact original dqhud_droid bitmap1 fills. Enemy HP reuses hp_* regions.
// Shape outlines, placements, masks and timeline animations are separate:
// bounds are metadata, never a substitute for original contour geometry.
std::vector<HudArtRegion> original_hud_regions();

// Supplied original atlas/descriptors; no legacy movie or UI event runtime.
// Transactional on failure. Original bitmap1 is MenusGraphics_droid.tga.
bool load_hud_skin(const std::filesystem::path& atlas_path,
                   const std::vector<HudArtRegion>& regions,
                   HUDSkin& skin, std::string& error);

// Map an actual shape vertex to normalized texture coordinates. Coordinates
// use the loader's top-row-first atlas convention; no contour is fabricated.
bool hud_art_uv(const HUDSkin& skin, const HudArtRegion& region,
                double x_twips, double y_twips,
                std::array<float, 2>& uv, std::string& error);

} // namespace dh::foundation
