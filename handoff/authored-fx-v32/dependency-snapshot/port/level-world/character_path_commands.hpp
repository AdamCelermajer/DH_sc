#pragma once
#include <cstdint>
namespace dh2::character {
struct PathToState40 {
 std::uintptr_t owner;
 std::uint32_t disabled,path_nonempty,limit,reserved;
 float path_target[3];std::uint32_t reserved1;
};
struct PathToRequest32 {
 std::uintptr_t owner;std::uint32_t limit,reserved;
 float target[3];std::uint32_t reserved1;
};
struct PathToServices16 {
 void* context;
 // Complete genuine FindPath service. Status0 is successful invocation;
 // the separate return word preserves its result. It can replace live route
 // storage synchronously. A missing service is never fabricated acceptance.
 int(*find_path)(void*,const PathToRequest32*,std::uint32_t* result);
};
struct PathToResult16 {std::uint32_t requested,limit,find_result,reserved;};
struct LookAtState16 {float position[3],heading_angle;};
static_assert(sizeof(void*)==8&&sizeof(PathToState40)==40);
static_assert(sizeof(PathToRequest32)==32&&sizeof(PathToServices16)==16);
static_assert(sizeof(PathToResult16)==16&&sizeof(LookAtState16)==16);
}
extern "C" {
// Complete original GameObject PathTo route-reuse/disabled/default-limit gates.
// 0 completed;1 malformed caller;2 unavailable/failed FindPath service. Calls
// with existing routes request replacement only for squared3D distance>40000.
// Nonfinite words follow original arithmetic/comparison, including NaN skip.
int dh2_character_path_to(dh2::character::PathToResult16*,
 const dh2::character::PathToState40*,const float* target,
 const dh2::character::PathToServices16*);
// Complete GameObject LookAt(Point), including recovered LookTowards math.
// Retains heading/destination metadata; changes only the stored heading angle.
// 0 complete;1 malformed memory contract. Exceptional float words are allowed;
// arithmetic NaNs follow native libm, without payload identity guarantees.
int dh2_character_look_at_point(dh2::character::LookAtState16*,const float*);
}
