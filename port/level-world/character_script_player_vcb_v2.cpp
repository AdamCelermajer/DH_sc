#include "character_script_player_vcb_v2.hpp"
#include "character_script_virtual.hpp"
extern "C" int dh2_character_script_player_vcb_v2(std::uint32_t* flags,const dh2_script_aliases* aliases){
 if(!flags||!aliases||reinterpret_cast<std::uintptr_t>(flags)%alignof(std::uint32_t))return -1;
 if(dh2_character_script_init_vcb(flags,aliases,0))return -1;
 const auto base=*flags;
 *flags=base|(dh2_script_alias_contains(aliases,"OnKill")?0x400u:0u);
 return 0;
}
