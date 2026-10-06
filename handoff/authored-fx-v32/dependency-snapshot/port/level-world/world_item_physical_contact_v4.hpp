#pragma once
#include "world_item_graph_v3.hpp"
#include "navigation_avoidance.hpp"
namespace dh2::character {
// Read-only source physical virtual query projection for a mixed native world.
// This prevents interpreting POItem context as a player/decor BodyOwner.
bool world_item_physical_contact_v4(WorldItemGraphV3&,navigation::PhysicalContact&,std::string&);
}
