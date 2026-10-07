#pragma once
#include "authored_fx_mesh_graph_v4.hpp"
namespace dh2::fx {
class AuthoredFxMeshGraphV6 {
 struct Impl;std::shared_ptr<Impl> impl_;
public:
 AuthoredFxMeshGraphV6(std::shared_ptr<const std::vector<std::uint8_t>>,std::shared_ptr<void>,scene::Scene&,std::vector<math::Matrix4f>&);
 bool initialize(std::string&);bool sample(std::int32_t,std::string&);
 bool draw_sources(const math::Matrix4f&,std::vector<CharacterFxMeshDrawSourceV4>&,std::string&)const;
};
}
