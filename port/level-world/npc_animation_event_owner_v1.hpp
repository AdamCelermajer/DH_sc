#pragma once
#include "character_animation_event_owner_v1.hpp"
#include "character_script_owner.hpp"
namespace dh2::character {
struct NpcAnimationEventBorrowV1 {
 AIEventState64* ai{};ScriptOwner* script{};std::uintptr_t character{};
 const std::int32_t* animator_lag{};data::PropertyView* properties{};
 skills::CharacterWorldRuntimeV1* world{};const std::uintptr_t* visual{};
 const scene::Scene* scene{};const std::uint32_t* visual_root{};
 void* floor_context{};int(*floor_type)(void*,const char**){};
};
// Actual selected NPC legacy ScriptOwner transport for source AISDefault
// OnAnimEvent3dca50. This does not construct another VM or select an AIS.
class NpcAnimationEventOwnerV1 {
 NpcAnimationEventBorrowV1 b_;data::EffectsTables::Borrow effects_;
 fx::CharacterMeshFxOwnerV1* mesh_{};std::string error_;std::uint32_t lua_error_{};
 fx::CharacterMeshFxOwnerV4* mesh_v4_{};
 bool foot(bool,float*);bool play(int,const float*);
public:
 NpcAnimationEventOwnerV1(NpcAnimationEventBorrowV1 b,data::EffectsTables::Borrow effects,fx::CharacterMeshFxOwnerV1* mesh):b_(b),effects_(std::move(effects)),mesh_(mesh){}
 void bind_mesh_v4(fx::CharacterMeshFxOwnerV4& manager)noexcept{mesh_v4_=&manager;}
 bool relay(const char* stripped_event,const dh2_script_callback_scope* scope=nullptr);
 const std::string& error()const noexcept{return error_;}
 std::uint32_t source_lua_error()const noexcept{return lua_error_;}
};
}
