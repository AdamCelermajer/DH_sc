#include "swamp_actor_floor_bridge.hpp"

#include "../../floor-types/floor_types.hpp"
#include "../../level-world/floor_source.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace dh2::irrlicht_game {
namespace {

bool bounded_text(const char* text, std::size_t capacity, std::string& out) {
    if (!text) return false;
    const auto* end = static_cast<const char*>(std::memchr(text, 0, capacity));
    if (!end) return false;
    out.assign(text, static_cast<std::size_t>(end - text));
    return true;
}

void identity_matrix(selector::Matrix& matrix) {
    matrix = {};
    matrix.values[0] = matrix.values[5] = matrix.values[10] =
        matrix.values[15] = 1.0f;
    matrix.identity = 1;
}

void include_point(octree::Box& bounds, const float* point, bool& initialized) {
    if (!initialized) {
        std::copy(point, point + 3, bounds.minimum);
        std::copy(point, point + 3, bounds.maximum);
        initialized = true;
        return;
    }
    for (unsigned axis = 0; axis < 3; ++axis) {
        bounds.minimum[axis] = std::min(bounds.minimum[axis], point[axis]);
        bounds.maximum[axis] = std::max(bounds.maximum[axis], point[axis]);
    }
}

bool source_flags_match(const SwampSourceSurfaceView& surface,
                        floor_source::Flags& flags, std::string& error) {
    const char* text = nullptr;
    std::size_t size = 0;
    if (surface.floor_type_tag_present) {
        text = surface.floor_type_tag;
        size = sizeof(surface.floor_type_tag);
    } else {
        text = surface.source_node_name;
        size = sizeof(surface.source_node_name);
    }
    if (!text) {
        error = "SWAMP floor source has no type text";
        return false;
    }
    const auto* end = static_cast<const char*>(std::memchr(text, 0, size));
    if (!end) {
        error = "SWAMP floor type text is unterminated";
        return false;
    }
    // Decode from native constructor defaults. Seeding with the claimed mask
    // would let a source label with missing bits pass this cross-check.
    flags = {0, 1};
    if (dh2_floor_source_flags(&flags, text,
            static_cast<std::uint32_t>(end - text)) != 0) {
        error = "SWAMP native floor-type decode rejected source text";
        return false;
    }
    if (flags.floor != surface.floor_type_flags) {
        error = "SWAMP source floor-type mask differs between adapters";
        return false;
    }
    return true;
}

} // namespace

struct SwampActorFloorBridge::Impl {
    floors::World world;
    std::vector<SwampActorFloorSource> source_floors;
    std::uint32_t path_mask = 0;
};

SwampActorFloorBridge::SwampActorFloorBridge() = default;
SwampActorFloorBridge::~SwampActorFloorBridge() = default;
SwampActorFloorBridge::SwampActorFloorBridge(SwampActorFloorBridge&&) noexcept = default;
SwampActorFloorBridge& SwampActorFloorBridge::operator=(SwampActorFloorBridge&&) noexcept = default;

bool SwampActorFloorBridge::build_module_zero(const SwampSourceNavigationView& source,
    std::uint32_t object_path_mask, std::string& error) {
    error.clear();
    if (impl_) {
        error = "SWAMP actor-floor bridge is already built";
        return false;
    }
    if (object_path_mask != swamp_module_zero_player_path_mask) {
        error = "SWAMP module-zero bridge requires source player path mask 0x2";
        return false;
    }
    if (!source.surfaces || !source.triangles || !source.surface_count
        || source.surface_count > source.surface_capacity
        || source.surface_count > 256U
        || source.triangle_count > source.triangle_capacity
        || source.triangle_count > 100000U) {
        error = "Invalid SWAMP source Navigation storage";
        return false;
    }

    try {
        auto candidate = std::make_unique<Impl>();
        candidate->path_mask = object_path_mask;
        unsigned source_module_floors = 0;
        unsigned total_triangles = 0;
        for (std::uint32_t i = 0; i < source.surface_count; ++i) {
            const auto& surface = source.surfaces[i];
            if (surface.module_index != 0) continue;
            ++source_module_floors;
            if (surface.source_surface_index != i) {
                error = "Module-zero SWAMP surface index differs from its source record";
                return false;
            }
            if (!surface.floor_type_flags_known) {
                error = "Module-zero SWAMP floor has unknown native flags";
                return false;
            }
            if (!surface.triangle_count || surface.first_triangle > source.triangle_count
                || surface.triangle_count > source.triangle_count - surface.first_triangle
                || surface.triangle_count > 100000U - total_triangles) {
                error = "Module-zero SWAMP floor has invalid triangle range";
                return false;
            }
            SwampActorFloorSource provenance{};
            provenance.actor_floor_index = static_cast<std::uint32_t>(candidate->world.records.size());
            provenance.source_surface_index = surface.source_surface_index;
            provenance.module_index = surface.module_index;
            provenance.module_source_record = surface.module_source_record;
            provenance.node_record = surface.node_record;
            provenance.geometry_index = surface.geometry_index;
            provenance.visible = surface.visible;
            provenance.source_triangle_count = surface.triangle_count;
            provenance.floor_type_flags = surface.floor_type_flags;
            provenance.floor_type_flags_known = surface.floor_type_flags_known;
            provenance.floor_type_tag_present = surface.floor_type_tag_present;
            if (!bounded_text(surface.module_name, sizeof(surface.module_name), provenance.module_name)
                || !bounded_text(surface.source_node_id, sizeof(surface.source_node_id), provenance.source_node_id)
                || !bounded_text(surface.source_node_name, sizeof(surface.source_node_name), provenance.source_node_name)
                || !bounded_text(surface.source_geometry_id, sizeof(surface.source_geometry_id), provenance.source_geometry_id)
                || !bounded_text(surface.source_geometry_name, sizeof(surface.source_geometry_name), provenance.source_geometry_name)
                || !bounded_text(surface.floor_type_tag, sizeof(surface.floor_type_tag), provenance.floor_type_tag)) {
                error = "Module-zero SWAMP floor source identity is unterminated";
                return false;
            }

            floor_source::Flags flags{};
            if (!source_flags_match(surface, flags, error)) return false;

            auto record = std::make_unique<floors::Record>();
            record->name = provenance.source_node_id;
            record->room = 0; // Only one selected catalogue module is present.
            record->geometry = provenance.geometry_index;
            record->flags = flags;
            record->triangles.reserve(surface.triangle_count);
            provenance.source_triangles.reserve(surface.triangle_count);
            bool bounds_initialized = false;
            for (std::uint32_t j = 0; j < surface.triangle_count; ++j) {
                const auto& input = source.triangles[surface.first_triangle + j];
                if (input.surface_index != provenance.source_surface_index) {
                    error = "SWAMP source triangle does not belong to its recorded surface";
                    return false;
                }
                collision::Triangle triangle{};
                // PFFloor's source mesh adapter emits indices in C/B/A order
                // before building collision selectors and the PF graph. The
                // imported Source Navigation view retains BRES A/B/C order,
                // so convert winding here while keeping original triangle IDs.
                std::memcpy(triangle.points[0], input.c, sizeof(input.c));
                std::memcpy(triangle.points[1], input.b, sizeof(input.b));
                std::memcpy(triangle.points[2], input.a, sizeof(input.a));
                for (const auto& point : triangle.points) {
                    for (const float coordinate : point) {
                        if (!std::isfinite(coordinate)
                            || std::fabs(coordinate) > 10000000.0f) {
                            error = "SWAMP source triangle coordinate is invalid";
                            return false;
                        }
                    }
                    include_point(record->world, point, bounds_initialized);
                }
                record->triangles.push_back(triangle);
                provenance.source_triangles.push_back({provenance.source_surface_index,
                    input.primitive_index,
                    input.source_triangle_index});
            }
            if (!bounds_initialized) {
                error = "Module-zero SWAMP floor has no finite source bounds";
                return false;
            }
            record->local = record->world;
            if (dh2_floor_source_bounds(&record->bounds, &record->world) != 0) {
                error = "SWAMP source floor bounds could not be expanded";
                return false;
            }
            identity_matrix(record->clone);

            const auto count = static_cast<std::uint32_t>(record->triangles.size());
            const auto tree_capacity = static_cast<std::size_t>(count) * 8U + 1U;
            if (tree_capacity > std::numeric_limits<std::uint32_t>::max()) {
                error = "SWAMP actor floor octree capacity overflow";
                return false;
            }
            record->octants.resize(tree_capacity);
            record->indices.resize(tree_capacity);
            record->scratch.resize(count);
            record->selected.resize(count);
            record->selected_ids.resize(count);
            record->tree = {record->octants.data(), record->indices.data(),
                record->scratch.data(), record->triangles.data(), count, 0, 0,
                static_cast<std::uint32_t>(tree_capacity),
                static_cast<std::uint32_t>(tree_capacity), count, 15, 0, 0};
            if (dh2_octree_build(&record->tree, record->triangles.data(), count, 15) != 0) {
                error = "SWAMP source floor octree build failed";
                return false;
            }
            record->workspace = {record->selected_ids.data(), record->selected.data(), count, 0};
            total_triangles += count;
            candidate->source_floors.push_back(std::move(provenance));
            candidate->world.records.push_back(std::move(record));
        }

        if (!source_module_floors || candidate->world.records.empty()) {
            error = "SWAMP source Navigation has no module-zero floor surfaces";
            return false;
        }
        if (!floors::build_graph(candidate->world, error)
            || !floors::post_load(candidate->world, error)) return false;

        // Graph construction reuses the same per-surface source flags as the
        // native floor loader. Keep the callback's owner at a stable address.
        if (candidate->world.graph.user != &candidate->world) {
            error = "SWAMP actor graph lost its floor-query owner";
            return false;
        }
        impl_ = std::move(candidate);
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    } catch (...) {
        error = "SWAMP actor-floor bridge allocation failed";
        return false;
    }
}

const navigation::CollisionWorld* SwampActorFloorBridge::collision_world() const {
    return impl_ ? &impl_->world.collision_world : nullptr;
}

const navigation::Graph* SwampActorFloorBridge::graph() const {
    return impl_ ? &impl_->world.graph : nullptr;
}

const std::vector<SwampActorFloorSource>& SwampActorFloorBridge::source_floors() const {
    static const std::vector<SwampActorFloorSource> empty;
    return impl_ ? impl_->source_floors : empty;
}

std::uint32_t SwampActorFloorBridge::object_path_mask() const {
    return impl_ ? impl_->path_mask : 0;
}

} // namespace dh2::irrlicht_game
