#pragma once
#include "fresh_inventory_owned_v4.hpp"
#include "combat.hpp"

namespace dh2::data {
struct EquipmentWeaponFacts12V1 {
 std::int32_t main_category{-1},off_category{-1};
 std::uint32_t flags{};
};
enum EquipmentWeaponFlagV1:std::uint32_t {
 weapon_main=1,weapon_bow=2,weapon_staff=4,weapon_dual=8,
 weapon_shield=16,weapon_two_raw=32,weapon_two_effective=64,weapon_ranged=128
};
static_assert(sizeof(EquipmentWeaponFacts12V1)==12);
// Exact original HasMainHand/HasBow/HasStaff/HasRangedWeapon,
// DualWielding/HasShield and HasTwoHander(true/false), from the frozen
// original-gold projection.
// Null records are genuine unequipped slots. Invalid/aliased outputs reject
// before writes; categories are actual word37, never inferred from Item names.
int equipment_weapon_facts_v1(EquipmentWeaponFacts12V1*,const ItemRecord164*,
                             const ItemRecord164*,std::int32_t flag1324) noexcept;
// Read-only adapter over the sole inventory and exact live property view.
// No item/equipment/property/RNG mirror, VM, timer or renderer owner. Both
// borrowers must outlive this facade; native caller enforces attach/retirement.
class PlayerEquipmentQueriesLiveV1 {
 const FreshInventoryOwnedV4* inventory_;
 const PropertyView* properties_;
public:
 PlayerEquipmentQueriesLiveV1(const FreshInventoryOwnedV4& i,const PropertyView& p):inventory_(&i),properties_(&p){}
 bool facts(EquipmentWeaponFacts12V1&,std::string&)const;
 // Source combat uses RAW two-hander, stance uses EFFECTIVE flag. The combat
 // projection preserves caller's FSM state/combo_hits and uses the same cached
 // property sheet. It rejects categories outside the existing combat domain.
 bool combat_view(CombatantView&,std::string&)const;
};
}
