#pragma once
#include "character_combat_hit_fx_v1.hpp"
#include "character_skill_combat_v6.hpp"
#include "visual_fx_preload.hpp"
namespace dh2::character::skills {
struct CombatFxRuntimeServicesV1 {
 fx::MeshFxAssetsV1 assets{};
 CombatHitFxServicesV1 transform{};
 // Source anchor/floor branches are only requested when the authored set
 // requires them. Fixed-rotation unanchored HitFX does not use a floor normal.
 fx::MeshFxServicesV1 remaining{};
};
class CharacterCombatFxRuntimeV1 {
 CharacterWorldRuntimeV1& world_;data::EffectsTables::Borrow tables_;
 const scene::Scene& scene_;DebugSwitches& debug_;const DebugFileServices24& files_;
 CombatFxRuntimeServicesV1 services_;fx::DebugModules* modules_{};
 std::unique_ptr<fx::CharacterMeshFxOwnerV1> manager_;std::string error_;
 static bool service(void*,fx::MeshFxRequestV1&,std::string&);
public:
 CharacterCombatFxRuntimeV1(CharacterWorldRuntimeV1&,data::EffectsTables::Borrow,
  const scene::Scene&,DebugSwitches&,const DebugFileServices24&,CombatFxRuntimeServicesV1);
 ~CharacterCombatFxRuntimeV1();
 CharacterCombatFxRuntimeV1(const CharacterCombatFxRuntimeV1&)=delete;
 int initialize();
 // Source F_ApplyResult HitFX only: 1 handled,0 other,negative required failure.
 int application(const SkillApplyRequestV6&,const data::CombatResult&);
 int animation_event(const char* actual_prefixed_name,std::uintptr_t owner);
 int frame(std::int32_t source_absolute_ms,std::int32_t source_app_dt);
 int draw_parts(std::vector<skinning::VisualDrawPartV6>&);
 const std::string& error()const noexcept{return error_;}
 fx::CharacterMeshFxOwnerV1& manager()noexcept{return *manager_;}
 const scene::Scene& live_scene()const noexcept{return scene_;}
};
}
