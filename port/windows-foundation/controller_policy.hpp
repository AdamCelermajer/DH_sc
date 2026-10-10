#pragma once
#include "actor_movement.hpp"
#include "asset_catalog.hpp"
#include <string>
namespace dh::foundation {
// Ground step/slope rules are explicitly supplied until original navigation
// equivalents have been verified. Body bounds and turn rate are bound separately.
bool load_controller_policy(const AssetCatalog&,const std::string& uri,
                            ActorMovementConfig&,std::string& provenance,std::string& error);
}
