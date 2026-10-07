#pragma once
#include <cstdint>
#include <map>
namespace dh2::character {
// CharAI C2 3cec68..9c. These are not Character.master14d4 or
// the reciprocal threat maps. Fresh construction alone produces this view.
struct CharacterAiPointerFieldsV105 {
 //CharAI C1 explicit3cec28/2c, mutated only by _UpdateAggro.
 std::int32_t aggro_delay8{},aggro_defer_c{};
 std::uintptr_t master50{},auxiliary58{};
 std::uint8_t master_alive54{1},master_sight55{1};
 std::map<std::int32_t,std::uintptr_t> observers5c;
};
}
