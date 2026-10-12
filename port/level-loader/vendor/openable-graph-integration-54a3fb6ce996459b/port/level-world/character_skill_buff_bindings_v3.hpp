#pragma once
#include "character_buffs.hpp"
#include "../script-runtime/script_runtime.h"
namespace dh2::character::skills {
struct SkillBuffBindingsV3 {
 BuffOwner* owner{};
 // UINT_MAX means the actual FX catalog count is not supplied. Its reached
 // nonnumber guard must fail delivery; a numeric FX ID bypasses this source
 // guard and still requires a real FX creation backend if nonnegative.
 std::uint32_t class_count{},fx_count{};
 void* conversion_context{};
 // Original Value::getNumber for string/identity/tag7. Nil/Boolean/number are
 // native. Failure is explicit; identities are never truncated to ARM32.
 int(*number)(void*,const dh2_script_value*,float*){};
};
int skill_create_buff_v3(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
int skill_remove_buff_v3(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
// Exact owned identity resolver. Accepts only a current BuffInst's first sheet;
// does not dereference arbitrary lightuserdata or keep expired sheet pointers.
int skill_buff_sheet_v3(BuffOwner*,std::uintptr_t,std::int32_t**);
}
