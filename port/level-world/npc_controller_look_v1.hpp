#pragma once
#include "character_world_runtime_v1.hpp"
namespace dh2::character {
// Narrow same-World provider for injury blur. No controller/path/angle clone.
// Canonical World actor must already expose real GameObject+178 backing; the
// controller's reference can alias it because original v2Controller CmdLookAt
// dispatches to Character/GameObject and owns no separate angle.
struct NpcControllerLookBorrowV1 {
 skills::CharacterWorldRuntimeV1* world{};
 std::uintptr_t identity{};
 float* source_heading_178{};
};
int npc_controller_look_v1(const NpcControllerLookBorrowV1&,std::uintptr_t target);
}
