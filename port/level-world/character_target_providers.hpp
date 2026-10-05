#pragma once
#include "character_combat_queries.hpp"
#include <cstdint>
namespace dh2::target_providers {
struct Character32 {
 std::uintptr_t identity; const character::CombatProperties896* properties;
 const char* name; std::uint32_t flags520;
 std::uint8_t dead1449,disabled81,visible8a,interactive415;
};
struct Types16 { const std::int32_t* types;std::uint32_t count,reserved; };
enum Query : std::uint32_t { char_type=1,is_monster,is_faerie,is_summoned,is_player,is_interactive,interaction_type,is_zonable,is_character,is_dead };
enum Service : std::uint32_t { virtual_dead=1,virtual_player,ai_friend,ai_enemy,virtual_character,live_state_flags };
struct Request24 {std::uint32_t service,reserved;std::uintptr_t subject,other;};
struct Services16 {void* context;int (*invoke)(void*,const Request24*,std::uintptr_t* result);};
struct Handle16 {std::int32_t key;std::uint32_t frame;std::uintptr_t cached;};
struct Record16 {std::int32_t key;std::uint32_t reserved;std::uintptr_t object;};
// Logical projection of source map operator[]: signed unique keys, absent keys
// insert an empty record. Caller owns stable object identities/shared handles.
struct Registry24 {Record16* records;std::uint32_t count,capacity,frame,reserved;};
static_assert(sizeof(Character32)==32&&sizeof(Types16)==16&&sizeof(Handle16)==16&&sizeof(Record16)==16&&sizeof(Registry24)==24);
// 0 success, 1 malformed input atomic, 2 invalid provider/capacity after effects.
extern "C" int dh2_character_target_query(std::int32_t*,std::uint32_t,Character32*,Character32*,const Types16*,const Services16*);
// Copies actual resolved Special_Sneak198 / Special_Sneak_Detection199 words.
extern "C" int dh2_character_sneak_fields(std::int32_t out[2],const character::CombatProperties896*);
extern "C" int dh2_gameobject_interaction_radius(float*,const float aabb[6]);
// Base GameObject policies: interactive0/type-1/IsCharacter0/zonable(byte^1).
extern "C" int dh2_gameobject_target_query(std::int32_t*,std::uint32_t,std::uint8_t byte2ed);
// GetHandle stamps shared frame, copies it, resolves through source cache/map,
// then invokes IsCharacter synchronously. Refreshed cached fields belong only
// to the local copy; source shared cached pointer is never overwritten here.
extern "C" int dh2_target_handle_character(std::uintptr_t*,Handle16* local,Handle16* shared,Registry24*,const Services16*);
}
