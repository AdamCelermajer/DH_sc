#include "gameobject_lua_representation.hpp"
#include <cstring>
namespace dh2::gameobject_lua {namespace {
#include "gameobject_lua_catalog.inc"
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
}
extern "C" int dh2_gameobject_lua_type(const char** out,std::uint32_t kind){
 if(!aligned(out)||kind>1)return 1;
 *out=kind==character?"Character":"GameObject";return 0;
}
extern "C" int dh2_gameobject_lua_methods(const dh2_script_object_method** out,std::uint32_t* count,std::uint32_t kind){
 if(!aligned(out)||!aligned(count)||kind>1)return 1;
 *out=kind==character?character_methods:gameobject_methods;
 *count=kind==character?sizeof(character_methods)/sizeof(*character_methods):sizeof(gameobject_methods)/sizeof(*gameobject_methods);return 0;
}
extern "C" int dh2_gameobject_lua_get_id(dh2_script_value* out,std::uint32_t* count,std::uintptr_t identity){
 if(!aligned(out)||!aligned(count))return 1;
 std::memset(out,0,sizeof(*out));out->type=DH2_SCRIPT_IDENTITY;out->identity=identity;*count=1;return 0;
}
extern "C" int dh2_gameobject_lua_get_target(dh2_script_value* out,std::uint32_t* count,const dh2::character::TargetState48* state){
 std::uintptr_t identity;
 if(!aligned(out)||!aligned(count)||dh2_character_target_identity(&identity,state))return 1;
 std::memset(out,0,sizeof(*out));out->type=DH2_SCRIPT_SOURCE_OBJECT;out->identity=identity;*count=1;return 0;
}
}
