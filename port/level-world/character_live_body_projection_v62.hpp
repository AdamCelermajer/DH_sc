#pragma once
#include "character_live_body_projection_v6.hpp"
namespace dh2::character {
// Generic source IsPlayer uses the same AiProps and actual Character name;
// supports the player branch without inventing an NPC ScriptSession.
bool character_live_body_projection_v62(RetainedCharacterActorV1&,const world::RetainedGameObjectVisualV1&,const physical::NpcBodyRequest&,physical::NpcBodyProjection&,std::string&);
}
