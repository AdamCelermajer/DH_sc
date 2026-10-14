#pragma once

#include "pc_cooldown_frame_v1.hpp"
#include "pc_skill_hud_projection_v1.hpp"
#include "../frontend/art/original_art.hpp"
#include "../platform_input/semantic_input.hpp"
#include "pc_gameplay_hud_source_art_v1.hpp"
#include "../hud_panels/touch_hud_art_v1.hpp"

#include <array>
#include <optional>
#include <string>
#include <vector>

namespace dh::foundation::generic_skills {

// Caller supplies the placement in the frontend's authored 480x320 stage.
// Button rings, grey (empty) overlays and CoolDown wedges are the original
// dqhud_droid button art (HUDBTN); the icons are original SkillIcon/Faery/potion
// artwork. The numeric key legends (1-5) and the hit circles are an explicit PC
// adaptation. No Android HUD placement is inferred here.
struct PcGameplayHudCirclePlacementV1 {
    float center_x{};
    float center_y{};
    float radius{};
    std::array<float, 4> key_label_bounds{}; // xmin,xmax,ymin,ymax, stage pixels
};

struct PcGameplayHudLayoutV1 {
    // Physical left/middle/right. The source slot identity is checked against
    // the projected PC frame, never encoded into the hit result.
    std::array<PcGameplayHudCirclePlacementV1, 3> skills{};
    PcGameplayHudCirclePlacementV1 faery{};
    PcGameplayHudCirclePlacementV1 potion{};
    // P16 SPACEBTN / HUDART: bottom-right action button = original btn_interact movie (hud_panels). Drawn at its
    // authored stage position. action_icon is the btimg frame (0 Chest .. 7 Inspect; out of range -> Attack);
    // action_pressed selects the pressed ring (Space held). Off unless action_enabled.
    bool action_enabled = false;
    std::int32_t action_icon = 5;
    bool action_pressed = false;
    // Source btn_spell CoolDown frame (0 ready .. 99 full), from the Faery
    // spell timer (SetSpellCooldown). Potions have no source cooldown.
    std::int32_t faery_cooldown_frame = 0;
    float key_label_height = 13.0f;
    // Exact NativeHUDGetActiveFaery result from the same current frame.
    // Source btn_spell.onPush passes active_id+1 to one-based gotoAndStop, so
    // nonnegative IDs select that zero-based sprite397 frame. -1 requests
    // invalid frame0 and leaves the freshly placed frame0 icon in place.
    // Missing means the caller has no source value and adds no icon.
    std::optional<std::int32_t> active_faery_id;
    // Set only from the same CharacterState/actor used to project the skill
    // frame. The count is optional so unknown inventory never becomes zero.
    struct PotionCount {
        std::string character_state_id;
        ActorId source_actor = invalid_actor_id;
        std::uint64_t quantity = 0;
    };
    std::optional<PotionCount> potion_count;
};

// v21.8 w4-hud: the ORIGINAL mobile (touch) layout, selectable next to the desktop layout above (--hud-layout
// original-touch). Geometry is not caller-supplied: every control sits at its authored dqhud_droid menu_HUD_2
// placement (hud_panels/touch_hud_art_v1: fixed skill1/2/3 + faery + attack on the right, joystick on the left, which is
// NOT drawn). The same button art/behaviours as the desktop layout are reused; there are no key legends. Only the
// dynamic state and the screen-edge anchor are supplied here.
struct PcTouchGameplayHudLayoutV1 {
    // Added to the x of every right-cluster element (skills, faery, attack, potion, music). The authored stage is
    // stretched across the whole screen by the original; here circles stay round and the cluster is anchored to the
    // right screen edge instead: shift = window stage width - 480 (0 for the authored 480x320 stage).
    float stage_shift_x = 0.0f;
    bool action_pressed = false;          // pointer/Space held: pressed ring of btn_interact
    std::int32_t action_icon = 5;         // btimg frame (0 Chest .. 7 Inspect; out of range -> Attack)
    std::int32_t faery_cooldown_frame = 0;
    std::optional<std::int32_t> active_faery_id; // as PcGameplayHudLayoutV1
    std::optional<PcGameplayHudLayoutV1::PotionCount> potion_count; // drawn in the authored cnt badge (number only)
};

// An additional pointer receiver (original-touch: the attack button and the music note).
struct PcGameplayHudExtraHitV1 {
    platform_input::Hit hit{};
    frontend::art::HitRegion region;
};

struct PcGameplayHudPresentationV1 {
    frontend::art::ScreenArt art;
    // Stable five-control ordering: PC skills1/2/3, Faery key4, potion key5.
    std::array<platform_input::Hit, 5> actions{};
    // original-touch only (empty for the desktop layout): attack, music. Tested before the five above.
    std::vector<PcGameplayHudExtraHitV1> extra_hits;
    // original-touch only: the authored Joystick.stick hit zone (x0,y0,x1,y1, left cluster, unshifted). It is NOT drawn
    // and NOT hit-tested: this is the seam where a mobile build adds the movement joystick (platform_input::Surface::joystick).
    std::optional<std::array<float, 4>> joystick_zone;
};

// Builds source-icon triangles, explicit PC circle/ring backing, numeric key
// legends and matching circular pointer geometry. On failure, output is intact.
bool compose_pc_gameplay_hud_v1(const PcSkillHudFrameV1&,
                                unsigned source_class_frame,
                                const PcGameplayHudLayoutV1&,
                                PcGameplayHudPresentationV1&,
                                std::string& error);

// Original-touch composition (see PcTouchGameplayHudLayoutV1). Hit regions 0..4 are the same five controls as the desktop
// layout (skill1/2/3 = physical left/middle/right = source slots 2,0,1 = authored btn_skill3, btn_skill1, btn_skill2,
// faery, potion); extra_hits carry the attack button and the music note. On failure, output is intact.
bool compose_pc_touch_gameplay_hud_v1(const PcSkillHudFrameV1&,
                                      unsigned source_class_frame,
                                      const PcTouchGameplayHudLayoutV1&,
                                      PcGameplayHudPresentationV1&,
                                      std::string& error);

// Input is in the same 480x320 authored stage as the packet. Physical hits
// return PC semantic controls/keys; source slot permutation happens later in
// pc_skill_number_to_source_slot_v1, exactly once.
bool pc_gameplay_hud_hit_test_v1(const PcGameplayHudPresentationV1&,
                                 float stage_x, float stage_y,
                                 platform_input::Hit&,
                                 std::string& error);

} // namespace dh::foundation::generic_skills
