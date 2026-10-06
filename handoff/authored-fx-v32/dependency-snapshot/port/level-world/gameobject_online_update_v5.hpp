#pragma once
#include "canonical_gameobject_base_owner_v1.hpp"
namespace dh2::world {
struct GameObjectOnlineUpdateServicesV5 {
 void* context{};
 bool(*online_byte5)(void*,bool&,std::string&){};
 bool(*hosting)(void*,bool&,std::string&){};
 bool(*not_owned_virtual54)(void*,std::uintptr_t,bool&,std::string&){};
};
// Whole RequireOnlineUpdate38b8b8. Constructor-null NetStruct100 and actual
// offline mode return naturally; no invented offline value is supplied.
bool gameobject_require_online_update_v5(CanonicalGameObjectBaseOwnerV1&,
 const GameObjectOnlineUpdateServicesV5&,std::string&);
}
