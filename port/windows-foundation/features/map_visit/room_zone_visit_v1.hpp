#pragma once
// RoomZone visitation for the Map page (Preview 16, stream map).
//
// Source rule (RoomZone::Update 0x396e9c; canonical mirror
// level-world/canonical_room_zone_v3.cpp source_update_v104): each frame a zone is
// tested against the six camera planes using its most-inside box corner. A zone
// that lies wholly outside any plane is inactive and is not visited this frame.
// An active, not yet visited zone is marked visited when the local player is inside
// its XY box (RoomZone::HasInside, inclusive XY). The visited flag is per module
// (Module::visited3fc), so one flag per LevelModuleZone id.
//
// Data comes from the CURRENT level only (LevelModuleZone from load_level_with_module_zones);
// nothing here is level-specific. Persistence uses CharacterState::visited_modules
// (save schema v4, keyed by level_uri + module id); absent entries are unvisited.
#include "../../assembled_level.hpp"
#include "../../camera.hpp"
#include "../../character_state.hpp"
#include "../../renderer.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace dh::foundation::map_visit {

// Plane (a,b,c,d): a point p is outside when a*x+b*y+c*z+d > 0.
using FrustumPlaneV1 = std::array<float, 4>;

// Camera basis used by both the frustum planes and the Map projection.
struct CameraBasisV1 {
    std::array<float, 3> eye{}, forward{}, right{}, up{};
    float tanHalfY = 0, tanHalfX = 0, nearPlane = 0, farPlane = 0;
};
// Vertical FOV and near/far follow Renderer::applyCamera. aspect must be positive
// (camera.aspectRatio when set, otherwise the viewport aspect).
bool camera_basis_v1(const Camera& camera, float viewportAspect, CameraBasisV1& out, std::string& error);

// Six planes ordered left, right, bottom, top, near, far. Inside is distance <= 0.
std::array<FrustumPlaneV1, 6> frustum_planes_v1(const CameraBasisV1& basis);

// RoomZone::Update's plane gate: true when the zone's most-inside corner is not
// outside any plane. bounds = minX,minY,minZ,maxX,maxY,maxZ.
bool zone_in_frustum_v1(const std::array<float, 6>& bounds, const std::array<FrustumPlaneV1, 6>& planes);

// RoomZone::HasInside: inclusive XY test.
bool zone_has_inside_xy_v1(const std::array<float, 6>& bounds, float x, float y);

class RoomZoneVisitTrackerV1 {
public:
    // Replaces the zone list. Zone ids must be unique. visitedIds (from the save) mark
    // zones already visited; ids that match no zone are ignored (the caller may have
    // loaded a different level). Returns false with an error on duplicate zone ids.
    bool configure(std::vector<LevelModuleZone> zones, const std::vector<std::uint32_t>& visitedIds, std::string& error);

    // One frame. Returns the ids that became visited this frame, in zone order.
    // The player position is optional: no player means no visit can occur.
    std::vector<std::uint32_t> update(const std::array<FrustumPlaneV1, 6>& planes,
                                      const std::optional<std::array<float, 3>>& player);

    bool visited(std::uint32_t id) const;
    bool in_view(std::uint32_t id) const;
    std::size_t visited_count() const;
    const std::vector<LevelModuleZone>& zones() const noexcept { return zones_; }
    // Zone index for an id, or nullptr.
    const LevelModuleZone* zone(std::uint32_t id) const;

private:
    std::vector<LevelModuleZone> zones_;
    std::vector<std::uint8_t> visited_;
    std::vector<std::uint8_t> inView_;
};

// Visited ids recorded for this level in the character (schema v4 visited_modules).
std::vector<std::uint32_t> visited_module_ids(const CharacterState&, const std::string& levelUri);

// Records ids as visited (visited=1) for this level. Idempotent; keeps the stored
// vector sorted and unique through menu_metadata::set_visited_module.
void record_visited_modules(CharacterState&, const std::string& levelUri, const std::vector<std::uint32_t>& ids);

} // namespace dh::foundation::map_visit
