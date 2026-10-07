#pragma once
#include "character_npc_initpost_owner_v1.hpp"
namespace dh2::character {
struct CharacterGenericInitPostFieldsV61 {
 NpcInitPostBorrowV1 common;
 const std::uintptr_t* save14e8{};
 // Actual embedded37c source stores, delivered through the same Gear owner.
 void* inventory_context{};
 bool(*store_gold3a4)(void*,std::int32_t,std::string&){};
 bool(*store_capacity3a8)(void*,std::uint8_t,std::string&){};
};
// One original Character::InitPost control-flow receiver for Character and
// Player catalog aliases. Helper endpoints are exact reached dependencies.
// Marker block entry3b5214 means the existing whole source marker initializer,
// including actual FX-set-vector-empty guard, nine slots and optional149c.
// Equipment grants occur EARLIER at3b395c, not AddMultiplayerHighlight3a41a0.
class CharacterGenericInitPostOwnerV61 {
 CharacterGenericInitPostFieldsV61 fields_;NpcInitPostServicesV1 services_;
 bool failed_{};std::string error_;std::uint32_t last_entry_{},calls_{};
 bool call(std::uint32_t,NpcInitPostResponseV1&,std::uint32_t=0,std::uint32_t=0,std::uintptr_t=0,const char* =nullptr);
 bool debug();bool is_player(bool&);bool player();
public:
 CharacterGenericInitPostOwnerV61(CharacterGenericInitPostFieldsV61 fields,NpcInitPostServicesV1 services):fields_(fields),services_(services){}
 bool initialize();
 const std::string& error()const noexcept{return error_;}
 std::uint32_t last_entry()const noexcept{return last_entry_;}
 std::uint32_t service_calls()const noexcept{return calls_;}
};
}
