#pragma once
#include "../../../../scene-materials/scene.hpp"
namespace dh::foundation::frontend {
// Original Show setter inputs and authored receiver identity metadata only.
// This does not create/register a CLight/LightPoint or bind material uniforms.
struct ClassSelectLightInputs {
    bool authored_light_present=false;
    std::string authored_node;
    std::uint32_t node_index{},source_light_type{};
    std::uint32_t light_set=0,slot=0;
    std::array<float,3> attenuation_setter{};
    std::array<float,4> position{},ambient{},diffuse{},specular{};
    std::array<float,3> normalized_attenuation{};
    std::array<float,4> scene_ambient{0,0,0,1};
};
bool read_class_select_light_inputs(const dh2::scene::Scene&,ClassSelectLightInputs&,std::string&);
// A real adapter must supply same scene first-light, spawned LightPoint,
// LightSetManager and CMaterial uniform owners. Metadata alone cannot succeed.
const char* required_class_select_light_owner();
}
