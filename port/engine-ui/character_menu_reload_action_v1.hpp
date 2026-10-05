#pragma once
#include "character_menu_reload_v1.hpp"
#include "character_menu_queries_owner_v1.hpp"
namespace dh2::ui {
struct CharacterMenuReloadActionGraphV1 {
 std::shared_ptr<void> owner;
 // Same original NativeGetPlayerChar(index,false), including genuine null.
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> player;
 // Source external __aeabi_d2iz boundary. Must implement its actual semantics;
 // the menu adapter neither casts an out-of-range double nor invents policy.
 std::function<bool(double,std::int32_t&,std::string&)> player_index;
 // Complete source reload services over THAT selected Character's graph.
 MenuReloadServices16V1 reload{};
};
class CharacterMenuReloadActionV1 {
 CharacterMenuReloadActionGraphV1 graph_;
public:
 explicit CharacterMenuReloadActionV1(CharacterMenuReloadActionGraphV1);
 // Whole original NativeReloadSkills43dbb8. Exactly one argument is coerced;
 // every other arity selects index0 without touching any argument. Preserves
 // the prior AS result, including genuine null-player and failed-prefix paths.
 bool dispatch(const char*,CharacterMenuCallV1&,std::string&)const;
};
}
