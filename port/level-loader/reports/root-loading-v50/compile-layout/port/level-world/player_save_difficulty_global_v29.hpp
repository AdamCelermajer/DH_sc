#pragma once
#include "../game-data/player_savegame_v1.hpp"
#include <memory>
namespace dh2::player {
// Original PlayerSavegame::m_difficultyLevel at9a6060 is one process BSS word,
// initially zero. It is distinct from NativeSetCurrentDifficulty's separate
// Application+4c subowner+0c and from Level's constructor arguments.
class PlayerSaveDifficultyGlobalV29 {
 std::int32_t difficulty_{};
 PlayerSaveDifficultyGlobalV29()=default;
 friend std::shared_ptr<PlayerSaveDifficultyGlobalV29> process_player_save_difficulty_global_v29();
public:
 std::int32_t value()const noexcept{return difficulty_;}
 // Exact PDFL first-word delivery, including signed/unvalidated source words.
 void store_from_pdfl(std::int32_t value)noexcept{difficulty_=value;}
};
std::shared_ptr<PlayerSaveDifficultyGlobalV29> process_player_save_difficulty_global_v29();
struct CharacterSaveDifficultyBorrowV29 {
 std::shared_ptr<void> character_owner;std::uintptr_t character{};
 const std::uintptr_t* save14e8{};
 std::shared_ptr<PlayerSaveDifficultyGlobalV29> actual_global;
};
bool character_game_difficulty_v29(const CharacterSaveDifficultyBorrowV29&,
 std::int32_t&,std::string&);
}
