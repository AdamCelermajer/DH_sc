#pragma once
#include "character_property_bindings.hpp"
namespace dh2::character::sneaking {
// Full source Skills runtime row (including header). Flags at+1c and skill
// activity type at+48 are consumed without interpreting other authored fields.
struct Skill76 {std::uint32_t words[19];};
struct List16 {const std::int32_t* ids;std::uint32_t count,reserved;};
struct Tables32 {const List16* lists;std::uint32_t list_count,reserved0;
 const Skill76* skills;std::uint32_t skill_count,reserved1;};
struct Character48;
struct AI24 {Character48* owner;const std::uintptr_t* scripts;std::uint32_t count,reserved;};
struct Character48 {std::uintptr_t identity;PropertySheet16 resolved;const Tables32* tables;
 AI24* ai;std::uint8_t changed415;std::uint8_t reserved[7];};
enum Operation:std::uint32_t {is_player=0,delete_buff=1,skill_check_active=2,skill_pre=3};
struct Request24 {std::uint32_t operation,index;std::uintptr_t receiver;
 std::uint32_t argument,reserved;};
struct Services16 {void* context;int(*invoke)(void*,const Request24*,std::uint32_t*);};
static_assert(sizeof(Skill76)==76&&sizeof(List16)==16&&sizeof(Tables32)==32);
static_assert(sizeof(Character48)==48&&sizeof(AI24)==24&&sizeof(Request24)==24&&sizeof(Services16)==16);
// All records/arrays are borrowed; retain backing through synchronous callbacks.
// Callback may mutate live projections/reenter, but cannot destroy held objects.
// delete_buff is required source PROPS_DelBuff(0x92,null); its effects must be
// delivered genuinely. Active/Pre are actual skill-script services, not accepted
// defaults. The resolved payload is source Character+ff8, all224 raw words.
}
// 0 complete,1 malformed borrowed projection,2 provider failure. Rejections may
// follow already-delivered source effects; no rollback or global state reset.
extern "C" unsigned dh2_character_cancel_sneaking(dh2::character::sneaking::Character48*,
 const dh2::character::sneaking::Services16*);
extern "C" unsigned dh2_character_cancel_skill(dh2::character::sneaking::AI24*,std::uint32_t,
 const dh2::character::sneaking::Services16*);
