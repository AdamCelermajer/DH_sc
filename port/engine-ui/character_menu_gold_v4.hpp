#pragma once
#include "character_menu_queries_owner_v1.hpp"
namespace dh2::ui {
struct CharacterMenuGoldServicesV4 {
 std::shared_ptr<void> owner;
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> player;
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> gold;
 // Same StringManager::parse508ef4; original literal8c9040 is ^d.
 std::function<bool(const char*,std::int32_t,std::string&,std::string&)> parse_integer;
};
bool character_menu_gold_call_v4(CharacterMenuCallV1&,const CharacterMenuGoldServicesV4&,std::string&);
}
