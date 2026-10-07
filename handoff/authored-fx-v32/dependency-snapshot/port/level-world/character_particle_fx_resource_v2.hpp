#pragma once
#include "character_mesh_fx_owner_v1.hpp"
#include "../engine-math/math.hpp"
namespace dh2::fx {
// Borrowed source render inputs retained by the same particle resource. Queue
// priority and registration order are intentionally NOT supplied here: those
// belong to SceneManager, not to the FX map iteration order.
struct CharacterParticleDrawSourceV3 {
 std::shared_ptr<const std::vector<std::uint8_t>> resource_bytes;
 const resources::BresView* image{};
 const scene::Scene* scene{};
 std::uint32_t emitter_node{}, material{};
 std::array<float,16> emitter_world{};
 std::uintptr_t fx_identity{}, particle_identity{};
 skinning::VisualDrawPartV6 part;
 // Source ISceneNode C1 stores0 at599404/+128 and599420/+12c;
 // getters5974e4/5974f4 return these same constructor-backed words.
 std::uint32_t camera_offset_word{},rendering_layer{};
 bool source_node_render_fields_ready{};
 // Same retained BRES-normalized source parameter, including identity hint.
 const math::Matrix4f* source_texture_matrix68{};
};
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
 virtual bool draw_sources_v3(std::vector<CharacterParticleDrawSourceV3>&,
  std::string& error)const {error="Required source particle render metadata";return false;}
};
struct CharacterParticleFxFactoryV2 {
 void* context{};
 bool (*create)(void*,std::shared_ptr<const std::vector<std::uint8_t>> exact_resource,
  const scene::Scene& same_live_scene,std::shared_ptr<CharacterParticleFxResourceV2>&,
  std::string&){};
};
}
