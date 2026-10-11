#include "map_camera_pose_v1.hpp"

#include "../../../level-world/gameplay_camera_scene_v3.hpp"

#include <cmath>

namespace dh::foundation::map_visit {

bool load_map_camera_pose_v1(const std::vector<std::uint8_t>& bytes, MapCameraPoseV1& out, std::string& error) {
    out = MapCameraPoseV1{};
    dh2::camera::GameplayCameraSceneV3 scene;
    if (!scene.load(bytes, error)) return false;
    std::uint32_t camera = 0;
    if (!scene.select(map_camera_node_v1, camera, error)) return false;
    float root[3]{}, eye[3]{}, target[3]{};
    if (!scene.root_position(root, error)) return false;
    if (!scene.eye_and_target(camera, eye, target, error)) return false;
    MapCameraPoseV1 next;
    for (int i = 0; i < 3; ++i) {
        next.eye_offset[i] = eye[i] - root[i];
        next.target_offset[i] = target[i] - root[i];
        if (!std::isfinite(next.eye_offset[i]) || !std::isfinite(next.target_offset[i])) {
            error = "Authored Map camera pose is nonfinite";
            return false;
        }
    }
    next.loaded = true;
    out = next;
    error.clear();
    return true;
}

} // namespace dh::foundation::map_visit
