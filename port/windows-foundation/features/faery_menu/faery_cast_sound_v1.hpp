#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace dh::foundation::faery_menu {

// P15 FAERYSOUND (B050). Original Celest/Hotty OnPreSkill_ PlaySound3D calls
// (faerie_celest.luac / faerie_hotty.luac, plain-text Lua source). They run on
// every cast after the UseMana/SetSpellCooldown prefix, whether or not the
// Pre target list is empty. The hit path (OnSkill_) has no sound: the Celest
// sound block there is commented out in the source.
//
// target_count is the Pre TargetListSearch size (Lua GetTargetListSize). Celest
// with 0 targets plays the medium lightning label (target_count < 6 includes 0)
// and then StaticBallKilled. Hotty with 0 targets plays only the mage staff fire.
// Returns the authored sound labels in play order; the labels are resolved
// through the same sounds table as the rest of the game (no substitutes).
std::vector<std::string> faery_pre_sound_labels_v1(bool hotty, std::size_t target_count);

} // namespace dh::foundation::faery_menu
