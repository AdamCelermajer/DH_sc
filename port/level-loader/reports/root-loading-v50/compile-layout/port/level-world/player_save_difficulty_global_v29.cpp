#include "player_save_difficulty_global_v29.hpp"
namespace dh2::player {
std::shared_ptr<PlayerSaveDifficultyGlobalV29> process_player_save_difficulty_global_v29(){
 static const auto source_global=std::shared_ptr<PlayerSaveDifficultyGlobalV29>(new PlayerSaveDifficultyGlobalV29());
 return source_global;
}
bool character_game_difficulty_v29(const CharacterSaveDifficultyBorrowV29& borrow,
 std::int32_t& out,std::string& error){
 error.clear();if(!borrow.character_owner||!borrow.character||!borrow.save14e8){error="Required actual Character SG_GetGameDifficulty14e8 field";return false;}
 if(!*borrow.save14e8){out=-1;return true;}
 if(!borrow.actual_global){error="Required same process PlayerSavegame difficulty global";return false;}
 out=borrow.actual_global->value();return true;
}
}
