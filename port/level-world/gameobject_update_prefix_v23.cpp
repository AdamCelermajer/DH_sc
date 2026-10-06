#include "gameobject_update_prefix_v23.hpp"
namespace dh2::world {
bool gameobject_update_begin_v23(const GameObjectUpdatePrefixServicesV23& s,std::string& e){
 if(!s.provider_lease||!s.debug){e="Required retained GameObject.Update Debug/Application provider";return false;}
 const int load=dh2_character_debug_load(s.debug,&s.files);if(load!=1){e="Required GameObject.Update Debug.Load status "+std::to_string(load);return false;}
 std::uint32_t ignored{};const int query=dh2_character_debug_get(&ignored,s.debug,"TraceUpdateGameObjectOnce",&s.files);
 if(query!=1){e="Required GameObject.Update Debug.Get status "+std::to_string(query);return false;}
 CanonicalObjectManagerV1* manager{};if(!s.manager38||!s.manager38(s.context,manager,e)||!manager){if(e.empty())e="Required actual Application ObjectManager38";return false;}
 ++manager->source_update_count58_v23();return true;
}
}
