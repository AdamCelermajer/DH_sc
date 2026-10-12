#pragma once
#include "character_state.hpp"
#include <cstdint>
namespace dh2::character {
struct PlayerInjureBorrowV7 {
 std::uintptr_t character{};
 State* state{};
 float* source_gate_14fc{}; // Actual Character ctor -1.f, set to3000 by source.
};
struct PlayerInjureServicesV7 {
 void* context{};
 int(*is_player)(void*,std::uintptr_t,bool*){};
 int(*animation_table)(void*,std::uintptr_t,int*){};
 // Actual CharacterAnimation row field15 at3c5e28. Out ofrange is delivered
 // found=false; missing table authority is a required-service failure.
 int(*injure_animation)(void*,int,bool* found,int*){};
 int(*constant)(void*,const char* key,const char* group,int*){};
 int(*stance)(void*,std::uintptr_t,int*){};
 int(*event)(void*,int,std::uintptr_t){};
 int(*transition)(void*,int,int,std::uintptr_t){};
 int(*debug)(void*,const char*){};
 int(*set_animation)(void*,int){};
 int(*cancel_sneaking)(void*,std::uintptr_t){};
 // Original blur calls Cmd_LookAt(actual Character+408), including null.
 int(*look_at_source_408)(void*,std::uintptr_t){};
};
// Complete SM_SetInjureState3c5d84 and CSInjured focus/blur native bodies.
// The SAME CharacterStateOwner owns outer transitions/events and animator.
int player_set_injure_v7(const PlayerInjureBorrowV7*,std::uintptr_t attacker,
 bool direct_transition,const PlayerInjureServicesV7*);
int player_injure_focus_v7(const PlayerInjureBorrowV7*,const PlayerInjureServicesV7*);
int player_injure_blur_v7(const PlayerInjureBorrowV7*,const PlayerInjureServicesV7*);
// CSInjured OnEvent3c0044 and OnUpdate3c0040 are source bx-lr bodies.
int player_injure_empty_v7(std::uint32_t original_function);
// Character::Update3ac058/3ac894 uses unsigned Application::GetDt ->float,
// subtracts only while positive, and does not clamp the reached negative value.
int player_injure_gate_tick_v7(float* source_gate_14fc,std::uint32_t dt_ms);
}
