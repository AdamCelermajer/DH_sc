#pragma once
#include "character_game_design.hpp"
#include "../game-data/loot_tables_v2.hpp"
#include "../game-data/loot_power_resources_v7.hpp"
#include "../game-data/loot_audiovisual_v8.hpp"
#include "../engine-ui/item_text_owner_v5.hpp"
#include <functional>
namespace dh2::character {
struct SourceMerchantEntryV114 {std::int32_t condition4{},merchandise8{};};
struct SourceMerchantRowV114 {std::int32_t buy4{},sell8{};std::vector<SourceMerchantEntryV114> entries;};
struct SourceItemResourceInputsV88 {
 std::shared_ptr<void> owner,localization_owner;
 std::function<bool(const char*,bool&,std::vector<std::uint8_t>&,std::string&)> asset;
 //The original six Loot tables already decoded by the actual campaign cache.
 data::LootTablesV2::Borrow loot;
 CharacterGameDesign::Borrow design;
 ui::HudTextV1* localization{};
 ui::HudTextEnvironmentV1 text_environment;
};
//Immutable Arrays resource + actual StringManager transport. No Item pool,
//Inventory, Character, Gear, HP/XP, RNG, selected slot or readiness is made.
//Gear and the source Item manager borrow these SAME row/power identities.
class SourceItemResourcesV88 {
 SourceItemResourceInputsV88 inputs_;
 data::ItemPowerTablesV5 definitions_;
 data::LootPowerResourcesV7 powers_;
 data::LootAudioVisualV8 audiovisual_;
 std::vector<SourceMerchantRowV114> merchants_v114_;
 std::vector<std::string> merchant_names_v114_;
 std::shared_ptr<data::ItemPresentationOwnerV5> presentation_;
 std::unique_ptr<ui::ItemTextOwnerV5> text_;
 bool attempted_{},ready_{};
 bool read(const char*,std::vector<std::uint8_t>&,std::string&);
public:
 explicit SourceItemResourcesV88(SourceItemResourceInputsV88 inputs):inputs_(std::move(inputs)){}
 bool load(std::string&);
 bool ready()const noexcept{return ready_;}
 const data::LootTablesV2::Borrow& loot()const noexcept{return inputs_.loot;}
 data::ItemPowerTablesV5::Borrow definitions()const{return definitions_.borrow();}
 data::LootPowerResourcesV7::Borrow powers()const{return powers_.borrow();}
 data::LootAudioVisualV8::Borrow audiovisual()const{return audiovisual_.borrow();}
 data::ItemTextServicesV5 text()const{return text_?text_->services():data::ItemTextServicesV5{};}
 const auto& presentation()const noexcept{return presentation_;}
 const auto& merchants_v114()const noexcept{return merchants_v114_;}
 const auto& merchant_names_v114()const noexcept{return merchant_names_v114_;}
};
}
