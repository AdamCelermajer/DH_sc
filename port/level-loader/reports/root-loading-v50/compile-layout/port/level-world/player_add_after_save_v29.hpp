#pragma once
#include "player_add_character_owner_v5.hpp"
#include "player_save_identity_authority_v29.hpp"
#include "player_info_skill_buffers_v26.hpp"
namespace dh2::player {
// Explicit continuation after already executed Character.InitializePlayerSavegame
// C1/SetCharacter. No replacement Save allocation or accepted initialize_save
// callback. Every remaining InitAll/Camera/Idle/skills/visibility/QuickSave and
// controller/light boundary still requires its real provider.
class PlayerAddAfterSaveV29 {
 PlayerManagerOwnerV1& manager_;PlayerSaveIdentityAuthorityV29& save_;
 PlayerAddServicesV5 services_;std::int32_t source_internal_input_{};
 PlayerNetworkLocalOwnerV4& network_;PlayerInfoSkillBuffersV26& arrays_;
 data::SavedSkillUpdateServicesV1 skill_update_;PlayerInfoFieldsV1* record_{};
 bool attempted_{},busy_{},failed_{};PlayerAddResultV5 result_;
 bool call(PlayerAddRequestV5,PlayerAddResponseV5&,std::string&);
 bool continuation(PlayerInfoFieldsV1&,std::int32_t*,std::string&);
public:
 PlayerAddAfterSaveV29(PlayerManagerOwnerV1& manager,PlayerSaveIdentityAuthorityV29& save,PlayerAddServicesV5 services,
  PlayerNetworkLocalOwnerV4& network,PlayerInfoSkillBuffersV26& arrays,data::SavedSkillUpdateServicesV1 skill_update)
  :manager_(manager),save_(save),services_(std::move(services)),network_(network),arrays_(arrays),skill_update_(skill_update){}
 bool execute(PlayerInfoFieldsV1&,std::int32_t* actual_count6c4,
              std::int32_t actual_internal_input,std::string&);
 const PlayerAddResultV5& result()const noexcept{return result_;}
 bool failed()const noexcept{return failed_;}
};
}
