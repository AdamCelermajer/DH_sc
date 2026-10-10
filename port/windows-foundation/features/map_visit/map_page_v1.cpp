#include "map_page_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation::map_visit {
namespace {
constexpr float pi = 3.14159265358979323846f;
}

MapExtentV1 map_extent_v1(const std::vector<LevelModuleZone>& zones) {
    MapExtentV1 e;
    for (const auto& zone : zones) {
        const auto& b = zone.bounds;
        if (!e.valid) {
            e.valid = true;
            e.minX = b[0]; e.minY = b[1]; e.minZ = b[2];
            e.maxX = b[3]; e.maxY = b[4]; e.maxZ = b[5];
        } else {
            e.minX = std::min(e.minX, b[0]); e.minY = std::min(e.minY, b[1]); e.minZ = std::min(e.minZ, b[2]);
            e.maxX = std::max(e.maxX, b[3]); e.maxY = std::max(e.maxY, b[4]); e.maxZ = std::max(e.maxZ, b[5]);
        }
    }
    return e;
}

MapViewV1 map_zoom_step_v1(MapViewV1 view, float delta) {
    if (!std::isfinite(delta)) return view;
    view.zoom = std::clamp(view.zoom * (delta > 0 ? 1.25f : (delta < 0 ? 0.8f : 1.0f)), map_zoom_min, map_zoom_max);
    return view;
}

MapViewV1 map_reset_zoom_v1(MapViewV1 view) {
    view.zoom = map_zoom_min;
    return view;
}

bool map_camera_v1(const MapExtentV1& extent, const std::optional<std::array<float, 3>>& player,
                   const MapViewV1& view, Camera& out, std::string& error) {
    if (!extent.valid) { error = "Map camera requires at least one module zone"; return false; }
    if (!std::isfinite(view.zoom) || view.zoom < map_zoom_min || view.zoom > map_zoom_max) {
        error = "Map zoom is outside its authored range"; return false;
    }
    const float centerX = (extent.minX + extent.maxX) * 0.5f;
    const float centerY = (extent.minY + extent.maxY) * 0.5f;
    // Full-level view centres on the level; a zoomed view follows the player.
    float cx = centerX, cy = centerY;
    if (view.zoom > map_zoom_min && player) {
        if (!std::isfinite((*player)[0]) || !std::isfinite((*player)[1])) { error = "Map player position is nonfinite"; return false; }
        cx = (*player)[0];
        cy = (*player)[1];
    }
    // Radius that encloses the level's XY corners from the centre (the reset view must show all of it).
    float radius = 0;
    for (const float x : {extent.minX, extent.maxX})
        for (const float y : {extent.minY, extent.maxY})
            radius = std::max(radius, std::hypot(x - centerX, y - centerY));
    radius = std::max(radius, 1.0f);
    const float halfTan = std::tan(map_camera_fov_radians * 0.5f);
    const float distance = radius * map_fit_margin / (halfTan * view.zoom);
    const float depthSpan = std::max(0.0f, extent.maxZ - extent.minZ);
    out = Camera{};
    out.eye = {cx, cy, extent.maxZ + distance};
    out.target = {cx, cy, extent.minZ};
    out.up = {-1.0f, 1.0f, 0.0f};
    out.verticalFovDegrees = map_camera_fov_radians * 180.0f / pi;
    out.nearPlane = distance * 0.5f;
    out.farPlane = distance * 2.0f + depthSpan + 1000.0f;
    out.aspectRatio = map_camera_aspect;
    error.clear();
    return true;
}

bool map_project_v1(const Camera& camera, const MapRectV1& rect, const std::array<float, 3>& world, float& px, float& py) {
    if (!(rect.width > 0) || !(rect.height > 0) || !std::isfinite(world[0]) || !std::isfinite(world[1]) || !std::isfinite(world[2])) return false;
    CameraBasisV1 basis;
    std::string error;
    if (!camera_basis_v1(camera, camera.aspectRatio > 0 ? camera.aspectRatio : rect.width / rect.height, basis, error)) return false;
    const std::array<float, 3> d{world[0] - basis.eye[0], world[1] - basis.eye[1], world[2] - basis.eye[2]};
    auto dot3 = [](const std::array<float, 3>& a, const std::array<float, 3>& b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; };
    const float depth = dot3(d, basis.forward);
    if (!(depth > 0.0f)) return false;
    const float ndcX = dot3(d, basis.right) / (depth * basis.tanHalfX);
    const float ndcY = dot3(d, basis.up) / (depth * basis.tanHalfY);
    if (ndcX < -1.0f || ndcX > 1.0f || ndcY < -1.0f || ndcY > 1.0f) return false;
    px = rect.x + (ndcX + 1.0f) * 0.5f * rect.width;
    py = rect.y + (1.0f - ndcY) * 0.5f * rect.height;
    return true;
}

} // namespace dh::foundation::map_visit
