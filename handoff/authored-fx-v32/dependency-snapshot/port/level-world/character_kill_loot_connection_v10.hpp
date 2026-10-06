#pragma once
#include "character_kill.hpp"
#include "character_loot_drop_v8.hpp"
namespace dh2::character {
struct CharacterKillLootServicesV10 {
 void* context{};
 // GetLootTable3a2fcc reads actual Character+101c directly. This is an
 // initialized Character property/cache field borrow, never an actor-name or
 // arbitrary table fallback. Root supplies its same canonical Character.
 bool(*table101c)(void*,std::uintptr_t,const std::int32_t*&,std::string&){};
};
class CharacterKillLootConnectionV10 {
 skills::CharacterLootDropV8& loot_;CharacterKillLootServicesV10 services_;
public:
 CharacterKillLootConnectionV10(skills::CharacterLootDropV8& loot,CharacterKillLootServicesV10 s):loot_(loot),services_(s){}
 // Only source kill_drop_loot is handled. Invoke from original Kill's ordered
 // callback, never an independent HP/death animation/XP receipt hook.
 bool route(KillActor56&,const KillRequest56&,KillResponse16&,bool& handled,std::string&);
};
}
