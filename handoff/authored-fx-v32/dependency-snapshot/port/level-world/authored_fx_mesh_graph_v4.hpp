#pragma once
#include "character_particle_fx_resource_v2.hpp"
namespace dh2::fx {
struct CharacterFxMeshDrawSourceV4 {
 std::shared_ptr<const std::vector<std::uint8_t>> resource_bytes;
 const resources::BresView* image{};const scene::Scene* scene{};
 std::uintptr_t fx_identity{},node_identity{};
 std::uint32_t node{},material{},primitive{},source_part{},camera_offset_word{},rendering_layer{};
 skinning::VisualDrawPartV6 part;
 const math::Matrix4f* source_texture_matrix68{};
};
// Mesh and material receivers over ONE same animated graph. Immutable vertex
// input stays cached; submission snapshots apply the real UV matrix equation.
class AuthoredFxMeshGraphV4 {
 struct Impl;std::shared_ptr<Impl> impl_;
public:
 AuthoredFxMeshGraphV4(std::shared_ptr<const std::vector<std::uint8_t>>,
  std::shared_ptr<void> same_graph_owner,scene::Scene&,std::vector<math::Matrix4f>&);
 bool initialize(std::string&);
 bool sample(std::int32_t,std::string&);
 bool draw_sources(const math::Matrix4f& actual_outer,
  std::vector<CharacterFxMeshDrawSourceV4>&,std::string&)const;
};
}
