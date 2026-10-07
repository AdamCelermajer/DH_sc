#pragma once
#include "../game-data/properties.hpp"
#include "../game-data/ai.hpp"
#include <string>
namespace dh2::character {
// SAME receiver field borrows. The NPC domain selects original IsPlayer=false;
// a reached true branch fails explicitly, never executes partial player init.
struct NpcInitPostBorrowV1 {
 std::uintptr_t identity{};std::uint8_t* called1394{};
 const std::int32_t* spawn_probability274{};
 const std::string* starting_waypoint13e8{};const std::int32_t* room64{};
 std::int16_t* properties_id13c8{};data::PropertyView* properties{};
 std::string* model290{};float* scale120{};
 std::uint32_t* delayed3ec{}; // SAME ScriptOwner.lifecycle().delayed projection
 const std::int32_t* self_fx_offset1488{};
 std::uintptr_t* self_fx1484{};const std::uintptr_t* visual2d8{};
 float* fade1440{};float* fade1444{};
 const float* position160{};float* initial_position1450{};
 const float* rotation16c{};float* initial_rotation145c{};
 void* lifecycle_context{};
 std::uint32_t*(*live_delayed3ec)(void*){};
};
struct NpcInitPostRequestV1 {
 std::uint32_t source_entry{},argument0{},argument1{};
 std::uintptr_t subject{},payload{};const char* text{};
};
struct NpcInitPostResponseV1 {
 std::int32_t value{};std::uintptr_t identity{};const char* text{};
 const data::AiProps* ai{};
};
struct NpcInitPostServicesV1 {
 void* context{};
 bool(*invoke)(void*,const NpcInitPostRequestV1&,NpcInitPostResponseV1&,std::string&){};
};
// Whole shipping Character::InitPost NPC control flow (3b4d60), retaining exact
// helper order/source stores. Complete helper bodies remain required backends.
// called1394 is set BEFORE spawn sampling and survives failure; failed attempt
// cannot be retried merely because a subsequent source call observes that gate.
class CharacterNpcInitPostOwnerV1 {
 NpcInitPostBorrowV1 fields_;NpcInitPostServicesV1 services_;
 bool failed_{};std::uint32_t last_entry_{},calls_{};std::string error_;
 bool call(std::uint32_t,NpcInitPostResponseV1&,std::uint32_t=0,std::uint32_t=0,std::uintptr_t=0,const char* =nullptr);
 bool debug();bool npc();
public:
 CharacterNpcInitPostOwnerV1(NpcInitPostBorrowV1 f,NpcInitPostServicesV1 s):fields_(f),services_(s){}
 bool initialize();
 std::uint32_t last_entry()const noexcept{return last_entry_;}
 std::uint32_t service_calls()const noexcept{return calls_;}
 const std::string& error()const noexcept{return error_;}
};
}
