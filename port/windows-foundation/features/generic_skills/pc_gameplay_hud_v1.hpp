#pragma once

#include "pc_cooldown_frame_v1.hpp"
#include "pc_skill_hud_projection_v1.hpp"
#include "../frontend/art/original_art.hpp"
#include "../platform_input/semantic_input.hpp"
#include "pc_gameplay_hud_source_art_v1.hpp"

#include <array>
#include <optional>
#include <string>

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

struct PcGameplayHudPresentationV1 {
    frontend::art::ScreenArt art;
    // Stable five-control ordering: PC skills1/2/3, Faery key4, potion key5.
    std::array<platform_input::Hit, 5> actions{};
};

// Builds source-icon triangles, explicit PC circle/ring backing, numeric key
// legends and matching circular pointer geometry. On failure, output is intact.
bool compose_pc_gameplay_hud_v1(const PcSkillHudFrameV1&,
                                unsigned source_class_frame,
                                const PcGameplayHudLayoutV1&,
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
