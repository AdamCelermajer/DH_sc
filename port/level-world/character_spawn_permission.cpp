#include "character_spawn_permission.hpp"
extern "C" int dh2_character_pre_spawn_permission(std::uint32_t* out,const dh2::character::SpawnPermission16* s){
 if(!out||!s||s->reserved||s->auto_spawn>255)return -1;
 const auto a=reinterpret_cast<std::uintptr_t>(out),b=reinterpret_cast<std::uintptr_t>(s);
 if(a<=b?b-a<4:a-b<16)return -1;
 *out=s->group?0:s->auto_spawn;return 1;
}
