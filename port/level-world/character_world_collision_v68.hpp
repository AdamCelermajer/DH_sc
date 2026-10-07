#pragma once
#include "character_world_npc_collision_v1.hpp"
#include <functional>
namespace dh2::character {
// SAME selected NPC or PlayerV6 session; no VM or property/life is copied.
struct CharacterCollisionScriptBorrowV68 {
 std::shared_ptr<void> receiver;
 std::shared_ptr<data::PropertyState> properties;
 std::shared_ptr<data::CombatActorState> life;
 std::function<bool(ScriptSessionView&)> active;
};
class CharacterWorldCollisionV68 {
 skills::CharacterWorldRuntimeV1& world_;
 CharacterWorldNpcStateOwnerV1& machine_;
 CharacterCollisionScriptBorrowV68 session_;
 ScriptCharacterObject& object_;
 AIEventState64& ai_;
 ControllerCommandState32& controller_;
 WorldNpcAISCollisionFieldsV1& fields_;
 WorldNpcCollisionGlobalsV1& globals_;
 DebugSwitches* debug_;
 const DebugFileServices24* files_;
 WorldNpcCollisionServicesV1 services_;
 std::string error_;
 ScriptCollisionState72* current_{}; // synchronous borrow, supports reentry
 std::uint32_t current_character_queries_{},current_persist_{};
 bool coherent();
 bool handle_character(std::uintptr_t&);
 bool debug_prefix();
 bool ais_collision(std::uint32_t slot,std::uintptr_t collider,std::uint32_t persist);
 bool script_collision(std::uintptr_t collider,std::uint32_t persist);
 bool query(std::uint32_t,std::uintptr_t,std::uintptr_t,std::uint32_t&);
 static std::uint32_t collision_service(void*,ScriptCollisionState72*,ScriptCollisionObject16*,const ScriptCollisionRequest32*);
 static int event_service(void*,AIEventState64*,const AIEventRequest40*,std::uint32_t*);
public:
 CharacterWorldCollisionV68(skills::CharacterWorldRuntimeV1&,CharacterWorldNpcStateOwnerV1&,
  CharacterCollisionScriptBorrowV68,ScriptCharacterObject&,AIEventState64&,ControllerCommandState32&,
  WorldNpcAISCollisionFieldsV1&,WorldNpcCollisionGlobalsV1&,DebugSwitches*,const DebugFileServices24*,WorldNpcCollisionServicesV1);
 // Complete POCharacter Begin/Persist/End -> same Character RaiseEvent ->
 // CharAI active AIS -> inherited AISDefault collision path. Source physical
 // Result is an independently proved bx lr body and invokes no AIS method.
 // peer_owner0 is the source absent physical-owner short circuit.
 bool physical_event(WorldNpcPhysicalEventV1,std::uintptr_t peer_owner,std::uint32_t source_persist);
 // Collision filter's exact source FSM prefix. Remaining default physical
 // filter must be composed with actual body/owner byte80 and shape filters.
 bool permits_filter(std::uint16_t other_category,bool&);
 const std::string& error()const noexcept{return error_;}
};
}


