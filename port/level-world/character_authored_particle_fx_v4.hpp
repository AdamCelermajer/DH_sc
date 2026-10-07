#pragma once
#include "character_authored_particle_fx_v3.hpp"
#include "../engine-animation/particle_cloud_runtime_v3.hpp"
#include "authored_fx_mesh_graph_v4.hpp"
namespace dh2::fx {
struct CharacterFxBoundForcesV4 {
 std::shared_ptr<void> lease;void* context{};
 int(*apply)(void*,animation::ParticleSeed100*,std::uint32_t,float,std::int32_t*,
  const math::Matrix4f& actual_outer,std::string&){};
};
struct CharacterFxForceFactoryV4 {
 void* context{};
 bool(*create)(void*,const resources::BresView&,const scene::Scene&,std::shared_ptr<void> same_scene_owner,
  std::uint32_t actual_emitter_instance,CharacterFxBoundForcesV4&,std::string&){};
};
class CharacterAuthoredCompositeFxResourceV4:public CharacterParticleFxResourceV2 {
public:
 virtual bool source_scene_frame_v4(std::int32_t absolute,std::int32_t dt,
  const math::Matrix4f& actual_outer,std::string&)=0;
 virtual bool mesh_draw_sources_v4(std::vector<CharacterFxMeshDrawSourceV4>&,std::string&)const=0;
 // Sync publishes the visual root after scene sampling. Draw must borrow that
 // current transform without ticking emission, animation or force history.
 virtual bool mesh_draw_sources_at_outer_v49(const math::Matrix4f& actual_outer,
  std::vector<CharacterFxMeshDrawSourceV4>&,std::string&)const=0;
};
// One retained source graph, static mesh receivers and independently retained
// cloud/model/RNG receivers for every authored emitter instance. The exact
// original resource is never rewritten into artificial single-emitter files.
class CharacterAuthoredParticleFxFactoryV4 {
 CharacterBloodFxSceneServicesV2 services_;CharacterFxForceFactoryV4 forces_;
 static bool create(void*,std::shared_ptr<const std::vector<std::uint8_t>>,
  const scene::Scene&,std::shared_ptr<CharacterParticleFxResourceV2>&,std::string&);
public:
 CharacterAuthoredParticleFxFactoryV4(CharacterBloodFxSceneServicesV2 s,CharacterFxForceFactoryV4 f):services_(s),forces_(f){}
 CharacterParticleFxFactoryV2 factory(){return {this,create};}
};
}
