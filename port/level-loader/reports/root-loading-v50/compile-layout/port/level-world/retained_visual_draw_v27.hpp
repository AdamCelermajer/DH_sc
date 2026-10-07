#pragma once
#include "retained_gameobject_visual_v1.hpp"
#include <memory>
namespace dh2::world {
struct RetainedVisualVertexV27 {float position[3]{},uv[2]{},color[4]{1,1,1,1};};
struct RetainedVisualPartV27 {
 std::uint32_t instance{},material{};
 std::vector<RetainedVisualVertexV27> vertices;
 std::vector<std::uint16_t> indices;
 std::array<float,16> matrix{};
 bool visible{},skinned{};
};
// GPU input adapter over the SAME registered visual. It does not load another
// Scene or advance an independent animation/physics/position clock.
class RetainedVisualDrawV27 {
 std::shared_ptr<RetainedGameObjectVisualV1> visual_;
 std::uintptr_t root_{};
 std::vector<RetainedVisualPartV27> parts_;
public:
 bool bind(std::shared_ptr<RetainedGameObjectVisualV1>,std::string&);
 bool refresh(std::string&);
 const std::shared_ptr<RetainedGameObjectVisualV1>& visual()const noexcept{return visual_;}
 const std::vector<RetainedVisualPartV27>& parts()const noexcept{return parts_;}
};
}
