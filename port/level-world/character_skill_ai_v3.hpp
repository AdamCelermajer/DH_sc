#pragma once
#include "character_skills.hpp"
#include "../game-data/skill_tables.hpp"
namespace dh2::character::skills {
struct SkillAIStateV3 {std::int32_t current{-1};std::uint8_t continued{},last{};std::uint16_t reserved{};};
struct SkillAIOwnerV3 {std::uintptr_t character;std::uint32_t flags,reserved;};
struct SkillAIContextV3 {SkillAIOwnerV3* owner;State40* slots;SkillAIStateV3* fields;std::int32_t script_step;std::uint32_t reserved;};
enum SkillAIOperationV3:std::uint32_t {skill_ai_usable_v3,skill_ai_active_v3,skill_ai_begin_v3,skill_ai_end_v3,skill_ai_use_v3,skill_ai_cancel_v3,skill_ai_focus_v3,skill_ai_event_v3,skill_ai_blur_v3};
enum SkillAIServiceV3:std::uint32_t {skill_ai_using_v3=1,skill_ai_casting_v3,skill_ai_row_v3,skill_ai_callback_v3,skill_ai_set_state_v3,skill_ai_player_v3,skill_ai_property_v3,skill_ai_trophy_manager_v3,skill_ai_network_player_v3,skill_ai_trophy_catalog_v3,skill_ai_unlock_v3,skill_ai_stop_loop_v3};
struct SkillAIRequest32V3 {std::uint32_t operation,index,value,reserved;std::uintptr_t character,subject;};
struct SkillAIResponse32V3 {std::uint32_t word,count;std::uintptr_t identity;const data::SkillProjection76* row;const char*const* names;};
struct SkillAIServices16V3 {void* context;int(*invoke)(void*,SkillAIContextV3*,const SkillAIRequest32V3*,SkillAIResponse32V3*);};
static_assert(sizeof(SkillAIStateV3)==8&&sizeof(SkillAIContextV3)==32&&sizeof(SkillAIRequest32V3)==32&&sizeof(SkillAIResponse32V3)==32&&sizeof(SkillAIServices16V3)==16);
}
// Complete bounded AI skill command/focus bodies. 0 delivered (source answer
// in out), -1 malformed, -2 required service/unsafe source assertion domain.
// Live owner/slot/field reloads are preserved; callback mutations are synchronous.
// Invalid usable indices return false; invalid active/cancel accesses fail.
extern "C" int dh2_character_skill_ai_v3(std::uint32_t*,dh2::character::skills::SkillAIContextV3*,std::uint32_t operation,std::uint32_t index,const dh2::character::skills::SkillAIServices16V3*);
