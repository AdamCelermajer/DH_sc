#include "character_combat_follower_v1.hpp"
namespace dh2::character::skills {
int character_combat_follower_v1(bool* out,const WorldTargetActorBorrowV1& actor,const data::AiTables& ai){
 if(!out||!actor.identity||!actor.character||actor.character->identity!=actor.identity||!actor.character->resolved)return -1;
 const auto* row=data::ai_props(ai,actor.character->resolved[1]);if(!row)return -1;
 *out=row->type==2;return 1;
}
}
