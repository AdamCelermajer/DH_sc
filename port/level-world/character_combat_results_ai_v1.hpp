#pragma once
#include "character_script_owner.hpp"
#include "character_script_owner_v2.hpp"
#include "../script-runtime/script_object_bridge.h"
namespace dh2::character {
struct CombatResultsAiBorrowV1 {
 // Same embedded CharAI selected AIS owner; a null owner is source activeAIS0.
 ScriptOwner* owner{};
 const dh2_script_callback_scope* scope{};
 // Actual retained active AIS source virtual+b4 slot, not inferred from kind.
 std::uint32_t source_character_callback{};
 ScriptOwnerV2* owner_v2{};
};
// Character overload3d0da4 -> AIS virtualb4 -> inherited Default3dc9a8.
// Selects freshly per receiver; attacker dispatch precedes target dispatch.
// Returns actual VM status (positive Lua error retained, negative native
// required failure retained); no GameObject-overload no-op substitution.
int character_combat_results_ai_v1(const CombatResultsAiBorrowV1&,
 std::uintptr_t attacker,std::uintptr_t target,std::uint8_t outcomes,
 std::string& diagnostic);
int character_combat_results_pair_v1(const CombatResultsAiBorrowV1& attacker_ai,
 const CombatResultsAiBorrowV1& target_ai,std::uintptr_t attacker,
 std::uintptr_t target,std::uint8_t outcomes,std::string& diagnostic);
}
