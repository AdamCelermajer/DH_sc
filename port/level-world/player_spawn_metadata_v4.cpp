#include "player_spawn_metadata_v4.hpp"
#include <cstring>
namespace dh2::player {
bool player_spawn_metadata_v4(std::int32_t actual_internal_input,PlayerSpawnMetadataV4& out,std::string& error){
 const world::CanonicalFactoryEntryV1* factory=nullptr;
 for(const auto& entry:world::canonical_factories_v1())if(!std::strcmp(entry.name,"Character")){factory=&entry;break;}
 if(!factory||factory->original_address!=0x340800){error="required original Character factory catalog producer";return false;}
 PlayerSpawnMetadataV4 result;
 // Source CString::Printf30eae4 receives PlayerCharacter_%d and the original
 // internal input. std::to_string preserves the signed decimal domain.
 result.name="PlayerCharacter_"+std::to_string(actual_internal_input);
 result.factory=factory;result.archetype=factory->name;result.room=-1;
 result.deferred=true;result.network=true;out=std::move(result);return true;
}
}
