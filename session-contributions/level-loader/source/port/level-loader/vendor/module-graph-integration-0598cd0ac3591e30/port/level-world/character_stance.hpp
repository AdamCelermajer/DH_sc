#pragma once
#include <cstdint>
namespace dh2::character {
enum StancePredicate : std::uint32_t {stance_is_player=1,stance_has_staff=2,
 stance_has_bow=4,stance_dual_wielding=8,stance_has_two_hander=16,stance_has_main_hand=32};
// Resolved current-equip-set queries, not visual equipment inference.
struct StanceFacts16 {std::uint32_t predicates=0;std::int32_t count=0;std::uint32_t reserved[2]{};};
static_assert(sizeof(void*)==8&&sizeof(StanceFacts16)==16);
}
// 1 success; -1 malformed before output mutation. Signed original clamp.
extern "C" int dh2_character_anim_stance(std::int32_t*,const dh2::character::StanceFacts16*);
