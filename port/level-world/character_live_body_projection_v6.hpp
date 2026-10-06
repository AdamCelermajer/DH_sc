#pragma once
#include "retained_character_actor_v1.hpp"
#include "retained_gameobject_visual_v1.hpp"
#include "character_npc_body.hpp"
namespace dh2::character {
// InitPhysicalObject after actual VisualObject.ApplyMeshBox and InitPost TRS.
// Consumes SAME published bounds, cached property fields and visual scale.
// Never reparses a model into a competing live scene or estimates draw bounds.
bool character_live_body_projection_v6(RetainedCharacterActorV1&,
 const world::RetainedGameObjectVisualV1&,const physical::NpcBodyRequest&,
 physical::NpcBodyProjection&,std::string&);
}
