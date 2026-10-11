#include "room_zone_visit_v1.hpp"

#include "../menu_metadata/menu_metadata_v1.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>

namespace dh::foundation::map_visit {
namespace {
constexpr float pi = 3.14159265358979323846f;

float dot(const std::array<float, 3>& a, const std::array<float, 3>& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
std::array<float, 3> cross(const std::array<float, 3>& a, const std::array<float, 3>& b) {
    return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}
bool normalize(std::array<float, 3>& v) {
    const float length = std::sqrt(dot(v, v));
    if (!(length > 1e-12f) || !std::isfinite(length)) return false;
    for (auto& component : v) component /= length;
    return true;
}
bool finite_all(const std::array<float, 6>& values) {
    return std::all_of(values.begin(), values.end(), [](float v) { return std::isfinite(v); });
}
}

bool camera_basis_v1(const Camera& camera, float viewportAspect, CameraBasisV1& out, std::string& error) {
    const Vec3 eye = camera.eye;
    const std::array<float, 3> target{camera.target.x, camera.target.y, camera.target.z};
    const std::array<float, 3> up{camera.up.x, camera.up.y, camera.up.z};
    const float values[]{eye.x, eye.y, eye.z, target[0], target[1], target[2], up[0], up[1], up[2]};
    for (float v : values) {
        if (!std::isfinite(v)) { error = "Camera basis requires finite eye, target and up"; return false; }
    }
    // Same clamps as Renderer::applyCamera.
    const float fov = std::isfinite(camera.verticalFovDegrees)
        ? std::clamp(camera.verticalFovDegrees, 1.0f, 175.0f) : 60.0f;
    const float nearPlane = std::isfinite(camera.nearPlane) ? std::max(0.001f, camera.nearPlane) : 0.1f;
    const float farPlane = std::isfinite(camera.farPlane) ? std::max(nearPlane + 1.0f, camera.farPlane) : 5000.0f;
    const float aspect = (std::isfinite(camera.aspectRatio) && camera.aspectRatio > 0.0f) ? camera.aspectRatio : viewportAspect;
    if (!(aspect > 0.0f) || !std::isfinite(aspect)) { error = "Camera basis requires a positive aspect"; return false; }
    std::array<float, 3> forward{target[0] - eye.x, target[1] - eye.y, target[2] - eye.z};
    if (!normalize(forward)) { error = "Camera basis requires eye and target to differ"; return false; }
    std::array<float, 3> right = cross(forward, up);
    if (!normalize(right)) { error = "Camera basis requires up not parallel to view"; return false; }
    const auto screenUp = cross(right, forward);
    out.eye = {eye.x, eye.y, eye.z};
    out.forward = forward;
    out.right = right;
    out.up = screenUp;
    out.tanHalfY = std::tan(fov * pi / 360.0f);
    out.tanHalfX = out.tanHalfY * aspect;
    out.nearPlane = nearPlane;
    out.farPlane = farPlane;
    error.clear();
    return true;
}

std::array<FrustumPlaneV1, 6> frustum_planes_v1(const CameraBasisV1& b) {
    // Side planes pass through the eye: outside when n . (p - eye) > 0 with
    // n = side +/- tanHalf * forward. Near/far use the forward distance.
    const auto& f = b.forward;
    const auto& r = b.right;
    const auto& u = b.up;
    auto side = [&](const std::array<float, 3>& n) {
        return FrustumPlaneV1{n[0], n[1], n[2], -dot(n, b.eye)};
    };
    auto combine = [&](const std::array<float, 3>& dir, float scale) {
        return std::array<float, 3>{dir[0] + f[0] * scale, dir[1] + f[1] * scale, dir[2] + f[2] * scale};
    };
    std::array<FrustumPlaneV1, 6> planes{};
    planes[0] = side(combine({-r[0], -r[1], -r[2]}, -b.tanHalfX));  // left
    planes[1] = side(combine(r, -b.tanHalfX));                      // right
    planes[2] = side(combine({-u[0], -u[1], -u[2]}, -b.tanHalfY));  // bottom
    planes[3] = side(combine(u, -b.tanHalfY));                      // top
    // Near: outside when dot(f, p-eye) < near, i.e. -f.(p-eye) + near > 0.
    planes[4] = {-f[0], -f[1], -f[2], dot(b.eye, f) + b.nearPlane};
    // Far: outside when dot(f, p-eye) > far.
    planes[5] = {f[0], f[1], f[2], -dot(b.eye, f) - b.farPlane};
    return planes;
}

bool zone_in_frustum_v1(const std::array<float, 6>& bounds, const std::array<FrustumPlaneV1, 6>& planes) {
    for (const auto& plane : planes) {
        // Most-inside corner (RoomZone::source_update_v104 selects the same corner per sign).
        const float x = plane[0] >= 0.0f ? bounds[0] : bounds[3];
        const float y = plane[1] >= 0.0f ? bounds[1] : bounds[4];
        const float z = plane[2] >= 0.0f ? bounds[2] : bounds[5];
        const float distance = plane[0] * x + plane[1] * y + plane[2] * z + plane[3];
        if (distance > 0.0f) return false;
    }
    return true;
}

bool zone_has_inside_xy_v1(const std::array<float, 6>& bounds, float x, float y) {
    return bounds[0] <= x && x <= bounds[3] && bounds[1] <= y && y <= bounds[4];
}

bool RoomZoneVisitTrackerV1::configure(std::vector<LevelModuleZone> zones,
                                       const std::vector<std::uint32_t>& visitedIds, std::string& error) {
    std::set<std::uint32_t> ids;
    for (const auto& zone : zones) {
        if (!ids.insert(zone.id).second) {
            error = "Duplicate module zone id " + std::to_string(zone.id);
            return false;
        }
        if (!finite_all(zone.bounds)) {
            error = "Module zone " + zone.name + " has nonfinite bounds";
            return false;
        }
        for (int axis = 0; axis < 3; ++axis) {
            if (zone.bounds[axis] > zone.bounds[axis + 3]) {
                error = "Module zone " + zone.name + " has inverted bounds";
                return false;
            }
        }
    }
    const std::set<std::uint32_t> saved(visitedIds.begin(), visitedIds.end());
    zones_ = std::move(zones);
    visited_.assign(zones_.size(), 0);
    inView_.assign(zones_.size(), 0);
    for (std::size_t i = 0; i < zones_.size(); ++i) visited_[i] = saved.count(zones_[i].id) ? 1 : 0;
    error.clear();
    return true;
}

std::vector<std::uint32_t> RoomZoneVisitTrackerV1::update(const std::array<FrustumPlaneV1, 6>& planes,
                                                          const std::optional<std::array<float, 3>>& player) {
    std::vector<std::uint32_t> newlyVisited;
    for (std::size_t i = 0; i < zones_.size(); ++i) {
        const auto& zone = zones_[i];
        if (!zone_in_frustum_v1(zone.bounds, planes)) {
            inView_[i] = 0;
            continue;
        }
        inView_[i] = 1;
        if (visited_[i]) continue;
        if (player && zone_has_inside_xy_v1(zone.bounds, (*player)[0], (*player)[1])) {
            visited_[i] = 1;
            newlyVisited.push_back(zone.id);
        }
    }
    return newlyVisited;
}

bool RoomZoneVisitTrackerV1::visited(std::uint32_t id) const {
    for (std::size_t i = 0; i < zones_.size(); ++i) if (zones_[i].id == id) return visited_[i] != 0;
    return false;
}

bool RoomZoneVisitTrackerV1::in_view(std::uint32_t id) const {
    for (std::size_t i = 0; i < zones_.size(); ++i) if (zones_[i].id == id) return inView_[i] != 0;
    return false;
}

std::size_t RoomZoneVisitTrackerV1::visited_count() const {
    return static_cast<std::size_t>(std::count(visited_.begin(), visited_.end(), std::uint8_t{1}));
}

const LevelModuleZone* RoomZoneVisitTrackerV1::zone(std::uint32_t id) const {
    for (const auto& zone : zones_) if (zone.id == id) return &zone;
    return nullptr;
}

std::vector<std::uint32_t> visited_module_ids(const CharacterState& state, const std::string& levelUri) {
    std::vector<std::uint32_t> ids;
    for (const auto& entry : state.visited_modules)
        if (entry.visited && entry.level_uri == levelUri) ids.push_back(entry.module_id);
    return ids;
}

void record_visited_modules(CharacterState& state, const std::string& levelUri, const std::vector<std::uint32_t>& ids) {
    for (const auto id : ids) menu_metadata::set_visited_module(state, levelUri, id, true);
}

} // namespace dh::foundation::map_visit
