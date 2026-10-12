#pragma once
#include <array>
#include <string>
namespace dh2::loader {
struct VisualTransformV1 {
    std::array<float,3> position{}, rotation_degrees{}, scale{1,1,1};
    std::array<float,4> quaternion{0,0,0,1};
};
// Pure visual projection of GameObject::InitPost/VisualObject::Sync. Does not
// execute InitPost's condition, probability, object or physical side effects.
bool project_visual_transform_v1(VisualTransformV1&,std::string& error);
}
