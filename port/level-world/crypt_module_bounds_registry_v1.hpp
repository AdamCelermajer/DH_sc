#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::crypt_module_bounds_registry_v1 {

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    import_failure,
    catalogue_failure,
    scene_failure,
    module_limit,
    identity_missing,
    catalogue_mismatch,
    unsupported_root,
    empty_root,
    non_geometry_instance,
    bounds_failure,
    allocation_failure
};

constexpr std::uint32_t max_modules = 256;

struct Bounds3 {
    float minimum[3];
    float maximum[3];
};

struct Entry {
    std::uint32_t module_index;
    std::uint32_t source_record;
    std::string module_name;
    std::string root_id;
    float origin[3];
    Bounds3 bounds;
    std::uint32_t scene_nodes;
    std::uint32_t geometry_instances;
    std::uint32_t draw_buffers;
    std::uint32_t ignored_non_geometry_instances;
    // Live Module+0x3fc projection. Save/deserialize wiring is separate work.
    std::uint8_t visited = 0;
};

// Owns copied module identities and bounds for one loaded source catalogue.
// catalogue_path is the source cache identity; the caller supplies the BRES
// bytes mapped to that path when building the registry.
struct Owner {
    std::string catalogue_path;
    std::vector<Entry> modules;
};

struct Result {
    std::uint32_t module_count;
    std::uint32_t failed_module_index;
    std::uint64_t scene_nodes;
    std::uint64_t geometry_instances;
    std::uint64_t draw_buffers;
};

// Import MLX bytes, open the supplied catalogue BRES, and retain only copied
// source identity, owner origins, finite bounds and summary counts. No live
// RoomZone objects, GameObject projections, or membership are created here.
// On failure Owner is unchanged; Result reports the failure module when known.
Status build_from_assets(const char* level_name, const char* source_path,
                         const std::uint8_t* mlx_bytes, std::size_t mlx_size,
                         const std::uint8_t* bres_bytes, std::size_t bres_size,
                         Owner*, Result*) noexcept;

const char* status_name(Status) noexcept;

} // namespace dh2::crypt_module_bounds_registry_v1
