#pragma once
#include <cstdint>
namespace dh2::target_search {
// Borrowed projections of the source object fields read directly by Search.
// identity is the actual owner/backend key, never a distance-derived identifier.
struct Object48 {
 std::uintptr_t identity;
 float position[3], target_position[3], rotation;
 std::int32_t character_word1310, character_word1314;
 // Historical `visible` name represents source ObjectBase enabled byte +0x8a.
 std::uint8_t visible, zoned, in_zone, has_target_position;
};
struct Entry16 { Entry16* next; Object48* object; };
struct Room16 { Room16* next; Entry16* objects; };
struct Registry8 { Room16* rooms; }; // intrusive sentinel; empty lists point to themselves
struct Target24 { std::uintptr_t identity; float distance, angle; std::uint32_t flags, reserved; };
struct List48 {
 Target24* heap; std::uint32_t count, capacity;
 Object48* owner; Object48* reference_character;
 std::uint32_t sort, character_flags, object_filter;
};
using List40 = List48; // source-facing historical name retained for callers
// A borrowed source IObjectList view. Cursor callbacks must read the canonical
// ObjectManager/CharacterList owners live; no list nodes are copied here.
struct ObjectListMethods {
 void* context;
 int (*reset)(void*);
 int (*at_end)(void*,std::uint32_t*);
 int (*get)(void*,Object48**);
 int (*next)(void*);
};
enum Service : std::uint32_t {
 resolve_character=1, is_player, is_dead, is_interactive, interaction_type,
 interaction_radius, is_zonable, is_enemy, melee_radius, is_character,
 is_game_object_valid, is_neutral, is_friend, is_merchant, is_cleaner,
 has_character_flag_2fa, character_state
};
struct Request24 { std::uint32_t service, reserved; std::uintptr_t subject, other; };
// resolve_character returns a borrowed Object48* in word; other boolean/integer
// services return word. Radius services return number. Callbacks are synchronous.
struct Response16 { std::uintptr_t word; float number; std::uint32_t reserved; };
struct Services16 { void* context; int (*invoke)(void*,const Request24*,Response16*); };
static_assert(sizeof(Object48)==48 && sizeof(Target24)==24 && sizeof(List48)==48);
static_assert(sizeof(ObjectListMethods)==40);
static_assert(sizeof(Entry16)==16 && sizeof(Room16)==16 && sizeof(Request24)==24);
// This domain is the original melee caller: character flags1/object filter0.
// 0 completed, 1 malformed entry (atomic), 2 invalid provider/topology or capacity
// exhausted after source-visible effects. Caller guarantees borrowed lifetimes.
extern "C" int dh2_target_list_init(List40*,Target24*,std::uint32_t,Object48*,std::uint32_t,const Services16*);
// Explicit source constructor form. Preserve the full source int values for
// (character_flags, object_filter, sort) without packing/truncation.
extern "C" int dh2_target_list_init_filtered(List48*,Target24*,std::uint32_t,Object48*,
 std::uint32_t character_flags,std::uint32_t object_filter,std::uint32_t sort,
 const Services16*);
extern "C" int dh2_target_search(List48*,const Registry8*,float radius,float cone,const Services16*);
// GameObject::_TargetListSearch supplies an explicit copied center while the
// reference Character, heading and predicate identity remain the same owner.
extern "C" int dh2_target_search_at(List48*,const Registry8*,float radius,float cone,
 const float* origin_xyz,const Services16*);
extern "C" int dh2_target_search_object_list(List48*,const ObjectListMethods*,
 float radius,float cone,const Services16*);
extern "C" int dh2_target_search_object_list_at(List48*,const ObjectListMethods*,
 float radius,float cone,const float* origin_xyz,const Services16*);
extern "C" int dh2_target_pop(List48*,Target24*);
}
