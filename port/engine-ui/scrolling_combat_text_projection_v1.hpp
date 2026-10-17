#pragma once

#include <string>

namespace dh2::ui {

// Projects a world point using the renderer's column-major GLES view-projection
// matrix. Screen coordinates use the Android top-left pixel origin. The point
// may be outside the viewport (the source camera still returns projected
// coordinates); points at/behind the camera or non-finite inputs fail closed.
bool project_world_to_screen_pixels_v1(const float view_projection[16],
                                       int width, int height,
                                       const float world[3],
                                       float screen_pixels[2],
                                       bool* inside_viewport,
                                       std::string& error) noexcept;

} // namespace dh2::ui
