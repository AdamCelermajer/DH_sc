#pragma once
#include "character_authored_particle_fx_v4.hpp"
#include "character_fx_anchor_rotation_v28.hpp"
namespace dh2::fx {
class CharacterMeshFxOwnerV4 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 // Versioned successor preserves the frozen source pool/state/timeline logic.
 // Positive particle resources require the actual typed factory; other
 // unsupported resource/set families retain explicit required failures.
 CharacterMeshFxOwnerV4(data::EffectsTables::Borrow,const scene::Scene& same_live_player_scene,MeshFxAssetsV1,MeshFxServicesV1,CharacterParticleFxFactoryV2);
 ~CharacterMeshFxOwnerV4();CharacterMeshFxOwnerV4(const CharacterMeshFxOwnerV4&)=delete;
 CharacterMeshFxOwnerV4& operator=(const CharacterMeshFxOwnerV4&)=delete;
 // Source constructor starts with precache byte0. Genuine PreCacheLibraries
 // performs Debug Load/GetModule before setting it; no assumed pool default.
 bool precache_libraries(std::string&);
 bool precached()const noexcept;
 bool play_set(std::int32_t,const float position[3],const float* nullable_rotation,std::uintptr_t nullable_anchor,std::uintptr_t* created_identity,std::string&);
 // Real CharAI fx_ path: ordered name lookup, actual owner position supplied by
 // caller, NULL anchor/parent as shipping _OnAnimEvent3d4434.
 bool animation_event(const char* event,const float actual_position[3],std::string&);
 bool drop(std::uintptr_t& identity,std::string&);
 // GrabAnimFX495430 one-step receiver. Unlike PlayAnimFXSet it keeps loop=-1,
 // so retained target indicators are controlled by Character, not auto-return.
 bool grab_marker_v28(std::int32_t set,std::uintptr_t nullable_anchor,std::uintptr_t& identity,std::string&);
 bool marker_anchor_v28(std::uintptr_t identity,std::uintptr_t anchor,bool reset,std::string&);
 bool marker_visible_v28(std::uintptr_t identity,bool visible,std::string&);
 // Separate scene phase and VisualFXManager phase preserve source ordering.
 bool scene_frame(std::int32_t absolute_ms,std::int32_t actual_app_dt,std::string&);
 bool manager_frame(std::int32_t actual_app_dt,std::string&);
 bool draw_parts(std::vector<skinning::VisualDrawPartV6>&,std::string&)const;
 // These borrows remain valid until this manager is destroyed. A caller must
 // compose actual SceneManager priority/distance/order before GL submission.
 bool particle_draw_sources_v3(std::vector<CharacterParticleDrawSourceV3>&,std::string&)const;
 bool mesh_draw_sources_v4(std::vector<CharacterFxMeshDrawSourceV4>&,std::string&)const;
 std::vector<MeshFxViewV1> views()const;
 std::size_t cold_creations()const noexcept;std::size_t warm_reuses()const noexcept;
 // Destruction is quiescent; callbacks may play/drop but may not destroy owner,
 // assets/services, table borrow or live Scene during an active call. Rebinding
 // to a replacement GL scene requires destroying this owner first.
};
}
