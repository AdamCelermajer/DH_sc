#pragma once
#include "../scene-materials/scene.hpp"
#include <memory>
namespace dh2::animation {
struct ParticleGravitySceneV1 {
 std::string source_name,source_node_name;std::uint32_t node_index{};
 float strength{},falloff{};std::uint32_t point_mode{};
 // Borrowed same retained FX scene; matrix changes with its authored animation.
 const scene::Scene* scene{};
 const std::array<float,16>* matrix()const noexcept;
};
// Exact type0 SForce + instance tag12 named lookup domain. No missing
// instance or unimplemented force family is silently omitted.
bool decode_particle_gravity_scene_v1(const resources::BresView&,const scene::Scene&,
 std::vector<ParticleGravitySceneV1>&,std::string&);
// Original emitter instance+18 force-name vector -> scene node lookup. Preserves
// declaration order; source missing names do not bind. Unknown found receiver
// types require a continuation rather than being accepted as gravity.
bool particle_gravity_bindings_v1(const resources::BresView&,std::uint32_t emitter_instance,
 const scene::Scene&,const std::vector<ParticleGravitySceneV1>&,
 std::vector<std::uint32_t>& actual_force_indices,std::string&);
}
