#pragma once
#include "savegame_stream_v2.hpp"
#include "../game-data/quest_savegame_v1.hpp"
#include <functional>
#include <memory>
namespace dh2::level {
struct PlayerSaveLevelStatesBorrowV45 {
 std::shared_ptr<void> receiver;
 const std::vector<std::string>* level_names{};const std::vector<std::string>* map_names{};
 const std::array<std::vector<std::int32_t>,3>* levels{};
 const std::array<std::vector<std::int32_t>,3>* maps{};
};
bool player_save_level_states_v45(const PlayerSaveLevelStatesBorrowV45&,SavegameStreamV2&,std::string&);
bool player_save_fast_travel_v45(const std::array<std::uint64_t,3>* actual_bitsets17c,SavegameStreamV2&,std::string&);
struct QuestSaveCollectionBorrowV45 {
 std::shared_ptr<void> receiver;
 const std::array<std::vector<std::uintptr_t>,3>* quests{};
 const data::SavedQuestProgressV1* progress{};
 // Exact retained Quest::_saveQuestData (state +condition/objective/list).
 std::function<bool(std::uintptr_t,SavegameStreamV2&,std::string&)> save_quest;
};
bool quest_save_collection_v45(const QuestSaveCollectionBorrowV45&,SavegameStreamV2&,std::string&);
// Source mode&1 picks regular at+b8; otherwise volatile at+118. Source mode
// belongs the SAME PlayerSaveLoad/Write owner, not an online default.
bool player_save_quests_v45(std::int32_t actual_mode178,const QuestSaveCollectionBorrowV45& regular,
 const QuestSaveCollectionBorrowV45& volatile_quests,SavegameStreamV2&,std::string&);
}
