#pragma once
#include "../script-runtime/script_runtime.h"
#include <cstdint>
#include <cstddef>
namespace dh2::character::skills {
enum CurrentSpellOperationV1:std::uint32_t {
 current_spell_selected_faery=1,current_spell_validate_faery=2,current_spell_saved_level=3
};
struct CurrentSpellRequest24V1 {
 std::uint32_t operation,id;std::int32_t difficulty;std::uint32_t reserved;
 std::uintptr_t character;
};
struct CurrentSpellResponse16V1 {std::int32_t value;std::uint32_t reserved[3];};
struct CurrentSpellServices16V1 {
 void* context;
 int(*invoke)(void*,const CurrentSpellRequest24V1*,CurrentSpellResponse16V1*);
};
static_assert(sizeof(CurrentSpellRequest24V1)==24&&sizeof(CurrentSpellResponse16V1)==16&&sizeof(CurrentSpellServices16V1)==16);
struct CurrentSpellBindingsV1 {std::uintptr_t character{};CurrentSpellServices16V1 services{};};
// Source GetCurrentSpellInfo ignores Arguments. Each SG selection is freshly
// delivered with difficulty=-1. GetCharFaery's validation is a real required
// service even though its returned row pointer is discarded by the source.
int current_spell_info_v1(void*,const dh2_script_value*,std::uint32_t,
 dh2_script_value*,std::uint32_t,std::uint32_t*,char*,std::size_t);
}
// 1 emits the original integer, -1 malformed, -2 required provider failure.
// Output changes only after the four ordered source services complete.
extern "C" int dh2_character_current_spell_level_v1(std::int32_t*,std::uintptr_t,
 const dh2::character::skills::CurrentSpellServices16V1*);
