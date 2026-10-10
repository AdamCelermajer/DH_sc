#pragma once
#include "character_menu_profile_load_v51.hpp"
#include "player_equipment_render_owner_v1.hpp"
namespace dh2::character {
struct CharacterProfileSlotStoreReceiptV59 {
 const data::PlayerSavegameV1* save{};
 std::int32_t value{-1};
};
struct CharacterProfileBootstrapInputsV59 {
 // All source receivers are already constructed. No replacement Save C1,
 // Character, Gear or PlayerInfo is allocated by this connection.
 std::shared_ptr<data::PlayerSavegameV1> save;
 std::shared_ptr<data::PlayerSaveLoadOwnerV1> load;
 std::shared_ptr<level::CampaignSaveProfileV45> profile;
 std::shared_ptr<const void> selected_file_lease;
 data::Bytes selected_file_bytes{};
 std::int32_t selected_slot{-1};
 std::uintptr_t character{};
 std::shared_ptr<void> actual_source_cells_lease;
 const std::uintptr_t* source_save14e8{};
 const std::int32_t* source_player_info_slot664{};
 // Menu CreatePlayer has no PlayerInfo association. Its actual SG_SetSlot
 // source is this SAME Save slot, never a fabricated PM664 record.
 const std::int32_t* source_direct_save_slot_v122{};
 std::function<bool(std::int32_t,std::string&)> source_set_slot;
 // Modern execution receipt for the actual SetSlot that preceded profile C1.
 // Validated against SAME current cells; not a source gameplay-ready flag.
 CharacterProfileSlotStoreReceiptV59 prior_slot_store;
 CharacterMenuProfileLoadServicesV51 reads;
 // Genuine online/volatile/network providers. The connection handles only
 // pre-Grant PROP and GEAR, then forwards all other reached source operations.
 data::PlayerSaveLoadServicesV1 remaining;
 // Fresh canonical InitPost owns its later SG_Load2 after LoadBase/recalc.
 // Legacy staged adoption retains prepare's historical masks1+2 behavior.
 bool defer_mask2_to_initpost_v62{};
};
class CharacterProfileBootstrapV59:public std::enable_shared_from_this<CharacterProfileBootstrapV59> {
 CharacterProfileBootstrapInputsV59 input_;
 std::shared_ptr<CharacterMenuProfileLoadV51> reader_;
 std::shared_ptr<CharacterMenuCampaignSaveV50> writer_;
 player::PlayerEquipmentRenderOwnerV1* equipment_{};
 bool attempted_{},prepared_{},initializing_{},load4_attempted_{},load4_complete_{},gear_delivered_{},finish_attempted_{},finished_{};
 bool coherent(std::string&)const;
 const std::int32_t* source_slot_v122()const noexcept{return input_.source_direct_save_slot_v122?input_.source_direct_save_slot_v122:input_.source_player_info_slot664;}
 bool remaining(const data::PlayerSaveLoadRequestV1&,data::PlayerSaveLoadResponseV1&,std::string&);
public:
 explicit CharacterProfileBootstrapV59(CharacterProfileBootstrapInputsV59);
 // Actual source field-reader and initialized-structure masks1 then2. These
 // are a staged native adoption of already-created campaign receivers, not a
 // claim that PlayerManager.AddCharacter/full GameState creation completed.
 bool prepare(std::string&);
 bool configure_equipment(player::PlayerEquipmentRenderInputsV1&,std::string&);
 bool before_initial_grant(player::PlayerEquipmentRenderOwnerV1&,std::string&);
 bool finish(player::PlayerEquipmentRenderOwnerV1&,CharacterMenuCampaignSaveServicesV50,std::string&);
 const auto& save()const noexcept{return input_.save;}
 const auto& load_owner()const noexcept{return input_.load;}
 const auto& profile()const noexcept{return input_.profile;}
 const auto& campaign_writer()const noexcept{return writer_;}
 const auto& quest_owner_v70()const noexcept{return input_.reads.quests;}
 bool gear_delivered()const noexcept{return gear_delivered_;}
 bool finished()const noexcept{return finished_;}
};
}
