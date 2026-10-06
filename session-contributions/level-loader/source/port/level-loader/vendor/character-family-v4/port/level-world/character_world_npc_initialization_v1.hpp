#pragma once
#include "actor_initialization.hpp"
#include "character_world_npc_state_owner_v1.hpp"
#include "character_script_session.hpp"
#include "character_ai_state_changed.hpp"
namespace dh2::character {
// Level::_LoadCharStates source operation, over the SAME registered character.
// This is not the separate full Character::InitPost/AI resource factory.
struct WorldNpcInitializationBorrowV1 {
 std::uintptr_t identity{};std::uint32_t room{};const char* name{};
 CharacterWorldNpcStateOwnerV1* state_owner{};
 CharacterScriptSession* script{};
};
class CharacterWorldNpcInitializationV1 {
 skills::CharacterWorldRuntimeV1& world_;
 const ActorInitialization& initialization_;
 std::string error_;
public:
 CharacterWorldNpcInitializationV1(skills::CharacterWorldRuntimeV1& world,const ActorInitialization& source):world_(world),initialization_(source){}
 // Actual CAI1 lookup provides source CString. World properties/life and FSM
 // borrows must match the retained ScriptSession and NPC owner exactly.
 // 1 complete; -1 invalid borrow before mutation; -2 reached source failure.
 int initialize(const WorldNpcInitializationBorrowV1&);
 const std::string& error()const noexcept{return error_;}
};
// The original Character constructor allocates/binds v2Controller and zeros
// byte+8 locked and byte+9 forced; +c then receives the same Character owner.
// This is the sole corresponding native controller lifetime, not a movement
// implementation or an alternate path-controller/physical/body authority.
class CharacterWorldNpcControllerV1 {
 ControllerCommandState32 command_{};
public:
 explicit CharacterWorldNpcControllerV1(std::uintptr_t owner){command_.controller=reinterpret_cast<std::uintptr_t>(&command_);command_.owner=owner;}
 CharacterWorldNpcControllerV1(const CharacterWorldNpcControllerV1&)=delete;
 CharacterWorldNpcControllerV1& operator=(const CharacterWorldNpcControllerV1&)=delete;
 std::uintptr_t identity()const noexcept{return command_.controller;}
 // Source global controller-block byte is an actual caller-produced snapshot.
 ControllerCommandState32* command_state(std::uint32_t global_blocked)noexcept{if(global_blocked>255)return nullptr;command_.global_blocked=global_blocked;return &command_;}
};
// Whole Character event1d -> whole CharAI dispatcher -> actual FSM getter ->
// active AIS source relay. AISExternal inherits the proven empty Default
// StateChanged at3dbe8c; other nonempty selected endpoints remain mandatory.
class CharacterWorldNpcStateChangedV1 {
 CharacterWorldNpcStateOwnerV1& states_;AIEventState64& ai_;
 const AIEventServices24* remaining_{};
 AIEventServices24 services_{};std::string error_;
 static std::int32_t service(void*,AIEventState64*,const AIEventRequest40*,std::uint32_t*);
public:
 CharacterWorldNpcStateChangedV1(CharacterWorldNpcStateOwnerV1&,AIEventState64&,const AIEventServices24* remaining=nullptr);
 int raise(std::uintptr_t previous);
 // StateOwnerServices callback entry; only the exact Character event1d request
 // is handled. Feed all other methods to their genuine source providers.
 int notify(const StateOwnerRequest48&);
 const std::string& error()const noexcept{return error_;}
};
}
