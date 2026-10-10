#include "runtime_equipment_preview_v1.hpp"
#include "../../../../engine-math/math.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdint>

namespace dh::foundation::equipment_menu {
namespace {
bool finite_matrix(const Mat4& matrix) noexcept {
    for (const float value : matrix)
        if (!std::isfinite(value)) return false;
    return true;
}
}

bool equipment_preview_rebase_v1(const dh2::scene::Node& root, Mat4& output,
        std::string& error) {
    const dh2::math::Quaternion original{
        root.quaternion[0], root.quaternion[1], root.quaternion[2], root.quaternion[3]};
    dh2::math::Vector3f degrees{};
    dh2_quat_to_euler_degrees(&original, &degrees);
    constexpr std::uint32_t degree_bits = 0x3c8efa35u;
    float degrees_to_radians{};
    std::memcpy(&degrees_to_radians, &degree_bits, sizeof(degrees_to_radians));
    dh2::math::Quaternion preview{};
    dh2_quat_from_euler(&preview, degrees.x * degrees_to_radians,
                        degrees.y * degrees_to_radians, -0.5f);
    dh2::math::Matrix4f original_matrix{}, preview_matrix{};
    dh2_quat_matrix_transposed(&original, &original_matrix);
    dh2_quat_matrix_transposed(&preview, &preview_matrix);
    Mat4 inverse{};
    inverse[15] = 1.0f;
    for (unsigned i = 0; i < 3; ++i) {
        if (!std::isfinite(root.scale[i]) || root.scale[i] == 0.0f) {
            error = "Equipment preview source root scale is singular";
            return false;
        }
        if (!std::isfinite(root.translation[i])) {
            error = "Equipment preview source root translation is nonfinite";
            return false;
        }
        for (unsigned j = 0; j < 3; ++j) {
            inverse[j * 4 + i] = original_matrix.m[i * 4 + j] / root.scale[i];
            preview_matrix.m[j * 4 + i] *= root.scale[i];
        }
        inverse[12 + i] = -(inverse[i] * root.translation[0] +
                            inverse[4 + i] * root.translation[1] +
                            inverse[8 + i] * root.translation[2]);
    }
    Mat4 preview_world{};
    std::copy(preview_matrix.m, preview_matrix.m + 16, preview_world.begin());
    const auto rebase = dh2::scene::multiply(preview_world, inverse);
    if (!finite_matrix(rebase)) {
        error = "Equipment preview source rebase became nonfinite";
        return false;
    }
    output = rebase;
    error.clear();
    return true;
}

bool compose_runtime_equipment_preview_frame_v1(const EquipmentPreviewSourceV1& source,
        RuntimeEquipmentPreviewFrameV1& output, std::string& error) {
    if (source.actor_id == invalid_actor_id || source.class_id.empty() || !source.visual ||
        !source.same_scene || !source.source_views || source.source_views->empty() ||
        !source.attachments) {
        error = "Equipment preview requires the current actor, class, visual, same-Scene body views and attachments";
        return false;
    }
    if (source.visual->retained_scene_borrow() != source.same_scene) {
        error = "Equipment preview body geometry belongs to a different retained Scene";
        return false;
    }
    const char* root_name = source.visual->source_motion_root_name(false);
    if (!root_name || !*root_name) {
        error = "Equipment preview source root is unavailable on the same retained visual";
        return false;
    }
    const auto root = std::find_if(source.same_scene->graph.begin(), source.same_scene->graph.end(),
        [&](const dh2::scene::Node& node) { return node.name == root_name; });
    if (root == source.same_scene->graph.end() || !finite_matrix(root->world)) {
        error = "Equipment preview source root node is absent from the retained Scene graph";
        return false;
    }

    RuntimeEquipmentPreviewFrameV1 next;
    next.revision = source.revision;
    next.actor_id = source.actor_id;
    next.class_id = source.class_id;
    next.pane = {};
    next.camera = equipment_preview_camera_v1();
    next.visual = source.visual;
    next.same_scene = source.same_scene;
    next.source_views = source.source_views;
    next.attachments = source.attachments;
    next.same_visual_root = &*root;
    next.same_visual_root_world = root->world;
    if (!equipment_preview_rebase_v1(*root, next.source_inventory_rebase, error)) return false;
    output = std::move(next);
    error.clear();
    return true;
}

} // namespace dh::foundation::equipment_menu
