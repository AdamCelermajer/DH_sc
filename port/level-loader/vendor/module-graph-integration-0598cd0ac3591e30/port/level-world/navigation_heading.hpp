#pragma once
#include <cstdint>
namespace dh2::navigation {
struct HeadingState {float direction[3],angle;std::uint32_t active,reserved;};
static_assert(sizeof(HeadingState)==24);
// Internal original arithmetic also accepts NaNs produced by obstacle forces.
// The public C API below retains its finite-caller validation contract.
void set_heading_unchecked(HeadingState&,const float* direction,std::uint32_t rotate);
// Original LookTowards arithmetic without the public finite-caller guard.
// Internal source coordinators can pass exceptional arithmetic results.
void look_towards_unchecked(float& angle,const float* direction);
}
extern "C" {
// Recovered GameObject::LookTowards. XY zero preserves the existing angle;
// Z is ignored. The original uses atan(x/-y), with explicit quadrant branches.
// 0 success, 1 malformed finite input. Output angle must not alias direction.
int dh2_nav_look_towards(float* angle,const float* direction);
// Recovered SetHeadingDirection. Clears Z, uses squared XY length > 1e-4
// for the active bit, and normalizes only when squared length > 1. Rotation
// uses the input vector, including original self-direction alias semantics.
// Direction may equal state->direction; other overlapping storage is unsupported.
// 0 success, 1 malformed caller with state preserved.
int dh2_nav_set_heading(dh2::navigation::HeadingState*,const float* direction,std::uint32_t rotate);
}
