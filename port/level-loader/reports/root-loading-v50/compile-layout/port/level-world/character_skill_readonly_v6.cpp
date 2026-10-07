#include "character_skill_readonly_v6.hpp"
#include "character_stance.hpp"
#include "character_target_search_v5.hpp"
// Headers are consumed before implementation token remapping. Frozen V5
// operations only query this exact inventory; the const successor adds no
// cloned inventory, alternate Session or new property state.
namespace dh2::data {using SkillReadOnlyInventoryV6=const FreshInventoryOwnedV4;}
#define CharacterSkillNativeBindingsV5 CharacterSkillNativeReadOnlyBindingsV6
#define FreshInventoryOwnedV4 SkillReadOnlyInventoryV6
#include "character_skill_native_v5.cpp"
#undef FreshInventoryOwnedV4
#undef CharacterSkillNativeBindingsV5
