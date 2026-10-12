#pragma once
#include "../game-data/animation_tables.hpp"
#include <cstdint>
namespace dh2::character {
// Cached Character property payload. Index32 is original owner+1078; index2
// is owner+1000. These are data words, not an original Character overlay.
struct CombatProperties896 {std::int32_t words[224];};
struct CombatItemInstance4 {std::int32_t item_id;};
// Read-only logical ItemTable row projection: source GetItem uses stride164,
// and HasRangedWeapon reads word22 (row+58). Unused words are opaque data.
struct CombatItemRecord164 {std::int32_t words[41];};
struct CombatEquipSet8 {const CombatItemInstance4* const* main_hand;};
struct CombatInventory16 {
 const CombatEquipSet8* sets;
 std::uint32_t count;
 std::int32_t current_set; // source signed byte inventory+2e
};
struct CombatAnimationBank8 {std::int32_t attack_sequence,reserved;};
static_assert(sizeof(CombatProperties896)==896&&sizeof(CombatItemRecord164)==164);
static_assert(sizeof(void*)==8&&sizeof(CombatEquipSet8)==8&&sizeof(CombatInventory16)==16);
// Existing decoded table bridge: reads bank.fields[0] (Attack) and actual
// sequence.type. No resolved capability boolean or fabricated stance is used.
int has_combo_attack(const CombatProperties896&,const data::AnimationTables&)noexcept;
}
extern "C" {
// Return source0/1; -1 malformed borrowed span before any unsafe dereference.
// Unused inventory/item spans are not inspected on property short-circuit.
int dh2_character_can_range_attack(const dh2::character::CombatProperties896*,
                                  const dh2::character::CombatInventory16*,
                                  const dh2::character::CombatItemRecord164*,std::uint32_t item_count);
int dh2_inventory_has_ranged_weapon(const dh2::character::CombatInventory16*,
                                    const dh2::character::CombatItemRecord164*,std::uint32_t item_count);
int dh2_character_has_combo_attack(const dh2::character::CombatProperties896*,
                                  const dh2::character::CombatAnimationBank8*,std::uint32_t bank_count,
                                  const std::int32_t* sequence_types,std::uint32_t sequence_count);
}
