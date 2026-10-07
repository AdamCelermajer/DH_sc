#pragma once
#include "character_world_runtime_v1.hpp"
#include "player_equipment_render_owner_v1.hpp"
#include "../engine-ui/settings_language_scene_v1.hpp"
#include "../engine-ui/item_language_localization_v1.hpp"
namespace dh2::character {
struct CharacterLanguageSceneServicesV1 {
 std::shared_ptr<void> owner;
 // Exact source ObjectManager list/tree projected from the actual World. Its
 // type and valid-byte pointers alias canonical source fields, especially
 // type14 +819. A characters-only registry cannot stand in for this graph.
 ui::SettingsLanguageScene24V1* scene{};
 skills::CharacterWorldRuntimeV1* actors{};
 const target_providers::Types16* ai_types{};
 const target_providers::Services16* target_services{};
 std::function<bool(std::uintptr_t,player::PlayerEquipmentRenderOwnerV1*&,std::string&)> gear;
 // Genuine full inventory refresh for merchants/non-player inventories.
 std::function<bool(std::uintptr_t,std::string&)> other_inventory;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_game_object;
 // Type3 ItemObject UpdateLocalization: real owned inventory index0 nullable.
 std::function<bool(std::uintptr_t,std::string&)> item_object;
 ui::ItemLanguagePowerServicesV1 powers;
};
class CharacterLanguageSceneConnectionV1 {
 CharacterLanguageSceneServicesV1 services_;std::string error_;
 static int invoke(void*,const ui::SettingsSceneRequest16V1*,std::uint32_t*);
 bool inventory(std::uintptr_t);
public:
 explicit CharacterLanguageSceneConnectionV1(CharacterLanguageSceneServicesV1);
 bool refresh(std::string&);
};
}
