#pragma once
#include <cstdint>
namespace dh2::target_search {
// Borrowed projections of the source object fields read directly by Search.
// identity is the actual owner/backend key, never a distance-derived identifier.
struct Object48 {
 std::uintptr_t identity;
 float position[3], target_position[3], rotation;
 std::int32_t character_word1310, character_word1314;
 std::uint8_t visible, zoned, in_zone, has_target_position;
};
struct Entry16 { Entry16* next; Object48* object; };
struct Room16 { Room16* next; Entry16* objects; };
struct Registry8 { Room16* rooms; }; // intrusive sentinel; empty lists point to themselves
struct Target24 { std::uintptr_t identity; float distance, angle; std::uint32_t flags, reserved; };
struct List40 {
 Target24* heap; std::uint32_t count, capacity;
 Object48* owner; Object48* reference_character;
 std::uint32_t sort, reserved; // 0: no-sort, 1: closest, 2: frontal
};
enum Service : std::uint32_t {
 resolve_character=1, is_player, is_dead, is_interactive, interaction_type,
 interaction_radius, is_zonable, is_enemy, melee_radius, is_character,
 is_merchant_v108,is_cleaner_v108,talk_required2fa_v108,is_neutral_v108,
 is_friend_v108,machine_state_v108
};
struct Request24 { std::uint32_t service, reserved; std::uintptr_t subject, other; };
// resolve_character returns a borrowed Object48* in word; other boolean/integer
// services return word. Radius services return number. Callbacks are synchronous.
struct Response16 { std::uintptr_t word; float number; std::uint32_t reserved; };
struct Services16 { void* context; int (*invoke)(void*,const Request24*,Response16*); };
static_assert(sizeof(Object48)==48 && sizeof(Target24)==24 && sizeof(List40)==40);
static_assert(sizeof(Entry16)==16 && sizeof(Room16)==16 && sizeof(Request24)==24);
// This domain is the original melee caller: character flags1/object filter0.
// 0 completed, 1 malformed entry (atomic), 2 invalid provider/topology or capacity
// exhausted after source-visible effects. Caller guarantees borrowed lifetimes.
extern "C" int dh2_target_list_init(List40*,Target24*,std::uint32_t,Object48*,std::uint32_t,const Services16*);
extern "C" int dh2_target_search(List40*,const Registry8*,float radius,float cone,const Services16*);
//Whole generic _IsCharacterValid4a1ab8/_IsGameObjectValid4a1950 policies.
//Does not change the frozen melee List40 layout or its flags1/type0 default.
extern "C" int dh2_target_search_policy_v108(List40*,const Registry8*,float radius,float cone,
 std::uint32_t character_flags,std::uint32_t object_type,const Services16*);
extern "C" int dh2_target_pop(List40*,Target24*);
}
