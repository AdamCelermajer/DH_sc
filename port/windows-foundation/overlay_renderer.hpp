#pragma once

#include <array>
#include <cstdint>
#include <cstddef>
#include <vector>

namespace dh::foundation {

struct OverlaySprite {
    float x = 0, y = 0, width = 0, height = 0;
    float u0 = 0, v0 = 0, u1 = 1, v1 = 1;
    std::uint32_t texture = 0;
    std::array<float,4> color{1,1,1,1};
};

enum class ProgressDirection { LeftToRight, BottomToTop };

struct OverlayTriangleVertex {
    float x = 0, y = 0, u = 0, v = 0;
};

// Sprite coordinates use screen pixels with a top-left origin. Texture handles
// refer to original UI assets uploaded through Renderer::createTexture().
class OverlayRenderer {
public:
    void begin(int width, int height);
    void drawSprite(const OverlaySprite& sprite);
    void drawProgress(const OverlaySprite& sprite, float fraction,
                      ProgressDirection direction = ProgressDirection::LeftToRight);
    // Count must describe complete triangles. Returns false for invalid data;
    // no partial geometry is drawn. Coordinates and UVs must be finite.
    // Texture zero draws genuine solid-fill geometry with the supplied color.
    bool drawTriangles(const OverlayTriangleVertex* vertices, std::size_t count,
                       std::uint32_t texture,
                       const std::array<float,4>& color = {1,1,1,1});
    bool drawTriangles(const std::vector<OverlayTriangleVertex>& vertices,
                       std::uint32_t texture,
                       const std::array<float,4>& color = {1,1,1,1}) {
        return drawTriangles(vertices.data(), vertices.size(), texture, color);
    }
    void end();
private:
    bool active_ = false;
    int previousMatrixMode_ = 0;
};

} // namespace dh::foundation
