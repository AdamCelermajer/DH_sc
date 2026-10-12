#pragma once
#include <cstdint>
#include "../level-world/character_timers.hpp"
namespace dh2::ui {
struct HudSkillSlotNode { const HudSkillSlotNode* left; const HudSkillSlotNode* right; std::int32_t key,value; };
struct HudSkillSlotTree {const HudSkillSlotNode* root;std::uint32_t maximum_nodes,reserved;};
struct HudSkillLevel8 {std::uint32_t word0;std::uint16_t level,word6;};
struct HudSavedSkillLevels {const HudSkillLevel8* rows;std::uint32_t count,reserved;};
struct HudCooldown {const dh2::character::TimerStore32* timers;std::int32_t timer_id;std::uint32_t reserved;};
struct HudSkillOwner {std::uintptr_t identity;std::uint32_t flags,reserved;};
struct HudSkillAI {HudSkillOwner* owner;std::int32_t script_stage;std::uint32_t reserved;const std::uintptr_t* skills;std::uint32_t skill_count,reserved1;const std::uintptr_t* spells;std::uint32_t spell_count,reserved2;};
enum class HudSkillOperation:std::uint32_t {current_state=1,selected_spell=2,script_usable=3,script_info=4};
struct HudSkillRequest {HudSkillOperation operation;std::uint32_t index;std::int32_t level,reserved;std::uintptr_t subject;};
struct HudSkillResponse {std::int32_t value;float fraction;};
struct HudSkillServices {void* context;int(*invoke)(void*,HudSkillAI*,const HudSkillRequest*,HudSkillResponse*);};
static_assert(sizeof(void*)==8&&sizeof(HudSkillAI)==48&&sizeof(HudSkillRequest)==24&&sizeof(HudSkillResponse)==8&&sizeof(HudSkillServices)==16&&sizeof(HudCooldown)==16);
// script_info response starts with the caller's previous response bytes. The
// original GetInfo can return after failed VCB without writing its float; the
// provider must preserve that prefix, not replace it with fabricated zero.
// Source Character null-savegame wrapper: nullptr tree/levels returns -1 value.
// Slot tree is the logical map0: actual GetCurrentSkillSet returns0 twice.
// Borrowed topology must stay valid; bounded/cycle rejection is native safety.
}
extern "C" {
int dh2_ui_hud_num_potions(const std::int16_t* quantity,std::int32_t* out) noexcept;
int dh2_ui_hud_skill_slot(const dh2::ui::HudSkillSlotTree*,std::int32_t slot,std::int32_t* out) noexcept;
int dh2_ui_hud_skill_level(const dh2::ui::HudSavedSkillLevels*,std::uint32_t index,std::int32_t* out) noexcept;
int dh2_ui_hud_cooldown(const dh2::ui::HudCooldown*,float* out) noexcept;
// op0 skill usable,1 spell usable,2 skill info,3 spell info. Index unsigned.
// Exact successful source domain; unchecked/asserting source OOB returns-1.
// 0 delivered,-1 malformed,-2 reached required script/FSM delivery failure.
int dh2_ui_hud_skill_query(dh2::ui::HudSkillAI*,std::uint32_t op,std::uint32_t index,std::int32_t level,dh2::ui::HudSkillResponse*,const dh2::ui::HudSkillServices*) noexcept;
}
