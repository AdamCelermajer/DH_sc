#pragma once
#include "character_state_owner.hpp"
#include "../game-data/ai.hpp"
#include "../game-data/animation_tables.hpp"
#include "../game-data/properties.hpp"
namespace dh2::character {
struct KnockbackReactionBorrowV1 {
 NativeFsm24* fsm{};data::PropertyView* properties{};
 const data::AiTables* ai{};const data::AnimationTables* animations{};
};
struct KnockbackReactionServicesV1 {
 void* context{};
 int(*constant)(void*,const char*,const char*,int*){};
 int(*stance)(void*,std::uintptr_t,int*){};
 int(*event)(void*,int,std::uintptr_t){};
 int(*transition)(void*,int,int,std::uintptr_t){};
 int(*debug)(void*,const char*){};
 int(*animation)(void*,int){};
 int(*filter)(void*,int,int,int,bool){};
 int(*reset_filter)(void*){};int(*pin)(void*){};int(*unpin)(void*){};
 int(*look_at)(void*,std::uintptr_t){};
 int(*cancel_sneaking)(void*){};
 int(*is_dead)(void*,bool*){};int(*stop_animation)(void*){};
 int(*set_dead)(void*,bool,std::uintptr_t,bool){};
};
int character_set_knockback_v1(const KnockbackReactionBorrowV1&,bool great,
 std::uintptr_t attacker,bool direct,const KnockbackReactionServicesV1&);
int character_knockback_focus_v1(const KnockbackReactionBorrowV1&,
 std::uintptr_t attacker,const KnockbackReactionServicesV1&);
int character_knockback_blur_v1(const KnockbackReactionBorrowV1&,const KnockbackReactionServicesV1&);
int character_knockback_event_v1(const KnockbackReactionBorrowV1&,std::uint32_t,const KnockbackReactionServicesV1&);
}
