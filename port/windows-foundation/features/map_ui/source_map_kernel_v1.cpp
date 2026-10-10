#include "source_map_kernel_v1.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace dh::foundation::map_ui {
namespace {
bool fail(std::string& error, const char* message) { error = message; return false; }
bool finite(float v) { return std::isfinite(v); }
bool valid_bounds(const Bounds3V1& b) {
    return finite(b.min_x) && finite(b.min_y) && finite(b.min_z) &&
           finite(b.max_x) && finite(b.max_y) && finite(b.max_z) &&
           b.min_x <= b.max_x && b.min_y <= b.max_y && b.min_z <= b.max_z;
}
}

bool source_map_show_bounds_v1(const std::vector<RoomZoneV1>& zones,
                              const std::vector<MapObjectV1>& objects,
                              const RoomServicesV1& services,
                              std::vector<std::shared_ptr<void>>& visited_out,
                              std::vector<std::shared_ptr<void>>& unvisited_out,
                              MapBoundsV1& bounds_out, std::string& error) {
    if (!services.has_inside) return fail(error, "Map Show: required actual RoomZone::HasInside service");
    std::vector<std::shared_ptr<void>> visited, unvisited;
    visited.reserve(zones.size());
    unvisited.reserve(zones.size());
    for (const auto& zone : zones) {
        if (!zone.owner) return fail(error, "Map Show: null live Level RoomZone");
        (zone.visited ? visited : unvisited).push_back(zone.owner);
    }

    bool have_x = false, have_y = false;
    float min_x = std::numeric_limits<float>::infinity();
    float min_y = std::numeric_limits<float>::infinity();
    float max_x = -std::numeric_limits<float>::infinity();
    float max_y = -std::numeric_limits<float>::infinity();
    for (const auto& object : objects) {
        if (!object.owner || !object.set_visible || !valid_bounds(object.bounds))
            return fail(error, "Map Show: required actual Level object bounds/visibility owner");
        const auto& b = object.bounds;
        const Point3V1 center{(b.min_x + b.max_x) * 0.5f,
                              (b.min_y + b.max_y) * 0.5f,
                              (b.min_z + b.max_z) * 0.5f};
        bool inside = false;
        for (const auto& room : visited) {
            bool hit = false;
            if (!services.has_inside(room, center, hit, error)) return false;
            if (hit) { inside = true; break; }
        }
        if (!object.set_visible(inside, error)) return false;
        if (!inside) continue;
        min_x = std::min(min_x, b.min_x); max_x = std::max(max_x, b.max_x);
        min_y = std::min(min_y, b.min_y); max_y = std::max(max_y, b.max_y);
        have_x = have_y = true;
    }

    if (!have_x) min_x = max_x = 0.0f;
    if (!have_y) min_y = max_y = 0.0f;
    const float range_x = max_x - min_x;
    const float range_y = max_y - min_y;
    MapBoundsV1 next{min_x - range_x, min_y - range_y,
                     max_x + range_x, max_y + range_y};
    if (!finite(next.min_x) || !finite(next.min_y) ||
        !finite(next.max_x) || !finite(next.max_y))
        return fail(error, "Map Show: source Level object map bounds overflow");
    visited_out = std::move(visited);
    unvisited_out = std::move(unvisited);
    bounds_out = next;
    error.clear();
    return true;
}

bool source_map_camera_offset_v1(const MapBoundsV1& bounds, const Point3V1& player,
                                 Point3V1& offset, std::string& error) {
    if (!finite(bounds.min_x) || !finite(bounds.min_y) ||
        !finite(bounds.max_x) || !finite(bounds.max_y) ||
        bounds.min_x > bounds.max_x || bounds.min_y > bounds.max_y ||
        !finite(player.x) || !finite(player.y) || !finite(player.z) ||
        !finite(offset.x) || !finite(offset.y) || !finite(offset.z))
        return fail(error, "Map camera update: invalid actual source bounds/player/camera offset");
    float x = player.x + offset.x;
    float y = player.y + offset.y;
    if (x < bounds.min_x) x = bounds.min_x;
    else if (x > bounds.max_x) x = bounds.max_x;
    if (y < bounds.min_y) y = bounds.min_y;
    else if (y > bounds.max_y) y = bounds.max_y;
    Point3V1 next{x - player.x, y - player.y, offset.z};
    if (!finite(next.x) || !finite(next.y))
        return fail(error, "Map camera update: source camera offset overflow");
    offset = next;
    error.clear();
    return true;
}

bool source_map_screen_coord_v1(const std::array<float, 4>& rect,
                               const Point3V1& screen,
                               std::array<float, 2>& output, std::string& error) {
    const float left = rect[0], right = rect[1], top = rect[2], bottom = rect[3];
    if (!finite(left) || !finite(top) || !finite(right) || !finite(bottom) ||
        !finite(screen.x) || !finite(screen.y) || right < left || bottom < top)
        return fail(error, "Map icon projection: invalid original RenderMap rect or CameraBase screen coordinate");
    const float width = right - left;
    const float height = bottom - top;
    const std::array<float, 2> next{
        (width * screen.x) * 0.5f + left + width * 0.5f,
        (-screen.y * height) * 0.5f + top + height * 0.5f};
    if (!finite(next[0]) || !finite(next[1]))
        return fail(error, "Map icon projection: source screen coordinate overflow");
    output = next;
    error.clear();
    return true;
}

bool source_map_project_marker_v1(
    const std::function<bool(const Point3V1&, Point3V1&, std::string&)>& get_screen,
    const Point3V1& marker, const std::array<float, 4>& rect,
    std::array<float, 2>& output, std::string& error) {
    if (!get_screen) return fail(error, "Map icon projection: required actual CameraBase::GetScreenCoord");
    Point3V1 screen{};
    if (!get_screen(marker, screen, error)) return false;
    return source_map_screen_coord_v1(rect, screen, output, error);
}

bool source_camera_base_screen_coord_v1(
    const SourceCameraMatricesV1& matrices, const Point3V1& p,
    Point3V1& output, std::string& error) {
    if (!matrices) return fail(error, "CameraBase screen projection: required same SceneManager matrix provider");
    if (!finite(p.x) || !finite(p.y) || !finite(p.z))
        return fail(error, "CameraBase screen projection: invalid source world position");

    SourceCameraMatrixV1 view{}, projection{};
    if (!matrices(0, view, error)) return false;
    if (!matrices(2, projection, error)) return false;
    for (const float value : view)
        if (!finite(value)) return fail(error, "CameraBase screen projection: nonfinite mode-0 view matrix");
    for (const float value : projection)
        if (!finite(value)) return fail(error, "CameraBase screen projection: nonfinite mode-2 projection matrix");

    // glitch::CMatrix4Base::setbyproduct_nocheck, preserving its source
    // column-major storage and operation order: projection * view.
    SourceCameraMatrixV1 product{};
    for (unsigned column = 0; column != 4; ++column) {
        for (unsigned row = 0; row != 4; ++row) {
            const unsigned i = column * 4 + row;
            const float t0 = projection[row] * view[column * 4];
            const float t1 = projection[4 + row] * view[column * 4 + 1];
            const float t2 = projection[8 + row] * view[column * 4 + 2];
            const float t3 = projection[12 + row] * view[column * 4 + 3];
            product[i] = ((t0 + t1) + t2) + t3;
        }
    }

    // Source multiplyWith1x4Matrix uses [x,y,z,1], then divides x and y by
    // the actual homogeneous W. Do not substitute eye/target projection.
    const float x0 = p.x * product[0];
    const float x1 = p.y * product[4];
    const float x2 = p.z * product[8];
    const float x3 = product[12];
    const float y0 = p.x * product[1];
    const float y1 = p.y * product[5];
    const float y2 = p.z * product[9];
    const float y3 = product[13];
    const float w0 = p.x * product[3];
    const float w1 = p.y * product[7];
    const float w2 = p.z * product[11];
    const float w3 = product[15];
    const float x = ((x0 + x1) + x2) + x3;
    const float y = ((y0 + y1) + y2) + y3;
    const float w = ((w0 + w1) + w2) + w3;
    if (!finite(x) || !finite(y) || !finite(w) || w == 0.0f)
        return fail(error, "CameraBase screen projection: invalid homogeneous source result");
    const Point3V1 next{x / w, y / w, 0.0f};
    if (!finite(next.x) || !finite(next.y))
        return fail(error, "CameraBase screen projection: nonfinite screen result");
    output = next;
    error.clear();
    return true;
}

bool source_map_camera_frame_current_v1(const SourceMapCameraFrameV1& source,
                                        std::string& error) {
    if (!source.frame_lease || !source.expected_scene_manager || !source.expected_camera ||
        !source.read_active)
        return fail(error, "Map camera projection: required actual frame lease, generation and camera/SceneManager identities");
    std::uintptr_t scene_manager{}, camera{};
    if (!source.read_active(source.frame_lease, source.generation,
                            scene_manager, camera, error)) return false;
    if (scene_manager != source.expected_scene_manager ||
        camera != source.expected_camera)
        return fail(error, "Map camera projection: active SceneManager/camera differs from retained source frame");
    error.clear();
    return true;
}

} // namespace dh::foundation::map_ui
