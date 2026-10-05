#pragma once
#include "original_cache_assets_v1.hpp"
#include "player_gameplay_binding.hpp"
#include "hud_text_v1.hpp"
#include "item_presentation_v5.hpp"
#include <map>
namespace dh2::android_ui {
struct PowerAttributePresentationV1 {
 std::int32_t property_id{},raw_value{},flags{};
 float source_number{};
 std::string property_name;
 // Source Attr enum differs from CharacterProperties indices. Both weapon
 // hands are reported; the gameplay gear kernel chooses the actual hand.
 std::vector<std::int32_t> right_property_ids,left_property_ids;
 std::vector<std::string> right_property_names,left_property_names;
};
struct PowerPresentationV1 {
 std::int32_t id{},palette{},special_effect{},description_id{},authored_sorting_order{};
 std::string identifier,description;
 std::vector<data::GearPowerProperty12V5> properties;
 std::vector<PowerAttributePresentationV1> attributes;
};
struct FaeryPresentationV1 {
 std::int32_t slot{},table_id{},element{},model_id{},name_id{},description_id{},spell_type{},type{};
 std::string identifier,name,description,script;
};
// Immutable cache tables and ephemeral derived text only. It retains no live
// player, inventory, Save, power mutation or second StringManager authority.
class CharacterPanelPresentationV1 {
 OriginalCacheAssetsV1& assets_;data::ItemPowerTablesV5 power_tables_;data::FaeryTables faery_tables_;
 bool ready_{};
 // Query adapter borrows these vectors through its synchronous writes. IDs
 // come from the actual live item each call, descriptions from original rows.
 std::map<const data::ItemInstanceV1*,std::vector<data::ItemPowerInstanceV5>> views_;
 bool prepare(std::string&);
 static bool raw(ui::HudTextV1&,const ui::HudTextEnvironmentV1&,std::int32_t,std::string&,std::string&);
 bool describe(std::int32_t,ui::HudTextV1&,const ui::HudTextEnvironmentV1&,PowerPresentationV1&,std::string&);
public:
 explicit CharacterPanelPresentationV1(OriginalCacheAssetsV1& assets):assets_(assets){}
 // Binds CharacterMenuQueriesGraphV1::powers. No power is appended/reordered
 // and the supplied item must be the actual same-world inventory instance.
 bool powers(const model_renderer::PlayerGameplayBinding&,const data::ItemInstanceV1&,
  ui::HudTextV1&,const ui::HudTextEnvironmentV1&,
  const std::vector<data::ItemPowerInstanceV5>*&,std::string&);
 bool power_details(const model_renderer::PlayerGameplayBinding&,const data::ItemInstanceV1&,
  ui::HudTextV1&,const ui::HudTextEnvironmentV1&,std::vector<PowerPresentationV1>&,std::string&);
 // Same selected source FaeryList from resolved property29, including its
 // original fallback0. Save availability/level remain the caller's live Save.
 bool faery(const model_renderer::PlayerGameplayBinding&,std::uint32_t slot,
  ui::HudTextV1&,const ui::HudTextEnvironmentV1&,FaeryPresentationV1&,std::string&);
};
}
