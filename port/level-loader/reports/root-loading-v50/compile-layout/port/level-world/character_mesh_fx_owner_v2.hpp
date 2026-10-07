#pragma once
#include "character_particle_fx_resource_v2.hpp"
namespace dh2::fx {
class CharacterMeshFxOwnerV2 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 // Versioned successor preserves the frozen source pool/state/timeline logic.
 // Positive particle resources require the actual typed factory; other
 // unsupported resource/set families retain explicit required failures.
 CharacterMeshFxOwnerV2(data::EffectsTables::Borrow,const scene::Scene& same_live_player_scene,MeshFxAssetsV1,MeshFxServicesV1,CharacterParticleFxFactoryV2);
 ~CharacterMeshFxOwnerV2();CharacterMeshFxOwnerV2(const CharacterMeshFxOwnerV2&)=delete;
 CharacterMeshFxOwnerV2& operator=(const CharacterMeshFxOwnerV2&)=delete;
 // Source constructor starts with precache byte0. Genuine PreCacheLibraries
 // performs Debug Load/GetModule before setting it; no assumed pool default.
 bool precache_libraries(std::string&);
 bool precached()const noexcept;
 bool play_set(std::int32_t,const float position[3],const float* nullable_rotation,std::uintptr_t nullable_anchor,std::uintptr_t* created_identity,std::string&);
 // Real CharAI fx_ path: ordered name lookup, actual owner position supplied by
 // caller, NULL anchor/parent as shipping _OnAnimEvent3d4434.
 bool animation_event(const char* event,const float actual_position[3],std::string&);
 bool drop(std::uintptr_t& identity,std::string&);
 // Separate scene phase and VisualFXManager phase preserve source ordering.
 bool scene_frame(std::int32_t absolute_ms,std::int32_t actual_app_dt,std::string&);
 bool manager_frame(std::int32_t actual_app_dt,std::string&);
 bool draw_parts(std::vector<skinning::VisualDrawPartV6>&,std::string&)const;
 // These borrows remain valid until this manager is destroyed. A caller must
 // compose actual SceneManager priority/distance/order before GL submission.
 bool particle_draw_sources_v3(std::vector<CharacterParticleDrawSourceV3>&,std::string&)const;
 std::vector<MeshFxViewV1> views()const;
 std::size_t cold_creations()const noexcept;std::size_t warm_reuses()const noexcept;
 // Destruction is quiescent; callbacks may play/drop but may not destroy owner,
 // assets/services, table borrow or live Scene during an active call. Rebinding
 // to a replacement GL scene requires destroying this owner first.
};
}
