#pragma once
#include "player_savegame_v1.hpp"
#include <functional>
namespace dh2::data {
struct PlayerSaveQuestSyncServicesV3 {
 std::shared_ptr<void> owner;
 // Actual GetOnline()->byte5, shared with existing gameplay platform.
 std::function<bool(bool&,std::string&)> online;
 std::function<bool(std::uintptr_t character,bool&,std::string&)> is_local_player;
 std::function<bool(std::string&)> nonlocal_assert;
 std::function<bool(bool&,std::string&)> local_hosting;
 // Whole original StreamBufferC1/expand1000/PackQuests/current difficulty/
 // messaging create/copy/send branch. Keeps actual StreamBuffer alive until
 // subsequent source readiness write; destructor is delivered separately.
 std::function<bool(PlayerSavegameV1&,std::string&)> pack_send_volatile_quests;
 std::function<bool(std::string&)> destroy_quest_stream;
 std::function<bool(std::uintptr_t& identity,std::int32_t& phase130,std::string&)> current_level;
 std::function<bool(std::uint8_t&,std::string&)> hosting_ready710;
 // Whole PlayerManager.ReceiveQuestSync; may invoke this owner's unpack route.
 std::function<bool(PlayerSavegameV1&,std::string&)> receive_quest_sync;
};
struct PlayerSaveQuestUnpackServicesV3 {
 std::shared_ptr<void> owner;
 std::function<bool(PlayerSavegameV1&,std::string&)> initialize_volatile_quests;
 std::function<bool(std::string&)> seek_stream_zero;
 std::function<bool(PlayerSavegameV1&,bool compile_flag,std::string&)> unpack_volatile_quests;
 std::function<bool(PlayerSavegameV1&,bool,std::string&)> compile_volatile_quests;
};
// Same retained Save. Native readiness is only set at original source writes;
// no profile-null proxy, campaign unlock, second Save or manual readiness API.
class PlayerSaveQuestSyncOwnerV3 {
 PlayerSavegameV1& save_;
public:
 explicit PlayerSaveQuestSyncOwnerV3(PlayerSavegameV1& save):save_(save){}
 bool try_sync(const PlayerSaveQuestSyncServicesV3&,std::string&);
 bool unpack_sync(const PlayerSaveQuestUnpackServicesV3&,std::string&);
};
}
