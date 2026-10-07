#pragma once
#include "navigation_motion.hpp"
#include <string>
namespace dh2::fx {
// AnimatedFX::SyncIrrData's actual PFWorld query. Normal is an in/out value:
// misses preserve it, and the source deliberately ignores the query result.
bool character_fx_floor_query_v3(const navigation::CollisionWorld* same_world,
                                const float position[3],float normal[3],
                                std::string& error);
}
