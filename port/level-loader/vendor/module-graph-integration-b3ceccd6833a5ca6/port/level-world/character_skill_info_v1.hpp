#pragma once
#include "character_skills.hpp"
#include <cstdint>
namespace dh2::character::skills {
enum SkillInfoOperationV1:std::uint32_t {skill_info_active_v1=1,skill_info_set_v1=2,skill_info_call_v1=3,skill_info_timer_v1=4};
struct SkillInfoRequestV1 {
 std::uint32_t operation{},level{};std::uintptr_t owner{},active{};
 const Instance32* instance{};const char* name{};std::int32_t timer{};std::uint32_t reserved{};
};
struct SkillInfoResponseV1 {
 std::uintptr_t active{};std::uint32_t source_error{},return_count{},first_type{};
 float first_number{};std::uint32_t timer_found{},elapsed{},duration{},reserved{};
};
struct SkillInfoServicesV1 {void* context{};int(*invoke)(void*,const SkillInfoRequestV1*,SkillInfoResponseV1*){};};
// AI_SkillInfo -> complete CharAISkillScript::GetInfo choreography. Valid index
// required; original assertion-disabled out-of-range loads are not successful
// branches. Source Call error is delivered source_error, not provider failure.
// The real Lua calls mutate the shared temporary property sheet. This operation
// does not synthesize that sheet or return fabricated skill property values.
// Callback may mutate active receiver/instance owner synchronously; instance,
// live vector and referenced owner/provider remain valid until return.
// Return0 delivered, -1 malformed, -2 required provider failure.
int character_skill_info_v1(State40*,std::uint32_t index,std::uint32_t level,
 float* fraction,const SkillInfoServicesV1*);
} // namespace
extern "C" int dh2_character_skill_info_v1(dh2::character::skills::State40*,
 std::uint32_t,std::uint32_t,float*,const dh2::character::skills::SkillInfoServicesV1*);
