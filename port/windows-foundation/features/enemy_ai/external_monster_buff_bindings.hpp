#pragma once
#include "../../../level-world/character_skill_buff_bindings_v3.hpp"
#include "../../../level-world/character_skill_properties_v1.hpp"
#include <memory>
namespace dh::foundation::enemy_ai {
// Borrow from the same CharProperties BuffOwner. The actual owner initializer
// must establish its groups/CharTimers/effects/recalc before this loan. This
// factory only routes original Lua entrypoints; it never allocates BuffOwner.
struct ExternalMonsterBuffBindings {
 std::shared_ptr<void> receiver_lease;
 dh2::character::skills::SkillBuffBindingsV3* buffs{};
 dh2::character::skills::SkillPropertyBindingsV1* properties{};
 dh2::data::PropertyView* same_buff_properties{};
};
int select_external_monster_buff(void*,std::uint32_t original_callback,
 dh2_script_function*,void**);
}
