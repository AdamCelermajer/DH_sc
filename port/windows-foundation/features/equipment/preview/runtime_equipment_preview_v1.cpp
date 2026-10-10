#include "runtime_equipment_preview_v1.hpp"
#include <cstring>

namespace dh::foundation::equipment_menu {
namespace {
constexpr float pane_width = 321.10f - 155.05f;
constexpr float pane_height = 282.75f - 87.70f;
constexpr std::uint32_t source_fov_bits = 0x3f0efb1au;

}

Camera equipment_preview_camera_v1() noexcept {
    Camera camera;
    camera.eye = {0.0f, -800.0f, 200.0f};
    camera.target = {0.0f, 0.0f, 200.0f};
    camera.up = {0.0f, 0.0f, 1.0f};
    float radians{};
    static_assert(sizeof(radians) == sizeof(source_fov_bits));
    std::memcpy(&radians, &source_fov_bits, sizeof(radians));
    constexpr float radians_to_degrees = 57.29577951308232f;
    camera.verticalFovDegrees = radians * radians_to_degrees;
    camera.nearPlane = 10.0f;
    camera.farPlane = 1000.0f;
    camera.aspectRatio = pane_width / pane_height;
    return camera;
}

} // namespace dh::foundation::equipment_menu
