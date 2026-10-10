#include "faery_cast_sound_v1.hpp"

namespace dh::foundation::faery_menu {

std::vector<std::string> faery_pre_sound_labels_v1(bool hotty, std::size_t target_count) {
    // Lua: target_count > 0 and < 3 -> small; < 6 -> medium (0 included); else big.
    const char* tier = target_count > 0 && target_count < 3 ? "small"
                     : target_count < 6 ? "medium" : "big";
    if (hotty) {
        // Hotty OnPreSkill_: the hit tiers only when the list is not empty;
        // an empty list plays sfx_mage_staff_elemental_fire alone.
        if (target_count == 0) return {"sfx_mage_staff_elemental_fire"};
        return {std::string("sfx_spell_fire_") + tier};
    }
    // Celest OnPreSkill_: the tier is always played; an empty list adds StaticBallKilled.
    std::vector<std::string> labels{std::string("sfx_spell_lightning_") + tier};
    if (target_count == 0) labels.emplace_back("StaticBallKilled");
    return labels;
}

} // namespace dh::foundation::faery_menu
