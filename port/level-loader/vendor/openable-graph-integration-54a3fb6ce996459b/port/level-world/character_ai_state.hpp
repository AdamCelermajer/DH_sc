#pragma once
#include <cstdint>
namespace dh2::character {
// Live identities, not an overlay of the original ARM32 CharAI object.
struct AttackQueryState16 {std::uintptr_t owner,target;};
enum AttackQueryService : std::uint32_t {
 attack_is_enemy=0,attack_inventory_can_melee,attack_in_melee_range,
 attack_owner_can_range,attack_in_range
};
struct AttackQueryRequest24 {
 std::uint32_t service,reserved;
 std::uintptr_t owner,target;
};
struct AttackQueryServices16 {
 void* context;
 // Synchronous source query boundaries; may refresh live owner/target.
 std::uint32_t(*query)(void*,AttackQueryState16*,const AttackQueryRequest24*);
};
static_assert(sizeof(void*)==8&&sizeof(AttackQueryState16)==16);
static_assert(sizeof(AttackQueryRequest24)==24&&sizeof(AttackQueryServices16)==16);
}
// 0 success, 1 malformed before callbacks/output mutation. The result retains
// the final range-query word, matching the original tailcall; other true paths
// produce 1. A null explicit target selects the source live state.target once.
extern "C" int dh2_character_ai_can_attack(dh2::character::AttackQueryState16*,
 std::uintptr_t,const dh2::character::AttackQueryServices16*,std::uint32_t*);
