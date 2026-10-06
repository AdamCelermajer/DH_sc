#pragma once
#include "character_world_ai_relationship_v1.hpp"
namespace dh2::character::skills {
// Whole AI_IsNeutral3d5a98. This is not !(IsEnemy||IsFriend): original
// handles/assertions/callback order differ, and missing row entries mean true.
extern "C" int dh2_world_ai_neutral_v1(WorldAiOutputV1*,const WorldAiRelationshipV1*,std::uintptr_t,const WorldAiServicesV1*);
}
