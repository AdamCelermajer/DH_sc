#pragma once
#include "../game-data/properties.hpp"
#include <cstdint>
namespace dh2::character {
struct HitPlayerTailBorrowV7 {
 std::uintptr_t character{},captured_main_player{};
 data::PropertyView* properties{};
 std::uint8_t* source_low_health_1448{};
 std::uint8_t* source_tutorial_2d{};
};
struct HitPlayerTailServicesV7 {
 void* context{};
 int(*online)(void*,bool*){};
 int(*is_player)(void*,std::uintptr_t,bool*){};
 int(*difficulty)(void*,std::uintptr_t,int*){};
 // Genuine ScriptManager lookup(name,true), optional StartScript(id,-1,false).
 int(*script_id)(void*,const char*,int*){};
 int(*start_script)(void*,int,int,bool){};
 int(*start_update_job)(void*){};
 // Source exact name search and VoxSoundManager::Play(index,false,0,0,false).
 int(*sound_index)(void*,const char*,int*){};
 int(*play_sound)(void*,int,bool,int,int,bool){};
};
// Player-only HitFor continuation after common HP/Kill processing and genuine
// IsPlayer. Self/self DoT then bypasses the distinct-attacker trophy branch.
// Borrows actual Character/Application bytes. Never owns them or changes HP.
// 1 complete, -1 malformed, -2 reached missing/failed required service.
int hit_player_tail_v7(const HitPlayerTailBorrowV7*,const HitPlayerTailServicesV7*);
}
