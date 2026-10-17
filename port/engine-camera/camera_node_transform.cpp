#include "camera_node_transform.hpp"

#include <cstring>
#include <limits>

namespace dh2::engine_camera::camera_node_transform { namespace {
using Address = std::uintptr_t;
struct Range { Address begin, end; };
struct Active { const CameraNode* node; Active* previous; };
thread_local Active* active_calls = nullptr;

bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) {
    const Address begin = reinterpret_cast<Address>(pointer);
    if (!pointer || !alignment || begin % alignment ||
        begin > std::numeric_limits<Address>::max() - size) return false;
    out = {begin, begin + size};
    return true;
}
bool overlaps(Range a, Range b) { return a.begin < b.end && b.begin < a.end; }
bool is_active(const CameraNode* node) {
    for (auto* call = active_calls; call; call = call->previous)
        if (call->node == node) return true;
    return false;
}
struct ActiveGuard {
    Active frame;
    explicit ActiveGuard(const CameraNode* node) : frame{node, active_calls} { active_calls = &frame; }
    ~ActiveGuard() { active_calls = frame.previous; }
};

} // namespace

Status recalculate_matrices(CameraNode* node, const camera_math::MathServices* math,
                            const SceneServices* scene) {
    Range n, m, s;
    if (!range(node, sizeof(*node), alignof(CameraNode), n) ||
        !range(math, sizeof(*math), alignof(camera_math::MathServices), m) ||
        !range(scene, sizeof(*scene), alignof(SceneServices), s) ||
        !scene->transform_state || !scene->recalculate_view_area ||
        overlaps(n, m) || overlaps(n, s) || overlaps(m, s))
        return Status::invalid_argument;
    if (math->context_extent) {
        Range context;
        if (!range(math->context, math->context_extent, 1, context) ||
            overlaps(context, n) || overlaps(context, m) || overlaps(context, s))
            return Status::invalid_argument;
    }
    if (scene->context_extent) {
        Range context;
        if (!range(scene->context, scene->context_extent, 1, context) ||
            overlaps(context, n) || overlaps(context, m) || overlaps(context, s))
            return Status::invalid_argument;
    }
    if (is_active(node)) return Status::reentrant_call;
    ActiveGuard active(node);

    camera_math::Matrix4 pending{};
    if (camera_math::build_look_at(&node->absolute_position, &node->target,
                                   &node->up, &pending, math) != camera_math::Status::complete)
        return Status::invalid_argument;
    // The original performs an exact 0x41-byte memcpy, leaving host padding
    // after the one-byte matrix hint untouched.
    std::memcpy(&node->view_matrix, &pending, 0x41);

    scene->transform_state(scene->context,
        reinterpret_cast<std::uint8_t*>(node) + 0x168, 0);
    scene->recalculate_view_area(scene->context, node);
    return Status::complete;
}

} // namespace dh2::engine_camera::camera_node_transform
