#include "module_scene_root_bounds.hpp"

#include "floor_source.hpp"
#include "selector.hpp"
#include "../asset-payloads/payloads.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

namespace dh2::module_scene_root_bounds { namespace {

struct Walk {
    const dh2::resources::BresView* catalogue;
    const dh2::scene_payload::Scene* scene;
    const dh2::world::ModuleBinding* binding;
    const std::uint32_t* records;
    std::uint32_t record_count;
    float owner[3];
    Result result{};
    bool has_bounds = false;
    Status status = Status::complete;
};

bool is_member(const Walk& walk, std::uint32_t record) {
    return std::find(walk.records, walk.records + walk.record_count, record)
        != walk.records + walk.record_count;
}

bool finite_box(const dh2::octree::Box& box) {
    for (unsigned axis = 0; axis != 3; ++axis) {
        if (!std::isfinite(box.minimum[axis]) || !std::isfinite(box.maximum[axis]) ||
            box.minimum[axis] > box.maximum[axis]) return false;
    }
    return true;
}

bool visit(const dh2::scene_payload::Node* node,
           const dh2::math::Matrix4f* serialized_world,
           std::uint32_t, void* opaque) {
    auto& walk = *static_cast<Walk*>(opaque);
    if (!is_member(walk, node->record)) return true;
    ++walk.result.scene_nodes;

    dh2::math::Matrix4f placed{};
    auto binding = *walk.binding;
    for (unsigned axis = 0; axis != 3; ++axis) {
        binding.placement_delta[axis] = walk.owner[axis] - binding.catalogue_origin[axis];
        if (!std::isfinite(binding.placement_delta[axis])) {
            walk.status = Status::unsupported_transform;
            return false;
        }
    }
    dh2::world::Diagnostic diagnostic{};
    if (dh2_world_place_matrix(&placed, &binding, serialized_world, &diagnostic)
        != dh2::world::Error::ok) {
        walk.status = Status::transform_failure;
        return false;
    }

    for (std::uint32_t i = 0; i < node->instances; ++i) {
        dh2::scene_payload::Instance instance{};
        if (dh2_scene_instance(node, static_cast<std::int32_t>(i), &instance)
            != dh2::scene_payload::Error::ok) {
            walk.status = Status::scene_failure;
            return false;
        }
        if (instance.type != 3) {
            // These module scenes contain static geometry only. We record other
            // instance types as omitted; a caller requiring full CSceneNode
            // semantics must reject a nonzero count.
            ++walk.result.ignored_non_geometry_instances;
            continue;
        }
        const auto geometry = dh2_scene_geometry_index(walk.scene, &instance);
        if (geometry < 0) {
            walk.status = Status::geometry_failure;
            return false;
        }
        dh2::assets::Mesh mesh{};
        if (dh2_mesh_open(&mesh, walk.catalogue, geometry) != dh2::assets::Error::ok) {
            walk.status = Status::geometry_failure;
            return false;
        }
        dh2::octree::Box local{};
        std::copy(mesh.minimum, mesh.minimum + 3, local.minimum);
        std::copy(mesh.maximum, mesh.maximum + 3, local.maximum);
        if (!finite_box(local)) {
            walk.status = Status::geometry_failure;
            return false;
        }
        dh2::selector::Matrix source_matrix{};
        std::copy(placed.m, placed.m + 16, source_matrix.values);
        source_matrix.identity = 0;
        dh2::octree::Box world{};
        if (dh2_floor_transform_bounds(&world, &local, &source_matrix) != 0 ||
            !finite_box(world)) {
            walk.status = Status::transform_failure;
            return false;
        }
        if (!walk.has_bounds) {
            walk.result.bounds = world;
            walk.has_bounds = true;
        } else {
            for (unsigned axis = 0; axis != 3; ++axis) {
                walk.result.bounds.minimum[axis] = std::min(
                    walk.result.bounds.minimum[axis], world.minimum[axis]);
                walk.result.bounds.maximum[axis] = std::max(
                    walk.result.bounds.maximum[axis], world.maximum[axis]);
            }
        }
        ++walk.result.geometry_instances;
        walk.result.draw_buffers += mesh.primitives;
    }
    return true;
}

} // namespace

Status build(const dh2::world::Module* module,
             const dh2::scene_payload::Scene* scene,
             const dh2::resources::BresView* catalogue,
             const float owner_world_position[3], Result* output) {
    if (!module || !scene || !catalogue || !owner_world_position || !output ||
        !catalogue->bytes || scene->image.bytes != catalogue->bytes ||
        scene->image.size != catalogue->size) return Status::invalid_argument;
    for (float value : {owner_world_position[0], owner_world_position[1],
                        owner_world_position[2]})
        if (!std::isfinite(value)) return Status::unsupported_transform;

    dh2::world::ModuleBinding binding{};
    dh2::world::Diagnostic diagnostic{};
    if (dh2_world_bind_module(&binding, module, scene, &diagnostic)
        != dh2::world::Error::ok) {
        return diagnostic.error == dh2::world::Error::unsupported_transform
            ? Status::unsupported_transform : Status::scene_failure;
    }
    std::vector<std::uint32_t> records(65536);
    std::uint32_t record_count = 0;
    if (dh2_world_module_records(records.data(),
            static_cast<std::uint32_t>(records.size()), &record_count,
            &binding, scene, &diagnostic) != dh2::world::Error::ok || !record_count)
        return Status::scene_failure;

    dh2::scene_payload::Visual visual{};
    if (dh2_scene_visual(scene, static_cast<std::int32_t>(binding.visual_index), &visual)
        != dh2::scene_payload::Error::ok) return Status::scene_failure;

    Walk walk{};
    walk.catalogue = catalogue;
    walk.scene = scene;
    walk.binding = &binding;
    walk.records = records.data();
    walk.record_count = record_count;
    std::copy(owner_world_position, owner_world_position + 3, walk.owner);
    const auto walked = dh2_scene_walk_visual(&visual, visit, &walk, 65536);
    if (walked != dh2::scene_payload::Error::ok)
        return walk.status == Status::complete ? Status::scene_failure : walk.status;
    if (!walk.has_bounds) return Status::empty_scene;
    *output = walk.result;
    return Status::complete;
}

} // namespace dh2::module_scene_root_bounds
