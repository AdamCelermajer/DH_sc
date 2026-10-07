#pragma once
#include "../scene-materials/scene.hpp"
#include <memory>
namespace dh2::fx {
// One retained source TRS interpreter owner over the SAME resource graph.
// Scalar component tracks carry full default XYZ, matching original setters.
class AuthoredFxTransformOwnerV5 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 AuthoredFxTransformOwnerV5(const resources::BresView&,scene::Scene&);
 ~AuthoredFxTransformOwnerV5();
 bool initialize(std::string&);
 bool sample(std::int32_t,std::string&);
};
}
