#include "authored_monster_session.hpp"
#include "../../../level-world/character_script_session.hpp"
#include "../../../level-world/character_script_session_v3.hpp"
// Compile-time integration checks against both real retained Session APIs.
template bool dh::foundation::enemy_ai::loan_authored_monster_callback(
 dh2::character::CharacterScriptSession&,const dh::foundation::enemy_ai::LiveEnemyBorrow&,
 dh::foundation::enemy_ai::SelectedMonsterCallback&,std::string&);
template bool dh::foundation::enemy_ai::loan_authored_monster_callback(
 dh2::character::CharacterScriptSessionV3&,const dh::foundation::enemy_ai::LiveEnemyBorrow&,
 dh::foundation::enemy_ai::SelectedMonsterCallback&,std::string&);
