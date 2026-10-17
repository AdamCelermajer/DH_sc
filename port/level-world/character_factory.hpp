#pragma once

#include "character_state.hpp"

#include <cstdint>

namespace dh2::object_manager_runtime_owner_v1 { class Owner; }

namespace dh2::character::factory {

constexpr std::uint32_t state_bit(std::uint32_t id) {
 return id<32u?(std::uint32_t{1}<<id):0u;
}
constexpr std::uint32_t source_character_registered_states=(1u<<20u)-1u;
constexpr std::uint32_t max_character_name=127u;

// Borrowed adapter record: source object ownership stays with the level/app.
// The normal Character constructor registers state ids 0..19; callers carry
// the actual registration mask so tests/adapters can fail closed if Spawn=1
// was not installed for an actor.
struct ActorRef {
 const char* exact_name=nullptr;
 State* state=nullptr;
 const Facts* facts=nullptr;
 const SpawnFacts* spawn_facts=nullptr;
 std::uint32_t registered_state_mask=0;
 // Optional identity key for the ObjectManager-backed Script_SpawnCharacter
 // path. Legacy callers can keep using request_spawn_character directly.
 std::uintptr_t object_identity=0;
 std::int32_t source_handle=-1;
};

enum class SpawnResult : std::int32_t {
 requested=1,
 lookup_miss=0,
 invalid_request=-1,
 ambiguous_name=-2,
 spawn_state_unregistered=-3,
 source_state_rejected=-4,
 object_not_registered=-5
};

// Implements the bounded source boundary of Script_SpawnCharacter::Execute:
// exactly one loaded Character name is required, then state 1 is requested.
// Missing/duplicate names and malformed actor records emit no services and do
// not modify any actor state. A repeated found request still follows native
// _SetState same-state blur/focus semantics.
SpawnResult request_spawn_character(ActorRef* actors,std::uint32_t actor_count,
 const char* exact_name,const Services* services);

// Generated/source-loaded worlds route Script_SpawnCharacter through the
// already-owned ObjectManager key map before applying the same Character FSM
// transition. This validates each candidate's retained source handle and
// identity without creating a second name/actor registry.
SpawnResult request_registered_spawn_character(
 dh2::object_manager_runtime_owner_v1::Owner& object_manager,
 ActorRef* actors,std::uint32_t actor_count,const char* exact_name,
 const Services* services);

const char* spawn_result_name(SpawnResult result);

}  // namespace dh2::character::factory
