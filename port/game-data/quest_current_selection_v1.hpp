#pragma once
#include "player_savegame_v1.hpp"
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

namespace dh2::data::quest_current_selection_v1 {
inline bool is_current(const PlayerSavegameV1& save,std::int32_t difficulty,
                       std::int32_t quest_id) noexcept {
    if(difficulty<0||difficulty>=3)return false;
    // Character::SG_GetCurrentQuest reads QuestSavegame +0x2c, the same field
    // that SG_SetCurrentQuest writes. word_38 is a distinct QEST field.
    return save.source_quest_log_b8().word_2c[std::size_t(difficulty)]==quest_id;
}

// Character::SG_GetCurrentQuest calls QuestSavegame::SG_VerifyCurrentQuest
// before returning +0x2c. A stale or absent selection falls back to the first
// active Primary quest in source vector order, and updates that Save word.
template<class QuestRows>
inline std::int32_t verify_current(PlayerSavegameV1& save,
                                   std::int32_t difficulty,
                                   const QuestRows& rows,
                                   std::int32_t primary_priority) noexcept {
    if(difficulty<0||difficulty>=3)return -1;
    auto& selected=save.source_quest_log_b8().word_2c[std::size_t(difficulty)];
    const auto current=selected;
    if(current>=0&&std::size_t(current)<rows.size()&&rows[std::size_t(current)].state==6)
        return current;
    for(std::size_t ordinal=0;ordinal<rows.size();++ordinal){
        const auto& row=rows[ordinal];
        if(row.state==6&&row.priority==primary_priority){
            selected=static_cast<std::int32_t>(ordinal);
            return selected;
        }
    }
    return current;
}
inline bool set(PlayerSavegameV1& save,bool online,std::int32_t difficulty,
                std::int32_t quest_id,std::string& error) {
    if(online){error="Online Quest selection is unavailable";return false;}
    if(difficulty<0||difficulty>=3){error="Quest difficulty is outside the source Save range";return false;}
    save.source_quest_log_b8().word_2c[std::size_t(difficulty)]=quest_id;
    error.clear();return true;
}

// The native UI adapter persists this completed mutation through the same
// PlayerSavegame transport. A rejected source setter must not reach SG_Save.
template<class Persist>
inline bool set_and_persist(PlayerSavegameV1& save,bool online,
                            std::int32_t difficulty,std::int32_t quest_id,
                            Persist&& persist,std::string& error) {
    if(online){error="Online Quest selection is unavailable";return false;}
    if(difficulty<0||difficulty>=3){error="Quest difficulty is outside the source Save range";return false;}
    if(save.source_quest_log_b8().word_2c[std::size_t(difficulty)]==quest_id){
        error.clear();return true;
    }
    if(!set(save,online,difficulty,quest_id,error))return false;
    return persist(save,error);
}
}
