#include "hud_art.hpp"
#include <cmath>
#include <new>
#include <utility>

namespace dh::foundation {

std::vector<HudArtRegion> original_hud_regions() {
    // Values decoded directly from the original dqhud_droid.swf bitmap fills.
    // Exact source offsets, hashes and placement timelines: reports/hud-source.json.
    return {
        {"portrait_warrior", 38, {-222, 837, -161, 869}, {15.10906982421875, 0, 0, 15.10906982421875, -10293, -10052}},
        {"portrait_rogue", 39, {-251, 808, -249, 839}, {15.10906982421875, 0, 0, 15.10906982421875, -9234, -10082}},
        {"portrait_mage", 40, {-257, 802, -213, 875}, {15.10906982421875, 0, 0, 15.10906982421875, -8160, -10082}},
        {"hp_background", 86, {-119, 1827, 87, 246}, {20, 0, 0, 20, -18536, -9561}},
        {"hp_fill", 88, {141, 2101, 442, 599}, {22.97454833984375, 0, 0, 20, -12431, -10109}},
        {"mp_background", 143, {141, 2078, -10, 143}, {20, 0, 0, 20.81634521484375, -18282, -10274}},
        {"mp_fill", 145, {317, 2270, 104, 266}, {20.092575073242188, 0, 0, 22.816696166992188, -18189, -10322}},
        {"xp_background", 148, {-75, 1091, 22, 108}, {20, 0, 0, 20, -18493, -10012}},
        {"xp_fill", 150, {-76, 1087, 26, 112}, {20, 0, 0, 20, -18492, -9489}},
        {"player_overlay_a", 153, {-66, 1199, -18, 176}, {20, 0, 0, 20, -17179, -9539}},
        {"player_overlay_b", 155, {-1875, 1627, -694, 667}, {20, 0, 0, 20, -15450, -9986}}
    };
}

bool hud_art_uv(const HUDSkin& skin, const HudArtRegion& region,
                double x, double y, std::array<float, 2>& uv,
                std::string& error) {
    if (!skin.atlas.width || !skin.atlas.height || !std::isfinite(x) || !std::isfinite(y)) {
        error = "HUD atlas or shape vertex is invalid";
        return false;
    }
    const auto& m = region.bitmap_matrix_twips;
    for (double value : m) if (!std::isfinite(value)) {
        error = "HUD bitmap matrix contains nonfinite values";
        return false;
    }
    const double determinant = m[0] * m[3] - m[1] * m[2];
    if (!std::isfinite(determinant) || std::abs(determinant) < 1e-12) {
        error = "HUD bitmap matrix is singular";
        return false;
    }
    const double px = (m[3] * (x - m[4]) - m[2] * (y - m[5])) / determinant;
    const double py = (-m[1] * (x - m[4]) + m[0] * (y - m[5])) / determinant;
    if (!std::isfinite(px) || !std::isfinite(py) || px < 0 || py < 0 ||
        px > skin.atlas.width || py > skin.atlas.height) {
        error = "HUD shape vertex maps outside the source atlas";
        return false;
    }
    uv = {static_cast<float>(px / skin.atlas.width),
          static_cast<float>(py / skin.atlas.height)};
    error.clear();
    return true;
}

bool load_hud_skin(const std::filesystem::path& atlas_path,
                   const std::vector<HudArtRegion>& regions,
                   HUDSkin& skin, std::string& error) {
    try {
        HUDSkin loaded;
        if (!load_texture(atlas_path, loaded.atlas, error)) return false;
        if (regions.empty()) {
            error = "HUD skin has no supplied original bitmap fills";
            return false;
        }
        loaded.regions = regions;
        for (const auto& region : loaded.regions) {
            const auto& b = region.bounds_twips;
            bool valid = !region.role.empty() && region.shape_id != 0;
            for (double value : b) valid = valid && std::isfinite(value);
            if (!valid || b[0] >= b[1] || b[2] >= b[3]) {
                error = "HUD region has invalid shape bounds or identity";
                return false;
            }
            std::array<float, 2> uv{};
            for (unsigned corner = 0; corner < 4; ++corner) {
                if (!hud_art_uv(loaded, region, b[corner & 1], b[2 + ((corner >> 1) & 1)], uv, error)) {
                    error = region.role + ": " + error;
                    return false;
                }
            }
        }
        skin = std::move(loaded);
        error.clear();
        return true;
    } catch (const std::bad_alloc&) {
        error = "Cannot allocate HUD artwork";
        return false;
    }
}

} // namespace dh::foundation
