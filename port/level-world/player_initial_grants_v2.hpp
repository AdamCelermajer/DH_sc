#pragma once
#include <cstdint>
namespace dh2::player {
enum InitialGrantOperationV2:std::uint32_t {
 online=0x7fd794,online_player_record=0x36eea8,
 num_items=0x3fc608,read_gold=0x3b3994,loot_property=0x3dedb4,
 add_loot=0x40407c,is_equippable=0x3fdeec,auto_equip=0x3a9fa8,
 update_skin=0x3a999c,has_skill_slots=0x3bbea0,set_skill_slot=0x3bbe54,
 swap_equipment=0x3fc6c8,skill_level=0x3bbed0,increment_skill=0x3bcc58,
 has_savegame=0x3bcc74,has_saved_rows=0x3bcc94,property_integer=0x3df6e0,
 skill_available=0x3bca50,skill_limit=0x4c4bdc,difficulty_unlocked=0x3bb918,
 saved_level_read=0x3bcd94,can_increment=0x3bc9ec,property_add=0x3e0798,
 saved_level_increment=0x3bceb8,update_all_skills=0x3d8894,
 properties_recalculate=0x3e0810,potion_capacity_store=0x3bcee4,
 debug_load=0x337888,debug_query=0x337a88
};
// read_gold is the exact direct Character+39c read at3b3994, not a method.
struct InitialGrantRequest32V2 {std::uintptr_t owner;std::uint32_t operation;std::int32_t arguments[5];};
struct InitialGrantResponse8V2 {std::int32_t value;std::uint32_t reserved;};
struct InitialGrantServices16V2 {void* context;int (*invoke)(void*,const InitialGrantRequest32V2*,InitialGrantResponse8V2*);};
static_assert(sizeof(void*)==8&&sizeof(InitialGrantRequest32V2)==32&&sizeof(InitialGrantResponse8V2)==8&&sizeof(InitialGrantServices16V2)==16);
}
extern "C" {
// Exact Character._InitEquipment3b395c and _InitSkillsSlots3b3a90 ordered
// caller projections. Borrowed services perform real live getters/mutations.
// Each callback result is reloaded; no parallel inventory/FSM is fabricated.
// 0 delivered, -1 malformed native call, -2 required service failure. Reached
// source writes/effects remain on failure. increment_skill's source bool is
// intentionally ignored; a required-native failure is not a source false.
int dh2_player_initial_equipment_v2(std::uintptr_t,const dh2::player::InitialGrantServices16V2*) noexcept;
int dh2_player_initial_skill_slots_v2(std::uintptr_t,const dh2::player::InitialGrantServices16V2*) noexcept;
// Actual IncSkill3bcc58. skill_limit argument0 chooses the literal source
// normal/hard/very-hard cap, not a difficulty clamped by this kernel.
// saved_level_increment MUST reload the live savegame/row after property_add,
// then wrap the genuine uint16 level. Read/store operations name source ARM
// instructions, not external engine methods. Debug args identify actual call
// sites (the provider retains genuine isTracingChar_Stats string ownership).
// Missing save/row Debug continuation is explicitly unsupported (-2).
int dh2_player_increment_skill_v2(std::int32_t* result,std::uintptr_t owner,
 std::int32_t saved_row,std::uint32_t test_only,const dh2::player::InitialGrantServices16V2*) noexcept;
}
