#pragma once
#include "character_body_config.hpp"
namespace dh2::physical {
struct ItemBodyInputV2 {
 void* physical{};float absolute_bounds[4]{},position[2]{};
 bool no_collisions{};
};
// Exact PhysicalObjectC2 branch selected by ItemObject.InitAgain3ec268:
// false,true,true,false,-3,0x40,4,0 -> circle, sensor, movable, nonbullet.
// This produces native definitions only; original source assignment/Debug/PF
// and POItem collision dispatch belong to the actual retained owner.
bool item_body_config_v2(CharacterBodyConfig&,const ItemBodyInputV2&)noexcept;
}
