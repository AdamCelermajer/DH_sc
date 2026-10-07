#pragma once
#include "player_manager_update_owner_v70.hpp"
#include "../game-data/player_savegame_v1.hpp"
namespace dh2::player {
struct PlayerManageFieldsV70 {
 std::shared_ptr<void> receiver;
 const std::string* name2d0{};
 const std::int32_t* level330{};
 const std::int32_t* class380{};
 const std::uint8_t* visible4e5{};
 const std::uint8_t* loading525{};
 // Native pointer-width projection of original PM PlayerInfo+680. This is
 // distinct from Character14e8, and never stored in the old int32 DTO tail.
 std::shared_ptr<data::PlayerSavegameV1>* profile680{};
};
enum class PlayerManageAssignmentV70 : std::uint32_t {name=0x371ccc,character_class=0x370ef0,level=0x370e48,visible=0x372a30,loading=0x372b14};
struct PlayerManageServicesV70 {
 std::shared_ptr<void> provider;
 std::function<bool(std::shared_ptr<void>&,const std::uint32_t*&,const std::uint8_t*&,std::string&)> current_level;
 std::function<bool(bool&,std::string&)> online;
 std::function<bool(PlayerInfoFieldsV1&,bool&,std::string&)> selected5c;
 std::function<bool(PlayerInfoFieldsV1&,PlayerManageFieldsV70&,std::string&)> fields;
 // Exact operator new(408,0) followed by PlayerSavegameC1(slot,1,false).
 // Implementer publishes the SAME retained allocation only after C1; failed
 // constructor storage stays in that receiver's lifetime owner.
 std::function<bool(PlayerInfoFieldsV1&,std::int32_t,std::shared_ptr<data::PlayerSavegameV1>&,std::string&)> construct_profile680;
 std::function<bool(bool&,std::string&)> game_center_enabled11,network_alias_enabled28;
 std::function<bool(std::string&,std::string&)> game_center_alias,network_alias;
 std::function<bool(PlayerInfoFieldsV1&,PlayerManageAssignmentV70,std::int32_t,const std::string*,std::string&)> assign;
 std::function<bool(PlayerInfoFieldsV1&,std::string&)> remote_changed;
 std::function<bool(std::string&)> clear_loading_info,remove_all_characters;
};
// Full offline _ManageCharacters37280c control flow, including original
// profile/name/class/level prefix and source AddCharacter count mutation.
class PlayerManageCharactersOwnerV70 {
 PlayerManagerOwnerV1& manager_;PlayerManageServicesV70 services_;bool delivering_{};
 bool offline(std::string&);
 bool assign(PlayerInfoFieldsV1&,PlayerManageAssignmentV70,std::int32_t,const std::string*,std::string&);
public:
 PlayerManageCharactersOwnerV70(PlayerManagerOwnerV1& m,PlayerManageServicesV70 s):manager_(m),services_(std::move(s)){}
 bool update(std::string&);
};
}


