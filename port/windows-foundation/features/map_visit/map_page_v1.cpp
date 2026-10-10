#include "map_page_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation::map_visit {
namespace {
constexpr float pi = 3.14159265358979323846f;
constexpr float invSqrt2 = 0.70710678118654752f;
constexpr float map_far_plane = 100000.0f; // authored CameraBase::SetData far

// Authored up vector (IDA CreateMapCamera: vtable+276 with (-1, 1, 0)).
constexpr std::array<float, 3> map_up{-1.0f, 1.0f, 0.0f};

// Eye and target offsets with the zoom applied to the eye distance (zoom 2 halves the eye-target distance).
void scaled_offsets(const MapCameraPoseV1& pose, float zoom, std::array<float, 3>& eye, std::array<float, 3>& target) {
    for (int i = 0; i < 3; ++i) {
        target[i] = pose.target_offset[i];
        eye[i] = pose.target_offset[i] + (pose.eye_offset[i] - pose.target_offset[i]) / zoom;
    }
}
} // namespace

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

MapExtentV1 map_visited_extent_v1(const std::vector<LevelModuleZone>& zones, const std::vector<bool>& visited) {
    // IDA Show: a zone counts when its centre is inside a visited zone (RoomZone::HasInside, inclusive XY).
    const auto insideVisited = [&](float x, float y) {
        for (std::size_t j = 0; j < zones.size() && j < visited.size(); ++j) {
            if (!visited[j]) continue;
            const auto& b = zones[j].bounds;
            if (x >= b[0] && x <= b[3] && y >= b[1] && y <= b[4]) return true;
        }
        return false;
    };
    MapExtentV1 e;
    for (const auto& zone : zones) {
        const auto& b = zone.bounds;
        if (!insideVisited((b[0] + b[3]) * 0.5f, (b[1] + b[4]) * 0.5f)) continue;
        if (!e.valid) {
            e.valid = true;
            e.minX = b[0]; e.minY = b[1]; e.minZ = b[2];
            e.maxX = b[3]; e.maxY = b[4]; e.maxZ = b[5];
        } else {
            e.minX = std::min(e.minX, b[0]); e.minY = std::min(e.minY, b[1]); e.minZ = std::min(e.minZ, b[2]);
            e.maxX = std::max(e.maxX, b[3]); e.maxY = std::max(e.maxY, b[4]); e.maxZ = std::max(e.maxZ, b[5]);
        }
    }
    if (!e.valid) return map_extent_v1(zones); // nothing visited yet: no padding, every zone
    // Padded by the visited width/height on each side (IDA: min - w .. max + w, min - h .. max + h).
    const float w = e.maxX - e.minX, h = e.maxY - e.minY;
    e.minX -= w; e.maxX += w;
    e.minY -= h; e.maxY += h;
    return e;
}

MapViewV1 map_zoom_step_v1(MapViewV1 view, float delta) {
    if (!std::isfinite(delta)) return view;
    view.zoom = std::clamp(view.zoom * (delta > 0 ? 1.25f : (delta < 0 ? 0.8f : 1.0f)), map_zoom_min, map_zoom_max);
    return view;
}

MapViewV1 map_pan_screen_v1(MapViewV1 view, float right, float up) {
    if (!std::isfinite(right) || !std::isfinite(up)) return view;
    // world = right * (1,1,0)/sqrt2 + up * (-1,1,0)/sqrt2
    view.panX += invSqrt2 * (right - up);
    view.panY += invSqrt2 * (right + up);
    return view;
}

MapViewV1 map_reset_zoom_v1(MapViewV1 view) {
    view.zoom = map_zoom_min;
    view.panX = 0.0f;
    view.panY = 0.0f;
    return view;
}

MapViewV1 map_clamp_view_v1(const MapCameraPoseV1& pose, const std::array<float, 3>& anchor,
                            const MapExtentV1& limits, const MapViewV1& view) {
    if (!pose.loaded || !limits.valid || !std::isfinite(view.zoom) || view.zoom < map_zoom_min || view.zoom > map_zoom_max)
        return view;
    std::array<float, 3> eyeOffset{}, targetOffset{};
    scaled_offsets(pose, view.zoom, eyeOffset, targetOffset);
    MapViewV1 next = view;
    // UpdateMapCamera: the camera position (eye XY) is clamped to the visited extent; the pan takes the difference.
    const float eyeX = anchor[0] + eyeOffset[0] + view.panX;
    const float eyeY = anchor[1] + eyeOffset[1] + view.panY;
    const float clampedX = std::clamp(eyeX, limits.minX, limits.maxX);
    const float clampedY = std::clamp(eyeY, limits.minY, limits.maxY);
    next.panX += clampedX - eyeX;
    next.panY += clampedY - eyeY;
    return next;
}

bool map_camera_v1(const MapCameraPoseV1& pose, const std::array<float, 3>& anchor,
                   const MapViewV1& view, Camera& out, std::string& error) {
    if (!pose.loaded) { error = "Map camera requires the authored minimapcameras pose"; return false; }
    if (!std::isfinite(view.zoom) || view.zoom < map_zoom_min || view.zoom > map_zoom_max) {
        error = "Map zoom is outside its range"; return false;
    }
    for (const float value : anchor) if (!std::isfinite(value)) { error = "Map anchor is nonfinite"; return false; }
    if (!std::isfinite(view.panX) || !std::isfinite(view.panY)) { error = "Map pan is nonfinite"; return false; }
    std::array<float, 3> eyeOffset{}, targetOffset{};
    scaled_offsets(pose, view.zoom, eyeOffset, targetOffset);
    const float shiftX = view.panX, shiftY = view.panY;
    out = Camera{};
    out.eye = {anchor[0] + eyeOffset[0] + shiftX, anchor[1] + eyeOffset[1] + shiftY, anchor[2] + eyeOffset[2]};
    out.target = {anchor[0] + targetOffset[0] + shiftX, anchor[1] + targetOffset[1] + shiftY, anchor[2] + targetOffset[2]};
    out.up = {map_up[0], map_up[1], map_up[2]};
    out.verticalFovDegrees = map_camera_fov_radians * 180.0f / pi;
    out.nearPlane = 1.0f;
    out.farPlane = map_far_plane;
    out.aspectRatio = map_camera_aspect;
    error.clear();
    return true;
}

float map_world_per_pixel_v1(const MapCameraPoseV1& pose, const MapViewV1& view, float rectHeightPx) {
    if (!pose.loaded || !(rectHeightPx > 0.0f) || !(view.zoom > 0.0f)) return 0.0f;
    float square = 0.0f;
    for (int i = 0; i < 3; ++i) {
        const float d = (pose.eye_offset[i] - pose.target_offset[i]) / view.zoom;
        square += d * d;
    }
    // The visible height at the target distance is 2 d tan(fov/2); it spans the rectangle height in pixels.
    return 2.0f * std::sqrt(square) * std::tan(map_camera_fov_radians * 0.5f) / rectHeightPx;
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
