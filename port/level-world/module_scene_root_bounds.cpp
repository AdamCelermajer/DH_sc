#include "module_scene_root_bounds.hpp"

#include "floor_source.hpp"
#include "selector.hpp"
#include "../asset-payloads/payloads.hpp"
#include "../engine-math/math.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

extern "C" void dh2_node_matrix(float* out16, const float* translation3,
                                const float* quaternion4, const float* scale3);

namespace dh2::module_scene_root_bounds { namespace {

float literal(std::uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

// Column-major affine product a*b (translation in m[12..14]).
dh2::math::Matrix4f multiply(const dh2::math::Matrix4f& a, const dh2::math::Matrix4f& b) {
    dh2::math::Matrix4f out{};
    for (unsigned c = 0; c < 4; ++c)
        for (unsigned r = 0; r < 4; ++r) {
            float sum = 0;
            for (unsigned k = 0; k < 4; ++k) sum += a.m[k * 4 + r] * b.m[c * 4 + k];
            out.m[c * 4 + r] = sum;
        }
    out.identity_hint = 0;
    return out;
}

struct Walk {
    const dh2::resources::BresView* catalogue;
    const dh2::scene_payload::Scene* scene;
    const dh2::world::ModuleBinding* binding;
    const std::uint32_t* records;
    std::uint32_t record_count;
    float owner[3];
    // Rotated or scaled Module: descendants are placed by root * to_root * serialized, where
    // to_root undoes the catalogue root (inverse of T(origin) * R0), so the module root alone
    // carries the placement transform.
    bool rotated = false;
    dh2::math::Matrix4f root{};
    dh2::math::Matrix4f to_root{};
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
    if (walk.rotated) {
        placed = multiply(walk.root, multiply(walk.to_root, *serialized_world));
        for (float value : placed.m)
            if (!std::isfinite(value)) {
                walk.status = Status::transform_failure;
                return false;
            }
    } else {
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
    // Original Module root: position, quaternion::set(ry,-rx,-rz) of the degrees converted by
    // 0x3c8efa35, and scale (|s|<0x38d1b717 resets to 1); the root TRS replaces the catalogue one.
    const auto& local = module->record.local;
    for (unsigned axis = 0; axis != 3; ++axis)
        if (local.rotation_degrees[axis] != 0.0f || local.scale[axis] != 1.0f) walk.rotated = true;
    // Unrotated Modules place descendants by translation only, which needs an identity catalogue root.
    if (!walk.rotated && binding.catalogue_rotated) return Status::unsupported_transform;
    if (walk.rotated) {
        float radians[3];
        float scale[3];
        for (unsigned axis = 0; axis != 3; ++axis) {
            radians[axis] = local.rotation_degrees[axis] * literal(0x3c8efa35);
            scale[axis] = std::fabs(local.scale[axis]) < literal(0x38d1b717) ? 1.0f : local.scale[axis];
        }
        dh2::math::Quaternion q{};
        dh2_quat_from_euler(&q, radians[1], -radians[0], -radians[2]);
        if (q.x == 0.0f && q.y == 0.0f && q.z == 0.0f && q.w == 1.0f) q = {0, 0, 0, 1};
        const float quaternion[4]{q.x, q.y, q.z, q.w};
        dh2_node_matrix(walk.root.m, owner_world_position, quaternion, scale);
        walk.root.identity_hint = 0;
    }
    // to_root = inverse(T(origin) * R0) = R0^T * T(-origin); R0 is a unit quaternion.
    {
        const float zero[3]{0, 0, 0}, unit[3]{1, 1, 1};
        dh2::math::Matrix4f catalogue{};
        dh2_node_matrix(catalogue.m, zero, binding.catalogue_rotation, unit);
        dh2::math::Matrix4f transpose{};
        for (unsigned c = 0; c != 3; ++c)
            for (unsigned r = 0; r != 3; ++r) transpose.m[c * 4 + r] = catalogue.m[r * 4 + c];
        transpose.m[15] = 1;
        transpose.identity_hint = 0;
        dh2::math::Matrix4f back{};
        for (unsigned i = 0; i < 16; ++i) back.m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
        for (unsigned axis = 0; axis != 3; ++axis) back.m[12 + axis] = -binding.catalogue_origin[axis];
        back.identity_hint = 0;
        walk.to_root = multiply(transpose, back);
    }
    const auto walked = dh2_scene_walk_visual(&visual, visit, &walk, 65536);
    if (walked != dh2::scene_payload::Error::ok)
        return walk.status == Status::complete ? Status::scene_failure : walk.status;
    if (!walk.has_bounds) return Status::empty_scene;
    *output = walk.result;
    return Status::complete;
}

} // namespace dh2::module_scene_root_bounds
