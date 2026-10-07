#pragma once
#include "character_heading_owner_v1.hpp"
namespace dh2::character {
struct CharacterHeadObjectBorrowV2 {
 ControllerCommandState32* controller{};
 CharacterHeadingOwnerV1* heading{};
 // Logical PathController32 stores the original source byte as uint32_t.
 // Borrow its sole heading.active field, rather than aliasing its low byte.
 const std::uint32_t* source_heading_active1b5{};
 const float* source_vec3_origin{};
 CharacterHeadingServicesV1 services;
};
// Whole actual v2Controller.Cmd_HeadTowards(GameObject)4053d0 and Character
// Ctrl_HeadTowards(GameObject)3ad8d4. Uses existing same-player/native heading
// owner; sourceNULL calls real point-zero branch only when heading was active.
bool character_command_head_object_v2(const CharacterHeadObjectBorrowV2&,
 std::uintptr_t target,std::string& error);
}
