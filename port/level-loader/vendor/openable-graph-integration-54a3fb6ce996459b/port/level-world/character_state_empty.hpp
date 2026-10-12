#pragma once
#include <cstdint>
namespace dh2::character {
// Independent method selector; explicitly map owner blur/focus/event enums.
enum StateMethod:std::uint32_t {state_method_focus=0,state_method_blur,state_method_update,state_method_event};
}
extern "C" {
// Remaining16 families only. Executes only instruction-proven 4B bx-lr bodies:
// 1 genuinely empty body completed, 0 genuine nonempty method remains required,
// -1 invalid state/selector/mismatched original method metadata. No state stores,
// callbacks, dependencies or invented successful nonempty method effects.
int dh2_character_state_empty_body(std::int32_t state,std::uint32_t operation,std::uint32_t source_function);
}
