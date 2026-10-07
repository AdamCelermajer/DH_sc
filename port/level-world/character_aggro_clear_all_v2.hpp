#pragma once
#include "character_target_bindings.hpp"
#include "../game-data/aggro.hpp"
#include <string>
namespace dh2::character {
struct AggroClearActorBorrowV2 {
 std::uintptr_t identity{};
 data::AggroTable* outgoing{};
 data::AggroTable* incoming{};
 TargetBindings48* target{};
};
struct AggroClearAllServicesV2 {
 void* context{};
 // Pure native identity/lifetime transport of source raw Character pointers.
 // Borrow the actual two relation maps and target; never create a substitute.
 bool(*actor)(void*,std::uintptr_t,AggroClearActorBorrowV2&,std::string&){};
 bool(*on_deaggro)(void*,std::uintptr_t receiver,std::uintptr_t other,
                  const dh2_script_callback_scope*,std::string&){};
};
struct AggroClearAllResultV2 {
 std::uint32_t reciprocal_erases{},notifications{},targets_cleared{};
 bool self_map_cleared{},complete{};
};
// Whole AI_ClearAllAggro3d5fa8 and AI_ClearAllAggroTowardMe3d6abc.
// Source RB maps use the project's existing sorted native 64-bit key transport.
// Reciprocal erases precede the source self-map clear, which precedes callbacks.
// true TowardMe performs real SetTarget(NULL,false) then SyncLastTarget;
// OnDied calls false and therefore does not stop or retarget other characters.
// Callbacks run after erases and may synchronously reenter or add new relations.
// Mutation of the traversed map inside true's SetTarget is an explicit boundary
// (the original iterators are invalidated too), never silently successful.
bool character_aggro_clear_all_v2(AggroClearAllResultV2&,
 AggroClearActorBorrowV2,const AggroClearAllServicesV2&,std::string&,
 const dh2_script_callback_scope* scope=nullptr);
bool character_aggro_clear_all_toward_me_v2(AggroClearAllResultV2&,
 AggroClearActorBorrowV2,bool clear_targets,const AggroClearAllServicesV2&,
 std::string&,const dh2_script_callback_scope* scope=nullptr);
}
