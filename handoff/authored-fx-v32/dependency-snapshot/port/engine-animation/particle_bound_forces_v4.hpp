#pragma once
#include "particle_force_scene_v2.hpp"
#include <map>
#include <memory>
namespace dh2::animation {
struct ParticleForceWorldServicesV4 {
 std::shared_ptr<void> owner;
 void* context{};
 // Same authored Scene node world and source-produced marker, not numeric
 // equality classification. Its initial value is read at proxy construction.
 bool(*node_world)(void*,const scene::Scene&,std::uint32_t,math::Matrix4f&,std::string&){};
};
// One per retained FX Scene, shared by all emitters. Sole typed force-node
// matrix projection; same graph's float64 bytes remain the pose authority.
class ParticleForceWorldOwnerV4 {
 const scene::Scene& scene_;
 ParticleForceWorldServicesV4 services_;
 std::map<std::uint32_t,math::Matrix4f> matrices_;
 std::vector<ParticleForceSceneV2> forces_;
 const std::uint8_t* image_bytes_{};std::size_t image_size_{};
 bool declarations_attempted_{},declarations_ready_{};
 math::Matrix4f outer_{};bool outer_written_{};
 bool produce(std::uint32_t,math::Matrix4f&,std::string&);
public:
 ParticleForceWorldOwnerV4(const scene::Scene& scene,ParticleForceWorldServicesV4 services):scene_(scene),services_(std::move(services)){}
 ParticleForceWorldOwnerV4(const ParticleForceWorldOwnerV4&)=delete;
 ParticleForceWorldOwnerV4& operator=(const ParticleForceWorldOwnerV4&)=delete;
 bool matrix(std::uint32_t,math::Matrix4f*&,std::string&);
 bool update_outer(const math::Matrix4f&,std::string&);
 bool bindings(const resources::BresView&,std::uint32_t,std::vector<std::uint32_t>&,std::string&);
 const ParticleForceSceneV2& force(std::uint32_t index)const{return forces_.at(index);}
 // Actual scene attribute successors write these SAME per-node fields.
 ParticleForceSceneV2& source_force(std::uint32_t index){return forces_.at(index);}
 const scene::Scene& scene()const noexcept{return scene_;}
};
class ParticleBoundForcesV4 {
 std::shared_ptr<ParticleForceWorldOwnerV4> world_;
 struct Bound {std::uint32_t force{};ParticleDeflectorModelV1 model{};};
 std::vector<Bound> bound_;
 bool attempted_{},ready_{};
public:
 ParticleBoundForcesV4()=default;
 ParticleBoundForcesV4(const ParticleBoundForcesV4&)=delete;
 ParticleBoundForcesV4& operator=(const ParticleBoundForcesV4&)=delete;
 bool decode(const resources::BresView&,const scene::Scene&,std::uint32_t actual_emitter_instance,
  std::shared_ptr<ParticleForceWorldOwnerV4>,std::string&);
 bool update_outer(const math::Matrix4f&,std::string&);
 bool apply(ParticleSeed100*,std::uint32_t,float,std::int32_t*,std::string&);
 std::size_t size()const noexcept{return bound_.size();}
 // No reset/reconstructor on cloud time reset: source proxy interface has only
 // destructor/apply. Rebinding requires actual proxy destruction/new owner.
};
}
