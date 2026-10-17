#pragma once

#include "character_aggro_target_search.hpp"

#include <cstdint>

namespace dh2::character::aggro_character_list {

using aggro_search::Character;
using aggro_search::GameObject;
using aggro_search::Services;
using aggro_search::Status;
using aggro_search::TargetInfo;
using aggro_search::TargetList;

// A normalized view of the source std::list<Character*> node. Only the next
// link and value used by CharacterList are represented; this is not a native
// std::list node overlay. The source list is circular and its sentinel has a
// null character value.
struct Entry {
    Entry* next;
    Character* character;
};

// Projection of the stack CharacterList used by CharAI::_UpdateAggro. The
// original object also has an IObjectList vptr; this adapter models the
// sentinel/current/end state consumed by Reset/AtEnd/Get/GetChar/Next.
struct CharacterList {
    Entry* sentinel;
    Entry* current;
    Entry* end;
};

// Typed source IObjectList boundary for a list with a different owned node
// layout. Methods read the live cursor/node at each call. The descriptor and
// cursor are borrowed, remain alive and disjoint from query output storage,
// and may not be destroyed by a synchronous candidate callback.
struct ObjectListMethods {
    void* context;
    int (*reset)(void*);
    int (*at_end)(void*, std::uint32_t*);
    int (*get)(void*, GameObject**);
    int (*get_char)(void*, Character**);
    int (*next)(void*);
};

// Bind a borrowed ObjectManager+0x60 sentinel and initialize the cursor.
// All entries and actors must remain alive for the synchronous query.
extern "C" int dh2_aggro_character_list_init(CharacterList*, Entry* sentinel);
extern "C" int dh2_aggro_character_list_reset(CharacterList*);
extern "C" int dh2_aggro_character_list_at_end(const CharacterList*,
                                                  std::uint32_t* output);
extern "C" int dh2_aggro_character_list_get(const CharacterList*,
                                               GameObject** output);
extern "C" int dh2_aggro_character_list_get_char(const CharacterList*,
                                                   Character** output);
extern "C" int dh2_aggro_character_list_next(CharacterList*);

// Reconstruct the _UpdateAggro caller's TargetList query over its actual
// CharacterList producer. Unlike the historical RoomRegistry API, this scans
// one borrowed flat ObjectManager Character list and does not infer PFRoom or
// RoomZone membership. List links are read live as Next() advances.
extern "C" int dh2_aggro_target_search_character_list(
    TargetList*, CharacterList*, float view_radius, float cone,
    const Services*);

extern "C" int dh2_aggro_target_search_object_list(
    TargetList*, const ObjectListMethods*, float view_radius, float cone,
    const Services*);

static_assert(sizeof(Entry) == 16);
static_assert(sizeof(CharacterList) == 24);
static_assert(sizeof(ObjectListMethods) == 48);

}  // namespace dh2::character::aggro_character_list
