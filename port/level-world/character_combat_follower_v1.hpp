#pragma once
#include "character_world_target_owner_v1.hpp"
namespace dh2::character::skills {
// Whole Character::IsFollower3a307c through GetCharAIId/GetCharAI/GetCharType.
// Borrow SAME canonical World character resolved[1] and retained AI table.
//1 delivered,-1 required actor/property/AI producer missing; output preserved.
int character_combat_follower_v1(bool*,const WorldTargetActorBorrowV1&,const data::AiTables&);
}
