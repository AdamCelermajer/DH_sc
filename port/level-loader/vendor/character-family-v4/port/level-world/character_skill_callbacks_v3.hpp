#pragma once
#include "character_skills.hpp"
namespace dh2::character::skills {
enum SkillCallbackV3:std::uint32_t {skill_pre_v3=0,skill_use_v3=1,skill_post_v3=2,skill_check_usable_v3=3,skill_check_active_v3=4};
enum SkillCallbackServiceV3:std::uint32_t {callback_active_v3=1,callback_set_v3=2,callback_erase_v3=3,callback_call_v3=4,callback_bool_v3=5,callback_release_v3=6};
struct SkillCallbackRequest48V3 {
 std::uint32_t operation{},index{};std::uintptr_t owner{},active{};
 const Instance32* instance{};const char* name{};std::uintptr_t results{};
};
struct SkillCallbackResponse32V3 {
 std::uintptr_t active{},results{};std::uint32_t source_error{},count{},boolean{},reserved{};
};
struct SkillCallbackServices16V3 {void* context{};int(*invoke)(void*,const SkillCallbackRequest48V3*,SkillCallbackResponse32V3*){};};
static_assert(sizeof(SkillCallbackRequest48V3)==48&&sizeof(SkillCallbackResponse32V3)==32);
// Complete CharAISkillScript Pre/Use/Post/Check choreography. Capture first
// receiver, SetSkill, erase its nonempty results, RELOAD owner/active, invoke
// actual callback, convert selected return, destroy results. CheckActive uses
// return1; CheckUsable return0; Pre/Use no results means true. Post ignores Call
// status. Providers retain a result token across erase/call/bool/release.
// Callback may mutate active and owned instance fields synchronously; instance
// and provider remain alive. The second active has no original null guard:
// unavailable delivery is explicit required failure, never accepted empty.
// 0 delivered; -1 malformed; -2 reached provider failure preserving prefix.
}
extern "C" int dh2_character_skill_callback_v3(std::uint32_t*,
 const dh2::character::skills::Instance32*,std::uint32_t,
 const dh2::character::skills::SkillCallbackServices16V3*);
