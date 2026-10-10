#pragma once

#include "../../hud_geometry.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace dh::foundation::generic_skills {

// One original dqhud_droid shape, already flattened through its display list.
// Coordinates are sprite-local pixels (twips / 20) of the button sprite; the
// composer fits the whole family (base, grey, cooldown) with one transform.
// bitmap=true: atlas UVs (MenusGraphics_droid bitmap1); bitmap=false: solid
// fill, drawn with the white texel tinted by rgba (already colour-multiplied).
struct PcGameplayHudArtBatchV1 {
    std::uint32_t source_shape_id{};
    bool bitmap{};
    std::array<float, 4> rgba{1.0f, 1.0f, 1.0f, 1.0f};
    std::vector<HudGeometryVertex> triangles;
};

// Button rings/fills: btn_skill sprite353, btn_spell sprite398, btn_potion sprite121.
// Icon holders (btimg/btframe), Grey, hitzone, CoolDown and glows are excluded.
const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_skill_base_v1() noexcept;
const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_spell_base_v1() noexcept;
const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_potion_base_v1() noexcept;

// Grey (unassigned/empty) overlay: sprite249 -> shape248 in the skill button space.
const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_grey_overlay_v1() noexcept;

// CoolDown sprite350 frame f (f = source GotoFrame value, 0..99). Frame 0 is
// empty (ready). Frame f >= 1 is the wedge shape 249+f. Source FastUpdate sets
// f = clamp((int)(remaining*100) - 1, 0, 99), remaining = 1 - elapsed/total.
const std::vector<PcGameplayHudArtBatchV1>& original_pc_gameplay_hud_cooldown_frame_v1(std::size_t frame) noexcept;

} // namespace dh::foundation::generic_skills
