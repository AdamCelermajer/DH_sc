#pragma once
#include "character_world_npc_state_owner_v1.hpp"
#include "../game-data/animation_tables.hpp"
namespace dh2::character {
struct NpcDeadStateBorrowV1 {
 CharacterWorldNpcStateOwnerV1* machine{};data::PropertyView* properties{};
 const data::AnimationTables* animations{};
 void* context{};int(*constant)(void*,const char*,const char*,int*){};
 int(*stance)(void*,std::uintptr_t,int*){};
};
class NpcDeadStateV1 {
 NpcDeadStateBorrowV1 b_;
 // Sole retained source FSM+38 word, constructor3c1b24 initializes0.
 std::int32_t secondary_animation_{};
 std::string error_;
public:
 explicit NpcDeadStateV1(NpcDeadStateBorrowV1 b):b_(b){}
 int set(bool mode,std::uintptr_t payload,bool force);
 std::int32_t& secondary_animation()noexcept{return secondary_animation_;}
 const std::string& error()const noexcept{return error_;}
};
}
