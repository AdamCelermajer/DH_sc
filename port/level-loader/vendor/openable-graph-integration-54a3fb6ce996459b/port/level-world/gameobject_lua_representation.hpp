#pragma once
#include "../script-runtime/script_object_bridge.h"
#include "character_target_bindings.hpp"
namespace dh2::gameobject_lua {
enum Class : std::uint32_t {game_object=0,character=1};
// This is original C++ UserData virtual class identity, not AIProps.Type.
// Native caller must supply the genuine actor class/identity and lifetime.
extern "C" int dh2_gameobject_lua_type(const char**,std::uint32_t);
extern "C" int dh2_gameobject_lua_methods(const dh2_script_object_method**,std::uint32_t*,std::uint32_t);
extern "C" int dh2_gameobject_lua_get_id(dh2_script_value*,std::uint32_t*,std::uintptr_t);
extern "C" int dh2_gameobject_lua_get_target(dh2_script_value*,std::uint32_t*,const dh2::character::TargetState48*);
}
