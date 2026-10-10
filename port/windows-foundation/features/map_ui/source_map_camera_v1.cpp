#include "source_map_camera_v1.hpp"
#include <algorithm>

namespace dh::foundation::map_ui {

bool source_map_project_runtime_frame_v1(
    const SourceMapCameraFrameV1& frame,
    const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>& camera,
    const Point3V1& position, const std::array<float, 4>& render_rect,
    std::array<float, 2>& output, std::string& error) {
    if (!camera || !camera->loaded()) {
        error = "Map camera projection: required same loaded GameplayCameraRuntimeV11";
        return false;
    }
    const auto identity = reinterpret_cast<std::uintptr_t>(camera.get());
    if (frame.expected_camera != identity) {
        error = "Map camera projection: frame camera identity does not name the retained runtime";
        return false;
    }
    if (!source_map_camera_frame_current_v1(frame, error)) return false;

    dh2::camera::CameraViewV11 view;
    if (!camera->view(view, error)) return false;
    // Recheck after acquiring both matrices so an active-camera/frame switch
    // during the source view borrow cannot publish mixed-frame coordinates.
    if (!source_map_camera_frame_current_v1(frame, error)) return false;
    const SourceCameraMatricesV1 matrices = [&view](std::uint32_t mode,
            SourceCameraMatrixV1& out, std::string& e) {
        if (mode == 0) {
            std::copy_n(view.view.values, out.size(), out.begin());
            e.clear();
            return true;
        }
        if (mode == 2) {
            std::copy_n(view.projection.values, out.size(), out.begin());
            e.clear();
            return true;
        }
        e = "Map camera projection: unsupported SceneManager matrix selector";
        return false;
    };
    Point3V1 screen;
    if (!source_camera_base_screen_coord_v1(matrices, position, screen, error))
        return false;
    return source_map_screen_coord_v1(render_rect, screen, output, error);
}

bool source_map_camera_configure_v1(
    const std::shared_ptr<dh2::camera::GameplayCameraRuntimeV11>& camera,
    std::string& error) {
    if (!camera || !camera->loaded() || !camera->level()) {
        error = "Map camera setup: required same loaded CameraLevel runtime";
        return false;
    }
    const MapCameraSourceConfigV1 source;

    // 0045351c: fields 133, 140, 136; target damping off.
    if (!camera->source_map_fields_v1(source.camera_byte_133,
            source.camera_word_136, source.camera_float_140, error) ||
        !camera->level()->target().enable_damping(source.damping_enabled, error))
        return false;

    // 0040e9a8 CameraBase::SetData: +316, +312, +304, +308, then +276
    // with (0,0,1). The following direct CreateMapCamera +276 overrides it.
    if (!camera->source_set_data_v1(source.set_data_0, source.set_data_1,
            source.set_data_2, source.set_data_3, false, error) ||
        !camera->source_set_camera_vector_v1(
            {source.post_set_data_camera_vector[0],
             source.post_set_data_camera_vector[1],
             source.post_set_data_camera_vector[2]}, error))
        return false;
    error.clear();
    return true;
}

} // namespace dh::foundation::map_ui
