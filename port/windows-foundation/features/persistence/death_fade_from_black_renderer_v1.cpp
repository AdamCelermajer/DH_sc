#include "death_fade_from_black_v1.hpp"

#include "death_screen_art_geometry_v1.hpp"
#include "../../overlay_renderer.hpp"

#include <algorithm>
#include <vector>

namespace dh::foundation::persistence::death_flow {
namespace {
OverlayTriangleVertex project(const art_data::Vertex& vertex,
                              float scale, float ox, float oy) {
    // Sprite743 root at (-20,-60) twips and nested sprite738 at (-3,+1).
    // The composed source matrix is [1,0,0,1,-23,-59].
    const float x = (vertex.x - 23.f) / 20.f;
    const float y = (vertex.y - 59.f) / 20.f;
    return {ox + x * scale, oy + y * scale, 0.f, 0.f};
}
std::uint8_t alpha_for_frame(std::uint8_t frame) {
    if (frame == 0) return 255;
    if (frame >= 32) return 0;
    return static_cast<std::uint8_t>(256 - frame * 8);
}
}

bool draw_original_fade_from_black_v1(const FadeFromBlackFrameV1& frame,
                                      ::dh::foundation::OverlayRenderer& overlay,
                                      int viewport_width, int viewport_height,
                                      std::string& error) {
    if (viewport_width <= 0 || viewport_height <= 0 || frame.frame > 32 ||
        frame.black_alpha != alpha_for_frame(frame.frame) ||
        frame.pop_menu_callback != (frame.frame == 32)) {
        error = "Invalid source sprite743 fade frame or viewport";
        return false;
    }
    const float scale = std::min(static_cast<float>(viewport_width) / 480.f,
                                 static_cast<float>(viewport_height) / 320.f);
    const float ox = (static_cast<float>(viewport_width) - 480.f * scale) * 0.5f;
    const float oy = (static_cast<float>(viewport_height) - 320.f * scale) * 0.5f;
    std::vector<OverlayTriangleVertex> projected;
    projected.reserve(art_data::kFade.size());
    for (const auto& vertex : art_data::kFade)
        projected.push_back(project(vertex, scale, ox, oy));
    const std::array<float, 4> black{0.f, 0.f, 0.f,
                                     static_cast<float>(frame.black_alpha) / 255.f};
    if (!overlay.drawTriangles(projected, 0, black)) {
        error = "Invalid source sprite743 black fade triangles";
        return false;
    }
    error.clear();
    return true;
}

} // namespace dh::foundation::persistence::death_flow
