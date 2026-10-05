#pragma once
#include "character_mesh_fx_owner_v1.hpp"
namespace dh2::fx {
// A real retained particle visual resource supplies the reached source cloud
// initialization, authored sampling, simulation and draw geometry. No resource
// is produced when any reached source stage fails.
class CharacterParticleFxResourceV2 {
public:
 virtual ~CharacterParticleFxResourceV2()=default;
 virtual std::int32_t start_ms()const noexcept=0;
 virtual std::int32_t end_ms()const noexcept=0;
 virtual bool sample_animation(std::int32_t local_animation_ms,std::string&)=0;
 virtual bool scene_frame(std::int32_t source_absolute_ms,std::int32_t actual_app_dt,
  const std::array<float,16>& actual_outer_world,std::string&)=0;
 virtual bool completed(bool&,std::string&)const=0;
 virtual bool draw_parts(std::vector<skinning::VisualDrawPartV6>&,std::string&)const=0;
};
struct CharacterParticleFxFactoryV2 {
 void* context{};
 bool (*create)(void*,std::shared_ptr<const std::vector<std::uint8_t>> exact_resource,
  const scene::Scene& same_live_scene,std::shared_ptr<CharacterParticleFxResourceV2>&,
  std::string&){};
};
}
