#include "scrolling_combat_text_projection_v1.hpp"

#include <cmath>

namespace dh2::ui {

bool project_world_to_screen_pixels_v1(const float m[16], int width, int height,
                                       const float p[3], float out[2],
                                       bool* inside, std::string& error) noexcept {
    error.clear();
    if (!m || !p || !out || !inside || width <= 0 || height <= 0) {
        error = "World-to-screen projection arguments are invalid";
        return false;
    }
    for (int i = 0; i < 16; ++i) if (!std::isfinite(m[i])) {
        error = "View-projection matrix contains a non-finite value";
        return false;
    }
    for (int i = 0; i < 3; ++i) if (!std::isfinite(p[i])) {
        error = "World position contains a non-finite value";
        return false;
    }

    // GLES matrices and vectors are column-major/column-vector. The current
    // reconstructed camera uses positive-forward clip W, matching the source
    // camera's projected world-to-screen callback.
    const float clip_x = m[0] * p[0] + m[4] * p[1] + m[8]  * p[2] + m[12];
    const float clip_y = m[1] * p[0] + m[5] * p[1] + m[9]  * p[2] + m[13];
    const float clip_w = m[3] * p[0] + m[7] * p[1] + m[11] * p[2] + m[15];
    if (!std::isfinite(clip_x) || !std::isfinite(clip_y) ||
        !std::isfinite(clip_w) || clip_w <= 1.0e-6f) {
        error = "World position is behind or too close to the camera plane";
        return false;
    }
    const float nx = clip_x / clip_w;
    const float ny = clip_y / clip_w;
    if (!std::isfinite(nx) || !std::isfinite(ny)) {
        error = "Projected world position is non-finite";
        return false;
    }
    const float x = (nx + 1.0f) * 0.5f * static_cast<float>(width);
    const float y = (1.0f - ny) * 0.5f * static_cast<float>(height);
    if (!std::isfinite(x) || !std::isfinite(y)) {
        error = "Screen position is non-finite";
        return false;
    }
    out[0] = x;
    out[1] = y;
    *inside = nx >= -1.0f && nx <= 1.0f && ny >= -1.0f && ny <= 1.0f;
    return true;
}

} // namespace dh2::ui
