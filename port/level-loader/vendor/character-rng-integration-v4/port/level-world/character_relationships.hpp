#pragma once
#include "character_target_providers.hpp"
#include "../game-data/ai.hpp"
namespace dh2::relationships {
struct Object48 {target_providers::Character32 character;target_providers::Handle16* handle;std::int32_t object_type;std::uint32_t reserved;};
// character.properties may be null for noncharacter object_type!=0. AI owners
// and resolved type0 objects require actual borrowed resolved cache storage.
struct State16 {Object48* owner;Object48* target;};
struct FactionRow16 {const data::AiFactionEntry* entries;std::uint32_t count,reserved;};
struct Factions16 {const FactionRow16* rows;std::uint32_t count,reserved;};
enum Service:std::uint32_t {virtual_player=1,virtual_interactive,virtual_interaction_type};
using Request24=target_providers::Request24;using Services16=target_providers::Services16;
static_assert(sizeof(Object48)==48&&sizeof(State16)==16&&sizeof(FactionRow16)==16&&sizeof(Factions16)==16);
// op1 Enemy / op2 Friend. Shared handle frame stamping and empty map insertion
// precede virtual callbacks. Registry records hold borrowed Object48 pointers,
// not character booleans. Candidate=null selects live State16.target.
// 0 success / 1 atomic malformed / 2 malformed provider or capacity after effects.
extern "C" int dh2_character_relationship(std::int32_t* output,std::uint32_t op,State16*,Object48* candidate,target_providers::Registry24*,const Factions16*,const Services16*);
}
