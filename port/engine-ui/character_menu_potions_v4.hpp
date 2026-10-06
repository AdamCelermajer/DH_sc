#pragma once
#include "character_menu_queries_owner_v1.hpp"
namespace dh2::ui {
struct CharacterMenuPotionServicesV4 {
 std::shared_ptr<void> owner;
 //43c388 NativeGetPlayerChar(index,false), distinct from36e478 local0,true.
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> player;
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> local_player;
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> count;
 std::function<bool(std::uintptr_t,std::int8_t&,std::string&)> capacity;
 // Actual GetPyCst(StrID,GAMEPLAYMENUS_POTIONS) then StringManager.getString.
 // Optional null is source null string, distinct from a missing service.
 std::function<bool(std::string&,bool& source_null,std::string&)> localized_format;
 // Recovered508ef4 typed varargs parse; a signed integer, not float parseEx.
 std::function<bool(const char*,std::int32_t,std::string&,std::string&)> parse_integer;
};
bool character_menu_potions_call_v4(const char*,CharacterMenuCallV1&,
 const CharacterMenuPotionServicesV4&,bool& handled,std::string&);
// Concrete same HudText varargs continuation; uses existing508ef4 domain.
bool character_menu_potion_integer_text_v4(HudTextV1&,const HudTextEnvironmentV1&,
 const char* actual_format,std::int32_t count,std::string&,std::string&);
// Concrete StrID/getString prefix over that same cache/environment.
bool character_menu_potion_localized_format_v4(HudTextV1&,const HudTextEnvironmentV1&,
 std::string&,bool& source_null,std::string&);
}
