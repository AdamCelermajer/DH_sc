// P15 FAERYSOUND: original Celest/Hotty OnPreSkill_ sound labels per target count.
#include "../faery_cast_sound_v1.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using dh::foundation::faery_menu::faery_pre_sound_labels_v1;

namespace {
int failures = 0;

void check(bool ok, const char* what) {
    if (!ok) {
        ++failures;
        std::printf("FAIL %s\n", what);
    }
}

bool same(const std::vector<std::string>& got, const std::vector<std::string>& want) {
    return got == want;
}
} // namespace

int main() {
    // Celest, empty Pre list: medium tier plus StaticBallKilled (faerie_celest.luac OnPreSkill_).
    check(same(faery_pre_sound_labels_v1(false, 0),
               {"sfx_spell_lightning_medium", "StaticBallKilled"}),
          "celest 0 targets plays medium and StaticBallKilled");
    check(same(faery_pre_sound_labels_v1(false, 1), {"sfx_spell_lightning_small"}),
          "celest 1 target plays small only");
    check(same(faery_pre_sound_labels_v1(false, 2), {"sfx_spell_lightning_small"}),
          "celest 2 targets plays small only");
    check(same(faery_pre_sound_labels_v1(false, 3), {"sfx_spell_lightning_medium"}),
          "celest 3 targets plays medium");
    check(same(faery_pre_sound_labels_v1(false, 5), {"sfx_spell_lightning_medium"}),
          "celest 5 targets plays medium");
    check(same(faery_pre_sound_labels_v1(false, 6), {"sfx_spell_lightning_big"}),
          "celest 6 targets plays big");

    // Hotty (faerie_hotty.luac OnPreSkill_): empty list is the mage staff fire only.
    check(same(faery_pre_sound_labels_v1(true, 0), {"sfx_mage_staff_elemental_fire"}),
          "hotty 0 targets plays mage staff fire only");
    check(same(faery_pre_sound_labels_v1(true, 1), {"sfx_spell_fire_small"}),
          "hotty 1 target plays fire small");
    check(same(faery_pre_sound_labels_v1(true, 4), {"sfx_spell_fire_medium"}),
          "hotty 4 targets plays fire medium");
    check(same(faery_pre_sound_labels_v1(true, 6), {"sfx_spell_fire_big"}),
          "hotty 6 targets plays fire big");

    // Hit-only labels must never be produced for a Pre call.
    for (std::size_t n = 0; n < 10; ++n) {
        for (const bool hotty : {false, true}) {
            const auto labels = faery_pre_sound_labels_v1(hotty, n);
            check(!labels.empty(), "every cast has a Pre sound label");
            if (!hotty && n > 0) check(labels.size() == 1, "celest with targets plays one tier label");
        }
    }

    if (failures == 0) std::printf("PASS faery_cast_sound_v1 (labels per target count)\n");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
