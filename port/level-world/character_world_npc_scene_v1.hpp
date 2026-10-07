#pragma once
#include "character_npc_body.hpp"
#include "visual_motion.hpp"
namespace dh2::character {
struct WorldNpcSceneServicesV1 {
 void* context{};
 // Changed rotation/scale source tail: CalcMeshBox then same-owner AABB/PF.
 bool (*refresh_bounds)(void*,std::string&){};
};
// Borrows the existing CPU visual and scene; no second scene, position or body.
class CharacterWorldNpcSceneV1 {
 visual::SceneBinding& visual_;scene::Scene& scene_;std::uint32_t& light_;
 WorldNpcSceneServicesV1 services_;
 bool changed_bounds(std::string&);
public:
 CharacterWorldNpcSceneV1(visual::SceneBinding&,scene::Scene&,std::uint32_t& light40,WorldNpcSceneServicesV1);
 // Receive genuine InitPost projection before InitFinal. This is the scene
 // receiver of those already-produced transforms, not a complete InitPost.
 bool receive_init_post(const physical::NpcBodyProjection&,const float* actual_position3,std::string&);
 bool sync(const float* actual_position3,const float* actual_euler_radians3,const float* actual_scale3,std::string&);
 void assign_light(std::uint32_t value) noexcept{light_=value;}
 bool find_node(const char*,std::uintptr_t&,std::string&)const;
 bool node_position(std::uintptr_t,float* xyz,std::string&)const;
};
}
