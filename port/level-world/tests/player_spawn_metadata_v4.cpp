#include "player_spawn_metadata_v4.hpp"
#include <cassert>
#include <iostream>
#include <limits>
int main(){
 dh2::player::PlayerSpawnMetadataV4 out;std::string error;
 const std::int32_t values[]={0,1,-1,17,std::numeric_limits<std::int32_t>::min(),std::numeric_limits<std::int32_t>::max()};
 const char* names[]={"PlayerCharacter_0","PlayerCharacter_1","PlayerCharacter_-1","PlayerCharacter_17","PlayerCharacter_-2147483648","PlayerCharacter_2147483647"};
 for(unsigned i=0;i<6;++i){assert(dh2::player::player_spawn_metadata_v4(values[i],out,error));assert(out.name==names[i]);
  assert(out.factory->original_address==0x340800&&std::string(out.factory->name)=="Character");
  assert(out.archetype==out.factory->name&&out.room==-1&&out.network&&out.deferred);}
 std::cout<<"player source Spawn metadata PASS6; no Spawn/AddCharacter success claimed\n";
}
