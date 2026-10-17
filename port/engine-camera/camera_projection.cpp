#include "camera_projection.hpp"

#include "projection_matrix.hpp"

#include <cmath>
#include <cstring>
#include <limits>

namespace dh2::engine_camera::camera_projection { namespace {

using Address = std::uintptr_t;
struct Range { Address begin, end; };

bool range(const void* pointer, std::size_t size, std::size_t alignment, Range& out) noexcept {
    const Address begin = reinterpret_cast<Address>(pointer);
    if (!pointer || !alignment || begin % alignment ||
        begin > std::numeric_limits<Address>::max() - size)
        return false;
    out = {begin, begin + size};
    return true;
}
bool overlaps(Range a, Range b) noexcept { return a.begin < b.end && b.begin < a.end; }

float as_float(std::uint32_t word) noexcept {
    float value;
    std::memcpy(&value, &word, sizeof(value));
    return value;
}
std::uint32_t as_word(float value) noexcept {
    std::uint32_t word;
    std::memcpy(&word, &value, sizeof(word));
    return word;
}

} // namespace

Status recalculate_projection_matrix(CameraNode* node, const Services* services) noexcept {
    Range n, s;
    if (!range(node, sizeof(*node), alignof(CameraNode), n) ||
        !range(services, sizeof(*services), alignof(Services), s) ||
        !services->is_orthogonal || !services->transform_state || overlaps(n, s))
        return Status::invalid_argument;
    if (services->context_extent) {
        Range context;
        if (!range(services->context, services->context_extent, 1, context) ||
            overlaps(context, n) || overlaps(context, s))
            return Status::invalid_argument;
    }

    camera_math::Matrix4 pending{};
    projection_matrix::Status built = projection_matrix::Status::invalid_argument;
    if (services->is_orthogonal(services->context, node)) {
        const float half_height = as_float(node->orthographic_half_height);
        const float aspect = as_float(node->aspect);
        const float height = half_height + half_height;
        const float width = height * aspect;
        built = projection_matrix::orthographic(
            as_word(width), as_word(height), node->near_plane, node->far_plane, &pending);
    } else if (node->far_at_infinity != 0) {
        built = projection_matrix::perspective_fov_infinite(
            node->fov, node->aspect, node->near_plane, &pending);
    } else {
        built = projection_matrix::perspective_fov(
            node->fov, node->aspect, node->near_plane, node->far_plane, &pending);
    }
    if (built != projection_matrix::Status::complete) return Status::invalid_argument;

    // Original function memcpy's the complete 16-word matrix plus the engine
    // identity hint, then calls setTransformState on the embedded frustum.
    std::memcpy(&node->projection_matrix, &pending, 0x41);
    services->transform_state(services->context,
        reinterpret_cast<std::uint8_t*>(node) + 0x168, 2);
    return Status::complete;
}

} // namespace dh2::engine_camera::camera_projection
