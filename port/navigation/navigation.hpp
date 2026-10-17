#pragma once

#include "../asset-payloads/payloads.hpp"
#include "../floor-types/floor_types.hpp"
#include "../world-data/world_scene.hpp"
#include <cstdint>

// Geometry extracted from the source-selected PFWorld floor mesh nodes.
// The arrays are owned by Navigation; names are copied from the borrowed
// level/BRES inputs. Surface metadata preserves the native floor tag and its
// parsed type mask. Triangles refer to that metadata through surface_index;
// this port still does not claim that every geometric hit is walkable.
namespace dh2::navigation {
enum class Error : std::uint32_t {
    ok, argument, level, allocation, limit, scene, mesh, unsupported_mesh,
    unsupported_transform, malformed
};
struct Diagnostic {
    Error error;
    char message[160];
};

constexpr std::uint32_t max_modules = 16;
constexpr std::uint32_t max_surfaces = 256;
constexpr std::uint32_t max_triangles = 100000;
constexpr std::uint32_t max_source_string = 128;
constexpr std::uint32_t max_floor_type_tag = 256;

struct Surface {
    std::uint32_t module_index;
    std::uint32_t module_source_record;
    std::uint32_t node_record;
    std::uint32_t geometry_index;
    std::uint32_t visible;
    std::uint32_t first_triangle;
    std::uint32_t triangle_count;
    std::uint32_t vertex_count;
    std::uint32_t primitive_count;
    // The decoded UserProperties `floortypes` value when present. An absent
    // property leaves the tag empty; the source node name remains available
    // separately and is used by the native fallback mask rule.
    bool floor_type_tag_present;
    char floor_type_tag[max_floor_type_tag];
    std::uint32_t floor_type_flags;
    bool floor_type_flags_known;
    char module_name[max_source_string];
    char source_node_id[max_source_string];
    char source_node_name[max_source_string];
    char source_geometry_id[max_source_string];
    char source_geometry_name[max_source_string];
};

// Source records retain provenance; the live PF graph's Triangle is a separate
// 36-byte geometry type. Both readers can be linked in one native image.
struct SurfaceTriangle {
    float a[3], b[3], c[3];
    std::uint32_t surface_index;
    std::uint32_t primitive_index;
    std::uint32_t source_triangle_index;
};
static_assert(sizeof(SurfaceTriangle) == 48);

struct Navigation {
    Surface* surfaces;
    std::uint32_t surface_count, surface_capacity;
    SurfaceTriangle* triangles;
    std::uint32_t triangle_count, triangle_capacity;
};

struct FloorHit {
    std::uint32_t surface_index;
    std::uint32_t primitive_index;
    std::uint32_t source_triangle_index;
    float height;
    float vertical_distance;
    float barycentric[3];
    // Height is a geometric result. It is not a walkability verdict.
    bool floor_type_flags_known;
    std::uint32_t floor_type_flags;
    bool floor_type_tag_present;
    char floor_type_tag[max_floor_type_tag];
};

struct SegmentHit {
    std::uint32_t surface_index;
    std::uint32_t primitive_index;
    std::uint32_t source_triangle_index;
    float fraction;
    float position[3];
    bool floor_type_flags_known;
    std::uint32_t floor_type_flags;
    bool floor_type_tag_present;
    char floor_type_tag[max_floor_type_tag];
};
}

extern "C" {
// Initialize Navigation with {}. The only supported source level at this
// stage is SWAMP's nine selected module instances and its shared swamp.bdae
// catalogue. On failure output is unchanged.
dh2::navigation::Error dh2_nav_build_swamp(dh2::navigation::Navigation*,
    const dh2::world::SourceLevel*, const dh2::scene_payload::Scene*,
    dh2::navigation::Diagnostic*);
void dh2_nav_free(dh2::navigation::Navigation*);
dh2::navigation::Error dh2_nav_surface(const dh2::navigation::Navigation*,
    std::uint32_t index, dh2::navigation::Surface*);
// Copy one imported record. The live dh2_nav_triangle symbol builds graph nodes.
dh2::navigation::Error dh2_nav_get_triangle(const dh2::navigation::Navigation*,
    std::uint32_t index, dh2::navigation::SurfaceTriangle*);
// Finds the closest geometric floor height in the caller's vertical band.
// Query coordinates are world X/Y; Z is the reference height. `edge_tolerance`
// is a barycentric tolerance in [0, 0.25]. The query reports one stable first
// source hit when distances tie. It does not filter by actor capabilities or
// floor-type flags.
dh2::navigation::Error dh2_nav_query_height(
    const dh2::navigation::Navigation*, float x, float y, float reference_z,
    float max_vertical_distance, float edge_tolerance,
    dh2::navigation::FloorHit*, bool* found);
// Actor-capability query layered over the same geometric scan. Floors with
// native Void/Wall category bits are skipped, then the low floor requirements
// must be a subset of the explicit object_path_mask (zero requirements pass).
// Returns the nearest eligible height within the caller's vertical band and
// includes that surface's tag/flags. Unknown flags are skipped. This returns
// data only; it does not perform movement, collision, or path-graph updates.
dh2::navigation::Error dh2_nav_query_actor_floor(
    const dh2::navigation::Navigation*, float x, float y, float reference_z,
    float max_vertical_distance, float edge_tolerance,
    std::uint32_t object_path_mask, dh2::navigation::FloorHit*, bool* found);
// Sweeps a zero-radius point over the finite segment [start,end] against the
// imported 3D floor triangles. Surfaces are visited in source order; the first
// eligible surface with a hit wins, and its nearest triangle hit is returned.
// With include_all=false, unknown floor masks and native void/wall categories
// are skipped. This geometric query does not model an actor radius or response.
dh2::navigation::Error dh2_nav_query_segment(
    const dh2::navigation::Navigation*, const float start[3], const float end[3],
    bool include_all, dh2::navigation::SegmentHit*, bool* found);
}
