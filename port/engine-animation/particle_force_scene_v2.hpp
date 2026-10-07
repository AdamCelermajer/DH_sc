#pragma once
#include "particle_cloud_models_v1.hpp"
#include "particle_deflector_v1.hpp"
#include "../scene-materials/scene.hpp"
namespace dh2::animation {
struct ParticleForceSceneV2 {
 std::string source_name,source_node_name;
 std::uint32_t type{},node_index{};
 const scene::Scene* scene{};
 ParticleGravityV1 gravity{};
 ParticleDeflectorParametersV1 deflector{};
 const std::array<float,16>* matrix()const noexcept;
};
bool decode_particle_force_scene_v2(const resources::BresView&,const scene::Scene&,
 std::vector<ParticleForceSceneV2>&,std::string&);
bool particle_force_bindings_v2(const resources::BresView&,std::uint32_t emitter_instance,
 const scene::Scene&,const std::vector<ParticleForceSceneV2>&,
 std::vector<std::uint32_t>& ordered_indices,std::string&);
}
