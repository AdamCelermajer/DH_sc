#pragma once

#include "source_map_kernel_v1.hpp"
#include "../../../level-world/gameplay_camera_runtime_v11.hpp"

namespace dh::foundation::map_ui {

// Uses view+projection returned by the SAME retained source camera runtime.
// A caller must lend the live frame lease and a validator that reads the real
// active SceneManager/CameraBase identities during that frame.
bool source_map_project_runtime_frame_v1(
    const SourceMapCameraFrameV1& frame,
    const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>& camera,
    const Point3V1& world_position,
    const std::array<float, 4>& render_rect,
    std::array<float, 2>& output, std::string& error);

// Replays the exact Map CreateMapCamera fields, damping call, CameraBase
// SetData dispatch and post-SetData vector override in source order, through
// the additive setters on the same retained camera runtime.
bool source_map_camera_configure_v1(
    const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>&,
    std::string& error);

} // namespace dh::foundation::map_ui
