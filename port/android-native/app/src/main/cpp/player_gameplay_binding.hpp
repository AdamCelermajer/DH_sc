#pragma once
#include "character_game_design.hpp"
#include "character_player_skills_v6.hpp"
#include "player_equipment_render_owner_v1.hpp"
#include "character_state.hpp"
#include "character_state_owner.hpp"
#include "character_world_runtime_v1.hpp"
#include "owned_hud_settings_v1.hpp"
#include "character_ai_attack.hpp"
namespace model_renderer {
// A synchronous GL-thread borrow. The world owner retains all authorities;
// callers must discard this descriptor before world replacement/recreation.
struct PlayerGameplayBinding {
 dh2::character::CharacterGameDesign::Borrow design;
 dh2::player::PlayerEquipmentRenderOwnerV1* gear{};
 dh2::character::skills::CharacterPlayerSkillsV6* skills{};
 dh2::data::PlayerSavegameV1* save{};
 dh2::data::PropertyView* properties{};
 dh2::data::CombatActorState* life{};
 dh2::character::State* state{};
 dh2::character::CharacterStateOwner* state_owner{};
 dh2::character::skills::CharacterWorldRuntimeV1* world_targets{};
 dh2::character::ControllerCommandState32 controller{};
 std::uintptr_t character{};
 std::int32_t difficulty{};
  bool active{};
  dh2::ui::OwnedHudSettingsV1* settings{};
 // Sole supplemental Character attack/OOI state, shared with native melee.
 dh2::character::AttackState64* attack_fields{};
 // Pins external services through menu callbacks and VM finalizers. The
 // descriptor itself remains a synchronous borrow of mutable gameplay state.
 std::shared_ptr<void> world_owner;
 // Scoped lifetime of the actual UI settings/text platform. World identity
 // remains the original owner; presentation is a separate resource borrow.
 std::shared_ptr<void> presentation_owner;
 dh2::data::SkillTables::Borrow skill_tables;
 const std::int32_t* skill_list_index{};
 std::shared_ptr<dh2::data::PropertySheet> temporary;
 dh2::character::DebugSwitches* debug{};
 const dh2::character::DebugFileServices24* debug_files{};
 dh2::ui::HudTextEnvironmentV1 text_environment;
 std::int32_t actor_index{-1};
 void* potion_capacity_context{};
 bool(*potion_capacity_store)(void*,std::int32_t,std::string&){};
};
PlayerGameplayBinding player_gameplay_binding();
std::string player_gameplay_action(int operation,int index);
}
