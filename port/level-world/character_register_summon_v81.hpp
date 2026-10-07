#pragma once
#include "../game-data/data.hpp"
#include "../script-runtime/script_runtime.h"
#include <functional>
#include <memory>
namespace dh2::character {
struct RegisterSummonBindingsV81 {
 std::shared_ptr<void> cache;
 const data::CharacterTable* characters{}; //SAME retained Session design borrow
 std::function<bool(std::int32_t,std::uint32_t,std::string&)> add;
};
}
extern "C" int dh2_character_register_summon_v81(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
