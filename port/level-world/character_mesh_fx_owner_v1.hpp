#pragma once
#include "character_fx_state_v1.hpp"
#include "../game-data/effects_tables.hpp"
#include "../engine-skinning/visual_skin_owner_v6.hpp"
#include <memory>
namespace dh2::fx {
enum class MeshFxOperationV1 {debug_load,module_enabled,set_switch,instance_switch,anchor_dead,anchor_disabled,
 anchor_stationary,anchor_position,anchor_rotation,anchor_scale,floor_normal};
struct MeshFxRequestV1 {
 MeshFxOperationV1 operation;std::uintptr_t identity{};const char* text{};
 std::uint32_t result{};float point[3]{};
 // anchor_rotation must return the source VisualObject/Character rotation in
 // radians, projected from this SAME live scene, not a rendered proxy actor.
 const scene::Scene* live_scene{};
};
struct MeshFxServicesV1 {void* context{};bool(*invoke)(void*,MeshFxRequestV1&,std::string&){};};
struct MeshFxAssetsV1 {void* context{};bool(*read)(void*,const char* exact_uri,std::vector<std::uint8_t>&,std::string&){};};
struct MeshFxViewV1 {std::uintptr_t identity{};FxState96V1 state;std::int32_t set{-1},current_ms{},start_ms{},end_ms{};bool pooled{},pending_return{},finished{};std::string uri;};
class CharacterMeshFxOwnerV1 {
 struct Impl;std::unique_ptr<Impl> impl_;
public:
 // Explicit mesh domain: type0 one-step, nonredirected sets; real node TRS and
 // CTextureTransformEx87..91. Source particle/light/controller constructors and
 // other set families fail with a required-continuation diagnostic.
 CharacterMeshFxOwnerV1(data::EffectsTables::Borrow,const scene::Scene& same_live_player_scene,MeshFxAssetsV1,MeshFxServicesV1);
 ~CharacterMeshFxOwnerV1();CharacterMeshFxOwnerV1(const CharacterMeshFxOwnerV1&)=delete;
 CharacterMeshFxOwnerV1& operator=(const CharacterMeshFxOwnerV1&)=delete;
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
 bool scene_frame(std::int32_t absolute_ms,std::string&);
 bool manager_frame(std::int32_t actual_app_dt,std::string&);
 bool draw_parts(std::vector<skinning::VisualDrawPartV6>&,std::string&)const;
 std::vector<MeshFxViewV1> views()const;
 std::size_t cold_creations()const noexcept;std::size_t warm_reuses()const noexcept;
 // Destruction is quiescent; callbacks may play/drop but may not destroy owner,
 // assets/services, table borrow or live Scene during an active call. Rebinding
 // to a replacement GL scene requires destroying this owner first.
};
}
