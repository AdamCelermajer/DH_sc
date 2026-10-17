#pragma once

#include "swamp_source_navigation_view.hpp"
#include "../../level-world/floors.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace dh2::irrlicht_game {

inline constexpr std::uint32_t swamp_module_zero_player_path_mask = 0x2U;

struct SwampActorTriangleSource {
    std::uint32_t source_surface_index = 0;
    std::uint32_t primitive_index = 0;
    std::uint32_t source_triangle_index = 0;
};

// Stable provenance retained beside each actor-runtime collision floor. The
// actor floor index is local to the one-room bridge; source_surface_index and
// the remaining IDs point back to the unmodified imported SWAMP Navigation.
struct SwampActorFloorSource {
    std::uint32_t actor_floor_index = 0;
    std::uint32_t source_surface_index = 0;
    std::uint32_t module_index = 0;
    std::uint32_t module_source_record = 0;
    std::uint32_t node_record = 0;
    std::uint32_t geometry_index = 0;
    std::uint32_t visible = 0;
    std::uint32_t source_triangle_count = 0;
    std::uint32_t floor_type_flags = 0;
    bool floor_type_flags_known = false;
    bool floor_type_tag_present = false;
    std::string module_name;
    std::string source_node_id;
    std::string source_node_name;
    std::string source_geometry_id;
    std::string source_geometry_name;
    std::string floor_type_tag;
    std::vector<SwampActorTriangleSource> source_triangles;
};

class SwampActorFloorBridge final {
    struct Impl;
    std::unique_ptr<Impl> impl_;

public:
    SwampActorFloorBridge();
    ~SwampActorFloorBridge();
    SwampActorFloorBridge(SwampActorFloorBridge&&) noexcept;
    SwampActorFloorBridge& operator=(SwampActorFloorBridge&&) noexcept;
    SwampActorFloorBridge(const SwampActorFloorBridge&) = delete;
    SwampActorFloorBridge& operator=(const SwampActorFloorBridge&) = delete;

    // Build an isolated one-room level-world collision/graph view of only
    // SWAMP module zero. The source Navigation remains borrowed and unchanged.
    // Floors with unknown source flags, malformed source triangle ownership,
    // or a mask other than the recovered module-zero player mask are rejected.
    bool build_module_zero(const SwampSourceNavigationView& source,
                          std::uint32_t object_path_mask,
                          std::string& error);

    const navigation::CollisionWorld* collision_world() const;
    const navigation::Graph* graph() const;
    const std::vector<SwampActorFloorSource>& source_floors() const;
    std::uint32_t object_path_mask() const;
};

} // namespace dh2::irrlicht_game
