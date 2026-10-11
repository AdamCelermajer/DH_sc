#include "death_screen_v1.hpp"
#include "death_screen_art_geometry_v1.hpp"
#include "death_letterbox_black_v1.hpp"

#include "../../overlay_renderer.hpp"

#include <algorithm>
#include <vector>

namespace dh::foundation::persistence::death_flow {
namespace {
std::array<float, 6> with_parent_transform(std::array<float, 6> matrix) {
    const auto& timeline = original_death_screen_timeline_v1();
    matrix[4] += timeline.root_x_twips;
    matrix[5] += timeline.root_y_twips;
    return matrix;
}

OverlayTriangleVertex project_vertex(const art_data::Vertex& vertex,
                                     const std::array<float, 6>& matrix,
                                     float scale, float offset_x, float offset_y) {
    const float x = (matrix[0] * vertex.x + matrix[2] * vertex.y + matrix[4]) / 20.f;
    const float y = (matrix[1] * vertex.x + matrix[3] * vertex.y + matrix[5]) / 20.f;
    return {offset_x + x * scale, offset_y + y * scale, 0.f, 0.f};
}

template <std::size_t N>
bool draw_panel_fill(OverlayRenderer& overlay,
                     const std::array<art_data::Vertex, N>& vertices,
                     const std::array<float, 4>& color,
                     const std::array<float, 6>& matrix,
                     float alpha, float scale, float ox, float oy, std::string& error) {
    std::vector<OverlayTriangleVertex> projected;
    projected.reserve(vertices.size());
    for (const auto& vertex : vertices)
        projected.push_back(project_vertex(vertex, matrix, scale, ox, oy));
    auto color_with_alpha = color;
    color_with_alpha[3] *= alpha;
    if (!overlay.drawTriangles(projected, 0, color_with_alpha)) {
        error = "Invalid source death panel triangles";
        return false;
    }
    return true;
}

bool draw_panel(OverlayRenderer& overlay, const std::array<float, 6>& matrix,
                std::uint8_t alpha, float scale, float ox, float oy, std::string& error) {
    const float factor = static_cast<float>(alpha) / 255.f;
    return draw_panel_fill(overlay, art_data::kPanelFill1,
                           art_data::kPanelFill1Color, matrix, factor, scale, ox, oy, error) &&
           draw_panel_fill(overlay, art_data::kPanelFill2,
                           art_data::kPanelFill2Color, matrix, factor, scale, ox, oy, error);
}

bool draw_fade(OverlayRenderer& overlay, const std::array<float, 6>& matrix,
               std::uint8_t alpha, float scale, float ox, float oy,
               std::string& error) {
    std::vector<OverlayTriangleVertex> projected;
    projected.reserve(art_data::kFade.size());
    for (const auto& vertex : art_data::kFade)
        projected.push_back(project_vertex(vertex, matrix, scale, ox, oy));
    const std::array<float, 4> black{0.f, 0.f, 0.f,
                                     static_cast<float>(alpha) / 255.f};
    if (!overlay.drawTriangles(projected, 0, black)) {
        error = "Invalid source death fade triangles";
        return false;
    }
    return true;
}
} // namespace

bool draw_original_death_screen_v1(const DeathScreenFrameV1& frame,
                                   OverlayRenderer& overlay,
                                   int viewport_width, int viewport_height,
                                   const DeathScreenDrawServicesV1& services,
                                   std::string& error) {
    if (!services.draw_localized_quote || viewport_width <= 0 || viewport_height <= 0) {
        error = "death screen viewport and localized-text provider are required";
        return false;
    }
    if (frame.quote_number < 1 || frame.quote_number > 5 || frame.draw_count > frame.draw_items.size()) {
        error = "invalid projected death screen frame";
        return false;
    }
    const float scale = std::min(static_cast<float>(viewport_width) / 480.f,
                                 static_cast<float>(viewport_height) / 320.f);
    const float ox = (static_cast<float>(viewport_width) - 480.f * scale) * 0.5f;
    const float oy = (static_cast<float>(viewport_height) - 320.f * scale) * 0.5f;
    // B082: the black panel surround and the black fade cover only the letterboxed stage; the bars outside
    // it take the strongest black alpha of those layers (drawn first, bars never overlap the stage).
    std::uint8_t bar_alpha = 0;
    for (std::size_t i = 0; i < frame.draw_count; ++i) {
        const auto role = frame.draw_items[i].role;
        if (role == DeathScreenArtRoleV1::original_panel_art || role == DeathScreenArtRoleV1::black_fade_overlay)
            bar_alpha = std::max(bar_alpha, frame.draw_items[i].alpha);
    }
    if (!draw_letterbox_black_v1(overlay, viewport_width, viewport_height, scale, ox, oy, bar_alpha)) {
        error = "Invalid letterbox black triangles";
        return false;
    }
    for (std::size_t i = 0; i < frame.draw_count; ++i) {
        const auto& item = frame.draw_items[i];
        const auto matrix = with_parent_transform(item.matrix_twips);
        bool ok = false;
        if (item.role == DeathScreenArtRoleV1::localized_death_quote) {
            ok = services.draw_localized_quote(services.context, frame.quote_number,
                                               matrix, item.local_bounds_twips, item.alpha, error);
        } else if (item.role == DeathScreenArtRoleV1::original_panel_art) {
            ok = draw_panel(overlay, matrix, item.alpha, scale, ox, oy, error);
        } else if (item.role == DeathScreenArtRoleV1::black_fade_overlay) {
            ok = draw_fade(overlay, matrix, item.alpha, scale, ox, oy, error);
        }
        if (!ok) return false;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::persistence::death_flow
