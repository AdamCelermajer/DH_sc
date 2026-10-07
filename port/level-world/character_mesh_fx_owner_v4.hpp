#pragma once
#include "character_authored_particle_fx_v4.hpp"
#include "character_fx_anchor_rotation_v28.hpp"
#include <optional>
namespace dh2::fx {
class VisualFxManagerLibrariesV63;
class CharacterMeshFxOwnerV4 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 // Versioned successor preserves the frozen source pool/state/timeline logic.
 // Positive particle resources require the actual typed factory. Authored
 // sets share the source sequence/redirect/random callback engine below.
 CharacterMeshFxOwnerV4(data::EffectsTables::Borrow,const scene::Scene& same_live_player_scene,MeshFxAssetsV1,MeshFxServicesV1,CharacterParticleFxFactoryV2);
 // Source campaign instance backend borrows its App-owned pre-player library.
 // This does not rebuild dictionaries/sets or allocate a replacement manager.
 CharacterMeshFxOwnerV4(std::shared_ptr<VisualFxManagerLibrariesV63>,const scene::Scene& same_live_player_scene,MeshFxAssetsV1,MeshFxServicesV1,CharacterParticleFxFactoryV2);
 ~CharacterMeshFxOwnerV4();CharacterMeshFxOwnerV4(const CharacterMeshFxOwnerV4&)=delete;
 CharacterMeshFxOwnerV4& operator=(const CharacterMeshFxOwnerV4&)=delete;
 // Source constructor starts with precache byte0. Genuine PreCacheLibraries
 // performs Debug Load/GetModule before setting it; no assumed pool default.
 bool precache_libraries(std::string&);
 bool precached()const noexcept;
 bool flush_libraries_v88(std::string&);
 const std::shared_ptr<VisualFxManagerLibrariesV63>& source_libraries_v63()const noexcept;
 bool play_set(std::int32_t,const float position[3],const float* nullable_rotation,std::uintptr_t nullable_anchor,std::uintptr_t* created_identity,std::string&);
 //Original Random::GetRandom(count,false) over the SAME process source seed.
 //Only random authored sets reach this leaf; no private FX RNG is created.
 void bind_set_random_v118(std::function<bool(std::uint32_t,std::uint32_t&,std::string&)>);
 // Real CharAI fx_ path: ordered name lookup, actual owner position supplied by
 // caller, NULL anchor/parent as shipping _OnAnimEvent3d4434.
 bool animation_event(const char* event,const float actual_position[3],std::string&);
 bool drop(std::uintptr_t& identity,std::string&);
 // Original StopEffect resolves the set's animated-file pool, rather than
 // dropping a separately stored script handle.
 bool drop_set_by_id_v117(std::int32_t,std::string&);
 // Native lifetime observer: effects may outlive their target. Match source
 // Update's dead-anchor NULL store, retaining the last submitted transform.
 void detach_anchor_v117(std::uintptr_t)noexcept;
 // GrabAnimFX495430 uses first-step resource and selected/random data. Unlike
 // PlayAnimFXSet it keeps loop=-1,
 // so retained target indicators are controlled by Character, not auto-return.
 bool grab_marker_v28(std::int32_t set,std::uintptr_t nullable_anchor,std::uintptr_t& identity,std::string&);
 bool marker_anchor_v28(std::uintptr_t identity,std::uintptr_t anchor,bool reset,std::string&);
 bool marker_store_anchor_v83(std::uintptr_t identity,std::uintptr_t anchor,std::string&);
 bool marker_sync_v83(std::uintptr_t identity,bool reset,std::string&);
 bool marker_rotation_v70(std::uintptr_t identity,const float source_rotation34[3],std::string&);
 bool marker_visible_v28(std::uintptr_t identity,bool visible,std::string&);
 //Quest TalkToNPC's actual visual204 store and animator looping suffix.
 bool marker_visual_owner_v76(std::uintptr_t identity,std::uintptr_t owner,std::string&);
 bool marker_loop_v76(std::uintptr_t identity,bool looping,std::string&);
 //PROPS_AddBuff: GetAnimController492550 then virtual1c PlayClip(index,true,0,0).
 //The returned borrower identifies this SAME retained controller, not a child.
 bool buff_anim_controller_v87(std::uintptr_t identity,std::uintptr_t&,std::string&);
 bool buff_play_clip_v87(std::uintptr_t controller,std::uint32_t clip,std::string&);
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
