#include "runtime_equipment_preview_v1.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
using namespace dh::foundation::equipment_menu;

namespace {
void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
bool near(float a, float b, float epsilon = 0.0001f) {
    return std::abs(a - b) <= epsilon;
}
}

int main() try {
    const EquipmentPreviewPaneV1 pane;
    check(near(pane.left, 155.05f) && near(pane.top, 87.70f) &&
          near(pane.right, 321.10f) && near(pane.bottom, 282.75f),
          "Equipment pane differs from the composed original SWF bounds");

    const Camera camera = equipment_preview_camera_v1();
    check(near(camera.eye.x, 0) && near(camera.eye.y, -800) && near(camera.eye.z, 200) &&
          near(camera.target.x, 0) && near(camera.target.y, 0) && near(camera.target.z, 200) &&
          near(camera.up.x, 0) && near(camera.up.y, 0) && near(camera.up.z, 1),
          "Equipment camera pose differs from original CreateAvatarCamera");
    check(near(camera.verticalFovDegrees, 32.00078f, 0.0001f) &&
          near(camera.nearPlane, 10) && near(camera.farPlane, 1000) &&
          near(camera.aspectRatio, (321.10f - 155.05f) / (282.75f - 87.70f)),
          "Equipment camera projection differs from source FOV, planes, or avatarpane aspect");

    std::cout << "equipment preview source camera PASS pane=155.05,87.70..321.10,282.75 "
                 "fov_bits=0x3f0efb1a fov_deg=" << camera.verticalFovDegrees
              << " aspect=" << camera.aspectRatio << "\n";
    return 0;
} catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
}
