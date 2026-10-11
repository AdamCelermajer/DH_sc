#include "projection_matrix.hpp"

#include <cmath>
#include <cstring>
#include <initializer_list>

namespace dh2::engine_camera::projection_matrix { namespace {

float as_float(Word word) noexcept {
    float value;
    std::memcpy(&value, &word, sizeof(value));
    return value;
}

Word as_word(float value) noexcept {
    Word word;
    std::memcpy(&word, &value, sizeof(word));
    return word;
}

bool finite(float value) noexcept { return std::isfinite(value); }

void initialize(camera_math::Matrix4* output) noexcept {
    for (Word& element : output->elements) element = 0;
    output->definitely_identity = 0;
}

bool output_ok(const camera_math::Matrix4* output) noexcept {
    return output && reinterpret_cast<std::uintptr_t>(output) % alignof(camera_math::Matrix4) == 0;
}

} // namespace

Status perspective_fov(Word fov_word, Word aspect_word, Word near_word,
                       Word far_word, camera_math::Matrix4* output) noexcept {
    if (!output_ok(output)) return Status::invalid_argument;
    const float fov = as_float(fov_word);
    const float aspect = as_float(aspect_word);
    const float near_plane = as_float(near_word);
    const float far_plane = as_float(far_word);
    if (!finite(fov) || !finite(aspect) || !finite(near_plane) || !finite(far_plane) ||
        aspect == 0.0f || far_plane == near_plane)
        return Status::invalid_argument;

    // This follows the upstream f64 reciprocal(tan(fov * 0.5)) sequence;
    // the original ARM helper converts its f32 FOV to double before tan.
    const double h = 1.0 / std::tan(static_cast<double>(fov) * 0.5);
    if (!std::isfinite(h) || h == 0.0) return Status::invalid_argument;
    const float w = static_cast<float>(h / static_cast<double>(aspect));
    const float hf = static_cast<float>(h);
    const float depth = far_plane / (far_plane - near_plane);
    const float depth_offset = (-near_plane * far_plane) / (far_plane - near_plane);
    if (!finite(w) || !finite(hf) || !finite(depth) || !finite(depth_offset))
        return Status::invalid_argument;

    camera_math::Matrix4 pending{};
    initialize(&pending);
    pending.elements[0] = as_word(w);
    pending.elements[5] = as_word(hf);
    pending.elements[10] = as_word(depth);
    pending.elements[11] = as_word(1.0f);
    pending.elements[14] = as_word(depth_offset);
    std::memcpy(output, &pending, 0x41);
    return Status::complete;
}

Status perspective_fov_infinite(Word fov_word, Word aspect_word, Word near_word,
                                camera_math::Matrix4* output) noexcept {
    if (!output_ok(output)) return Status::invalid_argument;
    const float fov = as_float(fov_word);
    const float aspect = as_float(aspect_word);
    const float near_plane = as_float(near_word);
    if (!finite(fov) || !finite(aspect) || !finite(near_plane) || aspect == 0.0f)
        return Status::invalid_argument;

    const double h = 1.0 / std::tan(static_cast<double>(fov) * 0.5);
    if (!std::isfinite(h) || h == 0.0) return Status::invalid_argument;
    const float w = static_cast<float>(h / static_cast<double>(aspect));
    const float hf = static_cast<float>(h);
    if (!finite(w) || !finite(hf)) return Status::invalid_argument;

    camera_math::Matrix4 pending{};
    initialize(&pending);
    pending.elements[0] = as_word(w);
    pending.elements[5] = as_word(hf);
    pending.elements[10] = as_word(1.0f);
    pending.elements[11] = as_word(1.0f);
    // Irrlicht's epsilon=0 infinity variant and the original DH2 helper both
    // encode -near in this slot.
    pending.elements[14] = near_word ^ 0x80000000u;
    std::memcpy(output, &pending, 0x41);
    return Status::complete;
}

Status orthographic(Word width_word, Word height_word, Word near_word,
                    Word far_word, camera_math::Matrix4* output) noexcept {
    if (!output_ok(output)) return Status::invalid_argument;
    const float width = as_float(width_word);
    const float height = as_float(height_word);
    const float near_plane = as_float(near_word);
    const float far_plane = as_float(far_word);
    if (!finite(width) || !finite(height) || !finite(near_plane) || !finite(far_plane) ||
        width == 0.0f || height == 0.0f || far_plane == near_plane)
        return Status::invalid_argument;

    const float x_scale = 2.0f / width;
    const float y_scale = 2.0f / height;
    const float z_scale = 1.0f / (far_plane - near_plane);
    const float z_offset = near_plane / (near_plane - far_plane);
    if (!finite(x_scale) || !finite(y_scale) || !finite(z_scale) || !finite(z_offset))
        return Status::invalid_argument;

    camera_math::Matrix4 pending{};
    initialize(&pending);
    pending.elements[0] = as_word(x_scale);
    pending.elements[5] = as_word(y_scale);
    pending.elements[10] = as_word(z_scale);
    pending.elements[14] = as_word(z_offset);
    pending.elements[15] = as_word(1.0f);
    std::memcpy(output, &pending, 0x41);
    return Status::complete;
}

Status fix_up_for_gles_default_orientation(camera_math::Matrix4* matrix,
                                           bool flip_y) noexcept {
    if (!output_ok(matrix)) return Status::invalid_argument;
    camera_math::Matrix4 pending{};
    std::memcpy(&pending, matrix, 0x41);
    float depth = as_float(pending.elements[10]);
    float offset = as_float(pending.elements[14]);
    if (!finite(depth) || !finite(offset)) return Status::invalid_argument;

    // The original branches on the projection's M[11]. Orthographic matrices
    // have zero there; perspective matrices have +1 for the positive-forward
    // engine convention. Keep the source's binary32 operation sequence.
    if (as_float(pending.elements[11]) == 0.0f) {
        depth = depth + depth;
        offset = (offset + offset) - 1.0f;
    } else {
        depth = (depth + depth) - 1.0f;
        offset = offset + offset;
    }
    if (!finite(depth) || !finite(offset)) return Status::invalid_argument;
    pending.elements[10] = as_word(depth);
    pending.elements[14] = as_word(offset);
    if (flip_y) {
        // CCommonGLDriverBase toggles the sign bit in column-major Y terms.
        for (const std::size_t index : {1u, 5u, 9u, 13u})
            pending.elements[index] ^= 0x80000000u;
    }
    pending.definitely_identity = 0;
    std::memcpy(matrix, &pending, 0x41);
    return Status::complete;
}

} // namespace dh2::engine_camera::projection_matrix
