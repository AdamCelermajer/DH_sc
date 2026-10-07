#include "player_spawn_metadata_v4.hpp"
#include "canonical_player_facet_v3.hpp"
#include "character_set_position_v7.hpp"
#include "../../../port/android-native/app/src/main/cpp/renderer_player_canonical_metadata_v4.inc"
#include <cassert>
#include <cstring>
#include <iostream>
int main(){
 std::string error;dh2::actor::RuntimeState runtime{};
 runtime.subobjects.position[0]=83;runtime.subobjects.local_bounds[0]=-74;
 runtime.object.motion.position[2]=123;runtime.controller.position[1]=42;
 auto before=runtime;
 auto metadata=PlayerCanonicalMetadataLiveV4::prepare(0,error);
 assert(metadata&&error.empty());assert(metadata->position_fields_v7().constructed);
 assert(!metadata->position_fields_v7().physical2dc&&!metadata->position_fields_v7().attached2e0);
 assert(std::memcmp(&before,&runtime,sizeof(runtime))==0);
 auto same=metadata;metadata->position_fields_v7().attached2e0=0x100000123ull;
 assert(metadata->publish_physical2dc_v20(0x100000456ull,error));
 assert(same->position_fields_v7().attached2e0==0x100000123ull);
 assert(same->position_fields_v7().physical2dc==0x100000456ull);
 assert(std::memcmp(&before,&runtime,sizeof(runtime))==0);
 assert(metadata->publish_physical2dc_v20(0,error));
 assert(!same->position_fields_v7().physical2dc&&same->position_fields_v7().attached2e0==0x100000123ull);
 assert(!metadata->position_fields_v7().adopt(0,0,error));
 assert(same->position_fields_v7().attached2e0==0x100000123ull);
 std::cout<<"Player camera pointer-only fresh/retained metadata PASS12 guards\n";
}
