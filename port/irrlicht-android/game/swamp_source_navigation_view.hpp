#pragma once

#include <cstdint>

namespace dh2::irrlicht_game {

// Neutral, fixed-width copy of the source nav import. It remains an explicit
// ownership/conversion boundary between serialized source navigation data and
// the actor-runtime's collision/floor graph, even though their C++ names are
// now disjoint (SurfaceTriangle/dh2_nav_get_triangle versus actor Triangle).
struct SwampSourceSurfaceView {
    std::uint32_t source_surface_index;
    std::uint32_t module_index;
    std::uint32_t module_source_record;
    std::uint32_t node_record;
    std::uint32_t geometry_index;
    std::uint32_t visible;
    std::uint32_t first_triangle;
    std::uint32_t triangle_count;
    std::uint32_t vertex_count;
    std::uint32_t primitive_count;
    std::uint32_t floor_type_flags;
    std::uint32_t floor_type_flags_known;
    std::uint32_t floor_type_tag_present;
    char module_name[128];
    char source_node_id[128];
    char source_node_name[128];
    char source_geometry_id[128];
    char source_geometry_name[128];
    char floor_type_tag[256];
};

struct SwampSourceTriangleView {
    float a[3];
    float b[3];
    float c[3];
    std::uint32_t surface_index;
    std::uint32_t primitive_index;
    std::uint32_t source_triangle_index;
};

struct SwampSourceNavigationView {
    const SwampSourceSurfaceView* surfaces;
    std::uint32_t surface_count;
    std::uint32_t surface_capacity;
    const SwampSourceTriangleView* triangles;
    std::uint32_t triangle_count;
    std::uint32_t triangle_capacity;
};

static_assert(sizeof(SwampSourceSurfaceView) == 948);
static_assert(sizeof(SwampSourceTriangleView) == 48);

} // namespace dh2::irrlicht_game
