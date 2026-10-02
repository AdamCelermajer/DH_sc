#ifndef DH2_EQUIPMENT_BONUSES_H
#define DH2_EQUIPMENT_BONUSES_H
#include "../loot-tables/loot.h"
#include "../character-state/state.h"
#ifdef __cplusplus
extern "C" {
#endif
struct dh2_equipment_slot { uint32_t present;int32_t item_id,type,slotting,weapon_kind; };
struct dh2_equipment { struct dh2_equipment_slot slots[2][3];uint32_t current_set,owner_hand_rule; };
enum dh2_equipment_flag { DH2_HAS_SHIELD=0,DH2_HAS_OFF_HAND=1,DH2_HAS_TWO_HANDER=2,DH2_HAS_TWO_HANDER_IGNORE_RULE=3 };
enum dh2_equipment_bonus { DH2_BONUS_CRIT=0,DH2_BONUS_ATTACK=1,DH2_BONUS_DAMAGE=2 };
/* Owned snapshot of slots 0..2 in two equipment sets. -1 item ID is absent.
 * current_set is 0/1; owner_hand_rule is the unsigned original owner+0x1324
 * (final field 203, Special_Equip_2H_In_One). Caller refreshes this snapshot input
 * after property changes; the full Character lifecycle remains unimplemented.
 * No original ItemInventory allocation, mutation, gear property contribution,
 * equip restrictions or item instances are implemented by this snapshot.
 * 0 succeeds; 1 invalid pointers/aliases; 2 invalid data/index/operation.
 * Every rejection preserves outputs and all inputs. */
uint32_t dh2_equipment_load(const struct dh2_loot_tables *,const int32_t [6],uint32_t,uint32_t,struct dh2_equipment *);
uint32_t dh2_equipment_get_set(const struct dh2_equipment *,int32_t,int32_t *);
uint32_t dh2_equipment_get_item(const struct dh2_equipment *,uint32_t,int32_t *);
uint32_t dh2_equipment_flag(const struct dh2_equipment *,uint32_t,int32_t *);
uint32_t dh2_equipment_bonus(const struct dh2_equipment *,const struct dh2_property_sheet *,uint32_t,uint32_t,int32_t *);
uint32_t dh2_equipment_get_int_bonus(const struct dh2_equipment *,const struct dh2_character_props *,const struct dh2_property_sheet *,uint32_t,uint32_t,int32_t *);
#ifdef __cplusplus
}
#endif
#endif
