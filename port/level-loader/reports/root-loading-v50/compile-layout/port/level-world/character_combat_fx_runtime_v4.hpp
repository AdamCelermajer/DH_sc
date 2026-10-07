#pragma once
#include "character_combat_fx_runtime_v2.hpp"
#include "character_mesh_fx_owner_v4.hpp"
#include "character_combat_hit_fx_v4.hpp"
namespace dh2::character::skills {
class CharacterCombatFxRuntimeV4 {
 CharacterWorldRuntimeV1& world_;data::EffectsTables::Borrow tables_;
 const scene::Scene& scene_;DebugSwitches& debug_;const DebugFileServices24& files_;
 CombatFxRuntimeServicesV2 services_;fx::DebugModules* modules_{};
 std::unique_ptr<fx::CharacterMeshFxOwnerV4> manager_;std::string error_;
 static bool service(void*,fx::MeshFxRequestV1&,std::string&);
public:
 CharacterCombatFxRuntimeV4(CharacterWorldRuntimeV1&,data::EffectsTables::Borrow,
  const scene::Scene&,DebugSwitches&,const DebugFileServices24&,CombatFxRuntimeServicesV2);
 ~CharacterCombatFxRuntimeV4();
 CharacterCombatFxRuntimeV4(const CharacterCombatFxRuntimeV4&)=delete;
 int initialize();
 // Source F_ApplyResult HitFX only: 1 handled,0 other,negative required failure.
 int application(const SkillApplyRequestV6&,const data::CombatResult&);
 int animation_event(const char* actual_prefixed_name,std::uintptr_t owner);
 int frame(std::int32_t source_absolute_ms,std::int32_t source_app_dt);
 int draw_parts(std::vector<skinning::VisualDrawPartV6>&);
 const std::string& error()const noexcept{return error_;}
 fx::CharacterMeshFxOwnerV4& manager()noexcept{return *manager_;}
 const scene::Scene& live_scene()const noexcept{return scene_;}
};
}
